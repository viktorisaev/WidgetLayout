#pragma once

#include <vector>
#include <Widget.h>
#include <LayoutData.h>


class VerticalStackWidget :	public Widget
{
public:
	VerticalStackWidget(int32_t _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize);
	virtual ~VerticalStackWidget();


	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _VisibleRect) override;
	WindowSize GetRequiredSize(const WindowSize& _MaxContentRect) override;

	void AddWidget(Widget* _Widget);

protected:
	WindowSize GetChildrenSize(const WindowSize& _ElementRect);


private:
	std::vector<Widget*> m_Widgets;

protected:
	LayoutData m_LayoutData;	// rect + margin
	WindowSize m_ElementSize;	// size of one element of the stack or ENVELOP_CHILD
};

