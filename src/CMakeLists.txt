set(BINARY ${CMAKE_PROJECT_NAME}_src)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()  # Prepares the CMakeList.txt for Conan.


include_directories(../include)


file(GLOB SOURCES *.cpp)
set(SOURCES ${SOURCES})

add_executable(Robotcar_src ${SOURCES})
add_library(Robotcar_src_lib STATIC ${SOURCES})

target_link_libraries(Robotcar_src PUBLIC ${CONAN_LIBS})
