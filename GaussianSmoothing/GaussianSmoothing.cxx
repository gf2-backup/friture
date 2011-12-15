#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDiscreteGaussianImageFilter.h"
 
int main(int argc, char * argv[])
{
  // Verify command line arguments
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [variance]" << std::endl;
    return EXIT_FAILURE;
    }
 
  // Parse command line argumentsa
  std::string inputFilename = argv[1];
 
  double variance = 4.0;
  if (argc > 3)
    {
    variance = atof(argv[3]);
    }
 
  // Setup types
  typedef itk::Image< unsigned char, 3 > UnsignedCharImageType;
  typedef itk::Image< float, 3 >         FloatImageType;
 
  typedef itk::ImageFileReader< UnsignedCharImageType >  readerType;
 
  typedef itk::DiscreteGaussianImageFilter<
    UnsignedCharImageType, FloatImageType >  filterType;
 
  // Create and setup a reader
  readerType::Pointer reader = readerType::New();
  reader->SetFileName( inputFilename.c_str() );
 
  // Create and setup a Gaussian filter
  filterType::Pointer gaussianFilter = filterType::New();
  gaussianFilter->SetInput( reader->GetOutput() );
  gaussianFilter->SetVariance(variance);

  typedef itk::ImageFileWriter< FloatImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( gaussianFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
 
  return EXIT_SUCCESS;
}


