#include "TorusScene.h"

#include "SphericalField.h"
#include "ToroidalField.h"
#include "AdditiveField.h"
#include "PlaneField.h"

//-----------------------------------
// TorusScene
//-----------------------------------

TorusScene::TorusScene(void)
{
}

TorusScene::~TorusScene(void)
{
}

void TorusScene::CreateFields()
{
	//Create the scalar fields
	m_field1 = new SphericalField(Vector3(1.6f,0,0), 0.75f);
	m_field3 = new SphericalField(Vector3(1.6f,0,0), 0.55f);
	m_field4 = new SphericalField(Vector3(1.6f,0,0), 0.45f);
	m_field2 = new ToroidalField(Vector3(0,0,0), 1.6f, 0.55f);

	m_finalField = new AdditiveField();
	m_finalField->AddField(m_field1);
	m_finalField->AddField(m_field2);
	m_finalField->AddField(m_field3);
	m_finalField->AddField(m_field4);
	
	m_field1->SetColor(ColourValue::White);
	m_field2->SetColor(ColourValue(0,1.0f,0));
	m_field3->SetColor(ColourValue(0,0,1.0f));
	m_field4->SetColor(ColourValue(1.0f,0,0));
}

void TorusScene::UpdateFields(float time)
{
	float angularSpeed = 1.5f;
	Vector3 circle = Vector3(cos( angularSpeed * time), 0, sin( angularSpeed * time));
	m_field1->SetCenter(1.6f * circle);

	angularSpeed = 2.0f;
	circle = Vector3(cos( angularSpeed * time), 0, -sin( angularSpeed * time));
	m_field3->SetCenter(-1.6f * circle);

	angularSpeed = 1.75f;
	circle = Vector3(cos( angularSpeed * time), 0, -sin( angularSpeed * time));
	m_field4->SetCenter(1.6f * circle);
}

const ScalarField3D* TorusScene::GetScalarField() const
{
	return m_finalField;
}

float TorusScene::GetSceneSize() const
{
	return 6.0f;
}

float TorusScene::GetSpaceResolution() const
{
	return 0.18f;
}


