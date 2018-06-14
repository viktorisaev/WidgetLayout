#include "pch.h"
#include "WidgetFactory.h"


WidgetFactory::WidgetFactory()
{
}


WidgetFactory::~WidgetFactory()
{
}



PageWidget * WidgetFactory::CreatePageWidget(DirectX::XMFLOAT4 _Color)
{
	PageWidget* widget = new PageWidget(_Color);

	return widget;
}



BoxWidget * WidgetFactory::CreateBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color)
{
	BoxWidget* widget = new BoxWidget(_Margin, _Size, _Color);

	return widget;
}



AspectRatioWidget * WidgetFactory::CreateAspectWrappedWidget(WindowSize _RefenerceSize, Widget * const _ContentWidget)
{
	AspectRatioWidget* widget = new AspectRatioWidget(_RefenerceSize, _ContentWidget);

	return widget;
}



StackWidget * WidgetFactory::CreateStackWidget(StackWidget::Direction _Direction, int32_t _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize)
{
	StackWidget* widget = new StackWidget(_Direction, _Margin, _Size, _Color, _ElementSize);

	return widget;
}



void WidgetFactory::DeleteWidget(Widget * _Widget)
{
	delete _Widget;
}
