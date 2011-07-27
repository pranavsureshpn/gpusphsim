#include "MetaballScene.h"

#include "./trunk/OgreMetaballs/DynamicMesh.h"
//#include "./trunk/OgreMetaballs/MarchingCubesImpl.h"
#include "./trunk/OgreMetaballs/MarchingCubesImplWithCuda.h"
#include "./trunk/OgreMetaballs/ScalarField3D.h"
//#include "./trunk/OgreMetaballs/CascadeScene.h"
#include "./trunk/OgreMetaballs/MyTestScene.h"
#include "./trunk/OgreMetaballs/MyTestScene2.h"
#include "../OgreSnowSim/OgreSimRenderable.h"
#include "../OgreSnowSim/OgreSimBuffer.h"
#include "MetaballCudaMgr.cuh"

extern float ScaleFactor;

//-----------------------------------
// MetaballScene
//-----------------------------------

MetaballScene::MetaballScene(SnowSim::Config *config)
:m_meshBuilder(NULL),
m_Config(config),
m_marchingCube(NULL),
m_scene(NULL),
bShowMCSurface(false),
bShowSPHParticles(true)
{
}

MetaballScene::~MetaballScene()
{
	if(m_marchingCube){
		m_marchingCube->unInitialize();
		delete m_marchingCube;
		m_marchingCube = NULL;
	}
	if(m_scene != NULL)
	{
		delete m_scene;
	}
}

void MetaballScene::createScene(Ogre::SceneManager *mSceneMgr, Ogre::SceneNode *node)
{
	//The scene lighting is composed of 3 directional lights and an ambient light
//	mSceneMgr->setAmbientLight( 0.2f * ColourValue::White );
// 	{
// 		Light* light = mSceneMgr->createLight("Light1");
// 		light->setType(Light::LT_DIRECTIONAL);
// 		light->setDirection(Vector3(0.0f,0.2f,0.5f));
// 		light->setDiffuseColour(ColourValue(0,0,1));
// 		light->setSpecularColour(ColourValue(.25, .25, .25));
// 	}
// 
// 	//A blue light on the Z axis
// 	{
// 		Light* light = mSceneMgr->createLight("Light2");
// 		light->setType(Light::LT_DIRECTIONAL);
// 		light->setDirection(Vector3(0.34f,-0.2f,-0.17f));
// 		light->setDiffuseColour(ColourValue(0.1f,0.2f,0.8f));
// 		light->setSpecularColour(ColourValue(.25, .25, .25));
// 	}
// 
// 	//A blue light on the Z axis
// 	{
// 		Light* light = mSceneMgr->createLight("Light3");
// 		light->setType(Light::LT_DIRECTIONAL);
// 		light->setDirection(Vector3(-0.34f,+0.2f,-0.17f));
// 		light->setDiffuseColour(ColourValue(0.3f,0.2f,0.6f));
// 		light->setSpecularColour(ColourValue(.25, .25, .25));
// 	}

	//Create a child node to attach the mesh to
//	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "MeshNode" );
//	node->setScale(Vector3::UNIT_SCALE);

	//Create a dynamic mesh and attach it to the node
	m_meshBuilder = new DynamicMesh("MyMesh");
	node->attachObject( m_meshBuilder );
}

void MetaballScene::createFrameListener(
	Ogre::Root *mRoot, Ogre::RenderWindow* mWindow, Ogre::Camera* mCamera,
	const GridParams& gridparam,
	SnowSim::OgreSimRenderable *particlesEntity)
{
	mParticlesEntity = particlesEntity;

	//Create a new frame listener responsible for the scene updates
	newFrameListener(mWindow, mCamera, m_meshBuilder, gridparam);
	mRoot->addFrameListener(this);

}
void MetaballScene::newFrameListener(
	Ogre::RenderWindow* win, Ogre::Camera* cam, DynamicMesh* meshBuilder,
	const GridParams& gridparam)
{
	m_totalTime = 0;

	m_meshBuilder = meshBuilder;

	m_scene = NULL;

	m_nbrScene = 5;
	//m_currentSceneId = 0;//please define CPU_SAMPLER in MarchingCubesImplWithCuda::SampleSpace()
	m_currentSceneId = 4;  //please undefine CPU_SAMPLER in MarchingCubesImplWithCuda::SampleSpace()

	ResetScene(gridparam);

	//Initialize the camera coordinates
//	m_camAzimuth = 0;
//	m_camPolar = 0;

	m_keyboardDelayMax = 2.0f;
	m_keyboardDelay = 0; 
}

void MetaballScene::ResetScene(const GridParams& gridparam)
{
	const int sceneId = m_currentSceneId;

	if(m_scene != NULL)
	{
		delete m_scene;
	}

	m_currentSceneId = sceneId % m_nbrScene;

	switch (m_currentSceneId)
	{  
	case 0:
		{
			m_scene = new MyTestScene();
			m_scene->SetParticleRadius(m_Config->marchingcubeSettings.BaseRadius*ScaleFactor);
			m_scene->SetSpaceResolution(m_Config->marchingcubeSettings.SpaceResolution*ScaleFactor);
			m_scene->SetSceneSize(gridparam.grid_size.x);
		}
		break;
	case 1:
		m_scene = new CascadeScene();
		m_scene->SetParticleRadius(0.5f*ScaleFactor*10.0f);
		m_scene->SetSpaceResolution(0.09f*ScaleFactor);
		m_scene->SetSceneSize(4.0f*ScaleFactor);
		break;
// 	case 2:
// 		m_scene = new TorusScene();
//		m_scene->SetParticleRadius(?);
//		m_scene->SetSpaceResolution(?);
//		m_scene->SetSceneSize(6.0);
// 		break;
// 	case 3:
// 		m_scene = new PlaneScene();
//		m_scene->SetParticleRadius(?);
//		m_scene->SetSpaceResolution(?);
//		m_scene->SetSceneSize(6.0);
// 		break;
  	case 4:
		{
			m_scene = new MyTestScene2();
			m_scene->SetParticleRadius(m_Config->marchingcubeSettings.BaseRadius*ScaleFactor);
			m_scene->SetSpaceResolution(m_Config->marchingcubeSettings.SpaceResolution*ScaleFactor);
			m_scene->SetSceneSize(gridparam.grid_size.x);
			MetaballCudaMgr::getSingletonPtr()->SetThreadsPerBlock(m_Config->marchingcubeSettings.ThreadsPerBlock);
		}
  		break;
	default:
		assert(0);
		break;
	}
	m_scene->SetParticlesEntity(mParticlesEntity);
	//m_scene->SetSceneSize(m_Config->terrainSettings.size);
	m_scene->CreateFields();


	//Create the object responsible for the mesh creation
	//m_marchingCube = new MarchingCubesImpl(m_meshBuilder);
	m_marchingCube = new MarchingCubesImplWithCuda(m_meshBuilder);
	m_marchingCube->SetScalarField(m_scene->GetScalarField());
	m_marchingCube->Initialize(m_scene->GetSceneSize(), m_scene->GetSpaceResolution(), 1);
//	m_camRadius = 140 * m_scene->GetSceneSize();
}


bool MetaballScene::frameStarted(const Ogre::FrameEvent& evt)
{		
 	m_keyboardDelay -= evt.timeSinceLastFrame;
// 
// 	if(!mKeyboard->isKeyDown(OIS::KC_SPACE))
// 	{
 		m_totalTime += evt.timeSinceLastFrame;
// 	}
// 
// 	if(mKeyboard->isKeyDown(OIS::KC_T))
// 	{
// 		mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
// 	}
// 	else
// 	{
// 		mCamera->setPolygonMode(Ogre::PM_SOLID);
// 	} 
// 
// 	if(mKeyboard->isKeyDown(OIS::KC_ADD) && m_keyboardDelay<0)
// 	{
// 		ResetScene(m_currentSceneId+1);
// 		m_keyboardDelay = m_keyboardDelayMax;
// 	}
// 	if(mKeyboard->isKeyDown(OIS::KC_MINUS) && m_keyboardDelay<0)
// 	{
// 		ResetScene(m_currentSceneId-1);
// 		m_keyboardDelay = m_keyboardDelayMax;
// 	}
// 
// 	if(!mKeyboard->isKeyDown(OIS::KC_MINUS) && !mKeyboard->isKeyDown(OIS::KC_ADD))
// 	{
// 		m_keyboardDelay = 0;
// 	}

	if(bShowMCSurface)
	{
		m_scene->UpdateFields(m_totalTime);

		//Recreate the mesh
		m_marchingCube->CreateMesh();
		//m_marchingCube->GetMeshBuilder()->setVisible(true);
	}else{
		//m_marchingCube->GetMeshBuilder()->setVisible(false);
	}

	return true;
}

bool MetaballScene::frameEnded(const Ogre::FrameEvent& evt)
{
	return true;
}

bool MetaballScene::keyPressed( const OIS::KeyEvent &arg )
{
	printf("MetaballScene::keyPressed()\n");
	if (arg.key == OIS::KC_F)   // toggle visibility of advanced frame stats
	{
		bShowMCSurface = !bShowMCSurface;
		printf("bShowMCSurface=%d\n",bShowMCSurface);
		m_marchingCube->GetMeshBuilder()->setVisible(bShowMCSurface);
	}
	if (arg.key == OIS::KC_G)   // toggle visibility of advanced frame stats
	{
		bShowSPHParticles = !bShowSPHParticles;
		printf("bShowSPHParticles=%d\n",bShowSPHParticles);
		mParticlesEntity->setVisible(bShowSPHParticles);
	}
	return true;
}
bool MetaballScene::keyReleased( const OIS::KeyEvent &arg )
{
	printf("MetaballScene::keyReleased()\n");

	return true;
}