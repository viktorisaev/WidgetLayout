#pragma once

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!


// interface to Widget
class IWidget
{
public:

	// TODO: visitor or iterator
	virtual void AddToRender(Sample3DSceneRenderer* _Render) = 0;


};

