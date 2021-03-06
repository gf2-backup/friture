/*=========================================================================
  Author: $Author: krm15 $  // Author of last commit
  Version: $Rev: 1550 $  // Revision of last commit
  Date: $Date: 2010-06-06 23:50:34 -0400 (Sun, 06 Jun 2010) $  // Date of last commit
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

#ifndef __itkGoFigure2TableToSegmentation_h
#define __itkGoFigure2TableToSegmentation_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkImageToImageFilter.h"

namespace itk
{
template < class TSegmentImage >
class ITK_EXPORT GoFigure2TableToSegmentation : public LightObject
{
  public:
    typedef GoFigure2TableToSegmentation Self;
    typedef LightObject                  Superclass;
    typedef SmartPointer< Self >         Pointer;
    typedef SmartPointer< const Self >   ConstPointer;

    itkStaticConstMacro ( ImageDimension, unsigned int,
      TSegmentImage::ImageDimension );

    /** Method for creation through object factory */
    itkNewMacro ( Self );

    /** Run-time type information */
    itkTypeMacro ( GoFigure2TableToSegmentation, LightObject );

    /** Display color table*/
    void PrintStart( std::istream& os, unsigned int indent );
    void PrintEnd( std::istream& os, unsigned int indent );
    void PrintCell( std::istream& os, unsigned int indent );
    void PrintSubCellular( std::istream& os, unsigned int indent );
    void PrintColor ( std::istream& os, unsigned int indent );
    void PrintChannel ( std::istream& os, unsigned int indent );
    void PrintIntensity( std::istream& os, unsigned int indent );
    void PrintCoordinate( std::istream& os, unsigned int indent );
    void PrintTrack( std::istream& os, unsigned int indent );
    void PrintContour( std::istream& os, unsigned int indent );
    void PrintMesh( std::istream& os, unsigned int indent );
    void PrintLineage( std::istream& os, unsigned int indent );
    void PrintTrackFamily( std::istream& os, unsigned int indent );

    typedef TSegmentImage                           SegmentImageType;
    typedef typename SegmentImageType::Pointer      SegmentImagePointer;
    typedef typename SegmentImageType::ConstPointer SegmentImageConstPointer;
    typedef typename SegmentImageType::IndexType    SegmentImageIndexType;
    typedef typename SegmentImageType::PixelType    SegmentImagePixelType;

    typedef std::map< int, int > MapLookup;

      /* This structure is derived from LevelSetFunction and stores intermediate
  values for computing time step sizes */
  struct Cell
    {
    int CellTypeID;
    std::string Name;
    std::string Description;
    };

  struct SubCellular
    {
    int SubCellularID;
    std::string Name;
    std::string Description;
    };

  struct Color
    {
    int ColorID;
    int Red;
    int Green;
    int Blue;
    int Alpha;
    std::string Name;
    std::string Description;
    };

  struct Channel
    {
    int ChannelID;
    int ChannelNumber;
    int ColorID;
    int ImagingSessionID;
    int NumberOfBits;
    std::string Name;
    };

  struct Intensity
    {
    int IntensityID;
    int Value;
    int MeshID;
    int ChannelID;
    };

  struct Coordinate
    {
    int CoordID;
    int PCoord;
    int RCoord;
    int CCoord;
    int XTileCoord;
    int YTileCoord;
    int ZTileCoord;
    int XCoord;
    int YCoord;
    int ZCoord;
    int TCoord;
    };

  struct Lineage
    {
    int ColorID;
    int CoordIDMax;
    int CoordIDMin;
    int ImagingSessionID;
    std::string Points;
    int TrackIDRoot;
    int LineageID;
    };

  struct Track
    {
    int TrackID;
    int LineageID;
    int ColorID;
    int CoordIDMax;
    int CoordIDMin;
    int ImagingSessionID;
    int TrackFamilyID;
    std::string Points;

    // my own helper container
    MapLookup MeshIDFromTCoordLookup;
    };

  struct Mesh
    {
    int MeshID;
    int CellTypeID;
    int SubCellularTypeID;
    int CoordIDMax;
    int CoordIDMin;
    int ColorID;
    int TrackID;
    int ImagingSessionID;
    std::string Points;
    };

  struct Contour
    {
      int MeshID;
      int ContourID;
      int CoordIDMax;
      int CoordIDMin;
      int ColorID;
      int ImagingSessionID;
      std::string Points;
    };

  struct ImagingSession
    {
    std::string Name;
    std::string CreationDate;
    std::string MicroscopeName;
    };

  struct TrackFamily
    {
    int TrackFamilyID;
    int TrackMotherID;
    int TrackDaughter1ID;
    int TrackDaughter2ID;
    };

  typedef std::vector< Cell > CellVectorType;
  typedef std::vector< SubCellular > SubCellularVectorType;
  typedef std::vector< Color > ColorVectorType;
  typedef std::vector< Intensity > IntensityVectorType;
  typedef std::vector< Channel > ChannelVectorType;
  typedef std::vector< Coordinate > CoordinateVectorType;
  typedef std::vector< Mesh > MeshVectorType;
  typedef std::vector< Contour > ContourVectorType;
  typedef std::vector< Track > TrackVectorType;
  typedef std::vector< Lineage > LineageVectorType;
  typedef std::vector< TrackFamily > TrackFamilyVectorType;


  void SetNumberOfCellTypes( int n )
    {
    m_NumberOfCellTypes = n;
    m_CellTypes.resize( n );
    }

  void SetNumberOfSubCellularTypes( int n )
    {
    m_NumberOfSubCellularTypes = n;
    m_SubCellularTypes.resize( n );
    }

  void SetNumberOfColors( int n )
    {
    m_NumberOfColors = n;
    m_Colors.resize( n );
    }

  void SetNumberOfChannels( int n )
    {
    m_NumberOfChannels = n;
    m_Channels.resize( n );
    }

  void SetNumberOfIntensitys( int n )
    {
    m_NumberOfIntensitys = n;
    m_Intensitys.resize( n );
    }

  void SetNumberOfCoordinates( int n )
    {
    m_NumberOfCoordinates = n;
    m_Coordinates.resize( n );
    }

  void SetNumberOfTracks( int n )
    {
    m_NumberOfTracks = n;
    m_Tracks.resize( n );
    }

  void SetNumberOfMeshes( int n )
    {
    m_NumberOfMeshes = n;
    m_Meshes.resize( n );
    m_Coordinates.resize( 2*n );
    }

  void SetNumberOfContours( int n )
    {
      m_NumberOfContours = n;
      m_Contours.resize( n );
      m_Coordinates.resize( 2*n );
    }

  void SetNumberOfTrackFamily( int n )
    {
    m_NumberOfTrackFamily = n;
    m_TrackFamily.resize( n );
    }

  ImagingSession        m_ImagingSession;
  CellVectorType        m_CellTypes;
  SubCellularVectorType m_SubCellularTypes;
  ColorVectorType       m_Colors;
  IntensityVectorType   m_Intensitys;
  ChannelVectorType     m_Channels;
  CoordinateVectorType  m_Coordinates;
  ContourVectorType     m_Contours;
  MeshVectorType        m_Meshes;
  TrackVectorType       m_Tracks;
  LineageVectorType     m_Lineages;
  TrackFamilyVectorType m_TrackFamily;

  MapLookup m_CoordID;
  MapLookup m_TrackID;
  MapLookup m_MeshID;
  MapLookup m_LineageID;
  MapLookup m_TrackFamilyID;

  int m_NumberOfCellTypes;
  int m_NumberOfSubCellularTypes;
  int m_NumberOfColors;
  int m_NumberOfChannels;
  int m_NumberOfIntensitys;
  int m_NumberOfCoordinates;
  int m_NumberOfContours;
  int m_NumberOfTracks;
  int m_NumberOfLineages;
  int m_NumberOfTrackFamily;
  int m_NumberOfMeshes;

  protected:
  GoFigure2TableToSegmentation();
  ~GoFigure2TableToSegmentation() {}

  std::string GetValue ( std::string start,
    std::istream& os, unsigned int len );

  private:
    GoFigure2TableToSegmentation ( Self& );   // intentionally not implemented
    void operator= ( const Self& );   // intentionally not implemented
  };

} /* namespace itk */

#include "itkGoFigure2TableToSegmentation.txx"
#endif
