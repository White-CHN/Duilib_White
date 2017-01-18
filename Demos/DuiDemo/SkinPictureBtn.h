#pragma once

#define DUI_CTR_PICTUREBTN		(_T("SkinPictureBtn"))


class CDuiSkinPictureBtn :
    public CDuiButton
{
    DECLARE_DUICONTROL(CDuiSkinPictureBtn)
public:
    CDuiSkinPictureBtn(void);
    virtual ~CDuiSkinPictureBtn(void);
public:
    CDuiString GetClass() const OVERRIDE;
    LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
    void PaintStatusImage(HDC hDC) OVERRIDE;

private:
    CDuiString m_BkName;
    CDuiString m_Author;
};

