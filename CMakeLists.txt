cmake_minimum_required(VERSION 2.8)
project(imbs)
add_executable (imbs main.cpp imbs.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIR})

# add_library (imbs SHARED imbs.cpp)
add_library (imbsLib imbs.cpp)

target_link_libraries (imbs ${OpenCV_LIBS})
target_link_libraries (imbsLib ${OpenCV_LIBS})


 set(CMAKE_BUILD_TYPE Release)
 set(CMAKE_CXX_FLAGS "/Zi /favor:INTEL64 /EHsc /arch:AVX /fp:fast /Gw /Qpar /Qpar-report:1 /Qvec-report:2")

 # Trying to get pdb files to be generated always, even if we are doing a release, to be able to profile.
 # based on info from https://msdn.microsoft.com/en-us/library/fsk896zz.aspx
 # also include to Zi flag above.
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL:NO /DEBUG /OPT:REF /OPT:ICF")