#include "StdAfx.h"
#include "DuiResourceManager.h"

namespace DuiLib
{
    CDuiResourceManager::CDuiResourceManager(void)
        : m_pQuerypInterface(NULL)
    {
    }


    CDuiResourceManager::~CDuiResourceManager(void)
    {
        for(int i = 0; i < m_mTextResourceHashMap.GetSize(); i++)
        {
            if(LPCTSTR key = m_mTextResourceHashMap.GetAt(i))
            {
                CDuiString* lpStr = static_cast<CDuiString*>(m_mTextResourceHashMap.Find(key));
                DUI_FREE_POINT(lpStr);
            }
        }
        for(int i = 0; i < m_mImageHashMap.GetSize(); i++)
        {
            if(LPCTSTR key = m_mImageHashMap.GetAt(i))
            {
                CDuiString* lpStr = static_cast<CDuiString*>(m_mImageHashMap.Find(key));
                DUI_FREE_POINT(lpStr);
            }
        }
        for(int i = 0; i < m_mXmlHashMap.GetSize(); i++)
        {
            if(LPCTSTR key = m_mXmlHashMap.GetAt(i))
            {
                CDuiString* lpStr = static_cast<CDuiString*>(m_mXmlHashMap.Find(key));
                DUI_FREE_POINT(lpStr);
            }
        }
    }

    CDuiResourceManager* CDuiResourceManager::GetInstance()
    {
        static CDuiResourceManager* pResourceManager = new CDuiResourceManager;
        return pResourceManager;
    }

    void CDuiResourceManager::Release(void)
    {
        delete this;
    }

    LPCTSTR CDuiResourceManager::GetLanguage()
    {
        return m_sLauguage;
    }

    void CDuiResourceManager::SetLanguage(LPCTSTR pstrLanguage)
    {
        m_sLauguage = pstrLanguage;
    }

    BOOL CDuiResourceManager::LoadLanguage(LPCTSTR pstrXml)
    {
        CDuiMarkup xml;
        if(*(pstrXml) == _T('<'))
        {
            if(!xml.Load(pstrXml))
            {
                return FALSE;
            }
        }
        else
        {
            if(!xml.LoadFromFile(pstrXml))
            {
                return FALSE;
            }
        }
        CDuiMarkupNode Root = xml.GetRoot();
        if(!Root.IsValid())
        {
            return FALSE;
        }

        LPCTSTR pstrClass = NULL;
        int nAttributes = 0;
        LPCTSTR pstrName = NULL;
        LPCTSTR pstrValue = NULL;
        LPTSTR pstr = NULL;

        //加载图片资源
        LPCTSTR pstrId = NULL;
        LPCTSTR pstrText = NULL;
        for(CDuiMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling())
        {
            pstrClass = node.GetName();
            if((_tcsicmp(pstrClass, _T("Text")) == 0) && node.HasAttributes())
            {
                //加载图片资源
                nAttributes = node.GetAttributeCount();
                for(int i = 0; i < nAttributes; i++)
                {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);

                    if(_tcsicmp(pstrName, _T("id")) == 0)
                    {
                        pstrId = pstrValue;
                    }
                    else if(_tcsicmp(pstrName, _T("value")) == 0)
                    {
                        pstrText = pstrValue;
                    }
                }
                if(pstrId == NULL ||  pstrText == NULL)
                {
                    continue;
                }

                CDuiString* lpstrFind = static_cast<CDuiString*>(m_mTextResourceHashMap.Find(pstrId));
                if(lpstrFind != NULL)
                {
                    lpstrFind->Assign(pstrText);
                }
                else
                {
                    m_mTextResourceHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrText));
                }
            }
            else
            {
                continue;
            }
        }

        return TRUE;
    }

    BOOL CDuiResourceManager::LoadResource(CIdToResource xml, LPCTSTR type /*= NULL*/)
    {
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
            HRSRC hResource = ::FindResource(CDuiPaintManager::GetResourceDll(), xml.m_lpstr, type);
            if(hResource == NULL)
            {
                return NULL;
            }
            HGLOBAL hGlobal = ::LoadResource(CDuiPaintManager::GetResourceDll(), hResource);
            if(hGlobal == NULL)
            {
                FreeResource(hResource);
                return NULL;
            }

            if(!m_xml.LoadFromMem((BYTE*)::LockResource(hGlobal), ::SizeofResource(CDuiPaintManager::GetResourceDll(), hResource)))
            {
                return NULL;
            }

            ::FreeResource(hResource);
        }

        return LoadResource(m_xml.GetRoot());
    }

    BOOL CDuiResourceManager::LoadResource(CDuiMarkupNode Root)
    {
        if(!Root.IsValid())
        {
            return FALSE;
        }

        LPCTSTR pstrClass = NULL;
        int nAttributes = 0;
        LPCTSTR pstrName = NULL;
        LPCTSTR pstrValue = NULL;
        LPTSTR pstr = NULL;

        //加载图片资源
        LPCTSTR pstrId = NULL;
        LPCTSTR pstrPath = NULL;
        for(CDuiMarkupNode node = Root.GetChild() ; node.IsValid(); node = node.GetSibling())
        {
            pstrClass = node.GetName();
            CDuiMarkupNode ChildNode = node.GetChild();
            if(ChildNode.IsValid())
            {
                LoadResource(node);
            }
            else if((_tcsicmp(pstrClass, _T("Image")) == 0) && node.HasAttributes())
            {
                //加载图片资源
                nAttributes = node.GetAttributeCount();
                for(int i = 0; i < nAttributes; i++)
                {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);

                    if(_tcsicmp(pstrName, _T("id")) == 0)
                    {
                        pstrId = pstrValue;
                    }
                    else if(_tcsicmp(pstrName, _T("path")) == 0)
                    {
                        pstrPath = pstrValue;
                    }
                }
                if(pstrId == NULL ||  pstrPath == NULL)
                {
                    continue;
                }
                CDuiString* pstrFind = static_cast<CDuiString*>(m_mImageHashMap.Find(pstrId));
                if(pstrFind != NULL)
                {
                    continue;
                }
                m_mImageHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrPath));
            }
            else if(_tcsicmp(pstrClass, _T("Xml")) == 0 && node.HasAttributes())
            {
                //加载XML配置文件
                nAttributes = node.GetAttributeCount();
                for(int i = 0; i < nAttributes; i++)
                {
                    pstrName = node.GetAttributeName(i);
                    pstrValue = node.GetAttributeValue(i);

                    if(_tcsicmp(pstrName, _T("id")) == 0)
                    {
                        pstrId = pstrValue;
                    }
                    else if(_tcsicmp(pstrName, _T("path")) == 0)
                    {
                        pstrPath = pstrValue;
                    }
                }
                if(pstrId == NULL ||  pstrPath == NULL)
                {
                    continue;
                }
                CDuiString* pstrFind = static_cast<CDuiString*>(m_mXmlHashMap.Find(pstrId));
                if(pstrFind != NULL)
                {
                    continue;
                }
                m_mXmlHashMap.Insert(pstrId, (LPVOID)new CDuiString(pstrPath));
            }
            else
            {
                continue;
            }
        }
        return TRUE;
    }

    void CDuiResourceManager::SetTextQueryInterface(IQueryControlText* pInterface)
    {
        m_pQuerypInterface = pInterface;
    }

    CDuiString CDuiResourceManager::GetText(LPCTSTR lpstrId, LPCTSTR lpstrType /*= NULL*/)
    {
        if(lpstrId == NULL)
        {
            return _T("");
        }

        CDuiString* lpstrFind = static_cast<CDuiString*>(m_mTextResourceHashMap.Find(lpstrId));
        if(lpstrFind == NULL && m_pQuerypInterface)
        {
            lpstrFind = new CDuiString(m_pQuerypInterface->QueryControlText(lpstrId, lpstrType));
            m_mTextResourceHashMap.Insert(lpstrId, (LPVOID)lpstrFind);
        }
        return lpstrFind == NULL ? lpstrId : *lpstrFind;
    }

    void CDuiResourceManager::ReloadText()
    {
        if(m_pQuerypInterface == NULL)
        {
            return;
        }
        //重载文字描述
        for(int i = 0; i < m_mTextResourceHashMap.GetSize(); i++)
        {
            LPCTSTR lpstrId = m_mTextResourceHashMap.GetAt(i);
            if(lpstrId == NULL)
            {
                continue;
            }
            LPCTSTR lpstrText = m_pQuerypInterface->QueryControlText(lpstrId, NULL);
            if(lpstrText != NULL)
            {
                CDuiString* lpStr = static_cast<CDuiString*>(m_mTextResourceHashMap.Find(lpstrId));
                lpStr->Assign(lpstrText);
            }
        }
    }

    LPCTSTR CDuiResourceManager::GetImagePath(LPCTSTR lpstrId)
    {
        CDuiString* lpStr = static_cast<CDuiString*>(m_mImageHashMap.Find(lpstrId));
        return lpStr == NULL ? NULL : lpStr->GetData();
    }

    LPCTSTR CDuiResourceManager::GetXmlPath(LPCTSTR lpstrId)
    {
        CDuiString* lpStr = static_cast<CDuiString*>(m_mXmlHashMap.Find(lpstrId));
        return lpStr == NULL ? NULL : lpStr->GetData();
    }

}