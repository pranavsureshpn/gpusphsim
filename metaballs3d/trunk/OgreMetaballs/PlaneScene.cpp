#include "PlaneScene.h"

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"

//-----------------------------------
// PlaneScene
//-----------------------------------

PlaneScene::PlaneScene(void)
{
}

PlaneScene::~PlaneScene(void)
{
}

void PlaneScene::CreateFields()
{
	//Create the scalar fields
	m_field1 = new PlaneField(Vector3(0,-1,0), 0);
	SphericalField* sField1 = new SphericalField(Vector3(1.3f,0,0.75f),0.85f);
	SphericalField* sField2 = new SphericalField(Vector3(-0.8f,0,-0.95f),1.15f);

	m_finalField = new AdditiveField();
	m_finalField->AddField(m_field1);
	m_finalField->AddField(sField1);
	m_finalField->AddField(sField2);

	m_field1->SetColor(ColourValue::White);
}

void PlaneScene::UpdateFields(float time)
{
}

const ScalarField3D* PlaneScene::GetScalarField() const
{
	return m_finalField;
}

float PlaneScene::GetSceneSize() const
{
	return 6.0f;
}

float PlaneScene::GetSpaceResolution() const
{
	return 0.2f;
}


