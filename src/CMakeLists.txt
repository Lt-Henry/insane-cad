
#main 
add_executable(insane-cad main.cpp Core.cpp Window.cpp View.cpp Raster.cpp Math.cpp Mesh.cpp Color.cpp Vbo.cpp)

#performance tests
add_executable(benchmark benchmark.cpp Raster.cpp Math.cpp Color.cpp Vbo.cpp)

#require pkg-config support
find_package(PkgConfig REQUIRED)

#required pkg-config libs
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)


include_directories(${GTKMM_INCLUDE_DIRS})

target_link_libraries(insane-cad ${GTKMM_LIBRARIES})

#add custom lib flags
add_definitions(-std=c++11 -O3)



