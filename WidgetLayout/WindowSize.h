#pragma once

#include <DirectXMath.h>



class WindowSize
{
public:
	WindowSize();
	WindowSize(int32_t _Width, int32_t _Height);
	~WindowSize();

	bool Contains(const WindowSize& _Other) const;

	int32_t GetWidth() const { return m_Size.x; }
	int32_t GetHeight() const { return m_Size.y; }



private:
	DirectX::XMINT2 m_Size;

};

