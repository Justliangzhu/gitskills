
#if !defined(AFX_CDXCSIZINGDIALOG_H__1D0F68C2_BF90_11D1_A589_444553540000__INCLUDED_)
#define AFX_CDXCSIZINGDIALOG_H__1D0F68C2_BF90_11D1_A589_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//
// cdxCSizingDialog.h : header file
// -----------------------------------------------------------------------
// Author:  Hans Bühler (hans.buehler@student.hu-berlin.de)
//          codex design (http://www-pool.mathematik.hu-berlin.de/~codex
// Version: 1.4
// Release: 3 (Jan 1999 to www.codeguru.com)
// -----------------------------------------------------------------------
// Notes to changes for release 2 (V1.3):
// -	Now uses cdxCDynamicControlsManager which was
//		cdxCSizingDialog::ControlInfo::Header before and had been extended
//		to handle all kinds of windows with controls.
//	-	Since being derived from both CDialog and cdxCDynamicControlsManager
//		it now allows to set custom minimum and maximum sizes :)
//	-	ReorganizeControls() always returns true now.
// -	StretchWindow() can now smallen the window, too.
//		Returns an error if the window would be too small.
// -	You can now easily make several controls act equally (they remain at
//		the same position and size).
//		To implement this, add the first control using AddSzControlEx() or any
//		of its short-cuts, get the returned cdxCDynamicControlsManager::ControlPosition
//		object and use its Add() member function to add the other controls.
// -	cdxCSizingDialog is now DECLAREed_DYNAMIC.
// -	RestoreWindowPosition() now allows to restore the visible state
//		what was infuctional before.
//		Note that the calling convention changed !!!!
// Changes to Rel.3 (V1.4):
// - Destructor no longer calls a virtual function.
// -----------------------------------------------------------------------
// Comments welcome.
//
//

#include	"cdxCDynamicControlsManager.h"

/*
 * cdxCSizingDialog[CDialog,cdxCDynamicControlsManager]
 * ====================================================
 * A CDialog dervied class that implements a sizable dialog class.
 *
 * The class is derived from cdxCDynamicControlsManager, therefore it
 * mainly calls the functions of this class.
 * Note the members of cdxCDynamicControlsManager, especially
 * AddSzControlEx() and its short-cuts.

 HOW TO USE:

	-	Using the resource edit, design your dialog in its minimum size (recommended).
	-	Create a dialog class for it.
		Create OnInitDialog().
	-	Open the header file, add an #include "cdxCSizingDialog.h" and replace all
		"CDialog"s by "cdxCSizingDialog".
	-	Open the implementation file, and replace all
		"CDialog"s by "cdxCSizingDialog".
	-	Open the constructor of your class where it calls the constructor
		of cdxCSizingDialog and set the Freedom and bMkIcon parameters
		to your preferred values.
	-	Open OnInitDialog().
		-	Ensure that you first call cdxCSizingDialog::OnInitDialog().
			Note that by default the dialog will be opened with 110% of its initial
			size. To change this, pass value different from 10 to OnInitDialog.
			Use 0 if you don't want the dialog to be resized.
		-	For each control that should change its pos/size when the window size
			changes, you need a "Control"-member variable (use the classwizard).
			Use AddSzControl(control_member,...) to make you controls being
			dynamically sizable.
			See cdxCDynamicControlsManager::AddSzControl() for further information.

 NOTES:

  -	If you overwrize OnSize() or OnGetMinMaxInfo(), don't forget to call the
		base-class functions.

 FURTHER UTILITIES:

 - PumpMessages() empties the windows message queue.
   Use it in lengthy operations to ensure the window will react
	on user size changes.
 - Use StoreWindowPosition() and RestoreWindowPosition() to save and
   load your window's size and position from the registry.
 - Use cdxCDynamicControlsManager::SetMinMaxSizes() to set the minimum and
	maximum sizes of the dialog.

 *
 * (w)Mar.1998 Hans Bühler, codex design (release 1)
 * (w)Oct.1998 Hans Bühler, codex design (release 2)
 *    hans.buehler@student.hu-berlin.de;
 *    posted to www.codeguru.com
 *    This is freeware; you use this on your own risk.
 */

class cdxCSizingDialog : public CDialog, public cdxCDynamicControlsManager
{
	DECLARE_DYNAMIC(cdxCSizingDialog);

protected:
	enum		// kept here for compatibility
	{
		DEF_STRETCH	=	10,
		exIgnore		=	0,
		exMaximum	=	100
	};

	//
	// members
	//

private:
	Freedom	m_InitFreedom;		// these are needed for compatibility to older
	bool		m_bMkIcon;			// versions of cdxCSizingDialog

protected:
	cdxCSizingDialog(UINT idd, CWnd* pParent = NULL, Freedom fd = fdAll, bool bSizeIcon = true) : CDialog(idd, pParent), m_InitFreedom(fd), m_bMkIcon(bSizeIcon) { ASSERT(idd != 0); }
public:
	virtual ~cdxCSizingDialog() { cdxCSizingDialog::DestroyWindow(); }

	//
	// reapply window size - ALWAYS returns true now
	//
public:
	bool ReorganizeControls(void) { cdxCDynamicControlsManager::ReorganizeControls(); return true; }
    virtual BOOL DestroyWindow(void) { cdxCDynamicControlsManager::DoDestroyWindow(); return CDialog::DestroyWindow(); }
	//
	// process all messages of the dialog
	//
protected:
	void PumpMessages(void);
	bool RestoreWindowPosition(LPCTSTR lpszProfile, UINT restoreFlags = rflg_none) { return cdxCDynamicControlsManager::RestoreWindowPosition(lpszProfile,restoreFlags); }
	bool StoreWindowPosition(LPCTSTR lpszProfile) { return cdxCDynamicControlsManager::StoreWindowPosition(lpszProfile); }

	//
	// dialog stuff
	//
protected:
	BOOL OnInitDialog(UINT iAddSzPercent, Freedom fd, bool bMkIcon);

	virtual BOOL OnInitDialog() { return OnInitDialog(10,m_InitFreedom,m_bMkIcon); }
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

	//
	// obsolete - kept for being compatible
	//
protected:
	BOOL OnInitDialog(UINT iAddSzPercent) { return OnInitDialog(iAddSzPercent,m_InitFreedom,m_bMkIcon); }
public:
	bool RestoreWindowPosition(LPCTSTR lpszProfile, bool bWithState) { return cdxCDynamicControlsManager::RestoreWindowPosition(lpszProfile,bWithState ? rflg_visibility : rflg_none); }
	bool RestoreWindowPosition(UINT idsProfile, bool bWithState = false) { CString s;s.LoadString(idsProfile); return cdxCDynamicControlsManager::RestoreWindowPosition(s,bWithState ? rflg_visibility : rflg_none); }
	bool StoreWindowPosition(UINT idsProfile) { CString s;s.LoadString(idsProfile); return cdxCDynamicControlsManager::StoreWindowPosition(s); }
};

#endif // !defined(AFX_CDXCSIZINGDIALOG_H__1D0F68C2_BF90_11D1_A589_444553540000__INCLUDED_)
