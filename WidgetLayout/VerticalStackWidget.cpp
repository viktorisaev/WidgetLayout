#include "pch.h"
#include "VerticalStackWidget.h"
#include <WidgetFactory.h>




VerticalStackWidget::VerticalStackWidget(int32_t _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize) :
  Widget(DirectX::XMINT2(0,0), _Size, _Color)
, m_LayoutData(_Margin, _Size)
, m_ElementSize(_ElementSize)
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



void VerticalStackWidget::UpdateLayout(const WindowRect& _VisibleRect)
{
	// 1) calculate content size
	WindowSize contentSize = GetChildrenSize(m_ElementSize);

	// margin
	DirectX::XMINT2 rectPos = _VisibleRect.GetPosition();
	rectPos.x += m_LayoutData.GetMargin();
	rectPos.y += m_LayoutData.GetMargin();

	// set draw rect
	Widget::SetPosition(rectPos);

	WindowSize size = m_LayoutData.GetRequiredSizeWithChildrenAndParent(contentSize, _VisibleRect.GetSize());

	Widget::SetSize(size);

	// 2) layout content
//	WindowSize singleChildSize = WindowSize::GetMin(, size);
	
	int32_t childY = 0;
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize rect = childWidget->GetRequiredSize(m_ElementSize);

		rect.SetWidth(min( rect.GetWidth(), size.GetWidth() ));

		DirectX::XMINT2 newPos = DirectX::XMINT2(rectPos.x, rectPos.y + childY);

		if (rect.GetHeight() == ENVELOP_CHILD)
		{
			rect.SetHeight(0);	// can't draw child of 'fit parent' if element size is not defined
		}

		WindowRect newRect = WindowRect( newPos , rect );
		childWidget->UpdateLayout( newRect );

		childY += rect.GetHeight();
	}
}




WindowSize VerticalStackWidget::GetChildrenSize(const WindowSize& _ElementRect)
{
	// calculate children container size
	int32_t w = 0;
	int32_t h = 0;
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize rect = childWidget->GetRequiredSize(_ElementRect);

		if (rect.GetWidth() != ENVELOP_CHILD)
		{
			w = max(w, rect.GetWidth());
		}

		if (rect.GetHeight() != ENVELOP_CHILD)
		{
			h += rect.GetHeight();
		}
	}

	return WindowSize(w, h);
}



WindowSize VerticalStackWidget::GetRequiredSize(const WindowSize & _MaxContentRect)
{
	WindowSize contentSize = GetChildrenSize(m_ElementSize);
	WindowSize envelopSize = m_LayoutData.GetEnvelopSize(contentSize);

	return envelopSize;
}


void VerticalStackWidget::AddWidget(Widget * _Widget)
{
	m_Widgets.push_back(_Widget);
}



void VerticalStackWidget::SetElementSize(const WindowSize& _WindowSize)
{
	m_ElementSize = _WindowSize;
}
