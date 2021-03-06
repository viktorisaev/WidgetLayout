#pragma once

#include <vector>
#include <Widget.h>
#include <LayoutData.h>


class StackWidget :	public Widget
{
public:

	enum Direction
	{
		Vertical,
		Horizontal
	};


	StackWidget(Direction _Direction, int32_t _Margin, const WindowSize& _WindowDefaultSize, DirectX::XMFLOAT4 _Color, WindowSize _ElementSize);
	virtual ~StackWidget();

// control
	void AddWidget(Widget* _Widget);
	void SetElementSize(const WindowSize& _WindowSize);

// IWidget
	void AddToRender(Sample3DSceneRenderer* _Render) override;
	void UpdateLayout(const WindowSize& _ParentSize) override;
	WindowSize GetEnvelopSize(const WindowSize& _ParentContentRect) override;
	virtual void BuildWorldPosition(const WindowPos & _ParentPos) override;


protected:
	WindowSize GetChildrenSize(const WindowSize& _ElementRect);


private:
	std::vector<Widget*> m_Widgets;

protected:
	LayoutData m_LayoutData;	// rect + margin
	WindowSize m_ElementSize;	// size of one element of the stack or ENVELOP_CHILD
	Direction m_Direction; // horizontal or vertical
	WindowSize m_ContentSize;	// calculated in UpdateLayout to pass to BuildWorldPosition
	WindowSize m_Size;	// size of the window after UpdateLayout
};

