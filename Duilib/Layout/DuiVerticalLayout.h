#pragma once

namespace DuiLib
{

    class DUILIB_API CDuiVerticalLayout
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiVerticalLayout)
    public:
        CDuiVerticalLayout(void);
        virtual ~CDuiVerticalLayout(void);
    public:
        virtual LPCTSTR GetClass() const;
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
    };


}
