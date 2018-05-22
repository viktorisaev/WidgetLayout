#pragma once

#include <DirectXMath.h>

#include <WindowRect.h>
#include <IWidget.h>


// abstract
class Widget : public IWidget
{
public:
	Widget()
	{ }

	Widget(DirectX::XMINT2 _Position, WindowSize _Size, DirectX::XMFLOAT4 _Color) :
		m_AbsRect(_Position, _Size)
	,	m_Color(_Color)
	{ }

	~Widget()
	{ }

	const WindowRect& GetAbsRect() const;
	DirectX::XMFLOAT4 GetColor() const;


protected:
	void SetPosition(DirectX::XMINT2 _Position);
	void SetSize(WindowSize _Size);




protected:
	WindowRect m_AbsRect;
	DirectX::XMFLOAT4 m_Color;

};

