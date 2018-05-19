#pragma once

#include <DirectXMath.h>


class Widget
{
public:
	Widget()
	{ }

	Widget(DirectX::XMINT2 _Position, DirectX::XMINT2 _Size, DirectX::XMFLOAT3 _Color) :
		m_Position(_Position)
	,	m_Size(_Size)
	,	m_Color(_Color)
	{ }

	~Widget()
	{ }

	DirectX::XMINT2 GetPosition() const;
	DirectX::XMINT2 GetSize() const;
	DirectX::XMFLOAT3 GetColor() const;


private:
	DirectX::XMINT2 m_Position;
	DirectX::XMINT2 m_Size;
	DirectX::XMFLOAT3 m_Color;

};

