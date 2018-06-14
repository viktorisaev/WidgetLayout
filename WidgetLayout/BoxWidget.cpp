#include "pch.h"
#include "BoxWidget.h"


BoxWidget::BoxWidget()
{
}


BoxWidget::~BoxWidget()
{
}






void BoxWidget::UpdateLayout(const WindowSize& _ParentSize)
{
	m_EnvelopSize = m_LayoutData.GetRequiredSizeWithParent(m_WindowDefaultSize, _ParentSize);	// store size for render
}




void BoxWidget::BuildWorldPosition(const WindowPos & _ParentPos)
{
	WindowPos newPos = WindowPos(_ParentPos.GetX() + m_LayoutData.GetMargin(), _ParentPos.GetY() + m_LayoutData.GetMargin());

	// set draw rect
	Widget::SetPosition(newPos);
	Widget::SetSize(m_LayoutData.GetContentSize(m_EnvelopSize));
}




WindowSize BoxWidget::GetEnvelopSize(const WindowSize& _ParentContentRect)
{
	return m_LayoutData.GetRequiredSizeWithParent(m_WindowDefaultSize, _ParentContentRect);
}




void BoxWidget::SetSize(const WindowSize & _Size)
{
	// m_LayoutData.SetSize(_Size);

	// set box size (texture? :)
	m_WindowDefaultSize = _Size;
}

