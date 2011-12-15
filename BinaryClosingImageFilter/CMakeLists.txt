project( BinaryClosingImageFilter )

cmake_minimum_required( VERSION 2.6 )

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

add_executable( BinaryClosingImageFilter BinaryClosingImageFilter.cxx )
target_link_libraries( BinaryClosingImageFilter ${ITK_LIBRARIES} )
