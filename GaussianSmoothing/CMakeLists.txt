cmake_minimum_required(VERSION 2.6)
 
project(GaussianSmoothing)
 
find_package(ITK)

if( ITK_FOUND )
  include(${ITK_USE_FILE})
endif()
 
add_executable(GaussianSmoothing GaussianSmoothing.cxx)
target_link_libraries(GaussianSmoothing
  ${ITK_LIBRARIES})
