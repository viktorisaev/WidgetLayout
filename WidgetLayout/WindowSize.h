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
	void SetWidth(const int32_t _Width) { m_Size.x = _Width; }
	void SetHeight(const int32_t _Height) { m_Size.y = _Height; }

	static WindowSize GetMin(const WindowSize _First, const WindowSize _Second);

	WindowSize GetResized(int32_t _Hor, int32_t _Vert);
	const WindowSize& Resize(int32_t _Hor, int32_t _Vert);

private:
	DirectX::XMINT2 m_Size;

};

