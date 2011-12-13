project( kishore )

cmake_minimum_required( VERSION 2.6 )

set( directories
  ComputeCentroid
  ExportFileTovtkPolyData
  FastMarching
  MegaCaptureToMHA
)

foreach( dir ${directories} )
  add_subdirectory( ${dir} )
endforeach()

