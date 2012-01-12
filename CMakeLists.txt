project( Friture )

cmake_minimum_required( VERSION 2.6 )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

set( directories
  ApplyGaussian
  AddGaussianToImage
  BinaryDilateImageFilter
  BinaryErodeImageFilter
  BinaryThreshold
  ComputeCentroid
  Decimate
  Extract
  ExportFileTovtkPolyData
  FastMarching
  FMMResultsToLabelMap
  FlipImage
  Gaussian4D
  GaussianSmoothing
  GenerateModelsFromLabels
  KMeans
  LargestConnectedComponent
  MegaCaptureToMHA
  PadImageFilter
  SegmentationToImportFile
  SeriesImage4D
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


