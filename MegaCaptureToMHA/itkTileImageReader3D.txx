/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTileImageReader3D.txx,v $
  Language:  C++
  Date:      $Date: 2010-04-12 13:10:45 $
  Version:   $Revision: 1.90 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTileImageReader3D_txx
#define __itkTileImageReader3D_txx

#include "itkTileImageReader3D.h"

namespace itk
{

template < class TInputImage >
TileImageReader3D< TInputImage >
::TileImageReader3D()
{
  m_SeriesFormat = "%d";
  m_Layout.Fill( 1 );
  m_Layout[ ImageDimension - 1 ] = 0;
}

template < class TInputImage >
void TileImageReader3D< TInputImage >
::GenerateData()
{
  TileImagePointer tile = TileImageFilterType::New();
  tile->SetLayout( m_Layout );

  InputIndexValueType firstFile = m_ROI.GetIndex()[2];
  InputIndexValueType lastFile = m_ROI.GetIndex()[2] + m_ROI.GetSize()[2] - 1;

  NameGeneratorPointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetSeriesFormat( m_SeriesFormat );
  nameGenerator->SetStartIndex( firstFile );
  nameGenerator->SetEndIndex( lastFile );
  nameGenerator->SetIncrementIndex( 1 );

  m_FileNames = nameGenerator->GetFileNames();

  ImageRegionType region;
  ImageIndexType  index;
  index[0] = m_ROI.GetIndex()[0];
  index[1] = m_ROI.GetIndex()[1];
  ImageSizeType   size;
  size[0] = m_ROI.GetSize()[0];
  size[1] = m_ROI.GetSize()[1];

  region.SetIndex( index );
  region.SetSize( size );

  ImagePointType origin;
  origin[0] = m_Spacing[0]*index[0];
  origin[1] = m_Spacing[1]*index[1];

  typename std::vector< ImagePointer > extracts;

  for( unsigned int i = 0; i < m_FileNames.size(); i++ )
  {
    ImageFileReaderPointer reader = ImageFileReaderType::New();
    reader->SetFileName( m_FileNames[i] );

    ROIFilterPointer roi = ROIFilterType::New();
    roi->SetInput( reader->GetOutput() );
    roi->SetRegionOfInterest( region );
    roi->Update();
    ImagePointer out = roi->GetOutput();
    out->DisconnectPipeline();

    // Modify the x, y origin
    out->SetOrigin( origin );

    extracts.push_back( out );
    tile->PushBackInput( extracts.back() );
  }

  tile->Update();
  m_Output = tile->GetOutput();
  m_Output->SetSpacing( this->m_Spacing );

  // Modify the z origin
  InputImagePointType m_Origin = m_Output->GetOrigin();
  for( unsigned int i = 2; i < ImageDimension; i++ )
  {
    m_Origin[i] = m_ROI.GetIndex()[i] * m_Spacing[i];
  }
  m_Output->SetOrigin( m_Origin );
}

template < class TInputImage >
void TileImageReader3D< TInputImage >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ROI: " << m_ROI << std::endl;
  os << indent << "Series Format: " <<   m_SeriesFormat << std::endl;
}

} //namespace ITK

#endif
