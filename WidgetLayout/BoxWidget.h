#pragma once

#include <Widget.h>

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!



class BoxWidget : public Widget
{
public:
	BoxWidget();

	BoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
	  Widget(DirectX::XMINT2(), _Size, _Color)
	, m_Margin(_Margin)
	, m_DefaultSize(_Size)
	{ }

	virtual ~BoxWidget();

	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _AvailableWindowRect) override;
	WindowSize GetRequiredSize(const WindowSize& _AvailableSize) override;

	void SetSize(const WindowSize& _AvailableSize);

protected:
	int m_Margin;
	WindowSize m_DefaultSize;	// default size

};

