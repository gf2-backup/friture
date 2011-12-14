#include "vnl/vnl_math.h"
#include <math.h>

#include "itkNumericTraits.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"

namespace itk
{
namespace Functor
{
template< class TInput, class TOutput >
class Gaussian
{
public:
  typedef Gaussian Self;
  typedef typename NumericTraits< TInput >::RealType RealType;

  Gaussian() :  m_Factor( 1. ),
                m_Sigma( 1. ) {}
  ~Gaussian() {}

  bool operator != ( const Self & ) const
    {
    return false;
    }
  bool operator==( const Self & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    return static_cast< TOutput >( m_Factor * vcl_exp( static_cast< RealType >(-0.5) * vnl_math_sqr( static_cast< RealType >( A ) )  / m_Sigma ) );
    }

private:
  RealType  m_Factor;
  RealType  m_Sigma;
};
}
}

int main(int argc, char * argv[])
{
  const unsigned int Dimension = 3;

  typedef float PixelType;

  typedef itk::Image< PixelType, Dimension >  ImageType;
  typedef itk::ImageFileReader< ImageType >   ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();

  typedef itk::Functor::Gaussian< PixelType, PixelType >                  FunctorType;
  typedef itk::UnaryFunctorImageFilter<ImageType,ImageType, FunctorType > FilterType;

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );
  filter->Update();

  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}
