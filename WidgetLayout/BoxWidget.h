#pragma once

#include <Widget.h>

#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!

#include <LayoutData.h>


class BoxWidget : public Widget
{
public:
	BoxWidget();

	BoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
	  Widget(_Color)
	, m_LayoutData(_Margin, _Size)
	{ }

	virtual ~BoxWidget();

	// TODO: visitor or iterator
//	void AddToRender(Sample3DSceneRenderer* _Render) override;	// implementation in base class is enough (position+color)
	void UpdateLayout(const WindowSize& _ParentSize) override;
	WindowSize GetEnvelopSize(const WindowSize& _ParentContentRect) override;
	virtual void BuildWorldPosition(const WindowPos & _ParentPos) override;


	void SetSize(const WindowSize& _Size);

protected:
	LayoutData m_LayoutData; // rect + margin
	WindowSize m_EnvelopSize;
};

