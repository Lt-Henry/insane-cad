
#main 
add_executable(insane-cad main.cpp Core.cpp Window.cpp View.cpp Mesh.cpp Color.cpp)

#require pkg-config support
find_package(PkgConfig REQUIRED)

#required pkg-config libs
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)


include_directories(${GTKMM_INCLUDE_DIRS})

target_link_libraries(insane-cad ${GTKMM_LIBRARIES} blaster)

#add custom lib flags
add_definitions(-std=c++11 -O3)



