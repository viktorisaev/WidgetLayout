#include "pch.h"
#include "VerticalStackWidget.h"
#include <WidgetFactory.h>




VerticalStackWidget::VerticalStackWidget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color) :
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
	_Render->AddColoredRectToList(this->GetRect(), this->GetColor());

	for (Widget* w : m_Widgets)
	{
		w->AddToRender(_Render);
	}
}

void VerticalStackWidget::UpdateLayout(const WindowRect & _ParentWindowRect)
{
	UINT y = 0;
	
	for (Widget* w : m_Widgets)
	{
		const WindowRect& rect = w->GetRect();

		DirectX::XMINT2 newPos = DirectX::XMINT2(rect.GetPosition().x, y);

		w->SetPosition(newPos);

		y += rect.GetSize().y;
	}

	for (Widget* w : m_Widgets)
	{
		w->UpdateLayout(_ParentWindowRect);
	}

}





void VerticalStackWidget::AddWidget(Widget * _Widget)
{
	m_Widgets.push_back(_Widget);
}
