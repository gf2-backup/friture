project( GenerateModelsFromLabels )

cmake_minimum_required( VERSION 2.6 )

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

find_package( VTK )

if( VTK_FOUND )
  include( ${VTK_USE_FILE} )
endif()

add_executable( GenerateModelsFromLabels GenerateModelsFromLabels.cxx )
target_link_libraries( GenerateModelsFromLabels
  ${ITK_LIBRARIES}
  ${VTK_LIBRARIES}
)
