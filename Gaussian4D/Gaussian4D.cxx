#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc != 7 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [variance]" << std::endl;
    return EXIT_FAILURE;
    }

  // Parse command line argumentsa
  std::string inputFilename = argv[1];

  const unsigned int Dimension = 4;

  // Setup types
  typedef itk::Image< unsigned char, Dimension > InputImageType;
  typedef itk::Image< float, Dimension >         OutputImageType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  typedef itk::DiscreteGaussianImageFilter< InputImageType, OutputImageType >  FilterType;

  // Create and setup a reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename.c_str() );

  // Create and setup a Gaussian filter
  FilterType::Pointer gaussianFilter = FilterType::New();
  gaussianFilter->SetInput( reader->GetOutput() );

  FilterType::ArrayType variance;
  variance[0] = atof( argv[3] );
  variance[1] = atof( argv[4] );
  variance[2] = atof( argv[5] );
  variance[3] = atof( argv[6] );

  gaussianFilter->SetVariance(variance);
  gaussianFilter->Update();

  typedef itk::BinaryThresholdImageFilter< OutputImageType, InputImageType>
    BinaryThresholdImageFilterType;

  BinaryThresholdImageFilterType::Pointer thresholdFilter
    = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(gaussianFilter->GetOutput());
  thresholdFilter->SetLowerThreshold( 128. );
  thresholdFilter->SetUpperThreshold( 255. );
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);

  typedef itk::ImageFileWriter< InputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( thresholdFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}

