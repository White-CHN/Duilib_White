#pragma once

namespace DuiLib
{


    class IDialogBuilderCallback
    {
    public:
        virtual CDuiControl* CreateControl(LPCTSTR pstrClass) = 0;
    };

    class DUILIB_API CDuiDlgBuilder
    {
    public:
        CDuiDlgBuilder(void);
        ~CDuiDlgBuilder(void);
    public:
        CDuiControl* Create(CIdToResource xml, LPCTSTR lpType = NULL, IDialogBuilderCallback* pCallback = NULL,
                            CDuiPaintManager* pManager = NULL, CDuiControl* pParent = NULL);

        void GetLastErrorMessage(LPTSTR lpMessage, SIZE_T szMax) const;
        void GetLastErrorLocation(LPTSTR lpSource, SIZE_T szMax) const;
    private:
        CDuiControl* Create(CDuiControl* pParent);
        CDuiControl* Parse(CDuiMarkupNode* pRoot, CDuiControl* pParent);
    private:
        CDuiPaintManager* m_pManager;
        IDialogBuilderCallback* m_pCallback;
        LPCTSTR m_lpType;

        CDuiMarkup m_xml;
    };



}