#pragma once

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!

#include <WindowSize.h>


// interface to Widget
class IWidget
{
public:

	// TODO: visitor or iterator
	virtual void AddToRender(Sample3DSceneRenderer* _Render) = 0;

	virtual void UpdateLayout(const WindowRect& _VisibleRect) = 0;

	virtual WindowSize GetRequiredSize(const WindowSize& _MaxContentRect) = 0;


};

