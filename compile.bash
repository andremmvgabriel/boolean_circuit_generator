cd build

cmake ..
cmake --build . --target ALL_BUILD --config Release -- /maxcpucount:12

cd ..
