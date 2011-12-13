project( ExportFileTovtkPolyData )

cmake_minimum_required(VERSION 2.8)

find_package( VTK )

if( VTK_FOUND )
  include( ${VTK_USE_FILE} )
endif()

find_package( ITK )

if( ITK_FOUND )
  include( ${ITK_USE_FILE} )
endif()

add_executable( ExportFileTovtkPolyData ExportFileTovtkPolyData.cxx
  vtkPolyDataMySQLTextReader.cxx
  vtkPolyDataMySQLTextWriter.cxx
)
target_link_libraries( ExportFileTovtkPolyData ${VTK_LIBRARIES} ${ITK_LIBRARIES} )

