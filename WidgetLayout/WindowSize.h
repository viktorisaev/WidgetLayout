#pragma once

#include <DirectXMath.h>



class WindowSize
{
public:
	WindowSize();
	WindowSize(int32_t _Width, int32_t _Height);
	WindowSize(const WindowSize& _Size);
	~WindowSize();

	bool Contains(const WindowSize& _Other) const;

	int32_t GetWidth() const { return m_Size.x; }
	int32_t GetHeight() const { return m_Size.y; }

	static WindowSize GetMin(const WindowSize _First, const WindowSize _Second);

private:
	DirectX::XMINT2 m_Size;

};

