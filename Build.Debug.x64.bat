@echo off

if not exist "shaders" mkdir "shaders"
cd shaders
if not exist "bin" mkdir "bin"
cd ../
if not exist "bin" mkdir "bin"
cd bin
if not exist "intermediate" mkdir "intermediate"
if not exist "native" mkdir "native"
cd native
if not exist "Debug-x64" mkdir "Debug-x64"
cd ../
cd intermediate
if not exist "dx11e" mkdir "dx11e"
cd dx11e
if not exist "Debug-x64" mkdir "Debug-x64"
cd ../
if not exist "hw3d" mkdir "hw3d"
cd hw3d
if not exist "Debug-x64" mkdir "Debug-x64"

cd ../../../

cd dx11e

make -f Makefile.Debug.x64

cd ../hw3d
make -f Makefile.Debug.x64
