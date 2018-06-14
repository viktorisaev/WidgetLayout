#include "pch.h"
#include "LayoutData.h"


LayoutData::LayoutData()
{
}

LayoutData::LayoutData(int32_t _Margin/*, WindowSize _Size*/) :
  m_Margin(_Margin)
//, m_Size(_Size)
{
}


LayoutData::~LayoutData()
{
}



//WindowSize LayoutData::GetMinimumSize()
//{
//	return WindowSize(m_Size.GetWidth() + GetDoubleMargin(), m_Size.GetHeight() + GetDoubleMargin());
//}




//void LayoutData::SetSize(const WindowSize & _Size)
//{
//	m_Size = _Size;
//}



WindowSize LayoutData::GetContentSize(const WindowSize & _EnvelopSize)
{
	return WindowSize(
		max(0, _EnvelopSize.GetWidth() - GetDoubleMargin()),
		max(0, _EnvelopSize.GetHeight() - GetDoubleMargin())
	);
}



WindowSize LayoutData::GetRequiredSizeWithParent(const WindowSize& _windowSize, const WindowSize & _ParentContentRect)
{
	// width
	int32_t w;
	if (_windowSize.GetWidth() == FIT_PARENT)
	{
		w = _ParentContentRect.GetWidth();
	}
	else
	{
		if (_windowSize.GetWidth() == ENVELOP_CHILD)
		{
			int i = 1;	// error! can't envelop child if no child size
		}
		w = GetMeasureWithEnvelop(_windowSize.GetWidth() + GetDoubleMargin(), _ParentContentRect.GetWidth());
	}

	// height
	int32_t h;
	if (_windowSize.GetHeight() == FIT_PARENT)
	{
		h = _ParentContentRect.GetHeight();
	}
	else
	{
		if (_windowSize.GetHeight() == ENVELOP_CHILD)
		{
			int i = 1;	// error! can't envelop child if no child size
		}
		h = GetMeasureWithEnvelop(_windowSize.GetHeight() + GetDoubleMargin(), _ParentContentRect.GetHeight());
	}

	return WindowSize( w , h );
}



WindowSize LayoutData::GetRequiredSizeWithContentAndParent(const WindowSize& _windowSize, const WindowSize& _ChildrenRect, const WindowSize& _ParentContentRect)
{
	// width
	int32_t w;
	const int32_t myWidth = _windowSize.GetWidth();
	if (myWidth == FIT_PARENT)
	{
		w = _ParentContentRect.GetWidth();
	}
	else
	{
		if (myWidth == ENVELOP_CHILD)
		{
			w = _ChildrenRect.GetWidth() + GetDoubleMargin();
		}
		else
		{
			w = GetMeasureWithEnvelop(myWidth + GetDoubleMargin(), _ParentContentRect.GetWidth());
		}
	}

	// height
	int32_t h;
	const int32_t myHeight = _windowSize.GetHeight();
	if (myHeight == FIT_PARENT)
	{
		h = _ParentContentRect.GetHeight();
	}
	else
	{
		if (myHeight == ENVELOP_CHILD)
		{
			h = _ChildrenRect.GetHeight() + GetDoubleMargin();
		}
		else
		{
			h = GetMeasureWithEnvelop(myHeight + GetDoubleMargin(), _ParentContentRect.GetHeight());
		}
	}

	return WindowSize(w, h);
}



WindowSize LayoutData::GetRequiredSizeWithContent(const WindowSize& _windowSize, const WindowSize& _ContentRect)
{
	// width
	int32_t w;
	const int32_t myWidth = _windowSize.GetWidth();
	if (myWidth == FIT_PARENT)	// not possible to define 'fit parent' if only content size provided
	{
		int i = 1;
	}
	else
	{
		if (myWidth == ENVELOP_CHILD)
		{
			w = _ContentRect.GetWidth() + GetDoubleMargin();
		}
		else
		{
			w = myWidth + GetDoubleMargin();
		}
	}

	// height
	int32_t h;
	const int32_t myHeight = _windowSize.GetHeight();
	if (myHeight == FIT_PARENT)	// not possible to define 'fit parent' if only content size provided
	{
		int i = 1;
	}
	else
	{
		if (myHeight == ENVELOP_CHILD)
		{
			h = _ContentRect.GetHeight() + GetDoubleMargin();
		}
		else
		{
			h = myHeight + GetDoubleMargin();
		}
	}

	return WindowSize(w, h);
}




WindowSize LayoutData::GetEnvelopSize(const WindowSize& _windowSize, const WindowSize & _ContentSize)
{
	return GetRequiredSizeWithContent(_windowSize, _ContentSize);
}




int32_t LayoutData::GetMargin()
{
	return m_Margin;
}

//WindowSize LayoutData::GetInternalSize()
//{
//	return m_Size;
//}




// calculate measure: envelop=ENVELOP_CHILD -> default, envelop!=ENVELOP_CHILD -> envelop
int32_t LayoutData::GetMeasureWithEnvelop(int32_t _DefaultMeasure, int32_t _EnvelopMeasure)
{
	int32_t measure = min( _DefaultMeasure , _EnvelopMeasure );
	return measure;
}
