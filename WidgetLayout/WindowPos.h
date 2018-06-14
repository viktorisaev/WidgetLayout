

#pragma once

#include <DirectXMath.h>


class WindowPos
{
public:
	WindowPos();
	~WindowPos();

	WindowPos(int32_t _X, int32_t _Y);
	WindowPos(const WindowPos& _Size);

	int32_t GetX() const { return m_Position.x; }
	int32_t GetY() const { return m_Position.y; }
	void SetX(const int32_t _Width) { m_Position.x = _Width; }
	void SetY(const int32_t _Height) { m_Position.y = _Height; }

	const WindowPos& Offset(int32_t _DeltaX, int32_t _DeltaY);

private:
	DirectX::XMINT2 m_Position;

};

