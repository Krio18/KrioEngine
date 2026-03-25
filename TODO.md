# KrioEngine - Development Roadmap

**Architecture:** Manager-based with Service Locator pattern
**Current Version:** 0.1.0-alpha
**Last Updated:** 2026-03-25

---

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Phase 0: Core Architecture](#phase-0-core-architecture)
3. [Phase 1: Minimal Rendering](#phase-1-minimal-rendering)
4. [Phase 2: Manager Infrastructure](#phase-2-manager-infrastructure)
5. [Phase 3: Scene & ECS](#phase-3-scene--ecs)
6. [Phase 4: Asset Pipeline](#phase-4-asset-pipeline)
7. [Phase 5: Advanced Systems](#phase-5-advanced-systems)
8. [Phase 6: Editor & Tools](#phase-6-editor--tools)
9. [Phase 7: Build & Distribution](#phase-7-build--distribution)
10. [Phase 8: Scripting](#phase-8-scripting-optional)
11. [Phase 9: Networking](#phase-9-networking-optional)
12. [Phase 10: Optimization & Polish](#phase-10-optimization--polish)
13. [Testing Strategy](#testing-strategy)
14. [Technical Decisions](#technical-decisions)
15. [Priority Order](#priority-order-next-steps)

---

## Architecture Overview

### Design Principles
- **Manager-based architecture** - Each major system has a dedicated Manager class responsible for its lifecycle and coordination
- **Service Locator pattern** - Provides global access to managers without tight coupling between systems
- **ECS for game objects** - EnTT library handles entity-component relationships for optimal cache performance
- **Data-driven design** - Configuration, assets, and scenes loaded from external files rather than hardcoded

### Target Feature Set (Unity-like Engine)
- GameObject/Entity system with reusable components
- Scene management with parent-child hierarchy
- Unified asset management with automatic reference counting and hot-reload
- Input abstraction with remappable action bindings
- Flexible camera system supporting multiple simultaneous cameras
- Material/Shader pipeline with instancing support
- Integrated physics simulation (Jolt Physics)
- Spatial 3D audio system (OpenAL)
- Visual scene editor (ImGui-based)

### Manager Initialization Order (Critical!)
**Dependencies must be initialized in this exact order to avoid null references:**

1. **Core Layer** (no dependencies)
   - TimeManager
   - InputManager

2. **Rendering Layer** (depends on Core)
   - ShaderManager
   - MaterialManager
   - MeshManager
   - CameraManager
   - RenderManager

3. **Scene Layer** (depends on Rendering)
   - SceneManager

4. **Simulation Layer** (depends on Scene)
   - PhysicsManager
   - AudioManager

5. **Asset Layer** (depends on all above)
   - AssetManager

---

## Phase 0: Core Architecture

**Goal:** Establish the foundational manager infrastructure that all other systems depend on

### 0.1 Service Locator
**Purpose:** Centralized registry for accessing managers globally without singletons

- [x] Create `src/Core/ServiceLocator.hpp`
- [x] Template-based registration: allow any manager type to be registered
- [x] Template-based retrieval: type-safe access to registered managers
- [x] Thread-safe access using mutex or atomic operations
- [x] Null service pattern: return safe null object when service not found (prevents crashes)
- [x] Shutdown method: cleanly destroy all registered services in reverse order
- [x] Debug mode: track which services are registered
**Why this matters:** Avoids global singletons while still providing convenient access. Makes testing easier since you can swap managers.

### 0.2 Time Management
**Purpose:** Unified time source for all game systems to ensure synchronization

- [x] Create `src/Core/TimeManager.hpp` and `.cpp`
- [x] Track deltaTime: frame duration affected by timeScale
- [x] Track unscaledDeltaTime: real frame duration ignoring timeScale
- [x] Track total elapsed time since engine start
- [x] Implement timeScale: allows slow-motion (0.5), pause (0.0), fast-forward (2.0)
- [x] Track frame count for debugging and profiling
- [x] Calculate and expose current FPS
- [x] Smoothed deltaTime to avoid spikes from single slow frames
- [x] Maximum deltaTime cap to prevent huge jumps (e.g., when debugging paused)

**Why this matters:** Physics, animation, and gameplay logic all need consistent time values. TimeScale enables bullet-time effects.

### 0.3 Refactor Application Class
**Purpose:** Convert from monolithic Application to manager-orchestration pattern

- [x] Remove singleton pattern from `Application`
- [x] Create `initializeManagers()` method that registers all managers with ServiceLocator
- [x] Create `shutdownManagers()` method that destroys managers in reverse order
- [x] Refactor main loop to call managers in correct sequence:
  - Update TimeManager first
  - Poll input via InputManager
  - Fixed timestep physics update via PhysicsManager
  - Variable timestep scene update via SceneManager
  - Render via RenderManager
- [x] Add exception handling around manager calls
- [x] Log manager initialization/shutdown for debugging

**Why this matters:** Clear separation of concerns. Each manager owns its domain. Application just orchestrates.

### 0.4 Event System
**Purpose:** Decouple systems via publish-subscribe messaging instead of direct calls

- [x] Create `src/Core/Event.hpp` base class
- [x] Define concrete event types:
  - WindowResizeEvent (width, height)
  - WindowCloseEvent
  - KeyPressedEvent (keycode, repeat)
  - KeyReleasedEvent (keycode)
  - MouseMovedEvent (x, y, deltaX, deltaY)
  - MouseButtonPressedEvent (button)
  - MouseButtonReleasedEvent (button)
  - MouseScrolledEvent (offsetX, offsetY)
- [x] Create `src/Core/EventBus.hpp`
- [x] Implement subscribe: register callbacks for specific event types
- [x] Implement publish: broadcast events to all subscribers
- [x] Support lambda callbacks for convenience
- [x] Support member function callbacks for class methods
- [x] Automatic unsubscribe when subscriber is destroyed (use weak_ptr)
- [x] Event queuing: queue events and dispatch at safe point in frame

**Why this matters:** WindowResize shouldn't directly call RenderManager. Publish event, RenderManager subscribes. Loose coupling.

---

## Phase 1: Minimal Rendering

**Goal:** Get visible output on screen as quickly as possible to validate the rendering pipeline

### 1.1 Shader System Foundation
**Purpose:** Load, compile, and manage shader programs that run on GPU

- [ ] Create `src/Renderer/Shader.hpp` and `.cpp`
- [ ] Wrap bgfx shader handle for automatic cleanup (RAII)
- [ ] Load vertex shader from file
- [ ] Load fragment shader from file
- [ ] Link vertex + fragment into shader program
- [ ] Create simple shaders in `shaders/` directory:
  - `v_simple.sc` - Passthrough vertex shader with MVP transformation
  - `f_simple.sc` - Solid color output (hardcoded or uniform)
- [ ] Use bgfx's shaderc tool to compile to platform-specific bytecode
- [ ] Cache compiled shaders to avoid recompilation
- [ ] Error handling for shader compilation failures

**Why this matters:** Can't render without shaders. Start simple, expand later.

### 1.2 Mesh Primitives
**Purpose:** Define geometry data that can be sent to GPU

- [ ] Create `src/Renderer/Mesh.hpp` and `.cpp`
- [ ] Define vertex format structure: Position (vec3), Normal (vec3), UV (vec2), Color (uint32)
- [ ] Wrap bgfx vertex buffer handle
- [ ] Wrap bgfx index buffer handle
- [ ] Implement static factory: `createTriangle()` - returns hardcoded triangle vertices
- [ ] Implement static factory: `createQuad()` - returns two triangles forming rectangle
- [ ] Implement static factory: `createCube()` - returns 36 vertices (6 faces × 2 triangles × 3 verts)
- [ ] Implement `draw()` method to submit mesh to bgfx
- [ ] Calculate and store bounding box for culling (later)

**Why this matters:** Need geometry to see anything. Primitives are easiest to verify rendering works.

### 1.3 First Triangle Milestone
**Purpose:** Validate entire rendering pipeline with simplest possible output

- [ ] In `Application::run()` create a triangle mesh using `Mesh::createTriangle()`
- [ ] Load simple shader program
- [ ] Submit draw call with identity matrices (no transformation)
- [ ] Set clear color to dark gray so triangle is visible
- [ ] **Success criteria:** Colored triangle visible in window center
- [ ] Debug: if nothing visible, verify vertex winding order (CCW vs CW)
- [ ] Debug: if nothing visible, check viewport size matches window

**Why this matters:** If triangle renders, entire pipeline works: window → bgfx init → shader compile → vertex upload → draw submission.

### 1.4 Transform Matrices (MVP Pipeline)
**Purpose:** Enable 3D positioning and camera control via matrix math

- [ ] Add GLM library to `vcpkg.json` for matrix math
- [ ] Create `src/Math/Transform.hpp` utility
- [ ] Implement Model matrix creation from position, rotation (quaternion), scale
- [ ] Implement View matrix creation from camera position and look direction
- [ ] Implement Projection matrix for perspective (FOV, aspect, near, far)
- [ ] Implement Projection matrix for orthographic (left, right, bottom, top, near, far)
- [ ] Create uniform buffer to pass MVP matrix to shaders
- [ ] Update vertex shader to multiply position by MVP
- [ ] Test with rotating cube: create cube, apply rotation each frame, submit with updated Model matrix
- [ ] **Success criteria:** Spinning cube visible on screen

**Why this matters:** 3D rendering requires transforming vertices from object space → world space → view space → clip space.

---

## Phase 2: Manager Infrastructure

**Goal:** Build the manager layer for all rendering and input subsystems

### 2.1 ShaderManager
**Purpose:** Centralized loading, caching, and hot-reload of shaders

- [ ] Create `src/Renderer/ShaderManager.hpp` and `.cpp`
- [ ] Maintain map of shader name → Shader object
- [ ] Implement `load(name)`: load shader from disk, compile, cache, return handle
- [ ] Implement `get(handle)`: retrieve cached shader by handle
- [ ] Implement `reload(handle)`: recompile shader from disk (for hot-reload)
- [ ] Handle compilation errors gracefully: log error, return fallback shader
- [ ] Track shader usage count for debugging
- [ ] Implement shader variants: same shader with different #defines (e.g., WITH_SHADOWS)

**Why this matters:** Avoids duplicate shader loads. Hot-reload enables edit-while-running workflow.

### 2.2 MaterialManager
**Purpose:** Manage material instances that reference shaders and define rendering properties

- [ ] Create `src/Renderer/Material.hpp` and `.cpp`
- [ ] Material stores: ShaderHandle reference, uniform values (color, metallic, roughness), texture slots
- [ ] Create `src/Renderer/MaterialManager.hpp` and `.cpp`
- [ ] Implement `create(shaderHandle)`: create new material with given shader
- [ ] Implement `get(materialHandle)`: retrieve material by handle
- [ ] Implement `instantiate(baseHandle)`: clone material for per-object customization
- [ ] Track base material → instance relationship for efficient updates
- [ ] Implement material sorting: group by shader to minimize state changes

**Why this matters:** Material instancing allows shared shader but unique colors/textures per object.

### 2.3 MeshManager (Resource Management)
**Purpose:** Load, cache, and manage mesh data with reference counting

- [ ] Create `src/Renderer/MeshManager.hpp` and `.cpp`
- [ ] Maintain map of mesh path → Mesh object
- [ ] Implement reference counting: increment on load, decrement on unload
- [ ] Implement `load(path)`: load OBJ file, parse vertices/indices, cache, return handle
- [ ] Implement `get(handle)`: retrieve cached mesh
- [ ] Implement `unload(handle)`: decrement ref count, destroy if zero
- [ ] Support loading from primitive names: "triangle", "cube", "sphere"
- [ ] Calculate mesh bounds (AABB) for frustum culling
- [ ] Async loading: load meshes on background thread (optional for now)

**Why this matters:** Multiple entities can share same mesh. Reference counting prevents memory leaks.

### 2.4 RenderManager
**Purpose:** High-level rendering coordinator that collects draw calls and submits to bgfx

- [ ] Create `src/Renderer/RenderManager.hpp` and `.cpp`
- [ ] Maintain list of submitted meshes for current frame
- [ ] Implement `submitMesh(mesh, material, transform)`: add to render queue
- [ ] Implement `submitCamera(camera)`: set active camera for this frame
- [ ] Implement `render()`: process queue and submit to bgfx
- [ ] Sort opaque meshes front-to-back (early depth rejection optimization)
- [ ] Sort transparent meshes back-to-front (correct alpha blending)
- [ ] Clear render queue after each frame
- [ ] Support multiple render passes: opaque, transparent, post-process
- [ ] Track draw call count and triangle count for profiling

**Why this matters:** Decouples "what to render" from "how to render". Enables optimizations like sorting.

### 2.5 CameraManager
**Purpose:** Manage multiple cameras and determine which camera renders to which viewport

- [ ] Create `src/Renderer/CameraManager.hpp` and `.cpp`
- [ ] Maintain list of active cameras in current scene
- [ ] Implement `registerCamera(camera)`: add camera to active list
- [ ] Implement `unregisterCamera(camera)`: remove camera from active list
- [ ] Implement `setMainCamera(camera)`: designate primary camera
- [ ] Implement `getMainCamera()`: retrieve primary camera
- [ ] Implement `getAllCameras()`: retrieve all cameras for multi-viewport rendering
- [ ] Handle camera priority: higher priority cameras render later (overlay UI)
- [ ] Validate camera settings: ensure valid FOV, aspect ratio

**Why this matters:** Supports split-screen, mini-map, render-to-texture. Main camera is most common case.

### 2.6 Camera Component
**Purpose:** Data container for camera projection and viewport settings

- [ ] Create `src/Renderer/Camera.hpp` and `.cpp`
- [ ] Store projection type: Perspective or Orthographic
- [ ] Store perspective params: FOV, aspect ratio, near plane, far plane
- [ ] Store orthographic params: left, right, bottom, top, near, far
- [ ] Store viewport rectangle: x, y, width, height (normalized 0-1 or pixel coordinates)
- [ ] Store clear flags: Skybox, SolidColor, DepthOnly, Nothing
- [ ] Store clear color if using SolidColor mode
- [ ] Store render texture target (optional, for render-to-texture effects)
- [ ] Implement `getViewMatrix()`: calculate from camera's transform component
- [ ] Implement `getProjectionMatrix()`: calculate from stored parameters
- [ ] Implement `getViewProjectionMatrix()`: cached multiplication of View × Projection

**Why this matters:** Camera is just data. Separate from Camera Controller which provides behavior.

### 2.7 Camera Controllers
**Purpose:** Reusable camera movement behaviors for different game styles

#### FPS Camera Controller
- [ ] Create `src/Renderer/Controllers/FPSCameraController.hpp` and `.cpp`
- [ ] WASD for forward/back/strafe movement
- [ ] Mouse X axis for yaw rotation
- [ ] Mouse Y axis for pitch rotation (clamp to avoid gimbal lock)
- [ ] Configurable move speed
- [ ] Configurable mouse sensitivity
- [ ] Optional sprint modifier (Shift key)

#### Orbit Camera Controller
- [ ] Create `src/Renderer/Controllers/OrbitCameraController.hpp` and `.cpp`
- [ ] Set target point to orbit around
- [ ] Mouse drag to rotate around target (spherical coordinates)
- [ ] Mouse scroll to zoom in/out (change orbit distance)
- [ ] Limit minimum and maximum zoom distance
- [ ] Limit pitch angle to avoid flipping

#### Follow Camera Controller
- [ ] Create `src/Renderer/Controllers/FollowCameraController.hpp` and `.cpp`
- [ ] Set target transform to follow
- [ ] Smooth follow using damping (lerp with time)
- [ ] Offset from target (e.g., behind and above player)
- [ ] Optional look-ahead: predict target movement
- [ ] Collision detection: move camera forward if occluded (advanced)

**Why this matters:** Controllers implement behavior. Swap controller = change camera feel. Reusable across projects.

### 2.8 InputManager
**Purpose:** Abstract platform input and provide consistent API across systems

#### Raw Input State
- [ ] Create `src/Platform/InputManager.hpp` and `.cpp`
- [ ] Track current keyboard state: map KeyCode → bool (pressed this frame)
- [ ] Track previous keyboard state for detecting key up/down transitions
- [ ] Implement `isKeyPressed(key)`: true while key held
- [ ] Implement `isKeyDown(key)`: true only on press frame
- [ ] Implement `isKeyUp(key)`: true only on release frame
- [ ] Track current mouse button state
- [ ] Implement `isMouseButtonPressed(button)`
- [ ] Implement `isMouseButtonDown(button)`
- [ ] Implement `isMouseButtonUp(button)`
- [ ] Track mouse position in screen coordinates
- [ ] Implement `getMousePosition()`: current position
- [ ] Implement `getMouseDelta()`: movement since last frame
- [ ] Track mouse scroll offset
- [ ] Implement `getMouseScroll()`: scroll amount this frame

#### Input Action System
- [ ] Create `src/Platform/InputAction.hpp`
- [ ] Define Action: named input binding (e.g., "Jump")
- [ ] Map action name to one or more KeyCodes
- [ ] Support multiple bindings for same action (Space OR Gamepad A for Jump)
- [ ] Implement `registerAction(name, keycode)`: bind action to key
- [ ] Implement `isActionPressed(name)`: check if any bound key is pressed
- [ ] Implement `isActionDown(name)`: check if action triggered this frame
- [ ] Load action mappings from JSON config file
- [ ] Support action modifiers: require Ctrl+S for "Save" action
- [ ] Support axis bindings: map W/S to "MoveForward" axis with +1/-1 values

**Why this matters:** Rebindable controls. Same code works with keyboard, gamepad, or touchscreen if you swap InputManager.

---

## Phase 3: Scene & ECS

**Goal:** Build entity-component system for game object management

### 3.1 EnTT Integration
**Purpose:** Use proven ECS library instead of building from scratch

- [ ] Add EnTT to `vcpkg.json`
- [ ] Create `src/ECS/Registry.hpp` wrapper around `entt::registry`
- [ ] Implement `createEntity()`: create new entity, return Entity wrapper
- [ ] Implement `destroyEntity(entity)`: remove entity and all components
- [ ] Implement `addComponent<T>(entity, args...)`: attach component to entity
- [ ] Implement `getComponent<T>(entity)`: retrieve component reference
- [ ] Implement `hasComponent<T>(entity)`: check if entity has component
- [ ] Implement `removeComponent<T>(entity)`: detach component from entity
- [ ] Implement `view<Components...>()`: iterate entities with specific components
- [ ] Implement `each<Components...>(callback)`: execute callback on matching entities

**Why this matters:** EnTT is battle-tested, cache-friendly, and fast. Don't reinvent ECS.

### 3.2 Entity Wrapper
**Purpose:** Friendly API on top of EnTT's raw entity ID

- [ ] Create `src/ECS/Entity.hpp`
- [ ] Store entity handle (entt::entity)
- [ ] Store registry pointer for convenience
- [ ] Implement `addComponent<T>(args...)`: forward to registry
- [ ] Implement `getComponent<T>()`: forward to registry
- [ ] Implement `hasComponent<T>()`: forward to registry
- [ ] Implement `removeComponent<T>()`: forward to registry
- [ ] Implement `destroy()`: remove entity from registry
- [ ] Implement `isValid()`: check if entity still exists
- [ ] Overload equality operators for entity comparison

**Why this matters:** Users call `entity.addComponent<Transform>()` instead of `registry.emplace<Transform>(entityID)`. Cleaner API.

### 3.3 Core Components

#### Transform Component
- [ ] Create `src/ECS/Components/Transform.hpp`
- [ ] Store position as vec3
- [ ] Store rotation as quaternion (avoids gimbal lock)
- [ ] Store scale as vec3
- [ ] Implement `getModelMatrix()`: calculate TRS matrix (Translation × Rotation × Scale)
- [ ] Implement `translate(offset)`: move by offset
- [ ] Implement `rotate(axis, angle)`: rotate around axis
- [ ] Implement `lookAt(target)`: point toward target position
- [ ] Store cached model matrix, dirty flag to avoid recalculation

#### MeshRenderer Component
- [ ] Create `src/ECS/Components/MeshRenderer.hpp`
- [ ] Store MeshHandle reference
- [ ] Store MaterialHandle reference
- [ ] Store castShadows boolean flag
- [ ] Store receiveShadows boolean flag
- [ ] Store rendering layer mask (for selective rendering)
- [ ] Store bounds (AABB) for frustum culling

#### Camera Component
- [ ] Create `src/ECS/Components/Camera.hpp`
- [ ] Reuse Camera class from Phase 2.6 as component
- [ ] Entity with Camera component is a camera in the scene
- [ ] Entity must also have Transform component for positioning

#### Tag Component
- [ ] Create `src/ECS/Components/Tag.hpp`
- [ ] Store entity name as string
- [ ] Store enabled boolean (entities can be disabled without destroying)
- [ ] Store layer (physics, rendering layers)
- [ ] Store tag (for grouping: "Player", "Enemy", "Pickup")

**Why this matters:** Components are pure data. Systems operate on components. This is the core of ECS.

### 3.4 Systems

#### RenderSystem
- [ ] Create `src/ECS/Systems/RenderSystem.hpp` and `.cpp`
- [ ] Query all entities with Transform + MeshRenderer components
- [ ] For each entity:
  - Retrieve mesh from MeshManager using MeshHandle
  - Retrieve material from MaterialManager using MaterialHandle
  - Calculate model matrix from Transform
  - Submit to RenderManager
- [ ] Skip disabled entities (Tag component enabled = false)
- [ ] Skip entities outside camera frustum (frustum culling)
- [ ] Execute during scene render phase

#### CameraSystem
- [ ] Create `src/ECS/Systems/CameraSystem.hpp` and `.cpp`
- [ ] Query all entities with Transform + Camera components
- [ ] For each camera entity:
  - Update Camera's view matrix using Transform position/rotation
  - Register camera with CameraManager
- [ ] Unregister cameras when entities destroyed
- [ ] Handle main camera switching (check Tag for "MainCamera")

**Why this matters:** Systems provide behavior. Adding RenderSystem makes entities with MeshRenderer automatically render.

### 3.5 Scene
**Purpose:** Container for entities and scene-level data

- [ ] Create `src/Scene/Scene.hpp` and `.cpp`
- [ ] Store ECS Registry instance
- [ ] Store scene name
- [ ] Implement `createEntity(name)`: create entity, add Tag component with name
- [ ] Implement `destroyEntity(entity)`: remove from registry
- [ ] Implement parent-child hierarchy:
  - Store map of entity → parent entity
  - Store map of entity → list of children
  - Implement `setParent(child, parent)`
  - Implement `getParent(entity)`
  - Implement `getChildren(entity)`
- [ ] Implement `update(deltaTime)`: execute all scene systems
- [ ] Store scene-level settings: ambient light color, fog, skybox

**Why this matters:** Scene owns all entities. Switching scenes = load different set of entities.

### 3.6 SceneManager
**Purpose:** Load, unload, and transition between scenes

- [ ] Create `src/Scene/SceneManager.hpp` and `.cpp`
- [ ] Store map of scene name → Scene object
- [ ] Store active scene pointer
- [ ] Implement `loadScene(name)`: unload current, load new, set as active
- [ ] Implement `unloadScene(name)`: destroy scene and all entities
- [ ] Implement `loadSceneAdditive(name)`: load scene without unloading current (for UI overlays)
- [ ] Implement `getActiveScene()`: return currently active scene
- [ ] Implement `update(deltaTime)`: call update on all active scenes
- [ ] Support async scene loading: load in background, switch when ready
- [ ] Scene transition callbacks: onSceneUnload, onSceneLoaded

**Why this matters:** Games have menus, levels, cutscenes. SceneManager handles transitions.

### 3.7 Scene Serialization
**Purpose:** Save and load scenes from disk for persistence

- [ ] Add nlohmann/json to `vcpkg.json`
- [ ] Create `src/Scene/SceneSerializer.hpp` and `.cpp`
- [ ] Implement `serialize(scene, filepath)`:
  - Iterate all entities in scene
  - For each entity, serialize all components as JSON
  - Store parent-child hierarchy
  - Store asset references as GUID (not path)
  - Write to file
- [ ] Implement `deserialize(filepath)`:
  - Parse JSON
  - Create entities
  - Recreate components from JSON data
  - Restore hierarchy
  - Resolve asset GUIDs to loaded assets
  - Return loaded scene
- [ ] Support component polymorphism: save component type name, factory pattern to recreate
- [ ] Validate scene file version for compatibility

**Why this matters:** Scenes created in editor must be saved. Saved scenes loaded at runtime.

---

## Phase 4: Asset Pipeline

**Goal:** Unified asset management with GUID-based references and hot-reload

### 4.1 Asset Base Class
**Purpose:** Common interface for all loadable resources

- [ ] Create `src/Assets/Asset.hpp`
- [ ] Store GUID (128-bit UUID, unique across all assets)
- [ ] Store file path relative to assets/ directory
- [ ] Store asset type enum (Texture, Mesh, Material, Scene, etc.)
- [ ] Store load state enum (Unloaded, Loading, Loaded, Failed)
- [ ] Store reference count (how many handles point to this asset)
- [ ] Implement virtual `load()`: subclasses load from disk
- [ ] Implement virtual `unload()`: subclasses free resources
- [ ] Implement virtual `reload()`: subclasses reload from disk (hot-reload)
- [ ] Implement `isLoaded()`: check if ready to use

**Why this matters:** Polymorphic asset handling. AssetManager doesn't need to know asset type details.

### 4.2 Asset Types

#### TextureAsset
- [ ] Create `src/Assets/TextureAsset.hpp` and `.cpp`
- [ ] Wrap bgfx texture handle
- [ ] Load image file (PNG, JPG, TGA) using stb_image
- [ ] Create bgfx texture from pixel data
- [ ] Store texture settings: wrap mode, filter mode, mipmap levels
- [ ] Generate mipmaps if needed

#### MeshAsset
- [ ] Create `src/Assets/MeshAsset.hpp` and `.cpp`
- [ ] Wrap Mesh object from Phase 1.2
- [ ] Load from OBJ file (or cached binary format)
- [ ] Store vertex count, triangle count for stats

#### MaterialAsset
- [ ] Create `src/Assets/MaterialAsset.hpp` and `.cpp`
- [ ] Load material definition from JSON
- [ ] Reference shader by GUID
- [ ] Reference textures by GUID
- [ ] Store uniform default values

#### ShaderAsset
- [ ] Create `src/Assets/ShaderAsset.hpp` and `.cpp`
- [ ] Load shader source from .sc files
- [ ] Compile using shaderc
- [ ] Cache compiled bytecode

#### SceneAsset
- [ ] Create `src/Assets/SceneAsset.hpp` and `.cpp`
- [ ] Wrap Scene object
- [ ] Load using SceneSerializer

#### PrefabAsset (later)
- [ ] Create `src/Assets/PrefabAsset.hpp` and `.cpp`
- [ ] Store entity template with components
- [ ] Instantiate multiple copies in scene

**Why this matters:** Typed assets provide specific functionality. Base class provides common interface.

### 4.3 AssetManager
**Purpose:** Central registry for loading and caching all assets

- [ ] Create `src/Assets/AssetManager.hpp` and `.cpp`
- [ ] Store map of GUID → Asset pointer
- [ ] Store map of file path → GUID
- [ ] Implement `load<T>(path)`: load asset from path, return typed handle
- [ ] Implement `get<T>(handle)`: retrieve asset by handle, return typed pointer
- [ ] Implement `unload(handle)`: decrement ref count, destroy if zero
- [ ] Implement `importAsset(path)`: import external file into project
- [ ] Support async loading: load on background thread, notify when ready
- [ ] Implement `getAssetByGUID(guid)`: retrieve asset by GUID
- [ ] Implement `getAllAssetsOfType<T>()`: get all assets of specific type

**Why this matters:** Single source of truth for all resources. Prevents duplicate loads.

### 4.4 Asset Import Pipeline
**Purpose:** Convert external files into engine-optimized formats

- [ ] Detect file type by extension (.png → TextureAsset, .obj → MeshAsset)
- [ ] Texture import:
  - Load image using stb_image
  - Compress using BC7 or ASTC (platform-dependent)
  - Generate mipmaps
  - Save optimized texture to assets/.cache/
- [ ] Mesh import:
  - Load using Assimp
  - Optimize vertex order for cache efficiency (meshoptimizer)
  - Generate tangent/bitangent for normal mapping
  - Save binary mesh format to assets/.cache/
- [ ] Scene import:
  - Parse and validate JSON
  - Resolve asset references
  - Save validated scene
- [ ] Generate .meta file for each asset:
  - Contains GUID
  - Contains import settings (compression, LOD levels, etc.)
  - Timestamp for detecting changes
- [ ] Update AssetDatabase.json with new asset entry

**Why this matters:** Artists provide raw assets. Import pipeline optimizes for runtime. .meta files track GUIDs.

### 4.5 Hot-Reload System
**Purpose:** Automatically reload assets when files change during development

- [ ] Implement file watcher (platform-specific):
  - Linux: inotify
  - macOS: FSEvents
  - Windows: ReadDirectoryChangesW
- [ ] Watch assets/ directory recursively
- [ ] On file modification detected:
  - Determine asset GUID from file path
  - Retrieve asset from AssetManager
  - Call asset's `reload()` method
  - Notify systems that use this asset (e.g., RenderManager)
- [ ] Debounce file events: ignore rapid successive changes (editors save multiple times)
- [ ] Support shader hot-reload: recompile shader, update all materials using it
- [ ] Support texture hot-reload: reload texture, update all materials using it
- [ ] Log reload events for debugging

**Why this matters:** Edit texture in Photoshop, save, instantly see in engine. Massive workflow improvement.

### 4.6 Asset Database
**Purpose:** Persistent mapping of GUIDs to file paths

- [ ] Create `assets/AssetDatabase.json`
- [ ] Structure: array of asset entries, each with GUID, path, type
- [ ] Save database when new assets imported
- [ ] Load database on engine startup
- [ ] Rebuild database command: scan assets/ directory, regenerate from .meta files
- [ ] Detect missing assets: GUID in database but file missing
- [ ] Detect new assets: file exists but no database entry

**Why this matters:** Moving files breaks path-based references. GUID-based references survive file moves.

---

## Phase 5: Advanced Systems

**Goal:** Complete core engine features for production-ready games

### 5.1 Texture System
**Purpose:** Load and manage 2D textures and cubemaps

- [ ] Add stb_image to `extern/` directory
- [ ] Create `src/Renderer/Texture.hpp` and `.cpp`
- [ ] Wrap bgfx texture handle with RAII
- [ ] Load image formats: PNG, JPG, TGA, BMP
- [ ] Implement texture settings:
  - Wrap mode: Clamp, Repeat, MirrorRepeat
  - Filter mode: Point (nearest), Bilinear, Trilinear
  - Anisotropic filtering level (1x, 2x, 4x, 8x, 16x)
  - Mipmap generation (auto or manual)
- [ ] Support texture types:
  - 2D textures (standard)
  - Cubemaps (6 faces for skybox, reflections)
  - Render textures (render to texture for post-processing)
- [ ] Compress textures for GPU (BC7 on desktop, ASTC on mobile)
- [ ] Streaming: load low-res mip first, stream high-res later

**Why this matters:** Textures are largest memory consumers. Proper management critical for performance.

### 5.2 Model Loading
**Purpose:** Import complex 3D models from standard formats

- [ ] Add Assimp to `vcpkg.json`
- [ ] Create `src/Assets/ModelImporter.hpp` and `.cpp`
- [ ] Load file formats: OBJ, FBX, GLTF, Collada
- [ ] Parse model hierarchy: parent-child relationships preserved
- [ ] Extract meshes: one MeshAsset per submesh
- [ ] Extract materials: create MaterialAsset for each material
- [ ] Extract textures: import referenced texture files
- [ ] Extract animations: bone hierarchy and keyframes (for future animation system)
- [ ] Generate missing data:
  - Normals if not provided
  - Tangents/bitangents for normal mapping
  - Texture coordinates if missing
- [ ] Optimize mesh: reorder vertices for GPU cache efficiency
- [ ] Generate LODs (Levels of Detail): simplified meshes for distance rendering (optional)

**Why this matters:** Can't manually create complex models. Assimp handles 40+ formats.

### 5.3 Lighting System
**Purpose:** Simulate light sources for realistic shading

#### Light Components
- [ ] Create `src/ECS/Components/DirectionalLight.hpp`
  - Represents sun or moon (parallel rays)
  - Properties: color (RGB), intensity, direction
  - Used for outdoor scenes

- [ ] Create `src/ECS/Components/PointLight.hpp`
  - Represents light bulb (radiates in all directions)
  - Properties: color, intensity, range (attenuation distance)
  - Used for lamps, candles, explosions

- [ ] Create `src/ECS/Components/SpotLight.hpp`
  - Represents flashlight or spotlight (cone of light)
  - Properties: color, intensity, range, inner cone angle, outer cone angle
  - Used for flashlights, car headlights, stage lights

#### Lighting System
- [ ] Create `src/Renderer/LightingSystem.hpp` and `.cpp`
- [ ] Query all entities with light components
- [ ] Collect light data into array
- [ ] Upload light data to GPU via Uniform Buffer Object (UBO)
- [ ] Limit maximum lights per frame (e.g., 8 point lights, 4 spot lights, 1 directional)
- [ ] Select most important lights (by distance to camera, intensity)
- [ ] Implement Blinn-Phong shading model:
  - Ambient term: base lighting level
  - Diffuse term: Lambert's cosine law
  - Specular term: Blinn halfway vector (faster than Phong)
- [ ] Optional: Implement PBR (Physically Based Rendering):
  - Roughness/metallic workflow
  - Cook-Torrance BRDF
  - Image-based lighting (IBL)

**Why this matters:** Lighting defines visual quality. Blinn-Phong is fast, PBR is realistic.

### 5.4 Shadow System
**Purpose:** Render shadows cast by objects to increase depth perception

- [ ] Implement shadow mapping technique:
  - Render scene from light's perspective to depth buffer (shadow map)
  - When rendering scene from camera, check if pixel is in shadow by sampling shadow map
- [ ] Create shadow map render pass:
  - Set light as camera
  - Render all shadow-casting objects to depth texture
  - Store result in RenderTexture
- [ ] Update fragment shader:
  - Transform pixel position to light space
  - Sample shadow map at that position
  - Compare depth: if pixel farther than shadow map, it's in shadow
- [ ] Implement PCF (Percentage Closer Filtering):
  - Sample multiple shadow map texels
  - Average results for soft shadow edges
- [ ] Directional light shadows:
  - Use cascade shadow maps (CSM)
  - Split camera frustum into cascades (near/medium/far)
  - Each cascade has its own shadow map resolution
  - Reduces perspective aliasing
- [ ] Point light shadows:
  - Render to cubemap (6 faces)
  - More expensive, use sparingly
- [ ] Handle shadow acne: bias depth comparison slightly

**Why this matters:** Shadows ground objects in world. Without shadows, objects look floating.

### 5.5 Physics Integration
**Purpose:** Simulate realistic physical interactions (collision, gravity)

#### Physics Library Choice
- [ ] Evaluate options:
  - Jolt Physics: Modern, fast, excellent 3D support, active development
  - Box2D: Proven, excellent 2D support, simpler but 2D only
  - PhysX: Industry standard, complex, large dependency
  - Bullet: Mature, good 3D, aging codebase
- [ ] **Recommendation:** Jolt for 3D engine
- [ ] Add Jolt to `extern/` as git submodule

#### Physics Manager
- [ ] Create `src/Physics/PhysicsManager.hpp` and `.cpp`
- [ ] Initialize Jolt physics system
- [ ] Create `src/Physics/PhysicsWorld.hpp`
- [ ] Implement fixed timestep update (e.g., 60 Hz):
  - Accumulate frame time
  - Step physics in fixed intervals
  - Prevents physics instability from variable framerate
- [ ] Implement gravity: apply constant downward force
- [ ] Configure collision layers: which objects collide with which
- [ ] Implement debug rendering: draw collision shapes for debugging

#### Physics Components
- [ ] Create `src/ECS/Components/RigidBody.hpp`
  - Properties: mass, velocity, angular velocity
  - Body type: Static (immovable), Dynamic (affected by forces), Kinematic (moved manually)
  - Constraints: freeze position/rotation axes
  - Drag: air resistance
  - Restitution: bounciness

- [ ] Create `src/ECS/Components/Collider.hpp`
  - Collider types: Box, Sphere, Capsule, Mesh (convex or concave)
  - Is trigger: passes through, no collision response, fires events only
  - Physics material: friction, restitution
  - Offset from entity position

#### Collision Callbacks
- [ ] Implement collision event system:
  - `onCollisionEnter(Entity other)`: called when collision starts
  - `onCollisionStay(Entity other)`: called while colliding
  - `onCollisionExit(Entity other)`: called when collision ends
  - `onTriggerEnter(Entity other)`: called when entering trigger volume
  - `onTriggerExit(Entity other)`: called when leaving trigger volume
- [ ] Integrate with EventBus: publish collision events

#### Raycast API
- [ ] Implement `raycast(origin, direction, maxDistance, hit)`:
  - Cast ray through physics world
  - Return first hit object
  - Fill hit structure: entity, position, normal, distance
- [ ] Implement `raycastAll(origin, direction, maxDistance)`:
  - Return all hits along ray, sorted by distance
- [ ] Use cases: shooting, mouse picking, ground detection

**Why this matters:** Physics makes games interactive. Jumping, collisions, ragdolls all need physics.

### 5.6 Audio System
**Purpose:** Play sounds and music with spatial 3D positioning

#### Audio Manager
- [ ] Add OpenAL to `vcpkg.json`
- [ ] Create `src/Audio/AudioManager.hpp` and `.cpp`
- [ ] Initialize OpenAL device and context
- [ ] Set listener position/orientation from main camera (for 3D audio)

#### Audio Clip
- [ ] Create `src/Audio/AudioClip.hpp` and `.cpp`
- [ ] Load audio file formats: WAV, OGG Vorbis
- [ ] Store audio buffer in OpenAL
- [ ] Store duration, sample rate, channel count

#### Audio Source Component
- [ ] Create `src/ECS/Components/AudioSource.hpp`
- [ ] Properties:
  - AudioClip reference (what to play)
  - Volume (0.0 - 1.0)
  - Pitch (0.5 = lower, 2.0 = higher)
  - Loop (repeat when finished)
  - Spatial (3D positioning) or flat (2D UI sound)
  - Min/max distance (for 3D attenuation)
  - Play on awake (start automatically)

#### Audio Mixer
- [ ] Create audio groups: Master, Music, SFX, Voice
- [ ] Each group has volume multiplier
- [ ] Audio sources assigned to groups
- [ ] Final volume = source volume × group volume × master volume
- [ ] Expose mixer controls in editor

#### Audio API
- [ ] Implement `play(AudioClip)`: play 2D sound (UI click)
- [ ] Implement `play3D(AudioClip, position)`: play 3D sound at world position
- [ ] Implement `stop(AudioSource)`: stop playing
- [ ] Implement `pause(AudioSource)`: pause, can resume
- [ ] Implement `resume(AudioSource)`: continue from pause
- [ ] Implement `setVolume(volume)`: set master volume
- [ ] Implement `setGroupVolume(group, volume)`: set group volume

**Why this matters:** Audio is 50% of game feel. Footsteps, gunshots, music set mood.

### 5.7 Particle System
**Purpose:** Simulate effects like fire, smoke, explosions, magic spells

- [ ] Create `src/ECS/Components/ParticleEmitter.hpp`
- [ ] Emission properties:
  - Rate: particles spawned per second
  - Burst: spawn N particles instantly
  - Emission shape: point, sphere, cone, box
- [ ] Particle properties:
  - Lifetime: how long before particle dies
  - Start/end color: interpolate over lifetime
  - Start/end size: interpolate over lifetime
  - Start velocity: initial direction and speed
  - Gravity modifier: how much gravity affects particles
- [ ] Rendering:
  - Billboard particles: always face camera
  - Texture: sprite for particle
  - Blending mode: additive (fire/magic) or alpha (smoke)
- [ ] Optimization:
  - GPU particle system: simulate on GPU using compute shaders
  - Particle pooling: reuse dead particles instead of allocating
  - Particle sorting: sort back-to-front for correct transparency
- [ ] Presets: fire, smoke, explosion, sparks, rain, snow

**Why this matters:** Particles add life to world. Explosions, magic effects, environmental ambience.

### 5.8 Post-Processing
**Purpose:** Screen-space effects applied after scene rendering

#### Render to Texture
- [ ] Create RenderTexture: texture you can render scene to
- [ ] Render scene to RenderTexture instead of screen
- [ ] Apply post-process effects to RenderTexture
- [ ] Render final result to screen

#### Post-Process Effects
- [ ] **Bloom**: glow around bright areas
  - Threshold: extract pixels brighter than value
  - Blur: Gaussian blur on bright areas
  - Composite: add blur back to original

- [ ] **Tone Mapping**: map HDR colors to displayable LDR range
  - Reinhard, ACES, Uncharted 2 tone mappers

- [ ] **Color Grading**: adjust color, contrast, saturation
  - Use LUT (Look-Up Table) for artistic color correction

- [ ] **Anti-Aliasing**: smooth jagged edges
  - FXAA: fast, post-process AA
  - SMAA: higher quality than FXAA

- [ ] **Depth of Field** (optional): blur based on distance from focus

- [ ] **Motion Blur** (optional): blur based on movement

#### Post-Process Stack
- [ ] Chain multiple effects: bloom → tone mapping → color grading → AA
- [ ] Allow enabling/disabling individual effects
- [ ] Expose effect parameters in editor

**Why this matters:** Post-processing is final polish. Bloom adds dreaminess, tone mapping prevents washed out colors.

### 5.9 Skybox
**Purpose:** Background environment visible behind all objects

- [ ] Create `src/Renderer/Skybox.hpp` and `.cpp`
- [ ] Load cubemap texture (6 images: +X, -X, +Y, -Y, +Z, -Z)
- [ ] Render cubemap as infinitely distant background:
  - Render as first pass (before scene)
  - Disable depth writes (skybox always behind)
  - Remove translation from view matrix (keeps skybox centered)
- [ ] Support HDR skybox for image-based lighting
- [ ] Support procedural sky:
  - Gradient from horizon to zenith
  - Time of day: sunrise/sunset colors
  - Atmospheric scattering (Rayleigh/Mie)

**Why this matters:** Empty black background looks unfinished. Skybox adds atmosphere and context.

### 5.10 Animation System
**Purpose:** Bring characters and objects to life with skeletal and property animation

#### Animation Data Structures
- [ ] Create `src/Animation/Skeleton.hpp`
  - Bone hierarchy: parent-child relationships
  - Bind pose: default bone transforms
  - Inverse bind matrices: for skinning calculation
  - Bone name to index mapping

- [ ] Create `src/Animation/AnimationClip.hpp`
  - Store keyframes: time + transform (position, rotation, scale)
  - Support multiple interpolation modes: Step, Linear, Cubic (Bezier)
  - Store animation duration and frame rate
  - Loop mode: Once, Loop, PingPong

- [ ] Create `src/Animation/AnimationTrack.hpp`
  - Keyframes for a single property (position, rotation, or scale)
  - Binary search for keyframe lookup by time
  - Interpolate between keyframes

#### Skeletal Animation
- [ ] Create `src/Animation/SkinnedMesh.hpp`
  - Vertex weights: up to 4 bones per vertex
  - Bone indices: which bones affect each vertex
  - Calculate final vertex position from bone transforms

- [ ] Create `src/Animation/AnimationSampler.hpp`
  - Sample animation clip at specific time
  - Return bone transforms for that frame
  - Handle looping and time wrapping

- [ ] Create `src/Animation/AnimationBlender.hpp`
  - Blend between two poses (crossfade)
  - Additive blending: layer animations (walk + wave)
  - Masked blending: different animations for upper/lower body

#### Animation State Machine
- [ ] Create `src/Animation/AnimatorController.hpp`
  - States: each state plays an animation clip
  - Transitions: conditions to switch states (parameters, time)
  - Parameters: bool, int, float, trigger (for gameplay control)
  - Blend trees: blend multiple animations based on parameters (walk/run by speed)

- [ ] Create `src/Animation/AnimatorState.hpp`
  - Reference to animation clip
  - Speed multiplier
  - Entry/exit events

- [ ] Create `src/Animation/AnimatorTransition.hpp`
  - Source and destination states
  - Transition duration (blend time)
  - Conditions: parameter comparisons
  - Has exit time: wait for animation to finish

#### Animator Component
- [ ] Create `src/ECS/Components/Animator.hpp`
  - Reference to AnimatorController asset
  - Current state and time
  - Parameter values
  - Implement `setParameter(name, value)`
  - Implement `trigger(name)`: set trigger parameter

#### Animation System (ECS)
- [ ] Create `src/ECS/Systems/AnimationSystem.hpp` and `.cpp`
  - Query entities with Animator + SkinnedMeshRenderer
  - Update animator state machine
  - Sample current animation
  - Apply bone transforms to skinned mesh
  - Upload bone matrices to GPU (uniform buffer)

#### Animation Events
- [ ] Support animation events: trigger at specific frame
  - Footstep sounds
  - Spawn particle effects
  - Enable/disable hitboxes
- [ ] Integrate with EventBus: publish animation events

**Why this matters:** Characters need to walk, run, attack. Without animation, games feel static and lifeless.

### 5.11 Runtime UI System
**Purpose:** In-game user interface for menus, HUD, dialogs (separate from editor ImGui)

#### UI Architecture
- [ ] Create `src/UI/UICanvas.hpp`
  - Root container for UI elements
  - Screen space vs world space rendering
  - Sort order for layering multiple canvases
  - Reference resolution for scaling (1920x1080 design resolution)
  - Scale mode: ConstantPixelSize, ScaleWithScreenSize, ConstantPhysicalSize

- [ ] Create `src/UI/UIElement.hpp` (base class)
  - Anchor points: where element attaches to parent (top-left, center, stretch)
  - Pivot point: element's own origin for rotation/scaling
  - Position offset from anchor
  - Size (width, height)
  - Rotation and scale
  - Enabled/visible flags
  - Parent-child hierarchy

#### Layout System
- [ ] Create `src/UI/UILayoutGroup.hpp`
  - Horizontal layout: children arranged left-to-right
  - Vertical layout: children arranged top-to-bottom
  - Grid layout: children in rows and columns
  - Spacing between children
  - Padding inside container
  - Child alignment (start, center, end, stretch)

- [ ] Create `src/UI/UIContentSizeFitter.hpp`
  - Fit to content: resize element to fit children
  - Horizontal/vertical fit modes: Unconstrained, MinSize, PreferredSize

#### UI Widgets
- [ ] Create `src/UI/Widgets/UIImage.hpp`
  - Display texture or sprite
  - Color tint
  - Image type: Simple, Sliced (9-patch), Tiled, Filled
  - Fill amount for progress bars (radial or linear)

- [ ] Create `src/UI/Widgets/UIText.hpp`
  - Display text string
  - Font asset reference
  - Font size, color
  - Alignment: left, center, right, justified
  - Overflow: truncate, ellipsis, overflow
  - Rich text support: <b>bold</b>, <i>italic</i>, <color=#FF0000>colored</color>

- [ ] Create `src/UI/Widgets/UIButton.hpp`
  - Normal, hovered, pressed, disabled states
  - Different sprites/colors per state
  - OnClick event callback
  - Transition type: ColorTint, SpriteSwap, Animation

- [ ] Create `src/UI/Widgets/UISlider.hpp`
  - Min/max value range
  - Current value
  - Fill image for progress visualization
  - Handle dragging
  - OnValueChanged callback

- [ ] Create `src/UI/Widgets/UIToggle.hpp`
  - Checkbox or radio button behavior
  - IsOn boolean state
  - Toggle group (for radio buttons)
  - OnValueChanged callback

- [ ] Create `src/UI/Widgets/UIInputField.hpp`
  - Text input with cursor
  - Placeholder text
  - Character limit
  - Content type: Standard, Integer, Decimal, Password
  - OnValueChanged, OnSubmit callbacks

- [ ] Create `src/UI/Widgets/UIScrollView.hpp`
  - Scrollable content area
  - Horizontal/vertical scrollbars
  - Scroll sensitivity
  - Elastic bounce at edges

#### UI Event System
- [ ] Create `src/UI/UIEventSystem.hpp`
  - Raycast against UI elements
  - Determine which element is under mouse/touch
  - Event propagation: bubble up through hierarchy
  - Pointer events: Enter, Exit, Down, Up, Click, Drag
  - Block game input when UI is focused

- [ ] Create `src/UI/UIRaycaster.hpp`
  - Screen-space raycast for screen-space canvases
  - World-space raycast for world-space canvases (VR menus, in-game screens)

#### Font Rendering
- [ ] Create `src/UI/Font.hpp`
  - Load font file (TTF/OTF) using stb_truetype or FreeType
  - Generate font atlas: texture containing all glyphs
  - Store glyph metrics: advance, bearing, size
  - Support multiple font sizes (or SDF fonts for any size)

- [ ] Create `src/UI/TextRenderer.hpp`
  - Generate mesh from text string
  - Kerning: adjust spacing between specific character pairs
  - Line breaking and word wrapping
  - Batch text rendering for performance

#### UI Rendering
- [ ] Create `src/UI/UIRenderer.hpp`
  - Render UI after scene (on top)
  - Batch UI draw calls by texture
  - Support transparency and blending
  - Render order: back-to-front within canvas
  - Separate render pass for UI

#### UI Animation
- [ ] Create `src/UI/UITween.hpp`
  - Animate UI properties over time
  - Properties: position, scale, rotation, alpha, color
  - Easing functions: Linear, EaseIn, EaseOut, EaseInOut, Bounce, Elastic
  - Sequence multiple tweens
  - OnComplete callback

**Why this matters:** Every game needs menus, health bars, inventory screens. ImGui is for developers, runtime UI is for players.

---

## Phase 6: Editor & Tools

**Goal:** Visual editor for scene creation and asset management

### 6.1 ImGui Integration
**Purpose:** UI library for editor interface

- [ ] Add Dear ImGui to `vcpkg.json`
- [ ] Integrate ImGui with bgfx renderer backend
- [ ] Create `src/Editor/EditorLayer.hpp` and `.cpp`
- [ ] Initialize ImGui context in Application startup
- [ ] Begin ImGui frame before rendering
- [ ] End ImGui frame and render after scene
- [ ] Handle ImGui input: pass mouse/keyboard events to ImGui
- [ ] Load ImGui font: use custom font for better readability
- [ ] Configure ImGui style: dark theme, custom colors

**Why this matters:** ImGui is immediate-mode GUI, perfect for tools. No complex UI state management.

### 6.2 Editor Windows

#### Viewport Window
- [ ] Render scene to RenderTexture
- [ ] Display RenderTexture in ImGui window using `ImGui::Image()`
- [ ] Handle viewport resize: update RenderTexture and camera aspect ratio
- [ ] Mouse picking: click entity in viewport to select
  - Render entities with unique colors to offscreen buffer
  - Read pixel color at mouse position
  - Map color back to entity ID
- [ ] Display viewport stats: FPS, draw calls, triangle count

#### Hierarchy Window
- [ ] Display all entities in scene as tree view
- [ ] Show parent-child relationships with indentation
- [ ] Click entity to select (highlight in Inspector)
- [ ] Drag and drop to reparent entities
- [ ] Right-click context menu:
  - Create child entity
  - Duplicate entity
  - Delete entity
  - Rename entity
- [ ] Search/filter entities by name

#### Inspector Window
- [ ] Display all components of selected entity
- [ ] For each component, show editable fields:
  - Transform: position (XYZ), rotation (euler or quaternion), scale (XYZ)
  - MeshRenderer: mesh dropdown, material dropdown
  - Camera: FOV slider, near/far planes
  - Light: color picker, intensity slider
- [ ] Add Component button: dropdown menu of component types
- [ ] Remove Component button for each component
- [ ] Changes applied immediately (live editing)

#### Console Window
- [ ] Display log messages from Logger
- [ ] Color-code by level: Info (white), Warning (yellow), Error (red)
- [ ] Filter by log level: show/hide Info/Warning/Error
- [ ] Search messages by text
- [ ] Clear button to empty console
- [ ] Auto-scroll to latest message
- [ ] Click message to show source file and line number (if available)

#### Asset Browser Window
- [ ] Display contents of `assets/` directory
- [ ] Folder tree view on left, file grid on right
- [ ] Show thumbnails for textures (render preview)
- [ ] Show icons for other file types (mesh, material, scene)
- [ ] Double-click to open:
  - Scene: load scene
  - Material: show material editor
  - Texture: show in image viewer
- [ ] Drag texture from browser to material slot in Inspector
- [ ] Right-click context menu:
  - Import new asset
  - Delete asset
  - Rename asset
  - Show in file explorer

#### Scene Window
- [ ] List all scene files in project
- [ ] Show currently loaded scene (highlighted)
- [ ] Double-click scene to load
- [ ] New Scene button: create empty scene
- [ ] Save Scene button: serialize current scene to file

#### Game Window
- [ ] Separate window for play mode
- [ ] Shows game view (from game camera, not editor camera)
- [ ] Play/Pause/Step buttons to control simulation
- [ ] Game window stats: FPS, frame time

**Why this matters:** Editor windows provide full access to engine features without writing code.

### 6.3 Gizmos
**Purpose:** Visual manipulation of objects in viewport

- [ ] Integrate ImGuizmo library (gizmo rendering for ImGui)
- [ ] Render translate gizmo: 3 arrows (XYZ) to drag entity position
- [ ] Render rotate gizmo: 3 circles (XYZ) to rotate entity
- [ ] Render scale gizmo: 3 cubes (XYZ) to scale entity
- [ ] Keyboard shortcuts to switch modes:
  - W key: translate mode
  - E key: rotate mode
  - R key: scale mode
- [ ] Toggle local vs world space:
  - Local: gizmo aligned with object rotation
  - World: gizmo aligned with world axes
- [ ] Snap to grid:
  - Enable snapping (hold Ctrl)
  - Configure snap increment (0.25, 0.5, 1.0 units)
  - Rotation snap (15°, 45°, 90°)

**Why this matters:** Moving objects with XYZ numbers is tedious. Gizmos are intuitive.

### 6.4 Editor Camera
**Purpose:** Separate camera for navigating scene while editing

- [ ] Create EditorCamera separate from game camera
- [ ] WASD keys for movement (forward, left, back, right)
- [ ] Q/E keys for up/down movement
- [ ] Hold right mouse button to enable mouse look
- [ ] Mouse scroll to change movement speed
- [ ] F key to focus on selected entity (frame in view)
- [ ] Toggle perspective/orthographic projection
- [ ] Save/restore editor camera position between sessions

**Why this matters:** Game camera might be attached to player. Need separate editor camera.

### 6.5 Play Mode
**Purpose:** Test game without leaving editor

- [ ] Play button: enter play mode
  - Save current scene state to temporary buffer
  - Start game simulation (physics, scripts)
  - Switch from Viewport to Game window
- [ ] Pause button: pause simulation
  - Stop time (timeScale = 0)
  - Allow inspecting entities mid-game
- [ ] Step button: advance one frame while paused
  - Useful for debugging frame-by-frame
- [ ] Stop button: exit play mode
  - Restore scene state from saved buffer
  - Discard all changes made during play
  - Switch back to Viewport window
- [ ] Disable editing in play mode: Inspector fields read-only

**Why this matters:** Iteration speed. Click Play, test change, click Stop, modify, repeat.

### 6.6 Prefab System
**Purpose:** Reusable entity templates

- [ ] Create Prefab from entity: save entity and children as asset
- [ ] Prefab file contains: entity hierarchy, all components, default values
- [ ] Instantiate Prefab: create copy of prefab in scene
- [ ] Prefab instances track source prefab (by GUID)
- [ ] Modify prefab instance: show overridden values in blue
- [ ] Apply changes to prefab: update source prefab and all instances
- [ ] Revert changes: reset instance to match prefab
- [ ] Break prefab link: instance becomes standalone entity
- [ ] Nested prefabs: prefab can contain other prefabs

**Why this matters:** Create enemy once, instantiate 100 times. Update prefab, all enemies update.

---

## Phase 7: Build & Distribution

**Goal:** Package and distribute standalone game builds

### 7.1 Build Configuration
**Purpose:** Define different build types for development and release

- [ ] Create build presets in CMake:
  - Debug: full symbols, no optimization, asserts enabled
  - Development: optimized with symbols, asserts enabled, hot-reload
  - Release: full optimization, no symbols, asserts disabled
  - Shipping: release + additional stripping, no editor code

- [ ] Conditional compilation flags:
  - `KRIO_EDITOR`: include editor code (ImGui, gizmos)
  - `KRIO_DEBUG`: enable debug features (profiling, visualization)
  - `KRIO_HOT_RELOAD`: enable asset hot-reload
  - Strip editor-only code from shipping builds

**Why this matters:** Development builds need debugging. Shipping builds need performance and small size.

### 7.2 Asset Packaging
**Purpose:** Bundle assets into optimized archives for distribution

- [ ] Create `src/Build/AssetPacker.hpp` and `.cpp`
- [ ] Package assets into archive files (.pak):
  - Single file contains multiple assets
  - Compressed using LZ4 or Zstd
  - Table of contents for fast lookup
  - Optional encryption for asset protection

- [ ] Asset cooking:
  - Convert all assets to platform-optimized format
  - Generate final texture mipmaps
  - Bake lighting if applicable
  - Validate all asset references

- [ ] Create `src/Assets/PackedAssetLoader.hpp`
  - Load assets from .pak files at runtime
  - Memory-mapped file access for performance
  - Seamless switch between loose files (dev) and packed (shipping)

- [ ] Asset bundles:
  - Group related assets (level1.pak, characters.pak)
  - Support downloadable content (DLC) as separate bundles
  - Dependency tracking between bundles

**Why this matters:** Shipping thousands of loose files is slow and unprofessional. Packed assets load faster.

### 7.3 Platform Export
**Purpose:** Generate standalone executables for target platforms

#### Desktop Builds
- [ ] Create `src/Build/BuildPipeline.hpp`
- [ ] Linux export:
  - Bundle executable + shared libraries
  - AppImage or Flatpak packaging (optional)
  - Steam runtime compatibility

- [ ] Windows export:
  - Bundle executable + DLLs
  - Generate installer (NSIS or WiX)
  - Code signing for Windows SmartScreen

- [ ] macOS export:
  - Create .app bundle structure
  - Code signing and notarization
  - Universal binary (Intel + Apple Silicon)

#### Build Automation
- [ ] Create build scripts:
  - One-click build for each platform
  - Version number injection
  - Build timestamp and git commit hash
  - Automated testing before build

- [ ] Output organization:
  - `builds/windows/KrioGame.exe`
  - `builds/linux/KrioGame`
  - `builds/macos/KrioGame.app`

**Why this matters:** Users don't have development environments. They need a double-clickable executable.

### 7.4 Editor Build Interface
**Purpose:** Expose build functionality in editor

- [ ] Build Settings window in editor:
  - Select target platform
  - Choose build configuration (Development/Release/Shipping)
  - Select scenes to include
  - Set player settings (resolution, fullscreen, company name)

- [ ] Build progress:
  - Progress bar during build
  - Log build steps
  - Error reporting with actionable messages

- [ ] Build and Run:
  - Build then immediately launch game
  - Useful for quick testing

**Why this matters:** Artists and designers should build without touching command line.

### 7.5 Version Management
**Purpose:** Track versions and manage updates

- [ ] Semantic versioning: MAJOR.MINOR.PATCH
- [ ] Build numbers: auto-increment on each build
- [ ] Version displayed in game (settings/about screen)
- [ ] Changelog generation from git commits (optional)
- [ ] Update checking: compare local version with server (optional)

**Why this matters:** Users need to know what version they have. Bug reports need version info.

---

## Phase 8: Scripting (Optional)

**Goal:** Enable gameplay logic in Lua instead of C++

### 8.1 Lua Integration
**Purpose:** Embed Lua VM for runtime scripting

- [ ] Add sol2 (C++ Lua binding library) to `vcpkg.json`
- [ ] Add Lua to `vcpkg.json`
- [ ] Create `src/Scripting/LuaEngine.hpp` and `.cpp`
- [ ] Initialize Lua VM on engine startup
- [ ] Load and execute Lua script files
- [ ] Handle Lua errors: catch exceptions, log error messages
- [ ] Implement Lua console: execute Lua commands at runtime

**Why this matters:** C++ requires recompilation. Lua scripts hot-reload instantly.

### 8.2 C++ to Lua Bindings
**Purpose:** Expose engine API to Lua scripts

- [ ] Bind core types:
  - Vector3: position, direction math
  - Quaternion: rotation math
  - Entity: access to game objects
  - Transform: get/set position, rotation, scale
  - Input: isKeyPressed, getMousePosition
  - Time: deltaTime, time, timeScale
- [ ] Bind managers:
  - SceneManager: loadScene, getActiveScene
  - AssetManager: load assets by path
- [ ] Bind components:
  - Each component type exposed to Lua
  - Can get/set component properties from Lua
- [ ] Register Lua functions as event callbacks

**Why this matters:** Lua code needs to interact with engine. Bindings provide the API.

### 8.3 LuaScript Component
**Purpose:** Attach Lua behavior to entities

- [ ] Create `src/ECS/Components/LuaScript.hpp`
- [ ] Store script file path
- [ ] Store reference to Lua state
- [ ] Lua script callbacks:
  - `onStart()`: called when entity spawned, initialize variables
  - `onUpdate(dt)`: called every frame, gameplay logic
  - `onDestroy()`: called when entity destroyed, cleanup
  - `onCollisionEnter(other)`: called when collision starts
- [ ] Access entity from Lua: global `entity` variable
- [ ] Access components from Lua: `entity:getComponent("Transform")`

**Why this matters:** Designers write Lua scripts. Programmers write C++ engine.

### 8.4 Lua API Documentation
**Purpose:** Document Lua API for script writers

- [ ] Generate API docs from C++ bindings using Doxygen or custom tool
- [ ] Provide example scripts:
  - Player controller script
  - Enemy AI script
  - Pickup item script
  - Camera follow script
- [ ] Autocomplete support:
  - Generate .lua stub files for IDE autocomplete
  - Or use Language Server Protocol (LSP)

**Why this matters:** Undocumented API is useless. Designers need examples to learn.

---

## Phase 9: Networking (Optional)

**Goal:** Enable multiplayer gameplay with client-server architecture

### 9.1 Network Architecture
**Purpose:** Foundation for networked multiplayer games

#### Architecture Choice
- [ ] Evaluate architectures:
  - **Client-Server**: authoritative server, clients send inputs (recommended)
  - **Peer-to-Peer**: simpler but harder to secure, prone to cheating
  - **Relay Server**: P2P through server for NAT traversal
- [ ] **Recommendation:** Client-Server for competitive games, P2P acceptable for co-op

#### Network Library
- [ ] Evaluate options:
  - **ENet**: simple, reliable UDP, battle-tested
  - **GameNetworkingSockets** (Valve): robust, NAT traversal, encryption
  - **yojimbo**: modern, designed for game networking
- [ ] **Recommendation:** ENet for simplicity, GameNetworkingSockets for production
- [ ] Add chosen library to dependencies

**Why this matters:** Architecture choice affects security, latency, and complexity.

### 9.2 NetworkManager
**Purpose:** Handle connections and message routing

- [ ] Create `src/Network/NetworkManager.hpp` and `.cpp`
- [ ] Connection management:
  - Host game (create server)
  - Join game (connect to server)
  - Disconnect handling
  - Connection timeout detection

- [ ] Message system:
  - Define message types (enum or ID)
  - Serialize messages to bytes
  - Deserialize bytes to messages
  - Message queuing and batching

- [ ] Reliability modes:
  - Unreliable: fire and forget (position updates)
  - Reliable: guaranteed delivery with ordering (chat, events)
  - Reliable unordered: guaranteed but can arrive out of order

- [ ] Network statistics:
  - Ping/latency measurement
  - Packet loss detection
  - Bandwidth usage

**Why this matters:** All network features depend on reliable connection management.

### 9.3 State Synchronization
**Purpose:** Keep game state consistent across clients

#### Entity Replication
- [ ] Create `src/Network/NetworkIdentity.hpp` component
  - Unique network ID for each networked entity
  - Owner: which client controls this entity
  - Authority: server or client authoritative

- [ ] Create `src/Network/NetworkTransform.hpp` component
  - Sync position, rotation, scale
  - Configurable sync rate (10-60 Hz)
  - Interpolation for smooth movement
  - Extrapolation for lag compensation

- [ ] Create `src/Network/ReplicationManager.hpp`
  - Track which entities need syncing
  - Determine what changed (dirty flags)
  - Prioritize updates by importance/distance
  - Delta compression: only send changes

#### Snapshot System
- [ ] Server sends world snapshots at fixed rate
- [ ] Clients interpolate between received snapshots
- [ ] Snapshot buffer: store recent snapshots for interpolation
- [ ] Jitter buffer: smooth out network timing variations

**Why this matters:** Players need to see consistent game state despite network latency.

### 9.4 Client-Side Prediction
**Purpose:** Make controls feel responsive despite latency

- [ ] Input prediction:
  - Client applies input immediately (don't wait for server)
  - Store input history with timestamps
  - Send inputs to server

- [ ] Server reconciliation:
  - Server processes input, sends authoritative state
  - Client compares predicted state to server state
  - If mismatch: replay inputs from correction point

- [ ] Smooth correction:
  - Don't snap to corrected position (jarring)
  - Smoothly interpolate to correct position
  - Threshold: only correct if error exceeds limit

**Why this matters:** Without prediction, 100ms latency means 100ms input delay. Unplayable.

### 9.5 Lag Compensation
**Purpose:** Fair hit detection despite latency

- [ ] Server-side rewind:
  - Store world state history (positions at each tick)
  - When processing shot, rewind to shooter's view time
  - Check hit against historical positions
  - Apply damage in current time

- [ ] Client hit markers:
  - Client predicts hits for immediate feedback
  - Server confirms or denies hits
  - Display feedback accordingly

**Why this matters:** Players shoot where they see enemies, not where enemies actually are.

### 9.6 Networked Components
**Purpose:** Network-aware versions of game components

- [ ] Create `src/ECS/Components/NetworkRigidbody.hpp`
  - Sync physics state (velocity, angular velocity)
  - Server authoritative physics
  - Client interpolation

- [ ] Create `src/ECS/Components/NetworkAnimator.hpp`
  - Sync animation state and parameters
  - Trigger animations across network
  - Animation events replicated

- [ ] Create `src/Network/NetworkSpawner.hpp`
  - Spawn networked entities
  - Assign network IDs
  - Sync spawn across clients

- [ ] Create `src/Network/NetworkDestroy.hpp`
  - Destroy entities across network
  - Handle pending references gracefully

**Why this matters:** Game systems need network-aware versions to work in multiplayer.

### 9.7 Remote Procedure Calls (RPC)
**Purpose:** Call functions across network boundary

- [ ] Create `src/Network/RPC.hpp`
- [ ] RPC types:
  - ClientRPC: server calls function on specific client
  - ServerRPC: client calls function on server
  - MulticastRPC: server calls function on all clients

- [ ] RPC parameters:
  - Serialize parameters automatically
  - Support basic types and custom serializable types

- [ ] RPC reliability:
  - Choose reliable or unreliable per RPC
  - Buffering for late joiners (optional)

**Why this matters:** Events like "player scored" need to trigger UI on all clients.

### 9.8 Lobby & Matchmaking
**Purpose:** Connect players before game starts

- [ ] Lobby system:
  - Create lobby (host)
  - Browse/search lobbies
  - Join lobby
  - Ready state per player
  - Start game when all ready

- [ ] Player info:
  - Player name
  - Team assignment
  - Character/loadout selection

- [ ] Chat:
  - Text chat in lobby
  - In-game chat (team/all)

- [ ] Matchmaking (optional, requires backend):
  - Skill-based matchmaking
  - Queue system
  - Match found notification

**Why this matters:** Players need a way to find and join games.

---

## Phase 10: Optimization & Polish

**Goal:** Performance improvements and developer experience enhancements

### 10.1 Profiling
**Purpose:** Measure performance to identify bottlenecks

- [ ] Integrate Tracy profiler (real-time profiling tool)
- [ ] Add profiling macros: `KRIO_PROFILE_SCOPE("FunctionName")`
- [ ] Instrument key systems:
  - RenderSystem update time
  - PhysicsManager fixed update time
  - SceneManager update time
  - Individual shader and draw call timings
- [ ] Measure memory allocations:
  - Track allocations per frame
  - Identify allocation hotspots
- [ ] Visualize in Tracy UI:
  - Frame time graph
  - CPU flame graph
  - GPU utilization
  - Memory usage over time
- [ ] Profile builds: enable profiling only in Debug/RelWithDebInfo, not Release

**Why this matters:** Can't optimize what you don't measure. Tracy shows exactly where time is spent.

### 10.2 Frustum Culling
**Purpose:** Skip rendering objects outside camera view

- [ ] Extract camera frustum planes from view-projection matrix
- [ ] Calculate bounding box (AABB) for each mesh
- [ ] Test each mesh's AABB against frustum planes
- [ ] If outside frustum, skip submitting to RenderManager
- [ ] Hierarchical culling: if parent outside, children also outside
- [ ] Debug visualization: draw frustum and bounding boxes in editor
- [ ] Measure culling efficiency: log percentage of objects culled

**Why this matters:** Rendering invisible objects wastes GPU. Frustum culling can save 50%+ draw calls.

### 10.3 Spatial Partitioning
**Purpose:** Accelerate spatial queries (culling, physics, raycasts)

#### Quadtree (2D)
- [ ] Create `src/Spatial/Quadtree.hpp`
- [ ] Divide 2D space into 4 quadrants recursively
- [ ] Store entities in leaf nodes
- [ ] Query by region: return entities in rectangle
- [ ] Used for 2D games or top-down views

#### Octree (3D)
- [ ] Create `src/Spatial/Octree.hpp`
- [ ] Divide 3D space into 8 octants recursively
- [ ] Store entities in leaf nodes
- [ ] Query by frustum: return entities in camera view
- [ ] Query by sphere: return entities in radius

#### Use Cases
- [ ] Frustum culling: query octree with camera frustum, render only returned entities
- [ ] Physics broad-phase: query octree for nearby entities before expensive collision checks
- [ ] Spatial queries: find all enemies within 10 units of player

**Why this matters:** Testing 10,000 entities against frustum is slow. Octree reduces to ~100 tests.

### 10.4 Render Batching
**Purpose:** Reduce draw calls by grouping similar objects

- [ ] Sort draw calls by material: group objects with same shader/textures
- [ ] Batch similar meshes: combine multiple meshes into single draw call
- [ ] Instanced rendering: render many copies of same mesh with one draw call
  - Upload array of transform matrices to GPU
  - Vertex shader reads matrix from array using instance ID
  - Draw 1000 trees with 1 draw call instead of 1000
- [ ] Measure batching effectiveness: log draw calls before/after

**Why this matters:** Each draw call has CPU overhead. 1000 draw calls → 10 batched = huge speedup.

### 10.5 Multi-threading
**Purpose:** Utilize multiple CPU cores for parallel work

- [ ] Job system:
  - Task-based parallelism (like Intel TBB or custom fiber system)
  - Submit jobs to queue, worker threads execute
- [ ] Parallel ECS systems:
  - EnTT supports parallel iteration
  - Process entities across multiple threads
  - Example: update 10,000 transforms in parallel
- [ ] Async asset loading:
  - Load assets on background thread
  - Notify when ready, upload to GPU on main thread
- [ ] Render thread separation (advanced):
  - Main thread builds render commands
  - Render thread submits to GPU
  - Double-buffer command lists

**Why this matters:** Modern CPUs have 8+ cores. Single-threaded code wastes 7 cores.

### 10.6 Memory Management
**Purpose:** Reduce allocations, improve cache performance, track memory usage

#### Custom Allocators
- [ ] Linear allocator:
  - Allocate memory in linear fashion, reset each frame
  - Used for temporary per-frame data
  - Very fast, no fragmentation

- [ ] Pool allocator:
  - Pre-allocate block of same-size objects (e.g., entities, components)
  - Fast allocation/deallocation, no fragmentation

- [ ] Stack allocator:
  - LIFO allocation (last allocated, first freed)
  - Used for scoped temporary data

#### Memory Tracking
- [ ] Track allocations per system:
  - How much memory does RenderManager use?
  - How much memory does PhysicsManager use?
- [ ] Memory budgets:
  - Set maximum memory per system
  - Warning when budget exceeded
- [ ] Leak detection:
  - Track allocations/deallocations
  - Report leaks on shutdown

**Why this matters:** Memory allocations are slow. Custom allocators 10x faster. Tracking prevents leaks.

---

## Testing Strategy

**Goal:** Ensure engine reliability through automated testing

### Unit Testing
**Purpose:** Verify individual components work correctly in isolation

- [ ] Add Google Test (gtest) to `vcpkg.json`
- [ ] Create `tests/` directory structure:
  - `tests/Core/` - Core system tests
  - `tests/ECS/` - Entity component tests
  - `tests/Math/` - Math utility tests
  - `tests/Assets/` - Asset loading tests

#### Core Tests
- [ ] `tests/Core/TestServiceLocator.cpp`
  - Register and retrieve services
  - Null service behavior
  - Shutdown cleanup

- [ ] `tests/Core/TestEventBus.cpp`
  - Subscribe and publish events
  - Multiple subscribers
  - Unsubscribe behavior
  - Event queuing

- [ ] `tests/Core/TestTimeManager.cpp`
  - Delta time calculation
  - Time scale modification
  - Frame counting

#### Math Tests
- [ ] `tests/Math/TestTransform.cpp`
  - Matrix composition (TRS)
  - Quaternion operations
  - Parent-child transforms

- [ ] `tests/Math/TestVector.cpp`
  - Vector operations (add, subtract, dot, cross)
  - Normalization
  - Distance calculations

#### ECS Tests
- [ ] `tests/ECS/TestRegistry.cpp`
  - Entity creation/destruction
  - Component add/remove/get
  - Entity iteration (views)

- [ ] `tests/ECS/TestScene.cpp`
  - Scene hierarchy
  - Parent-child relationships
  - Scene serialization/deserialization

**Why this matters:** Unit tests catch bugs early. Refactoring without tests is dangerous.

### Integration Testing
**Purpose:** Verify systems work together correctly

- [ ] `tests/Integration/TestRenderPipeline.cpp`
  - Shader loading + mesh rendering
  - Material system integration
  - Camera rendering

- [ ] `tests/Integration/TestPhysicsScene.cpp`
  - Physics + ECS integration
  - Collision callbacks
  - Transform synchronization

- [ ] `tests/Integration/TestAssetPipeline.cpp`
  - Asset import + load + hot-reload
  - Reference counting
  - GUID resolution

**Why this matters:** Individual components might work but fail together.

### Performance Testing
**Purpose:** Prevent performance regressions

- [ ] Create `tests/Performance/` directory
- [ ] Benchmark critical paths:
  - ECS iteration speed (10,000+ entities)
  - Render submission throughput
  - Asset loading time
  - Physics step duration

- [ ] Performance baselines:
  - Store baseline metrics
  - Compare against baselines in CI
  - Alert on significant regressions (>10%)

**Why this matters:** Performance can degrade silently. Benchmarks catch it.

### Test Infrastructure
**Purpose:** Make testing easy and automatic

- [ ] CMake test targets:
  - `make test` runs all tests
  - `make test_core` runs core tests only
  - `make test_performance` runs benchmarks

- [ ] Continuous Integration:
  - GitHub Actions workflow
  - Run tests on every commit
  - Run on multiple platforms (Linux, Windows, macOS)
  - Block merge if tests fail

- [ ] Test coverage:
  - Generate coverage reports (gcov/lcov)
  - Track coverage percentage
  - Identify untested code paths

- [ ] Test fixtures:
  - Shared setup/teardown for common scenarios
  - Mock services for isolated testing
  - Test assets in `tests/fixtures/`

**Why this matters:** Tests only help if they run automatically.

### Manual Testing Checklist
**Purpose:** Things that are hard to automate

- [ ] Visual verification:
  - Rendering looks correct
  - No visual artifacts
  - UI displays properly

- [ ] Input testing:
  - Keyboard/mouse responsive
  - Gamepad support (if applicable)
  - Input rebinding works

- [ ] Platform testing:
  - Test on each target platform
  - Different GPU vendors (NVIDIA, AMD, Intel)
  - Different screen resolutions

- [ ] Stress testing:
  - Many entities (10,000+)
  - Long play sessions (memory leaks)
  - Rapid scene switching

**Why this matters:** Some things require human judgment.

---

## Technical Decisions

### Coordinate System
- **Y-up convention**: Unity standard, Y axis points upward
- **Left-handed coordinate system**: X right, Y up, Z forward (into screen)
- **Units**: 1 engine unit = 1 meter (for physics realism)

**Why:** Consistency with Unity makes knowledge transfer easier. Left-handed matches DirectX.

### Rendering
- **Pipeline**: Forward rendering
  - Simple implementation, good for learning
  - Suitable for low-to-medium light counts
  - Later: Consider deferred rendering for many lights (50+)
- **Shading**: Progressive approach
  - Phase 1: Flat shading (solid colors)
  - Phase 2: Blinn-Phong (fast, good enough)
  - Phase 3: PBR (physically accurate, industry standard)
- **Backend**: bgfx auto-detection
  - Vulkan on Linux
  - Direct3D 12 on Windows
  - Metal on macOS
  - OpenGL fallback for older systems

**Why:** Forward is simpler to start. bgfx abstracts platform differences.

### ECS
- **Library**: EnTT
  - Fast: cache-friendly component storage (SoA)
  - Modern: header-only, C++17
  - Proven: used in production games
  - Entity ID: 32-bit integer (4 billion entities)

**Why:** Don't reinvent ECS. EnTT is best-in-class.

### Physics
- **Library**: Jolt Physics
  - Modern: developed for Horizon Forbidden West
  - Fast: highly optimized, multi-threaded
  - 3D: full 3D physics support
  - Active development: frequent updates
  - Alternative: Box2D if building 2D-only engine

**Why:** Jolt is state-of-the-art for 3D. Box2D is proven for 2D.

### Audio
- **Library**: OpenAL
  - Cross-platform: Windows, Linux, macOS, mobile
  - 3D spatial audio: built-in attenuation, doppler effect
  - Well-supported: mature, stable API

**Why:** OpenAL is standard for game audio. Alternatives (FMOD, Wwise) are commercial.

### Scripting
- **Language**: Lua
  - Lightweight: small runtime footprint
  - Fast: JIT compilation via LuaJIT
  - Embeddable: designed for game engines
- **Binding**: sol2
  - Type-safe: compile-time type checking
  - Modern: C++17 API, template-based
  - Easy: minimal boilerplate

**Why:** Lua is industry standard for game scripting. sol2 makes C++ binding painless.

### Assets
- **Metadata format**: JSON
  - Human-readable: easy to debug, version control friendly
  - .meta files store GUID and import settings
- **GUID**: 128-bit UUID (Universally Unique Identifier)
  - Globally unique: collision probability ~0
  - Allows file moves without breaking references
- **Import cache**: Binary format
  - Stored in `assets/.cache/`
  - Optimized for loading speed
  - Rebuilt when source asset changes

**Why:** JSON for config, binary for runtime. GUIDs enable robust references.

---

## Priority Order (Next Steps)

**If you're ready to start coding, follow this exact order:**

### Immediate Priority (v0.1.0 - Core Foundation)
1. **ServiceLocator** - Foundation for all managers, must be first ✅
2. **TimeManager** - Delta time needed by all systems ✅
3. **Refactor Application** - Implement manager lifecycle ✅
4. **Event System** - Decouple systems early ✅
5. **Shader basics** - Get triangle on screen (motivation!)
6. **Transform matrices (GLM)** - Enable 3D positioning

### Short-term Priority (v0.1.0 - Rendering)
7. **InputManager** - Keyboard/mouse abstraction
8. **Camera + CameraManager** - View/projection matrices
9. **FPSCameraController** - Interactive camera navigation
10. **MeshManager** - Load and cache meshes
11. **ShaderManager** - Load and cache shaders
12. **MaterialManager** - Material instances
13. **RenderManager** - Batch and submit draw calls

### Medium-term Priority (v0.2.0 - Scene System)
14. **EnTT integration** - ECS foundation
15. **Entity wrapper** - Friendly API
16. **Core components** - Transform, MeshRenderer, Camera, Tag
17. **Scene** - Entity container with hierarchy
18. **SceneManager** - Load/unload scenes
19. **RenderSystem** - ECS-driven rendering
20. **CameraSystem** - ECS camera handling

### Long-term Priority (v0.2.0 - Assets)
21. **Asset base class** - Polymorphic asset interface
22. **AssetManager** - Unified loading and caching
23. **TextureAsset** - Load images
24. **MeshAsset** - Load models (Assimp)
25. **MaterialAsset** - Load materials
26. **Scene serialization** - Save/load scenes
27. **Unit Tests** - Core and math tests (start early!)

### Feature Complete (v0.3.0 - Advanced Systems)
28. **Textures & Model loading** - Complete asset pipeline
29. **Lighting** - Blinn-Phong or PBR
30. **Shadows** - Shadow mapping
31. **Particle System** - Visual effects
32. **Animation System** - Skeletal animation basics
33. **Physics (Jolt)** - Collision and simulation
34. **Audio (OpenAL)** - Sound effects and music
35. **Post-processing** - Bloom, tone mapping, AA
36. **Skybox** - Environment background

### Editor & Tools (v0.4.0)
37. **ImGui integration** - Editor foundation
38. **Editor windows** - Hierarchy, Inspector, Console, Asset Browser
39. **Gizmos** - Transform manipulation
40. **Editor Camera** - Scene navigation
41. **Play Mode** - Test without recompile
42. **Prefab System** - Reusable entity templates
43. **Integration Tests** - System integration verification

### Production Ready (v0.5.0)
44. **Runtime UI System** - In-game menus and HUD
45. **Animation State Machine** - Animator controller
46. **Build Pipeline** - Asset packaging
47. **Platform Export** - Standalone builds (Linux, Windows, macOS)
48. **Scripting (Lua)** - Gameplay logic (optional)

### Multiplayer (v0.6.0 - Optional)
49. **NetworkManager** - Connection handling
50. **State Synchronization** - Entity replication
51. **Client-Side Prediction** - Responsive controls
52. **Lag Compensation** - Fair hit detection
53. **Lobby System** - Player matchmaking

### Optimization (v0.7.0+)
54. **Profiling (Tracy)** - Performance measurement
55. **Frustum culling** - Visibility optimization
56. **Spatial partitioning** - Octree for queries
57. **Render batching** - Draw call reduction
58. **Multi-threading** - Parallel ECS systems
59. **Memory allocators** - Custom allocation strategies
60. **Performance Tests** - Regression prevention

---

## Build System

### Dependencies to Add Progressively

**Already Integrated:**
- [x] SDL2 - Windowing and input
- [x] bgfx - Rendering abstraction
- [x] bx - bgfx utility library
- [x] bimg - bgfx image library

**Phase 0-1 (v0.1.0 - Foundation):**
- [ ] GLM - Vector/matrix math

**Phase 2-3 (v0.2.0 - ECS):**
- [ ] EnTT - Entity-component system

**Phase 4 (v0.2.0 - Assets):**
- [ ] stb_image - Image loading (header-only, add to extern/)
- [ ] Assimp - 3D model loading
- [ ] nlohmann-json - JSON parsing/serialization

**Phase 5 (v0.3.0 - Advanced Systems):**
- [ ] Jolt Physics - 3D physics simulation
- [ ] OpenAL - 3D audio
- [ ] stb_truetype - Font rendering (header-only) OR FreeType

**Phase 6 (v0.4.0 - Editor):**
- [ ] Dear ImGui - Editor UI
- [ ] ImGuizmo - Transform gizmos

**Phase 7 (v0.5.0 - Build Pipeline):**
- [ ] LZ4 or Zstd - Asset compression

**Phase 8 (v0.5.0 - Scripting):**
- [ ] sol2 - Lua binding library
- [ ] Lua - Scripting language

**Phase 9 (v0.6.0 - Networking):**
- [ ] ENet - Simple reliable UDP OR
- [ ] GameNetworkingSockets - Valve's networking library (more robust)

**Phase 10 (v0.7.0 - Optimization):**
- [ ] Tracy - Profiler
- [ ] meshoptimizer - Mesh optimization (optional)

**Testing (Start at v0.2.0):**
- [ ] Google Test (gtest) - Unit testing framework
- [ ] Google Benchmark - Performance testing (optional)

---

## Notes & Best Practices

### Development Workflow
- **Commit often** - Small, atomic commits for each feature
- **Test incrementally** - Compile and test after each task
- **Write tests early** - Add unit tests as you implement features
- **Profile early** - Don't optimize prematurely, but measure performance
- **Document as you go** - Add comments for complex logic
- **Code review yourself** - Read your code before committing
- **CI from the start** - Set up GitHub Actions early, run tests on every push

### Architecture Guidelines
- **RAII everywhere** - No manual resource management
- **Const correctness** - Mark methods const where possible
- **No raw pointers** - Use smart pointers or handles
- **Single responsibility** - Each class does one thing
- **Dependency injection** - Prefer passing dependencies over globals

### Performance Tips
- **Measure first** - Profile before optimizing
- **Cache-friendly data** - Use SoA (Structure of Arrays) where possible
- **Batch operations** - Process in bulk instead of one-by-one
- **Avoid allocations** - Reuse memory, use object pools
- **Multithreading** - Parallelize independent work

### Debugging Tips
- **Assertions** - Add `KRIO_ASSERT()` macros for invariants
- **Logging** - Log important events, not every frame
- **Debug visualization** - Draw bounding boxes, frustums, etc.
- **Hot-reload** - Edit without restarting engine
- **Reproducible bugs** - Save scene state when bug occurs

### Documentation Resources
- **bgfx**: https://bkaradzic.github.io/bgfx/
- **EnTT**: https://github.com/skypjack/entt
- **GLM**: https://github.com/g-truc/glm
- **Jolt Physics**: https://github.com/jrouwe/JoltPhysics
- **Dear ImGui**: https://github.com/ocornut/imgui
- **Tracy Profiler**: https://github.com/wolfpld/tracy

---

**You now have a complete roadmap. Start with Phase 0, work through sequentially. Good luck building KrioEngine!**
