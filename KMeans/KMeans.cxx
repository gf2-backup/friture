#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkScalarImageKmeansImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIterator.h"

int main(int argc, char* argv[])
{
  const unsigned int Dimension = 3;

  typedef unsigned char                       PixelType;
  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageFileReader< ImageType >   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::ScalarImageKmeansImageFilter< ImageType > KMeansFilterType;

  KMeansFilterType::Pointer kmeansFilter = KMeansFilterType::New();

  kmeansFilter->SetInput( reader->GetOutput() );
  kmeansFilter->SetUseNonContiguousLabels(true);
  kmeansFilter->AddClassWithInitialMean(0);
  kmeansFilter->AddClassWithInitialMean(5);
//  kmeansFilter->AddClassWithInitialMean(30);
  kmeansFilter->Update();

  KMeansFilterType::ParametersType estimatedMeans = kmeansFilter->GetFinalMeans();

  const unsigned int numberOfClasses = estimatedMeans.Size();

  for(unsigned int i = 0 ; i < numberOfClasses ; ++i)
    {
    std::cout << "cluster[" << i << "] ";
    std::cout << "    estimated mean : " << estimatedMeans[i] << std::endl;
    }

  typedef KMeansFilterType::OutputImageType  OutputImageType;

  typedef itk::RelabelComponentImageFilter<
                                OutputImageType,
                                OutputImageType > RelabelFilterType;

  RelabelFilterType::Pointer relabeler = RelabelFilterType::New();

  relabeler->SetInput( kmeansFilter->GetOutput() );
  relabeler->Update();

  typedef itk::RescaleIntensityImageFilter< ImageType, ImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput(relabeler->GetOutput());
  rescaleFilter->SetOutputMinimum(0);
  rescaleFilter->SetOutputMaximum(255);
  rescaleFilter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescaleFilter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
