project( BinaryDilateImageFilter )

cmake_minimum_required( VERSION 2.6 )

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

add_executable( BinaryDilateImageFilter BinaryDilateImageFilter.cxx )
target_link_libraries( BinaryDilateImageFilter ${ITK_LIBRARIES} )
