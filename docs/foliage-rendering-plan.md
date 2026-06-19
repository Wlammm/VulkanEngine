# Foliage Rendering — Design Plan

Status: **In progress** (Phase 1)
Owner: rendering
Last updated: 2026-06-19

## Goal

Add GPU-driven foliage rendering to the engine:

- Support multiple **foliage types** (grass, bushes, trees, …), each with its own
  meshes, materials and placement rules.
- **Paintable** onto the landscape from the editor, with adjustable **density** per
  type.
- **Scalable**: max draw distance and density both tunable, and density that
  thins with distance — all driven at runtime with no rebuild.
- Rendering is **fully GPU-driven** (compute culling → indirect draw), reusing the
  engine's existing indirect pipeline.

This design was chosen with two explicit decisions:

1. **Density-map driven** placement (not explicit per-instance lists). Painting
   edits per-chunk density textures; instances are generated on the GPU.
2. **Lean GPU-instanced + LOD** for the first shippable version. Occlusion
   culling, billboard imposters, wind and streaming come later.

## How it maps onto existing systems

The engine already has the hard parts:

- **GPU-driven indirect rendering** — `IndirectCullPass`
  (`Engine/Engine/Rendering/RenderingPasses/ComputePasses/IndirectCullPass.cpp`)
  runs a compute shader that builds `VkDrawIndexedIndirectCommand` + `PerDrawData`
  into buffers; `MainPass` consumes them via `drawIndexedIndirectCount` with a
  count buffer, organized into "shading bins". **Foliage is a new set of bins with
  its own cull pass** — same proven pattern.
- **Terrain** — `LandscapeRenderComponent` owns a `Heightfield` with
  `GetHeight(vec2)` (Perlin-based). `chunkSize = 64`, `chunkScale = 20000`.
  Painting raycasts the heightfield; scatter samples it for placement height.
- **Bindless textures + `.mat` materials** — foliage types reference albedo/normal/
  material bindless indices, no per-draw descriptor churn.
- **Reflection-based editor** — `EditorWindow` / `EditorSystem` auto-discovered via
  reflection; `Viewport` exposes normalized mouse position; `ImGuiPropertyDrawer`
  draws asset fields automatically. No brush/painting tool exists yet (net-new).
- **Reflection-based binary serialization** via `META(SerializeField)` — placement
  density maps persist for free.

### Key constraint

Scene meshes flow through `GPUSceneSystem::AddMeshInstance` as individual
`MeshInstanceData` (full `mat4` each). Foliage must **not** reuse that path — a
million grass blades would blow it up. Foliage gets its **own instance pool, own
count buffer and own indirect buffer**, registered with `GPUResourceManager`,
completely parallel to the scene path.

## Data model

### `FoliageType` (asset, `.foliage`)

Describes one kind of foliage:

- Mesh LODs (`List<SharedPtr<Model>>`, LOD0…LODn).
- Material per LOD (bindless albedo/normal/material indices).
- Base density (instances per m²), random scale range, random yaw,
  align-to-normal toggle.
- Scalability: `maxDrawDistance`, `densityFalloffStart`, LOD distance thresholds.
- (Later) wind params, collision flags, billboard atlas for far LOD.

### `FoliageInstanceData` (GPU struct, 16-byte aligned)

Kept deliberately small so millions fit. The full `mat4` is **reconstructed on the
GPU** during culling, never stored.

```
float3 position;      // world space
uint   scalePacked;   // packed scale (half2 / quantized)
uint   rotationPacked;// quantized yaw (+ pitch/roll later)
uint   typeIndex;     // index into the FoliageType table
```

~16–24 bytes vs. a full `mat4` `MeshInstanceData`. This is the single most
important memory decision.

### Persistence — density maps

Each landscape chunk owns one **R8 density texture per foliage type** (e.g.
128×128 over a `chunkScale` chunk). The texture **is** the saved foliage data:
compact, resolution-independent, serialized as a blob via `META(SerializeField)`.
No per-instance lists in v1.

## Runtime pipeline (GPU)

New passes, parallel to the existing scene cull/draw:

1. **Scatter compute** — for each resident chunk, one thread per density-map texel:
   read density, derive instance count via a stable hash (blue-noise jitter),
   sample the `Heightfield` for Y + surface normal, append `FoliageInstanceData`
   into the persistent per-chunk instance pool. Re-runs only when a chunk's density
   map changes or it becomes resident.
2. **`FoliageCullPass` (compute)** — mirrors `IndirectCullPass`. One thread per
   instance:
   - reconstruct world matrix + bounding sphere,
   - frustum cull,
   - distance cull vs `maxDrawDistance` (× `globalMaxDistanceScale`),
   - hash-based **density fade** past `densityFalloffStart`,
   - **LOD select** by distance,
   - `InterlockedAdd` into per-(type, LOD) count slot, emit
     `VkDrawIndexedIndirectCommand`.
3. **`FoliagePass` (graphics)** — `drawIndexedIndirectCount` per (type, LOD) bin.
   VS reads packed instance data via draw index, PS samples bindless material with
   alpha clip + depth write.

## Editor authoring (painting)

- `FoliageEditorSystem` (extends `EditorSystem`) + `FoliagePaintWindow`
  (`EditorWindow`).
- Brush: raycast viewport mouse (`Viewport::GetNormalizedMousePositionInViewport`)
  → heightfield hit → world point. Brush has radius, falloff, strength, active type.
- Paint / erase / single-place modes; LMB paints, Shift+LMB erases into the active
  chunk's density texture; debounced re-scatter of touched chunks.
- Brush preview ring via `DebugPass`.
- `FoliageType` fields drawn automatically by `ImGuiPropertyDrawer`.

## Scalability (built in from v1)

One `FoliageScalabilitySettings` cbuffer, consumed entirely in `FoliageCullPass`:

- `globalMaxDistanceScale` — multiplies every type's max distance (quality slider).
- `globalDensityScale` — 0..1 multiplier on accepted instances (hash threshold).
- `lodBias` — pushes LOD transitions nearer/farther.

Per-type overrides live on the `FoliageType` asset. Because culling/LOD/density are
GPU per-frame decisions, scalability is runtime sliders with zero rebuild.

## Rendering techniques

In v1:

- GPU instanced indirect (baseline; reuses existing primitives).
- Mesh LODs.
- Distance-based density fade.
- Chunk-level pre-cull (skip off-screen / over-distance chunks entirely).

Deferred (in priority order):

- Vertex wind animation (per-vertex sway, vertex-color gradient mask).
- Hi-Z / GPU occlusion culling (depth pyramid from `PrePass` depth).
- Billboard / octahedral imposters for the far band.
- Chunk streaming / residency for large worlds.
- (Far future) mesh-shader meshlet emission.

## Implementation phases

1. **Foundations** — `FoliageType` asset + packed `FoliageInstanceData` pool (new
   buffer registered with `GPUResourceManager`) + minimal `FoliagePass` with
   hardcoded instances. *Proves the new bin + data path.* ← current
2. **Scatter** — scatter compute from a hardcoded density map + heightfield
   sampling. *Proves density → instances.*
3. **Culling** — `FoliageCullPass` (frustum + distance + LOD + density fade),
   per-(type, LOD) indirect bins, `drawIndexedIndirectCount`.
4. **Editor painting** — brush into density maps + chunk re-scatter.
5. **Scalability + residency** — global sliders, chunk pre-cull, distance residency.

Later: wind → Hi-Z occlusion → imposters → streaming.

## Open notes for implementers

- Add foliage as **new bins with their own count + indirect buffers** registered
  with `GPUResourceManager`; never touch `GPUSceneSystem`.
- The heightfield is currently Perlin-procedural. Sampling normals in the scatter
  shader means either re-evaluating noise on the GPU or **baking a height/normal
  texture per chunk**. Baking is the cleaner long-term path and lets painted terrain
  edits work later.
