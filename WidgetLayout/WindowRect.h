#pragma once

#include <DirectXMath.h>

#include <WindowSize.h>



class WindowRect
{
public:
	WindowRect();
	WindowRect(DirectX::XMINT2 m_Position, WindowSize m_Size);
	~WindowRect();


	DirectX::XMINT2 GetPosition() const;
	WindowSize GetSize() const;

	void SetPosition(DirectX::XMINT2 _Position);
	void SetSize(WindowSize _Size);

	bool Contains(const WindowRect& _Other) const;


private:
	DirectX::XMINT2 m_Position;
	WindowSize m_Size;

};

