#include "pch.h"
#include "VerticalStackWidget.h"
#include <WidgetFactory.h>




VerticalStackWidget::VerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
	Widget(_Position, _Size, _Color)
{
}




VerticalStackWidget::~VerticalStackWidget()
{
	for (Widget* w : m_Widgets)
	{
		WidgetFactory::DeleteWidget(w);
	}
}





void VerticalStackWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(this->GetAbsRect(), this->GetColor());

	for (Widget* w : m_Widgets)
	{
		w->AddToRender(_Render);
	}
}

void VerticalStackWidget::UpdateLayout(const WindowRect & _AvailableWindowRect)
{
	UINT y = 0;
	
	for (Widget* w : m_Widgets)
	{
		const WindowRect& rect = w->GetAbsRect();

		DirectX::XMINT2 newPos = DirectX::XMINT2(_AvailableWindowRect.GetPosition().x, y);

		WindowRect newRect = WindowRect(newPos, rect.GetSize());
		w->UpdateLayout(newRect);

		y += rect.GetSize().GetHeight();
	}


}





void VerticalStackWidget::AddWidget(Widget * _Widget)
{
	m_Widgets.push_back(_Widget);
}
