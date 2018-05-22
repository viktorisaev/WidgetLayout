#include "pch.h"
#include "WindowSize.h"


WindowSize::WindowSize()
{
}

WindowSize::WindowSize(int32_t _Width, int32_t _Height) :
	m_Size(_Width, _Height)
{
}

WindowSize::WindowSize(const WindowSize & _Size) :
	m_Size(_Size.GetWidth(), _Size.GetHeight())
{
}


WindowSize::~WindowSize()
{
}




bool WindowSize::Contains(const WindowSize & _Other) const
{
	return (m_Size.x > _Other.m_Size.x) && (m_Size.y > _Other.m_Size.y);
}

WindowSize WindowSize::GetMin(const WindowSize _First, const WindowSize _Second)
{
	WindowSize w(min(_First.m_Size.x, _Second.m_Size.x), min(_First.m_Size.y, _Second.m_Size.y));

	return w;
}
