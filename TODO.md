# KrioEngine - TODO List

## 📋 Version 0.1.0 - Core Foundation (Current)

### 🔴 Critical - Input System
- [ ] Créer `src/Platform/Input.hpp` et `Input.cpp`
- [ ] Implémenter détection clavier (WASD, flèches, etc.)
- [ ] Implémenter détection souris (position, boutons, scroll)
- [ ] Créer système d'événements pour les inputs
- [ ] Ajouter support gamepad/manette (optionnel)

### 🔴 Critical - Camera System
- [ ] Créer `src/Renderer/Camera.hpp` et `Camera.cpp`
- [ ] Implémenter caméra perspective 3D
- [ ] Implémenter caméra orthographique 2D
- [ ] Ajouter matrices view/projection
- [ ] Créer contrôleur de caméra (FPS, orbital)

### 🔴 Critical - Basic Rendering
- [ ] Créer `src/Renderer/Shader.hpp` et `Shader.cpp`
- [ ] Compiler shaders vertex/fragment basiques
- [ ] Créer `src/Renderer/Mesh.hpp` et `Mesh.cpp`
- [ ] Implémenter rendu d'un triangle
- [ ] Implémenter rendu d'un quad/rectangle
- [ ] Implémenter rendu d'un cube

### 🟡 Important - ECS Integration (EnTT)
- [ ] Ajouter EnTT dans `vcpkg.json`
- [ ] Créer `src/ECS/Registry.hpp`
- [ ] Créer composants de base:
  - [ ] `Transform` (position, rotation, scale)
  - [ ] `MeshRenderer` (mesh + material)
  - [ ] `Camera` (intégrer avec système caméra)
- [ ] Créer systèmes de base:
  - [ ] `RenderSystem` (dessiner toutes les entités avec MeshRenderer)
  - [ ] `TransformSystem` (calcul matrices model)
- [ ] Intégrer ECS dans la game loop

### 🟡 Important - Math Library (GLM)
- [ ] Ajouter GLM dans `vcpkg.json`
- [ ] Créer `src/Math/Transform.hpp`
- [ ] Wrapper pour vec2/vec3/vec4
- [ ] Wrapper pour mat4
- [ ] Fonctions utilitaires (lerp, clamp, etc.)

---

## 📋 Version 0.2.0 - Assets & Visuals

### 🔴 Critical - Texture System
- [ ] Créer `src/Renderer/Texture.hpp` et `Texture.cpp`
- [ ] Ajouter stb_image dans `extern/`
- [ ] Charger textures PNG/JPG
- [ ] Gérer wrapping/filtering (clamp, repeat, linear, nearest)
- [ ] Créer atlas de textures (optionnel)

### 🔴 Critical - Model Loading
- [ ] Ajouter Assimp dans `vcpkg.json`
- [ ] Créer `src/Assets/ModelLoader.hpp` et `ModelLoader.cpp`
- [ ] Charger fichiers OBJ
- [ ] Charger fichiers FBX/GLTF (optionnel)
- [ ] Parser matériaux basiques

### 🟡 Important - Material System
- [ ] Créer `src/Renderer/Material.hpp` et `Material.cpp`
- [ ] Propriétés: albedo, metallic, roughness
- [ ] Support textures diffuse/normal/specular
- [ ] Système de shader variants

### 🟡 Important - Lighting
- [ ] Créer composant `DirectionalLight`
- [ ] Créer composant `PointLight`
- [ ] Créer composant `SpotLight`
- [ ] Implémenter Blinn-Phong lighting
- [ ] Implémenter PBR lighting (optionnel)

### 🟢 Nice to have - Scene Management
- [ ] Créer `src/Scene/Scene.hpp` et `Scene.cpp`
- [ ] Hiérarchie d'entités (parent/child)
- [ ] Charger/sauvegarder scènes
- [ ] Scene graph traversal

---

## 📋 Version 0.3.0 - Editor & Tools

### 🔴 Critical - ImGui Integration
- [ ] Ajouter Dear ImGui dans `vcpkg.json`
- [ ] Intégrer ImGui avec bgfx
- [ ] Créer `src/Editor/EditorLayer.hpp` et `EditorLayer.cpp`
- [ ] Fenêtre Viewport (rendu de la scène)
- [ ] Fenêtre Hierarchy (liste des entités)
- [ ] Fenêtre Inspector (propriétés de l'entité sélectionnée)
- [ ] Gizmos de transformation (translate/rotate/scale)

### 🟡 Important - Serialization
- [ ] Ajouter nlohmann/json dans `vcpkg.json`
- [ ] Créer `src/Serialization/SceneSerializer.hpp`
- [ ] Sérialiser entités en JSON
- [ ] Désérialiser entités depuis JSON
- [ ] Sauvegarder/charger scènes complètes

### 🟢 Nice to have - Asset Browser
- [ ] Fenêtre ImGui pour parcourir `assets/`
- [ ] Prévisualisation des textures
- [ ] Drag & drop de modèles dans la scène
- [ ] Import d'assets externes

---

## 📋 Version 0.4.0 - Physics & Audio

### 🟡 Important - Physics (Jolt Physics)
- [ ] Évaluer Jolt vs Box2D
- [ ] Ajouter bibliothèque physique dans `extern/`
- [ ] Créer `src/Physics/PhysicsWorld.hpp`
- [ ] Composants `RigidBody` et `Collider`
- [ ] Intégrer mise à jour physique dans game loop
- [ ] Détection de collisions
- [ ] Raycast

### 🟡 Important - Audio (OpenAL)
- [ ] Ajouter OpenAL dans `vcpkg.json`
- [ ] Créer `src/Audio/AudioEngine.hpp`
- [ ] Charger fichiers WAV/OGG
- [ ] Jouer sons 2D (UI, musique)
- [ ] Jouer sons 3D (effets positionnés dans l'espace)
- [ ] Contrôle volume/pitch

---

## 📋 Version 0.5.0 - Scripting & Advanced

### 🟢 Nice to have - Lua Scripting (sol2)
- [ ] Ajouter sol2 et Lua dans `vcpkg.json`
- [ ] Créer `src/Scripting/LuaEngine.hpp`
- [ ] Binder classes C++ vers Lua
- [ ] Composant `LuaScript`
- [ ] Callbacks: `onStart()`, `onUpdate(dt)`, `onDestroy()`
- [ ] Accès API engine depuis Lua

### 🟢 Nice to have - Spatial Partitioning
- [ ] Créer `src/Spatial/Quadtree.hpp` (2D)
- [ ] Créer `src/Spatial/Octree.hpp` (3D)
- [ ] Utiliser pour frustum culling
- [ ] Utiliser pour requêtes spatiales rapides

### 🟢 Nice to have - Profiling (Tracy)
- [ ] Intégrer Tracy profiler
- [ ] Macros `KRIO_PROFILE_SCOPE()`
- [ ] Mesurer performance render/update/physics
- [ ] Visualiser frame time

---

## 🔧 Technical Debt & Improvements

### Code Quality
- [ ] Remplacer singleton `Application::_instance` par injection de dépendances
- [ ] Créer système d'événements (Event bus)
- [ ] Ajouter assertions avec macros `KRIO_ASSERT()`
- [ ] Créer smart pointers custom (Ref<T>) si nécessaire

### Build System
- [ ] Créer presets CMake pour Debug/Release/RelWithDebInfo
- [ ] Ajouter support Ninja generator
- [ ] Configurer CCache pour compilation plus rapide
- [ ] Script `build.sh` / `build.bat` pour simplifier

### Testing
- [ ] Ajouter Catch2 dans `vcpkg.json`
- [ ] Créer `tests/` directory
- [ ] Tests unitaires pour Math
- [ ] Tests unitaires pour ECS
- [ ] Tests d'intégration

### Documentation
- [ ] Documenter architecture dans `docs/Architecture.md`
- [ ] Documenter conventions de code dans `docs/CodingStyle.md`
- [ ] Créer exemples d'utilisation dans `examples/`
- [ ] Ajouter diagrammes UML (optionnel)

---

## 📦 External Dependencies Roadmap

### À ajouter progressivement:
1. **v0.1**: EnTT, GLM
2. **v0.2**: stb_image, Assimp
3. **v0.3**: Dear ImGui, nlohmann-json
4. **v0.4**: Jolt Physics (ou Box2D), OpenAL
5. **v0.5**: sol2, Lua, Tracy

### Déjà intégrés:
- ✅ SDL2 (windowing)
- ✅ bgfx (rendering)

---

## 🎯 Priorités Immédiates (Next Steps)

**Si tu veux coder maintenant, commence par:**

1. **Input System** - Nécessaire pour toute interactivité
2. **Camera System** - Nécessaire pour voir quelque chose de différent
3. **Triangle Rendering** - Première chose visible à l'écran
4. **ECS Integration** - Architecture pour le reste du projet

**Ordre recommandé:**
```
Input → Camera → Triangle → Cube → ECS → Textures → Model Loading → ImGui
```

---

## 📝 Notes

- **Ne pas utiliser d'IA** pour le code après ce point (comme demandé)
- **Garder l'architecture modulaire**: chaque système dans son propre dossier
- **Tester régulièrement**: compiler après chaque feature
- **Committer souvent**: petits commits atomiques
- **Consulter docs bgfx**: https://bkaradzic.github.io/bgfx/overview.html
- **Consulter docs EnTT**: https://github.com/skypjack/entt

---

**Dernière mise à jour:** 2025-11-07
**Version du projet:** 0.1.0-alpha
