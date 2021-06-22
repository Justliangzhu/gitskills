// XLLinkDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "XLLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define LRL_MODEL 0
#define TU_MODEL 1
#define ZY_MODEL 2
#define S_MODEL 3
#define C_MODEL 4
#define LUAN_MODEL 5

#include "ZYLink.h"
/////////////////////////////////////////////////////////////////////////////
// XLLinkDlg dialog


XLLinkDlg::XLLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(XLLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(XLLinkDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	XLObj1=NULL;
	XLObj2=NULL;
	m_AorL1 = 1;
	m_AorL2 = 1;
}


void XLLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XLLinkDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_RADIO_NOL, m_ButtonNoL);
	DDX_Control(pDX, IDC_RADIO_HASL, m_ButtonHasL);
	DDX_Control(pDX, IDC_EDIT_A2, m_EditA2);
	DDX_Control(pDX, IDC_EDIT_A1, m_EditA1);
	DDX_Control(pDX, IDC_RADIO_A, m_ButtonA);
	DDX_Control(pDX, IDC_RADIO_L, m_ButtonL);
	DDX_Control(pDX, IDC_RADIO_LRL, m_ButtonLRLModel);
	DDX_Control(pDX, IDC_RADIO_TU, m_ButtonTUModel);
	DDX_Control(pDX, IDC_RADIO_ZY, m_ButtonZYModel);
	DDX_Control(pDX, IDC_RADIO_C, m_ButtonCModel);
	DDX_Control(pDX, IDC_RADIO_S, m_ButtonSModel);
	DDX_Control(pDX, IDC_RADIO_LUAN, m_ButtonLuanModel);
	DDX_Radio(pDX, IDC_RADIO_LRL, m_LinkModel);
	DDX_Text(pDX, IDC_EDIT_A1, m_AorL1);
	DDV_MinMaxFloat(pDX, m_AorL1, 0.f, 1.e+009f);
	DDX_Text(pDX, IDC_EDIT_A2, m_AorL2);
	DDV_MinMaxFloat(pDX, m_AorL2, 0.f, 1.e+009f);
	DDX_Radio(pDX, IDC_RADIO_A, m_AorL);
	DDX_Radio(pDX, IDC_RADIO_NOL, m_NotL);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XLLinkDlg, CDialog)
	//{{AFX_MSG_MAP(XLLinkDlg)
	ON_BN_CLICKED(IDC_BUTTON_SEL1, OnButtonSel1)
	ON_BN_CLICKED(IDC_BUTTON_SEL2, OnButtonSel2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XLLinkDlg message handlers

void XLLinkDlg::OnButtonSel1() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadFrame()->SetFocus();
	EnableWindow(FALSE);
	ads_name en;
	AcDbEntity * pEnt=NULL;

	if(RTNORM==acedEntSel(L"\n��ѡ����·:",en,asDblArray(PT1)))
	{
		acdbGetObjectId(XLId1,en);	
		/*acdbOpenObject(pEnt,XLId1,AcDb::kForRead);*/
		if(acdbOpenObject(pEnt,XLId1, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		pEnt->close();
		if(pEnt->isKindOf( JD_CENTER::desc() ) )
		{
			XLObj1=JD_CENTER::cast(pEnt);
		}
	}
	EnableWindow(TRUE);
	AutoModel();
}

void XLLinkDlg::OnButtonSel2() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();	
	EnableWindow(FALSE);

	ads_name en;
	AcDbEntity * pEnt=NULL;

	if(RTNORM==acedEntSel(L"\n��ѡ����·:",en,asDblArray(PT2)))
	{
		acdbGetObjectId(XLId2,en);	
		/*acdbOpenObject(pEnt,XLId2,AcDb::kForRead);*/
		if(acdbOpenObject(pEnt,XLId2, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		pEnt->close();
		if(pEnt->isKindOf( JD_CENTER::desc() ) )
		{
			XLObj2=JD_CENTER::cast(pEnt);
		}
	}

	EnableWindow(TRUE);
	AutoModel();
}

void XLLinkDlg::AutoModel()//�������ӷ�ʽ......
{
	UpdateData(TRUE);
	if(XLObj1!=NULL && XLObj2!=NULL)
	{
		//������Ӵ���ͶӰ���
		tylc1x=XLObj1->PROJ_ML(PT1.y,PT1.x);
		tylc2x=XLObj2->PROJ_ML(PT2.y,PT2.x);
		//������Ӵ�����Ԫ���
		XYNo1=XLObj1->GetXYNoFromTYLC(tylc1x);
		XYNo2=XLObj2->GetXYNoFromTYLC(tylc2x);
		//�����Ԫ����
		XYClass1=(int)(XLObj1->XYArray[XYNo1][0]+0.001);
		XYClass2=(int)(XLObj2->XYArray[XYNo2][0]+0.001);
		//������Ӵ��Ľ������
		JDNo1=XLObj1->XYJDNO[XYNo1];
		JDNo2=XLObj2->XYJDNO[XYNo2];

		BAS_DRAW_FUN::JdCenter *JdCen1,*JdCen2;

		//��ȡ��ӵ���������/Բ�ĵ�ָ��
		JdCen1=&XLObj1->JdCenArray[JDNo1];
		JdCen2=&XLObj2->JdCenArray[JDNo2];

		if(XYClass1==XianYuan::LINE && XYClass2==XianYuan::LINE)
		{
			//�Ȳ��ǻ�Բ����Ҳ����͹�����ӣ�ȱʡ��Ϊ��Բ������
			if(m_LinkModel!=LRL_MODEL && m_LinkModel!=TU_MODEL)
				m_LinkModel=LRL_MODEL;

			//L+L
			m_ButtonLRLModel.EnableWindow(TRUE);//��+Բ+��;
			m_ButtonTUModel.EnableWindow(TRUE);//͹��;

			//ZY
			m_ButtonZYModel.EnableWindow(FALSE);//ֱԲ��;

			//YY
			m_ButtonCModel.EnableWindow(FALSE);//C��
			m_ButtonSModel.EnableWindow(FALSE);//S��;
			m_ButtonLuanModel.EnableWindow(FALSE);//����;

			//������������
			m_ButtonA.EnableWindow(TRUE);//��A;
			m_ButtonL.EnableWindow(TRUE);//��L;			
			m_EditA2.EnableWindow(TRUE);//��1
			m_EditA1.EnableWindow(TRUE);//��2

			//���޼�ֱ��
			m_ButtonNoL.EnableWindow(FALSE);//�м�ֱ��
			m_ButtonHasL.EnableWindow(FALSE);//�޼�ֱ��


		}
		else if( (XYClass1==XianYuan::LINE && XYClass2==XianYuan::ARC) 
			||(XYClass1==XianYuan::ARC && XYClass2==XianYuan::LINE) )
		{
			m_LinkModel=ZY_MODEL;
			//L+L
			m_ButtonLRLModel.EnableWindow(FALSE);//��+Բ+��;
			m_ButtonTUModel.EnableWindow(FALSE);//͹��;

			//ZY
			m_ButtonZYModel.EnableWindow(TRUE);//ֱԲ��;

			//YY
			m_ButtonCModel.EnableWindow(FALSE);//C��
			m_ButtonSModel.EnableWindow(FALSE);//S��;
			m_ButtonLuanModel.EnableWindow(FALSE);//����;

			//������������
			m_ButtonA.EnableWindow(FALSE);//��A;
			m_ButtonL.EnableWindow(FALSE);//��L;			
			m_EditA2.EnableWindow(FALSE);//��1
			m_EditA1.EnableWindow(FALSE);//��2

			//���޼�ֱ��
			m_ButtonNoL.EnableWindow(FALSE);//�м�ֱ��
			m_ButtonHasL.EnableWindow(FALSE);//�޼�ֱ��
		}
		else if( (XYClass1==XianYuan::ARC && XYClass2==XianYuan::ARC) )
		{
			//�������ת
			LeftOrRight1=BAS_DRAW_FUN::SIGN(XLObj1->XYArray[XYNo1][4]);
			LeftOrRight2=BAS_DRAW_FUN::SIGN(XLObj2->XYArray[XYNo2][4]);
			//���Բ�ļ��뾶
			if(JdCen1->JDXZ==IsCenter)
			{
				R1=JdCen1->R;
				CenN1=JdCen1->N;
				CenE1=JdCen1->E;
			}
			else
			{
				R1=JdCen1->R;
				//��Բ�ģ���Բ������㣬ת90�ȷų�һ���뾶
				CenN1=XLObj1->XYArray[XYNo1][8]+R1*cos(XLObj1->XYArray[XYNo1][5]+XLObj1->XYArray[XYNo1][4]*0.5*PI);
				CenE1=XLObj1->XYArray[XYNo1][9]+R1*sin(XLObj1->XYArray[XYNo1][5]+XLObj1->XYArray[XYNo1][4]*0.5*PI);
			}

			if(JdCen2->JDXZ==IsCenter)
			{
				R2=JdCen2->R;
				CenN2=JdCen2->N;
				CenE2=JdCen2->E;
			}
			else
			{
				R2=JdCen2->R;
				//��Բ�ģ���Բ������㣬ת90�ȷų�һ���뾶
				CenN2=XLObj2->XYArray[XYNo2][8]+R2*cos(XLObj2->XYArray[XYNo2][5]+XLObj2->XYArray[XYNo2][4]*0.5*PI);
				CenE2=XLObj2->XYArray[XYNo2][9]+R2*sin(XLObj2->XYArray[XYNo2][5]+XLObj2->XYArray[XYNo2][4]*0.5*PI);
			}
			double f;
			DistO1O2=BAS_DRAW_FUN::xyddaa(CenN1,CenE1,CenN2,CenE2,&f);//���Բ�ľ�

			if(DistO1O2<=fabs(R1-R2))m_LinkModel=LUAN_MODEL;
			else
			{
				if(DistO1O2<fabs(R1+R2))m_LinkModel=C_MODEL;
				else
				{
					if(m_LinkModel!=C_MODEL && m_LinkModel!=S_MODEL)
						m_LinkModel=S_MODEL;
				}
			}
			//L+L
			m_ButtonLRLModel.EnableWindow(FALSE);//��+Բ+��;
			m_ButtonTUModel.EnableWindow(FALSE);//͹��;

			//ZY
			m_ButtonZYModel.EnableWindow(FALSE);//ֱԲ��;

			//YY
			if(m_LinkModel==LUAN_MODEL)//����
			{
				m_ButtonCModel.EnableWindow(FALSE);//C��
				m_ButtonSModel.EnableWindow(FALSE);//S��;
				m_ButtonLuanModel.EnableWindow(TRUE);//����;

				//������������
				m_ButtonA.EnableWindow(FALSE);//��A;
				m_ButtonL.EnableWindow(FALSE);//��L;			
				m_EditA2.EnableWindow(FALSE);//��1
				m_EditA1.EnableWindow(FALSE);//��2

				//���޼�ֱ��
				m_ButtonNoL.EnableWindow(FALSE);//�м�ֱ��
				m_ButtonHasL.EnableWindow(FALSE);//�޼�ֱ��
			}
			else
			{
				if(DistO1O2<fabs(R1+R2))m_ButtonSModel.EnableWindow(FALSE);//S��;
				else m_ButtonSModel.EnableWindow(TRUE);//C��;
				m_ButtonCModel.EnableWindow(TRUE);//C��
				m_ButtonLuanModel.EnableWindow(FALSE);//����;

				//������������
				m_ButtonA.EnableWindow(TRUE);//��A;
				m_ButtonL.EnableWindow(TRUE);//��L;			
				m_EditA2.EnableWindow(TRUE);//��1
				m_EditA1.EnableWindow(TRUE);//��2				

				//���޼�ֱ��
				m_ButtonNoL.EnableWindow(TRUE);//�м�ֱ��
				m_ButtonHasL.EnableWindow(TRUE);//�޼�ֱ��
			}
		}
		UpdateData(FALSE);
	}
}

void XLLinkDlg::OnOK() 
{
	// TODO: Add extra validation here	
	//CDialog::OnOK();
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if(XLObj1 == XLObj2)
	{
		ads_printf(L"��ѡΪͬһ��ʵ��!\n");
		return;
	}
	UpdateData(TRUE);
	if(XLObj1!=NULL && XLObj2!=NULL)
	{
		//////////////////////////////////////////////////////////////////////////			
		/*acdbOpenObject(XLObj1, XLId1, AcDb::kForWrite);*/
		AcDbObject* pObj;
		if(acdbOpenObject(pObj,XLId1,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		XLObj1=JD_CENTER::cast(pObj);
		/*acdbOpenObject(XLObj2, XLId2, AcDb::kForWrite);*/
		if(acdbOpenObject(pObj,XLId2,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		XLObj2=JD_CENTER::cast(pObj);
		//////////////////////////////////////////////////////////////////////////
		XLObj1->assertWriteEnabled();
		XLObj2->assertWriteEnabled();		
		/*
		//�����һ����·�����յ�����
		double QN1,QE1,ZN1,ZE1;
		QN1=XLObj1->XYArray[0][1];
		QE1=XLObj1->XYArray[0][2];
		ZN1=XLObj1->XYArray[XLObj1->XYNum][8];
		ZE1=XLObj1->XYArray[XLObj1->XYNum][9];
		//����ڶ�����·�����յ�����
		double QN2,QE2,ZN2,ZE2;
		QN2=XLObj2->XYArray[0][1];
		QE2=XLObj2->XYArray[0][2];
		ZN2=XLObj2->XYArray[XLObj2->XYNum][8];
		ZE2=XLObj2->XYArray[XLObj2->XYNum][9];
		//���յ㵽һ���յ�ľ���
		double DistQ1Q2,DistZ1Z2,DistQ2Z1,DistQ1Z2,f;
		DistQ1Q2=BAS_DRAW_FUN::xyddaa(QN1,QE1,QN2,QE2,&f);
		DistZ1Z2=BAS_DRAW_FUN::xyddaa(ZN1,ZE1,ZN2,ZE2,&f);
		DistQ1Z2=BAS_DRAW_FUN::xyddaa(QN1,QE1,ZN2,ZE2,&f);
		DistQ2Z1=BAS_DRAW_FUN::xyddaa(QN2,QE2,ZN1,ZE1,&f);
		double MinDist=min(DistQ1Q2,min(DistZ1Z2,min(DistQ1Z2,DistQ2Z1)));
		if(fabs(DistQ1Q2-MinDist)<0.001)XLObj1->Reverse();
		if(fabs(DistZ1Z2-MinDist)<0.001)XLObj2->Reverse();
		if(fabs(DistQ1Z2-MinDist)<0.001)
		{
		XLObj1->Reverse();
		XLObj2->Reverse();
		}

		//������Ӵ���ͶӰ���
		tylc1x=XLObj1->PROJ_ML(PT1.y,PT1.x);
		tylc2x=XLObj2->PROJ_ML(PT2.y,PT2.x);
		//������Ӵ�����Ԫ���
		XYNo1=XLObj1->GetXYNoFromTYLC(tylc1x);
		XYNo2=XLObj2->GetXYNoFromTYLC(tylc2x);
		//�����Ԫ����
		XYClass1=(int)(XLObj1->XYArray[XYNo1][0]+0.001);
		XYClass2=(int)(XLObj2->XYArray[XYNo2][0]+0.001);
		//������Ӵ��Ľ������
		JDNo1=XLObj1->XYJDNO[XYNo1];
		JDNo2=XLObj2->XYJDNO[XYNo2];*/


		if(XYClass1==XianYuan::LINE && XYClass2==XianYuan::LINE)
		{
			BAS_DRAW_FUN::JdCenter *JdCen1,*JdCen2,*JdCen3,*JdCen4;
			XLObj1->TranJD();
			XLObj2->TranJD();
			JDNo1--;
			//��ȡ��ӵ���������/Բ�ĵ�ָ��
			JdCen1=&XLObj1->JdCenArray[JDNo1];		
			JdCen2=&XLObj1->JdCenArray[JDNo1+1];
			JdCen3=&XLObj2->JdCenArray[JDNo2-1];
			JdCen4=&XLObj2->JdCenArray[JDNo2];

			AcGePoint3d Pt1(JdCen1->JDE,JdCen1->JDN,0.0);
			AcGePoint3d Pt2(JdCen2->JDE,JdCen2->JDN,0.0);
			AcGePoint3d Pt3(JdCen3->JDE,JdCen3->JDN,0.0);
			AcGePoint3d Pt4(JdCen4->JDE,JdCen4->JDN,0.0);

			double T2=JdCen1->T2;//���г�
			double fwj1=XLObj1->fwj(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
			XLObj1->getLineEndPt(Pt1,T2,fwj1,Pt1);

			double T1=JdCen3->T1;//ǰ�г�////plh04.26
			double fwj2=XLObj2->fwj(Pt4.x,Pt4.y,Pt3.x,Pt3.y);
			XLObj2->getLineEndPt(Pt4,T1,fwj2,Pt4);//plh06.01

			AcGePoint3d interPt;

			if( BAS_DRAW_FUN::INTERS(&interPt.x,&interPt.y,Pt1.x,Pt1.y,Pt2.x,Pt2.y,Pt3.x,Pt3.y,Pt4.x,Pt4.y)>=0 )
			{
				double rad=-1.0;
				if(JDNo2<XLObj2->JdCenNum-1)rad=XLObj1->GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,Pt4.x,Pt4.y,false,0,0,12);
				else rad=XLObj1->GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,Pt4.x,Pt4.y,false,0,0,1);
				if(rad>0)
				{
					//XLObj1->JdCenNum=(JDNo1+1)+(XLObj2->JdCenNum-JDNo2)+1;
					int i=JDNo1+1;

					BAS_DRAW_FUN::JdCenter newJdCen;

					newJdCen.JDXZ=IsJiaoDian;
					newJdCen.IsOrNotHuiTou=false;
					newJdCen.JdNo=i;
					newJdCen.N=interPt.y;
					newJdCen.E=interPt.x;
					newJdCen.JDN=interPt.y;
					newJdCen.JDE=interPt.x;
					if(m_AorL==1)//ΪL
					{
						//ת��ΪA
						m_AorL1=sqrt(rad*m_AorL1);
						m_AorL2=sqrt(rad*m_AorL2);
					}
					newJdCen.A1=m_AorL1;
					newJdCen.R=rad;
					newJdCen.A2=m_AorL2;
					newJdCen.LinkAfter=NotRelation;

					//�Ƴ����ߵĺ��潻��/Բ
					XLObj1->JdCenArray.RemoveAt(JDNo1+1,XLObj1->JdCenNum-(JDNo1+1));
					XLObj1->JdCenArray.Add(newJdCen);
					for(int j=JDNo2;j<XLObj2->JdCenNum;j++)XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
					XLObj1->JdCenNum=XLObj1->JdCenArray.GetSize();
				}
			}			
		}
		else if(XYClass1==XianYuan::LINE && XYClass2==XianYuan::ARC)
		{
			BAS_DRAW_FUN::JdCenter *JdCen1,*JdCen2,*JdCen3,*JdCen4;

			JDNo1--;
			//��ȡ��ӵ���������/Բ�ĵ�ָ��
			JdCen1=&XLObj1->JdCenArray[JDNo1];		
			JdCen2=&XLObj1->JdCenArray[JDNo1+1];
			JdCen3=&XLObj2->JdCenArray[JDNo2];
			JdCen4=&XLObj2->JdCenArray[JDNo2+1];

			startN1=JdCen1->JDN;
			startE1=JdCen1->JDE;
			endN1=JdCen2->JDN;
			endE1=JdCen2->JDE;

			CenN2=JdCen3->CenN;
			CenE2=JdCen3->CenE;
			R2=JdCen3->R;//Բ�ļ��뾶

			//���Aֵ
			int LeftOrRight=-1;
			ZYLink ZYObj(startN1,startE1,endN1,endE1,CenN2,CenE2,R2,-1);
			if(!ZYObj.ERRFlag)
				JdCen3->A1=ZYObj.pSPIRAL.A;
			else
				JdCen3->A1=0.0;

			//�б����Ծ���XLObj2�Ƿ���Ҫ����
			//1.�ٶ���ת,�û���������㷽λ���б���ת������ת
			double sfwj = ZYObj.pSPIRAL.StartFwj;
			double fwj1 = JdCen1->AftFwj;

			if(fabs(sfwj-fwj1)>0.5*PI)//��ת
			{
				//2.���б����ת�����¼��㻺�������յ������ͷ�λ��
				ZYObj.Set(startN1,startE1,endN1,endE1,CenN2,CenE2,R2,1);
				LeftOrRight=1;
			}
			//3.�û����յ�����ͶӰ��XLObj2�ϣ������ͶӰ������߷�λ�ǣ���ͬ�����򣬷�����
			//�Ƴ����ߵĺ��潻��/Բ
			XLObj1->JdCenArray.RemoveAt(JDNo1+1,XLObj1->JdCenNum-(JDNo1+1));
			if(JdCen3->LeftOrRight!=LeftOrRight)//XLObj2�跴��
			{
				JdCen4=&XLObj2->JdCenArray[JDNo2-1];
				if(JdCen3->JDXZ==IsJiaoDian)
				{
					//���¼��㽻������
					BAS_DRAW_FUN::INTERS(&JdCen3->N,&JdCen3->E,
						JdCen1->JDN,JdCen1->JDE,JdCen2->JDN,JdCen2->JDE,
						JdCen3->JDN,JdCen3->JDE,JdCen4->JDN,JdCen4->JDE);
				}
				for(int j=JDNo2;j>=0;j--)
				{
					if(XLObj2->JdCenArray[j].JDXZ==IsCenter)
						XLObj2->JdCenArray[j].LeftOrRight =  -1*XLObj2->JdCenArray[j].LeftOrRight;
					ads_printf(L"%d %lf %lf %d\n",j,XLObj2->JdCenArray[j].N,XLObj2->JdCenArray[j].E,XLObj2->JdCenArray[j].LeftOrRight);
					XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
				}
			}
			else
			{

				if(JdCen3->JDXZ==IsJiaoDian)
				{
					//���¼��㽻������
					BAS_DRAW_FUN::INTERS(&JdCen3->N,&JdCen3->E,
						JdCen1->JDN,JdCen1->JDE,JdCen2->JDN,JdCen2->JDE,
						JdCen3->JDN,JdCen3->JDE,JdCen4->JDN,JdCen4->JDE);
				}

				for(int j=JDNo2;j<XLObj2->JdCenNum;j++)
					XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
			}
			XLObj1->JdCenNum=XLObj1->JdCenArray.GetSize();			

		}
		else if(XYClass1==XianYuan::ARC && XYClass2==XianYuan::LINE)//������
		{
			BAS_DRAW_FUN::JdCenter *JdCen1,*JdCen2,*JdCen3,*JdCen4;

			JDNo2--;
			//��ȡ��ӵ���������/Բ�ĵ�ָ��
			JdCen1=&XLObj2->JdCenArray[JDNo2];		
			JdCen2=&XLObj2->JdCenArray[JDNo2+1];
			JdCen3=&XLObj1->JdCenArray[JDNo1];
			JdCen4=&XLObj1->JdCenArray[JDNo1-1];

			startN1=JdCen1->JDN;
			startE1=JdCen1->JDE;
			endN1=JdCen2->JDN;
			endE1=JdCen2->JDE;

			CenN2=JdCen3->CenN;
			CenE2=JdCen3->CenE;
			R2=JdCen3->R;//Բ�ļ��뾶

			if(JdCen3->JDXZ==IsJiaoDian)
			{
				//���¼��㽻������
				BAS_DRAW_FUN::INTERS(&JdCen3->N,&JdCen3->E,
					JdCen1->JDN,JdCen1->JDE,JdCen2->JDN,JdCen2->JDE,
					JdCen3->JDN,JdCen3->JDE,JdCen4->JDN,JdCen4->JDE);
			}

			//���Aֵ
			ZYLink ZYObj(startN1,startE1,endN1,endE1,CenN2,CenE2,R2,-1*JdCen3->LeftOrRight);
			if(!ZYObj.ERRFlag)
				JdCen3->A2=ZYObj.pSPIRAL.A;
			else
				JdCen3->A2=0.0;

			ads_printf(L"wwww=%0.1lf %0.1lf",R2,JdCen3->A2);

			//�б����Ծ���XLObj2�Ƿ���Ҫ����
			//1.��������������������ͷ�λ��
			double sfwj = ZYObj.pSPIRAL.StartFwj;
			double fwj2 = JdCen1->AftFwj;

			XLObj1->JdCenArray.RemoveAt(JDNo1+1,XLObj1->JdCenNum-(JDNo1+1));
			if(fabs(sfwj-fwj2)<0.1)//����
			{

				for(int j=JDNo2;j>=0;j--)
				{
					if(XLObj2->JdCenArray[j].JDXZ==IsCenter)
						XLObj2->JdCenArray[j].LeftOrRight =  -1*XLObj2->JdCenArray[j].LeftOrRight;
					XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
				}				
			}
			else
			{
				for(int j=JDNo2+1;j<XLObj2->JdCenNum;j++)XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
			}
			//�Ƴ����ߵĺ��潻��/Բ
			//�Ƴ����ߵĺ��潻��/Բ
			XLObj1->JdCenNum=XLObj1->JdCenArray.GetSize();
		}
		else if(XYClass1==XianYuan::ARC && XYClass2==XianYuan::ARC)
		{			
			//�Ƴ����ߵĺ��潻��/Բ
			XLObj1->TranCenter();
			XLObj2->TranCenter();
			XLObj1->JdCenArray.RemoveAt(JDNo1+1,XLObj1->JdCenNum-(JDNo1+1));

			bool Iffx=false;//�б�XLobj2�Ƿ���
			if(m_NotL==0)
			{
				int LM;
				if(m_LinkModel==LUAN_MODEL)
				{
					LM=Luan_Link;
					if(XLObj1->JdCenArray[JDNo1].LeftOrRight != XLObj2->JdCenArray[JDNo2].LeftOrRight)
						Iffx=true;//
				}
				if(m_LinkModel==C_MODEL)
				{
					LM=C_Link;
					if(XLObj1->JdCenArray[JDNo1].LeftOrRight != XLObj2->JdCenArray[JDNo2].LeftOrRight)
						Iffx=true;//					
				}
				if(m_LinkModel==S_MODEL)
				{
					LM=S_Link;
					if(XLObj1->JdCenArray[JDNo1].LeftOrRight == XLObj2->JdCenArray[JDNo2].LeftOrRight)
						Iffx=true;//
				}
				XLObj1->JdCenArray[JDNo1].LinkAfter=LM;
			}
			else
			{
				XLObj1->JdCenArray[JDNo1].LinkAfter=NotRelation;

				int lr1 = GetSide(XLObj1->JdCenArray[JDNo1].JDN,XLObj1->JdCenArray[JDNo1].JDE,
					XLObj2->JdCenArray[JDNo2].JDN,XLObj2->JdCenArray[JDNo2].JDE,
					XLObj1->JdCenArray[JDNo1].CenN,XLObj1->JdCenArray[JDNo1].CenE);
				int lr2 = GetSide(XLObj1->JdCenArray[JDNo1].JDN,XLObj1->JdCenArray[JDNo1].JDE,
					XLObj2->JdCenArray[JDNo2].JDN,XLObj2->JdCenArray[JDNo2].JDE,
					XLObj2->JdCenArray[JDNo2].CenN,XLObj2->JdCenArray[JDNo2].CenE);
				if(lr1*lr2>0)//ͬ��
				{
					if(XLObj1->JdCenArray[JDNo1].LeftOrRight != XLObj2->JdCenArray[JDNo2].LeftOrRight)
						Iffx=true;//
				}
				else//S
				{
					if(XLObj1->JdCenArray[JDNo1].LeftOrRight == XLObj2->JdCenArray[JDNo2].LeftOrRight)
						Iffx=true;//
				}

			}

			if(m_AorL==1)//ΪL
			{
				//ת��ΪA
				m_AorL1=sqrt(XLObj1->JdCenArray[JDNo1].R*m_AorL1);
				m_AorL2=sqrt(XLObj1->JdCenArray[JDNo1+1].R*m_AorL2);
			}

			if(Iffx)
			{
				for(int j=JDNo2;j>=0;j--)
				{
					if(XLObj2->JdCenArray[j].JDXZ==IsCenter)
						XLObj2->JdCenArray[j].LeftOrRight =  -1*XLObj2->JdCenArray[j].LeftOrRight;
					XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);
				}				
			}
			else
				for(int j=JDNo2;j<XLObj2->JdCenNum;j++)
					XLObj1->JdCenArray.Add(XLObj2->JdCenArray[j]);

			XLObj1->JdCenArray[JDNo1].A2=m_AorL1;
			XLObj1->JdCenArray[JDNo1+1].A1=m_AorL2;

			XLObj1->JdCenNum=XLObj1->JdCenArray.GetSize();	

		}
		/*
		else//��һ����Ϊֱ�߱ߣ�ԲԲ����
		{
		int QXClass;//��������
		//�ж�ǰ����Բ��ת��				
		if(pGtzx1->QXB[iJD1][8]<0.0)
		{
		if(pGtzx2->QXB[iJD2][8]<0.0)QXClass=C_F;
		else QXClass=S_F;
		}
		else
		{				
		if(pGtzx2->QXB[iJD2][8]<0.0)QXClass=S_S;
		else QXClass=C_S;
		}
		//ԲԲͨ��ֱ������
		double Lo1,R1,A1,Lo2,R2,A2;
		R1=pGtzx1->JDarray[iJD1][4];
		Lo1=pGtzx1->JDarray[iJD1][5];
		R2=pGtzx2->JDarray[iJD2][4];
		Lo2=pGtzx2->JDarray[iJD2][3];
		A1=sqrt(R1*Lo1);
		A2=sqrt(R2*Lo2);
		double CenN1,CenE1,CenN2,CenE2;//Բ������
		if(XYClass1==3)iXY1++;
		if(XYClass1==4)iXY1--;
		if(XYClass1==1)iXY1-=2;
		if(XYClass2==3)iXY2++;
		if(XYClass2==4)iXY2--;
		if(XYClass2==1)iXY2-=2;
		CenN1=pGtzx1->array[iXY1][8]+R1*cos(pGtzx1->array[iXY1][5]+0.5*PI*pGtzx1->array[iXY1][4]);
		CenE1=pGtzx1->array[iXY1][9]+R1*sin(pGtzx1->array[iXY1][5]+0.5*PI*pGtzx1->array[iXY1][4]);
		CenN2=pGtzx2->array[iXY2][8]+R2*cos(pGtzx2->array[iXY2][5]+0.5*PI*pGtzx2->array[iXY2][4]);
		CenE2=pGtzx2->array[iXY2][9]+R2*sin(pGtzx2->array[iXY2][5]+0.5*PI*pGtzx2->array[iXY2][4]);
		YYQXLink YYQXObj;
		YYQXObj.SetCir1(CenN1,CenE1,R1);
		YYQXObj.SetCir2(CenN2,CenE2,R2);
		YYQXObj.SetA(A1,A2);
		YYQXObj.SetQXClass(QXClass);
		YYQXObj.CalQX();

		//����������
		BAS_DRAW_FUN::INTERS( &pGtzx1->JDarray[iJD1][1],&pGtzx1->JDarray[iJD1][2],
		pGtzx1->JDarray[iJD1-1][1],pGtzx1->JDarray[iJD1-1][2],
		pGtzx1->JDarray[iJD1][1],pGtzx1->JDarray[iJD1][2],
		YYQXObj.GQN1,YYQXObj.GQE1,YYQXObj.GQN2,YYQXObj.GQE2 );

		BAS_DRAW_FUN::INTERS( &pGtzx2->JDarray[iJD2][1],&pGtzx2->JDarray[iJD2][2],
		YYQXObj.GQN1,YYQXObj.GQE1,YYQXObj.GQN2,YYQXObj.GQE2,
		pGtzx2->JDarray[iJD2][1],pGtzx2->JDarray[iJD2][2],
		pGtzx2->JDarray[iJD2+1][1],pGtzx2->JDarray[iJD2+1][2] );

		pGtzx1->NJD=(iJD1+1)+(pGtzx2->NJD-iJD2);
		for(int i=iJD1+1;i<pGtzx1->NJD;i++)
		{
		pGtzx1->JDarray[i][0]=i;
		for(int k=1;k<6;k++)pGtzx1->JDarray[i][k]
		=pGtzx2->JDarray[iJD2+(i-(iJD1+1))][k];
		}

		/ *
		AcDbLine *L=new AcDbLine(AcGePoint3d(YYQXObj.GQE1,YYQXObj.GQN1,0.0),AcGePoint3d(YYQXObj.GQE2,YYQXObj.GQN2,0.0));
		BAS_DRAW_FUN::AddEntityToDbs(L);
		* /
		}*/


		//////////////////////////////////////////////////////////////////////////			
		XLObj1->close();
		XLObj2->erase();
		XLObj2->close();
		XLObj1=NULL;
		XLObj2=NULL;
	}
	else
	{
		AfxMessageBox(L"��ѡ����·!");
	}
	acDocManager->unlockDocument(acDocManager->curDocument());	
}
