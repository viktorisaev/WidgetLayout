#include "pch.h"
#include "Widget.h"





const WindowRect& Widget::GetAbsRect() const
{
	return m_AbsRect;
}





DirectX::XMFLOAT4 Widget::GetColor() const
{
	return m_Color;
}

void Widget::SetPosition(DirectX::XMINT2 _Position)
{
	m_AbsRect.SetPosition(_Position);
}

void Widget::SetSize(WindowSize _Size)
{
	m_AbsRect.SetSize(_Size);
}
