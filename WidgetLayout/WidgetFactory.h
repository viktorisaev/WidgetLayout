#pragma once

#include <PageWidget.h>
#include <BoxWidget.h>
#include <StackWidget.h>
#include <AspectRatioWidget.h>



class WidgetFactory
{
public:
	WidgetFactory();
	~WidgetFactory();

	static PageWidget* CreatePageWidget(DirectX::XMFLOAT4 _Color);
	static BoxWidget* CreateBoxWidget(int _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color);
	static AspectRatioWidget* CreateAspectWrappedWidget(WindowSize _RefenerceSize, Widget * const _ContentWidget);
	static StackWidget* CreateStackWidget(StackWidget::Direction _Direction, int32_t _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize);

	static void DeleteWidget(Widget* _Widget);

};

