#pragma once
namespace DuiLib
{
    // 控件文字查询接口
    class DUILIB_API IQueryControlText
    {
    public:
        virtual LPCTSTR QueryControlText(LPCTSTR lpstrId, LPCTSTR lpstrType) = 0;
    };

    class DUILIB_API CDuiResourceManager
    {
    private:
        CDuiResourceManager(void);
        virtual ~CDuiResourceManager(void);
    public:
        static CDuiResourceManager* GetInstance();
        void Release(void);

        BOOL LoadResource(STRINGorID xml, LPCTSTR type = NULL);

        CDuiString GetText(LPCTSTR lpstrId, LPCTSTR lpstrType = NULL);

        LPCTSTR GetImagePath(LPCTSTR lpstrId);

        LPCTSTR GetXmlPath(LPCTSTR lpstrId);
    private:
        BOOL LoadResource(CDuiMarkupNode Root);
    private:
        IQueryControlText* m_pQuerypInterface;

        CDuiMarkup m_xml;
        CStdStringPtrMap m_mTextResourceHashMap;
        CStdStringPtrMap m_mImageHashMap;
        CStdStringPtrMap m_mXmlHashMap;
    };
}

