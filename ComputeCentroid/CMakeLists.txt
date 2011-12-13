project( ComputeCentroid )

cmake_minimum_required( VERSION 2.6 )

find_package( VTK )

if( VTK_FOUND )
  include( ${VTK_USE_FILE} )
endif()

add_executable( ComputeCentroid ComputeCentroid.cxx )
target_link_libraries( ComputeCentroid ${VTK_LIBRARIES} )

