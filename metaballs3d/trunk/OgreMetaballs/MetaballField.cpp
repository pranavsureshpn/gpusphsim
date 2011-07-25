#include "MetaballField.h"
#include "SphericalField.h"

//-----------------------------------
// AdditiveField
//-----------------------------------

MetaballField::MetaballField()
{
}

MetaballField::~MetaballField()
{
	std::vector<ScalarField3D*>::iterator iter;
	for(iter = m_fields.begin(); iter < m_fields.end(); ++iter)
	{
		delete *iter;
	}
}

void MetaballField::AddField(ScalarField3D* field)
{
	m_fields.push_back(field);
}

void MetaballField::RemoveField(ScalarField3D* field)
{	
	FieldList::iterator iter = std::find(m_fields.begin(), m_fields.end(), field);
	if(iter != m_fields.end())
	{
		m_fields.erase(iter);
	}
}

float MetaballField::Scalar(const Vector3& position) const
{
	float scalar = 0;
	for(size_t i = 0; i < m_fields.size(); ++i)
	{
		scalar += m_fields[i]->Scalar(position);
	}
	return scalar;
}

Vector3 MetaballField::Gradient(const Vector3& position) const
{
	Vector3 gradient = Vector3::ZERO;
	for(size_t i = 0; i < m_fields.size(); ++i)
	{
		gradient += m_fields[i]->Gradient(position);
	}
	return gradient;
}

ColourValue MetaballField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}
