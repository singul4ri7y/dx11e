@echo off

if not exist "bin" mkdir "bin"
cd bin
if not exist "shaders" mkdir "shaders"
cd shaders
if not exist "bin" mkdir "bin"
cd ../
if not exist "intermediate" mkdir "intermediate"
cd intermediate
if not exist "dx11e" mkdir "dx11e"
cd dx11e
if not exist "Release-x64" mkdir "Release-x64"
cd ../
if not exist "hw3d" mkdir "hw3d"
cd hw3d
if not exist "Release-x64" mkdir "Release-x64"
cd ../../
if not exist "dx11e" mkdir "dx11e"
cd dx11e
if not exist "Release-x64" mkdir "Release-x64"
cd ../
if not exist "hw3d" mkdir "hw3d"
cd hw3d
if not exist "Release-x64" mkdir "Release-x64"

cd ../../

cd dx11e

mingw32-make -f Makefile.Release.x64

cd ../hw3d
mingw32-make -f Makefile.Release.x64