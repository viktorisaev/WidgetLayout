#pragma once

#include <Widget.h>

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!



class BoxWidget : public Widget
{
public:
	BoxWidget();

	BoxWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color) :
		Widget(_Position, _Size, _Color)
	{ }

	virtual ~BoxWidget();

	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _ParentWindowRect) override;

};

