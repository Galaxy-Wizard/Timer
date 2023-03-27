#pragma once

#include <afxwin.h>

const int Little = 0;
const int Big = 1;

class CStaticExtended :
    public CStatic
{
protected:
    COLORREF CurrentColor;
    CString TextString;
    int Size;
public:
    virtual void SetTextColor(COLORREF ParameterCurrentColor);

    virtual void OnPaint();
    virtual void SetWindowTextW(LPCTSTR lpszString, int size);
};

