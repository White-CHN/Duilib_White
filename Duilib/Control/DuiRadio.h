#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiRadio :
        public CDuiOption
    {
        DECLARE_DUICONTROL(CDuiRadio)
    public:
        CDuiRadio(void);
        virtual ~CDuiRadio(void);
    public:
        CDuiString GetClass() const  OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;
        void DoEvent(CDuiEvent& event)  OVERRIDE;
    };
}
