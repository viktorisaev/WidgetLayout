#include "pch.h"
#include "VerticalStackWidget.h"
#include <WidgetFactory.h>




VerticalStackWidget::VerticalStackWidget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ChildSize) :
  Widget(_Position, _Size, _Color)
, m_ChildSize(_ChildSize)
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
	int32_t y = 0;

	int32_t childWidth = min(_AvailableWindowRect.GetSize().GetWidth(), this->GetAbsRect().GetSize().GetWidth());
//	if (m_ChildSize.GetWidth() > 0)
	{
		childWidth = m_ChildSize.GetWidth();
	}

	WindowSize childSize(childWidth, m_ChildSize.GetHeight());	// get "Auto" size

	DirectX::XMINT2 availPos = _AvailableWindowRect.GetPosition();
	DirectX::XMINT2 containerPos = this->GetAbsRect().GetPosition();
	containerPos.x += availPos.x;
	containerPos.y += availPos.y;
	
	for (Widget* w : m_Widgets)
	{
		WindowSize rect = w->GetRequiredSize(childSize);

		DirectX::XMINT2 newPos = DirectX::XMINT2(containerPos.x, containerPos.y + y);

		WindowRect newRect = WindowRect(newPos, rect);
		w->UpdateLayout(newRect);

		y += rect.GetHeight();
	}


}



WindowSize VerticalStackWidget::GetRequiredSize(const WindowSize & _AvailableSize)
{
	WindowSize w = WindowSize::GetMin(m_AbsRect.GetSize(), _AvailableSize);

	return w;
}





void VerticalStackWidget::AddWidget(Widget * _Widget)
{
	m_Widgets.push_back(_Widget);
}

void VerticalStackWidget::SetChildSize(WindowSize _ChildSize)
{
	m_ChildSize = _ChildSize;
}
