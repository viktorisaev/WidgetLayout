#include "pch.h"
#include "BoxWidget.h"


BoxWidget::BoxWidget()
{
}


BoxWidget::~BoxWidget()
{
}






void BoxWidget::AddToRender(Sample3DSceneRenderer * _Render)
{
	float _clipBottom = m_WindowDefaultSize.GetHeight() != FIT_PARENT ? float(m_WorldRect.GetSize().GetHeight()) / float(m_WindowDefaultSize.GetHeight()) : 1.0f;

	_Render->AddColoredRectToList(m_WorldRect, m_Color, m_TextureIndex, 0.0f, 0.0f, 1.0f, _clipBottom);
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
	WindowSize renderSize = m_LayoutData.GetContentSize(m_EnvelopSize);
	Widget::SetSize(renderSize);
}




WindowSize BoxWidget::GetEnvelopSize(const WindowSize& _ParentContentRect)
{
	m_NumberOfSizeCalculations += 1;
	return m_LayoutData.GetRequiredSizeWithParent(m_WindowDefaultSize, _ParentContentRect);
}




void BoxWidget::SetSize(const WindowSize & _Size)
{
	// m_LayoutData.SetSize(_Size);

	// set box size (texture? :)
	m_WindowDefaultSize = _Size;
}

