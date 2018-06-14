#pragma once

#include <DirectXMath.h>

#include <WindowPos.h>
#include <WindowSize.h>



class WindowRect
{
public:
	WindowRect();
	WindowRect(const WindowPos& _Position, const WindowSize& _Size);
	~WindowRect();


	const WindowPos& GetPosition() const;
	const WindowSize& GetSize() const;

	void SetPosition(const WindowPos& _Position);
	void SetSize(WindowSize _Size);

	bool Contains(const WindowRect& _Other) const;


private:
	WindowPos m_Position;
	WindowSize m_Size;

};

