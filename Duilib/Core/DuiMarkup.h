#pragma once


namespace DuiLib
{

    enum
    {
        XMLFILE_ENCODING_UTF8 = 0,
        XMLFILE_ENCODING_UNICODE = 1,
        XMLFILE_ENCODING_ASNI = 2,
    };

    class DUILIB_API CDuiMarkup
    {
        friend class CDuiMarkupNode;
    public:
        CDuiMarkup(LPCTSTR pstrXML = NULL);
        ~CDuiMarkup(void);
    public:
        BOOL Load(LPCTSTR pstrXML);
        BOOL LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding = XMLFILE_ENCODING_UTF8);
        BOOL LoadFromFile(LPCTSTR pstrFilename, int encoding = XMLFILE_ENCODING_UTF8);
        void Release();
        BOOL IsValid() const;

        void SetPreserveWhitespace(BOOL bPreserve = TRUE);
        void GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const;
        void GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const;

        CDuiMarkupNode GetRoot();
    public:
        typedef struct tagXMLELEMENT
        {
            size_t iStart;
            size_t iChild;
            size_t iNext;
            size_t iParent;
            size_t iData;
        } XMLELEMENT;
    private:
        BOOL _Parse();
        BOOL _Parse(LPTSTR& pstrText, size_t iParent);
        XMLELEMENT* _ReserveElement();
        inline void _SkipWhitespace(LPTSTR& pstr) const;
        inline void _SkipWhitespace(LPCTSTR& pstr) const;
        inline void _SkipIdentifier(LPTSTR& pstr) const;
        inline void _SkipIdentifier(LPCTSTR& pstr) const;
        BOOL _ParseData(LPTSTR& pstrText, LPTSTR& pstrData, char cEnd);
        void _ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest);
        BOOL _ParseAttributes(LPTSTR& pstrText);
        BOOL _Failed(LPCTSTR pstrError, LPCTSTR pstrLocation = NULL);

    private:
        LPTSTR m_pstrXML;
        XMLELEMENT* m_pElements;
        ULONG m_nElements;
        ULONG m_nReservedElements;
        TCHAR m_szErrorMsg[100];
        TCHAR m_szErrorXML[50];
        BOOL m_bPreserveWhitespace;
    };


}

