#include "pch.h"
#include "StackWidget.h"
#include <WidgetFactory.h>




StackWidget::StackWidget(Direction _Direction, int32_t _Margin, WindowSize _Size, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize) :
  Widget(DirectX::XMINT2(0,0), _Size, _Color)
, m_LayoutData(_Margin, _Size)
, m_ElementSize(_ElementSize)
, m_Direction(_Direction)
{
}



StackWidget::~StackWidget()
{
	for (Widget* w : m_Widgets)
	{
		WidgetFactory::DeleteWidget(w);
	}
}



void StackWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	Widget::AddToRender(_Render);

	for (Widget* w : m_Widgets)
	{
		w->AddToRender(_Render);
	}
}



void StackWidget::UpdateLayout(const WindowRect& _VisibleRect)
{
	// 1) calculate content size
	WindowSize contentSize = GetChildrenSize(m_ElementSize);

	// margin
	DirectX::XMINT2 rectPos = _VisibleRect.GetPosition();
	rectPos.x += m_LayoutData.GetMargin();
	rectPos.y += m_LayoutData.GetMargin();

	// set draw rect
	Widget::SetPosition(rectPos);

	WindowSize size = m_LayoutData.GetContentSize(m_LayoutData.GetRequiredSizeWithContentAndParent(contentSize, _VisibleRect.GetSize()));

	Widget::SetSize(size);

	// 2) layout content
//	WindowSize singleChildSize = WindowSize::GetMin(, size);
	
	int32_t childPos = 0;	// would used for X or Y depends on m_Direction
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize childEnvelopRect = childWidget->GetEnvelopSize(m_ElementSize);

		DirectX::XMINT2 newPos;

		if (m_Direction == Vertical)
		{
			 newPos = DirectX::XMINT2(rectPos.x, rectPos.y + childPos);

			childEnvelopRect.SetWidth(min(childEnvelopRect.GetWidth(), size.GetWidth()));

			if (childEnvelopRect.GetHeight() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetHeight(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childPos += childEnvelopRect.GetHeight();
		}
		else
		{	// Horizontal
			newPos = DirectX::XMINT2(rectPos.x + childPos, rectPos.y);

			childEnvelopRect.SetHeight(min(childEnvelopRect.GetHeight(), size.GetHeight()));

			if (childEnvelopRect.GetWidth() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetWidth(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childPos += childEnvelopRect.GetWidth();
		}

		WindowRect newRect = WindowRect( newPos , childEnvelopRect );
		childWidget->UpdateLayout( newRect );
	}
}




WindowSize StackWidget::GetChildrenSize(const WindowSize& _ElementRect)
{
	// calculate children container size
	int32_t w = 0;
	int32_t h = 0;
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize rect = childWidget->GetEnvelopSize(_ElementRect);

		if (m_Direction == Vertical)
		{
			if (rect.GetWidth() != ENVELOP_CHILD)
			{
				w = max(w, rect.GetWidth());
			}

			if (rect.GetHeight() != ENVELOP_CHILD)
			{
				h += rect.GetHeight();
			}
		}
		else
		{	// Horizontal
			if (rect.GetHeight() != ENVELOP_CHILD)
			{
				h = max(h, rect.GetHeight());
			}

			if (rect.GetWidth() != ENVELOP_CHILD)
			{
				w += rect.GetWidth();
			}
		}

	}

	return WindowSize(w, h);
}



WindowSize StackWidget::GetEnvelopSize(const WindowSize & _MaxContentRect)
{
	WindowSize contentSize = GetChildrenSize(m_ElementSize);
	WindowSize envelopSize = m_LayoutData.GetEnvelopSize(contentSize);

	return envelopSize;
}


void StackWidget::AddWidget(Widget * _Widget)
{
	m_Widgets.push_back(_Widget);
}



void StackWidget::SetElementSize(const WindowSize& _WindowSize)
{
	m_ElementSize = _WindowSize;
}
