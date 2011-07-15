#pragma once

#include "Common.h"
#include "ExampleApplication.h"

//-----------------------------------
// Forward declarations
//-----------------------------------

class DynamicMesh;
class MarchingCubesImpl;
class ScalarField3D;
class MetaballsAbstractScene;

//-----------------------------------
// MetaballsFrameListener
//-----------------------------------

class MetaballsFrameListener : public ExampleFrameListener
{
public:
	MetaballsFrameListener(RenderWindow* win, Camera* cam, DynamicMesh* meshBuilder);
	~MetaballsFrameListener(void);

protected:
	virtual void moveCamera() override;

	bool frameStarted(const FrameEvent& evt) override;
	bool frameEnded(const FrameEvent& evt) override;

	void ResetScene(int sceneId);

private:
	DynamicMesh* m_meshBuilder;
	MarchingCubesImpl* m_marchingCube;
	MetaballsAbstractScene* m_scene;

	float m_totalTime;

	Ogre::Radian m_camAzimuth, m_camPolar;
	float m_camRadius;

	float m_keyboardDelay;
	float m_keyboardDelayMax;

	int m_currentSceneId;
	int m_nbrScene;
};
