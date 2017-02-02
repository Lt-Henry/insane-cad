
# ***  insane cad  ****
cmake_minimum_required (VERSION 2.8)
project (insane-cad)

#add_subdirectory(rsrc)
add_subdirectory(libs/blaster)
include_directories(libs/blaster/include/)

include_directories(include)
add_subdirectory(src)


