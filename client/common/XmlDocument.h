// XmlParse.h: interface for the CXmlParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLDOCUMENT_H__7D272C3A_C971_4F51_98D7_09F974F994E3__INCLUDED_)
#define AFX_XMLDOCUMENT_H__7D272C3A_C971_4F51_98D7_09F974F994E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXmlElement : public CObject
{
public:
	CXmlElement() 
	{
		m_posFind = NULL;
		m_pParentElement = NULL;
	};
	virtual ~CXmlElement() 
	{
		while(!m_ChildElements.IsEmpty())
		{
			delete m_ChildElements.RemoveHead();
		}
	};

	CString m_strName;
	CString m_strAttributes;
	CString m_strData;
	
	CObList m_ChildElements;
	
	CXmlElement *m_pParentElement;

	POSITION m_posFind;
	CString m_strFind;
};

class CXmlDocument
{
public:
	CString RemoveChar(LPCTSTR szSrc,TCHAR cRemove);
	CXmlDocument();
	virtual ~CXmlDocument();

	CString Generate();
	BOOL Parse(LPCTSTR lpszString);

	BOOL Load(LPCTSTR lpszFileName);
	BOOL Store(LPCTSTR lpszFileName);

	CXmlElement *GetFirstChild(CXmlElement *pElement);
	CXmlElement *GetNextSibling(CXmlElement *pElement);
	
	CXmlElement *GetRootElement() { return &m_RootElement; }
	
	CXmlElement *FindElement(CXmlElement *pElement, LPCTSTR lpszName);
	CXmlElement *FindNextElement(CXmlElement *pElement);

	CXmlElement *AddElement(CXmlElement *pElement, LPCTSTR lpszName, LPCTSTR lpszData = NULL, LPCTSTR lpszAttributes = NULL);

	void DeleteContents();
protected:
	int m_nLevel;
	int ValidateTag(CString &strTag, CString &strResult);
	BOOL CreateTag(CXmlElement *pElement, CString &strTag);
	

	CString m_strXml;

	CXmlElement m_RootElement;
	CXmlElement *m_pCurElement;
};

#endif // !defined(AFX_XMLDOCUMENT_H__7D272C3A_C971_4F51_98D7_09F974F994E3__INCLUDED_)
