#include "vtkSmartPointer.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyData.h"

int main( int argc, char* argv[] )
{
  vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  vtkSmartPointer< vtkPolyData > mesh = reader->GetOutput();

  double centroid[3];
  mesh->GetCenter( centroid );

  std::cout << centroid[0] << "  " << centroid[1] << "  " << centroid[2] << std::endl;
  return EXIT_SUCCESS;
}
