#include "itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "itkNumericSeriesFileNames.h"

int main( int argc, char * argv[] )
{
  // Verify the number of parameters in the command line
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0]
              << " pattern firstSliceValue lastSliceValue outputImageFile"
              << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char   PixelType;
  const unsigned int Dimension = 4;

  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageSeriesReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter<   ImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const unsigned int first = atoi( argv[2] );
  const unsigned int last  = atoi( argv[3] );

  const char * outputFilename = argv[4];

  typedef itk::NumericSeriesFileNames    NameGeneratorType;

  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();

  nameGenerator->SetSeriesFormat( argv[1] );
  nameGenerator->SetStartIndex( first );
  nameGenerator->SetEndIndex( last );
  nameGenerator->SetIncrementIndex( 1 );

  std::vector< std::string > names = nameGenerator->GetFileNames();

  reader->SetFileNames( names  );
  reader->Update();

  ImageType::Pointer output = reader->GetOutput();

  ImageType::SpacingType spacing = output->GetSpacing();
  spacing[3] = 4.;

  output->SetSpacing( spacing );

  writer->SetFileName( outputFilename );
  writer->SetInput( output );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
