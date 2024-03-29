cmake_minimum_required (VERSION 3.1)

project (ib-client)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")


source_group("" FILES ${SOURCES} ${HEADERS}) 
source_group("Source Files" FILES	"src/main.cpp") 
source_group("Source Files\\client" FILES "src/client.h" "src/client.cpp")
source_group("Source Files\\contracts" FILES "src/contracts.h")

# OpenCV and DGM packages
#find_package(OpenCV 4.0 REQUIRED core features2d highgui imgproc imgcodecs ml PATHS "$ENV{OPENCVDIR}/build")
#find_package(DGM 1.7 REQUIRED PATHS "$ENV{DGMDIR}/build/install")

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Sets
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD 14)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /wd5040") 
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF")
endif(MSVC)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
set(IB_API_INCLUDE_DIR "" CACHE PATH "Path to the IP API")

# Properties -> C/C++ -> General -> Additional Include Directories
include_directories(${IB_API_INCLUDE_DIR} ${IB_API_INCLUDE_DIR}/client)

#definitions
add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)

add_executable(ib-client ${SOURCES} ${HEADERS} )

# Properties -> Linker -> Input -> Additional Dependencies
#target_link_libraries(ib-client ${IB_API_INCLUDE_DIR}/lib/TwsSocketClient.lib)
file(GLOB MYLIB ${IB_API_INCLUDE_DIR}/client/*.cpp)
add_library(TwsSocketClient STATIC ${MYLIB})
target_link_libraries(ib-client TwsSocketClient)