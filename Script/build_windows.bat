@echo off

call 

cd ..
cmake -S . -B build -G Ninja -DCMAKE_RC_COMPILER="Ninja" -DCMAKE_TOOLCHAIN_FILE:STRING="C:/tools/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_C_COMPILER="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/clang.exe"
cmake --build build --parallel --config Release --target all --verbose

cd Script








