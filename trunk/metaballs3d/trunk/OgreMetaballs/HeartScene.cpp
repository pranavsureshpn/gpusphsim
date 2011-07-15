#include "HeartScene.h"

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"
#include "HeartField.h"

//-----------------------------------
// HeartScene
//-----------------------------------

HeartScene::HeartScene(void)
{
}

HeartScene::~HeartScene(void)
{
}

void HeartScene::CreateFields()
{
	//Create the scalar fields
	m_finalField = new HeartField();
	m_finalField->SetColor(ColourValue::White);
}

void HeartScene::UpdateFields(float time)
{
}

const ScalarField3D* HeartScene::GetScalarField() const
{
	return m_finalField;
}

float HeartScene::GetSceneSize() const
{
	return 2.0f;
}

float HeartScene::GetSpaceResolution() const
{
	return 0.04f;
}

