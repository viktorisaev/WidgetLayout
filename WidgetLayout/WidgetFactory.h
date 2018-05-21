#pragma once

#include <PageWidget.h>
#include <BoxWidget.h>




class WidgetFactory
{
public:
	WidgetFactory();
	~WidgetFactory();

	static PageWidget* CreatePageWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color);
	static BoxWidget* CreateBoxWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color);

	static void DeleteWidget(Widget* _Widget);

};

