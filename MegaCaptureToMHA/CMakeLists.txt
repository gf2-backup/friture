project( MegaCaptureToMHA )

cmake_minimum_required(VERSION 2.8)

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

add_executable( MegaCaptureToMHA
  MegaCaptureToMHA.cxx
  )
target_link_libraries( MegaCaptureToMHA ${ITK_LIBRARIES} )


