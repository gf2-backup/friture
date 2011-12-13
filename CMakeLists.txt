project( kishore )

cmake_minimum_required( VERSION 2.6 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( directories
  ComputeCentroid
  ExportFileTovtkPolyData
  FastMarching
  MegaCaptureToMHA
)

foreach( dir ${directories} )
  add_subdirectory( ${dir} )
endforeach()

