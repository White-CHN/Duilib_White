#include "StdAfx.h"
#include "SkinPictureBtn.h"


IMPLEMENT_DUICONTROL(CDuiSkinPictureBtn)

CDuiSkinPictureBtn::CDuiSkinPictureBtn(void)
{
}


CDuiSkinPictureBtn::~CDuiSkinPictureBtn(void)
{
}

CDuiString CDuiSkinPictureBtn::GetClass() const
{
    return DUI_CTR_PICTUREBTN;
}

LPVOID CDuiSkinPictureBtn::GetInterface(LPCTSTR pstrName)
{
    if(_tcscmp(pstrName, DUI_CTR_PICTUREBTN) == 0)
    {
        return static_cast<CDuiSkinPictureBtn*>(this);
    }
    return CDuiButton::GetInterface(pstrName);
}

void CDuiSkinPictureBtn::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if(_tcscmp(pstrName, _T("bkname")) == 0)
    {
        m_BkName = pstrValue;
    }
    else if(_tcscmp(pstrName, _T("author")) == 0)
    {
        m_Author = pstrValue;
    }
    else
    {
        CDuiButton::SetAttribute(pstrName, pstrValue);
    }

}

void CDuiSkinPictureBtn::PaintStatusImage(HDC hDC)
{
    CDuiButton::PaintStatusImage(hDC);
    UINT uButtonState = GetButtonState();
    if(IsFocused())
    {
        uButtonState |= UISTATE_FOCUSED;
    }
    else
    {
        uButtonState &= ~ UISTATE_FOCUSED;
    }
    if(!IsEnabled())
    {
        uButtonState |= UISTATE_DISABLED;
    }
    else
    {
        uButtonState &= ~ UISTATE_DISABLED;
    }
    SetButtonStste(uButtonState);
    if((uButtonState & UISTATE_PUSHED) != 0 || (uButtonState & UISTATE_HOT) != 0)
    {

        DrawImage(hDC, GetFocusedImage()) ;

        //计算作者信息文字和背景图片名字文字的显示位置，这里是用了硬编码，请使用者自己修改
        RECT rcBkName = GetPos();
        LONG nTextPadding = (GetPos().right - GetPos().left  - CRenderEngine::GetTextSize(hDC, GetManager(), \
                             m_BkName.GetData(), GetFont(), GetTextStyle()).cx) / 2;
        rcBkName.left += nTextPadding;
        rcBkName.right -= nTextPadding;
        rcBkName.top += 15;
        rcBkName.bottom = rcBkName.top + 20;

        RECT rcAuthor = GetPos();
        nTextPadding = (GetPos().right - GetPos().left - CRenderEngine::GetTextSize(hDC, GetManager(), \
                        m_Author.GetData(), GetFont(), GetTextStyle()).cx) / 2;
        rcAuthor.left += nTextPadding;
        rcAuthor.right -= nTextPadding;
        rcAuthor.top += 40;
        rcAuthor.bottom = rcAuthor.top + 20;

        CRenderEngine::DrawText(hDC, GetManager(), rcBkName, m_BkName, 0xFFFFFFFF, GetFont(), GetTextStyle());
        CRenderEngine::DrawText(hDC, GetManager(), rcAuthor, m_Author, 0xFFAAAAAA, GetFont(), GetTextStyle());
        CRenderEngine::DrawRect(hDC, GetPos(), 2, 0xFF64B0FA);

    }
}
