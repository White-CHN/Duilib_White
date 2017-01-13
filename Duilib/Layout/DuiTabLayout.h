#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiTabLayout
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiTabLayout)
    public:
        CDuiTabLayout(void);
        virtual ~CDuiTabLayout(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(LPCTSTR pstrName) OVERRIDE;

        BOOL Add(CDuiControl* pControl) OVERRIDE;
        BOOL AddAt(CDuiControl* pControl, int iIndex) OVERRIDE;
        BOOL Remove(CDuiControl* pControl) OVERRIDE;
        void RemoveAll() OVERRIDE;

        virtual BOOL SelectItem(int iIndex) ;
        virtual BOOL SelectItem(CDuiControl* pControl) ;

        int GetCurSel() const;
        void SetCurSel(int iCurSel);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
    private:
        int m_iCurSel;
    };

}