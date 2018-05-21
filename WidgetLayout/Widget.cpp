#include "pch.h"
#include "Widget.h"





const WindowRect& Widget::GetRect() const
{
	return m_Rect;
}





DirectX::XMFLOAT4 Widget::GetColor() const
{
	return m_Color;
}

void Widget::SetSize(DirectX::XMINT2 _Size)
{
	m_Rect.SetSize(_Size);
}
