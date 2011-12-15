#include "itkImage.h"
#include "itkThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"


int main(int argc, char *argv[])
{
  if(argc < 3)
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputImageFile outputImageFile [lowerThreshold] [upperThreshold]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, Dimension > ImageType;

  PixelType lowerThreshold = 0;
  PixelType upperThreshold = 1;
  if (argc > 3)
    {
    lowerThreshold = atoi(argv[3]);
    }
  if (argc > 4)
    {
    upperThreshold = atoi(argv[4]);
    }

  typedef itk::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  typedef itk::ThresholdImageFilter <ImageType>
    ThresholdImageFilterType;

  ThresholdImageFilterType::Pointer thresholdFilter
    = ThresholdImageFilterType::New();
  thresholdFilter->SetInput(reader->GetOutput());
  thresholdFilter->ThresholdOutside(lowerThreshold, upperThreshold);
  thresholdFilter->SetOutsideValue(0);
  thresholdFilter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( thresholdFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
