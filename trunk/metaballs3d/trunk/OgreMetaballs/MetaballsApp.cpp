#include "MetaballsApp.h"

#include "DynamicMesh.h"
#include "MetaballsFrameListener.h"

//-----------------------------------
// MetaballsApp
//-----------------------------------

MetaballsApp::MetaballsApp()
{
}

MetaballsApp::~MetaballsApp()
{
}

void MetaballsApp::createScene()
{
	//The scene lighting is composed of 3 directional lights and an ambient light
	mSceneMgr->setAmbientLight( 0.2f * ColourValue::White );
/*
	//A red light on the X axis	
	{
		Light* light = mSceneMgr->createLight("Light3");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(1,0,0));
		light->setDiffuseColour(ColourValue(1,0,0));
		light->setSpecularColour(ColourValue(.25, .25, 25.));
	}

	//A green light on the Y axis
	{
		Light* light = mSceneMgr->createLight("Light2");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(0,1,0));
		light->setDiffuseColour(ColourValue(0,1,0));
		light->setSpecularColour(ColourValue(.25, .25, 25.));
	}

	//A blue light on the Z axis
	{
		Light* light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(0,0,1));
		light->setDiffuseColour(ColourValue(0,0,1));
		light->setSpecularColour(ColourValue(.25, .25, .25));
	}
*/

	//A blue light on the Z axis
	{
		Light* light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(0.0f,0.2f,0.5f));
		light->setDiffuseColour(ColourValue(0,0,1));
		light->setSpecularColour(ColourValue(.25, .25, .25));
	}

	//A blue light on the Z axis
	{
		Light* light = mSceneMgr->createLight("Light2");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(0.34f,-0.2f,-0.17f));
		light->setDiffuseColour(ColourValue(0.1f,0.2f,0.8f));
		light->setSpecularColour(ColourValue(.25, .25, .25));
	}

	//A blue light on the Z axis
	{
		Light* light = mSceneMgr->createLight("Light3");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDirection(Vector3(-0.34f,+0.2f,-0.17f));
		light->setDiffuseColour(ColourValue(0.3f,0.2f,0.6f));
		light->setSpecularColour(ColourValue(.25, .25, .25));
	}

	//Create a child node to attach the mesh to
	SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode( "MeshNode" );
	node->setScale(100 * Vector3::UNIT_SCALE);

	//Create a dynamic mesh and attach it to the node
	m_meshBuilder = new DynamicMesh("MyMesh");
	node->attachObject( m_meshBuilder );
}

void MetaballsApp::createFrameListener()
{
	//Create a new frame listener responsible for the scene updates
	mFrameListener = new MetaballsFrameListener(mWindow, mCamera, m_meshBuilder);
	mRoot->addFrameListener(mFrameListener);
}
