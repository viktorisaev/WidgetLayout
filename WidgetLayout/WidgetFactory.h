#pragma once

#include <PageWidget.h>
#include <BoxWidget.h>
#include <VerticalStackWidget.h>
#include <AspectBoxWidget.h>


class WidgetFactory
{
public:
	WidgetFactory();
	~WidgetFactory();

	static PageWidget* CreatePageWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static BoxWidget* CreateBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static AspectBoxWidget* CreateAspectBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static VerticalStackWidget* CreateVerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ChildSize);

	static void DeleteWidget(Widget* _Widget);

};

