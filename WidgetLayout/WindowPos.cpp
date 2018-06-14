#include "pch.h"
#include "WindowPos.h"


WindowPos::WindowPos()
{
}

WindowPos::WindowPos(int32_t _X, int32_t _Y) :
	m_Position(_X, _Y)
{
}

WindowPos::WindowPos(const WindowPos & _Pos) :
	m_Position(_Pos.GetX(), _Pos.GetY())
{
}


WindowPos::~WindowPos()
{
}




const WindowPos& WindowPos::Offset(int32_t _DeltaX, int32_t _DeltaY)
{
	m_Position.x += _DeltaX;
	m_Position.y += _DeltaY;

	return *this;
}
