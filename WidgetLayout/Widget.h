#pragma once

#include <DirectXMath.h>

#include <WindowRect.h>


class Widget
{
public:
	Widget()
	{ }

	Widget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT4 _Color) :
		m_Rect(_Position, _Size)
	,	m_Color(_Color)
	{ }

	~Widget()
	{ }

	const WindowRect& GetRect() const;
	DirectX::XMFLOAT4 GetColor() const;


private:
	WindowRect m_Rect;
	DirectX::XMFLOAT4 m_Color;

};

