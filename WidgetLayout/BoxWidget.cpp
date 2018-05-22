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

	m_AbsRect.SetPosition(newPos);
}
