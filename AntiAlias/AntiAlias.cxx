#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAntiAliasBinaryImageFilter.h"

int main(int argc, char argv[])
{
  if( argc != 4 )
    {
    std::cerr << argv[0] << " InputFileName OutputFileName NumberOfIterations" <<std ::endl;
    return EXIT_FAILURE;
    }
  const unsigned int Dimension = 3;

  typedef unsigned char InputPixelType;
  typedef float         OutputPixelType;

  typedef itk::Image< InputPixelType, Dimension >   InputImageType;
  typedef itk::ImageFileReader< InputImageType >    ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::AntiAliasBinaryImageFilter< InputImageType, OutputImageType >    AntiAliasBinaryImageFilterType;
  AntiAliasBinaryImageFilterType::Pointer filter = AntiAliasBinaryImageFilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->SetMaximumIterations( atoi( argv[3] ) );
  filter->Update();

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
