/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkFastMarchingImageFilterBase.h"
#include "itkFastMarchingReachedTargetNodesStoppingCriterion.h"

namespace{
// The following class is used to support callbacks
// on the filter in the pipeline that follows later
class ShowProgressObject
{
public:
  ShowProgressObject(itk::ProcessObject* o)
    {m_Process = o;}
  void ShowProgress()
    {std::cout << "Progress " << m_Process->GetProgress() << std::endl;}
  itk::ProcessObject::Pointer m_Process;
};
}

int main(int argc, char* argv[] )
{
  if( argc < 6 )
    {
    std::cerr << argv[0] << " takes 5 arguments" << std::endl;
    std::cerr << "1- speed image" << std::endl;
    std::cerr << "2- nuclei seeds (txt file)" << std::endl;
    std::cerr << "3- membrane seeds (txt file)" << std::endl;
    std::cerr << "4- Topological constraint" << std::endl;
    std::cerr << "    * 0: None" << std::endl;
    std::cerr << "    * 1: Strict" << std::endl;
    std::cerr << "    * 2: No Handle" << std::endl;
    std::cerr << "5- output label image" << std::endl;

    return EXIT_FAILURE;
    }

  // create a fastmarching object
  const unsigned Dimension = 3;

  typedef float                               PixelType;
  typedef itk::Image< PixelType, Dimension >  FloatImageType;

  // read the speed image
  typedef itk::ImageFileReader< FloatImageType> ReaderType;
  typedef ReaderType::Pointer                   ReaderPointer;

  ReaderPointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  FloatImageType::Pointer speedImage = reader->GetOutput();

  FloatImageType::RegionType region = speedImage->GetLargestPossibleRegion();

  FloatImageType::SizeType size = region.GetSize();

  typedef itk::FastMarchingImageFilterBase< FloatImageType, FloatImageType > FastMarchingType;
  FastMarchingType::Pointer fastmarching = FastMarchingType::New();
  fastmarching->SetInput( speedImage );

  typedef FastMarchingType::NodeType     NodeType;
  typedef FastMarchingType::NodePairType NodePairType;
  typedef FastMarchingType::NodePairContainerType NodePairContainerType;

  // setup alive points
  NodePairContainerType::Pointer AlivePoints  = NodePairContainerType::New();
  NodePairContainerType::Pointer TrialPoints  = NodePairContainerType::New();

  typedef FloatImageType::IndexType IndexType;
  typedef FloatImageType::PointType PointType;

  IndexType idx, temp;
  PointType p;

  // ---------------------------------------------------------------------------
  // read the file about the nuclei seed
  std::fstream NucFile;
  NucFile.open( argv[2], std::ios::in );

  unsigned int k = 0;
  double value;

  while( NucFile >> value )
    {
    if( k < Dimension )
      {
      p[k] = value;
      ++k;
      }

    if( k == Dimension )
      {
      // fill p
      speedImage->TransformPhysicalPointToIndex( p, idx );
      AlivePoints->push_back( NodePairType( idx, 0. ) );

      for( unsigned int dim = 0; dim < Dimension; dim++ )
        {
        temp = idx;
        if( temp[dim] > 0 )
          {
          temp[dim] -= 1;
          TrialPoints->push_back( NodePairType( idx, 1. ) );
          }

        if( temp[dim] < size[dim] - 1 )
          {
          temp = idx;
          temp[dim] += 1;
          TrialPoints->push_back( NodePairType( idx, 1. ) );
          }
        }
      k = 0;
      }
    }

  NucFile.close();

  fastmarching->SetAlivePoints( AlivePoints );
  fastmarching->SetTrialPoints( TrialPoints );

  // ---------------------------------------------------------------------------

  std::fstream MembraneSeeds;
  MembraneSeeds.open( argv[3], std::ios::in );

  std::vector< IndexType > TargetNodes;

  k = 0;

  // read the file about the membrane seeds
  while( MembraneSeeds >> value )
    {
    if( k < Dimension )
      {
      p[k] = value;
      ++k;
      }

    if( k == Dimension )
      {
      speedImage->TransformPhysicalPointToIndex( p, idx );
      TargetNodes.push_back( idx );
      k = 0;
      }
    }

  MembraneSeeds.close();

  typedef itk::FastMarchingReachedTargetNodesStoppingCriterion< FloatImageType, FloatImageType >
      CriterionType;

  CriterionType::Pointer criterion = CriterionType::New();
  criterion->SetTargetNodes( TargetNodes );
  criterion->SetTargetCondition( CriterionType::SomeTargets );//AllTargets );
  criterion->SetNumberOfTargetsToBeReached( 40 );

  fastmarching->SetStoppingCriterion( criterion );

  int topology = atoi( argv[4] );

  switch( topology )
    {
    case 0:
      std::cout << "No topological constraint" << std::endl;
      fastmarching->SetTopologyCheck( FastMarchingType::Nothing );
      break;

    default:
    case 1:
      std::cout << "Strict." << std::endl;
      fastmarching->SetTopologyCheck( FastMarchingType::Strict );
      break;

    case 2:
      std::cout << "No handles." << std::endl;
      fastmarching->SetTopologyCheck( FastMarchingType::NoHandles );
      break;
    }

  ShowProgressObject progressWatch(fastmarching);
  itk::SimpleMemberCommand<ShowProgressObject>::Pointer command;
  command = itk::SimpleMemberCommand<ShowProgressObject>::New();
  command->SetCallbackFunction(&progressWatch,
                               &ShowProgressObject::ShowProgress);
  fastmarching->AddObserver( itk::ProgressEvent(), command);

  try
    {
    fastmarching->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::ImageFileWriter< FloatImageType >  LabelImageWriterType;
  typename LabelImageWriterType::Pointer mapWriter = LabelImageWriterType::New();
  mapWriter->SetInput( fastmarching->GetOutput() );
  mapWriter->SetFileName( argv[5] );

  try
    {
    mapWriter->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
