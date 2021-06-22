// PickXdmDmx.cpp : implementation file
//

#include "stdafx.h"
#include "PickXdmDmx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
/////////////////////////////////////////////////////////////////////////////
// PickXdmDmx dialog
PickXdmDmx::PickXdmDmx(CWnd* pParent,JD_CENTER *pmLine)
	: cdxCSizingDialog(PickXdmDmx::IDD, pParent)
{
	//{{AFX_DATA_INIT(PickXdmDmx)
	m_NDmd=0;
	m_pm = pmLine;
	m_IfUpdateAll=-1;
	m_GC=0.0;
	m_RoadName = pmLine->RoadName;
	m_RowSum = 1;
	//}}AFX_DATA_INIT
}


void PickXdmDmx::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PickXdmDmx)

	//    DDX_GridControl(pDX, IDC_LIST1,m_grid);
	DDX_Text(pDX, IDC_EDITRow, m_RowSum);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PickXdmDmx, cdxCSizingDialog)
	//{{AFX_MSG_MAP(PickXdmDmx)

	ON_BN_CLICKED(IDC_BUTTON_PickMl, OnBUTTONPickMl)
	ON_BN_CLICKED(IDSAVE, OnSave)
	ON_BN_CLICKED(IDC_BUTTONADD, OnButtonadd)
	ON_BN_CLICKED(IDC_BUTTONDEL, OnButtondel)
	ON_WM_PAINT()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PickXdmDmx message handlers

void PickXdmDmx::OnBUTTONPickMl() //ʰȡ
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//SetFocus(false);
	ads_point Pt;

	int   track=1,type;
	struct resbuf entdata;// ����϶�ʱ�Ķ�̬����

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	ads_printf(L"ע�⣺��������ƽ����λ�ϲɵ㣬�Ҽ�����!\n");
	while (track>0) 
	{  
		acedGetAcadDwgView()->SetFocus();
		if(ads_grread (1, &type, &entdata)!=RTNORM)
			break;
		Pt[0]=entdata.resval.rpoint[0];
		Pt[1]=entdata.resval.rpoint[1];

		if (type==3)  //��������������һ��
		{      			
			AcGePoint3d spt,ept;
			spt.set(Pt[X]-1.0,Pt[Y],0.0);
			ept.set(Pt[X]+1.0,Pt[Y],0.0);
			BAS_DRAW_FUN draw;
			draw.makeline(spt,ept,8);
			spt.set(Pt[X],Pt[Y]-1.0,0.0);
			ept.set(Pt[X],Pt[Y]+1.0,0.0);
			draw.makeline(spt,ept,8);

			double cml;
			cml = m_pm->PROJ_ML(Pt[Y],Pt[X]);
			/*
			else
			{
			AcGePoint2d ResPt,INpt;
			INpt.set(HdmPt[X],HdmPt[Y]);
			ResPt = m_zm->Data_drawtoreal(INpt);
			cml = ResPt.x; 
			ads_printf(L"%lf %lf cml=%lf\n",HdmPt[X],HdmPt[Y],cml);
			}
			*/
			if(cml<m_pm->DLArray[0].ELC || cml>m_pm->XLLength)
			{
				ads_printf(L"�޷��ҵ���Ӧ��·���,������ָ�������λ��!\n");
				return;
			}

			CString  GH;
			ACHAR mes[80];
			//					 double xlc=m_pm->XLC(cml,GH);
			//					 char mes[80];
			//					 _tcscpy(m_ckml,L"\0");   _tcscpy(m_ckml,m_pm->LCchr(GH,xlc,m_pm->NLC));
			m_dml = m_pm->XLC1(cml);
			ads_initget(1+2+4,NULL);
			_stprintf(mes,L"\n������<%0.3lf>��������: ",m_dml);
			int rt = ads_getreal(mes,&m_GC);
			if (rt==RTCAN)
				break;

			int nRow = m_grid.GetFocusCell().row;
			if(nRow>=m_grid.GetRowCount()||nRow<1)
				nRow = m_grid.GetRowCount()-1;
			ads_printf(L"nrow=%d\n",nRow);

			if (nRow == m_grid.GetRowCount() - 1)
				OnButtonadd();

			m_IfUpdateAll = nRow;
			SetFocus();
			Set_Value();
			//					 Invalidate();   

			m_grid.SetFocusCell(nRow+1,1);
			//			 acedGetAcadDwgView()->SetFocus();
			track=1;

			acutPrintf(L"\n��ȡ��һ����Ҽ�����: ");//plhadd
		}
		else if((type!=5) && (type!=12))
			break;

		//	 if(type==3)  //  ��������Ҽ�����
		//   ads_initget(1+32,NULL);
		//   ads_getpoint(NULL,L"\n����ƽ��ͼ�ϲɼ����....",Pt); 
		//   ads_initget(RSG_OTHER, "C c E e");

	}
	ads_printf(L"�����߲ɼ�����!\n");
	ads_printf(L"Command: ");
	acDocManager->unlockDocument(acDocManager->curDocument());    
	m_grid.ResetScrollBars();
}

void PickXdmDmx::OnSave() 
{
	if(Sort()==1)//����
	{		
		Get_Value();
		// TODO: Add your control notification handler code here
		CFileDialog fname(false,NULL,PrjName);
		fname.m_ofn.lpstrDefExt = L"mdb";
		fname.m_ofn.lpstrInitialDir=workdir;
		fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
		fname.m_ofn.lpstrFilter=L"��·�������ݿ�(*.mdb)\0*.mdb\0\0";

		if(fname.DoModal()!=IDOK)
			return;
		PrjName =fname.GetPathName(); //��·����mdb	   
		//		ads_alert(PrjName);
		//////////////////////////////////////////////////////////////////////////
		AfxOleInit();
		try                 
		{	
			// �򿪱���Access��
#ifdef X64
			CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + PrjName;
#else
			CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + PrjName;
#endif // X64
			
			//strConnect+="���ݿ�.mdb";
			m_adoConnection.Disconnect();
			if (!m_adoConnection.Connect(strConnect))
			{
				AfxMessageBox(L"ʧ�� 1!");
				AfxMessageBox(L"�������ݿ�ʧ��!");			
				return ;
			}
			//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb",L"",L"",adModeUnknown);		
		}
		catch(_com_error e)
		{		
			AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");		
			return;
		} 	

		CString rcd; 
		CString GH;
		CString str;

		m_strSQL.Format(L"delete from �ݵ����߱� where ��·��='%s'",m_RoadName);//ɾ��ȫ��

		m_adoConnection.Execute(m_strSQL);

		m_strSQL.Format(L"select * from �ݵ����߱� where ��·��='%s'",m_pm->RoadName);
		/*
		if(m_irxd==0)//һ��
		m_strSQL ="select * from ���ڱ�";
		else  if(m_irxd<0)//����
		m_strSQL ="select * from �������ж����ڱ�";
		else  if(m_irxd>0)//����
		m_strSQL ="select * from �������ж����ڱ�";*/


		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			return ;
		}

		Report_input.SetAdoConnection(&m_adoConnection);
		Report_input.Open(m_strSQL, adCmdText);
		//	Report_input.MoveFirst();
		for(int i=0; i<m_NDmd; i++)
		{
			Report_input.AddNew();
			str.Format(L"%d",i);//ID
			_variant_t strText=str;
			Report_input.PutCollect(0,strText);
			double xlc=m_pm->XLC1(DmxArray[i].x);

			//			strText = GH;
			//			Report_input.PutCollect(1,strText);
			Report_input.PutCollect(1,xlc);//��׮���	
			Report_input.PutCollect(2,DmxArray[i].y);//��׮���	

			strText = m_RoadName;
			Report_input.PutCollect(3,strText);
			Report_input.MoveNext();
		}
		Report_input.Update();
		Report_input.Close();
		m_adoConnection.Disconnect();
		ads_alert(L"OK");
	}
	m_grid.ResetScrollBars();
	//	acedGetAcadDwgView()->SetFocus();
}

void PickXdmDmx::Input() //����
{
	// TODO: Add your control notification handler code here
	/*
	CFileDialog fname(true,NULL,PrjName);
	fname.m_ofn.lpstrDefExt = "mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT;
	fname.m_ofn.lpstrFilter="��·�������ݿ�(*.mdb)\0*.mdb\0\0";

	if(fname.DoModal()!=IDOK)
	return;
	PrjName =fname.GetPathName(); //��·����mdb	   	*/
	m_strSQL ="�ݵ����߱�";

	if(AttachMdb(m_strSQL)==0)
		return;
	CString GhK,rcd; 
	double dml;

	m_NDmd = Report_input.GetRecordCount();
	for(int i=0; i<DmxArray.length(); i++)//���
		DmxArray.removeAt(i);
	DmxArray.setLogicalLength(0);
	AcGePoint2d pt;
	CString str;
	Report_input.MoveFirst();
	while(!Report_input.IsEOF())
	{
		//	      Report_input.GetValueString(GhK,L"��׮��̹ں�");				
		Report_input.GetValueString(rcd,L"��׮�����");//��׮���	
		dml=_wtof(rcd);
		//		  dmltockml(GhK,dml,ckml);
		pt.x=m_pm->TYLC(dml);  
		Report_input.GetValueString(str,L"����߳�");//����߳�	
		pt.y = _wtof(str);
		DmxArray.append(pt);
		Report_input.MoveNext();
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	m_NDmd = DmxArray.length();
	m_IfUpdateAll=-1;//ȫ������
	//  Invalidate();
}

void PickXdmDmx::OnButtonadd() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		UpdateData();
		for(int k=0; k<m_RowSum; k++)
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
			m_grid.ResetScrollBars();
			m_grid.Invalidate();

			m_NDmd++;
		}
	}
	m_grid.ResetScrollBars();
}

void PickXdmDmx::OnButtondel() 
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
		m_grid.ResetScrollBars();
		m_grid.Invalidate();
		m_NDmd--;
	}
	m_grid.ResetScrollBars();
}

int PickXdmDmx::Sort() //����
{
	// TODO: Add your control notification handler code here
	double *dmx;
	ACHAR tmp[80];
	int count;

	count = 0;
	m_NDmd= m_grid.GetRowCount() - 2;  
	dmx = new double[m_NDmd*2];
	int flag;
	int i;
	for( i=1; i<=m_NDmd; i++)
	{
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);

		if(flag==0)//��
			continue;
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		dmx[count*2]=m_pm->TYLC(_wtof(tmp));
		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		dmx[count*2+1] = _wtof(tmp);
		count++;
	}	
	m_NDmd = count;
	queue(dmx , m_NDmd, 2,0 );
	for(i=0; i<DmxArray.length(); i++)//���
		DmxArray.removeAt(i);
	DmxArray.setLogicalLength(0);
	AcGePoint2d pt;
	for(i=0; i<m_NDmd; i++)
	{
		pt.x = dmx[i*2];
		pt.y = dmx[i*2+1];
		DmxArray.append(pt);
	}
	if(dmx) delete []dmx;
	m_IfUpdateAll=-1;
	m_grid.SetRowCount(2);
	//	OnPaint(); 
	SetFocus();
	Set_Value();
	//Invalidate();
	return 1;
}

void PickXdmDmx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	SetFocus();
	m_grid.ResetScrollBars();
	// TODO: Add your message handler code here
	//	Set_Value();

	// Do not call cdxCSizingDialog::OnPaint() for painting messages
}

BOOL PickXdmDmx::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);

	PrjName = m_pm->mdbname;

	/*
	CString curpm;
	char str[180];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	workdir=Cworkdir;
	curpm=workdir+"\\project";
	if((fpr = _wfopen(curpm,L"r"))==NULL)   
	curpm="";
	else
	{
	fwscanf(fpr,L"%s",str);
	curpm=str;
	fclose(fpr);
	}
	curpm = workdir + "\\data\\" + curpm + ".mdb";
	PrjName = curpm;
	*/
	//����
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=60;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	if(PrjName!="")
		Input();
	else
		m_NDmd=0;
	//daicol=1;
	m_grid.SetRowCount(m_NDmd+2);
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
		for(int i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}  
		//	   m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"���");
	m_grid.SetItemText(0,1,L"�� ��");
	m_grid.SetItemText(0,2,L"�� ��");
	int i=0;
	ACHAR tmp[30];
	if(m_NDmd !=0 )  	
	{
		for(i=0; i<m_NDmd; i++)
		{
			ACHAR Sml[80];
			CString  GH;
			ACHAR ckml[80];
			//			   double xlc=m_pm->XLC(DmxArray[i].x,GH);
			double xlc=m_pm->XLC1(DmxArray[i].x);
			//			   _tcscpy(ckml,L"\0");   _tcscpy(ckml,m_pm->LCchr(GH,xlc,2));

			ads_rtos(xlc,2,3,ckml);
			m_grid.SetItemText(i+1,1,ckml);

			ads_rtos(DmxArray[i].y,2,3,Sml);
			//			   _stprintf(Sml,L"%0.3lf",DmxArray[i].y);
			m_grid.SetItemText(i+1,2,Sml); 

			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);
		}
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);

	m_grid.SetColumnWidth(0,40);
	m_grid.SetColumnWidth(1,105);
	m_grid.SetColumnWidth(2,75);
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	SetFocus();
	m_grid.SetFocusCell(m_grid.GetRowCount()-1,1);
	AddSzControl(m_grid,mdResize,mdResize);
	m_grid.ResetScrollBars();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
int PickXdmDmx::AttachMdb(CString TabName)//�����ݿ�����
{
	ACHAR mes[300];
	AfxOleInit();
	try                 
	{	
		// �򿪱���Access��
#ifdef X64
		_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", PrjName);
#else
		_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", PrjName);
#endif // X64
		
		CString strConnect = mes;
		//strConnect+="���ݿ�.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"ʧ��  2!");
			AfxMessageBox(L"�������ݿ�ʧ��!");			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb",L"",L"",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ��Pm.mdb�Ƿ��ڵ�ǰ·����!");		
	} 	


	m_strSQL.Format(L"select * from %s where ��·��='%s' order by ���",TabName,m_pm->RoadName);	

	if (!m_adoConnection.IsOpen())
	{
		ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
		return 0;
	}
	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
		ads_printf(L"û��%s\n",TabName);			
		Report_input.Close();
		return 0;
	}
	return 1;
}

void PickXdmDmx::Set_Value()
{
	UpdateData();
	if(m_IfUpdateAll<0)//ȫ������
	{
		int i=0;
		ACHAR tmp[10];
		if(m_NDmd !=0 )  	
		{
			m_grid.SetRowCount(m_NDmd+2);
			for(i=0; i<m_NDmd; i++)
			{
				ACHAR Sml[80];
				CString  GH;
				ACHAR ckml[80];
				double xlc=m_pm->XLC1(DmxArray[i].x);
				//                _tcscpy(ckml,L"\0");   _tcscpy(ckml,m_pm->LCchr(GH,xlc,2));
				ads_rtos(xlc,2,3,ckml);

				m_grid.SetItemText(i+1,1,ckml);

				//   	       _stprintf(Sml,L"%0.3lf",DmxArray[i].y);
				ads_rtos(DmxArray[i].y,2,3,Sml);
				m_grid.SetItemText(i+1,2,Sml); 

				_stprintf(tmp,L"%d",i+1);
				m_grid.SetItemText(i+1,0,tmp);
			}
		}
		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);
	}
	else//���µ�m_IfUpdateAll�м�¼
	{
		_stprintf(m_ckml,L"%0.2lf",m_dml);
		m_grid.SetItemText(m_IfUpdateAll,1,m_ckml);
		ACHAR tmp[80];
		_stprintf(tmp,L"%0.3lf",m_GC);
		m_grid.SetItemText(m_IfUpdateAll,2,tmp);
	}
	m_grid.Invalidate();
}

void PickXdmDmx::Get_Value()
{
	ACHAR tmp[80];
	int i;
	for( i=0; i<DmxArray.length(); i++)//���
		DmxArray.removeAt(i);
	DmxArray.setLogicalLength(0);
	AcGePoint2d pt;

	int NRow;
	NRow=0;
	for(i=1; i<m_grid.GetRowCount(); i++)
	{
		int flag;
		m_grid.RowIsEpt(i,m_grid.GetColumnCount(),flag);

		if(flag)//����
			NRow++;
		else
			break;
	}//ͳ�Ʒǿ�����
	m_NDmd = NRow;  

	for(i=1; i<=m_NDmd; i++)
	{
		_tcscpy(tmp, m_grid.GetItemText(i, 1));
		pt.x=m_pm->TYLC(_wtof(tmp));
		_tcscpy(tmp, m_grid.GetItemText(i, 2));
		pt.y = _wtof(tmp);
		DmxArray.append(pt);	
	}	
	//	ads_printf(L"%lf %lf\n",DmxArray[0].x,DmxArray[0].y);
}

void PickXdmDmx::queue(double a[] , int nrow, int ncol,int inf )
{
	int i,j,k;
	double t;

	for(i=0; i<nrow; i++)
		for(j=i+1; j<nrow; j++)
		{
			if(a[i*ncol+inf] > a[j*ncol+inf])
			{
				for(k=0; k<ncol; k++)
				{
					t = a[i*ncol + k];
					a[i*ncol + k] = a[j*ncol+k];
					a[j*ncol + k] = t;
				}
			}
		}
}

BOOL PickXdmDmx::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN)
	{	
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}
