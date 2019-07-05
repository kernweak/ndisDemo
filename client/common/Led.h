#pragma once
// The constants needed to define the colours of the LED
#define ID_LED_RED		2001
#define ID_LED_GREEN	2002
#define ID_LED_BLUE		2003
#define ID_LED_YELLOW	2004

// The constants needed to define the shape of the LED
#define ID_SHAPE_ROUND	3001
#define ID_SHAPE_SQUARE	3002

// CLed

class CLed : public CStatic
{
	DECLARE_DYNAMIC(CLed)

public:
	CLed();
	virtual ~CLed();
	void SetLed(UINT nIDColor, UINT nIDShape,BOOL bBright = TRUE);

private:
	// The pens and brushes needed to do the drawing
	CPen m_PenBright,m_PenDark;
	CBrush m_BrushBright,m_BrushDark,m_BrushCurrent;

	// This variable is used to store the shape and color
	UINT m_nID;
	UINT m_nShape;
	BOOL m_bBright;

	void Draw();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};
