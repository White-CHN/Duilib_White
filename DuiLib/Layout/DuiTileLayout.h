#pragma once
namespace DuiLib
{
    class DUILIB_API CDuiTileLayout :
        public CDuiContainer
    {
        DECLARE_DUICONTROL(CDuiTileLayout)
    public:
        CDuiTileLayout(void);
        virtual ~CDuiTileLayout(void);
    public:
        CDuiString GetClass() const OVERRIDE;
        LPVOID GetInterface(CDuiString strName) OVERRIDE;

        SIZE GetItemSize() const;
        void SetItemSize(SIZE szItem);
        int GetColumns() const;
        void SetColumns(int nCols);

        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) OVERRIDE;

        void SetPos(RECT rc, BOOL bNeedInvalidate = TRUE) OVERRIDE;
    private:
        int m_nColumns;
        CDuiSize m_szItem;
    };


}
