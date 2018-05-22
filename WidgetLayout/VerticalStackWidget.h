#pragma once

#include <vector>
#include "Widget.h"


class VerticalStackWidget :	public Widget
{
public:
	VerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ChildSize);
	virtual ~VerticalStackWidget();


	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _AvailableWindowRect) override;
	WindowSize GetRequiredSize(const WindowSize& _AvailableSize) override;

	void AddWidget(Widget* _Widget);

	void SetChildSize(WindowSize _ChildSize);


private:
	std::vector<Widget*> m_Widgets;

	WindowSize m_ChildSize;
};

