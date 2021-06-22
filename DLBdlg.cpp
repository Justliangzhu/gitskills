// DLBdlg.cpp : implementation file
//

#include "stdafx.h"
#include "DLBdlg.h"
#include "JD_CENTER.h"
#include "GTZDM.h"
#include "EXMANAGER.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LOGFONT font={12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial"};
COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
COLORREF fixbkclr=RGB(155,188,0);
/////////////////////////////////////////////////////////////////////////////
// DLBdlg dialog
DLBdlg::DLBdlg(CWnd* pParent /*=NULL*/)
	: CDialog(DLBdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(DLBdlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DLBdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(DLBdlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//   DDX_GridControl(pDX, IDC_LIST1,m_grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DLBdlg, CDialog)
	//{{AFX_MSG_MAP(DLBdlg)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBUTTONdel)
	ON_BN_CLICKED(IDC_BUTTON_selLine, OnBUTTONselLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLBdlg message handlers

void DLBdlg::OnBUTTONAdd() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for( i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
			}
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.AutoSize();

		m_grid.Invalidate();
		m_NDL++;
	}
}

void DLBdlg::OnBUTTONdel() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.AutoSize();
		m_grid.Invalidate();
		m_NDL--;
	}

}

void DLBdlg::OnBUTTONselLine() 
{

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//GetParent()->SetFocus();	// Give AutoCAD the focus.
	acedGetAcadDwgView()->SetFocus();

	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	Acad::ErrorStatus es = acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	rc = ads_entsel(L"\nSelect a  平面/纵面 实体 : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{   
		ads_printf(L"\nError during object selection"); 	
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog*/
		return;   
	}
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();


	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{	
		JD_CENTER *pzLine = NULL;
		m_eId = eId;
		pzLine = JD_CENTER::cast(pObj);
		m_NDL = pzLine->DLNum;
		//daicol=1;
		m_grid.SetRowCount(m_NDL+2);
		m_grid.SetFixedRowCount(1);
		m_grid.SetColumnCount(3);
		m_grid.SetFixedColumnCount(1);
		m_grid.SetTextBkColor(txtbkclr);
		//	   m_grid.SetFixedBkColor(fixbkclr);

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
			DT_END_ELLIPSIS|DT_WORDBREAK;
		Item.lfFont=font;
		for(int k=0;k<m_grid.GetColumnCount();k++)
		{
			int i;
			for( i=0;i<m_grid.GetRowCount();i++) 
			{
				Item.row = i;
				Item.col = k;
				m_grid.SetItem(&Item);
			}  
			m_grid.SetItemState(i-1, k,GVIS_READONLY);
		}

		m_grid.SetItemText(0,0,L"序 号");
		m_grid.SetItemText(0,1,L"断前里程");
		m_grid.SetItemText(0,2,L"断后里程");

		int i=0;
		ACHAR tmp[10];
		if(m_NDL !=0 )  	
		{
			for(i=0; i<m_NDL; i++)
			{
				ACHAR Sml[80],Eml[80];
				if(pzLine->DLArray[i].BLC>=1000.0)
					_stprintf(Sml,L"%s%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else if(pzLine->DLArray[i].BLC>=100.0)
					_stprintf(Sml,L"%s0%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else if(pzLine->DLArray[i].BLC>=10.0)
					_stprintf(Sml,L"%s00%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else
					_stprintf(Sml,L"%s000%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);

				putplus(Sml, '+', '.', -3);
				m_grid.SetItemText(i+1,1,Sml);

				if(pzLine->DLArray[i].ELC>=1000.0)
					_stprintf(Eml,L"%s%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else if(pzLine->DLArray[i].ELC>=100.0)
					_stprintf(Eml,L"%s0%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else if(pzLine->DLArray[i].ELC>=10.0)
					_stprintf(Eml,L"%s00%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else
					_stprintf(Eml,L"%s000%0.3lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);

				putplus(Eml, '+', '.', -3);
				m_grid.SetItemText(i+1,2,Eml); 
				_stprintf(tmp,L"%d",i+1);
				m_grid.SetItemText(i+1,0,tmp);
			}
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);

		//m_grid.SetColumnWidth(0,60);
		//m_grid.SetColumnWidth(1,95);
		//m_grid.SetColumnWidth(2,95);
		m_grid.AutoSize();
		for( i=0;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);
		//	   m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);

		pzLine->close();
	}
	else if (pObj->isKindOf(GTZDM_ROAD::desc())) //是GTZX实体,取数据
	{	
		GTZDM_ROAD *pzLine;
		m_eId = eId;
		/*acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		m_NDL = pzLine->DLNum;
		//daicol=1;
		m_grid.SetRowCount(m_NDL+2);
		m_grid.SetFixedRowCount(1);
		m_grid.SetColumnCount(3);
		m_grid.SetFixedColumnCount(1);
		m_grid.SetTextBkColor(txtbkclr);
		//	   m_grid.SetFixedBkColor(fixbkclr);

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
			DT_END_ELLIPSIS|DT_WORDBREAK;
		Item.lfFont=font;
		for(int k=0;k<m_grid.GetColumnCount();k++)
		{
			int i;
			for( i=0;i<m_grid.GetRowCount();i++) 
			{
				Item.row = i;
				Item.col = k;
				m_grid.SetItem(&Item);
			}  
			m_grid.SetItemState(i-1, k,GVIS_READONLY);
		}

		m_grid.SetItemText(0,0,L"序 号");
		m_grid.SetItemText(0,1,L"断前里程");
		m_grid.SetItemText(0,2,L"断后里程");

		int i=0;
		ACHAR tmp[10];
		if(m_NDL !=0 )  	
		{
			for(i=0; i<m_NDL; i++)
			{
				ACHAR Sml[80],Eml[80];
				if(pzLine->DLArray[i].BLC>=1000.0)
					_stprintf(Sml,L"%s%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else if(pzLine->DLArray[i].BLC>=100.0)
					_stprintf(Sml,L"%s0%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else if(pzLine->DLArray[i].BLC>=10.0)
					_stprintf(Sml,L"%s00%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);
				else
					_stprintf(Sml,L"%s000%0.2lf",pzLine->DLArray[i].BGH,pzLine->DLArray[i].BLC);

				putplus(Sml, '+', '.', -3);
				m_grid.SetItemText(i+1,1,Sml);

				if(pzLine->DLArray[i].ELC>=1000.0)
					_stprintf(Eml,L"%s%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else if(pzLine->DLArray[i].ELC>=100.0)
					_stprintf(Eml,L"%s0%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else if(pzLine->DLArray[i].ELC>=10.0)
					_stprintf(Eml,L"%s00%0.2lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);
				else
					_stprintf(Eml,L"%s000%0.3lf",pzLine->DLArray[i].EGH,pzLine->DLArray[i].ELC);

				putplus(Eml, '+', '.', -3);
				m_grid.SetItemText(i+1,2,Eml); 
				_stprintf(tmp,L"%d",i+1);
				m_grid.SetItemText(i+1,0,tmp);
			}
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);

		//m_grid.SetColumnWidth(0,60);
		//m_grid.SetColumnWidth(1,95);
		//m_grid.SetColumnWidth(2,95);
		m_grid.AutoSize();
		for( i=0;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);
		//	   m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);

		pzLine->close();
	}
	else  if (pObj->isKindOf(EXMANAGER::desc())) //是GTZX实体,取数据
	{		
		EXMANAGER  *pzLine;
		m_eId = eId;
		/*acdbOpenObject(pzLine, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pzLine,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		m_NDL = pzLine->GetDLNum();
		//daicol=1;
		m_grid.SetRowCount(m_NDL+2);
		m_grid.SetFixedRowCount(1);
		m_grid.SetColumnCount(5);

		m_grid.SetFixedColumnCount(1);
		m_grid.SetTextBkColor(txtbkclr);
		//	   m_grid.SetFixedBkColor(fixbkclr);

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
			DT_END_ELLIPSIS|DT_WORDBREAK;
		Item.lfFont=font;
		for(int k=0;k<m_grid.GetColumnCount();k++)
		{
			int i;
			for( i=0;i<m_grid.GetRowCount();i++) 
			{
				Item.row = i;
				Item.col = k;
				m_grid.SetItem(&Item);
			}  
			m_grid.SetItemState(i-1, k,GVIS_READONLY);
		}

		m_grid.SetItemText(0,0,L"序 号");
		m_grid.SetItemText(0,1,L"断前里程");
		m_grid.SetItemText(0,2,L"断后里程");
		m_grid.SetItemText(0,3,L"分离段编号");
		m_grid.SetItemText(0,4,L"分离段名称");

		int i=0;
		ACHAR tmp[10];
		if(m_NDL !=0 )  	
		{
			int FLDNum=pzLine->FLDNum;
			for(int iFLD=0;iFLD<FLDNum;iFLD++)
			{
				JD_CENTER *pJC=pzLine->FLDArray[iFLD];
				for(int j=0; j<pJC->DLNum; j++,i++)
				{
					ACHAR Sml[80],Eml[80];
					if(pJC->DLArray[j].BLC>=1000.0)
						_stprintf(Sml,L"%s%0.2lf",pJC->DLArray[j].BGH,pJC->DLArray[j].BLC);
					else if(pJC->DLArray[j].BLC>=100.0)
						_stprintf(Sml,L"%s0%0.2lf",pJC->DLArray[j].BGH,pJC->DLArray[j].BLC);
					else if(pJC->DLArray[j].BLC>=10.0)
						_stprintf(Sml,L"%s00%0.2lf",pJC->DLArray[j].BGH,pJC->DLArray[j].BLC);
					else
						_stprintf(Sml,L"%s000%0.2lf",pJC->DLArray[j].BGH,pJC->DLArray[j].BLC);

					putplus(Sml, '+', '.', -3);
					m_grid.SetItemText(i+1,1,Sml);

					if(pJC->DLArray[j].ELC>=1000.0)
						_stprintf(Eml,L"%s%0.2lf",pJC->DLArray[j].EGH,pJC->DLArray[j].ELC);
					else if(pJC->DLArray[j].ELC>=100.0)
						_stprintf(Eml,L"%s0%0.2lf",pJC->DLArray[j].EGH,pJC->DLArray[j].ELC);
					else if(pJC->DLArray[j].ELC>=10.0)
						_stprintf(Eml,L"%s00%0.2lf",pJC->DLArray[j].EGH,pJC->DLArray[j].ELC);
					else
						_stprintf(Eml,L"%s000%0.3lf",pJC->DLArray[j].EGH,pJC->DLArray[j].ELC);

					putplus(Eml, '+', '.', -3);					
					m_grid.SetItemText(i+1,2,Eml); 
					ACHAR FLDNo[80];
					_stprintf(FLDNo,L"%d",pJC->DLArray[j].FLDNo);
					m_grid.SetItemText(i+1,3,FLDNo);
					m_grid.SetItemText(i+1,4,pJC->DLArray[j].FLDName);
					_stprintf(tmp,L"%d",i+1);
					m_grid.SetItemText(i+1,0,tmp);
				}
			}
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);

		//m_grid.SetColumnWidth(0,60);
		//m_grid.SetColumnWidth(1,95);
		//m_grid.SetColumnWidth(2,95);
		//m_grid.SetColumnWidth(3,95);
		m_grid.AutoSize();
		for( i=0;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);
		//		m_grid.ResetScrollBars();
		m_grid.SetFocusCell(1,1);

		pzLine->close();
	}
	else
	{
		pObj->close();
		acutPrintf(L"所选实体不是线路实体!\n");
		m_NDL = 0;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());	   
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	return ;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}

void DLBdlg::OnOK() 
{
	if(m_eId.isNull())
	{
		CDialog::OnOK();
		return ;
	}

	CDialog::OnOK();

	UpdateData();
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, m_eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	m_NDL =m_grid.GetRowCount()-2;
	if(m_NDL>0)
	{
		DLBZ *dlb=new DLBZ[m_NDL];
		for(int i=1; i<=m_NDL; i++)
		{
			ACHAR Sml[80],Eml[80];
			_tcscpy(Sml,m_grid.GetItemText(i, 1));
			BAS_DRAW_FUN::split_ckml(Sml, &dlb[i-1].BLC, dlb[i-1].BGH);
			_tcscpy(Eml,m_grid.GetItemText(i, 2));
			BAS_DRAW_FUN::split_ckml(Eml, &dlb[i-1].ELC, dlb[i-1].EGH);
			dlb[i-1].FLDNo=int(_wtof(m_grid.GetItemText(i, 3))+0.01);
			dlb[i-1].FLDName=m_grid.GetItemText(i, 4);
		}

		if (pObj->isKindOf(JD_CENTER::desc() ) ) //是GTZX实体,取数据
		{  
			JD_CENTER  *pzLine;
			if(acdbOpenObject(pObj, m_eId, AcDb::kForWrite)==Acad::eOk)
			{
				pzLine = JD_CENTER::cast(pObj);
				pzLine->assertWriteEnabled();
				pzLine->setDLB(dlb,m_NDL);   
				pzLine->close();

			}		  					
		}
		else if (pObj->isKindOf(GTZDM_ROAD::desc() ) ) //是GTZX实体,取数据
		{  
			GTZDM_ROAD  *pzLine;
			if(acdbOpenObject(pObj, m_eId, AcDb::kForWrite)==Acad::eOk)
			{
				pzLine = GTZDM_ROAD::cast(pObj);
				pzLine->assertWriteEnabled();
				pzLine->setDLB(dlb,m_NDL);   
				pzLine->close();				
			}		  					
		}		
		else if (pObj->isKindOf(EXMANAGER::desc() ) ) //是GTZX实体,取数据
		{ 
			EXMANAGER  *pzLine;
			/*acdbOpenObject(pzLine, m_eId, AcDb::kForWrite);*/
			if(acdbOpenObject(pzLine,m_eId,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}

			pzLine->assertWriteEnabled();
			pzLine->SetDLB(dlb,m_NDL);			
			pzLine->close();
		}
		delete []dlb;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();		
}

BOOL DLBdlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=40;
	rect.right-=10;
	rect.bottom-=45;
	m_grid.Create(rect, this, 900);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
