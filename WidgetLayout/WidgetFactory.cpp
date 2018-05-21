#include "pch.h"
#include "WidgetFactory.h"


WidgetFactory::WidgetFactory()
{
}


WidgetFactory::~WidgetFactory()
{
}



PageWidget * WidgetFactory::CreatePageWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color)
{
	PageWidget* widget = new PageWidget(_Position, _Size, _Color);

	return widget;
}




BoxWidget * WidgetFactory::CreateBoxWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color)
{
	BoxWidget* widget = new BoxWidget(_Position, _Size, _Color);

	return widget;
}




void WidgetFactory::DeleteWidget(Widget * _Widget)
{
	delete _Widget;
}
