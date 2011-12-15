#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkMedianImageFilter.h"
#include "itkSubtractImageFilter.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " InputImageFile OutputImageFile [radius]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;

  // Setup types
  typedef itk::Image<PixelType, 3 >                     ImageType;
  typedef itk::ImageFileReader<ImageType>               ReaderType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  // Create and setup a median filter
  typedef itk::MedianImageFilter<ImageType, ImageType > FilterType;
  FilterType::Pointer medianFilter = FilterType::New();
  FilterType::InputSizeType radius;
  radius.Fill(2);
  if (argc > 3)
    {
    radius.Fill(atoi(argv[3]));
    }

  medianFilter->SetRadius(radius);
  medianFilter->SetInput( reader->GetOutput() );
  medianFilter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( medianFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}


