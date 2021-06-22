//-----------------------------------------------------------------------------
#if !defined(ARX__DOCKTYPEHDMBAR_H__20031106_162800)
#define ARX__DOCKTYPEHDMBAR_H__20031106_162800

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"
//szf #include "CXTYPEHDM.h"

//-----------------------------------------------------------------------------
class DockTypeHdmBar : public CAcUiDockControlBar {
	DECLARE_DYNAMIC(DockTypeHdmBar)

public:
	DockTypeHdmBar () ;
	~DockTypeHdmBar () ;

//szf 	CCXTypeHdm *m_TypeHdmdlg;
	JD_CENTER *m_pm;
	GTZDM_ROAD *m_zm;
	CString m_ckml;
	//{{AFX_VIRTUAL(DockTypeHdmBar)
	public:
	virtual BOOL Create(CWnd*pParent, LPCSTR lpszTitle);
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(DockTypeHdmBar)
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags);
} ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //----- !defined(ARX__DOCKTYPEHDMBAR_H__20031106_162800)
