#include "MetaballsFrameListener.h"

#include "DynamicMesh.h"
#include "MarchingCubesImpl.h"
#include "ScalarField3D.h"
#include "TorusScene.h"
#include "PlaneScene.h"
#include "HeartScene.h"
#include "CascadeScene.h"

//-----------------------------------
// MetaballsFrameListener
//-----------------------------------

MetaballsFrameListener::MetaballsFrameListener(RenderWindow* win, Camera* cam, DynamicMesh* meshBuilder) : ExampleFrameListener(win, cam)
{
	m_totalTime = 0;

	m_meshBuilder = meshBuilder;

	m_scene = NULL;

	m_nbrScene = 2;
	m_currentSceneId = 0;

	ResetScene(m_currentSceneId);

	//Initialize the camera coordinates
	m_camAzimuth = 0;
	m_camPolar = 0;

	m_keyboardDelayMax = 2.0f;
	m_keyboardDelay = 0; 
}

MetaballsFrameListener::~MetaballsFrameListener(void)
{
}

void MetaballsFrameListener::moveCamera()
{
	//Change the camera coordinates according to the mouse input
	m_camAzimuth += mRotX;
	m_camPolar += mRotY;

	//Block the camera over the poles
	const Ogre::Radian maxPolarAngle = Ogre::Radian(Ogre::Math::HALF_PI) - Radian(0.01f);
	if(m_camPolar > maxPolarAngle)
		m_camPolar = maxPolarAngle;
	if(m_camPolar < -maxPolarAngle)
		m_camPolar = -maxPolarAngle;

	//Compute the new camera position using spherical coordinates
	Vector3 camPos = m_camRadius * Vector3(
		Ogre::Math::Cos(m_camAzimuth) * Ogre::Math::Cos(m_camPolar),
		Ogre::Math::Sin(m_camPolar),
		Ogre::Math::Sin(m_camAzimuth) * Ogre::Math::Cos(m_camPolar));

	mCamera->setPosition(camPos);
	mCamera->lookAt(Vector3::ZERO);
}

void MetaballsFrameListener::ResetScene(int sceneId)
{
	if(m_scene != NULL)
	{
		delete m_scene;
	}

	m_currentSceneId = sceneId % m_nbrScene;

	switch (m_currentSceneId)
	{  
	case 0:
		m_scene = new CascadeScene();
		break;
	case 1:
		m_scene = new TorusScene();
		break;
	default:
		m_scene = new CascadeScene();
		break;
	}

	m_scene->CreateFields();

	//Create the object responsible for the mesh creation
	m_marchingCube = new MarchingCubesImpl(m_meshBuilder);
	m_marchingCube->SetScalarField(m_scene->GetScalarField());
	m_marchingCube->Initialize(m_scene->GetSceneSize(), m_scene->GetSpaceResolution(), 1);

    m_camRadius = 140 * m_scene->GetSceneSize();
}

bool MetaballsFrameListener::frameStarted(const FrameEvent& evt)
{		
	m_keyboardDelay -= evt.timeSinceLastFrame;

	if(!mKeyboard->isKeyDown(OIS::KC_SPACE))
	{
		m_totalTime += evt.timeSinceLastFrame;
	}

	if(mKeyboard->isKeyDown(OIS::KC_T))
	{
		mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
	}
	else
	{
		mCamera->setPolygonMode(Ogre::PM_SOLID);
	} 

	if(mKeyboard->isKeyDown(OIS::KC_ADD) && m_keyboardDelay<0)
	{
		ResetScene(m_currentSceneId+1);
		m_keyboardDelay = m_keyboardDelayMax;
	}
	if(mKeyboard->isKeyDown(OIS::KC_MINUS) && m_keyboardDelay<0)
	{
		ResetScene(m_currentSceneId-1);
		m_keyboardDelay = m_keyboardDelayMax;
	}

	if(!mKeyboard->isKeyDown(OIS::KC_MINUS) && !mKeyboard->isKeyDown(OIS::KC_ADD))
	{
		m_keyboardDelay = 0;
	}

	m_scene->UpdateFields(m_totalTime);

	//Recreate the mesh
	m_marchingCube->CreateMesh();

	return ExampleFrameListener::frameStarted(evt);        
}

bool MetaballsFrameListener::frameEnded(const FrameEvent& evt)
{
	return ExampleFrameListener::frameEnded(evt);        
}