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
        virtual LPCTSTR GetClass() const;
        virtual LPVOID GetInterface(LPCTSTR pstrName);

        virtual BOOL Add(CDuiControl* pControl);
        virtual BOOL AddAt(CDuiControl* pControl, int iIndex);
        virtual BOOL Remove(CDuiControl* pControl);
        virtual void RemoveAll();

        virtual BOOL SelectItem(int iIndex);
        virtual BOOL SelectItem(CDuiControl* pControl);

        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
        virtual void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE);
    private:
        int m_iCurSel;
    };

}