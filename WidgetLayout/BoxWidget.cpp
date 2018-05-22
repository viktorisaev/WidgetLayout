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

void BoxWidget::UpdateLayout(const WindowRect & _AvailableWindowRect)
{
	DirectX::XMINT2 rectPos = _AvailableWindowRect.GetPosition();
	DirectX::XMINT2 newPos = DirectX::XMINT2(rectPos.x + m_Margin, rectPos.y + m_Margin);

	Widget::SetPosition(newPos);
	Widget::SetSize(_AvailableWindowRect.GetSize());
}

WindowSize BoxWidget::GetRequiredSize(const WindowSize& _AvailableSize)
{
	WindowSize w = WindowSize::GetMin(m_DefaultSize, _AvailableSize);

	return w;
}

void BoxWidget::SetSize(const WindowSize & _AvailableSize)
{
	m_DefaultSize = _AvailableSize;
}
