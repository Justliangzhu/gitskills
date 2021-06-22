// GouPDPage.cpp : implementation file
//

#include "stdafx.h"
#include "GouPDPage.h"
#include "hdmdes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
extern HdmDes HDM;
/////////////////////////////////////////////////////////////////////////////
// GouPDPage property page

IMPLEMENT_DYNCREATE(GouPDPage, CPropertyPage)

	GouPDPage::GouPDPage() : CPropertyPage(GouPDPage::IDD ,1)
{
	//{{AFX_DATA_INIT(GouPDPage)
	// NOTE: the ClassWizard will add member initialization here
	pm = NULL;
	NGouPD = 0;
	//}}AFX_DATA_INIT
}

GouPDPage::~GouPDPage()
{
}

void GouPDPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GouPDPage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GouPDPage, CPropertyPage)
	//{{AFX_MSG_MAP(GouPDPage)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTON_refresh, OnBUTTONrefresh)
	ON_BN_CLICKED(IDC_BUTTONDEL, OnButtondel)
	ON_BN_CLICKED(IDC_BUTTONpick, OnBUTTONpick)
	ON_BN_CLICKED(IDC_BUTTONsave, OnBUTTONsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GouPDPage message handlers

void GouPDPage::OnButtonadd() 
{
	// TODO: Add your control notification handler code here
	int i;
	CString tr;
	if(m_grid.GetRowCount()==1)          // �½������û����ʱ����1��
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
		int nRow = m_grid.GetFocusCell().row; // ��ǰ��

		if (nRow > 0)
		{ 
			Item.row = nRow;
			m_grid.InsertRow(_T("NEW"), nRow);

			// �ò�����֮����б���(0��)Ϊ���
			for(i=nRow; i<m_grid.GetRowCount(); i++)
			{
				CString str;
				Item.row=i;
				Item.col=0;
				str.Format(L"%d",i);
				Item.strText=str;
				m_grid.SetItem(&Item);
			}
			// ������Ϊ��
			for(i=1; i<m_grid.GetColumnCount(); i++) 
			{
				// �ڵ�ǰ��֮�����һ��������(ԭ��Grid�����ڵ�ǰ�в���������,������Ҫ�޸�һ��)
				m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
				m_grid.SetItemText(nRow+1,i,L" ");
			}
			m_grid.SetFocusCell(nRow+1,1);
			m_grid.Invalidate();
		}
	}  	

}

void GouPDPage::OnBUTTONrefresh() //ˢ�º����
{
	// TODO: Add your control notification handler code here
	//ȡ������,��θ��º����
	ACHAR tmp[50];
	UpdateData();
	int Flag;
	int iCols = m_grid.GetColumnCount();  // ���������

	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	int npd;
	npd =m_grid.GetRowCount()-1;
	if(npd>0)
	{
		GouPDdata goupd;
		for(int i=1; i<=npd; i++)
		{
			m_grid.RowIsEpt(i,iCols,Flag);
			if(Flag==0) 
				continue; // �����ǰ��ȫ����Ԫ��Ϊ��,�򲻱��� 


			_tcscpy(tmp,m_grid.GetItemText(i, 1));
			goupd.Sdml = _wtof(tmp);

			_tcscpy(tmp,m_grid.GetItemText(i, 2));
			goupd.SH = _wtof(tmp);

			_tcscpy(tmp,m_grid.GetItemText(i, 3));
			goupd.Edml = _wtof(tmp);

			_tcscpy(tmp,m_grid.GetItemText(i, 4));
			goupd.EH = _wtof(tmp);

			HDM.ModifySomeSG(roadname,goupd.Sdml,goupd.Edml,ZorY,goupd.SH,goupd.EH);		
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus(); // Reset the focus back to ourselves
}

void GouPDPage::OnButtondel() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	//	if  (nRow==m_grid.GetRowCount()-1) return;
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
		m_grid.Invalidate();
	}	
}

BOOL GouPDPage::OnInitDialog() 
{
	CRect rect;
	GetClientRect(rect);
	rect.left+=15;
	rect.top+=35;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	m_grid.SetRowCount(NGouPD+2);
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
		for(int i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}  
		//	   m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"���");
	m_grid.SetItemText(0,1,L"��ʼ׮��");
	m_grid.SetItemText(0,2,L"��ʼ���");
	m_grid.SetItemText(0,3,L"��ֹ׮��");
	m_grid.SetItemText(0,4,L"��ֹ���");//�����ټ���,�¶����뷽ʽ

	int i=0;
	ACHAR tmp[30];
	CString GH;
	if(NGouPD> 0 )  	
	{
		for(i=0; i<NGouPD; i++)
		{
			acdbRToS(GouPD[i].Sdml,2,3,tmp);
			m_grid.SetItemText(i+1,1,tmp);

			acdbRToS(GouPD[i].SH,2,3,tmp);
			m_grid.SetItemText(i+1,2,tmp);

			acdbRToS(GouPD[i].Edml,2,3,tmp);
			m_grid.SetItemText(i+1,3,tmp);

			acdbRToS(GouPD[i].EH,2,3,tmp);
			m_grid.SetItemText(i+1,4,tmp);

			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);			      
	}
	m_grid.SetColumnWidth(0,40);   
	m_grid.SetColumnWidth(1,80);
	m_grid.SetColumnWidth(2,80);
	m_grid.SetColumnWidth(3,80);
	m_grid.SetColumnWidth(4,80);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
	m_grid.Invalidate();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSEreturn TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void GouPDPage::OnOK() 
{
	// TODO: Add extra validation here
	CPropertyPage::OnOK();

	ACHAR tmp[30];
	CString tmp1;
	UpdateData();

	int Flag;
	int iCols = m_grid.GetColumnCount();  // ���������

	NGouPD =m_grid.GetRowCount()-1;

	if(NGouPD>0)
	{
		// ����ÿһ������
		GouPDdata goupd;
		GouPD.RemoveAll();
		for(int i=1; i<=NGouPD; i++)
		{
			m_grid.RowIsEpt(i,iCols,Flag);
			if(Flag==0) 
				continue; // �����ǰ��ȫ����Ԫ��Ϊ��,�򲻱��� 

			_tcscpy(tmp,m_grid.GetItemText(i, 1));
			goupd.Sdml = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 2));
			goupd.SH = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 3));
			goupd.Edml = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 4));
			goupd.EH = _wtof(tmp);//дͳһ���

			GouPD.Add(goupd);			
		}
		NGouPD = GouPD.GetSize();
	}

}

void GouPDPage::OnBUTTONpick() //ʰȡ
{
	// TODO: Add your control notification handler code here
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	AcDbEntity *pEnt;
	CString GH,str;

	GetParent()->ShowWindow(SW_HIDE);		// Hide our dialog
	acedGetAcadDwgView()->SetFocus();	

	int nRow = m_grid.GetFocusCell().row;
	int nCol = m_grid.GetFocusCell().col;
	if (nCol <= 2)//ʰȡ�����̺ͱ��
	{
		// TODO: Add your control notification handler code here
		if(ZorY==-1)
			rc = ads_entsel(L"\nʰȡ���������ˮ�� : ", en, asDblArray(PT));
		else
			rc = ads_entsel(L"\nʰȡ�Ҳ�������ˮ�� : ", en, asDblArray(PT));

		if (rc == RTNORM)
		{
			acdbGetObjectId(eId, en);
			/*acdbOpenObject(pEnt,eId,AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			pEnt->close();
			if(pEnt->isKindOf( LTGOU_ROAD::desc() ) )
			{
				LTGOU_ROAD *ltgou=NULL;
				ltgou=LTGOU_ROAD::cast(pEnt);
				if(ZorY!=ltgou->m_ZorY)
					ads_alert(L"�߲಻һ��,��ѡ��Բ�ˮ��!");
				double cml,dml,gouh;
				cml = ltgou->m_cml;
				int iroad = HDM.GetIRoadByName(roadname);

				dml = HDM.road[iroad].pm->XLC(cml,GH);
				gouh = ltgou->m_GouBotH;
				CString str;
				str.Format(L"%.3lf",dml);
				m_grid.SetItemText(nRow,1,str);
				str.Format(L"%.3lf",gouh);
				m_grid.SetItemText(nRow,2,str);
				m_grid.SetFocusCell(nRow,3);
			}
		}


	}
	else//ʰȡ�յ���̺ͱ��
	{
		// TODO: Add your control notification handler code here
		if(ZorY==-1)
			rc = ads_entsel(L"\nʰȡ����յ����ˮ�� : ", en, asDblArray(PT));
		else
			rc = ads_entsel(L"\nʰȡ�Ҳ��յ����ˮ�� : ", en, asDblArray(PT));

		if (rc == RTNORM)
		{

			acdbGetObjectId(eId, en);
			/*acdbOpenObject(pEnt,eId,AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"��ʵ��ʧ�ܣ�\n");
				return;
			}
			pEnt->close();
			if(pEnt->isKindOf( LTGOU_ROAD::desc() ) )
			{
				LTGOU_ROAD *ltgou=NULL;
				ltgou=LTGOU_ROAD::cast(pEnt);
				if(ZorY!=ltgou->m_ZorY)
					ads_alert(L"�߲಻һ��,��ѡ��Բ�ˮ��!");
				double cml,dml,gouh;
				cml = ltgou->m_cml;
				int iroad = HDM.GetIRoadByName(roadname);

				dml = HDM.road[iroad].pm->XLC(cml,GH);
				gouh = ltgou->m_GouBotH;
				str.Format(L"%.3lf",dml);
				m_grid.SetItemText(nRow,3,str);
				str.Format(L"%.3lf",gouh);
				m_grid.SetItemText(nRow,4,str);
				if(nRow==m_grid.GetRowCount()-1)
					OnButtonadd() ;
				m_grid.SetFocusCell(nRow+1,1);
			}
		}

	}

	GetParent()->ShowWindow(SW_SHOW); // Display our dialog again

	SetFocus(); // Reset the focus back to ourselves

	GetParent()->EnableWindow(TRUE); // Enable our dialog*/

}

void GouPDPage::OnBUTTONsave() //����
{
	// TODO: Add your control notification handler code here
	CPropertyPage::OnOK();

	ACHAR tmp[30];
	CString tmp1;
	UpdateData();
	NGouPD =m_grid.GetRowCount()-2;
	if(NGouPD>0)
	{
		GouPDdata goupd;
		GouPD.RemoveAll();
		for(int i=1; i<=NGouPD; i++)
		{
			_tcscpy(tmp,m_grid.GetItemText(i, 1));
			goupd.Sdml = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 2));
			goupd.SH = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 3));
			goupd.Edml = _wtof(tmp);//дͳһ���

			_tcscpy(tmp,m_grid.GetItemText(i, 4));
			goupd.EH = _wtof(tmp);//дͳһ���

			GouPD.Add(goupd);			
		}
	}

	///////////////////////////////////////////////////////////////////////
	HdmDataBase xlmdb;
	ACHAR mes[256];
	xlmdb.mdbFname = mdbFname;
	if(ZorY==-1)
	{
		xlmdb.NZGouPD = NGouPD;
		if(xlmdb.ZGouPD) delete[]xlmdb.ZGouPD;xlmdb.ZGouPD=NULL;
		if(xlmdb.NZGouPD>0)
		{
			xlmdb.ZGouPD = new  GouPDdata[xlmdb.NZGouPD];
			for(int j=0; j<xlmdb.NZGouPD; j++)
				xlmdb.ZGouPD[j] = GouPD[j];
			//д���ݿ�
			xlmdb.WriteGouPDB(mdbFname,roadname,L"�󹵵���"); 
			_stprintf(mes,L"�ѽ�%s-�󹵵������ݴ���%s��!",roadname,mdbFname);
			ads_alert(mes);
		}
	}
	else
	{
		xlmdb.NYGouPD = NGouPD;
		if(xlmdb.YGouPD) delete[]xlmdb.YGouPD;xlmdb.YGouPD=NULL;
		if(xlmdb.NYGouPD>0)
		{
			xlmdb.YGouPD = new  GouPDdata[xlmdb.NYGouPD];
			for(int j=0; j<xlmdb.NYGouPD; j++)
				xlmdb.YGouPD[j] = GouPD[j];
			//д���ݿ�
			xlmdb.WriteGouPDB(mdbFname,roadname,L"�ҹ�����"); 
			_stprintf(mes,L"�ѽ�%s-�ҹ��������ݴ���%s��!",roadname,mdbFname);
			ads_alert(mes);
		}
	}

}
