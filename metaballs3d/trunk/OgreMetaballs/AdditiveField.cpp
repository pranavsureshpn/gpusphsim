#include "AdditiveField.h"

//-----------------------------------
// AdditiveField
//-----------------------------------

AdditiveField::AdditiveField()
{
}

AdditiveField::~AdditiveField()
{
}

void AdditiveField::AddField(ScalarField3D* field)
{
	m_fields.push_back(field);
}

void AdditiveField::RemoveField(ScalarField3D* field)
{	
	FieldList::iterator iter = std::find(m_fields.begin(), m_fields.end(), field);
	if(iter != m_fields.end())
	{
		m_fields.erase(iter);
	}
}

float AdditiveField::Scalar(const Vector3& position) const
{
	float scalar = 0;
	for(size_t i = 0; i < m_fields.size(); ++i)
	{
		scalar += m_fields[i]->Scalar(position);
	}
	return scalar;
}

Vector3 AdditiveField::Gradient(const Vector3& position) const
{
	Vector3 gradient = Vector3::ZERO;
	for(size_t i = 0; i < m_fields.size(); ++i)
	{
		gradient += m_fields[i]->Gradient(position);
	}
	return gradient;
}

ColourValue AdditiveField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}
