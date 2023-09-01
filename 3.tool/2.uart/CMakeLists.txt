cmake_minimum_required(VERSION 3.0.0)
project(Uart VERSION 0.1.0)

include(CTest)
enable_testing()

# # this one is important
# SET(CMAKE_SYSTEM_NAME Linux)
# #this one not so much
# SET(CMAKE_SYSTEM_VERSION 1)

# # specify the cross compiler
# SET(CMAKE_C_COMPILER   arm-oe-linux-gnueabi-gcc)
# SET(CMAKE_CXX_COMPILER arm-oe-linux-gnueabi-g++)

# # search for programs in the build host directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# # for libraries and headers in the target directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_executable(Uart 
    src/bts_debug.cpp
    src/bts_frame_message.cpp
    src/bts_get_message.cpp
    src/handle_msg.cpp
    src/bts_serialport.cpp
    src/file.cpp
    src/main.cpp
)
target_include_directories(Uart PUBLIC include)
target_link_libraries(Uart)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)