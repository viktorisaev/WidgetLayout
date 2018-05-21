#pragma once

#include <vector>
#include "Widget.h"


class VerticalStackWidget :	public Widget
{
public:
	VerticalStackWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color);
	virtual ~VerticalStackWidget();


	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _ParentWindowRect) override;

	void AddWidget(Widget* _Widget);


private:
	std::vector<Widget*> m_Widgets;
};
