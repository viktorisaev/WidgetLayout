#pragma once

#include <Widget.h>
#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!


class PageWidget : public Widget
{
public:
	PageWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color);
	virtual ~PageWidget();


	void SetPageWidget(Widget* _Widget)
	{
		m_PageWidget = _Widget;
	}



	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _ParentWindowRect) override;




private:
	Widget* m_PageWidget;

};

