//-----------------------------------------------------------------------------
#if !defined(ARX__PICKDMXDOCKBAR_H__20040113_145249)
#define ARX__PICKDMXDOCKBAR_H__20040113_145249

//-----------------------------------------------------------------------------
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"
#include "PickXdmDmx.h"
#include "JD_Center.h"

//-----------------------------------------------------------------------------
class PickDmxDockBar : public CAdUiDockControlBar {
	DECLARE_DYNAMIC(PickDmxDockBar)

public:
	PickDmxDockBar () ;
    PickXdmDmx *PickZdmDmxDlg;
	//{{AFX_VIRTUAL(PickDmxDockBar)
	public:
	virtual BOOL Create(CWnd*pParent, LPCTSTR lpszTitle);
	JD_CENTER *m_pm;
//	int m_irxd;//线路性质,0-一线，《0左线，》0右线
//	CString m_RoadName;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(PickDmxDockBar)
	afx_msg int OnCreate (LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	virtual void SizeChanged (CRect *lpRect, BOOL bFloating, int flags);
} ;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //----- !defined(ARX__PICKDMXDOCKBAR_H__20040113_145249)
