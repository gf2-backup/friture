project( Friture )

cmake_minimum_required( VERSION 2.6 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( directories
  ApplyGaussian
  BinaryDilateImageFilter
  BinaryErodeImageFilter
  BinaryThreshold
  ComputeCentroid
  ExportFileTovtkPolyData
  FastMarching
  GaussianSmoothing
  KMeans
  LargestConnectedComponent
  MegaCaptureToMHA
  AssignTrackToMembrane
)

foreach( dir ${directories} )
  add_subdirectory( ${dir} )
endforeach()

configure_file( ${CMAKE_SOURCE_DIR}/Scripts/EmbryoBinaryMask.sh.in
  ${CMAKE_BINARY_DIR}/EmbryoBinaryMask.sh
  @ONLY
)

configure_file( ${CMAKE_SOURCE_DIR}/Scripts/PreprocessSpeedImage.sh.in
  ${CMAKE_BINARY_DIR}/PreprocessSpeedImage.sh
  @ONLY
)


