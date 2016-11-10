#include "StdAfx.h"
#include "DuiDlgBuilder.h"


namespace DuiLib
{


    CDuiDlgBuilder::CDuiDlgBuilder(void)
        : m_pManager(NULL)
        , m_pCallback(NULL)
        , m_pstrtype(NULL)
    {
    }


    CDuiDlgBuilder::~CDuiDlgBuilder(void)
    {
    }

    void CDuiDlgBuilder::GetLastErrorMessage(LPTSTR pstrMessage, SIZE_T cchMax) const
    {
        return m_xml.GetLastErrorMessage(pstrMessage, cchMax);
    }

    void CDuiDlgBuilder::GetLastErrorLocation(LPTSTR pstrSource, SIZE_T cchMax) const
    {
        return m_xml.GetLastErrorLocation(pstrSource, cchMax);
    }

    CDuiControl* DuiLib::CDuiDlgBuilder::Create(STRINGorID xml, LPCTSTR type /*= NULL*/, IDialogBuilderCallback* pCallback /*= NULL*/, CDuiPaintManager* pManager /*= NULL*/, CDuiControl* pParent /*= NULL*/)
    {
        m_pstrtype = type;
        m_pManager = pManager;
        m_pCallback = pCallback;
        //资源ID为0-65535，两个字节；字符串指针为4个字节
        //字符串以<开头认为是XML字符串，否则认为是XML文件
        if(HIWORD(xml.m_lpstr) != NULL)
        {
            if(*(xml.m_lpstr) == _T('<'))
            {
                if(!m_xml.Load(xml.m_lpstr))
                {
                    return NULL;
                }
            }
            else
            {
                if(!m_xml.LoadFromFile(xml.m_lpstr))
                {
                    return NULL;
                }
            }
        }
        else
        {
            HINSTANCE hInstence = CDuiPaintManager::GetResourceDll();
            if(hInstence == NULL)
            {
                return NULL;
            }
            HRSRC hResource = ::FindResource(hInstence, xml.m_lpstr, type);
            if(hResource == NULL)
            {
                return NULL;
            }
            HGLOBAL hGlobal = ::LoadResource(hInstence, hResource);
            if(hGlobal == NULL)
            {
                FreeResource(hResource);
                return NULL;
            }
            if(!m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(hInstence, hResource)))
            {
                ::FreeResource(hResource);
                return NULL;
            }
            ::FreeResource(hResource);
        }
        return Create(pParent);
    }

    CDuiControl* DuiLib::CDuiDlgBuilder::Create(CDuiControl* pParent)
    {
        CDuiMarkupNode root = m_xml.GetRoot();
        if(!root.IsValid())
        {
            return NULL;
        }
        if(m_pManager)
        {
            LPCTSTR pstrClass = NULL;
            int nAttributes = 0;
            LPCTSTR pstrName = NULL;
            LPCTSTR pstrValue = NULL;
            LPTSTR pstr = NULL;
            for(CDuiMarkupNode node = root.GetChild() ; node.IsValid(); node = node.GetSibling())
            {
                pstrClass = node.GetName();
                if(_tcsicmp(pstrClass, _T("Image")) == 0)
                {
                    nAttributes = node.GetAttributeCount();
                    LPCTSTR pImageName = NULL;
                    LPCTSTR pImageResType = NULL;
                    BOOL shared = FALSE;
                    DWORD mask = 0;
                    for(int i = 0; i < nAttributes; i++)
                    {
                        pstrName = node.GetAttributeName(i);
                        pstrValue = node.GetAttributeValue(i);
                        if(_tcsicmp(pstrName, _T("name")) == 0)
                        {
                            pImageName = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("restype")) == 0)
                        {
                            pImageResType = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("mask")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            mask = _tcstoul(pstrValue, &pstr, 16);
                        }
                        else if(_tcsicmp(pstrName, _T("shared")) == 0)
                        {
                            shared = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                    }
                    if(pImageName)
                    {
                        m_pManager->AddImage(pImageName, pImageResType, mask, FALSE, shared);
                    }
                }
                else if(_tcsicmp(pstrClass, _T("Font")) == 0)
                {
                    nAttributes = node.GetAttributeCount();
                    int id = -1;
                    LPCTSTR pFontName = NULL;
                    int size = 12;
                    BOOL bold = FALSE;
                    BOOL underline = FALSE;
                    BOOL italic = FALSE;
                    BOOL defaultfont = FALSE;
                    BOOL shared = FALSE;
                    for(int i = 0; i < nAttributes; i++)
                    {
                        pstrName = node.GetAttributeName(i);
                        pstrValue = node.GetAttributeValue(i);
                        if(_tcsicmp(pstrName, _T("id")) == 0)
                        {
                            id = _tcstol(pstrValue, &pstr, 10);
                        }
                        else if(_tcsicmp(pstrName, _T("name")) == 0)
                        {
                            pFontName = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("size")) == 0)
                        {
                            size = _tcstol(pstrValue, &pstr, 10);
                        }
                        else if(_tcsicmp(pstrName, _T("bold")) == 0)
                        {
                            bold = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("underline")) == 0)
                        {
                            underline = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("italic")) == 0)
                        {
                            italic = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("default")) == 0)
                        {
                            defaultfont = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("shared")) == 0)
                        {
                            shared = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                    }
                    if(id >= 0)
                    {
                        m_pManager->AddFont(id, pFontName, size, bold, underline, italic, shared);
                        if(defaultfont)
                        {
                            m_pManager->SetDefaultFont(pFontName, m_pManager->GetDPIObj()->Scale(size), bold, underline, italic, shared);
                        }
                    }
                }
                else if(_tcsicmp(pstrClass, _T("Default")) == 0)
                {
                    nAttributes = node.GetAttributeCount();
                    LPCTSTR pControlName = NULL;
                    LPCTSTR pControlValue = NULL;
                    BOOL shared = FALSE;
                    for(int i = 0; i < nAttributes; i++)
                    {
                        pstrName = node.GetAttributeName(i);
                        pstrValue = node.GetAttributeValue(i);
                        if(_tcsicmp(pstrName, _T("name")) == 0)
                        {
                            pControlName = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("value")) == 0)
                        {
                            pControlValue = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("shared")) == 0)
                        {
                            shared = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                    }
                    if(pControlName)
                    {
                        m_pManager->AddDefaultAttributeList(pControlName, pControlValue, shared);
                    }
                }
                else if(_tcsicmp(pstrClass, _T("Style")) == 0)
                {
                    nAttributes = node.GetAttributeCount();
                    LPCTSTR pName = NULL;
                    LPCTSTR pStyle = NULL;
                    BOOL shared = FALSE;
                    for(int i = 0; i < nAttributes; i++)
                    {
                        pstrName = node.GetAttributeName(i);
                        pstrValue = node.GetAttributeValue(i);
                        if(_tcsicmp(pstrName, _T("name")) == 0)
                        {
                            pName = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("value")) == 0)
                        {
                            pStyle = pstrValue;
                        }
                        else if(_tcsicmp(pstrName, _T("shared")) == 0)
                        {
                            shared = (_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                    }
                    if(pName)
                    {
                        m_pManager->AddStyle(pName, pStyle, shared);
                    }
                }
            }
            pstrClass = root.GetName();
            if(_tcsicmp(pstrClass, _T("Window")) == 0)
            {
                if(m_pManager->GetPaintWindow())
                {
                    int nAttributes = root.GetAttributeCount();
                    for(int i = 0; i < nAttributes; i++)
                    {
                        pstrName = root.GetAttributeName(i);
                        pstrValue = root.GetAttributeValue(i);
                        if(_tcsicmp(pstrName, _T("size")) == 0)
                        {
                            int cx = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            int cy = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetInitSize(m_pManager->GetDPIObj()->Scale(cx), m_pManager->GetDPIObj()->Scale(cy));
                        }
                        else if(_tcsicmp(pstrName, _T("sizebox")) == 0)
                        {
                            RECT rcSizeBox = { 0 };
                            LPTSTR pstr = NULL;
                            rcSizeBox.left = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            rcSizeBox.top = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcSizeBox.right = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcSizeBox.bottom = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetSizeBox(rcSizeBox);
                        }
                        else if(_tcsicmp(pstrName, _T("caption")) == 0)
                        {
                            RECT rcCaption = { 0 };
                            LPTSTR pstr = NULL;
                            rcCaption.left = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            rcCaption.top = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcCaption.right = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcCaption.bottom = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetCaptionRect(rcCaption);
                        }
                        else if(_tcsicmp(pstrName, _T("roundcorner")) == 0)
                        {
                            LPTSTR pstr = NULL;
                            int cx = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            int cy = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetRoundCorner(cx, cy);
                        }
                        else if(_tcsicmp(pstrName, _T("mininfo")) == 0)
                        {
                            LPTSTR pstr = NULL;
                            int cx = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            int cy = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetMinInfo(cx, cy);
                        }
                        else if(_tcsicmp(pstrName, _T("maxinfo")) == 0)
                        {
                            LPTSTR pstr = NULL;
                            int cx = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            int cy = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->SetMaxInfo(cx, cy);
                        }
                        else if(_tcsicmp(pstrName, _T("showdirty")) == 0)
                        {
                            m_pManager->SetShowUpdateRect(_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("opacity")) == 0 || _tcsicmp(pstrName, _T("alpha")) == 0)
                        {
                            m_pManager->SetOpacity(_ttoi(pstrValue));
                        }
                        else if(_tcscmp(pstrName, _T("layeredopacity")) == 0)
                        {
                            m_pManager->SetLayeredOpacity(_ttoi(pstrValue));
                        }
                        else if(_tcscmp(pstrName, _T("layered")) == 0 || _tcscmp(pstrName, _T("bktrans")) == 0)
                        {
                            m_pManager->SetLayered(_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("disabledfontcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->SetDefaultDisabledColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("defaultfontcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->SetDefaultFontColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("linkfontcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->SetDefaultLinkFontColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("linkhoverfontcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->SetDefaultLinkHoverFontColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("selectedcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->SetDefaultSelectedBkColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("shadowsize")) == 0)
                        {
                            m_pManager->GetShadow()->SetSize(_ttoi(pstrValue));
                        }
                        else if(_tcsicmp(pstrName, _T("shadowsharpness")) == 0)
                        {
                            m_pManager->GetShadow()->SetSharpness(_ttoi(pstrValue));
                        }
                        else if(_tcsicmp(pstrName, _T("shadowdarkness")) == 0)
                        {
                            m_pManager->GetShadow()->SetDarkness(_ttoi(pstrValue));
                        }
                        else if(_tcsicmp(pstrName, _T("shadowposition")) == 0)
                        {
                            LPTSTR pstr = NULL;
                            int cx = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            int cy = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->GetShadow()->SetPosition(cx, cy);
                        }
                        else if(_tcsicmp(pstrName, _T("shadowcolor")) == 0)
                        {
                            if(*pstrValue == _T('#'))
                            {
                                pstrValue = ::CharNext(pstrValue);
                            }
                            LPTSTR pstr = NULL;
                            DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
                            m_pManager->GetShadow()->SetColor(clrColor);
                        }
                        else if(_tcsicmp(pstrName, _T("shadowcorner")) == 0)
                        {
                            RECT rcCorner = { 0 };
                            LPTSTR pstr = NULL;
                            rcCorner.left = _tcstol(pstrValue, &pstr, 10);
                            ASSERT(pstr);
                            rcCorner.top = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcCorner.right = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            rcCorner.bottom = _tcstol(pstr + 1, &pstr, 10);
                            ASSERT(pstr);
                            m_pManager->GetShadow()->SetShadowCorner(rcCorner);
                        }
                        else if(_tcsicmp(pstrName, _T("shadowimage")) == 0)
                        {
                            m_pManager->GetShadow()->SetImage(pstrValue);
                        }
                        else if(_tcsicmp(pstrName, _T("showshadow")) == 0)
                        {
                            m_pManager->GetShadow()->ShowShadow(_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("gdiplustext")) == 0)
                        {
                            m_pManager->SetUseGdiplusText(_tcsicmp(pstrValue, _T("true")) == 0);
                        }
                        else if(_tcsicmp(pstrName, _T("textrenderinghint")) == 0)
                        {
                            m_pManager->SetGdiplusTextRenderingHint(_ttoi(pstrValue));
                        }
                    }
                }
            }
        }
        return Parse(&root, pParent);
    }


    CDuiControl* CDuiDlgBuilder::Parse(CDuiMarkupNode* pRoot, CDuiControl* pParent)
    {
        CDuiControl* pReturn = NULL;
        for(CDuiMarkupNode node = pRoot->GetChild() ; node.IsValid(); node = node.GetSibling())
        {
            LPCTSTR pstrClass = node.GetName();
            if(_tcsicmp(pstrClass, _T("Image")) == 0 || _tcsicmp(pstrClass, _T("Font")) == 0 ||
                    _tcsicmp(pstrClass, _T("Default")) == 0 || _tcsicmp(pstrClass, _T("Style")) == 0)
            {
                continue;
            }
            CDuiControl* pControl = NULL;
            if(_tcsicmp(pstrClass, _T("Include")) == 0)
            {
                if(!node.HasAttributes())
                {
                    continue;
                }
                int count = 1;
                LPTSTR pstr = NULL;
                TCHAR szValue[500] = { 0 };
                SIZE_T cchLen = SIZEOF_ARRAY(szValue) - 1;
                if(node.GetAttributeValue(_T("count"), szValue, cchLen))
                {
                    count = _tcstol(szValue, &pstr, 10);
                }
                cchLen = SIZEOF_ARRAY(szValue) - 1;
                if(!node.GetAttributeValue(_T("source"), szValue, cchLen))
                {
                    continue;
                }
                for(int i = 0; i < count; i++)
                {
                    CDuiDlgBuilder builder;
                    if(m_pstrtype != NULL)
                    {
                        // 使用资源dll，从资源中读取
                        WORD id = (WORD)_tcstol(szValue, &pstr, 10);
                        pControl = builder.Create((UINT)id, m_pstrtype, m_pCallback, m_pManager, pParent);
                    }
                    else
                    {
                        pControl = builder.Create((LPCTSTR)szValue, (UINT)0, m_pCallback, m_pManager, pParent);
                    }
                }
                continue;
            }
            else
            {
                CDuiString strClass =  _T("CDui");
                strClass = strClass + pstrClass ;
                pControl = dynamic_cast<CDuiControl*>(CDuiControlFactory::GetInstance()->CreateControl(strClass));
                // 检查插件
                if(pControl == NULL)
                {
                    CStdPtrArray* pPlugins = CDuiPaintManager::GetPlugins();
                    LPCREATECONTROL lpCreateControl = NULL;
                    for(int i = 0; i < pPlugins->GetSize(); ++i)
                    {
                        lpCreateControl = (LPCREATECONTROL)pPlugins->GetAt(i);
                        if(lpCreateControl != NULL)
                        {
                            pControl = lpCreateControl(pstrClass);
                            if(pControl != NULL)
                            {
                                break;
                            }
                        }
                    }
                }
                // 回掉创建
                if(pControl == NULL && m_pCallback != NULL)
                {
                    pControl = m_pCallback->CreateControl(pstrClass);
                }
            }
            if(pControl == NULL)
            {
#ifdef _DEBUG
                DUI_TRACE(_T("未知控件:%s"), pstrClass);
#else
                continue;
#endif
            }
            // Add children
            if(node.HasChildren())
            {
                Parse(&node, pControl);
            }
            // Attach to parent
            // 因为某些属性和父窗口相关，比如selected，必须先Add到父窗口
            if(pParent != NULL && pControl != NULL)
            {
                IContainer* pContainer = NULL;
                if(pContainer == NULL)
                {
                    pContainer = static_cast<IContainer*>(pParent->GetInterface(_T("IContainer")));
                }
                ASSERT(pContainer);
                if(pContainer == NULL)
                {
                    return NULL;
                }
                if(!pContainer->Add(pControl))
                {
                    delete pControl;
                    continue;
                }
            }
            if(pControl == NULL)
            {
                continue;
            }
            // Init default attributes
            if(m_pManager)
            {
                pControl->SetManager(m_pManager, NULL, FALSE);
                LPCTSTR pDefaultAttributes = m_pManager->GetDefaultAttributeList(pstrClass);
                if(pDefaultAttributes)
                {
                    pControl->ApplyAttributeList(pDefaultAttributes);
                }
            }
            // Process attributes
            if(node.HasAttributes())
            {
                // Set ordinary attributes
                int nAttributes = node.GetAttributeCount();
                for(int i = 0; i < nAttributes; i++)
                {
                    pControl->SetAttribute(node.GetAttributeName(i), node.GetAttributeValue(i));
                }
            }
            // Return first item
            if(pReturn == NULL)
            {
                pReturn = pControl;
            }
        }
        return pReturn;
    }



}	//namespace DuiLib
