/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-11

 Copyright (c) 2009-11, President and Fellows of Harvard College.
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "itkImageFileReader.h"
#include "itkMegaCaptureToMHA.h"
#include "itkTileImageReader3D.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"

int main(int argc, char **argv)
{
  if ( argc < 3)
    {
    std::cerr << "megacapture header (.meg) nucleiDir membraneDir" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::MegacaptureToMha FilterType;

  const unsigned int Dimension = 3;
  typedef itk::Image< unsigned char, Dimension > ImageType;

  typedef itk::TileImageReader3D< ImageType > TileReader3DType;
  typedef TileReader3DType::Pointer TileReader3DPointer;
  typedef itk::ImageFileWriter< ImageType > WriterType;
  typedef itk::NumericSeriesFileNames NameGeneratorType;

  std::ifstream importFile( argv[1] );
  std::string line( argv[1] );
  std::string prefix = line.substr( 0, line.length() - 4 );

  double m_SpacingX;
  double m_SpacingY;
  double m_SpacingZ;
  double m_DimensionX;
  double m_DimensionY;
  double m_TimeInterval;
  std::string m_FileType;
  unsigned int m_NumberOfZSlices;
  unsigned int m_NumberOfTimePoints;
  unsigned int m_NumberOfChannels;

  // Update the contents of the filter
  if (importFile.is_open())
  {
    FilterType::Pointer filter = FilterType::New();
    filter->PrintStart( importFile );

    std::cout << "Spacing: " << filter->GetSpacingX() << ' ' << filter->GetSpacingY() << ' ' << filter->GetSpacingZ() << std::endl;
    std::cout << "Dimension: " << filter->GetDimensionX() << ' ' << filter->GetDimensionY() << std::endl;
    std::cout << "NumberOfTimePoints: " << filter->GetNumberOfTimePoints() << std::endl;
    std::cout << "NumberOfZSlices: " << filter->GetNumberOfZSlices() << std::endl;
    std::cout << "NumberOfChannels: " << filter->GetNumberOfChannels() << std::endl;
    std::cout << "FileType: " << filter->GetFileType() << std::endl;
    std::cout << "TimeInterval: " << filter->GetTimeInterval() << std::endl;

    m_SpacingX = filter->GetSpacingX();
    m_SpacingY = filter->GetSpacingY();
    m_SpacingZ = filter->GetSpacingZ();
    m_DimensionX = filter->GetDimensionX();
    m_DimensionY = filter->GetDimensionY();
    m_NumberOfTimePoints = filter->GetNumberOfTimePoints();
    m_NumberOfZSlices =  filter->GetNumberOfZSlices();
    m_NumberOfChannels =  filter->GetNumberOfChannels();
    m_FileType =  "PNG"; //filter->GetFileType();
    m_TimeInterval =  filter->GetTimeInterval();

    ImageType::RegionType region;
    ImageType::IndexType index;
    ImageType::SizeType size;
    ImageType::SpacingType spacing;

    index[0] = 0;
    index[1] = 0;
    index[2] = 0;

    size[0] = m_DimensionX;
    size[1] = m_DimensionY;
    size[2] = m_NumberOfZSlices;

    region.SetIndex( index );
    region.SetSize( size );

    spacing[0] = m_SpacingX;
    spacing[1] = m_SpacingY;
    spacing[2] = m_SpacingZ;

    NameGeneratorType::Pointer nameGenerator1 = NameGeneratorType::New();
    nameGenerator1->SetSeriesFormat( "-PL00-CO00-RO00-ZT00-YT00-XT00-TM%04d" );
    nameGenerator1->SetStartIndex( 0 );
    nameGenerator1->SetEndIndex( m_NumberOfTimePoints-1 );
    nameGenerator1->SetIncrementIndex( 1 );

    NameGeneratorType::Pointer nameGenerator2 = NameGeneratorType::New();
    nameGenerator2->SetSeriesFormat( "-ch%02d" );
    nameGenerator2->SetStartIndex( 0 );
    nameGenerator2->SetEndIndex( m_NumberOfChannels-1 );
    nameGenerator2->SetIncrementIndex( 1 );

    for( unsigned int i = 0; i < m_NumberOfTimePoints; i++ )
    {
      for( unsigned int ch = 0; ch < m_NumberOfChannels; ch++ )
      {
        std::string m_SeriesFormat = prefix + nameGenerator1->GetFileNames()[i] +
          nameGenerator2->GetFileNames()[ch] + "-zs%04d." + m_FileType;

        std::cout << m_SeriesFormat << std::endl;
        TileReader3DPointer tileReader = TileReader3DType::New();
        tileReader->SetRegionOfInterest( region );
        tileReader->SetSeriesFormat( m_SeriesFormat.c_str() );
        tileReader->SetSpacing( spacing );
        tileReader->GenerateData();

        std::stringstream output;
        output << argv[2+ch] << i << ".mha";
        std::cout << output.str() << std::endl;
        WriterType::Pointer writer = WriterType::New();
        writer->SetFileName( output.str().c_str() );
        writer->SetInput( tileReader->GetOutput() );
        writer->Update();
      }
    }
  }

  return EXIT_SUCCESS;
}
