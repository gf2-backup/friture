/*=========================================================================
  Author: $Author: nr52 $  // Author of last commit
  Version: $Rev: 1473 $  // Revision of last commit
  Date: $Date: 2010-05-18 17:31:00 -0400 (Tue, 18 May 2010) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009

 Copyright (c) 2009, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"

//generate spheres for testing
//#include "vtkSphereSource.h"

int main ( int argc, char* argv[] )
{
/*  vtkSmartPointer<vtkSphereSource> sphere1 = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphere1->SetRadius(5);
 
  // create sphere
  vtkSmartPointer<vtkPolyDataWriter> writer1 =
    vtkSmartPointer<vtkPolyDataWriter>::New();
  writer1->SetInput( sphere1->GetOutput() );
  writer1->SetFileName( "radius5.vtk" );
  writer1->Write();

  vtkSmartPointer<vtkSphereSource> sphere2 = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphere2->SetRadius(10);
 
  // create sphere
  vtkSmartPointer<vtkPolyDataWriter> writer2 =
    vtkSmartPointer<vtkPolyDataWriter>::New();
  writer2->SetInput( sphere2->GetOutput() );
  writer2->SetFileName( "radius10.vtk" );
  writer2->Write();
*/
  if ( argc < 3 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " membrane.vtk nuclei.vtk" << std::endl;
    return EXIT_FAILURE;
  }

  // read membrane
  vtkSmartPointer<vtkPolyDataReader> membraneReader =
    vtkSmartPointer<vtkPolyDataReader>::New();
  membraneReader->SetFileName( argv[1] );
  membraneReader->Update();
  // read nuclei
  vtkSmartPointer<vtkPolyDataReader> nucleiReader =
    vtkSmartPointer<vtkPolyDataReader>::New();
  nucleiReader->SetFileName( argv[2] );
  nucleiReader->Update();

  // compare bounding boxes
  bool inside = true;
  double* membrane = membraneReader->GetOutput()->GetBounds();
  double* nuclei = nucleiReader->GetOutput()->GetCenter();
    
  //std::cout << "nuclei: " << nuclei[0] << " : " << nuclei[1] << " : " << nuclei[2] << std::endl;
  //std::cout << "membrane x: " << membrane[0] << " : "<< membrane[1] << std::endl;
  //std::cout << "membrane y: " << membrane[2] << " : " << membrane[3] << std::endl;
  //std::cout << "membrane z: " << membrane[4] << " : " << membrane[5] << std::endl;

  // if outside
  if(  nuclei[0] < membrane[0] || nuclei[0] > membrane[1]  // X
    || nuclei[1] < membrane[2] || nuclei[1] > membrane[3]  // Y
    || nuclei[2] < membrane[4] || nuclei[2] > membrane[5]) // Z
    {
    inside = false;
    }

  if(inside)
    {
    // char* to string
    std::string path = argv[2];
    // extract file name (DOESNT WORK ON WINDOWS)
    size_t found = path.find_last_of("/");
    // remove path
    std::string filename = path.substr(found+1);
    std::cout << "filename: " << filename << std::endl;
    
    // write polydata
    vtkSmartPointer<vtkPolyDataWriter> writer =
      vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetInput( membraneReader->GetOutput() );
    writer->SetFileName( filename.c_str() );
    writer->Write();
    
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }

  }
