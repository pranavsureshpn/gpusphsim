#pragma once

#include <Ogre.h>

class DynamicMesh;
class MetaballsFrameListenerAdapter;

class MetaballsAppAdapter
{
public:
	MetaballsAppAdapter(void);
	virtual ~MetaballsAppAdapter(void);

	void createScene(Ogre::SceneManager *sceneMgr);
	void createFrameListener(Ogre::Root *mRoot, Ogre::RenderWindow* mWindow, Ogre::Camera* mCamera);

private:
	DynamicMesh* m_meshBuilder;
	MetaballsFrameListenerAdapter *mFrameListener;
};
