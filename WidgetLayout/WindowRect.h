#pragma once

#include <DirectXMath.h>



class WindowRect
{
public:
	WindowRect();
	WindowRect(DirectX::XMINT2 m_Position, DirectX::XMINT2 m_Size);
	~WindowRect();


	DirectX::XMINT2 GetPosition() const;
	DirectX::XMINT2 GetSize() const;




private:
	DirectX::XMINT2 m_Position;
	DirectX::XMINT2 m_Size;

};

