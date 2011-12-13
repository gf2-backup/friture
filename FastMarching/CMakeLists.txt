project( fmm )

cmake_minimum_required( VERSION 2.6 )

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

add_executable( fmm fmm.cxx )
target_link_libraries( fmm ${ITK_LIBRARIES} )
