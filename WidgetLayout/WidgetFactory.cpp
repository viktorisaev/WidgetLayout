#include "pch.h"
#include "WidgetFactory.h"


WidgetFactory::WidgetFactory()
{
}


WidgetFactory::~WidgetFactory()
{
}



PageWidget * WidgetFactory::CreatePageWidget(const DirectX::XMFLOAT4& _Color)
{
	PageWidget* widget = new PageWidget(_Color);

	return widget;
}



BoxWidget * WidgetFactory::CreateBoxWidget(int _Margin, const WindowSize& _WindowDefaultSize, const DirectX::XMFLOAT4& _Color, UINT _TextureIndex)
{
	BoxWidget* widget = new BoxWidget(_Margin, _WindowDefaultSize, _Color, _TextureIndex);

	return widget;
}



AspectRatioWidget * WidgetFactory::CreateAspectWrappedWidget(WindowSize _RefenerceSize, Widget * const _ContentWidget)
{
	AspectRatioWidget* widget = new AspectRatioWidget(_RefenerceSize, _ContentWidget);

	return widget;
}



StackWidget * WidgetFactory::CreateStackWidget(StackWidget::Direction _Direction, int32_t _Margin, const WindowSize& _WindowDefaultSize, const DirectX::XMFLOAT4& _Color, WindowSize _ElementSize)
{
	StackWidget* widget = new StackWidget(_Direction, _Margin, _WindowDefaultSize, _Color, _ElementSize);

	return widget;
}



void WidgetFactory::DeleteWidget(Widget * _Widget)
{
	delete _Widget;
}
