#include "StdAfx.h"
#include "DuiMarkupNode.h"

namespace DuiLib
{


    CDuiMarkupNode::CDuiMarkupNode(CDuiMarkup* pOwner, size_t iPos)
        : m_iPos(iPos)
        , m_nAttributes(0)
        , m_pOwner(pOwner)
    {
        ZeroMemory(m_aAttributes, sizeof(m_aAttributes));
    }

    CDuiMarkupNode::CDuiMarkupNode()
        : m_iPos(0)
        , m_nAttributes(0)
        , m_pOwner(NULL)
    {
        ZeroMemory(m_aAttributes, sizeof(m_aAttributes));
    }

    CDuiMarkupNode::~CDuiMarkupNode(void)
    {
    }

    BOOL CDuiMarkupNode::IsValid() const
    {
        return m_pOwner != NULL;
    }



    CDuiMarkupNode CDuiMarkupNode::GetParent()
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return CDuiMarkupNode();
        }
        size_t iPos = m_pOwner->m_pElements[m_iPos].iParent;
        if(iPos == 0)
        {
            return CDuiMarkupNode();
        }
        return CDuiMarkupNode(m_pOwner, iPos);
    }

    CDuiMarkupNode CDuiMarkupNode::GetSibling()
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return CDuiMarkupNode();
        }
        size_t iPos = m_pOwner->m_pElements[m_iPos].iNext;
        if(iPos == 0)
        {
            return CDuiMarkupNode();
        }
        return CDuiMarkupNode(m_pOwner, iPos);
    }

    CDuiMarkupNode CDuiMarkupNode::GetChild()
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return CDuiMarkupNode();
        }
        size_t iPos = m_pOwner->m_pElements[m_iPos].iChild;
        if(iPos == 0)
        {
            return CDuiMarkupNode();
        }
        return CDuiMarkupNode(m_pOwner, iPos);
    }

    CDuiMarkupNode CDuiMarkupNode::GetChild(LPCTSTR pstrName)
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return CDuiMarkupNode();
        }
        size_t iPos = m_pOwner->m_pElements[m_iPos].iChild;
        while(iPos != 0)
        {
            if(_tcsicmp(m_pOwner->m_pstrXML + m_pOwner->m_pElements[iPos].iStart, pstrName) == 0)
            {
                return CDuiMarkupNode(m_pOwner, iPos);
            }
            iPos = m_pOwner->m_pElements[iPos].iNext;
        }
        return CDuiMarkupNode();
    }

    BOOL CDuiMarkupNode::HasSiblings() const
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return FALSE;
        }
        return m_pOwner->m_pElements[m_iPos].iNext > 0;
    }

    BOOL CDuiMarkupNode::HasChildren() const
    {
        if(m_pOwner == NULL || m_pOwner->m_pElements == NULL)
        {
            return FALSE;
        }
        return m_pOwner->m_pElements[m_iPos].iChild != 0;
    }

    LPCTSTR CDuiMarkupNode::GetName() const
    {
        if(m_pOwner == NULL || m_pOwner->m_pstrXML == NULL || m_pOwner->m_pElements == NULL)
        {
            return NULL;
        }
        return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
    }

    LPCTSTR CDuiMarkupNode::GetValue() const
    {
        if(m_pOwner == NULL || m_pOwner->m_pstrXML == NULL || m_pOwner->m_pElements == NULL)
        {
            return NULL;
        }
        return m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
    }

    BOOL CDuiMarkupNode::HasAttributes()
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        return m_nAttributes > 0;
    }

    BOOL CDuiMarkupNode::HasAttribute(LPCTSTR pstrName)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        for(int i = 0; i < m_nAttributes; i++)
        {
            if(_tcsicmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
            {
                return TRUE;
            }
        }
        return FALSE;
    }

    int CDuiMarkupNode::GetAttributeCount()
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        return m_nAttributes;
    }

    LPCTSTR CDuiMarkupNode::GetAttributeName(int iIndex)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        if(iIndex < 0 || iIndex >= m_nAttributes)
        {
            return _T("");
        }
        return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iName;
    }

    LPCTSTR CDuiMarkupNode::GetAttributeValue(int iIndex)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        if(iIndex < 0 || iIndex >= m_nAttributes)
        {
            return _T("");
        }
        return m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue;
    }

    LPCTSTR CDuiMarkupNode::GetAttributeValue(LPCTSTR pstrName)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        for(int i = 0; i < m_nAttributes; i++)
        {
            if(_tcsicmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
            {
                return m_pOwner->m_pstrXML + m_aAttributes[i].iValue;
            }
        }
        return _T("");
    }

    BOOL CDuiMarkupNode::GetAttributeValue(int iIndex, LPTSTR pstrValue, SIZE_T cchMax)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        if(iIndex < 0 || iIndex >= m_nAttributes)
        {
            return FALSE;
        }
        _tcsncpy(pstrValue, m_pOwner->m_pstrXML + m_aAttributes[iIndex].iValue, cchMax);
        return TRUE;
    }

    BOOL CDuiMarkupNode::GetAttributeValue(LPCTSTR pstrName, LPTSTR pstrValue, SIZE_T cchMax)
    {
        if(m_pOwner == NULL)
        {
            return FALSE;
        }
        if(m_nAttributes == 0)
        {
            _MapAttributes();
        }
        for(int i = 0; i < m_nAttributes; i++)
        {
            if(_tcsicmp(m_pOwner->m_pstrXML + m_aAttributes[i].iName, pstrName) == 0)
            {
                _tcsncpy(pstrValue, m_pOwner->m_pstrXML + m_aAttributes[i].iValue, cchMax);
                return TRUE;
            }
        }
        return FALSE;
    }

    void CDuiMarkupNode::_MapAttributes()
    {
        m_nAttributes = 0;
        LPCTSTR pstr = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iStart;
        LPCTSTR pstrEnd = m_pOwner->m_pstrXML + m_pOwner->m_pElements[m_iPos].iData;
        pstr += _tcslen(pstr) + 1;
        while(pstr < pstrEnd)
        {
            m_pOwner->_SkipWhitespace(pstr);
            m_aAttributes[m_nAttributes].iName = pstr - m_pOwner->m_pstrXML;
            pstr += _tcslen(pstr) + 1;
            m_pOwner->_SkipWhitespace(pstr);
            if(*pstr++ != _T('\"'))
            {
                return; // if( *pstr != _T('\"') ) { pstr = ::CharNext(pstr); return; }
            }
            m_aAttributes[m_nAttributes++].iValue = pstr - m_pOwner->m_pstrXML;
            if(m_nAttributes >= MAX_XML_ATTRIBUTES)
            {
                return;
            }
            pstr += _tcslen(pstr) + 1;
        }
    }

}
