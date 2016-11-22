#include "StdAfx.h"
#include "DuiIcon.h"

namespace DuiLib
{

    CDuiIcon::CDuiIcon(void)
        : m_bEnabled(FALSE)
        , m_uMessage(DUIMSG_ICON)
    {
        ZeroMemory(&m_Data, sizeof(m_Data));
    }

    CDuiIcon::~CDuiIcon(void)
    {
        DeleteIcon();
    }

    BOOL CDuiIcon::CreateIcon(HWND hWnd, UINT uIDResource, LPCTSTR pTipText, UINT uMessage)
    {
        if(!hWnd || uIDResource <= 0)
        {
            return FALSE;
        }
        if(uMessage != 0)
        {
            m_uMessage = uMessage;
        }
        m_Data.cbSize = sizeof(NOTIFYICONDATA);
        m_Data.hWnd = hWnd;
        m_Data.uID	 = uIDResource;
        m_Data.hIcon = LoadIcon(CDuiPaintManager::GetInstance(), MAKEINTRESOURCE(uIDResource));
        m_Data.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
        m_Data.uCallbackMessage = m_uMessage;
        if(pTipText)
        {
            _tcscpy(m_Data.szTip, pTipText);
        }
        m_bEnabled = Shell_NotifyIcon(NIM_ADD, &m_Data);
        return m_bEnabled;
    }

    void CDuiIcon::DeleteIcon()
    {
        if(m_bEnabled)
        {
            Shell_NotifyIcon(NIM_DELETE, &m_Data);
        }
    }

    BOOL CDuiIcon::SetTooltipText(LPCTSTR pTipText)
    {
        if(pTipText)
        {
            _tcscpy(m_Data.szTip, pTipText);
        }
        if(!m_bEnabled)
        {
            return FALSE;
        }
        m_Data.uFlags = NIF_TIP;
        return Shell_NotifyIcon(NIM_MODIFY, &m_Data);
    }

    BOOL CDuiIcon::SetTooltipText(UINT uIDResource)
    {
        TCHAR mbuf[256] = {0};
        LoadString(CDuiPaintManager::GetInstance(), uIDResource, mbuf, sizeof(mbuf));
        return SetTooltipText(mbuf);
    }

    CDuiString CDuiIcon::GetTooltipText() const
    {
        return m_Data.szTip;
    }

    BOOL CDuiIcon::SetIcon(HICON hIcon)
    {
        m_Data.uFlags = NIF_ICON;
        m_Data.hIcon = hIcon;

        if(!m_bEnabled)
        {
            return FALSE;
        }
        return Shell_NotifyIcon(NIM_MODIFY, &m_Data);
    }

    BOOL CDuiIcon::SetIcon(LPCTSTR pFile)
    {
        return SetIcon(LoadIcon(CDuiPaintManager::GetInstance(), pFile));
    }

    BOOL CDuiIcon::SetIcon(UINT uIDResource)
    {
        return SetIcon(LoadIcon(CDuiPaintManager::GetInstance(), MAKEINTRESOURCE(uIDResource)));
    }

    HICON CDuiIcon::GetIcon() const
    {
        return m_Data.hIcon;
    }

    void CDuiIcon::RemoveIcon()
    {
        m_Data.uFlags = 0;
        Shell_NotifyIcon(NIM_DELETE, &m_Data);
        m_bEnabled = FALSE;
    }

}
