#pragma once

#include <DirectXMath.h>

#include <WindowRect.h>
#include <IWidget.h>



extern int m_NumberOfSizeCalculations;


// TODO: not a good place
enum WIDGET_TEXTURES
{
	WIDGET_TEXTURE_FILL = 0,
	WIDGET_TEXTURE_FRAME,
	WIDGET_TEXTURE_EXACT,
	WIDGET_TEXTURE_BUTTON,
};



// abstract
class Widget : public IWidget
{
public:
	Widget()
	{ }

	Widget(const WindowSize& _WindowDefaultSize, const DirectX::XMFLOAT4& _Color) :
	  m_WorldRect()
	, m_Color(_Color)
	, m_WindowDefaultSize(_WindowDefaultSize)
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


protected:
	WindowSize m_WindowDefaultSize;	// setup size of the widget

	WindowRect m_WorldRect;
	DirectX::XMFLOAT4 m_Color;
};

