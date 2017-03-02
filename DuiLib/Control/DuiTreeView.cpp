#include "StdAfx.h"
#include "DuiTreeView.h"

namespace DuiLib
{
    IMPLEMENT_DUICONTROL(CDuiTreeNode)

    CDuiTreeNode::CDuiTreeNode(CDuiTreeNode* pParentNode /*= NULL*/)
        : m_bIsVisable(TRUE)
        , m_bIsCheckBox(FALSE)
        , m_iTreeLavel(0)
        , m_dwItemTextColor(0)
        , m_dwItemHotTextColor(0)
        , m_dwSelItemTextColor(0)
        , m_dwSelItemHotTextColor(0)
        , m_pTreeView(NULL)
        , m_pHoriz(new CDuiHorizontalLayout)
        , m_pFolderButton(new CDuiCheckBox)
        , m_pDottedLine(new CDuiLabel)
        , m_pCheckBox(new CDuiCheckBox)
        , m_pItemButton(new CDuiOption)
        , m_pParentTreeNode(NULL)
    {
        SetFixedHeight(18);
        SetFixedWidth(250);
        m_pFolderButton->SetFixedWidth(GetFixedHeight());
        m_pDottedLine->SetFixedWidth(2);
        m_pCheckBox->SetFixedWidth(GetFixedHeight());
        m_pItemButton->SetAttribute(_T("align"), _T("left"));
        m_pDottedLine->SetVisible(FALSE);
        m_pCheckBox->SetVisible(FALSE);
        m_pItemButton->SetMouseEnabled(FALSE);
        if(pParentNode)
        {
            if(pParentNode->GetClass() != DUI_CTR_TREENODE)
            {
                return;
            }
            m_pDottedLine->SetVisible(pParentNode->IsVisible());
            m_pDottedLine->SetFixedWidth(pParentNode->GetDottedLine()->GetFixedWidth() + 16);
            SetParentNode(pParentNode);
        }
        m_pHoriz->SetChildVAlign(DT_VCENTER);
        m_pHoriz->Add(m_pDottedLine);
        m_pHoriz->Add(m_pFolderButton);
        m_pHoriz->Add(m_pCheckBox);
        m_pHoriz->Add(m_pItemButton);
        Add(m_pHoriz);
    }

    CDuiTreeNode::~CDuiTreeNode(void)
    {

    }

    CDuiString CDuiTreeNode::GetClass() const
    {
        return DUI_CTR_TREENODE;
    }

    LPVOID CDuiTreeNode::GetInterface(CDuiString pstrName)
    {
        if(pstrName == DUI_CTR_TREENODE)
        {
            return static_cast<CDuiTreeNode*>(this);
        }
        return CDuiListContainerElement::GetInterface(pstrName);
    }

    void CDuiTreeNode::Invalidate()
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

                if(GetManager())
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
            CDuiContainer::Invalidate();
        }
    }

    BOOL CDuiTreeNode::Select(BOOL bSelect /*= TRUE*/)
    {
        BOOL nRet = CDuiListContainerElement::Select(bSelect);
        if(IsSelected())
        {
            m_pItemButton->SetTextColor(GetSelItemTextColor());
        }
        else
        {
            m_pItemButton->SetTextColor(GetItemTextColor());
        }

        return nRet;
    }

    BOOL CDuiTreeNode::SelectMulti(BOOL bSelect /*= TRUE*/)
    {
        BOOL nRet = CDuiListContainerElement::SelectMulti(bSelect);
        if(IsSelected())
        {
            m_pItemButton->SetTextColor(GetSelItemTextColor());
        }
        else
        {
            m_pItemButton->SetTextColor(GetItemTextColor());
        }

        return nRet;
    }

    BOOL CDuiTreeNode::Add(CDuiControl* pControl)
    {
        if(pControl->GetClass() == DUI_CTR_TREENODE)
        {
            return AddChildNode((CDuiTreeNode*)pControl);
        }

        return CDuiListContainerElement::Add(pControl);
    }

    BOOL CDuiTreeNode::AddAt(CDuiControl* pControl, int iIndex)
    {
        if(NULL == static_cast<CDuiTreeNode*>(pControl->GetInterface(DUI_CTR_TREENODE)))
        {
            return FALSE;
        }

        CDuiTreeNode* pIndexNode = static_cast<CDuiTreeNode*>(m_TreeNodes.GetAt(iIndex));
        if(!pIndexNode)
        {
            if(!m_TreeNodes.Add(pControl))
            {
                return FALSE;
            }
        }
        else if(pIndexNode && !m_TreeNodes.InsertAt(iIndex, pControl))
        {
            return FALSE;
        }

        if(!pIndexNode && m_pTreeView && m_pTreeView->GetItemAt(GetTreeIndex() + 1))
        {
            pIndexNode = static_cast<CDuiTreeNode*>(m_pTreeView->GetItemAt(GetTreeIndex() + 1)->GetInterface(DUI_CTR_TREENODE));
        }

        pControl = CalLocation((CDuiTreeNode*)pControl);

        if(m_pTreeView && pIndexNode)
        {
            return m_pTreeView->AddAt((CDuiTreeNode*)pControl, pIndexNode);
        }
        else
        {
            return m_pTreeView->Add((CDuiTreeNode*)pControl);
        }

        return TRUE;
    }

    BOOL CDuiTreeNode::Remove(CDuiControl* pControl)
    {
        return RemoveAt((CDuiTreeNode*)pControl);
    }

    BOOL CDuiTreeNode::GetVisibleTag() const
    {
        return m_bIsVisable;
    }

    void CDuiTreeNode::SetVisibleTag(BOOL bIsVisible)
    {
        m_bIsVisable = bIsVisible;
    }

    CDuiString CDuiTreeNode::GetItemText()
    {
        return m_pItemButton->GetText();
    }

    void CDuiTreeNode::SetItemText(LPCTSTR pstrValue)
    {
        m_pItemButton->SetText(pstrValue);
    }

    BOOL CDuiTreeNode::IsCheckBoxSelected() const
    {
        return m_pCheckBox->IsSelected();
    }

    void CDuiTreeNode::CheckBoxSelected(BOOL bSelected)
    {
        m_pCheckBox->Selected(bSelected);
    }

    BOOL CDuiTreeNode::IsHasChild() const
    {
        return !m_TreeNodes.IsEmpty();
    }

    BOOL CDuiTreeNode::AddChildNode(CDuiTreeNode* pTreeNodeUI)
    {
        if(!pTreeNodeUI)
        {
            return FALSE;
        }

        if(pTreeNodeUI->GetClass() != DUI_CTR_TREENODE)
        {
            return FALSE;
        }

        pTreeNodeUI = CalLocation(pTreeNodeUI);

        bool nRet = TRUE;

        if(m_pTreeView)
        {
            CDuiTreeNode* pNode = static_cast<CDuiTreeNode*>(m_TreeNodes.GetAt(m_TreeNodes.GetSize() - 1));
            if(!pNode || !pNode->GetLastNode())
            {
                nRet = m_pTreeView->AddAt(pTreeNodeUI, GetTreeIndex() + 1) >= 0;
            }
            else
            {
                nRet = m_pTreeView->AddAt(pTreeNodeUI, pNode->GetLastNode()->GetTreeIndex() + 1) >= 0;
            }
        }

        if(nRet)
        {
            m_TreeNodes.Add(pTreeNodeUI);
        }

        return nRet;
    }

    BOOL CDuiTreeNode::RemoveAt(CDuiTreeNode* pTreeNodeUI)
    {
        int nIndex = m_TreeNodes.Find(pTreeNodeUI);
        CDuiTreeNode* pNode = static_cast<CDuiTreeNode*>(m_TreeNodes.GetAt(nIndex));
        if(pNode && pNode == pTreeNodeUI)
        {
            while(pNode->IsHasChild())
            {
                pNode->RemoveAt(static_cast<CDuiTreeNode*>(pNode->m_TreeNodes.GetAt(0)));
            }

            m_TreeNodes.Remove(nIndex);

            if(m_pTreeView)
            {
                m_pTreeView->Remove(pTreeNodeUI);
            }

            return TRUE;
        }
        return FALSE;
    }

    CDuiTreeNode* CDuiTreeNode::GetParentNode() const
    {
        return m_pParentTreeNode;
    }

    void CDuiTreeNode::SetParentNode(CDuiTreeNode* pParentTreeNode)
    {
        m_pParentTreeNode = pParentTreeNode;
    }

    long CDuiTreeNode::GetCountChild() const
    {
        return m_TreeNodes.GetSize();
    }

    void CDuiTreeNode::SetTreeView(CDuiTreeView* pCTreeViewUI)
    {
        m_pTreeView = pCTreeViewUI;
    }

    CDuiTreeView* CDuiTreeNode::GetTreeView() const
    {
        return m_pTreeView;
    }

    CDuiTreeNode* CDuiTreeNode::GetChildNode(int nIndex) const
    {
        return static_cast<CDuiTreeNode*>(m_TreeNodes.GetAt(nIndex));
    }

    CDuiHorizontalLayout* CDuiTreeNode::GetTreeNodeHoriznotal() const
    {
        return m_pHoriz;
    }

    CDuiCheckBox* CDuiTreeNode::GetFolderButton() const
    {
        return m_pFolderButton;
    }

    CDuiLabel* CDuiTreeNode::GetDottedLine() const
    {
        return m_pDottedLine;
    }

    CDuiCheckBox* CDuiTreeNode::GetCheckBox() const
    {
        return m_pCheckBox;
    }

    CDuiOption* CDuiTreeNode::GetItemButton() const
    {
        return m_pItemButton;
    }

    CStdPtrArray CDuiTreeNode::GetTreeNodes()
    {
        return m_TreeNodes;
    }

    int CDuiTreeNode::GetTreeIndex()
    {
        if(!m_pTreeView)
        {
            return -1;
        }

        for(int nIndex = 0; nIndex < m_pTreeView->GetCount(); nIndex++)
        {
            if(this == m_pTreeView->GetItemAt(nIndex))
            {
                return nIndex;
            }
        }

        return -1;
    }

    int CDuiTreeNode::GetNodeIndex()
    {
        if(!GetParentNode() && !m_pTreeView)
        {
            return -1;
        }

        if(!GetParentNode() && m_pTreeView)
        {
            return GetTreeIndex();
        }

        return GetParentNode()->GetTreeNodes().Find(this);
    }

    BOOL CDuiTreeNode::GetVisibleFolderBtn() const
    {
        return m_pFolderButton->IsVisible();
    }

    void CDuiTreeNode::SetVisibleFolderBtn(BOOL bIsVisibled)
    {
        m_pFolderButton->SetVisible(bIsVisibled);
    }

    BOOL CDuiTreeNode::GetVisibleCheckBtn() const
    {
        return m_pCheckBox->IsVisible();
    }

    void CDuiTreeNode::SetVisibleCheckBtn(BOOL bIsVisibled)
    {
        m_pCheckBox->SetVisible(bIsVisibled);
    }

    DWORD CDuiTreeNode::GetItemTextColor() const
    {
        return m_dwItemTextColor;
    }

    void CDuiTreeNode::SetItemTextColor(DWORD dwItemTextColor)
    {
        m_dwItemTextColor	= dwItemTextColor;
        m_pItemButton->SetTextColor(m_dwItemTextColor);
    }

    DWORD CDuiTreeNode::GetItemHotTextColor() const
    {
        return m_dwItemHotTextColor;
    }

    void CDuiTreeNode::SetItemHotTextColor(DWORD dwItemHotTextColor)
    {
        m_dwItemHotTextColor = dwItemHotTextColor;
        Invalidate();
    }

    DWORD CDuiTreeNode::GetSelItemTextColor() const
    {
        return m_dwSelItemTextColor;
    }

    void CDuiTreeNode::SetSelItemTextColor(DWORD dwSelItemTextColor)
    {
        m_dwSelItemTextColor = dwSelItemTextColor;
        Invalidate();
    }

    DWORD CDuiTreeNode::GetSelItemHotTextColor() const
    {
        return m_dwSelItemHotTextColor;
    }

    void CDuiTreeNode::SetSelItemHotTextColor(DWORD dwSelHotItemTextColor)
    {
        m_dwSelItemHotTextColor = dwSelHotItemTextColor;
        Invalidate();
    }

    void CDuiTreeNode::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("text")) == 0)
        {
            m_pItemButton->SetText(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("horizattr")) == 0)
        {
            m_pHoriz->ApplyAttributeList(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("dotlineattr")) == 0)
        {
            m_pDottedLine->ApplyAttributeList(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("folderattr")) == 0)
        {
            m_pFolderButton->ApplyAttributeList(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("checkboxattr")) == 0)
        {
            m_pCheckBox->ApplyAttributeList(pstrValue);
        }
        else if(_tcsicmp(pstrName, _T("itemattr")) == 0)
        {
            m_pItemButton->ApplyAttributeList(pstrValue);
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
        else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemHotTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("selitemtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("selitemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelItemHotTextColor(clrColor);
        }
        else
        {
            CDuiListContainerElement::SetAttribute(pstrName, pstrValue);
        }
    }

    void CDuiTreeNode::DoEvent(CDuiEvent& event)
    {
        if(!IsMouseEnabled() && event.Type > DUIEVENT__MOUSEBEGIN && event.Type < DUIEVENT__MOUSEEND)
        {
            if(GetOwner() != NULL)
            {
                GetOwner()->DoEvent(event);
            }
            else
            {
                CDuiContainer::DoEvent(event);
            }
            return;
        }
        CDuiListContainerElement::DoEvent(event);
        if(event.Type == DUIEVENT_DBLCLICK)
        {
            if(IsEnabled())
            {
                GetManager()->SendNotify(this, DUI_MSGTYPE_TREEITEMDBCLICK);
                Invalidate();
            }
            return;
        }
        if(event.Type == DUIEVENT_MOUSEENTER)
        {
            if(IsEnabled())
            {
                if(IsSelected() && GetSelItemHotTextColor())
                {
                    m_pItemButton->SetTextColor(GetSelItemHotTextColor());
                }
                else
                {
                    m_pItemButton->SetTextColor(GetItemHotTextColor());
                }
            }
            else
            {
                m_pItemButton->SetTextColor(m_pItemButton->GetDisabledTextColor());
            }

            return;
        }
        if(event.Type == DUIEVENT_MOUSELEAVE)
        {
            if(IsEnabled())
            {
                if(IsSelected() && GetSelItemTextColor())
                {
                    m_pItemButton->SetTextColor(GetSelItemTextColor());
                }
                else if(!IsSelected())
                {
                    m_pItemButton->SetTextColor(GetItemTextColor());
                }
            }
            else
            {
                m_pItemButton->SetTextColor(m_pItemButton->GetDisabledTextColor());
            }

            return;
        }
    }

    CDuiTreeNode* CDuiTreeNode::GetLastNode()
    {
        if(!IsHasChild())
        {
            return this;
        }

        CDuiTreeNode* nRetNode = NULL;
        for(int nIndex = 0; nIndex < m_TreeNodes.GetSize(); nIndex++)
        {
            CDuiTreeNode* pNode = static_cast<CDuiTreeNode*>(m_TreeNodes.GetAt(nIndex));
            if(!pNode)
            {
                continue;
            }
            if(pNode->IsHasChild())
            {
                nRetNode = pNode->GetLastNode();
            }
            else
            {
                nRetNode = pNode;
            }
        }

        return nRetNode;
    }

    CDuiTreeNode* CDuiTreeNode::CalLocation(CDuiTreeNode* pTreeNodeUI)
    {
        pTreeNodeUI->GetDottedLine()->SetVisible(TRUE);
        pTreeNodeUI->GetDottedLine()->SetFixedWidth(m_pDottedLine->GetFixedWidth() + 16);
        pTreeNodeUI->SetParentNode(this);
        pTreeNodeUI->GetItemButton()->SetGroup(m_pItemButton->GetGroup());
        pTreeNodeUI->SetTreeView(m_pTreeView);

        return pTreeNodeUI;
    }

    //////////////////////////////////////////////////
    IMPLEMENT_DUICONTROL(CDuiTreeView)

    CDuiTreeView::CDuiTreeView(void)
        : m_bVisibleFolderBtn(TRUE)
        , m_bVisibleCheckBtn(FALSE)
        , m_uItemMinWidth(0)
    {
        GetHeader()->SetVisible(FALSE);
    }


    CDuiTreeView::~CDuiTreeView(void)
    {
    }

    CDuiString CDuiTreeView::GetClass() const
    {
        return DUI_CTR_TREEVIEW;
    }

    LPVOID CDuiTreeView::GetInterface(CDuiString pstrName)
    {
        if(_tcsicmp(pstrName, DUI_CTR_TREEVIEW) == 0)
        {
            return static_cast<CDuiTreeView*>(this);
        }
        return CDuiList::GetInterface(pstrName);
    }

    UINT CDuiTreeView::GetListType()
    {
        return LT_TREE;
    }

    BOOL CDuiTreeView::Add(CDuiTreeNode* pControl)
    {
        if(!pControl)
        {
            return FALSE;
        }
        if(pControl->GetClass() != DUI_CTR_TREENODE)
        {
            return FALSE;
        }

        pControl->OnNotify += MakeDelegate(this, &CDuiTreeView::OnDBClickItem);
        pControl->GetFolderButton()->OnNotify += MakeDelegate(this, &CDuiTreeView::OnFolderChanged);
        pControl->GetCheckBox()->OnNotify += MakeDelegate(this, &CDuiTreeView::OnCheckBoxChanged);

        pControl->SetVisibleFolderBtn(m_bVisibleFolderBtn);
        pControl->SetVisibleCheckBtn(m_bVisibleCheckBtn);
        if(m_uItemMinWidth > 0)
        {
            pControl->SetMinWidth(m_uItemMinWidth);
        }

        CDuiList::Add(pControl);

        if(pControl->GetCountChild() > 0)
        {
            int nCount = pControl->GetCountChild();
            for(int nIndex = 0; nIndex < nCount; nIndex++)
            {
                CDuiTreeNode* pNode = pControl->GetChildNode(nIndex);
                if(pNode)
                {
                    Add(pNode);
                }
            }
        }

        pControl->SetTreeView(this);
        return TRUE;
    }

    long CDuiTreeView::AddAt(CDuiTreeNode* pControl, int iIndex)
    {
        if(!pControl)
        {
            return -1;
        }
        if(pControl->GetClass() != DUI_CTR_TREENODE)
        {
            return -1;
        }
        pControl->OnNotify += MakeDelegate(this, &CDuiTreeView::OnDBClickItem);
        pControl->GetFolderButton()->OnNotify += MakeDelegate(this, &CDuiTreeView::OnFolderChanged);
        pControl->GetCheckBox()->OnNotify += MakeDelegate(this, &CDuiTreeView::OnCheckBoxChanged);
        pControl->SetVisibleFolderBtn(m_bVisibleFolderBtn);
        pControl->SetVisibleCheckBtn(m_bVisibleCheckBtn);

        if(m_uItemMinWidth > 0)
        {
            pControl->SetMinWidth(m_uItemMinWidth);
        }
        CDuiList::AddAt(pControl, iIndex);
        if(pControl->GetCountChild() > 0)
        {
            int nCount = pControl->GetCountChild();
            for(int nIndex = 0; nIndex < nCount; nIndex++)
            {
                CDuiTreeNode* pNode = pControl->GetChildNode(nIndex);
                if(pNode)
                {
                    return AddAt(pNode, iIndex + 1);
                }
            }
        }
        else
        {
            return iIndex + 1;
        }

        return -1;
    }

    BOOL CDuiTreeView::AddAt(CDuiTreeNode* pControl, CDuiTreeNode* _IndexNode)
    {
        if(!_IndexNode && !pControl)
        {
            return FALSE;
        }

        int nItemIndex = -1;
        for(int nIndex = 0; nIndex < GetCount(); nIndex++)
        {
            if(_IndexNode == GetItemAt(nIndex))
            {
                nItemIndex = nIndex;
                break;
            }
        }

        if(nItemIndex == -1)
        {
            return FALSE;
        }

        return AddAt(pControl, nItemIndex) >= 0;
    }

    BOOL CDuiTreeView::Remove(CDuiTreeNode* pControl)
    {
        if(pControl->GetCountChild() > 0)
        {
            int nCount = pControl->GetCountChild();
            for(int nIndex = 0; nIndex < nCount; nIndex++)
            {
                CDuiTreeNode* pNode = pControl->GetChildNode(nIndex);
                if(pNode)
                {
                    pControl->Remove(pNode);
                }
            }
        }
        CDuiList::Remove(pControl);
        return TRUE;
    }

    BOOL CDuiTreeView::RemoveAt(int iIndex)
    {
        CDuiTreeNode* pItem = (CDuiTreeNode*)GetItemAt(iIndex);
        if(pItem->GetCountChild())
        {
            Remove(pItem);
        }
        return TRUE;
    }

    void CDuiTreeView::RemoveAll()
    {
        CDuiList::RemoveAll();
    }

    bool CDuiTreeView::OnCheckBoxChanged(void* param)
    {
        CDuiNotify* pMsg = (CDuiNotify*)param;
        if(pMsg->sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            CDuiCheckBox* pCheckBox = (CDuiCheckBox*)pMsg->pSender;
            CDuiTreeNode* pItem = (CDuiTreeNode*)pCheckBox->GetParent()->GetParent();
            SetItemCheckBox(pCheckBox->GetCheck(), pItem);
            return TRUE;
        }
        return TRUE;
    }

    bool CDuiTreeView::OnFolderChanged(void* param)
    {
        CDuiNotify* pMsg = (CDuiNotify*)param;
        if(pMsg->sType == DUI_MSGTYPE_SELECTCHANGED)
        {
            CDuiCheckBox* pFolder = (CDuiCheckBox*)pMsg->pSender;
            CDuiTreeNode* pItem = (CDuiTreeNode*)pFolder->GetParent()->GetParent();
            pItem->SetVisibleTag(!pFolder->GetCheck());
            SetItemExpand(!pFolder->GetCheck(), pItem);
            return TRUE;
        }
        return TRUE;
    }

    bool CDuiTreeView::OnDBClickItem(void* param)
    {
        CDuiNotify* pMsg = (CDuiNotify*)param;
        if(_tcsicmp(pMsg->sType, DUI_MSGTYPE_TREEITEMDBCLICK) == 0)
        {
            CDuiTreeNode* pItem		= static_cast<CDuiTreeNode*>(pMsg->pSender);
            CDuiCheckBox* pFolder	= pItem->GetFolderButton();
            pFolder->Selected(!pFolder->IsSelected());
            pItem->SetVisibleTag(!pFolder->GetCheck());
            SetItemExpand(!pFolder->GetCheck(), pItem);
            return TRUE;
        }
        return FALSE;
    }

    BOOL CDuiTreeView::SetItemCheckBox(BOOL bSelected, CDuiTreeNode* pTreeNode /*= NULL*/)
    {
        if(pTreeNode)
        {
            if(pTreeNode->GetCountChild() > 0)
            {
                int nCount = pTreeNode->GetCountChild();
                for(int nIndex = 0; nIndex < nCount; nIndex++)
                {
                    CDuiTreeNode* pItem = pTreeNode->GetChildNode(nIndex);
                    pItem->GetCheckBox()->Selected(bSelected);
                    if(pItem->GetCountChild())
                    {
                        SetItemCheckBox(bSelected, pItem);
                    }
                }
            }
            return TRUE;
        }
        else
        {
            int nIndex = 0;
            int nCount = GetCount();
            while(nIndex < nCount)
            {
                CDuiTreeNode* pItem = (CDuiTreeNode*)GetItemAt(nIndex);
                pItem->GetCheckBox()->Selected(bSelected);
                if(pItem->GetCountChild())
                {
                    SetItemCheckBox(bSelected, pItem);
                }

                nIndex++;
            }
            return TRUE;
        }
        return FALSE;
    }

    void CDuiTreeView::SetItemExpand(BOOL bExpanded, CDuiTreeNode* pTreeNode /*= NULL*/)
    {
        if(pTreeNode)
        {
            if(pTreeNode->GetCountChild() > 0)
            {
                int nCount = pTreeNode->GetCountChild();
                for(int nIndex = 0; nIndex < nCount; nIndex++)
                {
                    CDuiTreeNode* pItem = pTreeNode->GetChildNode(nIndex);
                    pItem->SetVisible(bExpanded);
                    if(pItem->GetCountChild() && !pItem->GetFolderButton()->IsSelected())
                    {
                        SetItemExpand(bExpanded, pItem);
                    }
                }
            }
        }
        else
        {
            int nIndex = 0;
            int nCount = GetCount();
            while(nIndex < nCount)
            {
                CDuiTreeNode* pItem = (CDuiTreeNode*)GetItemAt(nIndex);
                pItem->SetVisible(bExpanded);
                if(pItem->GetCountChild() && !pItem->GetFolderButton()->IsSelected())
                {
                    SetItemExpand(bExpanded, pItem);
                }
                nIndex++;
            }
        }
    }

    BOOL CDuiTreeView::GetVisibleFolderBtn() const
    {
        return m_bVisibleFolderBtn;
    }

    void CDuiTreeView::SetVisibleFolderBtn(BOOL bIsVisibled)
    {
        m_bVisibleFolderBtn = bIsVisibled;
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pItem = static_cast<CDuiTreeNode*>(this->GetItemAt(nIndex));
            pItem->GetFolderButton()->SetVisible(m_bVisibleFolderBtn);
        }
    }

    BOOL CDuiTreeView::GetVisibleCheckBtn() const
    {
        return m_bVisibleCheckBtn;
    }

    void CDuiTreeView::SetVisibleCheckBtn(BOOL bIsVisibled)
    {
        m_bVisibleCheckBtn = bIsVisibled;
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pItem = static_cast<CDuiTreeNode*>(this->GetItemAt(nIndex));
            pItem->GetCheckBox()->SetVisible(m_bVisibleCheckBtn);
        }
    }

    UINT CDuiTreeView::GetItemMinWidth() const
    {
        return m_uItemMinWidth;
    }

    void CDuiTreeView::SetItemMinWidth(UINT bItemMinWidth)
    {
        m_uItemMinWidth = bItemMinWidth;
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pTreeNode = static_cast<CDuiTreeNode*>(GetItemAt(nIndex));
            if(pTreeNode)
            {
                pTreeNode->SetMinWidth(GetItemMinWidth());
            }
        }
        Invalidate();
    }

    void CDuiTreeView::SetItemTextColor(DWORD dwItemTextColor)
    {
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pTreeNode = static_cast<CDuiTreeNode*>(GetItemAt(nIndex));
            if(pTreeNode)
            {
                pTreeNode->SetItemTextColor(dwItemTextColor);
            }
        }
    }

    void CDuiTreeView::SetItemHotTextColor(DWORD dwItemHotTextColor)
    {
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pTreeNode = static_cast<CDuiTreeNode*>(GetItemAt(nIndex));
            if(pTreeNode)
            {
                pTreeNode->SetItemHotTextColor(dwItemHotTextColor);
            }
        }
    }

    void CDuiTreeView::SetSelItemTextColor(DWORD dwSelItemTextColor)
    {
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pTreeNode = static_cast<CDuiTreeNode*>(GetItemAt(nIndex));
            if(pTreeNode)
            {
                pTreeNode->SetSelItemTextColor(dwSelItemTextColor);
            }
        }
    }

    void CDuiTreeView::SetSelItemHotTextColor(DWORD dwSelHotItemTextColor)
    {
        for(int nIndex = 0; nIndex < CDuiList::GetCount(); nIndex++)
        {
            CDuiTreeNode* pTreeNode = static_cast<CDuiTreeNode*>(GetItemAt(nIndex));
            if(pTreeNode)
            {
                pTreeNode->SetSelItemHotTextColor(dwSelHotItemTextColor);
            }
        }
    }

    void CDuiTreeView::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
    {
        if(_tcsicmp(pstrName, _T("visiblefolderbtn")) == 0)
        {
            SetVisibleFolderBtn(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("visiblecheckbtn")) == 0)
        {
            SetVisibleCheckBtn(_tcsicmp(pstrValue, _T("true")) == 0);
        }
        else if(_tcsicmp(pstrName, _T("itemminwidth")) == 0)
        {
            SetItemMinWidth(_ttoi(pstrValue));
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
        else if(_tcsicmp(pstrName, _T("itemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetItemHotTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("selitemtextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelItemTextColor(clrColor);
        }
        else if(_tcsicmp(pstrName, _T("selitemhottextcolor")) == 0)
        {
            if(*pstrValue == _T('#'))
            {
                pstrValue = ::CharNext(pstrValue);
            }
            LPTSTR pstr = NULL;
            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
            SetSelItemHotTextColor(clrColor);
        }
        else
        {
            CDuiList::SetAttribute(pstrName, pstrValue);
        }
    }

}