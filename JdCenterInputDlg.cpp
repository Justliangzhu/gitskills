#include "JdCenterInputDlg.h"
// JdCenterInputDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "JdCenterInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "CWorkDir.h"
//起点A1记录起始里程，A2记录是缓长>0.0,A值=0.0
/////////////////////////////////////////////////////////////////////////////
// JdCenterInputDlg dialog
JdCenterInputDlg::JdCenterInputDlg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(JdCenterInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(JdCenterInputDlg)
	m_GZXS = TRUE;
	m_DRAWJD = TRUE;
	m_JDGH = _T("JD");
	m_JD0 = 1;
	m_addrows = 1;
	DrawFlag = false;
	m_ISLo = true;
	//}}AFX_DATA_INIT
	m_grid.Gzxs=m_GZXS;	
	JdCenObj=NULL;
	JdCenId = NULL;
	ShowTestDrawing=true;

	//	EXObj=NULL;
}


void JdCenterInputDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JdCenterInputDlg)
	DDX_Check(pDX, IDC_CHECK_GZXS, m_GZXS);
	DDX_Check(pDX, IDC_CHECK_DRAWJD, m_DRAWJD);
	DDX_Text(pDX, IDC_EDIT_JDGH, m_JDGH);
	DDX_Text(pDX, IDC_EDIT_JD0, m_JD0);
	DDX_Control(pDX, IDOK, m_TestDrawing);
	DDX_Text(pDX, IDC_EDITrows, m_addrows);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_JDCINPUT, m_grid);
}


BEGIN_MESSAGE_MAP(JdCenterInputDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(JdCenterInputDlg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, OnButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_GETPT, OnButtonGetpt)
	ON_BN_CLICKED(IDC_CHECK_GZXS, OnCheckGzxs)
	ON_BN_CLICKED(IDC_BUTTON_IN, OnButtonInData)
	ON_BN_CLICKED(IDC_BUTTON_OUT, OnButtonOutData)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_Lo, OnCHECKLo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &JdCenterInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JdCenterInputDlg message handlers
BOOL JdCenterInputDlg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);
	//设置为默认缓长l1和l2
	((CButton *)GetDlgItem(IDC_CHECK_Lo))->SetCheck(1);

	// TODO: Add extra initialization here
	if(!ShowTestDrawing) m_TestDrawing.EnableWindow(FALSE);
	CRect rect;
	GetWindowRect(rect);
	CSize szMin;
	szMin.cx=200;//rect.Width();
	szMin.cy=rect.Height();
	CSize szMax;
	szMax.cx=1024+100;
	szMax.cy=768-100;
	SetMinMaxSize(szMin,szMax/* ,bool bResizeIfNecessary =true */);

	RowNum=3;

	//GetClientRect(rect);
	//rect.left+=95;
	//rect.right-=15;
	//rect.bottom-=90;
	//m_grid.Create(rect, this, 100);

	m_grid.SetFixedRowCount(1);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetRowCount(RowNum);
	m_grid.SetColumnCount(HZ_COL+1);

	m_grid.SetItemText(0,XH_COL,L"行号");
	m_grid.SetItemText(0,CTRLPT_COL,L"控制点属性");
	m_grid.SetItemText(0,N_COL,L"坐标(N)");
	m_grid.SetItemText(0,E_COL,L"坐标(E)");
	m_grid.SetItemText(0,R_COL,L"半径(R)");
	m_grid.SetItemText(0,A1_COL,L"前缓(A1)");
	m_grid.SetItemText(0,A2_COL,L"后缓(A2)");
	m_grid.SetItemText(0,ZX_COL,L"转向");
	m_grid.SetItemText(0,LINK_COL,L"后接关系");
	m_grid.SetItemText(0,YQXLEN_COL,L"圆曲线长");
	m_grid.SetItemText(0,JIAZXLEN_COL,L"夹直线长");
	m_grid.SetItemText(0,T1_COL,L"前切线长");
	m_grid.SetItemText(0,T2_COL,L"后切线长");
	m_grid.SetItemText(0,ZH_COL,L"直缓里程");
	m_grid.SetItemText(0,HY_COL,L"缓圆里程");
	m_grid.SetItemText(0,YH_COL,L"圆缓里程");
	m_grid.SetItemText(0,HZ_COL,L"缓直里程");
	/*
	if(EXObj)
	{
	if(EXObj->FLDNum>0)JdCenObj=EXObj->FLDArray[0];
	}
	*/
	COLORREF clr=RGB(0,0,0);
	m_grid.SetGridColor(clr); 
	UpdateGrid();

	INIT();

	m_grid.SetFocusCell(1,1);
	//AddSzControl(m_grid,mdResize,mdResize);	

	//设置名称
	m_grid.SetItemText(0,A1_COL,L"前缓(L1)");
	m_grid.SetItemText(0,A2_COL,L"后缓(L2)");
	//设置起终点的半径、缓长不可写
	m_grid.SetItemState(1,5,GVIS_READONLY);
	m_grid.SetItemState(1,6,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),4,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),5,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),6,GVIS_READONLY);
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void JdCenterInputDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	cdxCSizingDialog::OnCancel();
}

void JdCenterInputDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	int RowNum = m_grid.GetRowCount();
	int nRow = m_grid.GetFocusCell().row;
	//if (nRow >= 0 )
	//	if(RowNum>3 && nRow >1 && nRow<RowNum-1)
	if(nRow >1)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}
	//设置起终点的半径、缓长不可写
	m_grid.SetItemState(1,5,GVIS_READONLY);
	m_grid.SetItemState(1,6,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),4,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),5,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),6,GVIS_READONLY);
	//	m_grid.AutoSize();	
	//	Invalidate();		
}

void JdCenterInputDlg::OnButtonInsert() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int nRow = m_grid.GetFocusCell().row;
	if(nRow==1)
	{
		m_grid.SetFocusCell(2,1);
		nRow = m_grid.GetFocusCell().row;
	}

	int i;
	CString tr;
	if(m_grid.GetRowCount()==1)          // 新建表格里没有行时，加2行
	{
		m_grid.SetRowCount(1); 
		for(i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
			m_grid.SetItemText(1,i,L" ");
		}

		m_grid.SetFocusCell(0,1);
		m_grid.Invalidate();
	}


	//if (nRow >= 0)
	for(int k=0; k<m_addrows; k++)
	{
		//	if (nRow > 0)
		//	{ 
		Item.row=nRow;

		if(nRow==1)
			m_grid.InsertRow(_T("NEW"), nRow+1);
		else
			m_grid.InsertRow(_T("NEW"), nRow);
		//	}
		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS;
		for(int i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			Item.col=0;
			Item.row=i;
			Item.strText=str;
			m_grid.SetItem(&Item);
			m_grid.SetItemText(i,0,str);
		}
		for( i=1;i<m_grid.GetColumnCount();i++) 
		{
			if(i==1)m_grid.SetItemText(nRow,i,L"交点");
			else if(i==ZX_COL)m_grid.SetItemText(nRow,i,L"不回头");
			else if(i==LINK_COL)m_grid.SetItemText(nRow,i,L"无关");
			else if(i==A1_COL)m_grid.SetItemText(nRow,i,L"0");
			else if(i==A2_COL)m_grid.SetItemText(nRow,i,L"0");
			else m_grid.SetItemText(nRow,i,L"");
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
		//}

		Item.col=0;
		Item.row=0;
		Item.strText="行号";
		m_grid.SetItem(&Item);
		Item.strText="";
		UpdateGrid();
		//		m_grid.AutoSize();	
	}
	//	Invalidate();

	//设置起终点的半径、缓长不可写
	m_grid.SetItemState(1,5,GVIS_READONLY);
	m_grid.SetItemState(1,6,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),4,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),5,GVIS_READONLY);
	m_grid.SetItemState((m_grid.GetRowCount()-1),6,GVIS_READONLY);
}

void JdCenterInputDlg::INIT()
{
	int i;
	/*
	LOGFONT font={5, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"宋体"};
	m_grid.SetFont(&font);
	*/
	COLORREF Warnclr=RGB(255,0,0);

	//如果实体已经存在，把实体内的参数赋给grid
	if(JdCenObj&&JdCenObj->JdCenNum>1)
	{
		//	if(JdCenObj->FLDNo<=0) m_FenLiDuan="主线";
		//	else m_FenLiDuan.Format(L"%d",JdCenObj->FLDNo);
		m_JDGH=JdCenObj->JDGH;
		m_JD0=JdCenObj->JD0;
		//	m_StartLC=JdCenObj->LCchr(JdCenObj->DLArray[0].EGH,JdCenObj->DLArray[0].ELC,JdCenObj->NLC);
		m_DRAWJD=JdCenObj->SHOWJD;
		m_grid.SetRowCount(JdCenObj->JdCenNum+1);
		UpdateGrid();
		int j;
		CString str;
		BAS_DRAW_FUN::JdCenter *CurJdCen,*PreJdCen;

		if(JdCenObj->JdCenArray[0].R > 0.000001)
		{
			m_ISLo = true;
			CButton *pbtn=(CButton *)GetDlgItem(IDC_CHECK_Lo);
			pbtn->SetCheck(1);
		}

		m_grid.SetColumnWidth(N_COL, 125);
		m_grid.SetColumnWidth(E_COL, 125);
		m_grid.SetColumnWidth(R_COL, 125);

		for(i=0;i<JdCenObj->JdCenNum;i++)
		{
			j=i+1;
			CurJdCen=&JdCenObj->JdCenArray[i];

			m_grid.SetRowHeight(j, DEFAULTCOLHEI);

			str.Format(L"%d",j);
			m_grid.SetItemText(j,XH_COL,str);

			if(i==0)
				m_grid.SetItemText(j,CTRLPT_COL,L"起点");
			else if(i==JdCenObj->JdCenNum-1)
				m_grid.SetItemText(j,CTRLPT_COL,L"终点");
			else if(CurJdCen->JDXZ==IsJiaoDian)
			{

				m_grid.SetItemText(j,CTRLPT_COL,L"交点");				
			}
			else m_grid.SetItemText(j,CTRLPT_COL,L"圆心");	

			str.Format(L"%.6lf",CurJdCen->N);
			m_grid.SetItemText(j,N_COL,str);
			
			str.Format(L"%.6lf",CurJdCen->E);
			m_grid.SetItemText(j,E_COL,str);

			if(i>0 && i<JdCenObj->JdCenNum-1)
			{
				if(	m_ISLo == true)//输入缓长
				{		
					if(CurJdCen->R>0.0001)
					{
						CurJdCen->Ls2 = CurJdCen->A2*CurJdCen->A2/CurJdCen->R;
						CurJdCen->Ls1 = (CurJdCen->A1*CurJdCen->A1/CurJdCen->R);

						if(JdCenObj->JdCenArray[i-1].LinkAfter==Luan_Link && CurJdCen->A1>1e-6)
							CurJdCen->Ls1 = fabs((CurJdCen->A1*CurJdCen->A1/CurJdCen->R)- (CurJdCen->A1*CurJdCen->A1/JdCenObj->JdCenArray[i-1].R));

						if(JdCenObj->JdCenArray[i].LinkAfter==Luan_Link && CurJdCen->A2>1e-6)
							CurJdCen->Ls2 = fabs((CurJdCen->A2*CurJdCen->A2/CurJdCen->R)- (CurJdCen->A2*CurJdCen->A2/JdCenObj->JdCenArray[i+1].R));

					}
					else
					{
						CurJdCen->Ls1=CurJdCen->Ls2=0.0;
					}

				}

				if( (CurJdCen->A1>0.0 && CurJdCen->A1<CurJdCen->R/3.0)
					|| CurJdCen->A1>CurJdCen->R)
				{
					m_grid.SetItemBkColour(j,A1_COL,Warnclr);
				}
				if( (CurJdCen->A2>0.0 && CurJdCen->A2<CurJdCen->R/3.0)
					|| CurJdCen->A2>CurJdCen->R)
				{
					m_grid.SetItemBkColour(j,A2_COL,Warnclr);
				}
			}

			if(m_ISLo)//输入缓长
			{
				if(i==0)
					CurJdCen->Ls1=CurJdCen->A1;			
				str.Format(L"%.6lf",CurJdCen->Ls1);
			}
			else
				str.Format(L"%.6lf",CurJdCen->A1);

			m_grid.SetItemText(j,A1_COL,str);

			if(i>0)
				if (abs(CurJdCen->R)>999999999)
					str="0";			
				else
					str.Format(L"%.6lf",CurJdCen->R);
			else	
				str="0";

			m_grid.SetItemText(j,R_COL,str);

			if(m_ISLo)//输入缓长
				str.Format(L"%.6lf",CurJdCen->Ls2);
			else
				str.Format(L"%.6lf",CurJdCen->A2);

			m_grid.SetItemText(j,A2_COL,str);


			if(CurJdCen->JDXZ==IsJiaoDian)
			{
				if(CurJdCen->IsOrNotHuiTou==1)m_grid.SetItemText(j,ZX_COL,L"顺向左转回头");
				else if(CurJdCen->IsOrNotHuiTou==2)m_grid.SetItemText(j,ZX_COL,L"顺向右转回头");
				else if(CurJdCen->IsOrNotHuiTou==3)m_grid.SetItemText(j,ZX_COL,L"逆向左转回头");
				else if(CurJdCen->IsOrNotHuiTou==4)m_grid.SetItemText(j,ZX_COL,L"逆向右转回头");
				else m_grid.SetItemText(j,ZX_COL,L"不回头");
			}
			else
			{
				if(CurJdCen->LeftOrRight==LEFT)m_grid.SetItemText(j,ZX_COL,L"左转");
				else m_grid.SetItemText(j,ZX_COL,L"右转");				
			}			

			if(CurJdCen->LinkAfter==S_Link)m_grid.SetItemText(j,LINK_COL,L"S型");
			else if(CurJdCen->LinkAfter==C_Link)m_grid.SetItemText(j,LINK_COL,L"C型");
			else if(CurJdCen->LinkAfter==Luan_Link)m_grid.SetItemText(j,LINK_COL,L"卵型");
			else m_grid.SetItemText(j,LINK_COL,L"无关");

			//圆曲线长
			//T1,T2
			//缓圆里程、圆缓里程
			if(i>0 && i<JdCenObj->JdCenNum-1)
			{				
				int CirXYNo=CurJdCen->CirXYNo;
				if(JdCenObj->XYArray && CirXYNo>=1 && CirXYNo<JdCenObj->XYNum)
				{
					str.Format(L"%.6lf",JdCenObj->XYArray[CirXYNo][2]);
					m_grid.SetItemText(j,YQXLEN_COL,str);
					//					if(JdCenObj->XYArray[CirXYNo][2]<0.0)m_grid.SetItemBkColour(j,YQXLEN_COL,Warnclr);
					double HYTYLC=JdCenObj->XYArray[CirXYNo][6];
					double YHTYLC=HYTYLC+JdCenObj->XYArray[CirXYNo][2];
					CString GH;
					double HYXLC,YHXLC;
					HYXLC=JdCenObj->XLC(HYTYLC,GH);
					str=BAS_DRAW_FUN::LCchr(GH,HYXLC,3);
					m_grid.SetItemText(j,HY_COL,str);
					YHXLC=JdCenObj->XLC(YHTYLC,GH);
					str=BAS_DRAW_FUN::LCchr(GH,YHXLC,3);
					m_grid.SetItemText(j,YH_COL,str);
					int iXY=CirXYNo-1;
					//	if(iXY>=1 && iXY<JdCenObj->XYNum && JdCenObj->XYJDNO[iXY]==i)
					//	{
					double ZHTYLC,ZHXLC;
					//ZHTYLC=JdCenObj->XYArray[iXY][6];
					ZHTYLC = HYTYLC - CurJdCen->A1*CurJdCen->A1/CurJdCen->R;
					ZHXLC=JdCenObj->XLC(ZHTYLC,GH);
					str=BAS_DRAW_FUN::LCchr(GH,ZHXLC,3);
					m_grid.SetItemText(j,ZH_COL,str);
					//	}
					//	iXY=CirXYNo+1;
					//	if(iXY>=1 && iXY<JdCenObj->XYNum && JdCenObj->XYJDNO[iXY]==i)
					//	{
					double HZTYLC,HZXLC;
					//	HZTYLC=JdCenObj->XYArray[iXY+1][6];
					HZTYLC = YHTYLC + CurJdCen->A2*CurJdCen->A2/CurJdCen->R;
					HZXLC=JdCenObj->XLC(HZTYLC,GH);
					str=BAS_DRAW_FUN::LCchr(GH,HZXLC,3);
					m_grid.SetItemText(j,HZ_COL,str);
					//		}
				}
				str.Format(L"%.6lf",CurJdCen->T1);
				m_grid.SetItemText(j,T1_COL,str);
				str.Format(L"%.6lf",CurJdCen->T2);
				m_grid.SetItemText(j,T2_COL,str);
			}
			if(i==0)
			{
				str=BAS_DRAW_FUN::LCchr(JdCenObj->DLArray[0].EGH,JdCenObj->DLArray[0].ELC,3);
				m_grid.SetItemText(j,HY_COL,str);
				m_grid.SetItemText(j,YH_COL,str);
				m_grid.SetItemText(j,ZH_COL,str);
				m_grid.SetItemText(j,HZ_COL,str);
			}
			if(i==JdCenObj->JdCenNum-1)
			{
				str=BAS_DRAW_FUN::LCchr(JdCenObj->DLArray[JdCenObj->DLNum-1].BGH,
					JdCenObj->DLArray[JdCenObj->DLNum-1].BLC,3);
				//ads_printf(L"BLC=%lf \n",JdCenObj->DLArray[JdCenObj->DLNum-1].BLC);
				m_grid.SetItemText(j,HY_COL,str);
				m_grid.SetItemText(j,YH_COL,str);
				m_grid.SetItemText(j,ZH_COL,str);
				m_grid.SetItemText(j,HZ_COL,str);
			}
			//与前一交点夹直线长
			if(i>0)
			{
				PreJdCen=&JdCenObj->JdCenArray[i-1];
				double jialen=PreJdCen->DistToAfterJD-PreJdCen->T2-CurJdCen->T1;
				//ads_printf(L"dis=%lf t2=%lf t1=%lf\n",PreJdCen->DistToAfterJD,PreJdCen->T2,CurJdCen->T1);		//20190714	不提示
				if(fabs(jialen)<0.01)jialen=0.0;
				str.Format(L"%.6lf",jialen);
				m_grid.SetItemText(j,JIAZXLEN_COL,str);
				if(jialen<0.0)m_grid.SetItemBkColour(j,JIAZXLEN_COL,Warnclr);
			}

		}
	}
	//	m_grid.AutoSize();	
	UpdateData(FALSE);
	DrawFlag = false;
}

void JdCenterInputDlg::UpdateGrid()
{
	COLORREF Readonlyclr=RGB(113,189,193);
	//    COLORREF Readonlyclr=RGB(255,255,0);

	int i,j;
	CString RowNo;
	for(i=0;i<m_grid.GetRowCount();i++)
	{
		for(j=0;j<m_grid.GetColumnCount();j++)
		{
			m_grid.SetItemFormat(i,j,DT_CENTER|DT_VCENTER|DT_WORDBREAK|DT_SINGLELINE|DT_END_ELLIPSIS);
		}
		if(i>0)
		{
			RowNo.Format(L"%d",i);
			m_grid.SetItemText(i,XH_COL,RowNo);
			for(j=1;j<8;j++)
				m_grid.SetItemBkColour(i,j,CLR_DEFAULT);
			for(j=9;j<m_grid.GetColumnCount();j++)
			{
				m_grid.SetItemBkColour(i,j,Readonlyclr);	
				//	m_grid.RedrawCell(i,j);
				//		//	   m_grid.RedrawColumn(j);
			}				

		}
	}
	//for(i=YQXLEN_COL;i<=HZ_COL;i++)
	//	      m_grid.SetColumnNoEditable(i);


	for(i=0;i<m_grid.GetRowCount();i++)
	{
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	}
	m_grid.SetColumnWidth(0,50);
	for(i=1;i<m_grid.GetColumnCount();i++)
	{
		m_grid.SetColumnWidth(i,80);
	}

	m_grid.SetColumnWidth(N_COL, 125);
	m_grid.SetColumnWidth(E_COL, 125);
	m_grid.SetColumnWidth(R_COL, 125);
	//////////////////////////////////////////////////////////////////////////
	//	m_grid.SetItemText(1,CTRLPT_COL,L"起点");
	////	m_grid.SetItemText(1,A1_COL,L"-");
	//	m_grid.SetItemText(1,R_COL,L"-");
	//	m_grid.SetItemText(1,A2_COL,L"-");
	//	m_grid.SetItemText(1,ZX_COL,L"-");
	//	m_grid.SetItemText(1,LINK_COL,L"-");
	//	
	//	int RowNum=m_grid.GetRowCount();
	//	m_grid.SetItemText(RowNum-1,CTRLPT_COL,L"终点");
	//	m_grid.SetItemText(RowNum-1,A1_COL,L"-");
	//	m_grid.SetItemText(RowNum-1,R_COL,L"-");
	//	m_grid.SetItemText(RowNum-1,A2_COL,L"-");
	//	m_grid.SetItemText(RowNum-1,ZX_COL,L"-");
	//	m_grid.SetItemText(RowNum-1,LINK_COL,L"-");

	//	m_grid.ResetScrollBars();

	//	m_grid.AutoSize();	
}
void JdCenterInputDlg::OnButtonGetpt() 
{
	// TODO: Add your control notification handler code here
	m_grid.UpdateData(TRUE);
	GetParent()->ShowWindow(SW_HIDE);
	acedGetAcadFrame()->SetFocus();
	ads_point resPt;
	AcGePoint3d PT;
	AcDbEntity *pEnt;
	AcDbObjectId eId;
	ads_name en;
	CString str;

	int nRow = m_grid.GetFocusCell().row;
	int nCol = m_grid.GetFocusCell().col;
	if(nRow>0)
	{		
		CString JDXZstr=m_grid.GetItemText(nRow,CTRLPT_COL);
		if(JDXZstr!="圆心" && (nCol==N_COL || nCol==E_COL || nCol==CTRLPT_COL))
		{
			if(acedGetPoint(NULL,L"请在屏幕上拾取交点坐标!\n",resPt)==RTNORM)
			{
				CString str;
				str.Format(L"%.8lf",resPt[Y]);
				m_grid.SetItemText(nRow,N_COL,str);
				str.Format(L"%.8lf",resPt[X]);
				m_grid.SetItemText(nRow,E_COL,str);
				m_grid.UpdateData(FALSE);
			}

		}
		else
		{
			if(RTNORM==acedEntSel(L"\n请拾取ARC/Circle实体,指定半径:",en,asDblArray(PT)))
			{
				acdbGetObjectId(eId,en);	
				/*acdbOpenObject(pEnt,eId,AcDb::kForRead);*/
				if(acdbOpenObject(pEnt,eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pEnt->close();
				if(pEnt->isKindOf( AcDbArc::desc() ) )
				{
					AcDbArc *Arc=AcDbArc::cast(pEnt);

					/*acdbOpenObject(Arc,eId,AcDb::kForRead);*/
					if(acdbOpenObject(Arc,eId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}

					CString str;
					if(JDXZstr=="圆心")
					{
						str.Format(L"%.8lf",Arc->center().y);
						m_grid.SetItemText(nRow,N_COL,str);
						str.Format(L"%.8lf",Arc->center().x);
						m_grid.SetItemText(nRow,E_COL,str);
					}
					str.Format(L"%.8lf",Arc->radius());
					m_grid.SetItemText(nRow,R_COL,str);
					m_grid.UpdateData(FALSE);

					Arc->close();
				}
				else if(pEnt->isKindOf( AcDbCircle::desc() ) )
				{
					AcDbCircle *Cir=AcDbCircle::cast(pEnt);

					/*acdbOpenObject(Cir,eId,AcDb::kForRead);*/
					if(acdbOpenObject(Cir,eId, AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					CString str;
					if(JDXZstr=="圆心")
					{
						str.Format(L"%.8lf",Cir->center().y);
						m_grid.SetItemText(nRow,N_COL,str);
						str.Format(L"%.8lf",Cir->center().x);
						m_grid.SetItemText(nRow,E_COL,str);
					}
					str.Format(L"%.8lf",Cir->radius());
					m_grid.SetItemText(nRow,R_COL,str);
					m_grid.UpdateData(FALSE);

					Cir->close();
				}
			}
		}
		//		else if(acedGetPoint(NULL,L"请在屏幕上拾取一点!\n",resPt)==RTNORM)
		//		{
		//			CString str;
		//			str.Format(L"%.8lf",resPt[Y]);
		//			m_grid.SetItemText(nRow,N_COL,str);
		//			str.Format(L"%.8lf",resPt[X]);
		//			m_grid.SetItemText(nRow,E_COL,str);
		//			m_grid.UpdateData(FALSE);
		//		}
	}
	GetParent()->ShowWindow(SW_SHOW);
	SetFocus();			
	//	m_grid.AutoSize();	
}

void JdCenterInputDlg::OnCheckGzxs() 
{
	// TODO: Add your control notification handler code here
	/*
	if(m_GZXS==TRUE)m_GZXS=FALSE;
	else m_GZXS=TRUE;
	*/
	UpdateData(TRUE);
	m_grid.Gzxs=m_GZXS;
	UpdateData(FALSE);
}

void JdCenterInputDlg::OnButtonInData() //恢复
{
	// TODO: Add your control notification handler code here
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		Path=WorkDir.JdCenBackUp;
		CFileDialog FDlg(TRUE,L"*.JdCen",Path);
		FDlg.m_ofn.Flags = OFN_CREATEPROMPT;
		FDlg.m_ofn.lpstrFilter=L"*.JdCen\0*.JdCen\0\0";		

		if(FDlg.DoModal()==IDOK)
		{
			Path=FDlg.GetPathName();
		}
		else return;
	}
	FILE *fp=_wfopen(Path,L"r+");
	if(fp==NULL)return;
	int num;
	ACHAR str[256];
	fwscanf(fp,L"%d",&num);
	m_grid.SetRowCount(num+1);
	for(int i=1;i<num+1;i++)
	{
		for(int j=0;j<=LINK_COL;j++)
		{
			fwscanf(fp,L"%s",str);
			if(i==1&&j==5)
			{
				double al;
				al = _wtof(str);
				if(al>0.00001)
				{
					m_ISLo = true;
					CButton *pbtn=(CButton *)GetDlgItem(IDC_CHECK_Lo);
					pbtn->SetCheck(1);
				}

			}
			m_grid.SetItemText(i,j,str);			
		}
	}
	fclose(fp);
	JdCenObj=NULL;
	INIT();
	m_grid.Invalidate();
	UpdateGrid();
	Invalidate();	
}

void JdCenterInputDlg::OnButtonOutData() 
{
	// TODO: Add your control notification handler code here
	int num=m_grid.GetRowCount()-1;
	if(num<=0)return;
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		Path=WorkDir.JdCenBackUp;
		CFileDialog FDlg(FALSE,L"*.JdCen",Path);
		FDlg.m_ofn.Flags = OFN_CREATEPROMPT;
		FDlg.m_ofn.lpstrFilter=L"*.JdCen\0*.JdCen\0\0";		

		if(FDlg.DoModal()==IDOK)
		{
			Path=FDlg.GetPathName();
		}
		else return;
	}
	FILE *fp=_wfopen(Path,L"w+");
	if(fp==NULL)return;
	fwprintf(fp,L"%d\n",num);
	CString str;
	for(int i=1;i<num+1;i++)
	{
		for(int j=0;j<9;j++)
		{
			str=m_grid.GetItemText(i,j);
			if(str=="" || str==" ")str="-";
			if(i==1&&j==5 && m_ISLo)
				str = "0.001";

			fwprintf(fp,L"%s ",str);
		}
		fwprintf(fp,L"\n");
	}
	fclose(fp);
}


void JdCenterInputDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if(DrawFlag)
	{;
	INIT();
	}		
	// Do not call cdxCSizingDialog::OnPaint() for painting messages
}


void JdCenterInputDlg::OnCHECKLo() 
{
	// TODO: Add your control notification handler code here
	double A1,A2,ls1,ls2,R;
	CString str;
	UpdateData(TRUE);
	m_ISLo = !m_ISLo;
	if(m_ISLo)//原来为A，现在为lo
	{

		int i,njdcen=m_grid.GetRowCount()-1;
		if(njdcen<2)return;
		//设置名称
		m_grid.SetItemText(0,A1_COL,L"前缓(L1)");
		m_grid.SetItemText(0,A2_COL,L"后缓(L2)");
		for(i=1;i<njdcen-1;i++)
		{
			//控制点属性
			//前缓(A1)
			str=m_grid.GetItemText(i+1,A1_COL);
			A1=_wtof(str);

			str=m_grid.GetItemText(i+1,R_COL);
			R=_wtof(str);

			//后缓(A2)
			str=m_grid.GetItemText(i+1,A2_COL);
			A2=_wtof(str);

			if(R>0.000001)
			{
				ls1 = A1*A1/R;
				ls2 = A2*A2/R;
			}
			else
			{
				ls1 = 0.0;
				ls2 = 0.0;
			}
			str.Format(L"%0.4lf",ls1);
			m_grid.SetItemText(i+1,A1_COL,str);

			str.Format(L"%0.4lf",ls2);
			m_grid.SetItemText(i+1,A2_COL,str);

		}
	}
	else
	{
		//设置名称
		m_grid.SetItemText(0,A1_COL,L"前缓(A1)");
		m_grid.SetItemText(0,A2_COL,L"后缓(A2)");
		int i,njdcen=m_grid.GetRowCount()-1;
		if(njdcen<2)return;
		for(i=1;i<njdcen-1;i++)
		{
			//控制点属性
			//前缓(A1)
			str=m_grid.GetItemText(i+1,A1_COL);
			ls1=_wtof(str);

			str=m_grid.GetItemText(i+1,R_COL);
			R=_wtof(str);

			//后缓(A2)
			str=m_grid.GetItemText(i+1,A2_COL);
			ls2=_wtof(str);

			A1 = sqrt(R*ls1);
			A2 = sqrt(R*ls2);
			str.Format(L"%0.4lf",A1);
			m_grid.SetItemText(i+1,A1_COL,str);

			str.Format(L"%0.4lf",A2);
			m_grid.SetItemText(i+1,A2_COL,str);
		}

	}
	m_grid.Invalidate();
	//UpdateData(FALSE);
}


void JdCenterInputDlg::OnBnClickedOk()
{
	//	cdxCSizingDialog::OnOK();
	UpdateData(TRUE);
	int i,njdcen=m_grid.GetRowCount()-1;
	if(njdcen<2)return;
	BAS_DRAW_FUN::JdCenter *jdcenay=new BAS_DRAW_FUN::JdCenter[njdcen];
	CString str;
	//	jdcenay[0].A1=0.0;

	for(i=0;i<njdcen;i++)
	{
		//控制点属性
		str=m_grid.GetItemText(i+1,CTRLPT_COL);
		if(str=="圆心")
			jdcenay[i].JDXZ=IsCenter;	
		else 
			jdcenay[i].JDXZ=IsJiaoDian;

		//坐标(N)
		str=m_grid.GetItemText(i+1,N_COL);
		jdcenay[i].N=_wtof(str);

		//坐标(E)
		str=m_grid.GetItemText(i+1,E_COL);
		jdcenay[i].E=_wtof(str);

		//前缓(A1)
		str=m_grid.GetItemText(i+1,A1_COL);
		jdcenay[i].A1=_wtof(str);

		m_grid.SetItemText(0,R_COL,L"半径(R)");
		str=m_grid.GetItemText(i+1,R_COL);
		jdcenay[i].R=_wtof(str);


		//后缓(A2)
		str=m_grid.GetItemText(i+1,A2_COL);
		jdcenay[i].A2=_wtof(str);




		//后接关系
		str = m_grid.GetItemText(i + 1, LINK_COL);
		if (str == "S型")
			jdcenay[i].LinkAfter = S_Link;
		else if (str == "C型")
			jdcenay[i].LinkAfter = C_Link;
		else if (str == "卵型")
			jdcenay[i].LinkAfter = Luan_Link;
		else 
			jdcenay[i].LinkAfter = NotRelation;
		COLORREF Warnclr = RGB(255, 0, 0);

		//转向
		str = m_grid.GetItemText(i + 1, ZX_COL);

		if(i>0 && i<njdcen-1)
		{
			if(jdcenay[i].JDXZ==IsCenter)
			{
				if(str=="左转")
					jdcenay[i].LeftOrRight=LEFT;
				else if(str=="右转") 
					jdcenay[i].LeftOrRight=RIGHT;
				else
				{
					ads_printf(L"第%d个控制点性质为圆心，您选择为回头/不回头，请选择左转或右转!\n",i);
					m_grid.SetItemBkColour(i+1,ZX_COL,Warnclr);
					jdcenay[i].LeftOrRight=RIGHT;
				}
			}
			else
			{

				if(str=="顺向左转回头")jdcenay[i].IsOrNotHuiTou=1;
				else if(str=="顺向右转回头")jdcenay[i].IsOrNotHuiTou=2;
				else if(str=="逆向左转回头")jdcenay[i].IsOrNotHuiTou=3;
				else if(str=="逆向右转回头")jdcenay[i].IsOrNotHuiTou=4;
				else if(str=="不回头")jdcenay[i].IsOrNotHuiTou=false;
				else
				{
					ads_printf(L"第%d个控制点性质为圆心，您选择左转或右转，请选择为回头/不回头!\n",i);
					m_grid.SetItemBkColour(i+1,ZX_COL,Warnclr);
					jdcenay[i].IsOrNotHuiTou=false;
				}

			}


			if (m_ISLo == TRUE)
			{
				jdcenay[i].Ls1 = jdcenay[i].A1;
				jdcenay[i].A1 = sqrt(jdcenay[i].A1*jdcenay[i].R);
				jdcenay[i].Ls2 = jdcenay[i].A2;
				jdcenay[i].A2 = sqrt(jdcenay[i].A2*jdcenay[i].R);
			}
		}

	}


	if(njdcen>0)
		jdcenay[njdcen-1].IsOrNotHuiTou=false;//终点不回头
	//缓和曲线长转A值
	for(i=1; i<njdcen-1; i++)
	{
		//if(jdcenay[i].R<0.001)//半径为0	//20190904尝试修改	允许用户设置为0
		//	jdcenay[i].R = 0.001;


		if(jdcenay[i].LinkAfter==Luan_Link)
		{
			//问题:前后半径一样大时
			if(jdcenay[i].Ls2>1e-6)
				jdcenay[i].A2=sqrt(jdcenay[i].Ls2*(jdcenay[i].R*jdcenay[i+1].R)
				/fabs(jdcenay[i].R-jdcenay[i+1].R));
			if(jdcenay[i+1].Ls1>1e-6)
				jdcenay[i+1].A1=sqrt(jdcenay[i+1].Ls1*(jdcenay[i].R*jdcenay[i+1].R)
				/fabs(jdcenay[i].R-jdcenay[i+1].R));
		}

	}
	if(njdcen>0)
	{
		jdcenay[njdcen-1].IsOrNotHuiTou=false;//终点不回头
		jdcenay[njdcen-1].A1=jdcenay[njdcen-1].A2=jdcenay[njdcen-1].Ls1=jdcenay[njdcen-1].Ls2=0.0;
		jdcenay[njdcen-1].LinkAfter = NotRelation;
	}

	jdcenay[0].A2=m_JD0;//起始交点A2表示交点号
	if(m_ISLo)
		jdcenay[0].R=0.001;//起点半径记录a/lo;

	CString LCGH,JDGH;
	double StartLc;	


	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	/*
	if(EXObj)
	{
	if(JdCenObj->FLDNo>0)
	{
	acdbOpenObject(EXObj,EXId,AcDb::kForWrite);

	JdCenObj->split_ckml(m_StartLC,&StartLc,LCGH);		
	JdCenObj->SetJDGH(m_JDGH);
	JdCenObj->JD0=m_JD0;
	_tcscpy(JdCenObj->LCGH,LCGH);
	jdcenay[0].A1=StartLc;
	JdCenObj->SetJdCenterArray(jdcenay,njdcen);
	if(m_DRAWJD)JdCenObj->SetShowJD(TRUE);
	else JdCenObj->SetShowJD(FALSE);

	EXObj->assertWriteEnabled();
	EXObj->close();
	}		
	}
	else*/

	Acad::ErrorStatus es;

	if(JdCenId!=NULL)
	{
		AcDbObject* pObj = NULL;
		es = acdbOpenObject(pObj,JdCenId,AcDb::kForWrite);
		JdCenObj = JD_CENTER::cast(pObj);
	}

	if(JdCenObj==NULL||(JdCenId!=NULL&&es==Acad::eWasErased)||JdCenObj->isErased())
	{

		JdCenObj = new JD_CENTER;
		//JdCenObj->split_ckml(m_StartLC,&StartLc,LCGH);		
		JdCenObj->SetJDGH(m_JDGH);
		JdCenObj->JD0 = m_JD0;
		if (m_FenLiDuan == "主线")JdCenObj->FLDNo = 0;
		else JdCenObj->FLDNo = (int)(_wtof(m_FenLiDuan) + 0.01);
		_tcscpy(JdCenObj->LCGH, L"K");
		//			jdcenay[0].A1=0.0;
		JdCenObj->SetJdCenterArray(jdcenay, njdcen);

		if (m_DRAWJD)JdCenObj->SetShowJD(TRUE);
		else JdCenObj->SetShowJD(FALSE);
		JdCenObj->setLayer(L"公路平面设计专用层");
		_tcscpy(JdCenObj->RoadName, m_rname);
		JdCenId = BAS_DRAW_FUN::AddEntityToDbs(JdCenObj);

	}
	else if(es==Acad::eOk)
	{
		JdCenObj->assertWriteEnabled();
		//acdbOpenObject(JdCenObj,JdCenId,AcDb::kForWrite);
		//	if(m_FenLiDuan=="主线")JdCenObj->FLDNo=0;
		//	else JdCenObj->FLDNo=(int)(_wtof(m_FenLiDuan)+0.01);
		JdCenObj->setLayer(L"公路平面设计专用层");
		//	JdCenObj->split_ckml(m_StartLC,&StartLc,LCGH);		

		JdCenObj->SetJDGH(m_JDGH);
		JdCenObj->JD0=m_JD0;
		//	_tcscpy(JdCenObj->LCGH,LCGH);
		//	jdcenay[0].A1=StartLc;

		JdCenObj->SetJdCenterArray(jdcenay,njdcen);

		_tcscpy(JdCenObj->RoadName,m_rname);

		if(m_DRAWJD)JdCenObj->SetShowJD(TRUE);
		else JdCenObj->SetShowJD(FALSE);
		JdCenObj->assertWriteEnabled();
		JdCenObj->CalXYArray();

		JdCenObj->close();
	}
	//道路名		
	//	m_ISLo=FALSE;



	UpdateData(FALSE);

	INIT();

	m_grid.Invalidate();


	/*
	JdCenObj=new JD_CENTER;
	JdCenObj->SetJdCenterArray(jdcenay,njdcen);
	JdCenObj->Draw();
	delete JdCenObj;
	*/	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	

	if(jdcenay) delete []jdcenay;

}


