#pragma once

#include <Widget.h>
#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!


class PageWidget : public Widget
{
public:
	PageWidget();
	~PageWidget();

	void SetPageWidget(Widget* _Widget)
	{
		m_PageWidget = _Widget;
	}



	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;




private:
	Widget* m_PageWidget;

};

