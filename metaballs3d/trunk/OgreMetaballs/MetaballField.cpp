#include "MetaballField.h"
#include "SphericalField.h"
#include <boost/math/special_functions/math_fwd.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
//-----------------------------------
// AdditiveField
//-----------------------------------

MetaballField::MetaballField()
{
	m_radiusSquared = -1.0f;
}

MetaballField::~MetaballField()
{
// 	std::vector<ScalarField3D*>::iterator iter;
// 	for(iter = m_fields.begin(); iter < m_fields.end(); ++iter)
// 	{
// 		delete *iter;
// 	}
}

void MetaballField::AddField(ScalarField3D* field)
{
	assert(0&&"MetaballField::AddField() is abandoned");
//	m_fields.push_back(field);
}

void MetaballField::RemoveField(ScalarField3D* field)
{	
	assert(0&&"MetaballField::RemoveField() is abandoned");
}

// float MetaballField::Scalar(const Vector3& position) const
// {
// 	float scalar = 0;
// 	for(size_t i = 0; i < m_fields.size(); ++i)
// 	{
// 		scalar += m_fields[i]->Scalar(position);
// 	}
// 	return scalar;
// }
float MetaballField::Scalar(const Vector3& position) const
{
	assert(m_radiusSquared!=-1.0f);
	const std::size_t size = m_spherePosition.size();

	// Sum = E( r/|p[i]-position|^2 )
	float scalar = 0;
	for(size_t i = 0; i < size; ++i)
	{
		scalar += m_radiusSquared / position.squaredDistance(m_spherePosition[i]);
	}
	//printf("m_sphereRaidus:%f\n", m_radiusSquared);
	return scalar;
}
Vector3 MetaballField::Gradient(const Vector3& position) const
{
	assert(m_sphereRaidus!=-1.0f);
	const std::size_t size = m_spherePosition.size();

	Vector3 gradient = Vector3::ZERO;
	for(size_t i = 0; i < size; ++i)
	{
		Vector3 relativePos = m_spherePosition[i] - position;
		float lengthSquared = relativePos.squaredLength();

		gradient += (2 * m_radiusSquared / (lengthSquared*lengthSquared)) * relativePos;
	}
	return gradient;
}

ColourValue MetaballField::Color(const Vector3& position) const
{
	//The color is uniform
	return m_color;
}

void MetaballField::SetFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size)
{
	printf("MetaballField::SetFieldBuffer(%d)\n", size);
	assert(ogreHardwareBuffer && size>0);
	m_spherePosition.resize(size);
	for(std::size_t i=0; i<size; ++i)
	{
		m_spherePosition[i].x = ogreHardwareBuffer[i].x;
		m_spherePosition[i].y = ogreHardwareBuffer[i].y;
		m_spherePosition[i].z = ogreHardwareBuffer[i].z;
	}
}
void MetaballField::UpdateFieldBuffer(Ogre::Vector4* &ogreHardwareBuffer, const std::size_t size)
{
	//printf("MetaballField::UpdateFieldBuffer(%d)\n", size);
	assert(ogreHardwareBuffer && m_spherePosition.size()==size);

	for(std::size_t i=0; i<size; ++i)
	{
		if( boost::math::isnan(m_spherePosition[i].x) 
			|| boost::math::isnan(m_spherePosition[i].y) 
			|| boost::math::isnan(m_spherePosition[i].z) )
		{
			printf("buf[%d]=<%f, %f, %f>\n", i,m_spherePosition[i].x, m_spherePosition[i].y, m_spherePosition[i].z);
			continue;
		}
		m_spherePosition[i].x = ogreHardwareBuffer[i].x;
		m_spherePosition[i].y = ogreHardwareBuffer[i].y;
		m_spherePosition[i].z = ogreHardwareBuffer[i].z;
		//printf("buf[%d]=<%f, %f, %f>\n", i,m_spherePosition[i].x, m_spherePosition[i].y, m_spherePosition[i].z);
	}
}