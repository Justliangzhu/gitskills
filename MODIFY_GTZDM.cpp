// MODIFY_GTZDM.cpp : implementation file
//

#include "stdafx.h"
#include "stdarx.h"
#include "resource.h"
#include "MODIFY_GTZDM.h"
#include "gtzdm.h"
#include "JD_CENTER.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// MODIFY_GTZDM property page
//
/////////
//返回从NSTART开始的N个字符
void mid(ACHAR *s,int nstart,int n,ACHAR *r)
{
	int length,i;
	length=_tcslen(s);
	for(i=0;i<n;i++)
	{
		r[i]=s[nstart+i-1];
	}
	r[n]='\0';
}
//找出字符c在字符串s中的位置,如返回值为0，表示找不到
int instr(ACHAR *s,ACHAR c)
{
	int slen,i=0,j=0;
	slen=_tcslen(s);
	for(i=0;i<slen;i++)
	{
		if(s[i]==c) 
		{
			j=i+1;
			break;
		}
	}
	return j;
}
//返回字符串最左边的N个字符
void left(int n,ACHAR *s,ACHAR *t)
{
	int i;
	i=_tcslen(s);
	if(i<n) n=i;
	for(i=0;i<n;i++)
	{
		t[i]=s[i];
	}
	t[n]='\0';
}
/*---------------------------------------------------------------------*/
//返回字符串最右边的N个字符
void right(int n,ACHAR *s,ACHAR *t)
{
	int i,len,j;
	len=_tcslen(s);
	if(len<n) n=len;
	for(i=len-n,j=0;i<=len-1;i++,j++)
	{
		t[j]=s[i];
	}
	t[n]='\0';
}
//去掉字符串两端的空格
void trim(ACHAR *s)
{
	ACHAR t[100];
	int length,nzempty,nyempty,i;

	length=_tcslen(s);
	nzempty=0;
	for(i=0;i<length;i++)
	{
		if(s[i]==' ')    
			nzempty=nzempty+1;
		else
			break;
	}

	nyempty=0;
	for(i=length-1;i>-1;i--)
	{
		if(s[i]==' ')    
			nyempty=nyempty+1;
		else
			break;
	}

	left(length-nyempty,s,t);
	right(length-nyempty-nzempty,t,s);
}

bool checkckml(ACHAR *a)
{
	//检查后保证：
	//要有"K"存在
	// K之后的第一个字符一定是数字
	// K之后只能有一个加号和'.'号，其它的都是数字
	// 最后一个字符不能是+或.
	int length,i,j,npoint=0,nplus=0,nerrchr=0;
	ACHAR c[30];
	trim(a);

	i=instr(a,'k');
	if(i==0) i=instr(a,'K');
	//无k号
	if(i==0)
	{
		MessageBox(GetActiveWindow(), L"输入里程应带冠号", L"错误信息",0); 
		return false;
	}

	// K之后的第一个字符一定是数字
	mid(a,i+1,1,c);
	j=toascii(c[0]);
	if(j<48||j>57)
	{
		MessageBox(GetActiveWindow(), L"里程冠号最后\n一个字符应是 k", L"错误信息",0); 

		return false;
	}

	// 最后一个字符不能是+或.
	length=_tcslen(a);
	if(a[length-1]=='+'||a[length-1]=='.') nerrchr=nerrchr+1;

	// K之后只能有一个加号和'.'号，其它的都是数字
	right(length-i,a,c);
	length=_tcslen(c);

	for(i=0;i<length;i++)
	{
		j=toascii(c[i]);
		if(c[i]=='.')
			npoint=npoint+1;
		else if(c[i]=='+')
			nplus=nplus+1;
		else if(j<48||j>57)
			nerrchr=nerrchr+1;
	}

	if(npoint>1||nplus>1||nerrchr>0)
	{
		MessageBox(GetActiveWindow(), L"输入里程错误", L"错误信息",0); 
		return false;
	}

	return true;
}
bool checkreal(ACHAR *a)
{
	//检查后保证：
	//第一个字符不能是小数点
	//最后一个字符不能是小数点
	//小数点不能超过两个
	//不能有数字之外的字符
	int length,i,npoint=0,nerrchr=0,nascii, n0=0;
	ACHAR c;
	trim(a);
	length=_tcslen(a);

	if(a[0]=='.') nerrchr=1+nerrchr;
	if(a[length-1]=='.') nerrchr=1+nerrchr;

	for(i=0;i<length;i++)
	{
		c=a[i];
		nascii=toascii(c);	   
		if(c=='.') {
			npoint=npoint+1;  //小数点个数
		}
		else if (c=='-'){
			n0++;
			if (i==0)
				continue;
			else 
				nerrchr++;
		}
		else if(nascii<48||nascii>57){
			nerrchr=nerrchr+1;
		}
	}
	if(npoint>1||nerrchr>0||n0>1)
	{
		MessageBox(GetActiveWindow(), L"输入数据错误", L"错误信息",0); 
		return false;
	}
	return true;
}
bool checkZDM(int nCol,ACHAR *a)
{
	bool rt;
	if (nCol==1)
		rt = checkckml(a);
	else
		rt = checkreal(a);
	return rt;
}

////////
////////
BEGIN_MESSAGE_MAP(CZDMGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CZDMGridCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CZDMGridCtrl::CZDMGridCtrl()
{
}

CZDMGridCtrl::~CZDMGridCtrl()
{
}

void CZDMGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
	ACHAR tmp[30];
	_tcscpy(tmp,str);
	/*
	if(checkZDM(nCol,tmp)) ;
	else 
	{
	return;  
	}*/

	CGridCtrl::OnEndEditCell(nRow,nCol,str);
}
//

IMPLEMENT_DYNCREATE(MODIFY_GTZDM, cdxCSizingDialog)

	MODIFY_GTZDM::MODIFY_GTZDM() : cdxCSizingDialog(MODIFY_GTZDM::IDD)
{
	//{{AFX_DATA_INIT(MODIFY_GTZDM)
	// NOTE: the ClassWizard will add member initialization here
	//	acedAlert(L"MODIFY_GTZDM");

	//}}AFX_DATA_INIT
}

MODIFY_GTZDM::~MODIFY_GTZDM()
{
	//	acedAlert(L"BOOL MODIFY_GTZDM::~MODIFY_GTZDM()");	
	if(pzdmLine)
	{
		pzdmLine->close();
		//		if(ZDMDOC==NULL)ZDMDOC=acDocManager->curDocument();
		//		acDocManager->sendStringToExecute(ZDMDOC, L"regen\n",false,true);
	}

}

void MODIFY_GTZDM::DoDataExchange(CDataExchange* pDX)
{
	//	acedAlert(L"BOOL MODIFY_GTZDM::DoDataExchange()");

	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MODIFY_GTZDM)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//	DDX_GridControl(pDX, IDC_LIST_ZP,m_grid);//变坡点
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MODIFY_GTZDM, cdxCSizingDialog)
	//{{AFX_MSG_MAP(MODIFY_GTZDM)
	ON_BN_CLICKED(IDC_ADDBPD, OnAddbpd)
	ON_BN_CLICKED(IDC_DELBPD, OnDelbpd)
	ON_BN_CLICKED(IDC_BUTTONdraw, OnBUTTONdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MODIFY_GTZDM message handlers


void MODIFY_GTZDM::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

void MODIFY_GTZDM::OnAddbpd() 
{
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
		for( i=1;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);

		/*
		if(m_grid.GetRowCount()>3)
		{
		if(nRow==m_grid.GetRowCount()-2)
		{
		for( i=1;i<m_grid.GetColumnCount();i++) 
		m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
		}
		else
		for( i=1;i<m_grid.GetColumnCount();i++) 
		m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
		}*/

		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}
	//设置第3、4、6列只读
	for (int i = 1;i < (m_grid.GetRowCount()-1);i++)
	{
		m_grid.SetItemState(i,3,GVIS_READONLY);
		m_grid.SetItemBkColour(i,3,RGB(128,128,128));
		m_grid.SetItemState(i,4,GVIS_READONLY);
		m_grid.SetItemBkColour(i,4,RGB(128,128,128));
		if(i > 1)
		{
			m_grid.SetItemState(i,6,GVIS_READONLY);
			m_grid.SetItemBkColour(i,6,RGB(128,128,128));
		}
	}
}

void MODIFY_GTZDM::OnDelbpd() 
{
	// TODO: Add your control notification handler code here
	CString str;
	int nRow = m_grid.GetFocusCell().row;

	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();		
	}
	//设置第3、4、6列只读
	for (int i = 1;i < (m_grid.GetRowCount()-1);i++)
	{
		m_grid.SetItemState(i,3,GVIS_READONLY);
		m_grid.SetItemBkColour(i,3,RGB(128,128,128));
		m_grid.SetItemState(i,4,GVIS_READONLY);
		m_grid.SetItemBkColour(i,4,RGB(128,128,128));
		if(i > 1)
		{
			m_grid.SetItemState(i,6,GVIS_READONLY);
			m_grid.SetItemBkColour(i,6,RGB(128,128,128));
		}
	}
}


BOOL MODIFY_GTZDM::OnInitDialog()
{
	LOGFONT font={12, 0, 0, 0, 400, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"Arial"};
	COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
	//	COLORREF fixbkclr=RGB(155,188,0);

	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;


	cdxCSizingDialog::OnInitDialog(0);


	CRect rect;
	GetClientRect(rect);
	rect.left += 10;
	rect.top += 80;
	rect.right -= 10;
	rect.bottom -= 10;
	m_grid.Create(rect, this, 900);

	pzdmLine=NULL;
	if(acdbOpenObject(pzdmLine,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"无法打开纵坡实体!");
		return false;
	}
	pzdmLine->close();

	// TODO: Add extra initialization here
	ACHAR  tmp[80];
	double tmph;
	int i;

	m_grid.SetRowCount(pzdmLine->NBPD+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(7);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	//	m_grid.SetFixedBkColor(fixbkclr);

	for (int k = 0; k < m_grid.GetColumnCount(); k++)
	{
		for (i = 0; i < m_grid.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}
		m_grid.SetItemState(i - 1, k, GVIS_READONLY);
	}

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.row = 0;
	Item.col = 0;
	m_grid.SetItem(&Item);
	Item.col = 1;
	m_grid.SetItem(&Item);
	Item.col = 2;
	m_grid.SetItem(&Item);
	Item.col = 3;
	m_grid.SetItem(&Item);
	Item.col = 4;
	m_grid.SetItem(&Item);
	Item.col = 5;
	m_grid.SetItem(&Item);

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;

	m_grid.SetItemText(0,0, L"序号");
	m_grid.SetItemText(0,1, L"里程");
	m_grid.SetItemText(0,2, L"高程");
	m_grid.SetItemText(0,3, L"坡度");
	m_grid.SetItemText(0,4, L"坡长");
	m_grid.SetItemText(0,5, L"竖曲线半径");
	m_grid.SetItemText(0,6, L"竖曲线长度");

	i=0;
	double PD=0.0,PC,BPD;
	if(pzdmLine->NBPD != 0)
	{
		double Bdml,Edml;
		_stprintf(tmp , L"%d" , pzdmLine->Xsw);
		m_grid.SetItemText(1,6,tmp);
		for (i = 0; i < pzdmLine->NBPD; i++)
		{
			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i + 1, 0, tmp);
			ACHAR Bckml[80];
			tmph = xytotable(pzdmLine->BPD_array[i][0], Bckml, pzdmLine->BPD_array[i][1]);
			m_grid.SetItemText(i + 1, 1, Bckml);

			ads_rtos(tmph, 2, pzdmLine->DesHXSW, tmp);
			//_stprintf(tmp , "%.2lf" ,tmph);
			m_grid.SetItemText(i + 1, 2, tmp);


			if (i < pzdmLine->NBPD - 1)
			{
				double H;
				ACHAR ckml[80];
				H = xytotable(pzdmLine->BPD_array[i + 1][0], ckml, pzdmLine->BPD_array[i + 1][1]);
				Bdml = _wtof(Bckml);
				Edml = _wtof(ckml);
				double Bml = pzdmLine->TYLC(Bdml);
				double Eml = pzdmLine->TYLC(Edml);
				BPD = PD;
				PD = (H - tmph) / (Eml - Bml);
				PC = Eml - Bml;
			}
			else
			{
				PD=PC=0.0;
			}
			ads_rtos(PD*100.0,2,pzdmLine->Xsw,tmp);
			m_grid.SetItemText(i + 1, 3, tmp);//坡度

			ads_rtos(PC,2,pzdmLine->DesHXSW,tmp);
			m_grid.SetItemText(i + 1, 4, tmp);//坡长

			if(i>0)
			{
				ads_rtos(pzdmLine->BPD_array[i][2], 2, 3, tmp);
			}
			else//i==0
				_stprintf(tmp , L"%d" ,pzdmLine->DesHXSW);

			m_grid.SetItemText(i + 1, 5, tmp);

			if(i>0)
			{
				//计算竖曲线长
				double verlen,R,di;
				R = pzdmLine->BPD_array[i][2];
				di = fabs(PD - BPD)*100.0;
				verlen = 2 * R*di / 200.0;
				ads_rtos(verlen, 2, 3, tmp);
				m_grid.SetItemText(i + 1, 6, tmp);

			}

		}     
	}
	_stprintf(tmp , L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	/////////////
	CRect m_sht;
	GetDlgItem(IDC_LIST_ZP)->GetClientRect(&m_sht);
	int w=(int)((m_sht.right-m_sht.left)/12);
	/////////////////	
	m_grid.SetColumnWidth(0,w);
	m_grid.SetColumnWidth(1,2.2*w);
	m_grid.SetColumnWidth(2,1.7*w);
	m_grid.SetColumnWidth(3,1.7*w);
	m_grid.SetColumnWidth(4,1.7*w);
	m_grid.SetColumnWidth(5,2*w);
	m_grid.SetColumnWidth(6,2*w);
	m_grid.SetRowHeight(0,20);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	//设置第3、4、6列只读
	for (int i = 1;i < (m_grid.GetRowCount()-1);i++)
	{
		m_grid.SetItemState(i,3,GVIS_READONLY);
		m_grid.SetItemBkColour(i,3,RGB(128,128,128));
		m_grid.SetItemState(i,4,GVIS_READONLY);
		m_grid.SetItemBkColour(i,4,RGB(128,128,128));
		if(i > 1)
		{
			m_grid.SetItemState(i,6,GVIS_READONLY);
			m_grid.SetItemBkColour(i,6,RGB(128,128,128));
		}
	}
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	UpdateData(true);	

	AddSzControl(m_grid,mdResize,mdResize);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

double MODIFY_GTZDM::xytotable(double x, ACHAR *xlc, double y)
{
	CString  GH;
	double LC = pzdmLine->Trs_Coor_System(x, MILE, TO_SLOPE);
	//double LC=(x-pzdmLine->X0)*pzdmLine->HBVSCALE+pzdmLine->K0;
	double dxlc = pzdmLine->XLC1(LC);
	_stprintf(xlc, L"%0.3lf",dxlc);
	double h = pzdmLine->Trs_Coor_System(y, LEVEL, TO_SLOPE);
	//h=y-pzdmLine->Y0+pzdmLine->H0;
	return h;
}

void MODIFY_GTZDM::tabletoxy(int iBPD,double xlc,double h,double R)
{
	double tylc;
	tylc=xlc;
	pzdmLine->BPD_array[iBPD][0] = pzdmLine->Trs_Coor_System(tylc, MILE, TO_DRAW);
	pzdmLine->BPD_array[iBPD][1] = pzdmLine->Trs_Coor_System(h, LEVEL, TO_DRAW);
	//pzdmLine->BPD_array[iBPD][0]=(tylc-pzdmLine->K0)/pzdmLine->HBVSCALE+pzdmLine->X0;
	//pzdmLine->BPD_array[iBPD][1]=h-pzdmLine->H0+pzdmLine->Y0;
	pzdmLine->BPD_array[iBPD][2] = R;
}

void MODIFY_GTZDM::OnOK() 
{
	ShuaXin();
	CDialog::OnOK();
	//// TODO: Add extra validation here
	//CString b,ctmp;
	//int i;
	//ACHAR tmp[30],tmp1[30],tmp2[30],PDstr[30],PCstr[30],BPDstr[30],BPCstr[30];
	//double bpddml;
	//double xlc=0,BGC;

	////	UpdateData(true);	
	//CDialog::OnOK();

	//if(acdbOpenObject(pzdmLine,eId, AcDb::kForWrite)!=Acad::eOk)
	//{
	//	ads_alert(L"无法打开纵坡实体!");
	//	return;
	//}
	//pzdmLine->assertWriteEnabled();
	//int NRow;
	//NRow=0;
	//for(i=1; i<m_grid.GetRowCount(); i++)
	//{
	//	int flag;
	//	m_grid.RowIsEpt(i,m_grid.GetColumnCount()-2,flag);

	//	if(flag)//不空
	//		NRow++;
	//	else
	//		break;
	//}//统计非空行数

	//pzdmLine->NBPD= NRow;  
	//xlc=0;
	//_tcscpy(tmp2, m_grid.GetItemText(1, 5));//取出设计标高小数位数
	//if(_wtoi(tmp2)>0)
	//{
	//	pzdmLine->DesHXSW = _wtoi(tmp2);
	//	pORI_SCALE.DesHXSW = pzdmLine->DesHXSW;
	//}
	//_tcscpy(tmp2, m_grid.GetItemText(1, 6));//取出坡度小数位数
	//if(_wtoi(tmp2)>=0)
	//{
	//	pzdmLine->Xsw= _wtoi(tmp2);
	//	pORI_SCALE.XSW = pzdmLine->Xsw;
	//}
	///*
	//_tcscpy(tmp2, m_grid.GetItemText(1, 7));//取出坡度小数位数
	//if(_wtoi(tmp2)>=0)
	//{
	//pzdmLine->Xsw= _wtoi(tmp2);
	//pORI_SCALE.XSW = pzdmLine->Xsw;
	//}*/


	//for(i=1; i<=pzdmLine->NBPD; i++)
	//{
	//	_tcscpy(tmp, m_grid.GetItemText(i, 1));
	//	_tcscpy(tmp1, m_grid.GetItemText(i, 2));
	//	_tcscpy(PDstr, m_grid.GetItemText(i, 3));
	//	_tcscpy(PCstr, m_grid.GetItemText(i, 4));
	//	_tcscpy(tmp2, m_grid.GetItemText(i, 5));
	//	bpddml= _wtof(tmp);
	//	if(i>1 && m_grid.IsEpt(i,1))//没输入里程，根据坡长和前一变坡点里程计算
	//	{
	//		_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
	//		if(!m_grid.IsEpt(i-1,4))
	//		{
	//			double BPDcml = xlc + _wtof(BPCstr);
	//			CString  GH;
	//			//	            double dxlc=pzLinep->XLC(BPDcml,GH);
	//			//	            _tcscpy(tmp,pzLinep->LCchr(GH,dxlc,3));
	//			bpddml=pzdmLine->XLC1(BPDcml);
	//		}
	//		else
	//		{
	//			ACHAR mes[80];
	//			_stprintf(mes, L"无法计算第%d个变坡点里程!",i);
	//			ads_alert(mes);
	//			continue;
	//		}
	//	}

	//	if(i>1 && m_grid.IsEpt(i,2))//没输入高程，根据前一变坡点坡度,坡长计算
	//	{
	//		_tcscpy(BPDstr, m_grid.GetItemText(i-1, 3));
	//		_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
	//		if(!m_grid.IsEpt(i-1,3) && !m_grid.IsEpt(i-1,4))
	//		{
	//			double BPDgc = BGC + _wtof(BPDstr)/100.0*_wtof(BPCstr); 
	//			ads_rtos(BPDgc,2,pzdmLine->DesHXSW,tmp1);
	//			//	_stprintf(tmp1, L"%0.3lf",);	
	//		}
	//		else
	//		{
	//			ACHAR mes[80];
	//			_stprintf(mes, L"无法计算第%d个变坡点高程!",i);
	//			ads_alert(mes);
	//			continue;
	//		}
	//	}
	//	else if(i>1)
	//	{
	//		_tcscpy(BPDstr, m_grid.GetItemText(i-1, 3));
	//		_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
	//		if(m_grid.IsEpt(i-1,2))//高程为空
	//		{
	//			if(!m_grid.IsEpt(i-1,3) && !m_grid.IsEpt(i-1,4))
	//			{
	//				double BPDgc = BGC + _wtof(BPDstr)/100.0*_wtof(BPCstr); 
	//				ads_rtos(BPDgc,2,pzdmLine->DesHXSW,tmp1);
	//			}	
	//		}
	//	}
	//	xlc=pzdmLine->TYLC(bpddml);
	//	BGC = _wtof(tmp1);
	//	tabletoxy(i-1,xlc,_wtof(tmp1),_wtof(tmp2));
	//}

	//if(!m_grid.IsEpt(pzdmLine->NBPD,3) && !m_grid.IsEpt(pzdmLine->NBPD,4))//最后一个变坡点的坡度和坡长仍然有值,增加一个变坡点
	//{
	//	_tcscpy(PDstr, m_grid.GetItemText(pzdmLine->NBPD, 3));//坡度
	//	_tcscpy(PCstr, m_grid.GetItemText(pzdmLine->NBPD, 4));//坡长
	//	if(_wtof(PCstr)>0.1)//有坡长
	//	{
	//		double cml = xlc + _wtof(PCstr);
	//		double BPDgc = BGC + _wtof(PDstr)/100.0*_wtof(PCstr);
	//		pzdmLine->NBPD++;
	//		tabletoxy(pzdmLine->NBPD-1,cml,BPDgc,0.0);//终点
	//	}
	//}
	//pzdmLine->close();	
}


BOOL MODIFY_GTZDM::PreTranslateMessage(MSG* pMsg) //////////////////新
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
		if(pMsg->wParam==VK_RETURN||pMsg->wParam==VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return true;
		}
		return cdxCSizingDialog::PreTranslateMessage(pMsg);
}

void MODIFY_GTZDM::OnBUTTONdraw() //绘图
{
	ShuaXin();
	acedGetAcadDwgView()->SetFocus();
	//更新控件
	COLORREF txtbkclr=RGB(0XFF, 0XFF,0XE0);
	ACHAR  tmp[80];
	double tmph;
	int i;

	m_grid.SetRowCount(pzdmLine->NBPD+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(7);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	//	m_grid.SetFixedBkColor(fixbkclr);

	for (int k = 0; k < m_grid.GetColumnCount(); k++)
	{
		for (i = 0; i < m_grid.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}
		m_grid.SetItemState(i - 1, k, GVIS_READONLY);
	}

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.row = 0;
	Item.col = 0;
	m_grid.SetItem(&Item);
	Item.col = 1;
	m_grid.SetItem(&Item);
	Item.col = 2;
	m_grid.SetItem(&Item);
	Item.col = 3;
	m_grid.SetItem(&Item);
	Item.col = 4;
	m_grid.SetItem(&Item);
	Item.col = 5;
	m_grid.SetItem(&Item);

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;

	m_grid.SetItemText(0,0, L"序号");
	m_grid.SetItemText(0,1, L"里程");
	m_grid.SetItemText(0,2, L"高程");
	m_grid.SetItemText(0,3, L"坡度");
	m_grid.SetItemText(0,4, L"坡长");
	m_grid.SetItemText(0,5, L"竖曲线半径");
	m_grid.SetItemText(0,6, L"长度");

	i=0;
	double PD=0.0,PC,BPD;
	if(pzdmLine->NBPD != 0)
	{
		double Bdml,Edml;
		_stprintf(tmp , L"%d" , pzdmLine->Xsw);
		m_grid.SetItemText(1,6,tmp);
		for (i = 0; i < pzdmLine->NBPD; i++)
		{
			_stprintf(tmp , L"%d" , i+1);
			m_grid.SetItemText(i + 1, 0, tmp);
			ACHAR Bckml[80];
			tmph = xytotable(pzdmLine->BPD_array[i][0], Bckml, pzdmLine->BPD_array[i][1]);
			m_grid.SetItemText(i + 1, 1, Bckml);

			ads_rtos(tmph, 2, pzdmLine->DesHXSW, tmp);
			//_stprintf(tmp , "%.2lf" ,tmph);
			m_grid.SetItemText(i + 1, 2, tmp);


			if (i < pzdmLine->NBPD - 1)
			{
				double H;
				ACHAR ckml[80];
				H = xytotable(pzdmLine->BPD_array[i + 1][0], ckml, pzdmLine->BPD_array[i + 1][1]);
				Bdml = _wtof(Bckml);
				Edml = _wtof(ckml);
				double Bml = pzdmLine->TYLC(Bdml);
				double Eml = pzdmLine->TYLC(Edml);
				BPD = PD;
				PD = (H - tmph) / (Eml - Bml);
				PC = Eml - Bml;
			}
			else
			{
				PD=PC=0.0;
			}
			ads_rtos(PD*100.0,2,pzdmLine->Xsw,tmp);
			m_grid.SetItemText(i + 1, 3, tmp);//坡度

			ads_rtos(PC,2,pzdmLine->DesHXSW,tmp);
			m_grid.SetItemText(i + 1, 4, tmp);//坡长

			if(i>0)
			{
				ads_rtos(pzdmLine->BPD_array[i][2], 2, 3, tmp);
			}
			else//i==0
				_stprintf(tmp , L"%d" ,pzdmLine->DesHXSW);

			m_grid.SetItemText(i + 1, 5, tmp);

			if(i>0)
			{
				//计算竖曲线长
				double verlen,R,di;
				R = pzdmLine->BPD_array[i][2];
				di = fabs(PD - BPD)*100.0;
				verlen = 2 * R*di / 200.0;
				ads_rtos(verlen, 2, 3, tmp);
				m_grid.SetItemText(i + 1, 6, tmp);

			}
			//m_grid.Refresh();
			m_grid.Invalidate();	
		}     
	}
	_stprintf(tmp , L"%d" , i+1);
	m_grid.SetItemText(i+1,0,tmp);

	/////////////
	CRect m_sht;
	GetDlgItem(IDC_LIST_ZP)->GetClientRect(&m_sht);
	int w=(int)((m_sht.right-m_sht.left)/12);
	/////////////////	
	m_grid.SetColumnWidth(0,w);
	m_grid.SetColumnWidth(1,2.2*w);
	m_grid.SetColumnWidth(2,1.7*w);
	m_grid.SetColumnWidth(3,1.7*w);
	m_grid.SetColumnWidth(4,1.7*w);
	m_grid.SetColumnWidth(5,2*w);
	m_grid.SetColumnWidth(6,2*w);
	m_grid.SetRowHeight(0, DEFAULTCOLHEI);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	return;
	//// TODO: Add your control notification handler code here
	//CString b,ctmp;
	//int i;
	//ACHAR tmp[30],tmp1[30],tmp2[30],PDstr[30],PCstr[30],BPDstr[30],BPCstr[30];
	//double xlc=0,BGC;

	//UpdateData(true);	
	//EnableWindow(false);
	//acedGetAcadDwgView()->SetFocus();	
	//acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//if(acdbOpenObject(pzdmLine,eId, AcDb::kForWrite)!=Acad::eOk)
	//{
	//	ads_alert(L"无法打开纵坡实体!");
	//	return;
	//}
	//pzdmLine->assertWriteEnabled();

	//int NRow;
	//NRow=0;
	//for(i=1; i<m_grid.GetRowCount(); i++)
	//{
	//	int flag;
	//	m_grid.RowIsEpt(i,m_grid.GetColumnCount()-2,flag);

	//	if(flag)//不空
	//		NRow++;
	//	else
	//		break;
	//}//统计非空行数
	//pzdmLine->NBPD= NRow;//m_grid.GetRowCount() - 2;  
	//xlc=0;
	//_tcscpy(tmp2, m_grid.GetItemText(1, 5));//取出设计标高小数位数
	//if(_wtoi(tmp2)>0)
	//{
	//	pzdmLine->DesHXSW = _wtoi(tmp2);
	//	pORI_SCALE.DesHXSW = pzdmLine->DesHXSW;
	//}
	//for(i=1; i<=pzdmLine->NBPD; i++)
	//{
	//	_tcscpy(tmp, m_grid.GetItemText(i, 1));
	//	_tcscpy(tmp1, m_grid.GetItemText(i, 2));
	//	_tcscpy(PDstr, m_grid.GetItemText(i, 3));
	//	_tcscpy(PCstr, m_grid.GetItemText(i, 4));
	//	_tcscpy(tmp2, m_grid.GetItemText(i, 5));

	//	if(i>1 && m_grid.IsEpt(i,1))//没输入里程，根据坡长和前一变坡点里程计算
	//	{
	//		_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
	//		if(_tcscmp(BPCstr, L"")!=0)
	//		{
	//			double BPDcml = xlc + _wtof(BPCstr);
	//			CString  GH;
	//			double dxlc=pzdmLine->XLC(BPDcml,GH);
	//			_tcscpy(tmp,pzdmLine->LCchr(GH,dxlc,3));	
	//		}
	//		else
	//		{
	//			ACHAR mes[80];
	//			_stprintf(mes, L"无法计算第%d个变坡点里程!",i);
	//			ads_alert(mes);
	//			continue;
	//		}
	//	}

	//	if(i>1 && m_grid.IsEpt(i,2))//没输入高程，根据前一变坡点坡度,坡长计算
	//	{
	//		_tcscpy(BPDstr, m_grid.GetItemText(i-1, 3));
	//		_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
	//		if(m_grid.IsEpt(i-1,3) && m_grid.IsEpt(i-1,4))
	//		{
	//			double BPDgc = BGC + _wtof(BPDstr)/100.0*_wtof(BPCstr); 
	//			ads_rtos(BPDgc,2,pzdmLine->DesHXSW,tmp1);
	//			//	_stprintf(tmp1, L"%0.3lf",BPDgc);	
	//		}
	//		else
	//		{
	//			ACHAR mes[80];
	//			_stprintf(mes, L"无法计算第%d个变坡点高程!",i);
	//			ads_alert(mes);
	//			continue;
	//		}
	//	}
	//	xlc=pzdmLine->TYLC(_wtof(tmp));
	//	BGC = _wtof(tmp1);
	//	//		ads_printf(L"%d %lf %lf %lf\n",i,xlc,_wtof(tmp1),_wtof(tmp2));
	//	tabletoxy(i-1,xlc,_wtof(tmp1),_wtof(tmp2));
	//}
	//pzdmLine->close();
	//acDocManager->unlockDocument(acDocManager->curDocument());
	//EnableWindow(true);
}
void MODIFY_GTZDM::ShuaXin()
{
	CString b,ctmp;
	int i;
	ACHAR tmp[30],tmp1[30],tmp2[30],PDstr[30],PCstr[30],BPDstr[30],BPCstr[30];
	double bpddml;
	double xlc=0,BGC;
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if(acdbOpenObject(pzdmLine,eId, AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"无法打开纵坡实体!");
		return;
	}
	pzdmLine->assertWriteEnabled();
	int NRow;
	NRow=0;
	for(i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount()-2,flag);

		if(flag)//不空
			NRow++;
		else
			break;
	}//统计非空行数

	pzdmLine->NBPD= NRow;  
	xlc=0;
	_tcscpy(tmp2, m_grid.GetItemText(1, 5));//取出设计标高小数位数
	if(_wtoi(tmp2)>0)
	{
		pzdmLine->DesHXSW = _wtoi(tmp2);
		pORI_SCALE.DesHXSW = pzdmLine->DesHXSW;
	}
	_tcscpy(tmp2, m_grid.GetItemText(1, 6));//取出坡度小数位数
	if(_wtoi(tmp2)>=0)
	{
		pzdmLine->Xsw= _wtoi(tmp2);
		pORI_SCALE.XSW = pzdmLine->Xsw;
	}
	/*
	_tcscpy(tmp2, m_grid.GetItemText(1, 7));//取出坡度小数位数
	if(_wtoi(tmp2)>=0)
	{
	pzdmLine->Xsw= _wtoi(tmp2);
	pORI_SCALE.XSW = pzdmLine->Xsw;
	}*/


	for(i=1; i<=pzdmLine->NBPD; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		_tcscpy(tmp1, m_grid.GetItemText(i, 2));
		_tcscpy(PDstr, m_grid.GetItemText(i, 3));
		_tcscpy(PCstr, m_grid.GetItemText(i, 4));
		_tcscpy(tmp2, m_grid.GetItemText(i, 5));
		bpddml= _wtof(tmp);
		if(i>1 && m_grid.IsEpt(i,1))//没输入里程，根据坡长和前一变坡点里程计算
		{
			_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
			if(!m_grid.IsEpt(i-1,4))
			{
				double BPDcml = xlc + _wtof(BPCstr);
				CString  GH;
				//	            double dxlc=pzLinep->XLC(BPDcml,GH);
				//	            _tcscpy(tmp,pzLinep->LCchr(GH,dxlc,3));
				bpddml=pzdmLine->XLC1(BPDcml);
			}
			else
			{
				ACHAR mes[80];
				_stprintf(mes, L"无法计算第%d个变坡点里程!",i);
				ads_alert(mes);
				continue;
			}
		}

		if(i>1 && m_grid.IsEpt(i,2))//没输入高程，根据前一变坡点坡度,坡长计算
		{
			_tcscpy(BPDstr, m_grid.GetItemText(i-1, 3));
			_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
			if(!m_grid.IsEpt(i-1,3) && !m_grid.IsEpt(i-1,4))
			{
				double BPDgc = BGC + _wtof(BPDstr)/100.0*_wtof(BPCstr); 
				ads_rtos(BPDgc,2,pzdmLine->DesHXSW,tmp1);
				//	_stprintf(tmp1, L"%0.3lf",);	
			}
			else
			{
				ACHAR mes[80];
				_stprintf(mes, L"无法计算第%d个变坡点高程!",i);
				ads_alert(mes);
				continue;
			}
		}
		else if(i>1)
		{
			_tcscpy(BPDstr, m_grid.GetItemText(i-1, 3));
			_tcscpy(BPCstr, m_grid.GetItemText(i-1, 4));
			if(m_grid.IsEpt(i-1,2))//高程为空
			{
				if(!m_grid.IsEpt(i-1,3) && !m_grid.IsEpt(i-1,4))
				{
					double BPDgc = BGC + _wtof(BPDstr)/100.0*_wtof(BPCstr); 
					ads_rtos(BPDgc,2,pzdmLine->DesHXSW,tmp1);
				}	
			}
		}
		xlc=pzdmLine->TYLC(bpddml);
		BGC = _wtof(tmp1);
		tabletoxy(i-1,xlc,_wtof(tmp1),_wtof(tmp2));
	}

	if(!m_grid.IsEpt(pzdmLine->NBPD,3) && !m_grid.IsEpt(pzdmLine->NBPD,4))//最后一个变坡点的坡度和坡长仍然有值,增加一个变坡点
	{
		_tcscpy(PDstr, m_grid.GetItemText(pzdmLine->NBPD, 3));//坡度
		_tcscpy(PCstr, m_grid.GetItemText(pzdmLine->NBPD, 4));//坡长
		if(_wtof(PCstr)>0.1)//有坡长
		{
			double cml = xlc + _wtof(PCstr);
			double BPDgc = BGC + _wtof(PDstr)/100.0*_wtof(PCstr);
			pzdmLine->NBPD++;
			tabletoxy(pzdmLine->NBPD-1,cml,BPDgc,0.0);//终点
		}
	}
	pzdmLine->close();	
	acDocManager->unlockDocument(acDocManager->curDocument());
}
