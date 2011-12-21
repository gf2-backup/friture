#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFixedArray.h"
#include "itkFlipImageFilter.h"

int main(int argc, char *argv[])
{
  const unsigned int Dimension = 3;

  typedef unsigned char                       PixelType;
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageFileReader< ImageType >   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::FlipImageFilter< ImageType > FlipImageFilterType;

  FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New ();
  flipFilter->SetInput( reader->GetOutput() );

  itk::FixedArray<bool, Dimension> flipAxes;
  flipAxes[0] = false;
  flipAxes[1] = true;
  flipAxes[2] = false;

  flipFilter->SetFlipAxes(flipAxes);
  flipFilter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( flipFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
