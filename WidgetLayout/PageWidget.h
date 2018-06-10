#pragma once

#include <Widget.h>
#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!


class PageWidget : public Widget
{
public:
	PageWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	virtual ~PageWidget();


	void SetPageWidget(Widget* const _Widget)
	{
		m_PageWidget = _Widget;
	}



	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowRect& _VisibleRect) override;
	WindowSize GetEnvelopSize(const WindowSize& _MaxContentRect) override;





private:
	Widget* m_PageWidget;

};

