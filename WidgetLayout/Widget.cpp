#include "pch.h"
#include "Widget.h"






void Widget::AddToRender(Sample3DSceneRenderer * _Render)
{
	_Render->AddColoredRectToList(m_WorldRect, m_Color, WIDGET_TEXTURE_FILL);
}



const WindowRect& Widget::GetWorldRect() const
{
	return m_WorldRect;
}


DirectX::XMFLOAT4 Widget::GetDebugColor() const
{
	return m_Color;
}



void Widget::SetPosition(const WindowPos& _Position)
{
	m_WorldRect.SetPosition(_Position);
}

void Widget::SetSize(WindowSize _Size)
{
	m_WorldRect.SetSize(_Size);
}
