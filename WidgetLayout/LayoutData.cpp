#include "pch.h"
#include "LayoutData.h"


LayoutData::LayoutData()
{
}

LayoutData::LayoutData(int32_t _Margin, WindowSize _Size) :
  m_Margin(_Margin)
, m_Size(_Size)
{
}


LayoutData::~LayoutData()
{
}



WindowSize LayoutData::GetMinimumSize()
{
	return WindowSize(m_Size.GetWidth() + GetDoubleMargin(), m_Size.GetHeight() + GetDoubleMargin());
}




void LayoutData::SetSize(const WindowSize & _Size)
{
	m_Size = _Size;
}



WindowSize LayoutData::GetContentSize(const WindowSize & _EnvelopSize)
{
	return WindowSize(
		max(0, _EnvelopSize.GetWidth() - GetDoubleMargin()),
		max(0, _EnvelopSize.GetHeight() - GetDoubleMargin())
	);
}



WindowSize LayoutData::GetRequiredSizeWithParent(const WindowSize & _MaxContentRect)
{
	// width
	int32_t w;
	if (m_Size.GetWidth() == FIT_PARENT)
	{
		w = _MaxContentRect.GetWidth();
	}
	else
	{
		if (m_Size.GetWidth() == ENVELOP_CHILD)
		{
			int i = 1;	// error! can't envelop chile if no child size
		}
		w = GetMeasureWithEnvelop(m_Size.GetWidth() + GetDoubleMargin(), _MaxContentRect.GetWidth());
	}

	// height
	int32_t h;
	if (m_Size.GetHeight() == FIT_PARENT)
	{
		h = _MaxContentRect.GetHeight();
	}
	else
	{
		if (m_Size.GetHeight() == ENVELOP_CHILD)
		{
			int i = 1;	// error! can't envelop chile if no child size
		}
		h = GetMeasureWithEnvelop(m_Size.GetHeight() + GetDoubleMargin(), _MaxContentRect.GetHeight());
	}

	return WindowSize( w , h );
}



WindowSize LayoutData::GetRequiredSizeWithChildrenAndParent(const WindowSize& _ChildrenRect, const WindowSize& _ParentContentRect)
{
	// width
	int32_t w;
	const int32_t myWidth = m_Size.GetWidth();
	if (myWidth == FIT_PARENT)
	{
		w = _ParentContentRect.GetWidth() - GetDoubleMargin();
	}
	else
	{
		if (myWidth == ENVELOP_CHILD)
		{
			w = _ChildrenRect.GetWidth();
		}
		else
		{
			w = GetMeasureWithEnvelop(myWidth + GetDoubleMargin(), _ParentContentRect.GetWidth());
		}
	}

	// height
	int32_t h;
	const int32_t myHeight = m_Size.GetHeight();
	if (myHeight == FIT_PARENT)
	{
		h = _ParentContentRect.GetHeight() - GetDoubleMargin();
	}
	else
	{
		if (myHeight == ENVELOP_CHILD)
		{
			h = _ChildrenRect.GetHeight();
		}
		else
		{
			h = GetMeasureWithEnvelop(myHeight + GetDoubleMargin(), _ParentContentRect.GetHeight());
		}
	}

	return WindowSize(w, h);
}



WindowSize LayoutData::GetEnvelopSize(const WindowSize & _ContentSize)
{
	return WindowSize(_ContentSize.GetWidth() + GetDoubleMargin(), _ContentSize.GetHeight() + GetDoubleMargin());
}




int32_t LayoutData::GetMargin()
{
	return m_Margin;
}

WindowSize LayoutData::GetInternalSize()
{
	return m_Size;
}




// calculate measure: envelop=ENVELOP_CHILD -> default, envelop!=ENVELOP_CHILD -> envelop
int32_t LayoutData::GetMeasureWithEnvelop(int32_t _DefaultMeasure, int32_t _EnvelopMeasure)
{
	int32_t measure = min( _DefaultMeasure , _EnvelopMeasure );
	return measure;
}
