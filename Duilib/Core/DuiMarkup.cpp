#include "StdAfx.h"
#include "DuiMarkup.h"

namespace DuiLib
{

    CDuiMarkup::CDuiMarkup(LPCTSTR pstrXML /*= NULL*/)
        : m_pstrXML(NULL)
        , m_pElements(NULL)
        , m_nElements(0)
        , m_nReservedElements(0)
        , m_bPreserveWhitespace(TRUE)
    {
        ZeroMemory(m_szErrorMsg, sizeof(m_szErrorMsg));
        ZeroMemory(m_szErrorXML, sizeof(m_szErrorXML));
        if(pstrXML != NULL)
        {
            Load(pstrXML);
        }
    }

    CDuiMarkup::~CDuiMarkup(void)
    {
        Release();
    }

    BOOL CDuiMarkup::Load(LPCTSTR pstrXML)
    {
        Release();
        SIZE_T cchLen = _tcslen(pstrXML) + 1;
        m_pstrXML = static_cast<LPTSTR>(malloc(cchLen * sizeof(TCHAR)));
        ::CopyMemory(m_pstrXML, pstrXML, cchLen * sizeof(TCHAR));
        BOOL bRes = _Parse();
        if(!bRes)
        {
            Release();
        }
        return bRes;
    }

    BOOL CDuiMarkup::LoadFromMem(BYTE* pByte, DWORD dwSize, int encoding /*= XMLFILE_ENCODING_UTF8*/)
    {
#ifdef _UNICODE
        if(encoding == XMLFILE_ENCODING_UTF8)
        {
            if(dwSize >= 3 && pByte[0] == 0xEF && pByte[1] == 0xBB && pByte[2] == 0xBF)
            {
                pByte += 3;
                dwSize -= 3;
            }
            DWORD nWide = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, NULL, 0);
            m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1) * sizeof(TCHAR)));
            ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, m_pstrXML, nWide);
            m_pstrXML[nWide] = _T('\0');
        }
        else if(encoding == XMLFILE_ENCODING_ASNI)
        {
            DWORD nWide = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pByte, dwSize, NULL, 0);
            m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1) * sizeof(TCHAR)));
            ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pByte, dwSize, m_pstrXML, nWide);
            m_pstrXML[nWide] = _T('\0');
        }
        else
        {
            if(dwSize >= 2 && ((pByte[0] == 0xFE && pByte[1] == 0xFF) || (pByte[0] == 0xFF && pByte[1] == 0xFE)))
            {
                dwSize = dwSize / 2 - 1;
                if(pByte[0] == 0xFE && pByte[1] == 0xFF)
                {
                    pByte += 2;
                    for(DWORD nSwap = 0 ; nSwap < dwSize ; nSwap ++)
                    {
                        register CHAR nTemp = pByte[(nSwap << 1) + 0 ];
                        pByte[(nSwap << 1) + 0 ] = pByte[(nSwap << 1) + 1 ];
                        pByte[(nSwap << 1) + 1 ] = nTemp;
                    }
                }
                else
                {
                    pByte += 2;
                }
                m_pstrXML = static_cast<LPTSTR>(malloc((dwSize + 1) * sizeof(TCHAR)));
                ::CopyMemory(m_pstrXML, pByte, dwSize * sizeof(TCHAR));
                m_pstrXML[dwSize] = _T('\0');
                pByte -= 2;
            }
        }
#else // !_UNICODE
        if(encoding == XMLFILE_ENCODING_UTF8)
        {
            if(dwSize >= 3 && pByte[0] == 0xEF && pByte[1] == 0xBB && pByte[2] == 0xBF)
            {
                pByte += 3;
                dwSize -= 3;
            }
            DWORD nWide = ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, NULL, 0);
            LPWSTR w_str = static_cast<LPWSTR>(malloc((nWide + 1) * sizeof(WCHAR)));
            ::MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pByte, dwSize, w_str, nWide);
            w_str[nWide] = L'\0';
            DWORD wide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, NULL, 0, NULL, NULL);
            m_pstrXML = static_cast<LPTSTR>(malloc((wide + 1) * sizeof(TCHAR)));
            ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)w_str, nWide, m_pstrXML, wide, NULL, NULL);
            m_pstrXML[wide] = _T('\0');
            free(w_str);
        }
        else if(encoding == XMLFILE_ENCODING_UNICODE)
        {
            if(dwSize >= 2 && ((pByte[0] == 0xFE && pByte[1] == 0xFF) || (pByte[0] == 0xFF && pByte[1] == 0xFE)))
            {
                dwSize = dwSize / 2 - 1;
                if(pByte[0] == 0xFE && pByte[1] == 0xFF)
                {
                    pByte += 2;
                    for(DWORD nSwap = 0 ; nSwap < dwSize ; nSwap ++)
                    {
                        register CHAR nTemp = pByte[(nSwap << 1) + 0 ];
                        pByte[(nSwap << 1) + 0 ] = pByte[(nSwap << 1) + 1 ];
                        pByte[(nSwap << 1) + 1 ] = nTemp;
                    }
                }
                else
                {
                    pByte += 2;
                }
                DWORD nWide = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pByte, dwSize, NULL, 0, NULL, NULL);
                m_pstrXML = static_cast<LPTSTR>(malloc((nWide + 1) * sizeof(TCHAR)));
                ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)pByte, dwSize, m_pstrXML, nWide, NULL, NULL);
                m_pstrXML[nWide] = _T('\0');
                pByte -= 2;
            }
        }
        else
        {
            m_pstrXML = static_cast<LPTSTR>(malloc((dwSize + 1) * sizeof(TCHAR)));
            ::CopyMemory(m_pstrXML, pByte, dwSize * sizeof(TCHAR));
            m_pstrXML[dwSize] = _T('\0');
        }
#endif // _UNICODE
        BOOL bRes = _Parse();
        if(!bRes)
        {
            Release();
        }
        return bRes;
    }

    BOOL CDuiMarkup::LoadFromFile(LPCTSTR pstrFilename, int encoding /*= XMLFILE_ENCODING_UTF8*/)
    {
        Release();
        CDuiString sFile = CDuiPaintManager::GetResourcePath();
        if(CDuiPaintManager::GetResourceZip().IsEmpty())
        {
            sFile += pstrFilename;
            HANDLE hFile = ::CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if(hFile == INVALID_HANDLE_VALUE)
            {
                return _Failed(_T("Error opening file"));
            }
            DWORD dwSize = ::GetFileSize(hFile, NULL);
            if(dwSize == 0)
            {
                return _Failed(_T("File is empty"));
            }
            if(dwSize > 4096 * 1024)
            {
                return _Failed(_T("File too large"));
            }
            DWORD dwRead = 0;
            BYTE* pByte = new BYTE[ dwSize ];
            ::ReadFile(hFile, pByte, dwSize, &dwRead, NULL);
            ::CloseHandle(hFile);
            if(dwRead != dwSize)
            {
                DUI_FREE_ARRAY(pByte);
                Release();
                return _Failed(_T("Could not read file"));
            }
            BOOL ret = LoadFromMem(pByte, dwSize, encoding);
            DUI_FREE_ARRAY(pByte);
            return ret;
        }
        else
        {
            sFile += CDuiPaintManager::GetResourceZip();
            HZIP hz = NULL;
            if(CDuiPaintManager::IsCachedResourceZip())
            {
                hz = (HZIP)CDuiPaintManager::GetResourceZipHandle();
            }
            else
            {
                CDuiString sFilePwd = CDuiPaintManager::GetResourceZipPwd();
#ifdef UNICODE
                char* pwd = w2a((wchar_t*)sFilePwd.GetData());
                hz = OpenZip(sFile.GetData(), pwd);
                DUI_FREE_ARRAY(pwd);
#else
                hz = OpenZip(sFile.GetData(), sFilePwd.GetData());
#endif
            }
            if(hz == NULL)
            {
                return _Failed(_T("Error opening zip file"));
            }
            ZIPENTRY ze;
            int i;
            if(FindZipItem(hz, pstrFilename, true, &i, &ze) != 0)
            {
                return _Failed(_T("Could not find ziped file"));
            }
            DWORD dwSize = ze.unc_size;
            if(dwSize == 0)
            {
                return _Failed(_T("File is empty"));
            }
            if(dwSize > 4096 * 1024)
            {
                return _Failed(_T("File too large"));
            }
            BYTE* pByte = new BYTE[ dwSize ];
            int res = UnzipItem(hz, i, pByte, dwSize);
            if(res != 0x00000000 && res != 0x00000600)
            {
                DUI_FREE_ARRAY(pByte);
                if(!CDuiPaintManager::IsCachedResourceZip())
                {
                    CloseZip(hz);
                }
                return _Failed(_T("Could not unzip file"));
            }
            if(!CDuiPaintManager::IsCachedResourceZip())
            {
                CloseZip(hz);
            }
            BOOL ret = LoadFromMem(pByte, dwSize, encoding);
            DUI_FREE_ARRAY(pByte);
            return ret;
        }
    }

    void CDuiMarkup::Release()
    {
        if(m_pstrXML != NULL)
        {
            free(m_pstrXML);
            m_pstrXML = NULL;
        }
        if(m_pElements != NULL)
        {
            free(m_pElements);
            m_pElements = NULL;
        }
        m_nElements = 0 ;
    }

    BOOL CDuiMarkup::IsValid() const
    {
        return m_pElements != NULL;
    }

    void CDuiMarkup::SetPreserveWhitespace(BOOL bPreserve /*= TRUE*/)
    {
        m_bPreserveWhitespace = bPreserve;
    }

    void CDuiMarkup::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
    {
        _tcsncpy(pstrMessage, m_szErrorMsg, cchMax);
    }

    void CDuiMarkup::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
    {
        _tcsncpy(pstrSource, m_szErrorXML, cchMax);
    }

    CDuiMarkupNode CDuiMarkup::GetRoot()
    {
        if(m_nElements == 0)
        {
            return CDuiMarkupNode();
        }
        return CDuiMarkupNode(this, 1);
    }

    BOOL CDuiMarkup::_Parse()
    {
        _ReserveElement(); // Reserve index 0 for errors
        ::ZeroMemory(m_szErrorMsg, sizeof(m_szErrorMsg));
        ::ZeroMemory(m_szErrorXML, sizeof(m_szErrorXML));
        LPTSTR pstrXML = m_pstrXML;
        return _Parse(pstrXML, 0);
    }

    BOOL CDuiMarkup::_Parse(LPTSTR& pstrText, size_t iParent)
    {
        _SkipWhitespace(pstrText);
        size_t iPrevious = 0;
        while(TRUE)
        {
            if(*pstrText == _T('\0') && iParent <= 1)
            {
                return TRUE;
            }
            _SkipWhitespace(pstrText);
            if(*pstrText != _T('<'))
            {
                return _Failed(_T("Expected start tag"), pstrText);
            }
            if(pstrText[1] == _T('/'))
            {
                return TRUE;
            }
            *pstrText++ = _T('\0');
            _SkipWhitespace(pstrText);
            // Skip comment or processing directive
            if(*pstrText == _T('!') || *pstrText == _T('?'))
            {
                TCHAR ch = *pstrText;
                if(*pstrText == _T('!'))
                {
                    ch = _T('-');
                }
                while(*pstrText != _T('\0') && !(*pstrText == ch && *(pstrText + 1) == _T('>')))
                {
                    pstrText = ::CharNext(pstrText);
                }
                if(*pstrText != _T('\0'))
                {
                    pstrText += 2;
                }
                _SkipWhitespace(pstrText);
                continue;
            }
            _SkipWhitespace(pstrText);
            // Fill out element structure
            XMLELEMENT* pEl = _ReserveElement();
            size_t iPos = pEl - m_pElements;
            pEl->iStart = pstrText - m_pstrXML;
            pEl->iParent = iParent;
            pEl->iNext = pEl->iChild = 0;
            if(iPrevious != 0)
            {
                m_pElements[iPrevious].iNext = iPos;
            }
            else if(iParent > 0)
            {
                m_pElements[iParent].iChild = iPos;
            }
            iPrevious = iPos;
            // Parse name
            LPCTSTR pstrName = pstrText;
            _SkipIdentifier(pstrText);
            LPTSTR pstrNameEnd = pstrText;
            if(*pstrText == _T('\0'))
            {
                return _Failed(_T("Error parsing element name"), pstrText);
            }
            // Parse attributes
            if(!_ParseAttributes(pstrText))
            {
                return FALSE;
            }
            _SkipWhitespace(pstrText);
            if(pstrText[0] == _T('/') && pstrText[1] == _T('>'))
            {
                pEl->iData = pstrText - m_pstrXML;
                *pstrText = _T('\0');
                pstrText += 2;
            }
            else
            {
                if(*pstrText != _T('>'))
                {
                    return _Failed(_T("Expected start-tag closing"), pstrText);
                }
                // Parse node data
                pEl->iData = ++pstrText - m_pstrXML;
                LPTSTR pstrDest = pstrText;
                if(!_ParseData(pstrText, pstrDest, _T('<')))
                {
                    return FALSE;
                }
                // Determine type of next element
                if(*pstrText == _T('\0') && iParent <= 1)
                {
                    return TRUE;
                }
                if(*pstrText != _T('<'))
                {
                    return _Failed(_T("Expected end-tag start"), pstrText);
                }
                if(pstrText[0] == _T('<') && pstrText[1] != _T('/'))
                {
                    if(!_Parse(pstrText, iPos))
                    {
                        return false;
                    }
                }
                if(pstrText[0] == _T('<') && pstrText[1] == _T('/'))
                {
                    *pstrDest = _T('\0');
                    *pstrText = _T('\0');
                    pstrText += 2;
                    _SkipWhitespace(pstrText);
                    SIZE_T cchName = pstrNameEnd - pstrName;
                    if(_tcsncmp(pstrText, pstrName, cchName) != 0)
                    {
                        return _Failed(_T("Unmatched closing tag"), pstrText);
                    }
                    pstrText += cchName;
                    _SkipWhitespace(pstrText);
                    if(*pstrText++ != _T('>'))
                    {
                        return _Failed(_T("Unmatched closing tag"), pstrText);
                    }
                }
            }
            *pstrNameEnd = _T('\0');
            _SkipWhitespace(pstrText);
        }
    }

    CDuiMarkup::XMLELEMENT* CDuiMarkup::_ReserveElement()
    {
        if(m_nElements == 0)
        {
            m_nReservedElements = 0;
        }
        if(m_nElements >= m_nReservedElements)
        {
            m_nReservedElements += (m_nReservedElements / 2) + 500;
            m_pElements = static_cast<XMLELEMENT*>(realloc(m_pElements, m_nReservedElements * sizeof(XMLELEMENT)));
        }
        return &m_pElements[m_nElements++];
    }

    void CDuiMarkup::_SkipWhitespace(LPTSTR& pstr) const
    {
        while(*pstr > _T('\0') && *pstr <= _T(' '))
        {
            pstr = ::CharNext(pstr);
        }
    }

    void CDuiMarkup::_SkipWhitespace(LPCTSTR& pstr) const
    {
        while(*pstr > _T('\0') && *pstr <= _T(' '))
        {
            pstr = ::CharNext(pstr);
        }
    }

    void CDuiMarkup::_SkipIdentifier(LPTSTR& pstr) const
    {
        // 属性只能用英文，所以这样处理没有问题
        while(*pstr != _T('\0') && (*pstr == _T('_') || *pstr == _T(':') || _istalnum(*pstr)))
        {
            pstr = ::CharNext(pstr);
        }
    }

    void CDuiMarkup::_SkipIdentifier(LPCTSTR& pstr) const
    {
        // 属性只能用英文，所以这样处理没有问题
        while(*pstr != _T('\0') && (*pstr == _T('_') || *pstr == _T(':') || _istalnum(*pstr)))
        {
            pstr = ::CharNext(pstr);
        }
    }

    BOOL CDuiMarkup::_ParseData(LPTSTR& pstrText, LPTSTR& pstrData, char cEnd)
    {
        while(*pstrText != _T('\0') && *pstrText != cEnd)
        {
            if(*pstrText == _T('&'))
            {
                while(*pstrText == _T('&'))
                {
                    _ParseMetaChar(++pstrText, pstrData);
                }
                if(*pstrText == cEnd)
                {
                    break;
                }
            }
            if(*pstrText == _T(' '))
            {
                *pstrData++ = *pstrText++;
                if(!m_bPreserveWhitespace)
                {
                    _SkipWhitespace(pstrText);
                }
            }
            else
            {
                LPTSTR pstrTemp = ::CharNext(pstrText);
                while(pstrText < pstrTemp)
                {
                    *pstrData++ = *pstrText++;
                }
            }
        }
        // Make sure that MapAttributes() works correctly when it parses
        // over a value that has been transformed.
        LPTSTR pstrFill = pstrData + 1;
        while(pstrFill < pstrText)
        {
            *pstrFill++ = _T(' ');
        }
        return TRUE;
    }

    void CDuiMarkup::_ParseMetaChar(LPTSTR& pstrText, LPTSTR& pstrDest)
    {
        if(pstrText[0] == _T('a') && pstrText[1] == _T('m') && pstrText[2] == _T('p') && pstrText[3] == _T(';'))
        {
            *pstrDest++ = _T('&');
            pstrText += 4;
        }
        else if(pstrText[0] == _T('l') && pstrText[1] == _T('t') && pstrText[2] == _T(';'))
        {
            *pstrDest++ = _T('<');
            pstrText += 3;
        }
        else if(pstrText[0] == _T('g') && pstrText[1] == _T('t') && pstrText[2] == _T(';'))
        {
            *pstrDest++ = _T('>');
            pstrText += 3;
        }
        else if(pstrText[0] == _T('q') && pstrText[1] == _T('u') && pstrText[2] == _T('o') && pstrText[3] == _T('t') && pstrText[4] == _T(';'))
        {
            *pstrDest++ = _T('\"');
            pstrText += 5;
        }
        else if(pstrText[0] == _T('a') && pstrText[1] == _T('p') && pstrText[2] == _T('o') && pstrText[3] == _T('s') && pstrText[4] == _T(';'))
        {
            *pstrDest++ = _T('\'');
            pstrText += 5;
        }
        else
        {
            *pstrDest++ = _T('&');
        }
    }

    BOOL CDuiMarkup::_ParseAttributes(LPTSTR& pstrText)
    {
        // 无属性
        LPTSTR pstrIdentifier = pstrText;
        if(*pstrIdentifier == _T('/') && *++pstrIdentifier == _T('>'))
        {
            return TRUE;
        }
        if(*pstrText == _T('>'))
        {
            return TRUE;
        }
        *pstrText++ = _T('\0');
        _SkipWhitespace(pstrText);
        while(*pstrText != _T('\0') && *pstrText != _T('>') && *pstrText != _T('/'))
        {
            _SkipIdentifier(pstrText);
            LPTSTR pstrIdentifierEnd = pstrText;
            _SkipWhitespace(pstrText);
            if(*pstrText != _T('='))
            {
                return _Failed(_T("Error while parsing attributes"), pstrText);
            }
            *pstrText++ = _T(' ');
            *pstrIdentifierEnd = _T('\0');
            _SkipWhitespace(pstrText);
            if(*pstrText++ != _T('\"'))
            {
                return _Failed(_T("Expected attribute value"), pstrText);
            }
            LPTSTR pstrDest = pstrText;
            if(!_ParseData(pstrText, pstrDest, _T('\"')))
            {
                return FALSE;
            }
            if(*pstrText == _T('\0'))
            {
                return _Failed(_T("Error while parsing attribute string"), pstrText);
            }
            *pstrDest = _T('\0');
            if(pstrText != pstrDest)
            {
                *pstrText = _T(' ');
            }
            pstrText++;
            _SkipWhitespace(pstrText);
        }
        return TRUE;
    }

    BOOL CDuiMarkup::_Failed(LPCTSTR pstrError, LPCTSTR pstrLocation /*= NULL*/)
    {
        // Register last error
        DUI_TRACE("XML Error: %s", pstrError);
        if(pstrLocation != NULL)
        {
            DUI_TRACE("%s", pstrLocation);
        }
        _tcsncpy(m_szErrorMsg, pstrError, (sizeof(m_szErrorMsg) / sizeof(m_szErrorMsg[0])) - 1);
        _tcsncpy(m_szErrorXML, pstrLocation != NULL ? pstrLocation : _T(""), SIZEOF_ARRAY(m_szErrorXML) - 1);
        return FALSE;
    }

}	// namespace DuiLib