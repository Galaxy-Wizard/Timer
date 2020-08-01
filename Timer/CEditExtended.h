#pragma once

#include <afxwin.h>

class CEditExtended :
    public CEdit
{
protected:
    COLORREF CurrentColor;
public:
    void SetTextColor(COLORREF ParameterCurrentColor);

    virtual void OnPaint();
};

