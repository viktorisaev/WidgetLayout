#pragma once

#include <Widget.h>

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!

#include <LayoutData.h>


class BoxWidget : public Widget
{
public:
	BoxWidget();

	BoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
	  Widget(DirectX::XMINT2(), _Size, _Color)
	, m_LayoutData(_Margin, _Size)
	{ }

	virtual ~BoxWidget();

	// TODO: visitor or iterator
//	void AddToRender(Sample3DSceneRenderer* _Render) override;	// implementation in base class is enough (position+color)
	void UpdateLayout(const WindowRect& _VisibleRect) override;
	WindowSize GetEnvelopSize(const WindowSize& _ParentContentRect) override;

	void SetSize(const WindowSize& _Size);

protected:
	LayoutData m_LayoutData;	// rect + margin
};

