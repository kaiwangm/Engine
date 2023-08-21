# Engine

[![GitHub release (latest SemVer)](https://img.shields.io/github/v/release/kaiwangm/Engine?style=flat-square)](https://github.com/kaiwangm/Engine/releases)
[![GitHub Workflow Status (branch)](https://img.shields.io/github/workflow/status/kaiwangm/Engine/Windows%20CI/main?style=flat-square)](https://github.com/kaiwangm/Engine/actions/workflows/Windows_CI.yml)
[![GitHub](https://img.shields.io/github/license/kaiwangm/Engine?style=flat-square)](https://github.com/kaiwangm/Engine/blob/main/LICENSE)

My toy modern OpenGL game engine.

![](https://raw.githubusercontent.com/kaiwangm/Engine/main/Docs/Screenshots/MoonlessBistro00.png)

## Getting Started

1. Clone the repo.

```
git clone https://github.com/kaiwangm/Engine.git
```

2. install dependencies.

```
git clone https://github.com/microsoft/vcpkg
.\vcpkg\bootstrap-vcpkg.bat
```

3. Build.
```
cd Engine
cmake -S . -B build -G Ninja
cmake --build build --parallel --config Release --target all
```

## Reference

1. Hazel Engine [Github](https://github.com/TheCherno/Hazel)






























