#include "StdAfx.h"
#include "DuiNotifyPump.h"


namespace DuiLib
{
    DUI_BASE_BEGIN_MESSAGE_MAP(CDuiNotifyPump)
    DUI_END_MESSAGE_MAP()

    CDuiNotifyPump::CDuiNotifyPump(void)
    {
    }


    CDuiNotifyPump::~CDuiNotifyPump(void)
    {
    }

    const DUI_MSGMAP_ENTRY* CDuiNotifyPump::DuiFindMessageEntry(const DUI_MSGMAP_ENTRY* lpEntry, CDuiNotify& msg)
    {
        CDuiString sMsgType = msg.sType;
        CDuiString sCtrlName = msg.pSender->GetName();
        const DUI_MSGMAP_ENTRY* pMsgTypeEntry = NULL;
        while(lpEntry->nSig != DuiSig_end)
        {
            if(lpEntry->sMsgType == sMsgType)
            {
                if(!lpEntry->sCtrlName.IsEmpty())
                {
                    if(lpEntry->sCtrlName == sCtrlName)
                    {
                        return lpEntry;
                    }
                }
                else
                {
                    pMsgTypeEntry = lpEntry;
                }
            }
            lpEntry++;
        }
        return pMsgTypeEntry;
    }

    BOOL CDuiNotifyPump::LoopDispatch(CDuiNotify& msg)
    {
        const DUI_MSGMAP_ENTRY* lpEntry = NULL;
        const DUI_MSGMAP* pMessageMap = NULL;

#ifndef DUILIB_STATIC
        for(pMessageMap = GetMessageMap(); pMessageMap != NULL; pMessageMap = (*pMessageMap->pfnGetBaseMap)())
#else
        for(pMessageMap = GetMessageMap(); pMessageMap != NULL; pMessageMap = pMessageMap->pBaseMap)
#endif
        {
#ifndef DUILIB_STATIC
            ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
#else
            ASSERT(pMessageMap != pMessageMap->pBaseMap);
#endif
            if((lpEntry = DuiFindMessageEntry(pMessageMap->lpEntries, msg)) != NULL)
            {
                DuiMessageMapFunctions mmf;
                mmf.pfn = lpEntry->pfn;

                BOOL bRet = FALSE;
                int nSig = lpEntry->nSig;
                switch(nSig)
                {
                    default:
                        ASSERT(FALSE);
                        break;
                    case DuiSig_lwl:
                        (this->*mmf.pfn_Notify_lwl)(msg.wParam, msg.lParam);
                        bRet = TRUE;
                        break;
                    case DuiSig_vn:
                        (this->*mmf.pfn_Notify_vn)(msg);
                        bRet = TRUE;
                        break;
                }
                return bRet;
            }
        }
        return FALSE;
    }

    BOOL CDuiNotifyPump::AddVirtualWnd(CDuiString strName, CDuiNotifyPump* pObject)
    {
        if(m_VirtualWndMap.Find(strName) == NULL)
        {
            m_VirtualWndMap.Insert(strName.GetData(), (LPVOID)pObject);
            return TRUE;
        }
        return FALSE;
    }

    BOOL CDuiNotifyPump::RemoveVirtualWnd(CDuiString strName)
    {
        if(m_VirtualWndMap.Find(strName) != NULL)
        {
            m_VirtualWndMap.Remove(strName);
            return TRUE;
        }
        return FALSE;
    }

    void CDuiNotifyPump::NotifyPump(CDuiNotify& msg)
    {
        ///遍历虚拟窗口
        if(!msg.sVirtualWnd.IsEmpty())
        {
            for(int i = 0; i < m_VirtualWndMap.GetSize(); i++)
            {
                if(LPCTSTR key = m_VirtualWndMap.GetAt(i))
                {
                    if(msg.sVirtualWnd == key)
                    {
                        CDuiNotifyPump* pObject = static_cast<CDuiNotifyPump*>(m_VirtualWndMap.Find(key, FALSE));
                        if(pObject && pObject->LoopDispatch(msg))
                        {
                            return;
                        }
                    }
                }
            }
        }

        ///
        //遍历主窗口
        LoopDispatch(msg);
    }

}