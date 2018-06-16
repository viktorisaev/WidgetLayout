#pragma once

#include <Widget.h>

//#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!

#include <LayoutData.h>


class BoxWidget : public Widget
{
public:
	BoxWidget();

	BoxWidget(int _Margin, const WindowSize& _WindowDefaultSize, const DirectX::XMFLOAT4& _Color, UINT _TextureIndex) :
	  Widget(_WindowDefaultSize, _Color)
	, m_LayoutData(_Margin)
	, m_TextureIndex(_TextureIndex)
	{ }

	virtual ~BoxWidget();

	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;	// special implementation, not enough in base class (position+color) - add texture
	void UpdateLayout(const WindowSize& _ParentSize) override;
	WindowSize GetEnvelopSize(const WindowSize& _ParentContentRect) override;
	virtual void BuildWorldPosition(const WindowPos & _ParentPos) override;


	void SetSize(const WindowSize& _Size);

protected:
	LayoutData m_LayoutData; // rect + margin
	WindowSize m_EnvelopSize;
	UINT m_TextureIndex;
};

