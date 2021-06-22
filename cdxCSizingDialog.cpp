// cdxCSizingDialog.cpp: implementation of the cdxCSizingPropSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cdxCSizingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdxCSizingDialog dialog
/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(cdxCSizingDialog,CDialog);

/////////////////////////////////////////////////////////////////////////////
// construction
/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(cdxCSizingDialog, CDialog)
	//{{AFX_MSG_MAP(cdxCSizingDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdxCSizingDialog message handlers
/////////////////////////////////////////////////////////////////////////////

/*
 * set up dialog; this function MUST be called before you add any controls
 * NOTE: The real OnInitDialog() virtual function calls this one by using
 * DEF_STRETCH as a iAddSzPercent.
 * This value automatically grows the size of the dialog (therefore it
 * won't appear in minimum height what I assume to be worse looking).
 * Call it with a value of 0 to make it as small as you designed it
 * in the resource editor.
 */

BOOL cdxCSizingDialog::OnInitDialog(UINT iAddSzPercent, Freedom fd, bool bMkIcon)
{
	BOOL	b	=	CDialog::OnInitDialog();

	ASSERT(::IsWindow(m_hWnd));

	cdxCDynamicControlsManager::DoInitWindow(*this,fd,bMkIcon);
	if(iAddSzPercent)
		StretchWindow((int)iAddSzPercent);
	return b;
}

/*
 * perform window sizing and adaption of controls
 */

void cdxCSizingDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	cdxCDynamicControlsManager::DoOnSize(nType,cx,cy);

	CRect	rect;
	GetClientRect(&rect);
}

/*
 * tell system what minimum size we have
 */

void cdxCSizingDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CDialog::OnGetMinMaxInfo(lpMMI);
	cdxCDynamicControlsManager::DoOnGetMinMaxInfo(lpMMI);
}

/////////////////////////////////////////////////////////////////////////////
// Additional utilities
/////////////////////////////////////////////////////////////////////////////

/*
 * pump all messages for this window
 * call it during lengthy operations to ensure
 * sizing takes place
 */

void cdxCSizingDialog::PumpMessages(void)
{
	// Must call Create() before using the dialog
	ASSERT(::IsWindow(m_hWnd));

	MSG msg;
	// Handle dialog messages for all windows 
	while(::PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE))
	{
		if(!CDialog::IsDialogMessage(&msg))		// not used by our dialog
		{
			::TranslateMessage(&msg);				// pass to application
			::DispatchMessage(&msg);  
		}
	}
}


