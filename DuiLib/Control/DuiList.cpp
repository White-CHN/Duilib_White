#include "StdAfx.h"
#include "DuiList.h"

namespace DuiLib
{
    ////////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiListHeaderItem)

    CDuiListHeaderItem::CDuiListHeaderItem()
        : m_bShowHtml(FALSE)
        , m_bDragable(TRUE)
        , m_iFont(-1)
        , m_nScale(0)
        , m_iSepWidth(4)
        , m_dwTextColor(0)
        , m_uButtonState(0)
        , m_uTextStyle(DT_VCENTER | DT_CENTER | DT_SINGLELINE)
    {
        SetMinWidth(16);
        SetTextPadding(CDuiRect(2, 0, 2, 0));
    }

    CDuiListHeaderItem::~CDuiListHeaderItem()
    {

    }

    CDuiString CDuiListHeaderItem::GetClass() const
    {
        return DUI_CTR_LISTHEADERITEM;
    }

    LPVOID CDuiListHeaderItem::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LISTHEADERITEM) == 0)
        {
            return this;
        }
        return CDuiContainer::GetInterface(pstrName);
    }

    void CDuiListHeaderItem::SetEnabled(BOOL bEnable /*= TRUE*/)
    {
        CDuiContainer::SetEnabled(bEnable);
        if(!IsEnabled())
        {
            m_uButtonState = 0;
        }
    }

    BOOL CDuiListHeaderItem::IsDragable() const
    {
        return m_bDragable;
    }

    void CDuiListHeaderItem::SetDragable(BOOL bDragable)
    {
        m_bDragable = bDragable;
        if(!m_bDragable)
        {
            m_uButtonState &= ~UISTATE_CAPTURED;
        }
    }

    DWORD CDuiListHeaderItem::GetSepWidth() const
    {
        return m_iSepWidth;
    }

    void CDuiListHeaderItem::SetSepWidth(int iWidth)
    {
        m_iSepWidth = iWidth;
    }

    DWORD CDuiListHeaderItem::GetTextStyle() const
    {
        return m_uTextStyle;
    }

    void CDuiListHeaderItem::SetTextStyle(UINT uStyle)
    {
        if(m_uTextStyle == uStyle)
        {
            return;
        }
        m_uTextStyle = uStyle;
        Invalidate();
    }

    DWORD CDuiListHeaderItem::GetTextColor() const
    {
        return m_dwTextColor;
    }


    void CDuiListHeaderItem::SetTextColor(DWORD dwTextColor)
    {
        m_dwTextColor = dwTextColor;
    }

    RECT CDuiListHeaderItem::GetTextPadding() const
    {
        return m_rcTextPadding;
    }

    void CDuiListHeaderItem::SetTextPadding(RECT rc)
    {
        if(m_rcTextPadding == rc)
        {
            return;
        }
        m_rcTextPadding = rc;
        Invalidate();
    }

    int CDuiListHeaderItem::GetFont() const
    {
        return m_iFont;
    }

    void CDuiListHeaderItem::SetFont(int index)
    {
        m_iFont = index;
    }

    BOOL CDuiListHeaderItem::IsShowHtml()
    {
        return m_bShowHtml;
    }

    void CDuiListHeaderItem::SetShowHtml(BOOL bShowHtml)
    {
        if(m_bShowHtml == bShowHtml)
        {
            return;
        }

        m_bShowHtml = bShowHtml;
        Invalidate();
    }

    LPCTSTR CDuiListHeaderItem::GetNormalImage() const
    {
        return m_sNormalImage;
    }

    void CDuiListHeaderItem::SetNormalImage(LPCTSTR pStrImage)
    {
        m_sNormalImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiListHeaderItem::GetHotImage() const
    {
        return m_sHotImage;
    }

    void CDuiListHeaderItem::SetHotImage(LPCTSTR pStrImage)
    {
        m_sHotImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiListHeaderItem::GetPushedImage() const
    {
        return m_sPushedImage;
    }

    void CDuiListHeaderItem::SetPushedImage(LPCTSTR pStrImage)
    {
        m_sPushedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiListHeaderItem::GetFocusedImage() const
    {
        return m_sFocusedImage;
    }

    void CDuiListHeaderItem::SetFocusedImage(LPCTSTR pStrImage)
    {
        m_sFocusedImage = pStrImage;
        Invalidate();
    }

    LPCTSTR CDuiListHeaderItem::GetSepImage() const
    {
        return m_sSepImage;
    }

    void CDuiListHeaderItem::SetSepImage(LPCTSTR pStrImage)
    {
        m_sSepImage = pStrImage;
        Invalidate();
    }

    int CDuiListHeaderItem::GetScale() const
    {
        return m_nScale;
    }

    void CDuiListHeaderItem::SetScale(int nScale)
    {
        m_nScale = nScale;
    }

    void CDuiListHeaderItem::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("dragable")) == 0)
        {
            SetDragable(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("sepwidth")) == 0)
        {
            SetSepWidth(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("align")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_uTextStyle &= ~(DT_CENTER | DT_RIGHT);
                m_uTextStyle |= DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_RIGHT);
                m_uTextStyle |= DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_uTextStyle &= ~(DT_LEFT | DT_CENTER);
                m_uTextStyle |= DT_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("endellipsis")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_uTextStyle |= DT_END_ELLIPSIS;
            }
            else
            {
                m_uTextStyle &= ~DT_END_ELLIPSIS;
            }
        }
        else if(_tcsicmp(pstrName, _T("font")) == 0)
        {
            SetFont(_ttoi(pstrValue));
        }
        else if(_tcsicmp(pstrName, _T("textcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("textpadding")) == 0)
        {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetTextPadding(rcTextPadding);
        }
        else if(_tcsicmp(pstrName, _T("showhtml")) == 0)
        {
            SetShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("normalimage")) == 0)
        {
            SetNormalImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("hotimage")) == 0)
        {
            SetHotImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("pushedimage")) == 0)
        {
            SetPushedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("focusedimage")) == 0)
        {
            SetFocusedImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("sepimage")) == 0)
        {
            SetSepImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("scale")) == 0)
        {
            LPTSTR pstr = NULL;
            SetScale(_tcstol(pstrValue, &pstr, 10));
        }
        else
        {
            CDuiContainer::SetAttribute(pstrName, pstrValue);
        }
    }

    SIZE CDuiListHeaderItem::EstimateSize(SIZE szAvailable)
    {
        if(GetFixedHeight() == 0)
        {
            return CDuiSize(GetFixedWidth(), GetManager()->GetDefaultFontInfo()->tm.tmHeight + 14);
        }
        return CDuiContainer::EstimateSize(szAvailable);
    }

    void CDuiListHeaderItem::PaintText(HDC hDC)
    {
        if(m_dwTextColor == 0)
        {
            m_dwTextColor = GetManager()->GetDefaultFontColor();
        }

        CDuiRect rcText = GetPos();
        rcText.left += GetTextPadding().left;
        rcText.top += GetTextPadding().top;
        rcText.right -= GetTextPadding().right;
        rcText.bottom -= GetTextPadding().bottom;

        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }
        int nLinks = 0;
        if(m_bShowHtml)
        {
            CRenderEngine::DrawHtmlText(hDC, GetManager(), rcText, sText, m_dwTextColor,
                                        NULL, NULL, nLinks, m_uTextStyle);
        }
        else
        {
            CRenderEngine::DrawText(hDC, GetManager(), rcText, sText, m_dwTextColor,
                                    m_iFont, m_uTextStyle);
        }
    }

    RECT CDuiListHeaderItem::GetThumbRect() const
    {
        if(m_iSepWidth >= 0)
        {
            return CDuiRect(GetPos().right - m_iSepWidth, GetPos().top, GetPos().right, GetPos().bottom);
        }
        else
        {
            return CDuiRect(GetPos().left, GetPos().top, GetPos().left - m_iSepWidth, GetPos().bottom);
        }
    }

    void CDuiListHeaderItem::PaintStatusImage(HDC hDC)
    {
        if(IsFocused())
        {
            m_uButtonState |= UISTATE_FOCUSED;
        }
        else
        {
            m_uButtonState &= ~ UISTATE_FOCUSED;
        }

        if((m_uButtonState & UISTATE_PUSHED) != 0)
        {
            if(m_sPushedImage.IsEmpty() && !m_sNormalImage.IsEmpty())
            {
                DrawImage(hDC, (LPCTSTR)m_sNormalImage);
            }
            if(!DrawImage(hDC, (LPCTSTR)m_sPushedImage)) {}
        }
        else if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(m_sHotImage.IsEmpty() && !m_sNormalImage.IsEmpty())
            {
                DrawImage(hDC, (LPCTSTR)m_sNormalImage);
            }
            if(!DrawImage(hDC, (LPCTSTR)m_sHotImage)) {}
        }
        else if((m_uButtonState & UISTATE_FOCUSED) != 0)
        {
            if(m_sFocusedImage.IsEmpty() && !m_sNormalImage.IsEmpty())
            {
                DrawImage(hDC, (LPCTSTR)m_sNormalImage);
            }
            if(!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) {}
        }
        else
        {
            if(!m_sNormalImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) {}
            }
        }

        if(!m_sSepImage.IsEmpty())
        {
            RECT rcThumb = GetThumbRect();
            rcThumb.left -= GetPos().left;
            rcThumb.top -= GetPos().top;
            rcThumb.right -= GetPos().left;
            rcThumb.bottom -= GetPos().top;

            m_sSepImageModify.SetEmpty();
            m_sSepImageModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), rcThumb.left, rcThumb.top, rcThumb.right, rcThumb.bottom);
            if(!DrawImage(hDC, (LPCTSTR)m_sSepImage, (LPCTSTR)m_sSepImageModify)) {}
        }
    }

    void CDuiListHeaderItem::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiContainer::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_SETFOCUS)
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_KILLFOCUS)
        {
            Invalidate();
        }
        if(event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
        {
            if(!IsEnabled())
            {
                return;
            }
            RECT rcSeparator = GetThumbRect();
            if(m_iSepWidth >= 0)
            {
                rcSeparator.left -= 4;
            }
            else
            {
                rcSeparator.right += 4;
            }
            if(::PtInRect(&rcSeparator, event.ptMouse))
            {
                if(m_bDragable)
                {
                    m_uButtonState |= UISTATE_CAPTURED;
                    ptLastMouse = event.ptMouse;
                }
            }
            else
            {
                m_uButtonState |= UISTATE_PUSHED;
                GetManager()->SendNotify(this, DUI_MSGTYPE_HEADERCLICK);
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                m_uButtonState &= ~UISTATE_CAPTURED;
                if(GetParent())
                {
                    GetParent()->NeedParentUpdate();
                }
            }
            else if((m_uButtonState & UISTATE_PUSHED) != 0)
            {
                m_uButtonState &= ~UISTATE_PUSHED;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            if((m_uButtonState & UISTATE_CAPTURED) != 0)
            {
                RECT rc = GetPos();
                if(m_iSepWidth >= 0)
                {
                    rc.right -= ptLastMouse.x - event.ptMouse.x;
                }
                else
                {
                    rc.left -= ptLastMouse.x - event.ptMouse.x;
                }

                if(rc.right - rc.left > GetMinWidth())
                {
                    ptLastMouse = event.ptMouse;
                    SetFixedWidth(rc.right - rc.left);
                }
            }
            return;
        }
        if(event.Type == UIEVENT_SETCURSOR)
        {
            RECT rcSeparator = GetThumbRect();
            if(m_iSepWidth >= 0) //111024 by cddjr, 增加分隔符区域，方便用户拖动
            {
                rcSeparator.left -= 4;
            }
            else
            {
                rcSeparator.right += 4;
            }
            if(IsEnabled() && m_bDragable && ::PtInRect(&rcSeparator, event.ptMouse))
            {
                ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
                return;
            }
        }
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        CDuiContainer::DoEvent(event);
    }

    /////////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiListHeader)

    CDuiListHeader::CDuiListHeader()
        : m_bIsScaleHeader(FALSE)
    {

    }

    CDuiListHeader::~CDuiListHeader()
    {

    }

    CDuiString CDuiListHeader::GetClass() const
    {
        return DUI_CTR_LISTHEADER;
    }

    LPVOID CDuiListHeader::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LISTHEADER) == 0)
        {
            return this;
        }
        return CDuiHorizontalLayout::GetInterface(pstrName);
    }

    BOOL CDuiListHeader::IsScaleHeader() const
    {
        return m_bIsScaleHeader;
    }

    void CDuiListHeader::SetScaleHeader(BOOL bIsScale)
    {
        m_bIsScaleHeader = bIsScale;
    }

    void CDuiListHeader::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("scaleheader")) == 0)
        {
            SetScaleHeader(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiHorizontalLayout::SetAttribute(pstrName, pstrValue);
        }
    }

    SIZE CDuiListHeader::EstimateSize(SIZE szAvailable)
    {
        SIZE cXY = {0, GetFixedHeight()};
        if(cXY.cy == 0 && GetManager() != NULL)
        {
            for(int it = 0; it < GetCount(); it++)
            {
                cXY.cy = MAX(cXY.cy, GetItemAt(it)->EstimateSize(szAvailable).cy);
            }
            int nMin = GetManager()->GetDefaultFontInfo()->tm.tmHeight + 6;
            cXY.cy = MAX(cXY.cy, nMin);
        }

        for(int it = 0; it < GetCount(); it++)
        {
            cXY.cx +=  GetItemAt(it)->EstimateSize(szAvailable).cx;
        }

        return cXY;
    }

    void CDuiListHeader::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();

        // Adjust for inset
        rc.left += GetInset().left;
        rc.top += GetInset().top;
        rc.right -= GetInset().right;
        rc.bottom -= GetInset().bottom;

        if(GetCount() == 0)
        {
            return;
        }

        // Determine the width of elements that are sizeable
        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };

        int nAdjustables = 0;
        int cxFixed = 0;
        int nEstimateNum = 0;
        for(int it1 = 0; it1 < GetCount(); it1++)
        {
            CDuiControl* pControl = GetItemAt(it1);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            if(sz.cx == 0)
            {
                nAdjustables++;
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            cxFixed += sz.cx +  pControl->GetPadding().left + pControl->GetPadding().right;
            nEstimateNum++;
        }
        cxFixed += (nEstimateNum - 1) * GetChildPadding();

        int cxExpand = 0;
        int cxNeeded = 0;
        if(nAdjustables > 0)
        {
            cxExpand = MAX(0, (szAvailable.cx - cxFixed) / nAdjustables);
        }
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosX = rc.left;

        int iAdjustable = 0;
        int cxFixedRemaining = cxFixed;

        int nHeaderWidth = GetWidth();
        CDuiList* pList = static_cast<CDuiList*>(GetParent());
        if(pList != NULL)
        {
            CDuiScrollBar* pVScroll = pList->GetVerticalScrollBar();
            if(pVScroll != NULL)
            {
                nHeaderWidth -= pVScroll->GetWidth();
            }
        }
        for(int it2 = 0; it2 < GetCount(); it2++)
        {
            CDuiControl* pControl = GetItemAt(it2);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(it2);
                continue;
            }
            RECT rcPadding = pControl->GetPadding();
            szRemaining.cx -= rcPadding.left;

            SIZE sz = {0, 0};
            if(m_bIsScaleHeader)
            {
                CDuiListHeaderItem* pHeaderItem = static_cast<CDuiListHeaderItem*>(pControl);
                sz.cx = int(nHeaderWidth * (float)pHeaderItem->GetScale() / 100);
            }
            else
            {
                sz = pControl->EstimateSize(szRemaining);
            }

            if(sz.cx == 0)
            {
                iAdjustable++;
                sz.cx = cxExpand;
                // Distribute remaining to last element (usually round-off left-overs)
                if(iAdjustable == nAdjustables)
                {
                    sz.cx = MAX(0, szRemaining.cx - rcPadding.right - cxFixedRemaining);
                }
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }
            }
            else
            {
                if(sz.cx < pControl->GetMinWidth())
                {
                    sz.cx = pControl->GetMinWidth();
                }
                if(sz.cx > pControl->GetMaxWidth())
                {
                    sz.cx = pControl->GetMaxWidth();
                }

                cxFixedRemaining -= sz.cx;
            }

            sz.cy = pControl->GetFixedHeight();
            if(sz.cy == 0)
            {
                sz.cy = rc.bottom - rc.top - rcPadding.top - rcPadding.bottom;
            }
            if(sz.cy < 0)
            {
                sz.cy = 0;
            }
            if(sz.cy < pControl->GetMinHeight())
            {
                sz.cy = pControl->GetMinHeight();
            }
            if(sz.cy > pControl->GetMaxHeight())
            {
                sz.cy = pControl->GetMaxHeight();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, rc.top + rcPadding.top, iPosX + sz.cx + rcPadding.left + rcPadding.right, rc.top + rcPadding.top + sz.cy};
            pControl->SetPos(rcCtrl);
            iPosX += sz.cx + GetChildPadding() + rcPadding.left + rcPadding.right;
            cxNeeded += sz.cx + rcPadding.left + rcPadding.right;
            szRemaining.cx -= sz.cx + GetChildPadding() + rcPadding.right;
        }
        cxNeeded += (nEstimateNum - 1) * GetChildPadding();
    }


    /////////////////////////////////////////////////////////////////////////////

    IMPLEMENT_DUICONTROL(CDuiListContainerElement)

    CDuiListContainerElement::CDuiListContainerElement()
        : m_iIndex(-1)
        , m_bSelected(FALSE)
        , m_uButtonState(0)
        , m_pOwner(NULL)
    {

    }

    CDuiListContainerElement::~CDuiListContainerElement()
    {

    }

    CDuiString CDuiListContainerElement::GetClass() const
    {
        return DUI_CTR_LISTCONTAINERELEMENT;
    }

    UINT CDuiListContainerElement::GetControlFlags() const
    {
        return UIFLAG_WANTRETURN;
    }

    LPVOID CDuiListContainerElement::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, GET_CLASS_NAME(IListItem)) == 0)
        {
            return static_cast<IListItem*>(this);
        }
        if(_tcsicmp(pstrName, DUI_CTR_LISTCONTAINERELEMENT) == 0)
        {
            return static_cast<CDuiListContainerElement*>(this);
        }
        return CDuiHorizontalLayout::GetInterface(pstrName);
    }

    int CDuiListContainerElement::GetIndex() const
    {
        return m_iIndex;
    }

    void CDuiListContainerElement::SetIndex(int iIndex)
    {
        m_iIndex = iIndex;
    }

    IListOwner* CDuiListContainerElement::GetOwner()
    {
        return m_pOwner;
    }

    void CDuiListContainerElement::SetOwner(CDuiControl* pOwner)
    {
        m_pOwner = static_cast<IListOwner*>(pOwner->GetInterface(GET_CLASS_NAME(IListOwner)));
    }

    void CDuiListContainerElement::SetVisible(BOOL bVisible /*= TRUE*/)
    {
        CDuiContainer::SetVisible(bVisible);
        if(!IsVisible() && m_bSelected)
        {
            m_bSelected = FALSE;
            if(m_pOwner != NULL)
            {
                m_pOwner->SelectItem(-1);
            }
        }
    }

    void CDuiListContainerElement::SetEnabled(BOOL bEnable /*= TRUE*/)
    {
        CDuiControl::SetEnabled(bEnable);
        if(!IsEnabled())
        {
            m_uButtonState = 0;
        }
    }

    BOOL CDuiListContainerElement::IsSelected() const
    {
        return m_bSelected;
    }

    BOOL CDuiListContainerElement::Select(BOOL bSelect /*= TRUE*/)
    {
        if(!IsEnabled())
        {
            return FALSE;
        }
        if(m_pOwner != NULL && m_bSelected)
        {
            m_pOwner->UnSelectItem(m_iIndex, TRUE);
        }
        if(bSelect == m_bSelected)
        {
            return TRUE;
        }
        m_bSelected = bSelect;
        if(bSelect && m_pOwner != NULL)
        {
            m_pOwner->SelectItem(m_iIndex);
        }
        Invalidate();

        return TRUE;
    }

    BOOL CDuiListContainerElement::SelectMulti(BOOL bSelect /*= TRUE*/)
    {
        if(!IsEnabled())
        {
            return FALSE;
        }
        if(bSelect == m_bSelected)
        {
            return TRUE;
        }

        m_bSelected = bSelect;
        if(bSelect && m_pOwner != NULL)
        {
            m_pOwner->SelectMultiItem(m_iIndex);
        }
        Invalidate();
        return TRUE;
    }

    BOOL CDuiListContainerElement::IsExpanded() const
    {
        return FALSE;
    }

    BOOL CDuiListContainerElement::Expand(BOOL bExpand /*= TRUE*/)
    {
        return FALSE;
    }

    void CDuiListContainerElement::Invalidate()
    {
        if(!IsVisible())
        {
            return;
        }

        if(GetParent())
        {
            CDuiContainer* pParentContainer = static_cast<CDuiContainer*>(GetParent()->GetInterface(DUI_CTR_CONTAINER));
            if(pParentContainer)
            {
                RECT rc = pParentContainer->GetPos();
                RECT rcInset = pParentContainer->GetInset();
                rc.left += rcInset.left;
                rc.top += rcInset.top;
                rc.right -= rcInset.right;
                rc.bottom -= rcInset.bottom;
                CDuiScrollBar* pVerticalScrollBar = pParentContainer->GetVerticalScrollBar();
                if(pVerticalScrollBar && pVerticalScrollBar->IsVisible())
                {
                    rc.right -= pVerticalScrollBar->GetFixedWidth();
                }
                CDuiScrollBar* pHorizontalScrollBar = pParentContainer->GetHorizontalScrollBar();
                if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
                {
                    rc.bottom -= pHorizontalScrollBar->GetFixedHeight();
                }

                RECT invalidateRc = GetPos();
                if(!::IntersectRect(&invalidateRc, &GetPos(), &rc))
                {
                    return;
                }

                CDuiControl* pParent = GetParent();
                CDuiRect rcTemp;
                CDuiRect rcParent;
                while(pParent = pParent->GetParent())
                {
                    rcTemp = invalidateRc;
                    rcParent = pParent->GetPos();
                    if(!::IntersectRect(&invalidateRc, &rcTemp, &rcParent))
                    {
                        return;
                    }
                }

                if(GetManager() != NULL)
                {
                    GetManager()->Invalidate(invalidateRc);
                }
            }
            else
            {
                CDuiContainer::Invalidate();
            }
        }
        else
        {
            CDuiControl::Invalidate();
        }
    }

    BOOL CDuiListContainerElement::Activate()
    {
        if(!CDuiContainer::Activate())
        {
            return FALSE;
        }
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMACTIVATE);
        }
        return TRUE;
    }

    void CDuiListContainerElement::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("selected")) == 0)
        {
            Select();
        }
        else
        {
            CDuiHorizontalLayout::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiListContainerElement::DrawItemText(HDC hDC, const RECT& rcItem)
    {

    }

    void CDuiListContainerElement::DrawItemBk(HDC hDC, const RECT& rcItem)
    {
        ASSERT(m_pOwner);
        if(m_pOwner == NULL)
        {
            return;
        }
        ListInfo* pInfo = m_pOwner->GetListInfo();
        DWORD iBackColor = 0;
        if(!pInfo->bAlternateBk || m_iIndex % 2 == 0)
        {
            iBackColor = pInfo->dwBkColor;
        }

        if((m_uButtonState & UISTATE_HOT) != 0)
        {
            iBackColor = pInfo->dwHotBkColor;
        }
        if(IsSelected())
        {
            iBackColor = pInfo->dwSelectedBkColor;
        }
        if(!IsEnabled())
        {
            iBackColor = pInfo->dwDisabledBkColor;
        }
        if(iBackColor != 0)
        {
            CRenderEngine::DrawColor(hDC, GetPos(), GetAdjustColor(iBackColor));
        }

        if(!IsEnabled())
        {
            if(!pInfo->sDisabledImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sDisabledImage)) {}
                else
                {
                    return;
                }
            }
        }
        if(IsSelected())
        {
            if(!pInfo->sSelectedImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sSelectedImage)) {}
                else
                {
                    return;
                }
            }
        }
        if((m_uButtonState & UISTATE_HOT) != 0)
        {
            if(!pInfo->sHotImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sHotImage)) {}
                else
                {
                    return;
                }
            }
        }
        if(!GetBkImage().IsEmpty())
        {
            if(!pInfo->bAlternateBk || m_iIndex % 2 == 0)
            {
                if(!DrawImage(hDC, (LPCTSTR)GetBkImage())) {}
            }
        }

        if(GetBkImage().IsEmpty())
        {
            if(!pInfo->sBkImage.IsEmpty())
            {
                if(!DrawImage(hDC, (LPCTSTR)pInfo->sBkImage)) {}
                else
                {
                    return;
                }
            }
        }

        if(pInfo->dwLineColor != 0)
        {
            if(pInfo->bShowRowLine)
            {
                RECT rcLine = { GetPos().left, GetPos().bottom - 1, GetPos().right, GetPos().bottom - 1 };
                CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
            }
            if(pInfo->bShowColumnLine)
            {
                for(int i = 0; i < pInfo->nColumns; i++)
                {
                    RECT rcLine = { pInfo->rcColumn[i].right - 1, GetPos().top, pInfo->rcColumn[i].right - 1, GetPos().bottom };
                    CRenderEngine::DrawLine(hDC, rcLine, 1, GetAdjustColor(pInfo->dwLineColor));
                }
            }
        }
    }

    void CDuiListContainerElement::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }

        CDuiRect rcTemp;
        if(!::IntersectRect(&rcTemp, &rcPaint, &GetPos()))
        {
            return;
        }

        CRenderClip clip;
        CRenderClip::GenerateClip(hDC, rcTemp, clip);
        CDuiControl::DoPaint(hDC, rcPaint);

        DrawItemBk(hDC, GetPos());

        if(GetCount() > 0)
        {
            RECT rc = GetPos();
            rc.left += GetInset().left;
            rc.top +=  GetInset().top;
            rc.right -=  GetInset().right;
            rc.bottom -=  GetInset().bottom;
            if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
            {
                rc.right -= GetVerticalScrollBar()->GetFixedWidth();
            }
            if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
            {
                rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
            }

            if(!::IntersectRect(&rcTemp, &rcPaint, &rc))
            {
                for(int it = 0; it < GetCount(); it++)
                {
                    CDuiControl* pControl = GetItemAt(it);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
            else
            {
                CRenderClip childClip;
                CRenderClip::GenerateClip(hDC, rcTemp, childClip);
                for(int it = 0; it < GetCount(); it++)
                {
                    CDuiControl* pControl = GetItemAt(it);
                    if(!pControl->IsVisible())
                    {
                        continue;
                    }
                    if(!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos()))
                    {
                        continue;
                    }
                    if(pControl ->IsFloat())
                    {
                        if(!::IntersectRect(&rcTemp, &GetPos(), &pControl->GetPos()))
                        {
                            continue;
                        }
                        CRenderClip::UseOldClipBegin(hDC, childClip);
                        pControl->DoPaint(hDC, rcPaint);
                        CRenderClip::UseOldClipEnd(hDC, childClip);
                    }
                    else
                    {
                        if(!::IntersectRect(&rcTemp, &rc, &pControl->GetPos()))
                        {
                            continue;
                        }
                        pControl->DoPaint(hDC, rcPaint);
                    }
                }
            }
        }

        if(GetVerticalScrollBar() != NULL && GetVerticalScrollBar()->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &GetVerticalScrollBar()->GetPos()))
            {
                GetVerticalScrollBar()->DoPaint(hDC, rcPaint);
            }
        }

        if(GetHorizontalScrollBar() != NULL && GetHorizontalScrollBar()->IsVisible())
        {
            if(::IntersectRect(&rcTemp, &rcPaint, &GetHorizontalScrollBar()->GetPos()))
            {
                GetHorizontalScrollBar()->DoPaint(hDC, rcPaint);
            }
        }
    }

    void CDuiListContainerElement::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiHorizontalLayout::SetPos(rc, bNeedInvalidate);
        if(m_pOwner == NULL)
        {
            return;
        }
        UINT uListType = m_pOwner->GetListType();
        if(uListType != LT_LIST && uListType != LT_TREE)
        {
            return;
        }

        CDuiList* pList = static_cast<CDuiList*>(m_pOwner);

        if(uListType == LT_TREE)
        {
            pList = static_cast<CDuiList*>(pList->GetInterface(DUI_CTR_LIST));
            if(pList == NULL)
            {
                return;
            }
        }

        CDuiListHeader* pHeader = pList->GetHeader();
        if(pHeader == NULL || !pHeader->IsVisible())
        {
            return;
        }
        int nCount = GetCount();
        for(int i = 0; i < nCount; i++)
        {
            CDuiControl* pListItem = GetItemAt(i);
            CDuiControl* pHeaderItem = pHeader->GetItemAt(i);
            if(pHeaderItem == NULL)
            {
                return;
            }
            RECT rcHeaderItem = pHeaderItem->GetPos();
            if(pListItem != NULL && !(rcHeaderItem.left == 0 && rcHeaderItem.right == 0))
            {
                RECT rt = pListItem->GetPos();
                rt.left = rcHeaderItem.left;
                rt.right = rcHeaderItem.right;
                pListItem->SetPos(rt);
            }
        }
    }

    void CDuiListContainerElement::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(m_pOwner != NULL)
            {
                m_pOwner->DoEvent(event);
            }
            else
            {
                CDuiContainer::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_DBLCLICK)
        {
            if(IsEnabled())
            {
                Activate();
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_KEYDOWN && IsEnabled())
        {
            if(event.chKey == VK_RETURN)
            {
                Activate();
                Invalidate();
                return;
            }
        }
        if(event.Type == UIEVENT_BUTTONDOWN)
        {
            if(IsEnabled())
            {
                if((GetKeyState(VK_CONTROL) & 0x8000))
                {
                    SelectMulti(!IsSelected());
                }
                else
                {
                    Select();
                }
            }
            return;
        }
        if(event.Type == UIEVENT_BUTTONUP)
        {
            if(IsEnabled())
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMCLICK);
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSEMOVE)
        {
            return;
        }
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                m_uButtonState |= UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            if((m_uButtonState & UISTATE_HOT) != 0)
            {
                m_uButtonState &= ~UISTATE_HOT;
                Invalidate();
            }
            return;
        }
        if(event.Type == UIEVENT_TIMER)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_TIMER, event.wParam, event.lParam);
            return;
        }

        if(event.Type == UIEVENT_CONTEXTMENU)
        {
            if(IsContextMenuUsed())
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
                return;
            }
        }
        // An important twist: The list-item will send the event not to its immediate
        // parent but to the "attached" list. A list may actually embed several components
        // in its path to the item, but key-presses etc. needs to go to the actual list.
        if(m_pOwner != NULL)
        {
            m_pOwner->DoEvent(event);
        }
        else
        {
            CDuiControl::DoEvent(event);
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiListTextElement)

    CDuiListTextElement::CDuiListTextElement()
        : m_nLinks(0)
        , m_nHoverLink(-1)
        , m_pOwner(NULL)
    {

    }

    CDuiListTextElement::~CDuiListTextElement()
    {
        CDuiString* pText = NULL;
        for(int it = 0; it < m_aTexts.GetSize(); it++)
        {
            pText = static_cast<CDuiString*>(m_aTexts[it]);
            DUI_FREE_POINT(pText);
        }
        m_aTexts.Empty();
    }

    CDuiString CDuiListTextElement::GetClass() const
    {
        return DUI_CTR_LISTTEXTELEMENT;
    }

    LPVOID CDuiListTextElement::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LISTTEXTELEMENT) == 0)
        {
            return static_cast<CDuiListTextElement*>(this);
        }
        return CDuiListLabelElement::GetInterface(pstrName);
    }

    UINT CDuiListTextElement::GetControlFlags() const
    {
        return UIFLAG_WANTRETURN | ((IsEnabled() && m_nLinks > 0) ? UIFLAG_SETCURSOR : 0);
    }

    LPCTSTR CDuiListTextElement::GetText(int iIndex) const
    {
        CDuiString* pText = static_cast<CDuiString*>(m_aTexts.GetAt(iIndex));
        if(pText)
        {
            if(!IsResourceText())
            {
                return pText->GetData();
            }
            return CDuiResourceManager::GetInstance()->GetText(*pText);
        }
        return NULL;
    }

    void CDuiListTextElement::SetText(int iIndex, LPCTSTR pstrText)
    {
        if(m_pOwner == NULL)
        {
            return;
        }

        ListInfo* pInfo = m_pOwner->GetListInfo();
        if(iIndex < 0 || iIndex >= pInfo->nColumns)
        {
            return;
        }
        while(m_aTexts.GetSize() < pInfo->nColumns)
        {
            m_aTexts.Add(NULL);
        }

        CDuiString* pText = static_cast<CDuiString*>(m_aTexts[iIndex]);
        if((pText == NULL && pstrText == NULL) || (pText && *pText == pstrText))
        {
            return;
        }

        if(pText)
        {
            DUI_FREE_POINT(pText);
        }
        m_aTexts.SetAt(iIndex, new CDuiString(pstrText));

        Invalidate();
    }

    void CDuiListTextElement::SetOwner(CDuiControl* pOwner)
    {
        CDuiListElement::SetOwner(pOwner);
        m_pOwner = static_cast<IList*>(pOwner->GetInterface(GET_CLASS_NAME(IList)));
    }

    CDuiString* CDuiListTextElement::GetLinkContent(int iIndex)
    {
        if(iIndex >= 0 && iIndex < m_nLinks)
        {
            return &m_sLinks[iIndex];
        }
        return NULL;
    }

    void CDuiListTextElement::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(m_pOwner != NULL)
            {
                m_pOwner->DoEvent(event);
            }
            else
            {
                CDuiListLabelElement::DoEvent(event);
            }
            return;
        }

        // When you hover over a link
        if(event.Type == UIEVENT_SETCURSOR)
        {
            for(int i = 0; i < m_nLinks; i++)
            {
                if(::PtInRect(&m_rcLinks[i], event.ptMouse))
                {
                    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
                    return;
                }
            }
        }
        if(event.Type == UIEVENT_BUTTONUP && IsEnabled())
        {
            for(int i = 0; i < m_nLinks; i++)
            {
                if(::PtInRect(&m_rcLinks[i], event.ptMouse))
                {
                    GetManager()->SendNotify(this, DUI_MSGTYPE_LINK, i);
                    return;
                }
            }
        }
        if(m_nLinks > 0 && event.Type == UIEVENT_MOUSEMOVE)
        {
            int nHoverLink = -1;
            for(int i = 0; i < m_nLinks; i++)
            {
                if(::PtInRect(&m_rcLinks[i], event.ptMouse))
                {
                    nHoverLink = i;
                    break;
                }
            }

            if(m_nHoverLink != nHoverLink)
            {
                Invalidate();
                m_nHoverLink = nHoverLink;
            }
        }
        if(m_nLinks > 0 && event.Type == UIEVENT_MOUSELEAVE)
        {
            if(m_nHoverLink != -1)
            {
                Invalidate();
                m_nHoverLink = -1;
            }
        }
        CDuiListLabelElement::DoEvent(event);
    }

    SIZE CDuiListTextElement::EstimateSize(SIZE szAvailable)
    {
        ListInfo* pInfo = NULL;
        if(m_pOwner)
        {
            pInfo = m_pOwner->GetListInfo();
        }

        SIZE cXY = {GetFixedWidth(), GetFixedHeight()};
        if(cXY.cy == 0 && GetManager() != NULL)
        {
            cXY.cy = GetManager()->GetFontInfo(pInfo->nFont)->tm.tmHeight + 8;
            if(pInfo)
            {
                cXY.cy += pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
            }
        }

        return cXY;
    }

    void CDuiListTextElement::DrawItemText(HDC hDC, const RECT& rcItem)
    {
        if(m_pOwner == NULL)
        {
            return;
        }
        ListInfo* pInfo = m_pOwner->GetListInfo();
        DWORD iTextColor = pInfo->dwTextColor;

        if((GetButtonState() & UISTATE_HOT) != 0)
        {
            iTextColor = pInfo->dwHotTextColor;
        }
        if(IsSelected())
        {
            iTextColor = pInfo->dwSelectedTextColor;
        }
        if(!IsEnabled())
        {
            iTextColor = pInfo->dwDisabledTextColor;
        }
        IListCallback* pCallback = m_pOwner->GetTextCallback();

        m_nLinks = 0;
        int nLinks = SIZEOF_ARRAY(m_rcLinks);
        for(int i = 0; i < pInfo->nColumns; i++)
        {
            RECT rcItem = { pInfo->rcColumn[i].left, GetPos().top, pInfo->rcColumn[i].right, GetPos().bottom };
            rcItem.left += pInfo->rcTextPadding.left;
            rcItem.right -= pInfo->rcTextPadding.right;
            rcItem.top += pInfo->rcTextPadding.top;
            rcItem.bottom -= pInfo->rcTextPadding.bottom;

            CDuiString strText;
            if(pCallback)
            {
                strText = pCallback->GetItemText(this, GetIndex(), i);
            }
            else
            {
                strText.Assign(GetText(i));
            }

            if(pInfo->bShowHtml)
                CRenderEngine::DrawHtmlText(hDC, GetManager(), rcItem, strText.GetData(), iTextColor, \
                                            &m_rcLinks[m_nLinks], &m_sLinks[m_nLinks], nLinks, pInfo->uTextStyle);
            else
                CRenderEngine::DrawText(hDC, GetManager(), rcItem, strText.GetData(), iTextColor, \
                                        pInfo->nFont, pInfo->uTextStyle);

            m_nLinks += nLinks;
            nLinks = SIZEOF_ARRAY(m_rcLinks) - m_nLinks;
        }
        for(int i = m_nLinks; i < SIZEOF_ARRAY(m_rcLinks); i++)
        {
            m_rcLinks[i].Empty();
            m_sLinks[i].SetEmpty();
        }
    }

    ////////////////////////////////////////////////////////////
    CDuiListBody::CDuiListBody(CDuiList* pOwner)
        : m_pOwner(pOwner)
        , m_pCompareFunc(NULL)
        , m_compareData(0)
    {
        ASSERT(m_pOwner);
    }

    CDuiListBody::~CDuiListBody()
    {

    }

    BOOL CDuiListBody::SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
    {
        if(!pfnCompare)
        {
            return FALSE;
        }
        m_pCompareFunc = pfnCompare;
        m_compareData = dwData;
        CDuiControl** pData = (CDuiControl**)GetItems()->GetData();
        qsort_s(GetItems()->GetData(), GetCount(), sizeof(CDuiControl*), CDuiListBody::ItemComareFunc, this);
        for(int i = 0; i < GetCount(); ++i)
        {
            IListItem* pItem = (IListItem*)(GetItemAt(i)->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pItem)
            {
                pItem->SetIndex(i);
                pItem->Select(FALSE);
            }
        }
        m_pOwner->SelectItem(-1);
        if(GetManager())
        {
            SetPos(GetPos());
            Invalidate();
        }

        return TRUE;
    }

    int CDuiListBody::GetScrollStepSize() const
    {
        if(m_pOwner != NULL)
        {
            return m_pOwner->GetScrollStepSize();
        }

        return CDuiVerticalLayout::GetScrollStepSize();
    }

    void CDuiListBody::SetScrollPos(SIZE szPos, BOOL bMsg /*= TRUE*/)
    {
        int cx = 0;
        int cy = 0;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            int iLastScrollPos = GetVerticalScrollBar()->GetScrollPos();
            GetVerticalScrollBar()->SetScrollPos(szPos.cy);
            cy = GetVerticalScrollBar()->GetScrollPos() - iLastScrollPos;
        }

        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            int iLastScrollPos = GetHorizontalScrollBar() ->GetScrollPos();
            GetHorizontalScrollBar()->SetScrollPos(szPos.cx);
            cx = GetHorizontalScrollBar()->GetScrollPos() - iLastScrollPos;
        }

        CDuiRect rcPos;
        for(int it2 = 0; it2 < GetCount(); it2++)
        {
            CDuiControl* pControl = GetItemAt(it2);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }

            rcPos = pControl->GetPos();
            rcPos.left -= cx;
            rcPos.right -= cx;
            rcPos.top -= cy;
            rcPos.bottom -= cy;
            pControl->SetPos(rcPos, TRUE);
        }

        Invalidate();
        if(m_pOwner)
        {
            CDuiListHeader* pHeader = m_pOwner->GetHeader();
            if(pHeader == NULL)
            {
                return;
            }
            ListInfo* pInfo = m_pOwner->GetListInfo();
            pInfo->nColumns = MIN(pHeader->GetCount(), UILIST_MAX_COLUMNS);

            if(!pHeader->IsVisible())
            {
                for(int it = 0; it < pHeader->GetCount(); it++)
                {
                    pHeader->GetItemAt(it)->SetInternVisible(TRUE);
                }
            }
            for(int i = 0; i < pInfo->nColumns; i++)
            {
                CDuiControl* pControl = pHeader->GetItemAt(i);
                if(!pControl->IsVisible())
                {
                    continue;
                }
                if(pControl->IsFloat())
                {
                    continue;
                }

                RECT rcPos = pControl->GetPos();
                rcPos.left -= cx;
                rcPos.right -= cx;
                pControl->SetPos(rcPos);
                pInfo->rcColumn[i] = pControl->GetPos();
            }
            if(!pHeader->IsVisible())
            {
                for(int it = 0; it < pHeader->GetCount(); it++)
                {
                    pHeader->GetItemAt(it)->SetInternVisible(FALSE);
                }
            }
        }
    }

    void CDuiListBody::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiControl::SetPos(rc, bNeedInvalidate);
        rc = GetPos();

        // Adjust for inset
        rc.left += GetInset().left;
        rc.top += GetInset().top;
        rc.right -= GetInset().right;
        rc.bottom -= GetInset().bottom;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            rc.right -= GetVerticalScrollBar()->GetFixedWidth();
        }
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
        }

        // Determine the minimum size
        SIZE szAvailable = { rc.right - rc.left, rc.bottom - rc.top };
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            szAvailable.cx += GetHorizontalScrollBar()->GetScrollRange();
        }

        int cxNeeded = 0;
        int nAdjustables = 0;
        int cyFixed = 0;
        int nEstimateNum = 0;
        for(int it1 = 0; it1 < GetCount(); it1++)
        {
            CDuiControl* pControl = GetItemAt(it1);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            if(sz.cy == 0)
            {
                nAdjustables++;
            }
            else
            {
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
            }
            cyFixed += sz.cy + pControl->GetPadding().top + pControl->GetPadding().bottom;

            RECT rcPadding = pControl->GetPadding();
            sz.cx = MAX(sz.cx, 0);
            if(sz.cx < pControl->GetMinWidth())
            {
                sz.cx = pControl->GetMinWidth();
            }
            if(sz.cx > pControl->GetMaxWidth())
            {
                sz.cx = pControl->GetMaxWidth();
            }
            cxNeeded = MAX(cxNeeded, sz.cx);
            nEstimateNum++;
        }
        cyFixed += (nEstimateNum - 1) * GetChildPadding();

        if(m_pOwner)
        {
            CDuiListHeader* pHeader = m_pOwner->GetHeader();
            if(pHeader != NULL && pHeader->GetCount() > 0)
            {
                cxNeeded = MAX(0, pHeader->EstimateSize(CDuiSize(rc.right - rc.left, rc.bottom - rc.top)).cx);
                if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
                {
                    int nOffset = GetHorizontalScrollBar()->GetScrollPos();
                    RECT rcHeader = pHeader->GetPos();
                    rcHeader.left = rc.left - nOffset;
                    pHeader->SetPos(rcHeader);
                }
            }
        }

        // Place elements
        int cyNeeded = 0;
        int cyExpand = 0;
        if(nAdjustables > 0)
        {
            cyExpand = MAX(0, (szAvailable.cy - cyFixed) / nAdjustables);
        }
        // Position the elements
        SIZE szRemaining = szAvailable;
        int iPosY = rc.top;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            iPosY -= GetVerticalScrollBar()->GetScrollPos();
        }
        int iPosX = rc.left;
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            iPosX -= GetHorizontalScrollBar()->GetScrollPos();
        }
        int iAdjustable = 0;
        int cyFixedRemaining = cyFixed;
        for(int it2 = 0; it2 < GetCount(); it2++)
        {
            CDuiControl* pControl = GetItemAt(it2);
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                SetFloatPos(it2);
                continue;
            }

            RECT rcPadding = pControl->GetPadding();
            szRemaining.cy -= rcPadding.top;
            SIZE sz = pControl->EstimateSize(szRemaining);
            if(sz.cy == 0)
            {
                iAdjustable++;
                sz.cy = cyExpand;
                // Distribute remaining to last element (usually round-off left-overs)
                if(iAdjustable == nAdjustables)
                {
                    sz.cy = MAX(0, szRemaining.cy - rcPadding.bottom - cyFixedRemaining);
                }
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
            }
            else
            {
                if(sz.cy < pControl->GetMinHeight())
                {
                    sz.cy = pControl->GetMinHeight();
                }
                if(sz.cy > pControl->GetMaxHeight())
                {
                    sz.cy = pControl->GetMaxHeight();
                }
                cyFixedRemaining -= sz.cy;
            }

            sz.cx = MAX(cxNeeded, szAvailable.cx - rcPadding.left - rcPadding.right);

            if(sz.cx < pControl->GetMinWidth())
            {
                sz.cx = pControl->GetMinWidth();
            }
            if(sz.cx > pControl->GetMaxWidth())
            {
                sz.cx = pControl->GetMaxWidth();
            }

            RECT rcCtrl = { iPosX + rcPadding.left, iPosY + rcPadding.top, iPosX + rcPadding.left + sz.cx, iPosY + sz.cy + rcPadding.top + rcPadding.bottom };
            pControl->SetPos(rcCtrl);

            iPosY += sz.cy + GetChildPadding() + rcPadding.top + rcPadding.bottom;
            cyNeeded += sz.cy + rcPadding.top + rcPadding.bottom;
            szRemaining.cy -= sz.cy + GetChildPadding() + rcPadding.bottom;
        }
        cyNeeded += (nEstimateNum - 1) * GetChildPadding();

        if(GetHorizontalScrollBar() != NULL)
        {
            if(cxNeeded > rc.right - rc.left)
            {
                if(GetHorizontalScrollBar()->IsVisible())
                {
                    GetHorizontalScrollBar()->SetScrollRange(cxNeeded - (rc.right - rc.left));
                }
                else
                {
                    GetHorizontalScrollBar()->SetVisible(TRUE);
                    GetHorizontalScrollBar()->SetScrollRange(cxNeeded - (rc.right - rc.left));
                    GetHorizontalScrollBar()->SetScrollPos(0);
                    rc.bottom -= GetHorizontalScrollBar()->GetFixedHeight();
                }
            }
            else
            {
                if(GetHorizontalScrollBar()->IsVisible())
                {
                    GetHorizontalScrollBar()->SetVisible(FALSE);
                    GetHorizontalScrollBar()->SetScrollRange(0);
                    GetHorizontalScrollBar()->SetScrollPos(0);
                    rc.bottom += GetHorizontalScrollBar()->GetFixedHeight();
                }
            }
        }

        // Process the scrollbar
        ProcessScrollBar(rc, cxNeeded, cyNeeded);
    }

    void CDuiListBody::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(m_pOwner != NULL)
            {
                m_pOwner->DoEvent(event);
            }
            else
            {
                CDuiVerticalLayout::DoEvent(event);
            }
            return;
        }

        CDuiVerticalLayout::DoEvent(event);
    }

    int __cdecl CDuiListBody::ItemComareFunc(void* pvlocale, const void* item1, const void* item2)
    {
        CDuiListBody* pThis = (CDuiListBody*)pvlocale;
        if(!pThis || !item1 || !item2)
        {
            return 0;
        }
        return pThis->ItemComareFunc(item1, item2);
    }

    int __cdecl CDuiListBody::ItemComareFunc(const void* item1, const void* item2)
    {
        CDuiControl* pControl1 = *(CDuiControl**)item1;
        CDuiControl* pControl2 = *(CDuiControl**)item2;
        return m_pCompareFunc((UINT_PTR)pControl1, (UINT_PTR)pControl2, m_compareData);
    }

    ////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4355)

    IMPLEMENT_DUICONTROL(CDuiList)

    CDuiList::CDuiList(void)
        : m_bMultiSel(FALSE)
        , m_bScrollSelect(FALSE)
        , m_iCurSel(-1)
        , m_iExpandedItem(-1)
        , m_pCallback(NULL)
        , m_pList(new CDuiListBody(this))
        , m_pHeader(new CDuiListHeader)
    {
        m_ListInfo.bAlternateBk = FALSE;
        m_ListInfo.bShowRowLine = FALSE;
        m_ListInfo.bShowColumnLine = FALSE;
        m_ListInfo.bShowHtml = FALSE;
        m_ListInfo.bMultiExpandable = FALSE;
        m_ListInfo.dwTextColor = 0xFF000000;
        m_ListInfo.dwBkColor = 0;
        m_ListInfo.dwSelectedTextColor = 0xFF000000;
        m_ListInfo.dwSelectedBkColor = 0xFFC1E3FF;
        m_ListInfo.dwHotTextColor = 0xFF000000;
        m_ListInfo.dwHotBkColor = 0xFFE9F5FF;
        m_ListInfo.dwDisabledTextColor = 0xFFCCCCCC;
        m_ListInfo.dwDisabledBkColor = 0xFFFFFFFF;
        m_ListInfo.dwLineColor = 0;
        m_ListInfo.nColumns = 0;
        m_ListInfo.nFont = -1;
        m_ListInfo.uTextStyle = DT_VCENTER | DT_SINGLELINE;

        Add(m_pHeader);
        CDuiVerticalLayout::Add(m_pList);
    }


    CDuiList::~CDuiList(void)
    {
    }

    CDuiString CDuiList::GetClass() const
    {
        return DUI_CTR_LIST;
    }

    LPVOID CDuiList::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_LIST) == 0)
        {
            return static_cast<CDuiList*>(this);
        }
        if(_tcsicmp(pstrName, GET_CLASS_NAME(IList)) == 0)
        {
            return static_cast<IList*>(this);
        }
        if(_tcsicmp(pstrName, GET_CLASS_NAME(IListOwner)) == 0)
        {
            return static_cast<IListOwner*>(this);
        }
        return CDuiVerticalLayout::GetInterface(pstrName);
    }

    BOOL CDuiList::SortItems(PULVCompareFunc pfnCompare, UINT_PTR dwData)
    {
        if(!m_pList)
        {
            return FALSE;
        }
        return m_pList->SortItems(pfnCompare, dwData);
    }

    CStdPtrArray* CDuiList::GetItems()
    {
        return m_pList->GetItems();
    }

    CDuiControl* CDuiList::GetItemAt(int iIndex) const
    {
        return m_pList->GetItemAt(iIndex);
    }

    int CDuiList::GetItemIndex(CDuiControl* pControl) const
    {
        if(pControl->GetInterface(DUI_CTR_LISTHEADER) != NULL)
        {
            return CDuiVerticalLayout::GetItemIndex(pControl);
        }
        if(pControl->GetClass() == DUI_CTR_LISTHEADERITEM)
        {
            return m_pHeader->GetItemIndex(pControl);
        }

        return m_pList->GetItemIndex(pControl);
    }

    BOOL CDuiList::SetItemIndex(CDuiControl* pControl, int iIndex)
    {
        if(pControl->GetInterface(DUI_CTR_LISTHEADER) != NULL)
        {
            return CDuiVerticalLayout::SetItemIndex(pControl, iIndex);
        }
        if(pControl->GetClass() == DUI_CTR_LISTHEADERITEM)
        {
            return m_pHeader->SetItemIndex(pControl, iIndex);
        }

        int iOrginIndex = m_pList->GetItemIndex(pControl);
        if(iOrginIndex == -1)
        {
            return FALSE;
        }
        if(iOrginIndex == iIndex)
        {
            return TRUE;
        }

        IListItem* pSelectedListItem = NULL;
        if(m_iCurSel >= 0)
        {
            pSelectedListItem = static_cast<IListItem*>(GetItemAt(m_iCurSel)->GetInterface(GET_CLASS_NAME(IListItem)));
        }
        if(!m_pList->SetItemIndex(pControl, iIndex))
        {
            return FALSE;
        }
        int iMinIndex = min(iOrginIndex, iIndex);
        int iMaxIndex = max(iOrginIndex, iIndex);
        for(int i = iMinIndex; i < iMaxIndex + 1; ++i)
        {
            IListItem* pListItem = static_cast<IListItem*>(m_pList->GetItemAt(i)->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }
        if(m_iCurSel >= 0 && pSelectedListItem != NULL)
        {
            m_iCurSel = pSelectedListItem->GetIndex();
        }
        return TRUE;
    }

    int CDuiList::GetCount() const
    {
        return m_pList->GetCount();
    }

    BOOL CDuiList::Add(CDuiControl* pControl)
    {
        // Override the Add() method so we can add items specifically to
        // the intended widgets. Headers are assumed to be
        // answer the correct interface so we can add multiple list headers.
        if(pControl->GetInterface(DUI_CTR_LISTHEADER) != NULL)
        {
            if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
            {
                CDuiVerticalLayout::Remove(m_pHeader);
                m_pHeader = static_cast<CDuiListHeader*>(pControl);
            }
            m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
            return CDuiVerticalLayout::AddAt(pControl, 0);
        }
        // We also need to recognize header sub-items
        if(pControl->GetClass() == DUI_CTR_LISTHEADERITEM)
        {
            BOOL ret = m_pHeader->Add(pControl);
            m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
            return ret;
        }
        // The list items should know about us
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem != NULL)
        {
            pListItem->SetOwner(this);
            pListItem->SetIndex(GetCount());
        }
        return m_pList->Add(pControl);
    }

    BOOL CDuiList::AddAt(CDuiControl* pControl, int iIndex)
    {
        // Override the AddAt() method so we can add items specifically to
        // the intended widgets. Headers and are assumed to be
        // answer the correct interface so we can add multiple list headers.
        if(pControl->GetInterface(DUI_CTR_LISTHEADER) != NULL)
        {
            if(m_pHeader != pControl && m_pHeader->GetCount() == 0)
            {
                CDuiVerticalLayout::Remove(m_pHeader);
                m_pHeader = static_cast<CDuiListHeader*>(pControl);
            }
            m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
            return CDuiVerticalLayout::AddAt(pControl, 0);
        }
        // We also need to recognize header sub-items
        if(pControl->GetClass() == DUI_CTR_LISTHEADERITEM)
        {
            BOOL ret = m_pHeader->AddAt(pControl, iIndex);
            m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
            return ret;
        }
        if(!m_pList->AddAt(pControl, iIndex))
        {
            return FALSE;
        }

        // The list items should know about us
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem != NULL)
        {
            pListItem->SetOwner(this);
            pListItem->SetIndex(iIndex);
        }

        for(int i = iIndex + 1; i < m_pList->GetCount(); ++i)
        {
            pListItem = static_cast<IListItem*>(m_pList->GetItemAt(i)->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }
        if(m_iCurSel >= iIndex)
        {
            m_iCurSel += 1;
        }
        return TRUE;
    }

    BOOL CDuiList::Remove(CDuiControl* pControl)
    {
        if(pControl->GetInterface(DUI_CTR_LISTHEADER) != NULL)
        {
            return CDuiVerticalLayout::Remove(pControl);
        }
        // We also need to recognize header sub-items
        if(pControl->GetClass() == DUI_CTR_LISTHEADERITEM)
        {
            return m_pHeader->Remove(pControl);
        }

        int iIndex = m_pList->GetItemIndex(pControl);
        if(iIndex == -1)
        {
            return FALSE;
        }

        if(!m_pList->RemoveAt(iIndex))
        {
            return FALSE;
        }

        for(int i = iIndex; i < m_pList->GetCount(); ++i)
        {
            IListItem* pListItem = static_cast<IListItem*>(m_pList->GetItemAt(i)->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }

        if(iIndex == m_iCurSel && m_iCurSel >= 0)
        {
            int iSel = m_iCurSel;
            m_iCurSel = -1;
            SelectItem(FindSelectable(iSel, FALSE));
        }
        else if(iIndex < m_iCurSel)
        {
            m_iCurSel -= 1;
        }
        return TRUE;
    }

    BOOL CDuiList::RemoveAt(int iIndex)
    {
        if(!m_pList->RemoveAt(iIndex))
        {
            return FALSE;
        }

        for(int i = iIndex; i < m_pList->GetCount(); ++i)
        {
            IListItem* pListItem = static_cast<IListItem*>(m_pList->GetItemAt(i)->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem != NULL)
            {
                pListItem->SetIndex(i);
            }
        }

        if(iIndex == m_iCurSel && m_iCurSel >= 0)
        {
            int iSel = m_iCurSel;
            m_iCurSel = -1;
            SelectItem(FindSelectable(iSel, FALSE));
        }
        else if(iIndex < m_iCurSel)
        {
            m_iCurSel -= 1;
        }
        return TRUE;
    }

    void CDuiList::RemoveAll()
    {
        m_iCurSel = -1;
        m_iExpandedItem = -1;
        m_pList->RemoveAll();
    }

    UINT CDuiList::GetListType()
    {
        return LT_LIST;
    }

    ListInfo* CDuiList::GetListInfo()
    {
        return &m_ListInfo;
    }

    int CDuiList::GetCurSel() const
    {
        if(m_aSelItems.GetSize() <= 0)
        {
            return -1;
        }
        else
        {
            return (int)m_aSelItems.GetAt(0);
        }

        return -1;
    }

    void CDuiList::Scroll(int dx, int dy)
    {
        if(dx == 0 && dy == 0)
        {
            return;
        }
        SIZE sz = m_pList->GetScrollPos();
        m_pList->SetScrollPos(CDuiSize(sz.cx + dx, sz.cy + dy));
    }

    void CDuiList::EnsureVisible(int iIndex)
    {
        if(m_iCurSel < 0)
        {
            return;
        }
        RECT rcItem = m_pList->GetItemAt(iIndex)->GetPos();
        RECT rcList = m_pList->GetPos();
        RECT rcListInset = m_pList->GetInset();

        rcList.left += rcListInset.left;
        rcList.top += rcListInset.top;
        rcList.right -= rcListInset.right;
        rcList.bottom -= rcListInset.bottom;

        CDuiScrollBar* pHorizontalScrollBar = m_pList->GetHorizontalScrollBar();
        if(pHorizontalScrollBar && pHorizontalScrollBar->IsVisible())
        {
            rcList.bottom -= pHorizontalScrollBar->GetFixedHeight();
        }

        int iPos = m_pList->GetScrollPos().cy;
        if(rcItem.top >= rcList.top && rcItem.bottom < rcList.bottom)
        {
            return;
        }
        int dx = 0;
        if(rcItem.top < rcList.top)
        {
            dx = rcItem.top - rcList.top;
        }
        if(rcItem.bottom > rcList.bottom)
        {
            dx = rcItem.bottom - rcList.bottom;
        }
        Scroll(0, dx);
    }

    BOOL CDuiList::SelectItem(int iIndex, BOOL bTakeFocus /*= FALSE*/)
    {
        // 取消所有选择项
        UnSelectAllItems();
        // 判断是否合法列表项
        if(iIndex < 0)
        {
            return FALSE;
        }
        CDuiControl* pControl = GetItemAt(iIndex);
        if(pControl == NULL)
        {
            return FALSE;
        }
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem == NULL)
        {
            return FALSE;
        }
        if(!pListItem->Select(true))
        {
            return FALSE;
        }
        int iLastSel = m_iCurSel;
        m_iCurSel = iIndex;
        m_aSelItems.Add((LPVOID)iIndex);
        EnsureVisible(iIndex);
        if(bTakeFocus)
        {
            pControl->SetFocus();
        }
        if(GetManager() != NULL && iLastSel != m_iCurSel)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, iIndex);
        }

        return TRUE;
    }

    BOOL CDuiList::SelectMultiItem(int iIndex, BOOL bTakeFocus /*= FALSE*/)
    {
        if(!IsMultiSelect())
        {
            return SelectItem(iIndex, bTakeFocus);
        }

        if(iIndex < 0)
        {
            return FALSE;
        }
        CDuiControl* pControl = GetItemAt(iIndex);
        if(pControl == NULL)
        {
            return FALSE;
        }
        IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
        if(pListItem == NULL)
        {
            return FALSE;
        }
        if(m_aSelItems.Find((LPVOID)iIndex) >= 0)
        {
            return FALSE;
        }
        if(!pListItem->SelectMulti(TRUE))
        {
            return FALSE;
        }

        m_iCurSel = iIndex;
        m_aSelItems.Add((LPVOID)iIndex);
        EnsureVisible(iIndex);
        if(bTakeFocus)
        {
            pControl->SetFocus();
        }
        if(GetManager() != NULL)
        {
            GetManager()->SendNotify(this, DUI_MSGTYPE_ITEMSELECT, iIndex);
        }
        return TRUE;
    }

    BOOL CDuiList::UnSelectItem(int iIndex, BOOL bOthers /*= FALSE*/)
    {
        if(!IsMultiSelect())
        {
            return FALSE;
        }
        if(bOthers)
        {
            for(int i = m_aSelItems.GetSize() - 1; i >= 0; --i)
            {
                int iSelIndex = (int)m_aSelItems.GetAt(i);
                if(iSelIndex == iIndex)
                {
                    continue;
                }
                CDuiControl* pControl = GetItemAt(iSelIndex);
                if(pControl == NULL)
                {
                    continue;
                }
                if(!pControl->IsEnabled())
                {
                    continue;
                }
                IListItem* pSelListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
                if(pSelListItem == NULL)
                {
                    continue;
                }
                if(!pSelListItem->SelectMulti(FALSE))
                {
                    continue;
                }
                m_aSelItems.Remove(i);
            }
        }
        else
        {
            if(iIndex < 0)
            {
                return FALSE;
            }
            CDuiControl* pControl = GetItemAt(iIndex);
            if(pControl == NULL)
            {
                return FALSE;
            }
            if(!pControl->IsEnabled())
            {
                return FALSE;
            }
            IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem == NULL)
            {
                return FALSE;
            }
            int aIndex = m_aSelItems.Find((LPVOID)iIndex);
            if(aIndex < 0)
            {
                return FALSE;
            }
            if(!pListItem->SelectMulti(FALSE))
            {
                return FALSE;
            }
            if(m_iCurSel == iIndex)
            {
                m_iCurSel = -1;
            }
            m_aSelItems.Remove(aIndex);
        }
        return TRUE;
    }

    CDuiListHeader* CDuiList::GetHeader() const
    {
        return m_pHeader;
    }

    CDuiContainer* CDuiList::GetList() const
    {
        return m_pList;
    }

    IListCallback* CDuiList::GetTextCallback() const
    {
        return m_pCallback;
    }

    void CDuiList::SetTextCallback(IListCallback* pCallback)
    {
        m_pCallback = pCallback;
    }

    int CDuiList::GetExpandedItem() const
    {
        return m_iExpandedItem;
    }

    BOOL CDuiList::ExpandItem(int iIndex, BOOL bExpand /*= TRUE*/)
    {
        if(m_iExpandedItem >= 0 && !m_ListInfo.bMultiExpandable)
        {
            CDuiControl* pControl = GetItemAt(m_iExpandedItem);
            if(pControl != NULL)
            {
                IListItem* pItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
                if(pItem != NULL)
                {
                    pItem->Expand(FALSE);
                }
            }
            m_iExpandedItem = -1;
        }
        if(bExpand)
        {
            CDuiControl* pControl = GetItemAt(iIndex);
            if(pControl == NULL)
            {
                return FALSE;
            }
            if(!pControl->IsVisible())
            {
                return FALSE;
            }
            IListItem* pItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pItem == NULL)
            {
                return FALSE;
            }
            m_iExpandedItem = iIndex;
            if(!pItem->Expand(TRUE))
            {
                m_iExpandedItem = -1;
                return FALSE;
            }
        }
        NeedUpdate();
        return TRUE;
    }

    BOOL CDuiList::IsMultiSelect() const
    {
        return m_bMultiSel;
    }

    void CDuiList::SetMultiSelect(BOOL bMultiSel)
    {
        m_bMultiSel = bMultiSel;
        if(!bMultiSel)
        {
            UnSelectAllItems();
        }
    }

    void CDuiList::UnSelectAllItems()
    {
        for(int i = 0; i < m_aSelItems.GetSize(); ++i)
        {
            int iSelIndex = (int)m_aSelItems.GetAt(i);
            CDuiControl* pControl = GetItemAt(iSelIndex);
            if(pControl == NULL)
            {
                continue;
            }
            if(!pControl->IsEnabled())
            {
                continue;
            }
            IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem == NULL)
            {
                continue;
            }
            if(!pListItem->SelectMulti(FALSE))
            {
                continue;
            }
        }
        m_aSelItems.Empty();
        m_iCurSel = -1;
    }

    void CDuiList::SelectAllItems()
    {
        for(int i = 0; i < GetCount(); ++i)
        {
            CDuiControl* pControl = GetItemAt(i);
            if(pControl == NULL)
            {
                continue;
            }
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(!pControl->IsEnabled())
            {
                continue;
            }
            IListItem* pListItem = static_cast<IListItem*>(pControl->GetInterface(GET_CLASS_NAME(IListItem)));
            if(pListItem == NULL)
            {
                continue;
            }
            if(!pListItem->SelectMulti(TRUE))
            {
                continue;
            }

            m_aSelItems.Add((LPVOID)i);
            m_iCurSel = i;
        }
    }

    int CDuiList::GetSelectItemCount() const
    {
        return m_aSelItems.GetSize();
    }

    int CDuiList::GetNextSelItem(int nItem) const
    {
        if(m_aSelItems.GetSize() <= 0)
        {
            return -1;
        }

        if(nItem < 0)
        {
            return (int)m_aSelItems.GetAt(0);
        }
        int aIndex = m_aSelItems.Find((LPVOID)nItem);
        if(aIndex < 0)
        {
            return -1;
        }
        if(aIndex + 1 > m_aSelItems.GetSize() - 1)
        {
            return -1;
        }
        return (int)m_aSelItems.GetAt(aIndex + 1);
    }

    SIZE CDuiList::GetScrollPos() const
    {
        return m_pList->GetScrollPos();
    }

    SIZE CDuiList::GetScrollRange() const
    {
        return m_pList->GetScrollRange();
    }

    void CDuiList::SetScrollPos(SIZE szPos, BOOL bMsg /*= TRUE*/)
    {
        m_pList->SetScrollPos(szPos, bMsg);
    }

    void CDuiList::LineUp()
    {
        int cyLine = GetScrollStepSize();
        if(cyLine == 0)
        {
            cyLine = 8;
            if(GetManager())
            {
                cyLine = GetManager()->GetDefaultFontInfo()->tm.tmHeight + 8;
            }
        }

        SIZE sz = GetScrollPos();
        sz.cy -= cyLine;
        SetScrollPos(sz);
    }

    void CDuiList::LineDown()
    {
        int cyLine = GetScrollStepSize();
        if(cyLine == 0)
        {
            cyLine = 8;
            if(GetManager())
            {
                cyLine = GetManager()->GetDefaultFontInfo()->tm.tmHeight + 8;
            }
        }

        SIZE sz = GetScrollPos();
        sz.cy += cyLine;
        SetScrollPos(sz);
    }

    void CDuiList::LineLeft()
    {
        int nScrollStepSize = GetScrollStepSize();
        int cxLine = nScrollStepSize == 0 ? 8 : nScrollStepSize;

        SIZE sz = GetScrollPos();
        sz.cx -= cxLine;
        SetScrollPos(sz);
    }

    void CDuiList::LineRight()
    {
        int nScrollStepSize = GetScrollStepSize();
        int cxLine = nScrollStepSize == 0 ? 8 : nScrollStepSize;

        SIZE sz = GetScrollPos();
        sz.cx += cxLine;
        SetScrollPos(sz);
    }

    void CDuiList::PageUp()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().bottom - GetPos().top - GetInset().top - GetInset().bottom;
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            iOffset -= GetHorizontalScrollBar()->GetFixedHeight();
        }
        sz.cy -= iOffset;
        SetScrollPos(sz);
    }

    void CDuiList::PageDown()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().bottom - GetPos().top - GetInset().top - GetInset().bottom;
        if(GetHorizontalScrollBar() && GetHorizontalScrollBar()->IsVisible())
        {
            iOffset -= GetHorizontalScrollBar()->GetFixedHeight();
        }
        sz.cy += iOffset;
        SetScrollPos(sz);
    }

    void CDuiList::PageLeft()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().right - GetPos().left - GetInset().left - GetInset().right;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            iOffset -= GetVerticalScrollBar()->GetFixedWidth();
        }
        sz.cx -= iOffset;
        SetScrollPos(sz);
    }

    void CDuiList::PageRight()
    {
        SIZE sz = GetScrollPos();
        int iOffset = GetPos().right - GetPos().left - GetInset().left - GetInset().right;
        if(GetVerticalScrollBar() && GetVerticalScrollBar()->IsVisible())
        {
            iOffset -= GetVerticalScrollBar()->GetFixedWidth();
        }
        sz.cx += iOffset;
        SetScrollPos(sz);
    }

    void CDuiList::HomeUp()
    {
        SIZE sz = GetScrollPos();
        sz.cy = 0;
        SetScrollPos(sz);
    }

    void CDuiList::EndDown()
    {
        SIZE sz = GetScrollPos();
        sz.cy = GetScrollRange().cy;
        SetScrollPos(sz);
    }

    void CDuiList::HomeLeft()
    {
        SIZE sz = GetScrollPos();
        sz.cx = 0;
        SetScrollPos(sz);
    }

    void CDuiList::EndRight()
    {
        SIZE sz = GetScrollPos();
        sz.cx = GetScrollRange().cx;
        SetScrollPos(sz);
    }

    CDuiScrollBar* CDuiList::GetVerticalScrollBar() const
    {
        return m_pList->GetVerticalScrollBar();
    }

    CDuiScrollBar* CDuiList::GetHorizontalScrollBar() const
    {
        return m_pList->GetHorizontalScrollBar();
    }

    BOOL CDuiList::GetScrollSelect()
    {
        return m_bScrollSelect;
    }

    void CDuiList::SetScrollSelect(BOOL bScrollSelect)
    {
        m_bScrollSelect = bScrollSelect;
    }

    BOOL CDuiList::IsMultiExpanding()
    {
        return m_ListInfo.bMultiExpandable;
    }

    void CDuiList::SetMultiExpanding(BOOL bMultiExpandable)
    {
        m_ListInfo.bMultiExpandable = bMultiExpandable;
    }

    int CDuiList::GetItemFont() const
    {
        return m_ListInfo.nFont;
    }

    void CDuiList::SetItemFont(int index)
    {
        m_ListInfo.nFont = index;
        NeedUpdate();
    }

    UINT CDuiList::GetItemTextStyle()
    {
        return m_ListInfo.uTextStyle;
    }

    void CDuiList::SetItemTextStyle(UINT uStyle)
    {
        m_ListInfo.uTextStyle = uStyle;
        NeedUpdate();
    }

    RECT CDuiList::GetItemTextPadding()
    {
        return m_ListInfo.rcTextPadding;
    }

    void CDuiList::SetItemTextPadding(RECT rc)
    {
        m_ListInfo.rcTextPadding = rc;
        NeedUpdate();
    }

    DWORD CDuiList::GetItemTextColor()
    {
        return m_ListInfo.dwTextColor;
    }

    void CDuiList::SetItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiList::GetItemBkColor()
    {
        return m_ListInfo.dwBkColor;
    }

    void CDuiList::SetItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwBkColor = dwBkColor;
        Invalidate();
    }

    LPCTSTR CDuiList::GetItemBkImage()
    {
        return m_ListInfo.sBkImage;
    }

    void CDuiList::SetItemBkImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sBkImage = pStrImage;
        Invalidate();
    }

    BOOL CDuiList::IsAlternateBk()
    {
        return m_ListInfo.bAlternateBk;
    }

    void CDuiList::SetAlternateBk(BOOL bAlternateBk)
    {
        m_ListInfo.bAlternateBk = bAlternateBk;
        Invalidate();
    }

    DWORD CDuiList::GetSelectedItemTextColor()
    {
        return m_ListInfo.dwSelectedTextColor;
    }

    void CDuiList::SetSelectedItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwSelectedTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiList::GetSelectedItemBkColor()
    {
        return m_ListInfo.dwSelectedBkColor;
    }

    void CDuiList::SetSelectedItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwSelectedBkColor = dwBkColor;
        Invalidate();
    }

    LPCTSTR CDuiList::GetSelectedItemImage()
    {
        return m_ListInfo.sSelectedImage;
    }

    void CDuiList::SetSelectedItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sSelectedImage = pStrImage;
        Invalidate();
    }

    DWORD CDuiList::GetHotItemTextColor()
    {
        return m_ListInfo.dwHotTextColor;
    }

    void CDuiList::SetHotItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwHotTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiList::GetHotItemBkColor()
    {
        return m_ListInfo.dwHotBkColor;
    }

    void CDuiList::SetHotItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwHotBkColor = dwBkColor;
        Invalidate();
    }

    LPCTSTR CDuiList::GetHotItemImage()
    {
        return m_ListInfo.sHotImage;
    }

    void CDuiList::SetHotItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sHotImage = pStrImage;
        Invalidate();
    }

    DWORD CDuiList::GetDisabledItemTextColor()
    {
        return m_ListInfo.dwDisabledTextColor;
    }

    void CDuiList::SetDisabledItemTextColor(DWORD dwTextColor)
    {
        m_ListInfo.dwDisabledTextColor = dwTextColor;
        Invalidate();
    }

    DWORD CDuiList::GetDisabledItemBkColor()
    {
        return m_ListInfo.dwDisabledBkColor;
    }

    void CDuiList::SetDisabledItemBkColor(DWORD dwBkColor)
    {
        m_ListInfo.dwDisabledBkColor = dwBkColor;
        Invalidate();
    }

    LPCTSTR CDuiList::GetDisabledItemImage()
    {
        return m_ListInfo.sDisabledImage;
    }

    void CDuiList::SetDisabledItemImage(LPCTSTR pStrImage)
    {
        m_ListInfo.sDisabledImage = pStrImage;
        Invalidate();
    }

    DWORD CDuiList::GetItemLineColor()
    {
        return m_ListInfo.dwLineColor;
    }

    void CDuiList::SetItemLineColor(DWORD dwLineColor)
    {
        m_ListInfo.dwLineColor = dwLineColor;
        Invalidate();
    }

    BOOL CDuiList::IsItemShowRowLine()
    {
        return m_ListInfo.bShowRowLine;
    }

    void CDuiList::SetItemShowRowLine(BOOL bShowLine /*= FALSE*/)
    {
        m_ListInfo.bShowRowLine = bShowLine;
        Invalidate();
    }

    BOOL CDuiList::IsItemShowColumnLine()
    {
        return m_ListInfo.bShowColumnLine;
    }

    void CDuiList::SetItemShowColumnLine(BOOL bShowLine /*= FALSE*/)
    {
        m_ListInfo.bShowColumnLine = bShowLine;
        Invalidate();
    }

    BOOL CDuiList::IsItemShowHtml()
    {
        return m_ListInfo.bShowHtml;
    }

    void CDuiList::SetItemShowHtml(BOOL bShowHtml /*= TRUE*/)
    {
        if(m_ListInfo.bShowHtml == bShowHtml)
        {
            return;
        }

        m_ListInfo.bShowHtml = bShowHtml;
        NeedUpdate();
    }

    void CDuiList::EnableScrollBar(BOOL bEnableVertical /*= TRUE*/, BOOL bEnableHorizontal /*= FALSE*/)
    {
        m_pList->EnableScrollBar(bEnableVertical, bEnableHorizontal);
    }

    void CDuiList::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("header")) == 0)
        {
            GetHeader()->SetVisible(_tcsicmp(pstrValue, _T("hidden")) != 0);
        }
        else if(_tcsicmp(pstrName, _T("headerbkimage")) == 0)
        {
            GetHeader()->SetBkImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("scrollselect")) == 0)
        {
            SetScrollSelect(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("multiexpanding")) == 0)
        {
            SetMultiExpanding(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemfont")) == 0)
        {
            m_ListInfo.nFont = _ttoi(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemalign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("left")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_CENTER | DT_RIGHT);
                m_ListInfo.uTextStyle |= DT_LEFT;
            }
            if(_tcsstr(pstrValue, _T("center")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_RIGHT);
                m_ListInfo.uTextStyle |= DT_CENTER;
            }
            if(_tcsstr(pstrValue, _T("right")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_LEFT | DT_CENTER);
                m_ListInfo.uTextStyle |= DT_RIGHT;
            }
        }
        else if(_tcsicmp(pstrName, _T("itemvalign")) == 0)
        {
            if(_tcsstr(pstrValue, _T("top")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_VCENTER | DT_BOTTOM);
                m_ListInfo.uTextStyle |= DT_TOP;
            }
            if(_tcsstr(pstrValue, _T("vcenter")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_TOP | DT_BOTTOM);
                m_ListInfo.uTextStyle |= DT_VCENTER;
            }
            if(_tcsstr(pstrValue, _T("bottom")) != NULL)
            {
                m_ListInfo.uTextStyle &= ~(DT_TOP | DT_VCENTER);
                m_ListInfo.uTextStyle |= DT_BOTTOM;
            }
        }
        else if(_tcsicmp(pstrName, _T("itemendellipsis")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                m_ListInfo.uTextStyle |= DT_END_ELLIPSIS;
            }
            else
            {
                m_ListInfo.uTextStyle &= ~DT_END_ELLIPSIS;
            }
        }
        else if(_tcsicmp(pstrName, _T("itemtextpadding")) == 0)
        {
            RECT rcTextPadding = { 0 };
            LPTSTR pstr = NULL;
            rcTextPadding.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcTextPadding.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetItemTextPadding(rcTextPadding);
        }
        else if(_tcsicmp(pstrName, _T("itemtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itembkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itembkimage")) == 0)
        {
            SetItemBkImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemaltbk")) == 0)
        {
            SetAlternateBk(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelectedItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemselectedimage")) == 0)
        {
            SetSelectedItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemhotbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetHotItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemhotimage")) == 0)
        {
            SetHotItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledbkcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetDisabledItemBkColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemdisabledimage")) == 0)
        {
            SetDisabledItemImage(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemlinecolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemLineColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("itemshowrowline")) == 0)
        {
            SetItemShowRowLine(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemshowcolumnline")) == 0)
        {
            SetItemShowColumnLine(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemshowhtml")) == 0)
        {
            SetItemShowHtml(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcscmp(pstrName, _T("multiselect")) == 0)
        {
            SetMultiSelect(_tcscmp(pstrValue, _T("true")) == 0);
        }
        else
        {
            CDuiVerticalLayout::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiList::SetPos(RECT rc, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiVerticalLayout::SetPos(rc, bNeedInvalidate);

        if(m_pHeader == NULL)
        {
            return;
        }
        // Determine general list information and the size of header columns
        m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);
        // The header/columns may or may not be visible at runtime. In either case
        // we should determine the correct dimensions...

        if(!m_pHeader->IsVisible())
        {
            for(int it = 0; it < m_pHeader->GetCount(); it++)
            {
                static_cast<CDuiControl*>(m_pHeader->GetItemAt(it))->SetInternVisible(TRUE);
            }
            m_pHeader->SetPos(CDuiRect(rc.left, 0, rc.right, 0), bNeedInvalidate);
        }

        for(int i = 0; i < m_ListInfo.nColumns; i++)
        {
            CDuiControl* pControl = static_cast<CDuiControl*>(m_pHeader->GetItemAt(i));
            if(!pControl->IsVisible())
            {
                continue;
            }
            if(pControl->IsFloat())
            {
                continue;
            }
            RECT rcPos = pControl->GetPos();
            m_ListInfo.rcColumn[i] = pControl->GetPos();
        }
        if(!m_pHeader->IsVisible())
        {
            for(int it = 0; it < m_pHeader->GetCount(); it++)
            {
                static_cast<CDuiControl*>(m_pHeader->GetItemAt(it))->SetInternVisible(FALSE);
            }
        }
        m_pList->SetPos(m_pList->GetPos(), bNeedInvalidate);
    }

    void CDuiList::Move(SIZE szOffset, BOOL bNeedInvalidate /*= TRUE*/)
    {
        CDuiVerticalLayout::Move(szOffset, bNeedInvalidate);
        if(!m_pHeader->IsVisible())
        {
            m_pHeader->Move(szOffset, FALSE);
        }
    }

    void CDuiList::DoEvent(TEventUI& event)
    {
        if(!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
        {
            if(GetParent() != NULL)
            {
                GetParent()->DoEvent(event);
            }
            else
            {
                CDuiVerticalLayout::DoEvent(event);
            }
            return;
        }

        if(event.Type == UIEVENT_SETFOCUS)
        {
            SetFocused(TRUE);
            return;
        }
        if(event.Type == UIEVENT_KILLFOCUS)
        {
            SetFocused(FALSE);
            return;
        }
        if(event.Type == UIEVENT_KEYDOWN)
        {
            switch(event.chKey)
            {
                case VK_UP:
                {
                    if(m_aSelItems.GetSize() > 0)
                    {
                        int index = GetMinSelItemIndex() - 1;
                        UnSelectAllItems();
                        index > 0 ? SelectItem(index, TRUE) : SelectItem(0, TRUE);
                    }
                }
                return;
                case VK_DOWN:
                {
                    if(m_aSelItems.GetSize() > 0)
                    {
                        int index = GetMaxSelItemIndex() + 1;
                        UnSelectAllItems();
                        index + 1 > m_pList->GetCount() ? SelectItem(GetCount() - 1, true) : SelectItem(index, true);
                    }
                }
                return;
                case VK_PRIOR:
                    PageUp();
                    return;
                case VK_NEXT:
                    PageDown();
                    return;
                case VK_HOME:
                    SelectItem(FindSelectable(0, false), TRUE);
                    return;
                case VK_END:
                    SelectItem(FindSelectable(GetCount() - 1, TRUE), TRUE);
                    return;
                case VK_RETURN:
                    if(m_iCurSel != -1)
                    {
                        GetItemAt(m_iCurSel)->Activate();
                    }
                    return;
                case 0x41:// Ctrl+A
                {
                    if(IsMultiSelect() && (GetKeyState(VK_CONTROL) & 0x8000))
                    {
                        SelectAllItems();
                    }
                    return;
                }
            }
        }
        if(event.Type == UIEVENT_SCROLLWHEEL)
        {
            switch(LOWORD(event.wParam))
            {
                case SB_LINEUP:
                    if(m_bScrollSelect && !IsMultiSelect())
                    {
                        SelectItem(FindSelectable(m_iCurSel - 1, FALSE), TRUE);
                    }
                    else
                    {
                        LineUp();
                    }
                    return;
                case SB_LINEDOWN:
                    if(m_bScrollSelect && !IsMultiSelect())
                    {
                        SelectItem(FindSelectable(m_iCurSel + 1, TRUE), TRUE);
                    }
                    else
                    {
                        LineDown();
                    }
                    return;
            }
        }
        CDuiVerticalLayout::DoEvent(event);
    }

    int CDuiList::GetMinSelItemIndex()
    {
        if(m_aSelItems.GetSize() <= 0)
        {
            return -1;
        }
        int min = (int)m_aSelItems.GetAt(0);
        int index;
        for(int i = 0; i < m_aSelItems.GetSize(); ++i)
        {
            index = (int)m_aSelItems.GetAt(i);
            if(min > index)
            {
                min = index;
            }
        }
        return min;
    }

    int CDuiList::GetMaxSelItemIndex()
    {
        if(m_aSelItems.GetSize() <= 0)
        {
            return -1;
        }
        int max = (int)m_aSelItems.GetAt(0);
        int index;
        for(int i = 0; i < m_aSelItems.GetSize(); ++i)
        {
            index = (int)m_aSelItems.GetAt(i);
            if(max < index)
            {
                max = index;
            }
        }
        return max;
    }

}