// JMXYDLG.cpp : implementation file
//

#include "stdafx.h"
#include "JMXYDLG.h"
#include "CWorkDir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJMXYDLG dialog
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

CJMXYDLG::CJMXYDLG(CWnd* pParent /*=NULL*/,AcDbObjectId eId)
	: cdxCSizingDialog(CJMXYDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJMXYDLG)
	m_XYSFwj = _T("");
	m_XYSX = _T("");
	m_XYSY = _T("");
	m_XYFNAME = _T("");
	m_XYSml = _T("0.0");
	//}}AFX_DATA_INIT
	m_eId = eId;
}

void CJMXYDLG::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJMXYDLG)
	DDX_Text(pDX, IDC_EDIT_XYSFWJ, m_XYSFwj);
	DDX_Text(pDX, IDC_EDIT_XYSX, m_XYSX);
	DDX_Text(pDX, IDC_EDIT_XYSY, m_XYSY);
	DDX_Text(pDX, IDC_EDIT_XYFILENAME, m_XYFNAME);
	DDX_Text(pDX, IDC_EDIT_XLSML, m_XYSml);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJMXYDLG, cdxCSizingDialog)
	//{{AFX_MSG_MAP(CJMXYDLG)
	ON_BN_CLICKED(IDC_BUTTON_XYADD, OnButtonXyadd)
	ON_BN_CLICKED(IDC_BUTTON_XYDEL, OnButtonXydel)
	ON_BN_CLICKED(IDC_BUTTON_INPUTFILE, OnButtonInputfile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJMXYDLG message handlers//从JDCEN中得到线元数据 存在pXYArr中
void CJMXYDLG::GetXYArrFromJDCen(JD_CENTER *pm)
{
	int i,j;
	XYarray xray;
	xray.eID=0;
	for(i=0;i<=pm->XYNum;i++)
	{
       for(j=0;j<10;j++)
		   xray.xyarr[j]=pm->XYArray[i][j];
	   pXYArr.Add(xray);
	}
}
BOOL CJMXYDLG::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);
	
	// TODO: Add extra initialization here
      m_XYNum = 0;
	  if(m_eId)
	  {
		  AcDbObject *pObj;	  
		  /*acdbOpenObject(pObj, m_eId, AcDb::kForRead);*/
		  if(acdbOpenObject(pObj,m_eId, AcDb::kForRead)!=Acad::eOk)
		  {
			  ads_printf(L"打开实体失败！\n");
			  return false;
		  }
		  pObj->close();
		  if(pObj->isKindOf(JD_CENTER::desc())) 
		  { 
			  JD_CENTER  *pzLine=JD_CENTER::cast(pObj);
			  m_XYNum = pzLine->XYNum;
			  GetXYArrFromJDCen(pzLine);
		  }
		  else
		  {
			  FormXYarray(m_eId,pXYArr);	
			  m_XYNum = pXYArr.GetSize()-1;
		  }
	  }

	  CRect rect;
	  GetClientRect(rect);
	  rect.left+=10;
	  rect.top+=150;
	  rect.right-=10;
	  rect.bottom-=10;
	  m_grid.Create(rect, this, 900);
	  if(m_XYNum)
		  m_grid.SetRowCount(m_XYNum+1);
	  else
		  m_grid.SetRowCount(2);
	  m_grid.SetFixedRowCount(1);
	  m_grid.SetColumnCount(6);
	  m_grid.SetFixedColumnCount(1);
	  m_grid.SetTextBkColor(txtbkclr);
	  //	   m_grid.SetFixedBkColor(fixbkclr);

	  Item.mask = GVIF_TEXT|GVIF_FORMAT;
	  Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		  DT_END_ELLIPSIS|DT_WORDBREAK;
	  Item.lfFont=font;

	  for(int k=0;k<m_grid.GetColumnCount();k++)
	  {
		  for(int i=0;i<m_grid.GetRowCount();i++) 
		  {
			  Item.row = i;
			  Item.col = k;
			  m_grid.SetItem(&Item);
		  }  
		  //	   m_grid.SetItemState(i-1, k,GVIS_READONLY);
	  }

	  m_grid.SetItemText(0,0,L"序 号");
	  m_grid.SetItemText(0,1,L"转 向");
	  m_grid.SetItemText(0,2,L"线元长度");
	  m_grid.SetItemText(0,3,L"A  值");
	  m_grid.SetItemText(0,4,L"起点半径");
	  m_grid.SetItemText(0,5,L"终点半径");	   	   

	  int i=1;
	  ACHAR tmp[30];
	  double xylen,dms;
	  if(m_XYNum > 0 )  	
	  {
		  m_XYSX.Format(L"%0.8lf",pXYArr[0].xyarr[1]);
		  m_XYSY.Format(L"%0.8lf",pXYArr[0].xyarr[2]);
		  dms=dms_rad(pXYArr[0].xyarr[3]);
		  m_XYSFwj.Format(L"%lf",dms);
		  m_XYSml.Format(L"%0.3lf",pXYArr[0].xyarr[4]);
		  for(i=1; i<m_XYNum; i++)
		  {
			  if(pXYArr[i].xyarr[4]>0.1)
				  m_grid.SetItemText(i,1,L"右转");
			  else
				  m_grid.SetItemText(i,1,L"左转");
			  xylen = pXYArr[i+1].xyarr[6]-pXYArr[i].xyarr[6];	
			  _stprintf(tmp,L"%0.8lf",xylen);			   
			  m_grid.SetItemText(i,2,tmp);
			  if(pXYArr[i].xyarr[0]>2.1)
				  _stprintf(tmp,L"%0.8lf",pXYArr[i].xyarr[1]);
			  else
				  _tcscpy(tmp,L"-1");
			  m_grid.SetItemText(i,3,tmp);
			  if(pXYArr[i].xyarr[0]<1.1)//直线
			  {
				  _tcscpy(tmp,L"99999");
				  m_grid.SetItemText(i,4,tmp);
				  m_grid.SetItemText(i,5,tmp);
			  }
			  else if(pXYArr[i].xyarr[0]<2.1)//圆
			  {
				  _stprintf(tmp,L"%0.8lf",pXYArr[i].xyarr[1]);
				  m_grid.SetItemText(i,4,tmp);
				  m_grid.SetItemText(i,5,tmp);
			  }
			  else if(pXYArr[i].xyarr[0]<3.1)//前缓
			  {
				  _tcscpy(tmp,L"99999");
				  m_grid.SetItemText(i,4,tmp);
				  _stprintf(tmp,L"%0.8lf",pXYArr[i].xyarr[2]);
				  m_grid.SetItemText(i,5,tmp);
			  }
			  else if(pXYArr[i].xyarr[0]<4.1)//后缓
			  {
				  _tcscpy(tmp,L"99999");
				  m_grid.SetItemText(i,5,tmp);
				  _stprintf(tmp,L"%0.8lf",pXYArr[i].xyarr[2]);
				  m_grid.SetItemText(i,4,tmp);
			  }
			  else//不完整缓
			  {
				  _stprintf(tmp,L"%0.8lf",pXYArr[i].xyarr[2]);
				  m_grid.SetItemText(i,4,tmp);
				  _stprintf(tmp,L"%0.3lf",pXYArr[i].xyarr[3]);
				  m_grid.SetItemText(i,5,tmp);
			  }
			  _stprintf(tmp,L"%d",i);
			  m_grid.SetItemText(i,0,tmp);
		  }
	  }
	  _stprintf(tmp,L"%d",i);
	  m_grid.SetItemText(i,0,tmp);
	  m_grid.SetColumnWidth(0,50);
	  m_grid.SetColumnWidth(1,70);
	  for(i=2; i<7; i++)
		  m_grid.SetColumnWidth(i,80);	   
	  for( i=0;i<m_grid.GetRowCount();i++)
		  m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	  m_grid.ResetScrollBars();
	  m_grid.SetFocusCell(1,1);

	  AddSzControl(m_grid,mdResize,mdResize);
	  UpdateData(false);
	  return TRUE;  // return TRUE unless you set the focus to a control
	  // EXCEPTION: OCX Property Pages should return FALSE
}

void CJMXYDLG::OnButtonXyadd() 
{
	// TODO: Add your control notification handler code here
	int i;
	CString tr;
	if(m_grid.GetRowCount()==1)          // 新建表格里没有行时，加1行
	{
		m_grid.SetRowCount(2);    
		for(i=1;i<m_grid.GetColumnCount();i++)
		{
			m_grid.SetItemText(1,i,L" ");
		}
		m_grid.SetItemText(1,0,L"1");
		m_grid.SetFocusCell(0,1);
		m_grid.Invalidate();
	}
	else
	{
		int nRow = m_grid.GetFocusCell().row; // 当前行

		if (nRow > 0)
		{ 
			Item.row = nRow;
			m_grid.InsertRow(_T("NEW"), nRow);

			// 置插入行之后的行标题(0列)为序号
			for(i=nRow; i<m_grid.GetRowCount(); i++)
			{
				CString str;
				Item.row=i;
				Item.col=0;
				str.Format(L"%d",i);
				Item.strText=str;
				m_grid.SetItem(&Item);
			}
			// 其它列为空
			for(i=1; i<m_grid.GetColumnCount(); i++) 
			{
				// 在当前行之后插入一行新数据(原来Grid类是在当前行插入新数据,所以需要修改一下)
				m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
				// 添加新行时,初始化指定数据项值 2005.07.27 ldf -->
				m_grid.SetItemText(nRow+1,i,L" ");

				// <-- 2005.07.27 ldf 
			}
			m_grid.SetFocusCell(nRow+1,1);
			m_grid.Invalidate();
		}
	}  
}

void CJMXYDLG::OnButtonXydel() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	//if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		if(nRow==m_grid.GetRowCount())
			nRow--;
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}		
}

void CJMXYDLG::OnOK() 
{
	// TODO: Add extra validation here	
	//	cdxCSizingDialog::OnOK();
	int i;
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcDbEntity *pEnt;
	for(i=1;i<m_XYNum;i++)
	{
		if(pXYArr[i].eID)
		{
			if(acdbOpenObject(pEnt,pXYArr[i].eID,AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
			{
				pEnt->erase();
				pEnt->close();
			}
		}
	}
	FormDrawXYarr();

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/	
	acDocManager->unlockDocument(acDocManager->curDocument());	
}

void CJMXYDLG::FormDrawXYarr()
{
	UpdateData(true);
	int NRow,i;
	NRow=0;
	for(i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);		   
		if(flag)//不空
			NRow++;
		else
			break;
	}//统计非空行数
	//	ads_printf(L"Nrow=%d\n",NRow);
	pXYArr.RemoveAll();
	if(NRow>0)
	{
		//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

		//(R1>R2)/////////////////////////////////////////////////////////////////////////
		//1:			  L		R    A         A              A                A        //
		//2:					L	 R         R              R1               R2       //
		//3:                                                  R2               R1       //
		//////////////////////////////////////////////////////////////////////////////////
		//4: 左右转
		//5: 起始切线方位角
		//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
		//7: 半径 R
		//8，9：线元起点坐标 N,E
		//最后一个线元为终点
		XYarray xray;
		xray.eID=0;
		xray.xyarr[0]=0;
		xray.xyarr[1]=_wtof(m_XYSX);
		xray.xyarr[2]=_wtof(m_XYSY);
		double dms=_wtof(m_XYSFwj);
		xray.xyarr[3]=rad(dms);
		xray.xyarr[4]=_wtof(m_XYSml);
		pXYArr.Add(xray);
		xray.xyarr[5]=rad(dms);;
		xray.xyarr[6]=_wtof(m_XYSml);
		xray.xyarr[8]=_wtof(m_XYSX);
		xray.xyarr[9]=_wtof(m_XYSY);
		double SR,ER,xylen;
		double XYEX,XYEY,XYEFwj,A;

		for(i=1;i<=NRow;i++)
		{
			if(m_grid.GetItemText(i, 1)=="左转")
				xray.xyarr[4]=-1;
			else
				xray.xyarr[4] =1;
			SR=_wtof(m_grid.GetItemText(i, 4));
			ER=_wtof(m_grid.GetItemText(i, 5));
			xylen = _wtof(m_grid.GetItemText(i, 2));
			A = _wtof(m_grid.GetItemText(i, 3));
			if(fabs(SR)>99990&&fabs(ER)>99990)//直线
			{
				xray.xyarr[0]=1;
				xray.xyarr[1]=xylen;
				xray.xyarr[2]=0;
				xray.xyarr[3]=0;
				xray.xyarr[7]=1e40;
			}
			else if(fabs(SR-ER)<0.001)//圆
			{
				xray.xyarr[0]=2;
				xray.xyarr[1]=SR;
				xray.xyarr[2]=xylen;
				xray.xyarr[3]=0;
				xray.xyarr[7]=SR;
			}
			else if(fabs(SR)>99990&&ER>0.0)//前缓
			{
				xray.xyarr[0]=3;
				if(A>0.01)
				{
					xray.xyarr[1]=A;
					xylen = A*A/ER;
				}
				else
					xray.xyarr[1]=sqrt(ER*xylen);
				xray.xyarr[2]=ER;
				xray.xyarr[3]=0;
				xray.xyarr[7]=ER;				
			}
			else if(fabs(ER)>99990&&SR>0.0)//后缓
			{
				xray.xyarr[0]=4;
				if(A>0.01)
				{
					xray.xyarr[1]=A;
					xylen = A*A/SR;
				}
				else
					xray.xyarr[1]=sqrt(SR*xylen);

				xray.xyarr[2]=SR;
				xray.xyarr[3]=0;
				xray.xyarr[7]=SR;				
			}
			else if(SR > ER)//不完整缓R大->R小
			{
				xray.xyarr[0]=5;
				if(A>0.01)
				{
					xray.xyarr[1]=A;
					xylen = fabs(A*A/SR-A*A/ER);
				}
				else
					xray.xyarr[1]=xray.xyarr[1]=sqrt(xylen*SR*ER/fabs(SR-ER));
				xray.xyarr[2]=SR;
				xray.xyarr[3]=ER;
				xray.xyarr[7]=0;		
			}
			else if(SR < ER)//不完整缓R小->R大
			{
				xray.xyarr[0]=6;
				if(A>0.01)
				{
					xray.xyarr[1]=A;
					xylen = fabs(A*A/SR-A*A/ER);
				}
				else
					xray.xyarr[1]=sqrt(xylen*SR*ER/fabs(SR-ER));
				xray.xyarr[2]=SR;
				xray.xyarr[3]=ER;
				xray.xyarr[7]=0;		
			}
			else
				ads_printf(L"数据有误!");
			pXYArr.Add(xray);
			CalXYXY(pXYArr[i].xyarr,xylen,XYEX,XYEY,XYEFwj);
			xray.xyarr[5]=XYEFwj;
			xray.xyarr[6]=pXYArr[i].xyarr[6]+xylen;
			xray.xyarr[8]=XYEX;
			xray.xyarr[9]=XYEY;
		}
		m_XYNum = pXYArr.GetSize()-1;
		pXYArr.Add(xray);
		//		ads_printf(L"xyn=%d\n",m_XYNum);
		for(i=1;i<=m_XYNum;i++)
		{
			if(pXYArr[i].xyarr[0]<1.1)
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,7);
			else if(pXYArr[i].xyarr[0]>2.1)
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,6);
			else
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,2);
		}

		for(i=1; i<=m_XYNum; i++)
			SetXYXdata(pXYArr[i].eID, pXYArr[i+1].eID,pXYArr[i].xyarr);
		
		/*double XMin = pJC->XYArray[0][8], XMax = 0.00, YMin = pJC->XYArray[0][9], YMax = 0.00;
		for (int i = 0; i < pJC->NJD; i++)
		{
			if (XMin > pJC->JDarray[i][2])
				XMin = pJC->JDarray[i][2];

			if (XMax < pJC->JDarray[i][2])
				XMax = pJC->JDarray[i][2];

			if (YMin > pJC->JDarray[i][1])
				YMin = pJC->JDarray[i][1];

			if (YMax < pJC->JDarray[i][1])
				YMax = pJC->JDarray[i][1];
		}


		///*pSJJX = NULL;*/

		/*BAS_DRAW_FUN_JYX MZOOM;
		MZOOM.mZOOM(XMin, YMin, XMax, YMax);
		//TieluID = BAS_DRAW_FUN::AddEntityToDbs(pSJEX);*/
	
		/*JD_CENTER* pJC = NULL;
		pJC = new JD_CENTER;
		pJC->XYArrayToJD();
		AcDbEntity* pEntity = NULL;
		pJC->setLayer(L"公路平面设计专用层");
		AcDbObjectId XianLuID = NULL;
		XianLuID = BAS_DRAW_FUN::AddEntityToDbs(pJC);*/
		/*if (XianLuID != NULL)
			XianLuArray_ZDCTZY.push_back(XianLuID);*/


	}
}

void CJMXYDLG::OnButtonInputfile() 
{
	// TODO: Add your control notification handler code here
	int i; 
	ACHAR tmp[256];
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		//	Path=WorkDir.JdCenBackUp;
		CFileDialog FDlg(TRUE,L"*.xy");
		FDlg.m_ofn.Flags = OFN_CREATEPROMPT;
		FDlg.m_ofn.lpstrFilter=L"*.xy\0*.xy\0\0";		

		if(FDlg.DoModal()==IDOK)
		{
			Path=FDlg.GetPathName();
		}
		else return;
	}
	FILE *fp=_wfopen(Path,L"r+");
	m_XYFNAME = Path;
	if(fp==NULL)return;
	int num;
	ACHAR str[256];
	pXYArr.RemoveAll();
	m_XYNum = 0;
	fwscanf(fp,L"%d",&num);

	if(num > 0 )  	
	{
		ACHAR xy[5][256];
		int j;
		int i;
		for(j=0; j<5; j++)
			fwscanf(fp,L"%s", xy[j]);

		m_XYSX=xy[0];
		m_XYSY=xy[1];
		m_XYSFwj=xy[2];
		m_XYSml = xy[3];

		m_grid.SetRowCount(num);

		Item.mask = GVIF_TEXT|GVIF_FORMAT;
		Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
			DT_END_ELLIPSIS|DT_WORDBREAK;
		Item.lfFont=font;

		for(int k=0;k<m_grid.GetColumnCount();k++)
		{
			for(i=0;i<m_grid.GetRowCount();i++) 
			{
				Item.row = i;
				Item.col = k;
				m_grid.SetItem(&Item);
			}  
		}
		m_grid.SetItemText(0,0,L"序 号");
		m_grid.SetItemText(0,1,L"转 向");
		m_grid.SetItemText(0,2,L"线元长度");
		m_grid.SetItemText(0,3,L"A  值");
		m_grid.SetItemText(0,4,L"起点半径");
		m_grid.SetItemText(0,5,L"终点半径");	   	   
		for( i=0; i<num-1; i++)
		{
			for(j=0; j<5; j++)
			{
				fwscanf(fp,L"%s", xy[j]);
				m_grid.SetItemText(i+1,j+1,xy[j]);				
			}

			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}


	}
	_stprintf(tmp,L"%d",i);
	m_grid.SetItemText(i,0,tmp);
	m_grid.SetColumnWidth(0,50);
	m_grid.SetColumnWidth(1,70);	   

	for(i=2; i<7; i++)
		m_grid.SetColumnWidth(i,80);	   
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	AddSzControl(m_grid,mdResize,mdResize);
	UpdateData(false);
	fclose(fp);

}

void CJMXYDLG::OnButtonSave() //存盘
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int num=0;
	for(int i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);		   
		if(flag)//不空
			num++;
		else
			break;
	}//统计非空行数

	if(num<=0)return;
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		//	Path=WorkDir.JdCenBackUp;
		CFileDialog FDlg(FALSE,L"*.xy",NULL,OFN_OVERWRITEPROMPT);
		FDlg.m_ofn.Flags = OFN_CREATEPROMPT;
		FDlg.m_ofn.Flags = OFN_OVERWRITEPROMPT;
		FDlg.m_ofn.lpstrFilter=L"*.xy\0*.xy\0\0";		

		if(FDlg.DoModal()==IDOK)
		{
			Path=FDlg.GetPathName();
		}
		else return;
	}
	m_XYFNAME = Path;
	FILE *fp=_wfopen(Path,L"w+");
	if(fp==NULL)return;

	fwprintf(fp,L"%d\n",num+1);
	fwprintf(fp,L"%s %s %s %s 0 \n",m_XYSX,m_XYSY,m_XYSFwj,m_XYSml);
	CString str;
	for(int  i=1;i<num+1;i++)
	{
		for(int j=1;j<6;j++)
		{
			str=m_grid.GetItemText(i,j);
			if(str=="" || str==" ")str="-";
			fwprintf(fp,L"%s ",str);
		}
		fwprintf(fp,L"\n");
	}
	fclose(fp);
	UpdateData(false);
}





















