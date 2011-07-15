#pragma once
#include "ScalarField3D.h"

//-----------------------------------
// AdditiveField
//-----------------------------------

typedef std::vector<ScalarField3D*> FieldList;

class AdditiveField :
	public ScalarField3D
{
public:
	AdditiveField();
	virtual ~AdditiveField();

	void AddField(ScalarField3D* field);
	void RemoveField(ScalarField3D* field);

	virtual float Scalar(const Vector3& position) const override;
	virtual Vector3 Gradient(const Vector3& position) const override;
	virtual ColourValue Color(const Vector3& position) const override;

private:
	FieldList m_fields;
};
