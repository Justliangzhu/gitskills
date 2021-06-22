// hooseLMX.cpp : implementation file
//

#include "stdafx.h"
#include "hooseLMX.h"
#include "xldatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ChooseLMX dialog


ChooseLMX::ChooseLMX(CWnd* pParent /*=NULL*/)
	: CDialog(ChooseLMX::IDD, pParent)
{
	//{{AFX_DATA_INIT(ChooseLMX)
	m_L_CXDX = true;
	m_L_FGDX = true;
	m_L_LYDX = true;
	m_L_TLJX = true;
	m_L_YLJX = true;
	m_Y_TLJX = true;
	m_Y_CXDX = true;
	m_Y_LYDX = true;
	m_Y_YLJX = true;
	m_Y_FGDX = true;
	m_sdml = 0.0;
	m_edml = 0.0;
	m_pm = NULL;
	//}}AFX_DATA_INIT
}


void ChooseLMX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChooseLMX)
	DDX_Check(pDX, IDC_CHECK_L_CXDX, m_L_CXDX);
	DDX_Check(pDX, IDC_CHECK_L_FGDX, m_L_FGDX);
	DDX_Check(pDX, IDC_CHECK_L_LYDX, m_L_LYDX);
	DDX_Check(pDX, IDC_CHECK_L_TLJX, m_L_TLJX);
	DDX_Check(pDX, IDC_CHECK_L_YLJX, m_L_YLJX);
	DDX_Check(pDX, IDC_CHECK_Y_TLJX, m_Y_TLJX);
	DDX_Check(pDX, IDC_CHECK_YCXDX, m_Y_CXDX);
	DDX_Check(pDX, IDC_CHECK_YLYDX, m_Y_LYDX);
	DDX_Check(pDX, IDC_CHECK_YYLJX, m_Y_YLJX);
	DDX_Check(pDX, IDC_CHECKY_FGDX, m_Y_FGDX);
	DDX_Text(pDX, IDC_EDIT_SDML, m_sdml);
	DDX_Text(pDX, IDC_EDIT_EDML, m_edml);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChooseLMX, CDialog)
	//{{AFX_MSG_MAP(ChooseLMX)
	ON_BN_CLICKED(IDC_BUTTON_SELXL, OnButtonSelxl)
	ON_BN_CLICKED(IDC_BUTTON_PickSdml, OnBUTTONPickSdml)
	ON_BN_CLICKED(IDC_BUTTON_PickEdml, OnBUTTONPickEdml)
	ON_BN_CLICKED(IDC_BUTTONdraw, OnBUTTONdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChooseLMX message handlers

void ChooseLMX::OnButtonSelxl() 
{
	// TODO: Add your control notification handler code here

	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml,tmp;
	CString mdbname="",roadname="",HxBh="";
	XLDataBase DBS;

	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	rc = ads_entsel(L"\n选择平面线路实体 : ", en, asDblArray(PT));
	if (rc == RTNORM) 
	{
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
			m_pm = JD_CENTER::cast(pObj);
			//		acdbOpenObject(m_pm, eId, AcDb::kForRead);
			m_sdml = m_pm->DLArray[0].ELC;
			m_edml = m_pm->DLArray[m_pm->DLNum-1].BLC;
			UpdateData(FALSE);
		}
		else
		{
			DBS.GetXLXdata(eId,mdbname,roadname);
			if(mdbname!=""&&roadname!="")
			{
				//				DBS.Read_XLDbs(mdbname,"控制点表",roadname);
				DBS.Read_XLDbs(mdbname,L"线元表",roadname);						
				DBS.Read_XLDbs(mdbname,L"断链表",roadname);
				if(DBS.XYNum>0)
				{
					m_pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
					//					m_pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					if(DBS.NDL>0)
						m_pm->setDLB(DBS.DLArray,DBS.NDL);
					_tcscpy(m_pm->mdbname,mdbname);
					_tcscpy(m_pm->RoadName,roadname);   
					m_sdml = m_pm->DLArray[0].ELC;
					m_edml = m_pm->DLArray[m_pm->DLNum-1].BLC;
					UpdateData(FALSE);				
				}
			}
		}
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/
}

void ChooseLMX::OnBUTTONPickSdml() 
{
	// TODO: Add your control notification handler code here
	if(m_pm==NULL)
	{
		ads_alert(L"请先选择平面线路实体!");
		return ;
	}

	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	AcGePoint3d Pt;
	acedGetPoint(NULL,L"\n请拾取起点里程:\n",asDblArray(Pt));

	m_sdml = m_pm->XLC1(m_pm->PROJ_ML(Pt.y,Pt.x));		

	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 

}

void ChooseLMX::OnBUTTONPickEdml() 
{
	// TODO: Add your control notification handler code here
	if(m_pm==NULL)
	{
		ads_alert(L"请先选择平面线路实体!");
		return ;
	}

	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	AcGePoint3d Pt;
	acedGetPoint(NULL,L"\n请拾取终点里程:\n",asDblArray(Pt));

	m_edml = m_pm->XLC1(m_pm->PROJ_ML(Pt.y,Pt.x));		

	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 

}

void ChooseLMX::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void ChooseLMX::OnBUTTONdraw() //绘图
{


	double sml=0,eml,sfgdk,efgdk,slydk,elydk,scxdk,ecxdk,syljk,eyljk,stljk,etljk;


	UpdateData();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD 
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	

	//读取左路面宽数据
	XLDataBase xlmdb;
	CString mdbname,roadname;
	double Scml,Ecml;
	if(m_pm==NULL)
	{
		ads_alert(L"请先选择平面线路实体!");
		return;
	}

	mdbname = m_pm->mdbname;
	roadname = m_pm->RoadName;
	Scml = m_pm->TYLC(m_sdml);
	Ecml = m_pm->TYLC(m_edml);

	int sinf=0;
	xlmdb.Read_XLDbs(mdbname,L"左路面宽分段表",roadname);
	if(xlmdb.NLMK>0)//左侧路面宽
	{

		sfgdk = -1*xlmdb.lmk[0].fgdk;
		slydk = sfgdk + -1*xlmdb.lmk[0].lydk;
		scxdk = slydk + -1*xlmdb.lmk[0].cxdk;
		syljk = scxdk + -1*xlmdb.lmk[0].yljk;
		stljk = syljk + -1*xlmdb.lmk[0].tljk;

		for(int i=0; i<xlmdb.NLMK; i++)
		{
			eml = m_pm->TYLC(xlmdb.lmk[i].xlc);
			efgdk = -1*xlmdb.lmk[i].fgdk;
			elydk = efgdk + -1*xlmdb.lmk[i].lydk;
			ecxdk = elydk + -1*xlmdb.lmk[i].cxdk;
			eyljk = ecxdk + -1*xlmdb.lmk[i].yljk;
			etljk = eyljk + -1*xlmdb.lmk[i].tljk;
			if(Scml>eml+0.01)
				continue;
			if(sinf==0)
			{
				if(fabs(eml-sml)>0.001)
				{
					sfgdk = sfgdk + (efgdk-sfgdk)*(Scml-sml)/(eml-sml);
					slydk = slydk + (elydk-slydk)*(Scml-sml)/(eml-sml);
					scxdk = scxdk + (ecxdk-scxdk)*(Scml-sml)/(eml-sml);
					syljk = syljk + (eyljk-syljk)*(Scml-sml)/(eml-sml);
					stljk = stljk + (etljk-stljk)*(Scml-sml)/(eml-sml);
				}
				sml = Scml;
				sinf=1;
			}

			if(sml>Ecml-0.001)
				break;
			if(eml>Ecml-0.001)
			{
				if(fabs(eml-sml)>0.001)
				{
					efgdk = sfgdk + (efgdk-sfgdk)*(Ecml-sml)/(eml-sml);
					elydk = slydk + (elydk-slydk)*(Ecml-sml)/(eml-sml);
					ecxdk = scxdk + (ecxdk-scxdk)*(Ecml-sml)/(eml-sml);
					eyljk = syljk + (eyljk-syljk)*(Ecml-sml)/(eml-sml);
					etljk = stljk + (etljk-stljk)*(Ecml-sml)/(eml-sml);
				}
				eml = Ecml;								
			}

			if(m_L_FGDX)
				m_pm->DrawKD(sml,sfgdk,eml,efgdk,xlmdb.lmk[i].bkmode,7);
			if(m_L_LYDX)
				m_pm->DrawKD(sml,slydk,eml,elydk,xlmdb.lmk[i].bkmode,6);
			if(m_L_CXDX)
				m_pm->DrawKD(sml,scxdk,eml,ecxdk,xlmdb.lmk[i].bkmode,6);
			if(m_L_YLJX)
				m_pm->DrawKD(sml,syljk,eml,eyljk,xlmdb.lmk[i].bkmode,7);
			if(m_L_TLJX)
				m_pm->DrawKD(sml,stljk,eml,etljk,xlmdb.lmk[i].bkmode,7);
			sml = eml;
			sfgdk = efgdk;
			slydk = elydk;
			scxdk = ecxdk;
			syljk = eyljk;				   
			stljk = etljk;				   
		}
		delete [] xlmdb.lmk;
		xlmdb.lmk=NULL;
	}

	sinf=0;

	xlmdb.Read_XLDbs(mdbname,L"右路面宽分段表",roadname);
	if(xlmdb.NLMK>0)//左侧路面宽
	{
		sml = m_pm->DLArray[0].ELC;
		sfgdk = xlmdb.lmk[0].fgdk;
		slydk = sfgdk + xlmdb.lmk[0].lydk;
		scxdk = slydk + xlmdb.lmk[0].cxdk;
		syljk = scxdk + xlmdb.lmk[0].yljk;
		stljk = syljk + xlmdb.lmk[0].tljk;

		for(int i=0; i<xlmdb.NLMK; i++)
		{
			eml = m_pm->TYLC(xlmdb.lmk[i].xlc);
			efgdk = xlmdb.lmk[i].fgdk;
			elydk = efgdk + xlmdb.lmk[i].lydk;
			ecxdk = elydk + xlmdb.lmk[i].cxdk;
			eyljk = ecxdk + xlmdb.lmk[i].yljk;
			etljk = eyljk + xlmdb.lmk[i].tljk;
			if(Scml>eml+0.01)
				continue;
			if(sinf==0)
			{
				if(fabs(eml-sml)>0.001)
				{
					sfgdk = sfgdk + (efgdk-sfgdk)*(Scml-sml)/(eml-sml);
					slydk = slydk + (elydk-slydk)*(Scml-sml)/(eml-sml);
					scxdk = scxdk + (ecxdk-scxdk)*(Scml-sml)/(eml-sml);
					syljk = syljk + (eyljk-syljk)*(Scml-sml)/(eml-sml);
					stljk = stljk + (etljk-stljk)*(Scml-sml)/(eml-sml);
				}
				sml = Scml;
				sinf=1;
			}

			if(sml>Ecml-0.001)
				break;
			if(eml>Ecml-0.001)
			{
				if(fabs(eml-sml)>0.001)
				{
					efgdk = sfgdk + (efgdk-sfgdk)*(Ecml-sml)/(eml-sml);
					elydk = slydk + (elydk-slydk)*(Ecml-sml)/(eml-sml);
					ecxdk = scxdk + (ecxdk-scxdk)*(Ecml-sml)/(eml-sml);
					eyljk = syljk + (eyljk-syljk)*(Ecml-sml)/(eml-sml);
					etljk = stljk + (etljk-stljk)*(Ecml-sml)/(eml-sml);
				}
				eml = Ecml;								
			}


			if(m_Y_FGDX)
				m_pm->DrawKD(sml,sfgdk,eml,efgdk,xlmdb.lmk[i].bkmode,7);
			if(m_Y_LYDX)
				m_pm->DrawKD(sml,slydk,eml,elydk,xlmdb.lmk[i].bkmode,6);
			if(m_Y_CXDX)
				m_pm->DrawKD(sml,scxdk,eml,ecxdk,xlmdb.lmk[i].bkmode,6);
			if(m_Y_YLJX)
				m_pm->DrawKD(sml,syljk,eml,eyljk,xlmdb.lmk[i].bkmode,7);
			if(m_Y_TLJX)
				m_pm->DrawKD(sml,stljk,eml,etljk,xlmdb.lmk[i].bkmode,7);
			sml = eml;
			sfgdk = efgdk;
			slydk = elydk;
			scxdk = ecxdk;
			syljk = eyljk;				   
			stljk = etljk;				   
		}
		delete [] xlmdb.lmk;
		xlmdb.lmk=NULL;
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW);		// Hide our dialog
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 
}
