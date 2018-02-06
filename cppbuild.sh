#!/bin/bash

# There are some major hardcodings in here, but that will eventually be ironed out.
IMBS_VERSION=3.1
PLATFORM=windows-x86_64

# copy all required files to the cppbuild dir.
mkdir cppbuild
cp *.cpp *.hpp *.h CMakeLists.txt cppbuild
cd cppbuild

# Compiling to create a .lib, ready to be included
cmake -DCMAKE_CONFIGURATION_TYPES=Release -DOpenCV_DIR="C:/dev/opencv/build" -G "Visual Studio 12 2013 Win64"
msbuild.exe imbs.sln -p:PlatformToolset=v120 -p:PreferredToolArchitecture=x64
mv Release/imbsLib.lib ../imbs_310.lib
mv Release/imbs.dll ../imbs_310.dll