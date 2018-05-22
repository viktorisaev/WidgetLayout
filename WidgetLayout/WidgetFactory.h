#pragma once

#include <PageWidget.h>
#include <BoxWidget.h>
#include <VerticalStackWidget.h>



class WidgetFactory
{
public:
	WidgetFactory();
	~WidgetFactory();

	static PageWidget* CreatePageWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static BoxWidget* CreateBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static VerticalStackWidget* CreateVerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color);

	static void DeleteWidget(Widget* _Widget);

};

