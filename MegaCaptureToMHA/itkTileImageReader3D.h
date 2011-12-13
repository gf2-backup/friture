/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkTileImageReader3D.h,v $
  Language:  C++
  Date:      $Date: 2009-04-25 12:25:44 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTileImageReader3D_h
#define __itkTileImageReader3D_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObject.h"
#include "itkTileImageFilter.h"
#include "itkNumericSeriesFileNames.h"
#include "itkImageFileReader.h"
#include "itkRegionOfInterestImageFilter.h"
#include <vector>

namespace itk
{

/** \class TileImageReader3D
 * \brief Read a tile in 3D
 *
 * \ingroup IOFilters
 *
 */
template <class TInputImage>
class ITK_EXPORT TileImageReader3D : public Object
{
public:
  /** Standard class typedefs. */
  typedef TileImageReader3D           Self;
  typedef Object                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileImageReader3D, Object);

  /** Some convenient typedefs. */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::PixelType  InputImagePixelType;
  typedef typename InputImageType::IndexType  InputImageIndexType;
  typedef typename InputImageIndexType::IndexValueType InputIndexValueType;
  typedef typename InputImageType::SpacingType InputImageSpacingType;
  typedef typename InputImageType::PointType  InputImagePointType;

  typedef Image< unsigned char, 2 >           ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::RegionType      ImageRegionType;
  typedef typename ImageType::IndexType       ImageIndexType;
  typedef typename ImageType::SizeType        ImageSizeType;
  typedef typename ImageType::PointType       ImagePointType;

  typedef TileImageFilter< ImageType, InputImageType > TileImageFilterType;
  typedef typename TileImageFilterType::Pointer   TileImagePointer;
  typedef typename TileImageFilterType::LayoutArrayType LayoutArrayType;

  typedef NumericSeriesFileNames                   NameGeneratorType;
  typedef typename NameGeneratorType::Pointer      NameGeneratorPointer;
  typedef ImageFileReader< ImageType >             ImageFileReaderType;
  typedef typename ImageFileReaderType::Pointer    ImageFileReaderPointer;
  typedef RegionOfInterestImageFilter< ImageType, ImageType > ROIFilterType;
  typedef typename ROIFilterType::Pointer          ROIFilterPointer;

  itkStaticConstMacro( ImageDimension, unsigned int,
    TInputImage::ImageDimension );

  itkSetStringMacro(SeriesFormat);
  itkGetStringMacro(SeriesFormat);

  void SetRegionOfInterest( InputImageRegionType& roi )
  {
    m_ROI = roi;
  }

  void SetSpacing( InputImageSpacingType& sp )
  {
    m_Spacing = sp;
  }

  void SetLayout( LayoutArrayType& layout )
  {
    m_Layout = layout;
  }

  InputImageRegionType GetRegionOfInterest()
  {
    return m_ROI;
  }

  InputImagePointer GetOutput()
  {
    return m_Output;
  }

  /** Does the real work. */
  void GenerateData();

protected:
  TileImageReader3D();
  ~TileImageReader3D(){}
  void PrintSelf(std::ostream& os, Indent indent) const;

  InputImageRegionType m_ROI;
  InputImagePointer m_Output;
  InputImageSpacingType m_Spacing;

  /** A string for formatting the names of files in the series. */
  std::string m_SeriesFormat;
  std::vector<std::string>  m_FileNames;
  LayoutArrayType m_Layout;

private:
  TileImageReader3D(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} //namespace ITK

#include "itkTileImageReader3D.txx"

#endif // __itkTileImageReader3D_h
