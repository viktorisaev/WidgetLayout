#include "pch.h"
#include "WindowRect.h"


WindowRect::WindowRect()
{
}


WindowRect::~WindowRect()
{
}



WindowRect::WindowRect(DirectX::XMINT2 _Position, WindowSize _Size) :
	m_Position(_Position)
,	m_Size(_Size)
{

}



DirectX::XMINT2 WindowRect::GetPosition() const
{
	return m_Position;
}



WindowSize WindowRect::GetSize() const
{
	return m_Size;
}

void WindowRect::SetPosition(DirectX::XMINT2 _Position)
{
	m_Position = _Position;
}

void WindowRect::SetSize(WindowSize _Size)
{
	m_Size = _Size;
}




bool WindowRect::Contains(const WindowRect & _Other) const
{
	return this->m_Size.Contains(_Other.m_Size);
}
