// DBBZ.cpp : implementation file
//

#include "stdafx.h"
#include "DBBZ.h"
#include "DrawPM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DBBZ dialog


DBBZ::DBBZ(CWnd* pParent /*=NULL*/)
	: CDialog(DBBZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(DBBZ)
	m_qhdis = _T("0.4");
	m_zydis = _T("0.4");
	m_zhxsw = _T("三位");
	m_gcxsw = _T("两位");
	m_kdxsw = _T("三位");
	m_hpxsw = _T("两位");
	m_bzdir = true;
	m_pm=NULL;
	//}}AFX_DATA_INIT
}


void DBBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DBBZ)
	DDX_Text(pDX, IDC_EDIT_DBQHBJ, m_qhdis);
	DDX_Text(pDX, IDC_EDIT_ZYBJ, m_zydis);
	DDX_CBString(pDX, IDC_COMBO_DBZHXSW, m_zhxsw);
	DDX_CBString(pDX, IDC_COMBO_DBGCXSW, m_gcxsw);
	DDX_CBString(pDX, IDC_COMBO_DBKDXSW, m_kdxsw);
	DDX_CBString(pDX, IDC_COMBO_DBHPXSW, m_hpxsw);
	DDX_Check(pDX, IDC_CHECK_BZDIR, m_bzdir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DBBZ, CDialog)
	//{{AFX_MSG_MAP(DBBZ)
	ON_BN_CLICKED(IDC_RADIO_DBZH, OnRadioDbzh)
	ON_BN_CLICKED(IDC_RADIO_DBSJG, OnRadioDbsjg)
	ON_BN_CLICKED(IDC_RADIO_DBLJG, OnRadioDbljg)
	ON_BN_CLICKED(IDC_RADIO_DBWID, OnRadioDbwid)
	ON_BN_CLICKED(IDC_RADIO_DBHP, OnRadioDbhp)
	ON_BN_CLICKED(IDC_RADIO_DBLJHP, OnRadioDbljhp)
	ON_BN_CLICKED(IDC_RADIO_DBPXHX, OnRadioDbpxhx)
	ON_BN_CLICKED(IDC_RADIO_DBCZHX, OnRadioDbczhx)
	ON_BN_CLICKED(IDC_RADIO_DBXX1, OnRadioDbxx1)
	ON_BN_CLICKED(IDC_RADIO_DBXX2, OnRadioDbxx2)
	ON_BN_CLICKED(IDC_RADIO_DBXX3, OnRadioDbxx3)
	ON_BN_CLICKED(IDC_RADIO_DBXX4, OnRadioDbxx4)
	ON_BN_CLICKED(IDC_BUTTON_DBPICKHX, OnButtonDbpickhx)
	ON_BN_CLICKED(IDC_BUTTON_DBPICKZX, OnButtonDbpickzx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DBBZ message handlers

BOOL DBBZ::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_DBZH);
	pBtn->SetCheck(1);		
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_DBPXHX);
	pBtn->SetCheck(1);	
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_DBXX1);
	pBtn->SetCheck(1);
	bzinf = 0;
	dirinf = 0;
	xxinf = 1;
	NHxId = 0;
	NZxId = 0;
	m_pm = NULL;
	mdbname = "";
	roadname = "";
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DBBZ::OnRadioDbzh() 
{
	// TODO: Add your control notification handler code here
	bzinf = 0;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(false);		
}

void DBBZ::OnRadioDbsjg() 
{
	// TODO: Add your control notification handler code here
	bzinf = 1;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(true);	
}

void DBBZ::OnRadioDbljg() 
{
	// TODO: Add your control notification handler code here
	bzinf = 2;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(true);
}

void DBBZ::OnRadioDbwid() 
{
	// TODO: Add your control notification handler code here
	bzinf = 3;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(false);	
}

void DBBZ::OnRadioDbhp() 
{
	// TODO: Add your control notification handler code here
	bzinf = 4;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(false);	
}

void DBBZ::OnRadioDbljhp() 
{
	// TODO: Add your control notification handler code here
	bzinf = 5;
	GetDlgItem(IDC_RADIO_DBXX1)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX2)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX3)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBXX4)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZYBJ)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_DBCZHX)->EnableWindow(false);	
}

void DBBZ::OnRadioDbpxhx() 
{
	// TODO: Add your control notification handler code here
	dirinf = 0;	
}

void DBBZ::OnRadioDbczhx() 
{
	// TODO: Add your control notification handler code here
	dirinf = 1;	
}

void DBBZ::OnRadioDbxx1() 
{
	// TODO: Add your control notification handler code here
	xxinf =1;
}

void DBBZ::OnRadioDbxx2() 
{
	// TODO: Add your control notification handler code here
	xxinf =2;
}

void DBBZ::OnRadioDbxx3() 
{
	// TODO: Add your control notification handler code here
	xxinf =3;
}

void DBBZ::OnRadioDbxx4() 
{
	// TODO: Add your control notification handler code here
	xxinf =4;
}

void DBBZ::OnButtonDbpickhx() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc,xsw;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int i,type,track=1;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	AcDbObject *pobj;
	double cml=-100,tmp;
	AcDbPolyline *pent,*prent=NULL;
	ACHAR choice[5];
	CString mname,rname,hxbh;
	sml = 9999999999;
	eml = -999999999;

	NHxId = 0;	
	HxIdArr.removeSubArray(0,HxIdArr.length()-1);
	ads_initget(RSG_OTHER,L"S s G g");
	rc = ads_getkword(L"\n选一组横线 (G) /选指定横线  (S): <G>  ", choice);
	if(rc==RTCAN)
		return;
	if(rc==RTNONE)
		_tcscpy(choice ,L"G");
	if(_tcscmp(choice ,L"S")==0 || _tcscmp(choice ,L"s")==0)//多选模式
	{
		while (track>0) 
		{  
			rc = ads_entsel(L"\n添加横线 : ", en, asDblArray(PT));
			if(rc==RTCAN)
			{
				NHxId = 0;
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/			   
				return;
			}
			if (rc == RTNORM) 	   
			{   
				AcDbObjectId eId;
				acdbGetObjectId(eId, en);
				/*acdbOpenObject(pobj,eId,AcDb::kForRead);*/
				if(acdbOpenObject(pobj, eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pobj->close();
				if(pobj->isKindOf(AcDbPolyline::desc()))
				{
					pent = AcDbPolyline::cast(pobj);  
					m_pm->GetDBXdata(eId,mdbname,roadname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);
					if(cml>-0.1)//是横线
					{
						sml = sml < cml ? sml:cml;
						eml = eml > cml ? eml:cml;
						pent->highlight();
						NHxId = HxIdArr.length();
						for(i=0;i<NHxId;i++)
							if(HxIdArr[i]==eId)
								break;
						if(i==NHxId)
							HxIdArr.append(eId);
					}
				}			  
			}	
			else
			{
				if(prent)
					prent->unhighlight();
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/	
				return;
			}

			if(ads_grread (track, &type, &entdata)!=RTNORM)
				break;
			if((type!=5) && (type!=12))
			{
				if(prent)
					prent->unhighlight();
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/	
				return;
			}
		}
	}
	else//组选
	{
		rc = ads_entsel(L"\n选择一条横线 : ", en, asDblArray(PT));
		if(rc==RTCAN)
		{
			NHxId = 0;
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/			   
			return;
		}
		if (rc == RTNORM) 	   
		{   	
			AcDbObjectId eId;
			acdbGetObjectId(eId, en);
			/*acdbOpenObject(pobj,eId,AcDb::kForRead);*/
			if(acdbOpenObject(pobj, eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pobj->close();
			if(pobj->isKindOf(AcDbPolyline::desc()))
			{
				pent = AcDbPolyline::cast(pobj); 
				m_pm->GetDBXdata(eId,mdbname,roadname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);

				if(cml>-0.1)//是横线
				{
					AcArray<CString> layerArray;
					layerArray.append("端部横线");
					m_pm->GetDBHXEntity(HxIdArr,layerArray,mdbname,roadname,hxbh);
					NHxId = HxIdArr.length();
					for(i=0;i<NHxId;i++)
					{
						/*acdbOpenObject(pobj,HxIdArr[i],AcDb::kForRead);*/
						if(acdbOpenObject(pobj, HxIdArr[i], AcDb::kForRead)!=Acad::eOk)
						{
							ads_printf(L"打开实体失败！\n");
							return;
						}
						pobj->close();
						m_pm->GetDBXdata(HxIdArr[i],mname,rname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);
						sml = sml < cml ? sml:cml;
						eml = eml > cml ? eml:cml;
						pent = AcDbPolyline::cast(pobj); 
						pent->highlight();
					}			     
				}
				else
					ads_printf(L"选择横线错误!\n");
			}
		}			  
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/		
}

void DBBZ::OnButtonDbpickzx() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int i,type,track=1;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	AcDbObject *pobj;
	AcDbEntity *pzxent;
	AcDbPolyline *pent,*prent=NULL;

	NZxId = 0;
	ZxIdArr.removeSubArray(0,ZxIdArr.length()-1);
	if(bzinf==2||bzinf==5)//标注土路肩高和土路肩横坡
	{
		rc = ads_entsel(L"\n请用鼠标选取硬路肩边线 : ", en, asDblArray(PT));
		if(rc==RTCAN)
		{
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/			   
			return;
		}
		if (rc == RTNORM) 	   
		{   
			AcDbObjectId eId;
			acdbGetObjectId(eId, en);
			/*acdbOpenObject(pobj,eId,AcDb::kForRead);*/
			if(acdbOpenObject(pobj, eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pobj->close();
			if(pobj->isKindOf(AcDbPolyline::desc()))
			{
				pent = AcDbPolyline::cast(pobj);  
				pent->highlight();
				ZxIdArr.append(eId);
			}
		}

		rc = ads_entsel(L"\n请用鼠标选取土路肩边线 : ", en, asDblArray(PT));
		if(rc==RTCAN)
		{
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/			   
			return;
		}
		if (rc == RTNORM) 	   
		{   
			AcDbObjectId eId;
			acdbGetObjectId(eId, en);
			/*  acdbOpenObject(pobj,eId,AcDb::kForRead);*/
			if(acdbOpenObject(pobj, eId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pobj->close();
			if(pobj->isKindOf(AcDbPolyline::desc()))
			{
				pent = AcDbPolyline::cast(pobj);  
				pent->highlight();
				ZxIdArr.append(eId);
			}
		}			  
	}
	else
	{
		while (track>0) 
		{  
			rc = ads_entsel(L"\n依此选取要标注的纵线 : ", en, asDblArray(PT));
			if(rc==RTCAN)
			{
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/			   
				return;
			}
			if (rc == RTNORM) 	   
			{   
				AcDbObjectId eId;
				acdbGetObjectId(eId, en);
				/*acdbOpenAcDbEntity(pzxent,eId,AcDb::kForRead);*/
				if(acdbOpenObject(pzxent, eId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pzxent->close();
				pzxent->highlight();
				NZxId = ZxIdArr.length();
				for(i=0;i<NZxId;i++)
					if(ZxIdArr[i]==eId)
						break;
				if(i==NZxId)
					ZxIdArr.append(eId);
			}
			else
			{
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/	
				return;
			}

			if(ads_grread (track, &type, &entdata)!=RTNORM)
				break;
			if((type!=5) && (type!=12))
			{
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog*/	
				return;
			}
		}
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/		
}

void DBBZ::OnOK() 
{
	// TODO: Add extra validation here
	int i,j,xsw;
	CString mname,rname;
	double cml,value,ang,qhdis,zydis,texth=0.8,disx,disy,prjml,hp,width,ht1,ht2,hp1;
	AcDbObject *pobj;
	AcDbEntity *pzxent;
	AcDbPolyline *pent;
	ACHAR bztext[180],str[50],BzLayer[100];
	AcGePoint3d spt,ept,PrePt,Jpt,ptbb,ptee;
	AcGePoint3dArray Pts; 
	ads_point ptb,pte,pt,tpt,arwpt,sspt,eept;
	AcDbObjectId eId;
	CString hxbh;

	UpdateData();
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	
	qhdis = _wtof(m_qhdis);
	zydis = _wtof(m_zydis);
	if(mdbname!=""&&roadname!="")
	{
		//		DBS.Read_XLDbs(mdbname,L"控制点表",roadname);
		DBS.Read_XLDbs(mdbname,L"线元表",roadname);
		DBS.Read_XLDbs(mdbname,L"断链表",roadname);
		if(DBS.XYNum>0)
		{
			m_pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			//			m_pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				m_pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(m_pm->mdbname,mdbname);
			_tcscpy(m_pm->RoadName,roadname);     
		}
		if(m_pm)
		{
			CDrawPM drawpm;
			if(bzinf==1||bzinf==2||bzinf==4||bzinf==5)
			{
				drawpm.pm = m_pm;
				drawpm.Scml = sml;
				drawpm.Ecml = eml;
				drawpm.ReadLMK("左路面宽分段表");
				drawpm.ReadLMK("右路面宽分段表");
				drawpm.DrawBGDATA();
			}
			double tmp;
			for(i=0;i<HxIdArr.length();i++)
			{		
				/*acdbOpenObject(pobj,HxIdArr[i],AcDb::kForRead);	*/
				if(acdbOpenObject(pobj, HxIdArr[i], AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pobj->close();
				pent = AcDbPolyline::cast(pobj); 	
				m_pm->GetDBXdata(HxIdArr[i],mname,rname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);

				if(mname==mdbname&&rname==roadname)
				{
					if(bzinf==0)
					{
						_tcscpy(BzLayer,L"桩号");
						value = m_pm->XLC1(cml);
						double ys=fmod(value,1000.0);
						if(fabs(ys-1000.0)<0.001||fabs(ys)<0.001)//公里桩
							_stprintf(bztext,L"+%0.0lf",value);
						else
						{
							if(m_zhxsw=="一位")
								xsw = 1;
							else if(m_zhxsw=="两位")
								xsw = 2; 
							else
								xsw = 3;
							ads_rtos(ys,2,xsw,str);
							if(ys<10)
								_stprintf(bztext,L"+00%s",str);
							else if(ys<100)
								_stprintf(bztext,L"+0%s",str);
							else
								_stprintf(bztext,L"+%s",str);
						}
					}

					PrePt.x=-100;
					PrePt.y=-100;
					pent->getPointAt(0,spt);
					pent->getPointAt(1,ept);
					ptb[X]=spt.x;ptb[Y]=spt.y;
					pte[X]=ept.x;pte[Y]=ept.y;
					pent->unhighlight();
					for(j=0;j<ZxIdArr.length();j++)
					{
						/*acdbOpenObject(pzxent,ZxIdArr[j],AcDb::kForRead);*/
						if(acdbOpenObject(pzxent, ZxIdArr[j], AcDb::kForRead)!=Acad::eOk)
						{
							ads_printf(L"打开实体失败！\n");
							return;
						}
						pzxent->close();
						pzxent->unhighlight();
						pzxent->intersectWith(pent,AcDb::kOnBothOperands,Pts);
						if(Pts.length()>0)
						{						   
							ang = ads_angle(ptb,pte);
							Jpt = Pts[0];
							if(bzinf==1)//设计高
							{
								_tcscpy(BzLayer,L"设计高");
								drawpm.CalYLMHt(Pts[0].y,Pts[0].x,prjml,value,hp);
								if(m_gcxsw=="一位")
									xsw = 1;
								else if(m_gcxsw=="两位")
									xsw = 2;
								else
									xsw = 3;
								ads_rtos(value,2,xsw,bztext);							  
							}
							else if(bzinf==2)//设计高
							{
								_tcscpy(BzLayer,L"路肩高");
								if(PrePt.x >-0.1)
								{
									drawpm.CalTLMHt(PrePt.y,PrePt.x,Pts[0].y,Pts[0].x,prjml,value,hp);
									if(m_gcxsw=="一位")
										xsw = 1;					
									else if(m_gcxsw=="两位")
										xsw = 2;			
									else
										xsw = 3;
									ads_rtos(value,2,xsw,bztext);							    
								}
							}
							else if(bzinf==4)//横坡度
							{
								_tcscpy(BzLayer,L"路面横坡");
								if(PrePt.x >-0.1)
								{
									drawpm.CalYLMHt(PrePt.y,PrePt.x,prjml,ht1,hp);
									drawpm.CalYLMHt(Pts[0].y,Pts[0].x,prjml,ht2,hp);
									value = sqrt((PrePt.x-Pts[0].x)*(PrePt.x-Pts[0].x)+(PrePt.y-Pts[0].y)*(PrePt.y-Pts[0].y));					
									hp1 = (ht2-ht1)*100/value;
									//		                        ads_printf(L"ht2=%lf ht1=%lf hp=%lf value=%lf\n",ht2,ht1,hp1,value);
									if(m_hpxsw=="一位")
										xsw = 1;
									else if(m_hpxsw=="两位")
										xsw = 2;
									else
										xsw = 3;
									ads_rtos(fabs(hp1),2,xsw,str);
									_stprintf(bztext,L"%s%s",str,L"%");
								}
							}
							else if(bzinf==5)//横坡度
							{
								_tcscpy(BzLayer,L"路肩横坡");
								if(PrePt.x >-0.1)
								{
									drawpm.CalYLMHt(PrePt.y,PrePt.x,prjml,ht1,hp1);
									drawpm.CalTLMHt(PrePt.y,PrePt.x,Pts[0].y,Pts[0].x,prjml,ht2,hp);
									value = sqrt((PrePt.x-Pts[0].x)*(PrePt.x-Pts[0].x)+(PrePt.y-Pts[0].y)*(PrePt.y-Pts[0].y));
									hp1 = (ht2-ht1)*100/value;
									if(m_hpxsw=="一位")
										xsw = 1;
									else if(m_hpxsw=="两位")
										xsw = 2;		
									else
										xsw = 3;
									ads_rtos(fabs(hp1),2,xsw,str);	
									_stprintf(bztext,L"%s%s",str,L"%");
								}
							}
							else if(bzinf==3&&PrePt.x>-0.1)//宽度
							{
								_tcscpy(BzLayer,L"宽度");
								value = sqrt((PrePt.x-Pts[0].x)*(PrePt.x-Pts[0].x)+(PrePt.y-Pts[0].y)*(PrePt.y-Pts[0].y));
								if(m_kdxsw=="一位")
									xsw = 1;
								else if(m_kdxsw=="两位")
									xsw = 2;
								else
									xsw = 3;
								ads_rtos(value,2,xsw,bztext);
							}

							if(bzinf==1||bzinf==2)//设计高或路肩高
							{
								pt[X] = Jpt.x;
								pt[Y] = Jpt.y;
								if(xxinf==1)//第一象限
								{
									//平行横线
									disx = zydis+0.5*_tcslen(bztext)*texth*0.6;
									disy = qhdis+0.5*texth;
									ads_polar(pt,ang,disx,tpt);
									ads_polar(tpt,ang+0.5*PI,disy,pt);
									if(dirinf==1)//垂直横线
									{
										pt[X] = Jpt.x;
										pt[Y] = Jpt.y;
										disx = zydis+0.5*texth;
										disy = qhdis+0.5*_tcslen(bztext)*texth*0.6;
										ang+=0.5*PI;	
										ads_polar(pt,ang-0.5*PI,disx,tpt);
										ads_polar(tpt,ang,disy,pt);								  
									}
								}
								else if(xxinf==2)//第2象限
								{
									//平行横线							   
									disx = zydis+0.5*_tcslen(bztext)*texth*0.6;
									disy = -qhdis-0.5*texth;
									ads_polar(pt,ang,disx,tpt);
									ads_polar(tpt,ang+0.5*PI,disy,pt);
									if(dirinf==1)//垂直横线
									{
										pt[X] = Jpt.x;
										pt[Y] = Jpt.y;
										disx = zydis+0.5*texth;
										disy = -qhdis-0.5*_tcslen(bztext)*texth*0.6;                          
										ang+=0.5*PI;
										ads_polar(pt,ang-0.5*PI,disx,tpt);
										ads_polar(tpt,ang,disy,pt);									  
									}
								}
								else if(xxinf==3)
								{
									disx = -zydis-0.5*_tcslen(bztext)*texth*0.6;
									disy = -qhdis-0.5*texth;
									ads_polar(pt,ang,disx,tpt);
									ads_polar(tpt,ang+0.5*PI,disy,pt);
									if(dirinf==1)//垂直横线
									{
										pt[X] = Jpt.x;
										pt[Y] = Jpt.y;
										disx = -zydis-0.5*texth;
										disy = -qhdis-0.5*_tcslen(bztext)*texth*0.6;                          
										ang+=0.5*PI;
										ads_polar(pt,ang-0.5*PI,disx,tpt);
										ads_polar(tpt,ang,disy,pt);										                    		  
									}
								}
								else//第4象限
								{
									disx = -zydis-0.5*_tcslen(bztext)*texth*0.6;
									disy = qhdis+0.5*texth;
									ads_polar(pt,ang,disx,tpt);
									ads_polar(tpt,ang+0.5*PI,disy,pt);
									if(dirinf==1)//垂直横线
									{
										pt[X] = Jpt.x;
										pt[Y] = Jpt.y;
										disx = -zydis-0.5*texth;
										disy = qhdis+0.5*_tcslen(bztext)*texth*0.6;                          
										ang+=0.5*PI;
										ads_polar(pt,ang-0.5*PI,disx,tpt);
										ads_polar(tpt,ang,disy,pt);					                    		  
									}
								}
							}
							else
							{
								pt[X] = PrePt.x;
								pt[Y] = PrePt.y;
								width = 0.5*sqrt((PrePt.x-Pts[0].x)*(PrePt.x-Pts[0].x)+(PrePt.y-Pts[0].y)*(PrePt.y-Pts[0].y));
								if(qhdis>=0)
									disy = qhdis+0.5*texth;
								else
									disy = qhdis-0.5*texth;
								ads_polar(pt,ang,width,tpt);
								ads_polar(tpt,ang+0.5*PI,disy,pt);
								ads_polar(tpt,ang+0.5*PI,0.5*qhdis,arwpt);
							}
							spt.x = pt[X];
							spt.y = pt[Y];
							if(bzinf!=1&&bzinf!=2)//
							{
								if(PrePt.x>-0.1)
								{	
									if(m_bzdir)								   
										eId = m_pm->maketext(spt,bztext,ang,texth,0,3,L"HZ",BzLayer);
									else
										eId = m_pm->maketext(spt,bztext,ang+PI,texth,0,3,L"HZ",BzLayer);
									m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,Pts[0].x,Pts[0].y,PrePt.x,PrePt.y,xsw);
									if(bzinf==4||bzinf==5)
									{
										int LorR;
										LorR = m_pm->JudgePTLorR(Pts[0].y,Pts[0].x,disx,m_pm->XYArray,m_pm->XYNum);
										if(LorR==0)
											LorR = m_pm->JudgePTLorR(PrePt.y,PrePt.x,disx,m_pm->XYArray,m_pm->XYNum);
										if(LorR<0 && bzinf==4)//左侧 路面横坡
											hp1 = - hp1;
										if((LorR<0&&hp1<0)||(LorR>0&&hp1>0))
										{
											ads_polar(arwpt,ang,1.5,sspt);
											ads_polar(arwpt,ang+PI,1.5,eept);
											ptbb.x = sspt[X],ptbb.y = sspt[Y];
											ptee.x = eept[X],ptee.y = eept[Y];
											eId = m_pm->makeline(ptbb,ptee,0,0,BzLayer);
											m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,arwpt[X],arwpt[Y],-9999,-9999,xsw);
											ads_polar(arwpt,ang+PI,1.0,eept);
											ads_polar(eept,ang+PI*0.5,0.1,sspt);
											ptbb.x = sspt[X],ptbb.y = sspt[Y];
											ads_polar(eept,ang-PI*0.5,0.1,sspt);
											spt.x = sspt[X],spt.y = sspt[Y];
											eId = m_pm->makesolid(ptee,ptbb,spt,0,BzLayer);
											m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,arwpt[X],arwpt[Y],-9999,-9999,xsw);
										}
										else
										{
											ads_polar(arwpt,ang,1.5,eept);
											ads_polar(arwpt,ang+PI,1.5,sspt);
											ptbb.x = sspt[X],ptbb.y = sspt[Y];
											ptee.x = eept[X],ptee.y = eept[Y];
											eId = m_pm->makeline(ptbb,ptee,0,0,BzLayer);
											m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,arwpt[X],arwpt[Y],-9999,-9999,xsw);
											ads_polar(arwpt,ang,1.0,eept);
											ads_polar(eept,ang+PI*0.5,0.1,sspt);
											ptbb.x = sspt[X],ptbb.y = sspt[Y];
											ads_polar(eept,ang-PI*0.5,0.1,sspt);
											spt.x = sspt[X],spt.y = sspt[Y];
											eId = m_pm->makesolid(ptee,ptbb,spt,0,BzLayer);
											m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,arwpt[X],arwpt[Y],-9999,-9999,xsw);
										}
									}
								}
							}
							else//高程
							{
								if(bzinf==1)
								{
									if(m_bzdir)
										eId = m_pm->maketext(spt,bztext,ang,texth,0,3,L"HZ",BzLayer);
									else
										eId = m_pm->maketext(spt,bztext,ang+PI,texth,0,3,L"HZ",BzLayer);
									m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,Pts[0].x,Pts[0].y,-9999,-9999,xsw);
								}
								else
								{
									if(PrePt.x>-0.1)
									{
										if(m_bzdir)
											eId = m_pm->maketext(spt,bztext,ang,texth,0,3,L"HZ",BzLayer);
										else
											eId = m_pm->maketext(spt,bztext,ang+PI,texth,0,3,L"HZ",BzLayer);
										m_pm->SetDBXdata(eId,mdbname,roadname,cml,hxbh,Pts[0].x,Pts[0].y,PrePt.x,PrePt.y,xsw);
									}
								}
							}
							PrePt.x = Pts[0].x;
							PrePt.y = Pts[0].y;
							Pts.removeSubArray(0,Pts.length()-1);
						}
					}
				}
			}
		}
	}
	else
		ads_alert(L"请先正确选择横线!");

	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/	
	//	CDialog::OnOK();
}
