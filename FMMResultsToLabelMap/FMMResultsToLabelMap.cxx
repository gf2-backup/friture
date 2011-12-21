#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelShapeKeepNObjectsImageFilter.h"
#include "itkImageFileWriter.h"

int main( int argc, char* argv[] )
{
  if(argc < 4 )
    {
    std::cerr << "Usage: ";
    std::cerr << argv[0] << " inputImageFile outputImageFile numberofelements [lowerThreshold] [upperThreshold]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef unsigned char                           InputPixelType;
  typedef itk::Image< InputPixelType, Dimension > InputImageType;
  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef unsigned char                             OutputPixelType;
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;

  InputPixelType  lowerThreshold = itk::NumericTraits< InputPixelType >::Zero;
  InputPixelType  upperThreshold = itk::NumericTraits< InputPixelType >::One;

  if (argc > 4)
    {
    lowerThreshold = atof(argv[4]);
    }
  if (argc > 5)
    {
    upperThreshold = atof(argv[5]);
    }

  typedef itk::BinaryThresholdImageFilter< InputImageType, OutputImageType > BinaryThresholdImageFilterType;

  BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput( reader->GetOutput() );
  thresholdFilter->SetLowerThreshold( lowerThreshold );
  thresholdFilter->SetUpperThreshold( upperThreshold );
  thresholdFilter->SetOutsideValue( itk::NumericTraits< OutputPixelType >::Zero );
  thresholdFilter->SetInsideValue( itk::NumericTraits< OutputPixelType >::max() );
  thresholdFilter->Update();

  typedef itk::BinaryImageToLabelMapFilter< OutputImageType > BinaryImageToLabelMapFilterType;
  BinaryImageToLabelMapFilterType::Pointer binaryImageToLabelMapFilter = BinaryImageToLabelMapFilterType::New();
  binaryImageToLabelMapFilter->SetInput( thresholdFilter->GetOutput() );
  binaryImageToLabelMapFilter->SetInputForegroundValue( itk::NumericTraits< OutputPixelType >::max() );
  binaryImageToLabelMapFilter->Update();

  std::cout << "Number Of Objects: " << binaryImageToLabelMapFilter->GetNumberOfObjects() << std::endl;

  typedef itk::LabelMapToLabelImageFilter<BinaryImageToLabelMapFilterType::OutputImageType, OutputImageType> LabelMapToLabelImageFilterType;
  LabelMapToLabelImageFilterType::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();
  labelMapToLabelImageFilter->SetInput( binaryImageToLabelMapFilter->GetOutput() );
  labelMapToLabelImageFilter->Update();

  typedef itk::LabelShapeKeepNObjectsImageFilter< OutputImageType > LabelShapeKeepNObjectsImageFilterType;
  LabelShapeKeepNObjectsImageFilterType::Pointer labelShapeKeepNObjectsImageFilter = LabelShapeKeepNObjectsImageFilterType::New();
  labelShapeKeepNObjectsImageFilter->SetInput( labelMapToLabelImageFilter->GetOutput() );
  labelShapeKeepNObjectsImageFilter->SetBackgroundValue( 0 );
  labelShapeKeepNObjectsImageFilter->SetNumberOfObjects( atoi( argv[3] ) );
  labelShapeKeepNObjectsImageFilter->SetAttribute(
        LabelShapeKeepNObjectsImageFilterType::LabelObjectType::NUMBER_OF_PIXELS );
  labelShapeKeepNObjectsImageFilter->Update();

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
  writer->SetInput( labelMapToLabelImageFilter->GetOutput() );
  writer->Update();

  return EXIT_SUCCESS;
}
