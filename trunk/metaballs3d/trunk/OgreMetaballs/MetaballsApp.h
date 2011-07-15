#pragma once

#include "Common.h"
#include "ExampleApplication.h"

//-----------------------------------
// Forward declarations
//-----------------------------------

class DynamicMesh;

//-----------------------------------
// MetaballsApp
//-----------------------------------

class MetaballsApp : public ExampleApplication
{
public:
	MetaballsApp(void);
	~MetaballsApp(void);

protected:
	virtual void createScene() override;
	virtual void createFrameListener() override;

private:
	DynamicMesh* m_meshBuilder;
};
