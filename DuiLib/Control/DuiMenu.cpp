#include "StdAfx.h"
#include "DuiMenu.h"

namespace DuiLib
{

    ///////////////////////////////////////////////////////////////////////////

    MenuObserverImpl::MenuObserverImpl()
        : m_pPaintManager(NULL)
        , m_pMenuCheckInfo(NULL)
    {

    }

    MenuObserverImpl::~MenuObserverImpl()
    {
    }

    void MenuObserverImpl::AddReceiver(MenuMenuReceiverImplBase* receiver)
    {
        if(receiver == NULL)
        {
            return;
        }
        m_vtReceivers.push_back(receiver);
        receiver->AddObserver(this);
    }

    void MenuObserverImpl::RemoveReceiver(MenuMenuReceiverImplBase* receiver)
    {
        if(receiver == NULL)
        {
            return;
        }
        for(vector<MenuMenuReceiverImplBase*>::iterator it = m_vtReceivers.begin(); it != m_vtReceivers.end(); ++it)
        {
            if(*it == receiver)
            {
                m_vtReceivers.erase(it);
                break;
            }
        }
    }

    BOOL MenuObserverImpl::RBroadcast(ContextMenuParam param)
    {
        for(vector<MenuMenuReceiverImplBase*>::iterator it = m_vtReceivers.begin(); it != m_vtReceivers.end(); ++it)
        {
            (*it)->Receive(param);
        }

        return BOOL();
    }

    CDuiPaintManager* MenuObserverImpl::GetManager() const
    {
        return m_pPaintManager;
    }

    void MenuObserverImpl::SetManger(CDuiPaintManager* pManager)
    {
        if(pManager != NULL)
        {
            m_pPaintManager = pManager;
        }
    }

    CStdStringPtrMap* MenuObserverImpl::GetMenuCheckInfo() const
    {
        return m_pMenuCheckInfo;
    }

    void MenuObserverImpl::SetMenuCheckInfo(CStdStringPtrMap* pInfo)
    {
        if(pInfo != NULL)
        {
            m_pMenuCheckInfo = pInfo;
        }
        else
        {
            m_pMenuCheckInfo = NULL;
        }
    }

    MenuObserverImpl::Iterator::Iterator(MenuObserverImpl& table)
        : index(0)
        , _tbl(table)
        , ptr(NULL)
    {

    }

    MenuObserverImpl::Iterator::Iterator(const Iterator& v)
        : index(v.index)
        , _tbl(v._tbl)
        , ptr(v.ptr)
    {

    }

    MenuMenuReceiverImplBase* MenuObserverImpl::Iterator::next()
    {
        if(index >= _tbl.m_vtReceivers.size())
        {
            return NULL;
        }

        for(; index < _tbl.m_vtReceivers.size();)
        {
            ptr = (_tbl.m_vtReceivers)[ index++ ];
            if(ptr)
            {
                return ptr;
            }
        }
        return NULL;
    }

    /////////////////////////////////////////////////////////////////////////

    MenuReceiverImpl::MenuReceiverImpl()
    {

    }


    MenuReceiverImpl::~MenuReceiverImpl()
    {
    }

    void MenuReceiverImpl::AddObserver(MenuMenuObserverImplBase* observer)
    {
        m_vtObservers.push_back(observer);
    }

    void MenuReceiverImpl::RemoveObserver()
    {
        for(vector<MenuMenuObserverImplBase*>::iterator it = m_vtObservers.begin(); it != m_vtObservers.end(); ++it)
        {
            (*it)->RemoveReceiver(this);
        }
    }

    BOOL MenuReceiverImpl::Receive(ContextMenuParam param)
    {
        return BOOL();
    }

    /////////////////////////////////////////////////////

    IMPLEMENT_DUICONTROL(CDuiMenuElement)

    CDuiMenuElement::CDuiMenuElement()
        : m_bCheckItem(FALSE)
        , m_bDrawLine(FALSE)
        , m_bShowExplandIcon(FALSE)
        , m_dwLineColor(DEFAULT_LINE_COLOR)
        , m_pMenuWnd(NULL)
    {
        SetFixedXY(CDuiSize(ITEM_DEFAULT_WIDTH, ITEM_DEFAULT_HEIGHT));
        m_szIconSize.cy = ITEM_DEFAULT_ICON_SIZE;
        m_szIconSize.cx = ITEM_DEFAULT_ICON_SIZE;

        m_rcLinePadding.top = m_rcLinePadding.bottom = 0;
        m_rcLinePadding.left = DEFAULT_LINE_LEFT_INSET;
        m_rcLinePadding.right = DEFAULT_LINE_RIGHT_INSET;
    }

    CDuiMenuElement::~CDuiMenuElement()
    {

    }

    CDuiString CDuiMenuElement::GetClass() const
    {
        return DUI_CTR_MENUELEMENT;
    }

    LPVOID CDuiMenuElement::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_MENUELEMENT) == 0)
        {
            return static_cast<CDuiMenuElement*>(this);
        }
        return CDuiListContainerElement::GetInterface(pstrName);
    }

    void CDuiMenuElement::EmptyMenuWnd()
    {
        m_pMenuWnd = NULL;
    }

    void CDuiMenuElement::SetLineType()
    {
        m_bDrawLine = TRUE;
        if(GetFixedHeight() == 0 || GetFixedHeight() == ITEM_DEFAULT_HEIGHT)
        {
            SetFixedHeight(DEFAULT_LINE_HEIGHT);
        }

        SetMouseChildEnabled(FALSE);
        SetMouseEnabled(FALSE);
        SetEnabled(FALSE);
    }

    DWORD CDuiMenuElement::GetLineColor() const
    {
        return m_dwLineColor;
    }

    void CDuiMenuElement::SetLineColor(DWORD color)
    {
        if(m_dwLineColor == color)
        {
            return;
        }
        m_dwLineColor = color;
        Invalidate();
    }


    CDuiRect CDuiMenuElement::GetLinePadding() const
    {
        return m_rcLinePadding;
    }

    void CDuiMenuElement::SetLinePadding(CDuiRect rcInset)
    {
        if(m_rcLinePadding == rcInset)
        {
        }
        m_rcLinePadding = rcInset;
        Invalidate();
    }

    CDuiString CDuiMenuElement::GetIcon() const
    {
        return m_strIcon;
    }

    void CDuiMenuElement::SetIcon(LPCTSTR strIcon)
    {
        m_strIcon = strIcon;
    }

    CDuiSize CDuiMenuElement::GetIconSize()
    {
        return m_szIconSize;
    }

    void CDuiMenuElement::SetIconSize(CDuiSize szIcon)
    {
        if(m_szIconSize == szIcon)
        {
            return;
        }
        m_szIconSize = szIcon;
        Invalidate();
    }

    MenuItemInfo* CDuiMenuElement::GetItemInfo(LPCTSTR pstrName) const
    {
        if(pstrName == NULL || lstrlen(pstrName) <= 0)
        {
            return NULL;
        }

        CStdStringPtrMap* mCheckInfos = CDuiMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
        if(mCheckInfos != NULL)
        {
            MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
            if(pItemInfo != NULL)
            {
                return pItemInfo;
            }
        }

        return NULL;
    }

    MenuItemInfo* CDuiMenuElement::SetItemInfo(LPCTSTR pstrName, BOOL bChecked)
    {
        if(pstrName == NULL || lstrlen(pstrName) <= 0)
        {
            return NULL;
        }

        CStdStringPtrMap* mCheckInfos = CDuiMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
        if(mCheckInfos != NULL)
        {
            MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
            if(pItemInfo == NULL)
            {
                pItemInfo = new MenuItemInfo;
                pItemInfo->strName = pstrName;
                pItemInfo->bChecked = bChecked;
                mCheckInfos->Insert(pstrName, pItemInfo);
            }
            else
            {
                pItemInfo->bChecked = bChecked;
            }

            return pItemInfo;
        }
        return NULL;
    }

    BOOL CDuiMenuElement::GetChecked() const
    {
        LPCTSTR pstrName = GetName();
        if(pstrName == NULL || lstrlen(pstrName) <= 0)
        {
            return FALSE;
        }

        CStdStringPtrMap* mCheckInfos = CDuiMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
        if(mCheckInfos != NULL)
        {
            MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
            if(pItemInfo != NULL)
            {
                return pItemInfo->bChecked;
            }
        }
        return FALSE;
    }

    void CDuiMenuElement::SetChecked(BOOL bCheck /*= TRUE*/)
    {
        SetItemInfo(GetName(), bCheck);
    }

    BOOL CDuiMenuElement::GetCheckItem() const
    {
        return m_bCheckItem;
    }

    void CDuiMenuElement::SetCheckItem(BOOL bCheckItem /*= FALSE*/)
    {
        m_bCheckItem = bCheckItem;
    }

    BOOL CDuiMenuElement::GetShowExplandIcon() const
    {
        return m_bShowExplandIcon;
    }

    void CDuiMenuElement::SetShowExplandIcon(BOOL bShow)
    {
        m_bShowExplandIcon = bShow;
    }

    void CDuiMenuElement::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("icon")) == 0)
        {
            SetIcon(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("iconsize")) == 0)
        {
            LPTSTR pstr = NULL;
            LONG cx = 0, cy = 0;
            cx = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            cy = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetIconSize(CDuiSize(cx, cy));
        }
        else if(_tcsicmp(pstrName, _T("checkitem")) == 0)
        {
            SetCheckItem(_tcsicmp(pstrValue, _T("true")) == 0 ? true : false);
        }
        else if(_tcsicmp(pstrName, _T("ischeck")) == 0)
        {
            CStdStringPtrMap* mCheckInfos = CDuiMenuWnd::GetGlobalContextMenuObserver().GetMenuCheckInfo();
            if(mCheckInfos != NULL)
            {
                BOOL bFind = FALSE;
                for(int i = 0; i < mCheckInfos->GetSize(); i++)
                {
                    MenuItemInfo* itemInfo = (MenuItemInfo*)mCheckInfos->GetAt(i);
                    if(lstrcmpi(itemInfo->strName, GetName()) == 0)
                    {
                        bFind = TRUE;
                        break;
                    }
                }
                if(!bFind)
                {
                    SetChecked(_tcsicmp(pstrValue, _T("true")) == 0 ? TRUE : FALSE);
                }
            }
        }
        else if(_tcsicmp(pstrName, _T("linetype")) == 0)
        {
            if(_tcsicmp(pstrValue, _T("true")) == 0)
            {
                SetLineType();
            }
        }
        else if(_tcsicmp(pstrName, _T("expland")) == 0)
        {
            SetShowExplandIcon(_tcsicmp(pstrValue, _T("true")) == 0 ? TRUE : FALSE);
        }
        else if(_tcsicmp(pstrName, _T("linecolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            SetLineColor(_tcstoul(pstrValue, &pstr, 16));
        }
        else if(_tcsicmp(pstrName, _T("linepadding")) == 0)
        {
            RECT rcInset = { 0 };
            LPTSTR pstr = NULL;
            rcInset.left = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
            rcInset.top = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcInset.right = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            rcInset.bottom = _tcstol(pstr + 1, &pstr, 10);
            ASSERT(pstr);
            SetLinePadding(rcInset);
        }
        else if(_tcsicmp(pstrName, _T("height")) == 0)
        {
            SetFixedHeight(_ttoi(pstrValue));
        }
        else
        {
            CDuiListContainerElement::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiMenuElement::DrawItemText(HDC hDC, const RECT& rcItem)
    {
        CDuiString sText = GetText();
        if(sText.IsEmpty())
        {
            return;
        }

        if(GetOwner() == NULL)
        {
            return;
        }
        ListInfo* pInfo = GetOwner()->GetListInfo();
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
        int nLinks = 0;
        RECT rcText = rcItem;
        RECT rcTextPadding = pInfo->rcTextPadding;
        GetManager()->GetDPIObj()->Scale(&rcTextPadding);
        rcText.left += rcTextPadding.left;
        rcText.right -= rcTextPadding.right;
        rcText.top += rcTextPadding.top;
        rcText.bottom -= rcTextPadding.bottom;

        if(pInfo->bShowHtml)
            CRenderEngine::DrawHtmlText(hDC, GetManager(), rcText, sText, iTextColor, \
                                        NULL, NULL, nLinks, DT_SINGLELINE | pInfo->uTextStyle);
        else
            CRenderEngine::DrawText(hDC, GetManager(), rcText, sText, iTextColor, \
                                    pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
    }

    void CDuiMenuElement::DrawItemIcon(HDC hDC, const RECT& rcItem)
    {
        if(!m_strIcon.IsEmpty() && !(m_bCheckItem && !GetChecked()))
        {
            SIZE cxyFixed = GetFixedXY();

            SIZE szIconSize = m_szIconSize;
            szIconSize.cx = GetManager()->GetDPIObj()->Scale(szIconSize.cx);
            szIconSize.cy = GetManager()->GetDPIObj()->Scale(szIconSize.cy);
            ListInfo* pInfo = GetOwner()->GetListInfo();
            RECT rcTextPadding = pInfo->rcTextPadding;
            GetManager()->GetDPIObj()->Scale(&rcTextPadding);
            int padding = (rcTextPadding.left - szIconSize.cx) / 2;
            RECT rcDest =
            {
                padding,
                (cxyFixed.cy - szIconSize.cy) / 2,
                padding + m_szIconSize.cx,
                (cxyFixed.cy - szIconSize.cy) / 2 + szIconSize.cy
            };
            GetManager()->GetDPIObj()->ScaleBack(&rcDest);
            CDuiString pStrImage;
            pStrImage.Format(_T("dest='%d,%d,%d,%d'"), rcDest.left, rcDest.top, rcDest.right, rcDest.bottom);
            DrawImage(hDC, m_strIcon, pStrImage);
        }
    }

    void CDuiMenuElement::DrawItemExpland(HDC hDC, const RECT& rcItem)
    {
        if(m_bShowExplandIcon)
        {
            CDuiString strExplandIcon;
            strExplandIcon = GetManager()->GetDefaultAttributeList(_T("ExplandIcon"));
            if(strExplandIcon.IsEmpty())
            {
                return;
            }
            SIZE cxyFixed = GetFixedXY();
            int padding = GetManager()->GetDPIObj()->Scale(ITEM_DEFAULT_EXPLAND_ICON_WIDTH) / 3;
            const TDrawInfo* pDrawInfo = GetManager()->GetDrawInfo((LPCTSTR)strExplandIcon, NULL);
            const TImageInfo* pImageInfo = GetManager()->GetImageEx(pDrawInfo->sImageName, NULL, 0);
            if(!pImageInfo)
            {
                return;
            }
            RECT rcDest =
            {
                cxyFixed.cx - pImageInfo->nX - padding,
                (cxyFixed.cy - pImageInfo->nY) / 2,
                cxyFixed.cx - pImageInfo->nX - padding + pImageInfo->nX,
                (cxyFixed.cy - pImageInfo->nY) / 2 + pImageInfo->nY
            };
            GetManager()->GetDPIObj()->ScaleBack(&rcDest);
            CDuiString pStrImage;
            pStrImage.Format(_T("dest='%d,%d,%d,%d'"),
                             rcDest.left,
                             rcDest.top,
                             rcDest.right,
                             rcDest.bottom);

            DrawImage(hDC, strExplandIcon, pStrImage);
        }
    }

    void CDuiMenuElement::DoPaint(HDC hDC, const RECT& rcPaint)
    {
        CDuiControl::DoPaint(hDC, rcPaint);
        SIZE cxyFixed = GetFixedXY();
        RECT rcLinePadding = m_rcLinePadding;
        GetManager()->GetDPIObj()->Scale(&rcLinePadding);

        if(!::IntersectRect(&GetPaintRect(), &rcPaint, &GetPos()))
        {
            return;
        }

        if(m_bDrawLine)
        {
            RECT rcLine = { GetPos().left +  rcLinePadding.left, GetPos().top + cxyFixed.cy / 2, GetPos().right - rcLinePadding.right, GetPos().top + cxyFixed.cy / 2 };
            CRenderEngine::DrawLine(hDC, rcLine, 1, m_dwLineColor);
        }
        else
        {
            DrawItemBk(hDC, GetPos());
            DrawItemText(hDC, GetPos());
            DrawItemIcon(hDC, GetPos());
            DrawItemExpland(hDC, GetPos());
            for(int i = 0; i < GetCount(); ++i)
            {
                if(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) == NULL)
                {
                    GetItemAt(i)->DoPaint(hDC, rcPaint);
                }
            }
        }
    }

    SIZE CDuiMenuElement::EstimateSize(SIZE szAvailable)
    {
        SIZE cxyFixed = GetFixedXY();
        SIZE cXY = {0};
        for(int it = 0; it < GetCount(); it++)
        {
            CDuiControl* pControl = GetItemAt(it);
            if(!pControl->IsVisible())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            cXY.cy += sz.cy;
            if(cXY.cx < sz.cx)
            {
                cXY.cx = sz.cx;
            }
        }
        if(cXY.cy == 0)
        {
            ListInfo* pInfo = GetOwner()->GetListInfo();

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
            CDuiString sText = GetText();

            RECT rcText = { 0, 0, MAX(szAvailable.cx, cxyFixed.cx), 9999 };
            RECT rcTextPadding = pInfo->rcTextPadding;
            GetManager()->GetDPIObj()->Scale(&rcTextPadding);
            rcText.left += rcTextPadding.left;
            rcText.right -= rcTextPadding.right;
            if(pInfo->bShowHtml)
            {
                int nLinks = 0;
                CRenderEngine::DrawHtmlText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, iTextColor, NULL, NULL, nLinks, DT_CALCRECT | pInfo->uTextStyle);
            }
            else
            {
                CRenderEngine::DrawText(GetManager()->GetPaintDC(), GetManager(), rcText, sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle);
            }
            cXY.cx = rcText.right - rcText.left + rcTextPadding.left + rcTextPadding.right ;
            cXY.cy = rcText.bottom - rcText.top + rcTextPadding.top + rcTextPadding.bottom;
        }

        if(cxyFixed.cy != 0)
        {
            cXY.cy = cxyFixed.cy;
        }
        if(cXY.cx < cxyFixed.cx)
        {
            cXY.cx =  cxyFixed.cx;
        }
        SetFixedXY(CDuiSize(MulDiv(cXY.cx, 100, GetManager()->GetDPIObj()->GetScale()), MulDiv(cXY.cy, 100, GetManager()->GetDPIObj()->GetScale())));
        return cXY;
    }

    void CDuiMenuElement::CreateMenuWnd()
    {
        if(m_pMenuWnd)
        {
            return;
        }

        m_pMenuWnd = new CDuiMenuWnd();
        ASSERT(m_pMenuWnd);

        ContextMenuParam param;
        param.hWnd = GetManager()->GetPaintWindow();
        param.wParam = 2;
        CDuiMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);

        m_pMenuWnd->Init(static_cast<CDuiMenuElement*>(this), _T(""), CDuiPoint(), NULL);
    }

    void CDuiMenuElement::DoEvent(TEventUI& event)
    {
        if(event.Type == UIEVENT_MOUSEENTER)
        {
            CDuiListContainerElement::DoEvent(event);
            if(m_pMenuWnd)
            {
                return;
            }
            BOOL hasSubMenu = FALSE;
            for(int i = 0; i < GetCount(); ++i)
            {
                if(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
                {
                    (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(TRUE);
                    (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(TRUE);

                    hasSubMenu = TRUE;
                }
            }
            if(hasSubMenu)
            {
                GetOwner()->SelectItem(GetIndex(), TRUE);
                CreateMenuWnd();
            }
            else
            {
                ContextMenuParam param;
                param.hWnd = GetManager()->GetPaintWindow();
                param.wParam = 2;
                CDuiMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
                GetOwner()->SelectItem(GetIndex(), TRUE);
            }
            return;
        }

        if(event.Type == UIEVENT_MOUSELEAVE)
        {
            BOOL hasSubMenu = FALSE;
            for(int i = 0; i < GetCount(); ++i)
            {
                if(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
                {
                    hasSubMenu = TRUE;
                }
            }

            if(!hasSubMenu)
            {
                GetOwner()->SelectItem(-1, TRUE);
            }
        }

        if(event.Type == UIEVENT_BUTTONUP)
        {
            if(IsEnabled())
            {
                CDuiListContainerElement::DoEvent(event);

                if(m_pMenuWnd)
                {
                    return;
                }

                BOOL hasSubMenu = FALSE;
                for(int i = 0; i < GetCount(); ++i)
                {
                    if(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
                    {
                        (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(TRUE);
                        (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(TRUE);

                        hasSubMenu = TRUE;
                    }
                }
                if(hasSubMenu)
                {
                    CreateMenuWnd();
                }
                else
                {
                    SetChecked(!GetChecked());
                    CDuiMenu* pMenu = static_cast<CDuiMenu*>(GetManager()->GetRoot());
                    BOOL isClosing = pMenu->GetMenuWnd()->IsClosing();
                    if(IsWindow(GetManager()->GetPaintWindow()) && !isClosing)
                    {
                        if(CDuiMenuWnd::GetGlobalContextMenuObserver().GetManager() != NULL)
                        {
                            MenuCmd* pMenuCmd = new MenuCmd();
                            pMenuCmd->strName = GetName();
                            pMenuCmd->strUserData = GetUserData();
                            pMenuCmd->strText = GetText();
                            pMenuCmd->bChecked = GetChecked();
                            SendMessage(CDuiMenuWnd::GetGlobalContextMenuObserver().GetManager()->GetPaintWindow(), DUIMSG_MENU_CLICK, (WPARAM)pMenuCmd, (LPARAM)this);
                            DUI_FREE_POINT(pMenuCmd);
                        }
                    }
                    ContextMenuParam param;
                    param.hWnd = GetManager()->GetPaintWindow();
                    param.wParam = 1;
                    CDuiMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
                }
            }
            return;
        }

        if(event.Type == UIEVENT_KEYDOWN && event.chKey == VK_RIGHT)
        {
            if(m_pMenuWnd)
            {
                return;
            }
            BOOL hasSubMenu = FALSE;
            for(int i = 0; i < GetCount(); ++i)
            {
                if(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
                {
                    (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetVisible(TRUE);
                    (static_cast<CDuiMenuElement*>(GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(TRUE);
                    hasSubMenu = TRUE;
                }
            }
            if(hasSubMenu)
            {
                GetOwner()->SelectItem(GetIndex(), TRUE);
                CreateMenuWnd();
            }
            else
            {
                ContextMenuParam param;
                param.hWnd = GetManager()->GetPaintWindow();
                param.wParam = 2;
                CDuiMenuWnd::GetGlobalContextMenuObserver().RBroadcast(param);
                GetOwner()->SelectItem(GetIndex(), TRUE);
            }

            return;
        }
        CDuiListContainerElement::DoEvent(event);
    }

    ///////////////////////////////////////////////
    CDuiMenuWnd::CDuiMenuWnd()
        : bIsClosing(FALSE)
        , m_dwAlignment(eMenuAlignment_Left | eMenuAlignment_Top)
        , m_pLayout(NULL)
        , m_pOwner(NULL)
        , m_xml(_T(""))
    {

    }

    CDuiMenuWnd::~CDuiMenuWnd()
    {

    }

    LPCTSTR CDuiMenuWnd::GetWindowClassName() const
    {
        return GET_CLASS_NAME(CDuiMenuWnd);
    }

    void CDuiMenuWnd::OnFinalMessage(HWND hWnd)
    {
        RemoveObserver();
        if(m_pOwner != NULL)
        {
            for(int i = 0; i < m_pOwner->GetCount(); i++)
            {
                if(static_cast<CDuiMenuElement*>(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)) != NULL)
                {
                    (static_cast<CDuiMenuElement*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pOwner->GetParent());
                    (static_cast<CDuiMenuElement*>(m_pOwner->GetItemAt(i)))->SetVisible(FALSE);
                    (static_cast<CDuiMenuElement*>(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(FALSE);
                }
            }
            m_pOwner->EmptyMenuWnd();
            UINT uButtonStatus = m_pOwner->GetButtonState();
            uButtonStatus &= ~ UISTATE_PUSHED;
            m_pOwner->SetButtonState(uButtonStatus);
            m_pOwner->Invalidate();

            // 内部创建的内部删除
            CDuiWnd::OnFinalMessage(hWnd);
        }
    }

    void CDuiMenuWnd::Notify(TNotifyUI& msg)
    {
        if(GetGlobalContextMenuObserver().GetManager() != NULL)
        {
            if(msg.sType == DUI_MSGTYPE_CLICK || msg.sType == DUI_MSGTYPE_VALUECHANGED)
            {
                GetGlobalContextMenuObserver().GetManager()->SendNotify(msg, FALSE);
            }
        }
    }

    CDuiControl* CDuiMenuWnd::CreateControl(LPCTSTR pstrClassName)
    {
        if(_tcsicmp(pstrClassName, DUI_CTR_MENU) == 0)
        {
            return new CDuiMenu();
        }
        else if(_tcsicmp(pstrClassName, DUI_CTR_MENUELEMENT) == 0)
        {
            return new CDuiMenuElement();
        }
        return NULL;
    }

    void CDuiMenuWnd::Close(UINT nRet /*= IDOK*/)
    {
        PostMessage(WM_CLOSE, (WPARAM)nRet, 0L);
        bIsClosing = TRUE;
    }

    BOOL CDuiMenuWnd::Receive(ContextMenuParam param)
    {
        switch(param.wParam)
        {
            case 1:
                Close();
                break;
            case 2:
            {
                HWND hParent = GetParent(GetHWND());
                while(hParent != NULL)
                {
                    if(hParent == param.hWnd)
                    {
                        Close();
                        break;
                    }
                    hParent = GetParent(hParent);
                }
            }
            break;
            default:
                break;
        }

        return TRUE;
    }

    void CDuiMenuWnd::Init(CDuiMenuElement* pOwner, STRINGorID xml, POINT point, CDuiPaintManager* pMainPaintManager, CStdStringPtrMap* pMenuCheckInfo /*= NULL*/, DWORD dwAlignment /*= eMenuAlignment_Left | eMenuAlignment_Top*/)
    {
        m_BasedPoint = point;
        m_pOwner = pOwner;
        m_pLayout = NULL;
        m_xml = xml;
        m_dwAlignment = dwAlignment;

        // 如果是一级菜单的创建
        if(pOwner == NULL)
        {
            ASSERT(pMainPaintManager != NULL);
            GetGlobalContextMenuObserver().SetManger(pMainPaintManager);
            if(pMenuCheckInfo != NULL)
            {
                GetGlobalContextMenuObserver().SetMenuCheckInfo(pMenuCheckInfo);
            }
        }

        GetGlobalContextMenuObserver().AddReceiver(this);

        Create((m_pOwner == NULL) ? pMainPaintManager->GetPaintWindow() : m_pOwner->GetManager()->GetPaintWindow(), NULL, WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());

        // HACK: Don't deselect the parent's caption
        HWND hWndParent = GetHWND();
        while(::GetParent(hWndParent) != NULL)
        {
            hWndParent = ::GetParent(hWndParent);
        }

        ::ShowWindow(GetHWND(), SW_SHOW);
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
    }

    CDuiMenu* CDuiMenuWnd::GetMenuRoot()
    {
        return static_cast<CDuiMenu*>(m_PaintManager.GetRoot());
    }

    BOOL CDuiMenuWnd::IsClosing() const
    {
        return bIsClosing;
    }

    void CDuiMenuWnd::ResizeMenu()
    {
        CDuiControl* pRoot = m_PaintManager.GetRoot();

#if defined(WIN32) && !defined(UNDER_CE)
        MONITORINFO oMonitor = {0};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
#else
        CDuiRect rcWork;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
        SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
        szAvailable = pRoot->EstimateSize(szAvailable);
        m_PaintManager.SetInitSize(szAvailable.cx, szAvailable.cy);

        //必须是Menu标签作为xml的根节点
        CDuiMenu* pMenuRoot = static_cast<CDuiMenu*>(pRoot);
        ASSERT(pMenuRoot);

        SIZE szInit = m_PaintManager.GetInitSize();
        CDuiRect rc;
        CDuiPoint point = m_BasedPoint;
        rc.left = point.x;
        rc.top = point.y;
        rc.right = rc.left + szInit.cx;
        rc.bottom = rc.top + szInit.cy;

        int nWidth = rc.GetWidth();
        int nHeight = rc.GetHeight();

        if(m_dwAlignment & eMenuAlignment_Right)
        {
            rc.right = point.x;
            rc.left = rc.right - nWidth;
        }

        if(m_dwAlignment & eMenuAlignment_Bottom)
        {
            rc.bottom = point.y;
            rc.top = rc.bottom - nHeight;
        }
        SetForegroundWindow(GetHWND());
        MoveWindow(GetHWND(), rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
        ::SetWindowPos(GetHWND(), HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(), rc.GetHeight() + pMenuRoot->GetInset().bottom + pMenuRoot->GetInset().top, SWP_SHOWWINDOW);
    }

    void CDuiMenuWnd::ResizeSubMenu()
    {
        // Position the popup window in absolute space
        RECT rcOwner = m_pOwner->GetPos();
        RECT rc = rcOwner;

        int cxFixed = 0;
        int cyFixed = 0;

#if defined(WIN32) && !defined(UNDER_CE)
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(GetHWND(), MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
#else
        CDuiRect rcWork;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
        SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };

        for(int it = 0; it < m_pOwner->GetCount(); it++)
        {
            if(m_pOwner->GetItemAt(it)->GetInterface(_T("MenuElement")) != NULL)
            {
                CDuiControl* pControl = m_pOwner->GetItemAt(it);
                SIZE sz = pControl->EstimateSize(szAvailable);
                cyFixed += sz.cy;
                if(cxFixed < sz.cx)
                {
                    cxFixed = sz.cx;
                }
            }
        }

        RECT rcWindow;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);

        rc.top = rcOwner.top;
        rc.bottom = rc.top + cyFixed;
        ::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
        rc.left = rcWindow.right;
        rc.right = rc.left + cxFixed;
        rc.right += 2;

        BOOL bReachBottom = FALSE;
        BOOL bReachRight = FALSE;
        LONG chRightAlgin = 0;
        LONG chBottomAlgin = 0;

        RECT rcPreWindow = {0};
        MenuObserverImpl::Iterator iterator(CDuiMenuWnd::GetGlobalContextMenuObserver());
        MenuMenuReceiverImplBase* pReceiver = iterator.next();
        while(pReceiver != NULL)
        {
            CDuiMenuWnd* pContextMenu = dynamic_cast<CDuiMenuWnd*>(pReceiver);
            if(pContextMenu != NULL)
            {
                GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);

                bReachRight = rcPreWindow.left >= rcWindow.right;
                bReachBottom = rcPreWindow.top >= rcWindow.bottom;
                if(pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow() ||  bReachBottom || bReachRight)
                {
                    break;
                }
            }
            pReceiver = iterator.next();
        }

        if(bReachBottom)
        {
            rc.bottom = rcWindow.top;
            rc.top = rc.bottom - cyFixed;
        }

        if(bReachRight)
        {
            rc.right = rcWindow.left;
            rc.left = rc.right - cxFixed;
        }

        if(rc.bottom > rcWork.bottom)
        {
            rc.bottom = rc.top;
            rc.top = rc.bottom - cyFixed;
        }

        if(rc.right > rcWork.right)
        {
            rc.right = rcWindow.left;
            rc.left = rc.right - cxFixed;
        }

        if(rc.top < rcWork.top)
        {
            rc.top = rcOwner.top;
            rc.bottom = rc.top + cyFixed;
        }

        if(rc.left < rcWork.left)
        {
            rc.left = rcWindow.right;
            rc.right = rc.left + cxFixed;
        }

        MoveWindow(GetHWND(), rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + m_pLayout->GetInset().top + m_pLayout->GetInset().bottom, FALSE);
    }

    LRESULT CDuiMenuWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bShowShadow = FALSE;
        if(m_pOwner != NULL)
        {
            LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
            styleValue &= ~WS_CAPTION;
            ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
            RECT rcClient;
            ::GetClientRect(*this, &rcClient);
            ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
                           rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

            m_PaintManager.Init(GetHWND());
            m_PaintManager.GetDPIObj()->SetScale(m_pOwner->GetManager()->GetDPIObj()->GetDPI());
            // The trick is to add the items to the new container. Their owner gets
            // reassigned by this operation - which is why it is important to reassign
            // the items back to the righfull owner/manager when the window closes.
            m_pLayout = new CDuiMenu();
            m_PaintManager.SetForceUseSharedRes(TRUE);
            m_pLayout->SetManager(&m_PaintManager, NULL, TRUE);
            LPCTSTR pDefaultAttributes = m_pOwner->GetManager()->GetDefaultAttributeList(DUI_CTR_MENU);
            if(pDefaultAttributes)
            {
                m_pLayout->ApplyAttributeList(pDefaultAttributes);
            }
            m_pLayout->SetAutoDestroy(FALSE);

            for(int i = 0; i < m_pOwner->GetCount(); i++)
            {
                if(m_pOwner->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
                {
                    (static_cast<CDuiMenuElement*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
                    m_pLayout->Add(m_pOwner->GetItemAt(i));
                }
            }

            CDuiShadow* pShadow = m_pOwner->GetManager()->GetShadow();
            pShadow->CopyShadow(m_PaintManager.GetShadow());
            bShowShadow = m_PaintManager.GetShadow()->IsShowShadow();
            m_PaintManager.GetShadow()->ShowShadow(FALSE);
            m_PaintManager.SetLayered(m_pOwner->GetManager()->IsLayered());
            m_PaintManager.AttachDialog(m_pLayout);
            m_PaintManager.AddNotifier(this);

            ResizeSubMenu();
        }
        else
        {
            m_PaintManager.Init(GetHWND());
            m_PaintManager.GetDPIObj()->SetScale(GetGlobalContextMenuObserver().GetManager()->GetDPIObj()->GetDPI());
            CDuiDlgBuilder builder;

            CDuiControl* pRoot = builder.Create(m_xml, UINT(0), this, &m_PaintManager);
            ASSERT(pRoot);
            bShowShadow = m_PaintManager.GetShadow()->IsShowShadow();
            m_PaintManager.GetShadow()->ShowShadow(FALSE);
            m_PaintManager.AttachDialog(pRoot);
            m_PaintManager.AddNotifier(this);

            ResizeMenu();
        }
        GetMenuRoot()->SetMenuWnd(this);
        m_PaintManager.GetShadow()->ShowShadow(bShowShadow);
        m_PaintManager.GetShadow()->Create(&m_PaintManager);
        return 0;
    }

    LRESULT CDuiMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        HWND hFocusWnd = (HWND)wParam;

        BOOL bInMenuWindowList = FALSE;
        ContextMenuParam param;
        param.hWnd = GetHWND();

        MenuObserverImpl::Iterator iterator(GetGlobalContextMenuObserver());
        MenuMenuReceiverImplBase* pReceiver = iterator.next();
        while(pReceiver != NULL)
        {
            CDuiMenuWnd* pContextMenu = dynamic_cast<CDuiMenuWnd*>(pReceiver);
            if(pContextMenu != NULL && pContextMenu->GetHWND() ==  hFocusWnd)
            {
                bInMenuWindowList = TRUE;
                break;
            }
            pReceiver = iterator.next();
        }

        if(!bInMenuWindowList)
        {
            param.wParam = 1;
            GetGlobalContextMenuObserver().RBroadcast(param);
            return 0;
        }
        return 0;
    }

    LRESULT CDuiMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(wParam == VK_ESCAPE || wParam == VK_LEFT)
        {
            Close();
        }
        return 0;
    }

    LRESULT CDuiMenuWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
        if(!::IsIconic(*this))
        {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++;
            rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT CDuiMenuWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if(m_pOwner != NULL)
        {
            m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), FALSE);
            m_pOwner->SetPos(m_pOwner->GetPos());
            m_pOwner->SetFocus();
        }
        return 0;
    }

    LRESULT CDuiMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch(uMsg)
        {
            case WM_CREATE:
                lRes = OnCreate(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KILLFOCUS:
                lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
                break;
            case WM_KEYDOWN:
                lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
                break;
            case WM_SIZE:
                lRes = OnSize(uMsg, wParam, lParam, bHandled);
                break;
            case WM_CLOSE:
                lRes = OnClose(uMsg, wParam, lParam, bHandled);
                break;
            case WM_RBUTTONDOWN:
            case WM_CONTEXTMENU:
            case WM_RBUTTONUP:
            case WM_RBUTTONDBLCLK:
                return 0;
                break;
            default:
                bHandled = FALSE;
                break;
        }
        //if(bHandled)
        //{
        //    return lRes;
        //}
        if(m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
        {
            return lRes;
        }
        return CDuiWnd::HandleMessage(uMsg, wParam, lParam);
    }

    MenuObserverImpl& CDuiMenuWnd::GetGlobalContextMenuObserver()
    {
        static MenuObserverImpl s_context_menu_observer;
        return s_context_menu_observer;
    }

    void CDuiMenuWnd::DestroyMenu()
    {
        CStdStringPtrMap* mCheckInfos = GetGlobalContextMenuObserver().GetMenuCheckInfo();
        if(mCheckInfos != NULL)
        {
            for(int i = 0; i < mCheckInfos->GetSize(); i++)
            {
                MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(mCheckInfos->GetAt(i));
                DUI_FREE_POINT(pItemInfo);
            }
            mCheckInfos->Resize(0);
        }
    }

    MenuItemInfo* CDuiMenuWnd::SetMenuItemInfo(LPCTSTR pstrName, BOOL bChecked)
    {
        if(pstrName == NULL || lstrlen(pstrName) <= 0)
        {
            return NULL;
        }

        CStdStringPtrMap* mCheckInfos = GetGlobalContextMenuObserver().GetMenuCheckInfo();
        if(mCheckInfos != NULL)
        {
            MenuItemInfo* pItemInfo = (MenuItemInfo*)mCheckInfos->Find(pstrName);
            if(pItemInfo == NULL)
            {
                pItemInfo = new MenuItemInfo;
                pItemInfo->strName =  pstrName;
                pItemInfo->bChecked = bChecked;
                mCheckInfos->Insert(pstrName, pItemInfo);
            }
            else
            {
                pItemInfo->bChecked = bChecked;
            }

            return pItemInfo;
        }
        return NULL;
    }

    /////////////////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiMenu)

    CDuiMenu::CDuiMenu(void)
        : m_pMenuWnd(NULL)
    {
        if(GetHeader() != NULL)
        {
            GetHeader()->SetVisible(FALSE);
        }
    }


    CDuiMenu::~CDuiMenu(void)
    {
    }


    CDuiString CDuiMenu::GetClass() const
    {
        return DUI_CTR_MENU;
    }

    LPVOID CDuiMenu::GetInterface(LPCTSTR pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_MENU) == 0)
        {
            return static_cast<CDuiMenu*>(this);
        }
        return CDuiList::GetInterface(pstrName);
    }

    BOOL CDuiMenu::Add(CDuiControl* pControl)
    {
        CDuiMenuElement* pMenuItem = static_cast<CDuiMenuElement*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
        if(pMenuItem == NULL)
        {
            return FALSE;
        }

        for(int i = 0; i < pMenuItem->GetCount(); ++i)
        {
            if(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
            {
                (static_cast<CDuiMenuElement*>(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(FALSE);
            }
        }
        return CDuiList::Add(pControl);
    }

    BOOL CDuiMenu::AddAt(CDuiControl* pControl, int iIndex)
    {
        CDuiMenuElement* pMenuItem = static_cast<CDuiMenuElement*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
        if(pMenuItem == NULL)
        {
            return FALSE;
        }

        for(int i = 0; i < pMenuItem->GetCount(); ++i)
        {
            if(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT) != NULL)
            {
                (static_cast<CDuiMenuElement*>(pMenuItem->GetItemAt(i)->GetInterface(DUI_CTR_MENUELEMENT)))->SetInternVisible(FALSE);
            }
        }
        return CDuiList::AddAt(pControl, iIndex);
    }

    int CDuiMenu::GetItemIndex(CDuiControl* pControl) const
    {
        CDuiMenuElement* pMenuItem = static_cast<CDuiMenuElement*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
        if(pMenuItem == NULL)
        {
            return -1;
        }

        return CDuiList::GetItemIndex(pControl);
    }

    BOOL CDuiMenu::SetItemIndex(CDuiControl* pControl, int iIndex)
    {
        CDuiMenuElement* pMenuItem = static_cast<CDuiMenuElement*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
        if(pMenuItem == NULL)
        {
            return FALSE;
        }

        return CDuiList::SetItemIndex(pControl, iIndex);
    }

    BOOL CDuiMenu::Remove(CDuiControl* pControl)
    {
        CDuiMenuElement* pMenuItem = static_cast<CDuiMenuElement*>(pControl->GetInterface(DUI_CTR_MENUELEMENT));
        if(pMenuItem == NULL)
        {
            return FALSE;
        }

        return CDuiList::Remove(pControl);
    }

    CDuiMenuWnd* CDuiMenu::GetMenuWnd()
    {
        return m_pMenuWnd;
    }

    void CDuiMenu::SetMenuWnd(CDuiMenuWnd* pMenuWnd)
    {
        if(pMenuWnd)
        {
            m_pMenuWnd = pMenuWnd;
        }
    }

    SIZE CDuiMenu::EstimateSize(SIZE szAvailable)
    {
        int cxFixed = 0;
        int cyFixed = 0;
        for(int it = 0; it < GetCount(); it++)
        {
            CDuiControl* pControl = GetItemAt(it);
            if(!pControl->IsVisible())
            {
                continue;
            }
            SIZE sz = pControl->EstimateSize(szAvailable);
            cyFixed += sz.cy;
            if(cxFixed < sz.cx)
            {
                cxFixed = sz.cx;
            }
        }

        for(int it = 0; it < GetCount(); it++)
        {
            CDuiControl* pControl = GetItemAt(it);
            if(!pControl->IsVisible())
            {
                continue;
            }

            pControl->SetFixedWidth(MulDiv(cxFixed, 100, GetManager()->GetDPIObj()->GetScale()));
        }

        return CDuiSize(cxFixed, cyFixed);
    }



}