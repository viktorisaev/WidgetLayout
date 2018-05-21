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

	void SetPosition(DirectX::XMINT2 _Position);
	void SetSize(DirectX::XMINT2 _Size);




private:
	DirectX::XMINT2 m_Position;
	DirectX::XMINT2 m_Size;

};

