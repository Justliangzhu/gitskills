//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "PickDmxDockBar.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(PickDmxDockBar, CAdUiDockControlBar)

	BEGIN_MESSAGE_MAP(PickDmxDockBar, CAdUiDockControlBar)
		//{{AFX_MSG_MAP(PickDmxDockBar)
		ON_WM_CREATE()
		ON_WM_SIZE()
		ON_WM_PAINT()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()

	//-----------------------------------------------------------------------------
	// PickDmxDockBar *pInstance =new PickDmxDockBar ;
	// pInstance->Create (acedGetAcadFrame (), "My title bar") ;
	// pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
	// pInstance->RestoreControlBar () ;

	static CLSID clsPickDmxDockBar ={ 0x3bb4f032, 0xabd0, 0x4f19, { 0x91, 0x70, 0xf2, 0x58, 0xe4, 0x41, 0x1e, 0xf3 } } ;

	//-----------------------------------------------------------------------------
	PickDmxDockBar::PickDmxDockBar() : CAdUiDockControlBar () {
		//{{AFX_DATA_INIT(PickDmxDockBar)
		//}}AFX_DATA_INIT
	}

	BOOL PickDmxDockBar::Create(CWnd*pParent, LPCTSTR lpszTitle) {
		CString strWndClass ;
		strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
		CRect rect (0, 0, 250, 200) ;
		if ( !CAdUiDockControlBar::Create (
			strWndClass,
			lpszTitle,
			WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
			rect,
			pParent, 0
			)
			)
			return (FALSE) ;

		SetToolID (&clsPickDmxDockBar) ;

		//----- TODO: Add your code here	
		return (TRUE) ;
	}

	int PickDmxDockBar::OnCreate (LPCREATESTRUCT lpCreateStruct) {
		if ( CAdUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
			return (-1) ;

		//----- TODO: Add your code here
		if (m_pm==NULL)
		{
			return 0;
		}
		PickZdmDmxDlg = new PickXdmDmx(this,m_pm);
		// create it and set the parent as the dockctrl bar
		PickZdmDmxDlg->Create(IDD_DIALOG_DMX, this);//IDD_DIALOG_DMX
		//  move the window over so we can see the control lines
		PickZdmDmxDlg->MoveWindow (0, 0, 100, 400, TRUE);
		PickZdmDmxDlg->ShowWindow(SW_SHOW);

		return (0) ;
	}

	void PickDmxDockBar::SizeChanged (CRect *lpRect, BOOL bFloating, int flags) {
		//----- TODO: Add your code here
		if (PickZdmDmxDlg==NULL)
		{
			return;
		}
		PickZdmDmxDlg->SetWindowPos (this, lpRect->left+4, lpRect->top+4, lpRect->Width (), lpRect->Height (), SWP_NOZORDER);
	}

	void PickDmxDockBar::OnSize(UINT nType, int cx, int cy) 
	{
		CAdUiDockControlBar::OnSize(nType, cx, cy);
		if (PickZdmDmxDlg==NULL)
		{
			return;
		}
		PickZdmDmxDlg->MoveWindow (0, 0, cx, cy);
		// TODO: Add your message handler code here	
	}

	void PickDmxDockBar::OnPaint() 
	{
		CPaintDC dc(this); // device context for painting

		// TODO: Add your message handler code here
		if (PickZdmDmxDlg==NULL)
		{
			return;
		}
		PickZdmDmxDlg->UpdateData(false);

		PickZdmDmxDlg->Invalidate();

		// Do not call CAdUiDockControlBar::OnPaint() for painting messages
	}
