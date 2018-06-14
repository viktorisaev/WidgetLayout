#pragma once

#include <DirectXMath.h>

#include <WindowRect.h>
#include <IWidget.h>



// abstract
class Widget : public IWidget
{
public:
	Widget()
	{ }

	Widget(const DirectX::XMFLOAT4& _Color) :
		m_WorldRect()
	,	m_Color(_Color)
	{ }

	~Widget()
	{ }

	const WindowRect& GetWorldRect() const;
	DirectX::XMFLOAT4 GetDebugColor() const;

	virtual void AddToRender(Sample3DSceneRenderer* _Render) override;


protected:
	void SetPosition(const WindowPos& _Position);
	void SetSize(WindowSize _Size);

	const WindowRect& GetRect() { return m_WorldRect; }



private:
	WindowRect m_WorldRect;
	DirectX::XMFLOAT4 m_Color;

};

