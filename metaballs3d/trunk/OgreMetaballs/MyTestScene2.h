#pragma once

#include "Common.h"
#include "MetaballsAbstractScene.h"
#include "MetaballField.h"
//-----------------------------------
// Forward declarations
//-----------------------------------
class MetaballField;


//-----------------------------------
// MyTestScene2
//-----------------------------------
class MyTestScene2 : public MetaballsAbstractScene
{
public:
	MyTestScene2();
	virtual ~MyTestScene2();

protected:
	virtual void CreateFields() override;
	virtual void UpdateFields(float time) override;

	virtual const ScalarField3D* GetScalarField() const override;

	void createBalls();
	void destroyBalls();

private:
	MetaballField m_finalField;
	//BallList m_balls;

	void createBalls_HardwareBufferPosition();
};
