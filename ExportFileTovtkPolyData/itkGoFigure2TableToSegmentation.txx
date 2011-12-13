/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1658 $  // Revision of last commit
  Date: $Date: 2010-06-14 15:49:25 -0400 (Mon, 14 Jun 2010) $  // Date of last commit
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

#ifndef __itkGoFigure2TableToSegmentation_txx
#define __itkGoFigure2TableToSegmentation_txx

#include "itkGoFigure2TableToSegmentation.h"

namespace itk
{
template < class TSegmentImage >
GoFigure2TableToSegmentation< TSegmentImage >
::GoFigure2TableToSegmentation()
{
  m_NumberOfCellTypes = 0;
  m_NumberOfSubCellularTypes = 0;
  m_NumberOfColors = 0;
  m_NumberOfChannels = 0;
  m_NumberOfMeshes = 0;
  m_NumberOfTracks = 0;
}


template < class TSegmentImage >
std::string
GoFigure2TableToSegmentation< TSegmentImage >::
GetValue ( std::string start, std::istream& os, unsigned int len )
{
  std::string line;
  getline( os, line );
  line = line.substr( len + start.length(), line.length() - len - 2*start.length() - 1 );

  return line;
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintStart ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  //"<?xml version=\"1.0\" ?>";
  getline( os, line );
  //"<ExportTraces version=\"1\"";
  getline( os, line );
  //"  <imagingsession>";
  getline( os, line );

  this->m_ImagingSession.Name = GetValue( "<Name>", os, len + 4 );
  this->m_ImagingSession.CreationDate = GetValue( "<CreationDate>", os, len + 4 );
  this->m_ImagingSession.MicroscopeName = GetValue( "<MicroscopeName>", os, len + 4 );

  //"  </imagingsession>"
  getline( os, line );
}

template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintEnd ( std::istream& os, unsigned int indent )
{
  //"</ExportTraces>"
  std::string line;
  getline( os, line );
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintColor ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfcolor>", os, len );
  this->m_NumberOfColors = atoi( line.c_str() );
  m_Colors.resize( this->m_NumberOfColors );

  for( unsigned int i = 0; i < this->m_Colors.size(); i++ )
  {
    //<color>
    getline( os, line );

    line = GetValue( "<Alpha>", os, len + 2 );
    this->m_Colors[i].Alpha = atoi( line.c_str() );
    line = GetValue( "<Blue>", os, len + 2 );
    this->m_Colors[i].Blue = atoi( line.c_str() );

    line = GetValue( "<ColorID>", os, len + 2 );
    this->m_Colors[i].ColorID = atoi( line.c_str() );

    this->m_Colors[i].Description = GetValue( "<Description>", os, len + 2 );

    line = GetValue( "<Green>", os, len + 2 );
    this->m_Colors[i].Green = atoi( line.c_str() );

    this->m_Colors[i].Name = GetValue( "<Name>", os, len + 2 );

    line = GetValue( "<Red>", os, len + 2 );
    this->m_Colors[i].Red = atoi( line.c_str() );

    //</color>
    getline( os, line );
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintCell ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfcelltype>", os, indent );
  this->m_NumberOfCellTypes = atoi( line.c_str() );
  m_CellTypes.resize( this->m_NumberOfCellTypes );

  for( unsigned int i = 0; i < this->m_CellTypes.size(); i++ )
    {
    //"<celltype>";
    getline( os, line );
    line = GetValue( "<CellTypeID>", os, len + 2 );
    this->m_CellTypes[i].CellTypeID = atoi( line.c_str() );
    this->m_CellTypes[i].Description = GetValue( "<Description>", os, len + 2 );
    this->m_CellTypes[i].Name = GetValue( "<Name>", os, len + 2 );

    //"</celltype>";
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintSubCellular( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfsubcellulartype>", os, len );
  this->m_NumberOfSubCellularTypes = atoi( line.c_str() );
  m_SubCellularTypes.resize( this->m_NumberOfSubCellularTypes );

  for( unsigned int i = 0; i < this->m_SubCellularTypes.size(); i++ )
    {
    //"<subcellulartype>"
    getline( os, line );
    this->m_SubCellularTypes[i].Description = GetValue( "<Description>", os, len + 2 );
    this->m_SubCellularTypes[i].Name = GetValue( "<Name>", os, len + 2 );

    line = GetValue( "<SubCellularID>", os, len + 2 );
    this->m_SubCellularTypes[i].SubCellularID = atoi( line.c_str() );

    //"</subcellulartype>"
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintCoordinate ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  unsigned int coordID;

  std::string line;
  line = GetValue( "<NumberOfcoordinate>", os, len );
  this->m_NumberOfCoordinates = atoi( line.c_str() );
  m_Coordinates.resize( this->m_NumberOfCoordinates );

  for( unsigned int i = 0; i < this->m_Coordinates.size(); i++ )
    {
    //<coordinate>
    getline( os, line );

    line = GetValue( "<CCoord>", os, len + 2 );
    this->m_Coordinates[i].CCoord = atoi( line.c_str() );
    line = GetValue( "<CoordID>", os, len + 2 );
    coordID = atoi( line.c_str() );
    this->m_Coordinates[i].CoordID = coordID;
    line = GetValue( "<PCoord>", os, len + 2 );
    this->m_Coordinates[i].PCoord = atoi( line.c_str() );
    line = GetValue( "<RCoord>", os, len + 2 );
    this->m_Coordinates[i].RCoord = atoi( line.c_str() );
    line = GetValue( "<TCoord>", os, len + 2 );
    this->m_Coordinates[i].TCoord = atoi( line.c_str() );

    line = GetValue( "<XCoord>", os, len + 2 );
    this->m_Coordinates[i].XCoord = atoi( line.c_str() );

    line = GetValue( "<XTileCoord>", os, len + 2 );
    this->m_Coordinates[i].XTileCoord = atoi( line.c_str() );
    line = GetValue( "<YCoord>", os, len + 2 );
    this->m_Coordinates[i].YCoord = atoi( line.c_str() );
    line = GetValue( "<YTileCoord>", os, len + 2 );
    this->m_Coordinates[i].YTileCoord = atoi( line.c_str() );
    line = GetValue( "<ZCoord>", os, len + 2 );
    this->m_Coordinates[i].ZCoord = atoi( line.c_str() );
    line = GetValue( "<ZTileCoord>", os, len + 2 );
    this->m_Coordinates[i].ZTileCoord = atoi( line.c_str() );

    this->m_CoordID[coordID] = i;

    //</coordinate>
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintLineage( std::istream& os, unsigned int indent )
{
  std::string line;
  unsigned int len = static_cast<unsigned int>( indent );
  line = GetValue( "<NumberOflineage>", os, len );

  this->m_NumberOfLineages = atoi( line.c_str() );
  m_Lineages.resize( this->m_NumberOfLineages );

  for( unsigned int i = 0; i < this->m_NumberOfLineages; i++ )
    {
    //<lineage>
    getline( os, line );

    line = GetValue( "<ColorID>", os, len + 2 );
    this->m_Lineages[i].ColorID = atoi( line.c_str() );
    line = GetValue( "<CoordIDMax>", os, len + 2 );
    this->m_Lineages[i].CoordIDMax = atoi( line.c_str() );
    line = GetValue( "<CoordIDMin>", os, len + 2 );
    this->m_Lineages[i].CoordIDMin = atoi( line.c_str() );
    line = GetValue( "<ImagingSessionID>", os, len + 2 );
    this->m_Lineages[i].ImagingSessionID = atoi( line.c_str() );

    this->m_Lineages[i].Points = GetValue( "<Points>", os, len + 2 );

    line = GetValue( "<TrackIDRoot>", os, len+2 );
    this->m_Lineages[i].TrackIDRoot = atoi( line.c_str() );

    line = GetValue( "<LineageID>", os, len+2 );
    int lineageID = atoi( line.c_str() );

    this->m_Lineages[i].LineageID = lineageID;

    this->m_LineageID[lineageID] = i;

    //</lineage>;
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintTrackFamily( std::istream& os, unsigned int indent )
{
  std::string line;
  unsigned int len = static_cast<unsigned int>( indent );
  line = GetValue( "<NumberOftrackfamily>", os, len );

  this->m_NumberOfTrackFamily = atoi( line.c_str() );
  m_TrackFamily.resize( this->m_NumberOfTrackFamily );

  for( unsigned int i = 0; i < this->m_NumberOfTrackFamily; i++ )
    {
    //<trackfamily>
    getline( os, line );

    line = GetValue( "<TrackFamilyID>", os, len + 2 );
    this->m_TrackFamily[i].TrackFamilyID = atoi( line.c_str() );
    line = GetValue( "<TrackIDMother>", os, len + 2 );
    this->m_TrackFamily[i].TrackMotherID = atoi( line.c_str() );
    line = GetValue( "<TrackIDDaughter1>", os, len + 2 );
    this->m_TrackFamily[i].TrackDaughter1ID = atoi( line.c_str() );
    line = GetValue( "<TrackIDDaughter2>", os, len + 2 );
    this->m_TrackFamily[i].TrackDaughter2ID = atoi( line.c_str() );

    //</trackfamily>;
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintTrack ( std::istream& os, unsigned int indent )
{
  unsigned int trackID;
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOftrack>", os, len );
  this->m_NumberOfTracks = atoi( line.c_str() );
  m_Tracks.resize( this->m_NumberOfTracks );

    std::cout << line << std::endl;

  for( unsigned int i = 0; i < this->m_Tracks.size(); i++ )
    {
    //<track>
    getline( os, line );

    line = GetValue( "<ColorID>", os, len + 2 );
    this->m_Tracks[i].ColorID = atoi( line.c_str() );
    line = GetValue( "<CoordIDMax>", os, len + 2 );
    this->m_Tracks[i].CoordIDMax = atoi( line.c_str() );
    line = GetValue( "<CoordIDMin>", os, len + 2 );
    this->m_Tracks[i].CoordIDMin = atoi( line.c_str() );
    line = GetValue( "<ImagingSessionID>", os, len + 2 );
    this->m_Tracks[i].ImagingSessionID = atoi( line.c_str() );

    this->m_Tracks[i].Points = GetValue( "<Points>", os, len + 2 );

    line = GetValue( "<TrackFamilyID>", os, len + 2 );
    this->m_Tracks[i].TrackFamilyID = atoi( line.c_str() );
    line = GetValue( "<lineageID>", os, len + 2 );
    this->m_Tracks[i].LineageID = atoi( line.c_str() );
    line = GetValue( "<trackID>", os, len + 2 );
    trackID = atoi( line.c_str() );
    this->m_Tracks[i].TrackID = trackID;

    this->m_TrackID[trackID] = i;

    //</track>;
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintMesh ( std::istream& os, unsigned int indent )
{
  unsigned int meshID;
  unsigned int trackID;
  unsigned int coordidmin;

  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfmesh>", os, len );
  this->m_NumberOfMeshes = atoi( line.c_str() );

  m_Meshes.resize( this->m_NumberOfMeshes );

  for( unsigned int i = 0; i < this->m_Meshes.size(); i++ )
    {
    //<mesh>
    getline( os, line );

    line = GetValue( "<CellTypeID>", os, len + 2 );
    this->m_Meshes[i].CellTypeID = atoi( line.c_str() );
    line = GetValue( "<ColorID>", os, len + 2 );
    this->m_Meshes[i].ColorID = atoi( line.c_str() );
    line = GetValue( "<CoordIDMax>", os, len + 2 );
    this->m_Meshes[i].CoordIDMax = atoi( line.c_str() );
    line = GetValue( "<CoordIDMin>", os, len + 2 );
    coordidmin = atoi( line.c_str() );
    this->m_Meshes[i].CoordIDMin = coordidmin;
    line = GetValue( "<ImagingSessionID>", os, len + 2 );
    this->m_Meshes[i].ImagingSessionID = atoi( line.c_str() );

    this->m_Meshes[i].Points = GetValue( "<Points>", os, len + 2 );

    line = GetValue( "<SubCellularID>", os, len + 2 );
    this->m_Meshes[i].SubCellularTypeID = atoi( line.c_str() );
    line = GetValue( "<meshID>", os, len + 2 );
    meshID = atoi( line.c_str() );
    this->m_Meshes[i].MeshID = meshID;
    line = GetValue( "<trackID>", os, len + 2 );
    trackID = atoi( line.c_str() );
    this->m_Meshes[i].TrackID = trackID;

    this->m_MeshID[meshID] = i;

    unsigned int j = this->m_TrackID[trackID];
    unsigned int k = this->m_CoordID[coordidmin];
    unsigned int t = this->m_Coordinates[k].TCoord;

    if ( this->m_NumberOfTracks > 0 )
    {
      this->m_Tracks[j].MeshIDFromTCoordLookup[t] = meshID;
    }

    //</mesh>
    getline( os, line );
    }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintContour ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfcontour>", os, len );
  this->m_NumberOfContours = atoi( line.c_str() );
  m_Contours.resize( this->m_NumberOfContours );

  for( unsigned int i = 0; i < this->m_Contours.size(); i++ )
  {
    //<contour>
    getline( os, line );

    line = GetValue( "<ColorID>", os, len + 2 );
    this->m_Contours[i].ColorID = atoi( line.c_str() );
    line = GetValue( "<CoordIDMax>", os, len + 2 );
    this->m_Contours[i].CoordIDMax = atoi( line.c_str() );
    line = GetValue( "<CoordIDMin>", os, len + 2 );
    this->m_Contours[i].CoordIDMin = atoi( line.c_str() );
    line = GetValue( "<ImagingSessionID>", os, len + 2 );
    this->m_Contours[i].ImagingSessionID = atoi( line.c_str() );
    this->m_Contours[i].Points = GetValue( "<Points>", os, len + 2 );
    line = GetValue( "<ContourID>", os, len + 2 );
    this->m_Contours[i].ContourID = atoi( line.c_str() );
    line = GetValue( "<MeshID>", os, len + 2 );
    this->m_Contours[i].MeshID = atoi( line.c_str() );

    //</contour>
    getline( os, line );
  }
}


template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintChannel ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfchannel>", os, len );
  this->m_NumberOfChannels = atoi( line.c_str() );
  m_Channels.resize( this->m_NumberOfChannels );

  for( unsigned int i = 0; i < this->m_Channels.size(); i++ )
  {
    //<channel>
    getline( os, line );
    line = GetValue( "<ChannelID>", os, len );
    this->m_Channels[i].ChannelID= atoi( line.c_str() );
    line = GetValue( "<ChannelNumber>", os, len );
    this->m_Channels[i].ChannelNumber = atoi( line.c_str() );
    line = GetValue( "<ColorID>", os, len );
    this->m_Channels[i].ColorID = atoi( line.c_str() );
    line = GetValue( "<ImagingSessionID>", os, len );
    this->m_Channels[i].ImagingSessionID = atoi( line.c_str() );
    this->m_Channels[i].Name = GetValue( "<Name>", os, len );
    line = GetValue( "<NumberOfBits>", os, len );
    this->m_Channels[i].NumberOfBits = atoi( line.c_str() );
    //</channel>
    getline( os, line );
  }
}



template < class TSegmentImage >
void
GoFigure2TableToSegmentation< TSegmentImage >::
PrintIntensity ( std::istream& os, unsigned int indent )
{
  unsigned int len = static_cast<unsigned int>( indent );

  std::string line;
  line = GetValue( "<NumberOfintensity>", os, len );
  this->m_NumberOfIntensitys = atoi( line.c_str() );
  m_Intensitys.resize( this->m_NumberOfIntensitys );

  for( unsigned int i = 0; i < this->m_Intensitys.size(); i++ )
  {
    //<intensity>
    getline( os, line );

    line = GetValue( "<ChannelID>", os, len );
    this->m_Intensitys[i].ChannelID= atoi( line.c_str() );
    line = GetValue( "<IntensityID>", os, len );
    this->m_Intensitys[i].IntensityID = atoi( line.c_str() );
    line = GetValue( "<MeshID>", os, len );
    this->m_Intensitys[i].MeshID = atoi( line.c_str() );
    line = GetValue( "<Value>", os, len );
    this->m_Intensitys[i].Value = atoi( line.c_str() );

    //</intensity>
    getline( os, line );
  }
}

} /* end namespace itk */

#endif
