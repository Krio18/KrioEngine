# KrioEngine

<div align="center">

  **A Modern C++ Game Engine - Early Development**

  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
  [![C++20](https://img.shields.io/badge/C++-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
  [![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](https://github.com/Krio/KrioEngine)

  *Clean architecture from day one*

</div>

---

## ⚠️ Development Status

**KrioEngine is in very early development (v0.1.0-alpha)**

Currently implemented:
- Cross-platform windowing (SDL2)
- Modern rendering backend (bgfx)
- Basic game loop with accurate delta time
- Logging system
- Clean modular architecture

This is a foundation project. Most features listed below are **planned** but not yet implemented.

---

## 🚀 Quick Start

### Prerequisites

- **CMake** 3.21 or higher
- **C++20** compatible compiler:
  - GCC 11+
  - Clang 13+
  - MSVC 19.29+ (Visual Studio 2019 16.11+)
- **vcpkg** package manager

### Installation

#### 1. Install vcpkg (if not already installed)

**Linux/macOS:**
```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)
```

**Windows (PowerShell):**
```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
$env:VCPKG_ROOT = $PWD
```

#### 2. Clone KrioEngine

```bash
git clone --recursive https://github.com/Krio/KrioEngine.git
cd KrioEngine
```

#### 3. Build

**Linux/macOS:**
```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j$(nproc)
```

**Windows:**
```powershell
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 🛠️ Technology Stack

| Component | Library | Status |
|-----------|---------|--------|
| **Windowing** | SDL2 | ✅ Integrated |
| **Rendering** | bgfx | ✅ Integrated |
| **Build System** | CMake + vcpkg | ✅ Working |
| **ECS** | EnTT | 📋 Planned |
| **Math** | GLM | 📋 Planned |
| **UI** | Dear ImGui | 📋 Planned |
| **Physics** | Jolt/Box2D | 📋 Planned |
| **Audio** | OpenAL | 📋 Planned |
| **Scripting** | sol2 (Lua) | 📋 Planned |

---

**Design Patterns:**
- **Modular Architecture**: Strict separation of concerns
- **RAII**: Automatic resource management
- **Data-Oriented Design**: Planned for ECS
- **Cross-platform Abstraction**: Platform layer isolates OS-specific code

---

## 🤝 Contributing

This is a personal learning project, but contributions are welcome!

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- [bgfx](https://github.com/bkaradzic/bgfx) by Branimir Karadzic - Amazing rendering abstraction
- [SDL2](https://www.libsdl.org/) - Cross-platform windowing
- [EnTT](https://github.com/skypjack/entt) (planned) by Michele Caini
- [Dear ImGui](https://github.com/ocornut/imgui) (planned) by Omar Cornut
