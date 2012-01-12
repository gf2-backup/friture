#include "itkImageFileReader.h"
#include "itkExtractImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkMatrix.h"

int main(int argc, char * argv[])
{
  if ( argc < 4 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile slice outputFile <dim>" << std::endl;
    return EXIT_FAILURE;
  }

  const unsigned int Dimension = 4;
  const unsigned int RDimension = 3;

  typedef unsigned char PixelType;

  typedef itk::Image< PixelType, Dimension > ImageType;
  typedef itk::Image< PixelType, RDimension > RImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::ExtractImageFilter< ImageType, RImageType > ExtractType;
  typedef itk::ImageFileWriter< RImageType > WriterType;

  unsigned int slice = atoi(argv[2]);

  unsigned int dim = Dimension-1;

  if ( argc > 4 )
    {
    dim = atoi( argv[4] );
    }

  ImageType::Pointer input;
  {
  // filename generator instance
  ReaderType::Pointer cellReader = ReaderType::New();
  cellReader->SetFileName( argv[1] );
  cellReader->Update();
  input = cellReader->GetOutput();
  input->DisconnectPipeline();
  }

  ImageType::SizeType inputSize = input->GetLargestPossibleRegion().GetSize();
  inputSize[dim] = 0;

  ImageType::IndexType idx = input->GetLargestPossibleRegion().GetIndex();
  idx[dim] = slice;

  ImageType::RegionType region;
  region.SetSize( inputSize );
  region.SetIndex( idx );

  ExtractType::Pointer extract = ExtractType::New();
  extract->SetInput( input );
  extract->SetExtractionRegion( region );
  extract->SetDirectionCollapseToIdentity();
  extract->Update();

  std::cout << "size: " << inputSize << std::endl;
  std::cout << "index: " << idx << std::endl;
  std::cout << "output size: " << extract->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( extract->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return EXIT_SUCCESS;
}
