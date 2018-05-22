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
	Widget::SetSize( _AvailableWindowRect.GetSize().GetResized(-m_Margin*2, -m_Margin*2) );
}




WindowSize BoxWidget::GetRequiredSize(const WindowSize& _AvailableSize)
{
	int32_t w = m_DefaultSize.GetWidth() + m_Margin * 2;
	int32_t h = m_DefaultSize.GetHeight() + m_Margin * 2;

	if (_AvailableSize.GetWidth() > 0)
	{
		w = _AvailableSize.GetWidth();
	}

	if (_AvailableSize.GetHeight() > 0)
	{
		h = _AvailableSize.GetHeight();
	}

	return WindowSize(w, h);
}




void BoxWidget::SetSize(const WindowSize & _AvailableSize)
{
	m_DefaultSize = _AvailableSize;
}
