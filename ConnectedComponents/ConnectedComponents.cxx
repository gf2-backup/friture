#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"


int main( int argc, char *argv[])
{
  const unsigned int Dimension = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, Dimension >  ImageType;

  typedef itk::ImageFileReader< ImageType >   ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::Image< unsigned short, Dimension > OutputImageType;

  typedef itk::ConnectedComponentImageFilter <ImageType, OutputImageType >
    ConnectedComponentImageFilterType;

  ConnectedComponentImageFilterType::Pointer labelFilter
    = ConnectedComponentImageFilterType::New ();
  labelFilter->SetInput(reader->GetOutput());
  labelFilter->Update();

  std::cout << "Number of objects: " << labelFilter->GetObjectCount() << std::endl;

  typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(itk::NumericTraits<unsigned short>::max());
  rescaleFilter->SetInput(labelFilter->GetOutput());
  rescaleFilter->Update();

  typedef itk::ImageFileWriter< OutputImageType >   WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( rescaleFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
