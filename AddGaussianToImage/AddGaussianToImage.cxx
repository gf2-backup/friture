#include "itkImageDuplicator.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImage.h"
#include "itkAddImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char* argv[] )
{
  const unsigned int Dimension = 3;

  typedef float PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();

  ImageType::SpacingType  spacing = image->GetSpacing();
  ImageType::PointType    origin =  image->GetOrigin();
  ImageType::SizeType     size =    image->GetLargestPossibleRegion().GetSize();

  typedef itk::ImageDuplicator< ImageType > DuplicatorType;
  DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(image);
  duplicator->Update();

  ImageType::Pointer seedImage = duplicator->GetOutput();
  seedImage->FillBuffer( 0. );

  double scale = 1.;

  if( argc > 5 )
    {
    scale = atof( argv[5] );
    }

  // read a text file
  std::fstream spheres( argv[3], std::ios::in );

  unsigned int k = 0;
  float value;

  ImageType::PointType  pt;
  ImageType::IndexType  idx;

  while( spheres >> value )
    {
    if( k < Dimension )
      {
      pt[k] = value;
      std::cout << k << " : " << value << std::endl;
      ++k;
      }
    if( k == Dimension )
      {
      seedImage->TransformPhysicalPointToIndex( pt, idx );

      seedImage->SetPixel( idx, scale );

      std::cout << idx << std::endl;

      k = 0;
      }
    }

  typedef itk::DiscreteGaussianImageFilter< ImageType, ImageType > GaussianFilterType;
  GaussianFilterType::Pointer filter = GaussianFilterType::New();
  filter->SetInput( seedImage );

  GaussianFilterType::ArrayType sigma;
  sigma[0] = atof( argv[4] );
  sigma[0] *= sigma[0];
  sigma[1] = sigma[0];
  sigma[2] = sigma[0];

  filter->SetUseImageSpacing( true );
  filter->SetVariance( sigma );
  filter->GetOutput();

  typedef itk::AddImageFilter< ImageType > AddFilterType;
  AddFilterType::Pointer add = AddFilterType::New();
  add->SetInput1( image );
  add->SetInput2( filter->GetOutput() );
  add->Update();

  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescale = RescaleFilterType::New();
  rescale->SetInput( add->GetOutput() );
  rescale->SetOutputMinimum( 0. );
  rescale->SetOutputMaximum( 0.99 );
  rescale->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( rescale->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
