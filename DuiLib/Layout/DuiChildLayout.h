#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiChildLayout
        : public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiChildLayout)
    public:
        CDuiChildLayout(void);
        virtual ~CDuiChildLayout(void);
    public:
        LPVOID GetInterface(CDuiString strName) OVERRIDE;
        CDuiString GetClass() const OVERRIDE;

        void Init() OVERRIDE;

        CDuiString GetChildLayoutXML() const;
        void SetChildLayoutXML(CDuiString pXML);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;
    private:
        CDuiString m_pstrXMLFile;
    };

}
