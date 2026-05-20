A custom C++ game engine written using the Vulkan Rendering API.


Rendering Features:
Bindless textures
Indirect rendering with count buffer (No readback to CPU required)
Raytraced shadows
Automatic shader resource binding
Global Vertex & Index buffers with defragmentation support

Other Features:
Reflection system as a pre-build step that traverses the abstract syntax tree's (AST) and provides reflection information used to binarily serialize any type in the engine, and for editor purposes
Custom editor using ImGui
Custom forward declarable UniquePtr implementation
Coroutine support
Async asset loading system
PhysX implementation
Custom Delegate and MulticastDelegate implementations

