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
	_Render->AddColoredRectToList(this->GetAbsRect(), this->GetColor());
}




void BoxWidget::UpdateLayout(const WindowRect& _VisibleRect)
{
	WindowSize size = m_LayoutData.GetRequiredSizeWithParent(_VisibleRect.GetSize());

	DirectX::XMINT2 rectPos = _VisibleRect.GetPosition();
	DirectX::XMINT2 newPos = DirectX::XMINT2(rectPos.x + m_LayoutData.GetMargin(), rectPos.y + m_LayoutData.GetMargin());

	// set draw rect
	Widget::SetPosition(newPos);
	Widget::SetSize( m_LayoutData.GetContentSize(size) );
}




WindowSize BoxWidget::GetEnvelopSize(const WindowSize& _MaxContentRect)
{
	return m_LayoutData.GetRequiredSizeWithParent(_MaxContentRect);
}




void BoxWidget::SetSize(const WindowSize & _Size)
{
	m_LayoutData.SetSize(_Size);
}
