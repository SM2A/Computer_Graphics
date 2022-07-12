mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cd ..
cmake --build .\build\ --target Bicycle --config Debug
copy build\Bicycle.exe Bicycle.exe