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

#ifndef __itkMegacaptureToMha_h
#define __itkMegacaptureToMha_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkObject.h"

namespace itk
{
class ITK_EXPORT MegacaptureToMha : public LightObject
{
  public:
    typedef MegacaptureToMha Self;
    typedef LightObject                  Superclass;
    typedef SmartPointer< Self >         Pointer;
    typedef SmartPointer< const Self >   ConstPointer;

    /** Method for creation through object factory */
    itkNewMacro ( Self );

    /** Run-time type information */
    itkTypeMacro ( MegacaptureToMha, LightObject );

    /** Display color table*/
    void PrintStart( std::istream& os )
    {
      std::string line, start;

      // MegaCapture
      getline( os, line );

      // <ImageSessionData>
      getline( os, line );

      // Version 3.0
      getline( os, line );

      // ExperimentTitle
      getline( os, line );

      // ExperimentDescription
      getline( os, line );

      // TimeInterval 120.01755818429191
      start = "TimeInterval";
      getline( os, line );
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_TimeInterval = atof( line.c_str() );

      // Objective
      getline( os, line );

      // VoxelSizeX 0.10924550317102567
      start = "VoxelSizeX";
      getline( os, line );
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_SpacingX = atof( line.c_str() );

      // VoxelSizeY 0.10924550317102567
      start = "VoxelSizeY";
      getline( os, line );
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_SpacingY = atof( line.c_str() );

      // VoxelSizeZ 0.55
      start = "VoxelSizeZ";
      getline( os, line );
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_SpacingZ = atof( line.c_str() );

      // DimensionX 1024
      getline( os, line );
      start = "DimensionX";
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_DimensionX = atoi( line.c_str() );

      // DimensionY 831
      getline( os, line );
      start = "DimensionY";
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_DimensionY = atoi( line.c_str() );

      // DimensionPL 1
      getline( os, line );

      // DimensionCO 1
      getline( os, line );

      // DimensionRO 1
      getline( os, line );

      // DimensionZT 1
      getline( os, line );

      // DimensionYT 1
      getline( os, line );

      // DimensionXT 1
      getline( os, line );

      // DimensionTM 100
      start = "DimensionTM";
      getline( os, line );
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_NumberOfTimePoints = atoi( line.c_str() );

      // DimensionZS 58
      getline( os, line );
      start = "DimensionZS";
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_NumberOfZSlices = atoi( line.c_str() );

      // DimensionCH 1
      getline( os, line );
      start = "DimensionCH";
      line = line.substr( start.length() + 1, line.length() - start.length() );
      m_NumberOfChannels = atoi( line.c_str() );

      // ChannelColor00 16777215
      for( unsigned int i = 0; i < m_NumberOfChannels; i++ )
      {
        getline( os, line );
      }

      // ChannelDepth 8
      getline( os, line );

      // FileType PNG
      start = "FileType";
      getline( os, line );
      m_FileType = line.substr( start.length() + 1, line.length() - start.length() );

      // </ImageSessionData>
      getline( os, line );

      // <Image>
      getline( os, line );
    }

  itkGetConstMacro ( SpacingX, double );
  itkGetConstMacro ( SpacingY, double );
  itkGetConstMacro ( SpacingZ, double );
  itkGetConstMacro ( DimensionX, unsigned int );
  itkGetConstMacro ( DimensionY, unsigned int );
  itkGetConstMacro ( NumberOfChannels, unsigned int );
  itkGetConstMacro ( NumberOfTimePoints, unsigned int );
  itkGetConstMacro ( NumberOfZSlices, unsigned int );
  itkGetConstMacro ( TimeInterval, double );
  itkGetConstMacro ( FileType, std::string );

  protected:
  MegacaptureToMha()
  {
    m_SpacingX = 0;
    m_SpacingY = 0;
    m_SpacingZ = 0;
    m_DimensionX = 0;
    m_DimensionY = 0;
    m_TimeInterval = 0.0;
    m_NumberOfZSlices = 0;
    m_NumberOfTimePoints = 0;
    m_NumberOfChannels = 0;
  }
  ~MegacaptureToMha() {}

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
  unsigned int m_ChannelDepth;

  private:
    MegacaptureToMha ( Self& );   // intentionally not implemented
    void operator= ( const Self& );   // intentionally not implemented
  };

} /* namespace itk */

#endif
