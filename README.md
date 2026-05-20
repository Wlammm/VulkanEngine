# Custom C++ Game Engine

A custom C++ game engine written using the Vulkan Rendering API, focused on modern rendering techniques, performance, and custom tooling.

---

## Rendering Features

- Bindless textures
- Indirect rendering with count buffers *(no CPU readback required)*
- Ray-traced shadows
- Automatic shader resource binding
- Global vertex and index buffers
  - Includes defragmentation support

---

## Engine Features

### Reflection & Serialization System

A custom reflection system implemented as a pre-build step that traverses the Abstract Syntax Tree (AST) to generate metadata.

Used for:
- Binary serialization of arbitrary engine types
- Runtime type information
- Editor

---

### Custom Editor

Built using ImGui with support for:
- Property inspection using ReflectionSystem
- Asset management
- Runtime debugging tools
- Editor utilities

---

### Memory & Utility Systems

- Custom forward-declarable `UniquePtr` implementation
- Custom `Delegate` and `MulticastDelegate` implementations
- Coroutine support
- Asynchronous asset loading system

---

### Physics
- PhysX integration

---

## Technical Highlights

- GPU-driven rendering pipeline
- Async resource management
- Reflection-powered serialization pipeline
- Real-time editor integration
- Custom engine infrastructure and tooling
