#pragma once

namespace DuiLib
{
    class DUILIB_API CDuiActiveX
        : public CDuiControl
        , public IMessageFilterUI
    {
        DECLARE_DUICONTROL(CDuiActiveX)
    public:
        CDuiActiveX(void);
        virtual ~CDuiActiveX(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) OVERRIDE;
    };

}