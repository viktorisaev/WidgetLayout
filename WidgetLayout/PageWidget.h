#pragma once

#include <Widget.h>
#include <Content/Sample3DSceneRenderer.h>	// TODO: remove!!!


class PageWidget : public Widget
{
public:
	PageWidget(const DirectX::XMFLOAT4& _Color);
	virtual ~PageWidget();


	void SetPageWidget(Widget* const _Widget)
	{
		m_PageWidget = _Widget;
	}



	// TODO: visitor or iterator
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowSize& _ParentSize) override;
	WindowSize GetEnvelopSize(const WindowSize& _MaxContentRect) override;
	virtual void BuildWorldPosition(const WindowPos & _ParentPos) override;



private:
	Widget* m_PageWidget;

};

