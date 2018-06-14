#include "pch.h"
#include "StackWidget.h"
#include <WidgetFactory.h>




StackWidget::StackWidget(Direction _Direction, int32_t _Margin, const WindowSize& _WindowDefaultSize, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize) :
  Widget(_WindowDefaultSize, _Color)
, m_LayoutData(_Margin)
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



void StackWidget::UpdateLayout(const WindowSize& _ParentSize)
{
	// 1) calculate content size
	m_ContentSize = GetChildrenSize(m_ElementSize);

	m_Size = m_LayoutData.GetContentSize(m_LayoutData.GetRequiredSizeWithContentAndParent(m_WindowDefaultSize, m_ContentSize, _ParentSize));

	// 2) layout content
//	WindowSize singleChildSize = WindowSize::GetMin(, size);
	
	int32_t childPos = 0;	// would used for X or Y depends on m_Direction
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize childEnvelopRect = childWidget->GetEnvelopSize(m_ElementSize);

		WindowPos newPos;

		if (m_Direction == Vertical)
		{
			 newPos = WindowPos(0, childPos);

			childEnvelopRect.SetWidth(min(childEnvelopRect.GetWidth(), m_Size.GetWidth()));

			if (childEnvelopRect.GetHeight() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetHeight(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childPos += childEnvelopRect.GetHeight();
		}
		else
		{	// Horizontal
			newPos = WindowPos(childPos, 0);

			childEnvelopRect.SetHeight(min(childEnvelopRect.GetHeight(), m_Size.GetHeight()));

			if (childEnvelopRect.GetWidth() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetWidth(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childPos += childEnvelopRect.GetWidth();
		}

		WindowSize newRect = WindowSize( childEnvelopRect );
		childWidget->UpdateLayout( newRect );
	}	// for each child
}





void StackWidget::BuildWorldPosition(const WindowPos & _ParentPos)
{
	// content size is m_ContentSize

	// margin
	WindowPos rectPos = _ParentPos;
	rectPos.Offset( m_LayoutData.GetMargin(), m_LayoutData.GetMargin() );

	// set draw rect
	Widget::SetPosition(rectPos);

	Widget::SetSize(m_Size);

	// 2) layout content

	int32_t childDelta;	// would used for X or Y depends on m_Direction
	for (Widget* childWidget : m_Widgets)
	{
		WindowSize childEnvelopRect = childWidget->GetEnvelopSize(m_ElementSize);

		WindowPos newPos;

		if (m_Direction == Vertical)
		{
			childEnvelopRect.SetWidth(min(childEnvelopRect.GetWidth(), m_Size.GetWidth()));

			if (childEnvelopRect.GetHeight() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetHeight(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childDelta = childEnvelopRect.GetHeight();
		}
		else
		{	// Horizontal
			childEnvelopRect.SetHeight(min(childEnvelopRect.GetHeight(), m_Size.GetHeight()));

			if (childEnvelopRect.GetWidth() == ENVELOP_CHILD)
			{
				childEnvelopRect.SetWidth(0);	// can't draw child of 'fit parent' if element size is not defined
			}
			childDelta = childEnvelopRect.GetWidth();
		}

		childWidget->BuildWorldPosition(rectPos);

		// promote
		if (m_Direction == Vertical)
		{
			rectPos.Offset(0, childDelta);
		}
		else
		{	// Horizontal
			rectPos.Offset(childDelta, 0);
		}

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
	m_NumberOfSizeCalculations += 1;

	WindowSize contentSize = GetChildrenSize(m_ElementSize);
	WindowSize envelopSize = m_LayoutData.GetRequiredSizeWithContentAndParent(m_WindowDefaultSize, contentSize, _MaxContentRect);

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
