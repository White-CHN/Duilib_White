#pragma once
#include <ShellAPI.h>

namespace DuiLib
{
    class CDuiIcon
    {
    public:
        CDuiIcon(void);
        virtual ~CDuiIcon(void);

    public:
        BOOL CreateIcon(HWND hWnd, UINT uIDResource, LPCTSTR pTipText = NULL, UINT uMessage = 0);
        void DeleteIcon();
        BOOL SetTooltipText(LPCTSTR pTipText);
        BOOL SetTooltipText(UINT uIDResource);
        CDuiString GetTooltipText() const;

        BOOL SetIcon(HICON hIcon);
        BOOL SetIcon(LPCTSTR pFile);
        BOOL SetIcon(UINT uIDResource);
        HICON GetIcon() const;
        void RemoveIcon();

    private:
        BOOL m_bEnabled;
        UINT m_uMessage;
        NOTIFYICONDATA m_Data;
    };
}

