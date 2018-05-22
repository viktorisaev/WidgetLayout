#include "pch.h"
#include "WidgetFactory.h"


WidgetFactory::WidgetFactory()
{
}


WidgetFactory::~WidgetFactory()
{
}



PageWidget * WidgetFactory::CreatePageWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color)
{
	PageWidget* widget = new PageWidget(_Position, _Size, _Color);

	return widget;
}




BoxWidget * WidgetFactory::CreateBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color)
{
	BoxWidget* widget = new BoxWidget(_Margin, _Size, _Color);

	return widget;
}

AspectBoxWidget * WidgetFactory::CreateAspectBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color)
{
	AspectBoxWidget* widget = new AspectBoxWidget(_Margin, _Size, _Color);

	return widget;
}




VerticalStackWidget * WidgetFactory::CreateVerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ChildSize)
{
	VerticalStackWidget* widget = new VerticalStackWidget(_Position, _Size, _Color, _ChildSize);

	return widget;
}




void WidgetFactory::DeleteWidget(Widget * _Widget)
{
	delete _Widget;
}
