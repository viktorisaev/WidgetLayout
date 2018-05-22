#include "pch.h"
#include "WindowSize.h"


WindowSize::WindowSize()
{
}

WindowSize::WindowSize(int32_t _Width, int32_t _Height) :
	m_Size(_Width, _Height)
{
}


WindowSize::~WindowSize()
{
}




bool WindowSize::Contains(const WindowSize & _Other) const
{
	return (m_Size.x > _Other.m_Size.x) && (m_Size.y > _Other.m_Size.y);
}
