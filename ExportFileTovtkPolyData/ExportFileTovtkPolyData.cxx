/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
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

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVector.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "vtkPolyData.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"
#include "vtkIntArray.h"
#include "vtkFieldData.h"
#include "vtkPolyDataMySQLTextReader.h"
#include "vtkPolyDataMySQLTextWriter.h"
#include "itkGoFigure2TableToSegmentation.h"

#include <vnl/vnl_vector.h>

int main ( int argc, char* argv[] )
{
  if ( argc < 3 )
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " importFile cellTypeId" << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 3;
  typedef itk::Image< unsigned char, Dimension > FeatureImageType;
  typedef itk::Image< float, Dimension >         InputImageType;
  typedef itk::Image< unsigned int, Dimension >  SegmentImageType;

  typedef itk::ImageFileReader< FeatureImageType > FeatureReaderType;
  typedef itk::ImageFileReader< SegmentImageType > SegmentReaderType;
  typedef vnl_vector< unsigned int > VectorType;
  typedef itk::GoFigure2TableToSegmentation< SegmentImageType > FilterType;
  typedef vnl_vector< unsigned int > vnlVectorType;
  std::map< int, int > coordMap;

  // Update the contents of the filter
  FilterType::Pointer filter = FilterType::New();

  // Open and read the import file till end
  std::ifstream importFile( argv[1] );
  std::string fname, fname2;
  int coordID = 0, colorID;
  unsigned int timePt = 0, trackID, meshID;
  std::string line;
  if (importFile.is_open())
  {
    filter->PrintStart( importFile, 0 );
    filter->PrintColor( importFile, 2 );
    filter->PrintCell( importFile, 2 );
    filter->PrintSubCellular( importFile, 2 );
    filter->PrintCoordinate( importFile, 2 );
    filter->PrintLineage( importFile, 2 );
    filter->PrintTrack( importFile, 2 );
    filter->PrintMesh( importFile, 2 );
    filter->PrintChannel( importFile, 2 );
    filter->PrintIntensity( importFile, 2 );
    filter->PrintEnd( importFile, 0 );

    // Fill a map of coord ids since they are not 0 indexed
    // in GoFigure2 output
    for( unsigned int i = 0; i < filter->m_Coordinates.size(); i++ )
    {
      coordID = filter->m_Coordinates[i].CoordID;
      coordMap[coordID] = i;
    }

    // Find the largest time-point
    unsigned int m_NumberOfTimePoints = 0;
    for( unsigned int i = 0; i < filter->m_Meshes.size(); i++ )
    {
      coordID = filter->m_Meshes[i].CoordIDMin;
      coordID = coordMap[coordID];
      timePt = filter->m_Coordinates[coordID].TCoord;
      if ( timePt > m_NumberOfTimePoints )
      {
        m_NumberOfTimePoints = timePt;
      }
    }

    std::cout << "Number of TimePoints: " << m_NumberOfTimePoints << std::endl;

    vnlVectorType ConsecutiveMeshIdLookup;
    ConsecutiveMeshIdLookup.set_size( m_NumberOfTimePoints + 1 );
    for ( timePt = 0; timePt <= m_NumberOfTimePoints; timePt++ )
    {
      ConsecutiveMeshIdLookup[timePt] = 1;
    }

    for( unsigned int i = 0; i < filter->m_Meshes.size(); i++ )
    {
      vtkPolyDataMySQLTextReader* convert_reader = vtkPolyDataMySQLTextReader::New();
      convert_reader->SetIsContour( false );
      vtkPolyData* output = convert_reader->GetPolyData( filter->m_Meshes[i].Points );

      coordID = filter->m_Meshes[i].CoordIDMin;
      coordID = coordMap[coordID];

      timePt = filter->m_Coordinates[coordID].TCoord;
      meshID = filter->m_Meshes[i].MeshID;
      trackID = filter->m_Meshes[i].TrackID;
      colorID = filter->m_Meshes[i].ColorID;

      // update vtkFilename as timePt_meshId.vtk
//       if ( filter->m_Meshes[i].CellTypeID == atoi(argv[2]) )
    if( timePt == atoi( argv[2] ) )
      {
      std::ostringstream istream;
      istream << timePt << "_" << ConsecutiveMeshIdLookup[timePt] << ".vtk";
      fname = istream.str();
      std::cout << fname << ' ' << filter->m_Meshes[i].MeshID << ' ' << filter->m_Meshes[i].ColorID << std::endl;

      // add track ID inside the vtk polydata
      // polydata->GetPointData()->GetArray("trackID") to access this information
      // create the array
      vtkSmartPointer<vtkIntArray> trackIDArray = vtkSmartPointer<vtkIntArray>::New();
      trackIDArray->SetNumberOfComponents(1);
      trackIDArray->SetNumberOfValues(1);
      trackIDArray->SetName("trackID");
      trackIDArray->SetValue(0, trackID);
      // add value in the polydata
      output->GetFieldData()->AddArray(trackIDArray);

      vtkSmartPointer<vtkPolyDataWriter> writer =
        vtkSmartPointer<vtkPolyDataWriter>::New();
      writer->SetInput( output );
      writer->SetFileName( fname.c_str() );
      writer->Write();

      // update trackId as timePt.txt
      std::ostringstream istream2;
      istream2 << timePt << ".txt";
      fname2 = istream2.str();
      std::cout << fname2 << std::endl;

      ofstream trackFile ( fname2.c_str(), ios::app );
      if ( trackFile.is_open() )
        {
        trackFile << trackID << std::endl;
        }
      trackFile.close();

      ConsecutiveMeshIdLookup[timePt]++;
      }
    }

    importFile.close();
  }

  return EXIT_SUCCESS;
  }
