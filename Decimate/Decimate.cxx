#include "itkQuadEdgeMesh.h"
#include "itkQuadEdgeMeshDecimationCriteria.h"
#include "itkSquaredEdgeLengthDecimationQuadEdgeMeshFilter.h"
#include "itkVTKPolyDataReader.h"
#include "itkVTKPolyDataWriter.h"

int main( int argc, char* argv[] )
{
  const unsigned int Dimension = 3;
  typedef double CoordType;

  typedef itk::QuadEdgeMesh< CoordType, Dimension >    MeshType;
  typedef itk::VTKPolyDataReader< MeshType >           MeshReaderType;
  typedef itk::VTKPolyDataWriter< MeshType >           MeshWriterType;

  MeshReaderType::Pointer reader = MeshReaderType::New( );
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update( );
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown while reading the input file " << std::endl;
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  MeshType::Pointer mesh = reader->GetOutput( );

    typedef itk::NumberOfFacesCriterion< MeshType > CriterionType;

    typedef itk::SquaredEdgeLengthDecimationQuadEdgeMeshFilter<
    MeshType, MeshType, CriterionType > DecimationType;

    CriterionType::Pointer criterion = CriterionType::New();
    criterion->SetTopologicalChange( false );
    criterion->SetNumberOfElements( atoi( argv[3]) );

    DecimationType::Pointer decimate = DecimationType::New();
    decimate->SetInput( mesh );
    decimate->SetCriterion( criterion );
    decimate->Update();

    // ** WRITE OUTPUT **
    MeshWriterType::Pointer writer = MeshWriterType::New( );
    writer->SetInput( decimate->GetOutput( ) );
    writer->SetFileName( argv[2] );
    writer->Update();

    return EXIT_SUCCESS;
}
