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
