#pragma once

#include <Ogre.h>
#include <OISKeyboard.h>
#include "../OgreSnowSim/SnowSimConfig.h"
//#include "../SPHSimLib/UniformGrid.cuh"

class DynamicMesh;
class MarchingCubesInterface;
class MetaballsAbstractScene;
namespace SnowSim
{
	class OgreSimRenderable;
}

class MetaballScene : public Ogre::FrameListener
{
public:
	MetaballScene(SnowSim::Config *config);
	virtual ~MetaballScene(void);

	void createScene(Ogre::SceneManager *sceneMgr, Ogre::SceneNode *node);
	void createFrameListener(
		Ogre::Root *mRoot, 
		Ogre::RenderWindow* mWindow,
		Ogre::Camera* mCamera,
		const GridParams& gridparam,
		SnowSim::OgreSimRenderable *mParticlesEntity);

	virtual bool keyPressed( const OIS::KeyEvent &arg );
	virtual bool keyReleased( const OIS::KeyEvent &arg );

private:
	DynamicMesh* m_meshBuilder;

protected:

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	virtual bool frameEnded(const Ogre::FrameEvent& evt);

	void ResetScene(const GridParams& gridparam);
	void newFrameListener(
		Ogre::RenderWindow* win, 
		Ogre::Camera* cam, 
		DynamicMesh* meshBuilder,
		const GridParams& gridparam);

private:
	MarchingCubesInterface* m_marchingCube;
	MetaballsAbstractScene* m_scene;

	float m_totalTime;

//	Ogre::Radian m_camAzimuth, m_camPolar;
//	float m_camRadius;

	float m_keyboardDelay;
	float m_keyboardDelayMax;

	int m_currentSceneId;
	int m_nbrScene;

	SnowSim::Config *m_Config;
	SnowSim::OgreSimRenderable *mParticlesEntity;
	bool bShowMCSurface;
	bool bShowSPHParticles;
};