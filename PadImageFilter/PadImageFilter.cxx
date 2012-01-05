#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConstantPadImageFilter.h"

int main( int argc, char* argv[] )
{
  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName ( argv[1] );
  reader->Update();

  typedef itk::ConstantPadImageFilter <ImageType, ImageType> ConstantPadImageFilterType;

  ImageType::SizeType lowerExtendRegion;
  lowerExtendRegion[0] = 10;
  lowerExtendRegion[1] = 10;
  lowerExtendRegion[2] = 10;

  ImageType::SizeType upperExtendRegion;
  upperExtendRegion[0] = 10;
  upperExtendRegion[1] = 10;
  upperExtendRegion[2] = 10;

  ConstantPadImageFilterType::Pointer padFilter = ConstantPadImageFilterType::New();
  padFilter->SetInput( reader->GetOutput() );
  //padFilter->SetPadBound(outputRegion); // Calls SetPadLowerBound(region) and SetPadUpperBound(region)
  padFilter->SetPadLowerBound(lowerExtendRegion);
  padFilter->SetPadUpperBound(upperExtendRegion);
  padFilter->SetConstant( 0 );
  padFilter->Update();

  typedef itk::ImageFileWriter< ImageType > TempWriterType;
  TempWriterType::Pointer tempwriter = TempWriterType::New();
  tempwriter->SetInput( padFilter->GetOutput() );
  tempwriter->SetFileName( argv[2] );
  tempwriter->Update();
}
