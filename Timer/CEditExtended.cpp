#include "pch.h"
#include "CEditExtended.h"


void CEditExtended::SetTextColor(COLORREF ParameterCurrentColor)
{
	CurrentColor = ParameterCurrentColor;
}

void CEditExtended::OnPaint()
{
	CPaintDC dc(this);

	CString WindowText;
	
	GetWindowText(WindowText);

	dc.SetTextColor(CurrentColor);

	CFont CurrentFont;

	CurrentFont.CreateFontW(24, 18, 16, 20, FW_NORMAL, TRUE, TRUE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, nullptr);

	HGDIOBJ LastFont = dc.SelectObject(CurrentFont);

	dc.TextOutW(50, 16, WindowText);

	dc.SelectObject(LastFont);

	CurrentFont.DeleteObject();
}