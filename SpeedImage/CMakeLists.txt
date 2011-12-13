cmake_minimum_required(VERSION 2.6)
 
project(SpeedImage)
 
find_package(ITK)

if( ITK_FOUND )
  include(${ITK_USE_FILE})
endif()
 
add_executable(SpeedImage SpeedImage.cxx)
target_link_libraries(SpeedImage
  ${ITK_LIBRARIES})
