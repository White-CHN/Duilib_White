#pragma once

#define  DUI_CTR_ANIMBUTTON                          (_T("AnimButton"))

class CDuiAnimButton
    : public CDuiButton
{
    DECLARE_DUICONTROL(CDuiAnimButton)
public:
    CDuiAnimButton(void);
    virtual ~CDuiAnimButton(void);
public:
    CDuiString GetClass() const OVERRIDE;
    LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

    void DoPaint(HDC hDC, const RECT& rcPaint) OVERRIDE;
private:
    vector<CDuiString> m_vtImages;
};
