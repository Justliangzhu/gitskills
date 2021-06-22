// SetSprialdlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "SetSprialdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetSprialdlg dialog


SetSprialdlg::SetSprialdlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetSprialdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetSprialdlg)
	m_qdpj = 0.0;
	m_qxcd = 0.0;
	m_qdpianjv = 0.0;
	m_qxzdbj = 0.0;
	m_qxqdbj = 0.0;
	m_qxcsaz = 0.0;
	m_qxpzj = 0.0;
	m_pazypian = 0;
	m_qdzyce = 0;
	m_qdzypian = 0;	
	//}}AFX_DATA_INIT
	Vpazypian  = -1;
	Vqdzyce = -1;
	Vqdzypian = -1;

}
SetSprialdlg::SetSprialdlg(AcDbObjectId EntityId,CWnd* pParent): CDialog(SetSprialdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetSprialdlg)
	m_qdpj = 0.0;
	m_qxcd = 0.0;
	m_qdpianjv = 0.0;
	m_qxzdbj = 0.0;
	m_qxqdbj = 0.0;
	m_qxcsaz = 0.0;
	m_qxpzj = 0.0;
	m_pazypian = 0;
	m_qdzyce = 0;
	m_qdzypian = 0;	
	//}}AFX_DATA_INIT
	Vpazypian = -1;
	Vqdzyce = -1;
	Vqdzypian = -1;
	JDid = EntityId;
	GottnEntId = EntityId;
	CreatOrnot = FALSE;
	RadForWard = 0;
}
void SetSprialdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetSprialdlg)
	DDX_Text(pDX, IDC_EDITSSJIAODU, m_qdpj);
	DDX_Text(pDX, IDC_EDITSSLEN, m_qxcd);
	DDX_Text(pDX, IDC_EDITSSPIANJV, m_qdpianjv);
	DDX_Text(pDX, IDC_EDITSSRE, m_qxzdbj);
	DDX_Text(pDX, IDC_EDITSSRS, m_qxqdbj);
	DDX_Text(pDX, IDC_EDITSSVA, m_qxcsaz);
	DDX_Text(pDX, IDC_EDITSSVDEVERT, m_qxpzj);
	DDX_Radio(pDX, IDC_RADIOSSPAZOUPIAN, m_pazypian);
	DDX_Radio(pDX, IDC_RADIOSSZOUCE, m_qdzyce);
	DDX_Radio(pDX, IDC_RADIOSSZOUPIAN, m_qdzypian);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(SetSprialdlg, CDialog)
	//{{AFX_MSG_MAP(SetSprialdlg)
	ON_BN_CLICKED(IDSETSPRIALOK, OnSetsprialok)
	ON_BN_CLICKED(IDSETSPRIALCANCEL, OnSetsprialcancel)
	ON_BN_CLICKED(IDC_RADIOSSZOUCE, OnRadiosszouce)
	ON_BN_CLICKED(IDC_RADIOSSYOUCE, OnRadiossyouce)
	ON_BN_CLICKED(IDC_RADIOSSPAZOUPIAN, OnRadiosspazoupian)
	ON_BN_CLICKED(IDC_RADIOSSZOUPIAN, OnRadiosszoupian)
	ON_BN_CLICKED(IDC_RADIOSSYOUPIAN, OnRadiossyoupian)
	ON_BN_CLICKED(IDC_RADIOSSPAYOUPIAN2, OnRadiosspayoupian2)
	ON_BN_CLICKED(IDC_BUTTONDRAGENDDOT,OnButtonDragenddot)
	ON_BN_CLICKED(IDC_BUTTONDRUPDATE,OnButtonUpdate)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,OnAcadKeepFocus)
	//	ON_EN_CHANGE(IDC_EDITSSRE, OnChangeEditssre)
	//	ON_EN_CHANGE(IDC_EDITSSVA, OnChangeEditssva)
	//	ON_EN_CHANGE(IDC_EDITSSLEN,OnChangeEditsslen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetSprialdlg message handlers

void SetSprialdlg::OnSetsprialok() 
{
	OnButtonUpdate();
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcDbPolyline * pline;
	if (acdbOpenObject(pline,entid,AcDb::kForWrite) == Acad::eOk)
	{
		pline->erase();
		pline->close();	
	}	
	if (!ResultS) 
	{
		ads_printf(L"\n修改缓曲线出错或缓曲线布设不成功!\n");
	}
	else 
	{

		//		if (SelItem == 1||SelItem ==2) 
		//		{
		JD_CENTER *JdCenObj=new JD_CENTER;
		//			JdCenObj->assertWriteEnabled();
		JdCenObj->SetXYArray(arrxy);
		m_XYNum = JdCenObj->XYNum;
		XYarray xray;
		xray.eID=0;
		int i,j;
		for(i=0;i<=m_XYNum;i++)
		{
			for(j=0;j<10;j++)
				xray.xyarr[j]=JdCenObj->XYArray[i][j];
			pXYArr.Add(xray);
		}

		for(i=1;i<=m_XYNum;i++)
		{
			if(pXYArr[i].xyarr[0]<1.1)
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,7);
			else if(pXYArr[i].xyarr[0]>2.1)
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,6);
			else
				DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,2);
		}

		int res=GetXdataFromEntity(GottnEntId,xray);

		if(res)//InputEntId有扩展数据
			SetXYXdata(GottnEntId, pXYArr[1].eID,xray.xyarr);	
		SetXYXdata(pXYArr[1].eID,0,pXYArr[1].xyarr);
		//			AddEntityToDbs(JdCenObj);
		/*
		}
		else 
		{
		JD_CENTER *JdCenObj = NULL;
		if (acdbOpenObject(JdCenObj,GottnEntId,AcDb::kForWrite)!=Acad::eOk) 
		{
		return;
		}
		JdCenObj->assertWriteEnabled();
		JdCenObj->AddXYArray(arrxy);
		AddEntityToDbs(JdCenObj);
		}*/


	}	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();

	ResultS = FALSE;
	CDialog::OnOK();
}

void SetSprialdlg::OnSetsprialcancel() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	UpdateData(TRUE);
	//删除旧的线元样式;
	if (entid != NULL) 
	{
		AcDbPolyline * pline;
		if(acdbOpenObject(pline,entid,AcDb::kForWrite)==Acad::eOk)
		{
			pline->erase();
			pline->close();	
			entid = NULL;
		}

	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	CDialog::OnCancel();
}

void SetSprialdlg::OnRadiosszouce() 
{
	// TODO: Add your control notification handler code here
	Vqdzyce = -1;
}

void SetSprialdlg::OnRadiossyouce() 
{
	// TODO: Add your control notification handler code here
	Vqdzyce = 1;
}

void SetSprialdlg::OnRadiosspazoupian() 
{
	// TODO: Add your control notification handler code here
	Vpazypian = -1;
}

void SetSprialdlg::OnRadiosszoupian() 
{
	// TODO: Add your control notification handler code here
	Vqdzypian = -1;
}

void SetSprialdlg::OnRadiossyoupian() 
{
	// TODO: Add your control notification handler code here
	Vqdzypian = 1;
}

void SetSprialdlg::OnRadiosspayoupian2() 
{
	// TODO: Add your control notification handler code here
	Vpazypian = 1;
}

afx_msg LRESULT SetSprialdlg::OnAcadKeepFocus(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

void SetSprialdlg::OnButtonDragenddot()
{
	//	AcDbCustomOsnapManager  OsnapManager;
	// 激活CAD文档;
	struct resbuf resOSMODE;//设置OSNAP
	resOSMODE.restype=RTSHORT;

	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		return;
	}
	UpdateData(TRUE);
	//如果显示样式还没有创建;
	if (!CreatOrnot) 
	{
		//获取计算单元;
		GetComItemFromSelEntity(GottnEntId,InterItem);
		GetComItemFromSelEntity(GottnEntId,ComItem,InterItem.ItemDef,FALSE);
		CreatOrnot = TRUE;
	}
	//获取选择的线元类型;
	SelItem = int(InterItem.ItemType);
	//定义圆引缓时圆的偏向变量;
	double CirDef = 0;
	//直引缓时;
	if (SelItem == 1 || SelItem == 31)
	{
		//直线或JD直线实体的终点;
		SprSX = InterItem.FDotX;
		SprSY = InterItem.FDotY;
		SprOA = InterItem.FDotOrient;
		SprSR = 1E50;
		RadForWard = LargeToSmall;
	}
	else if (SelItem == 2 || SelItem == 32 
		|| SelItem == 33 || SelItem == 35)
	{
		//圆弧或JD圆弧实体的终点;
		SprSX = ComItem.FDotX;
		SprSY = ComItem.FDotY;
		SprOA = ComItem.FDotOrient;
		SprSR = ComItem.ItemPro;
		RadForWard = SmallToLarge;

		CirDef = ComItem.ItemDef;
	}
	else if(SelItem == 34)
	{
		//缓曲线的源点;
		SprSX = ComItem.FDotX;
		SprSY = ComItem.FDotY;
		SprOA = ComItem.FDotOrient;
		SprSR = ComItem.ItemEndR;
		RadForWard = LargeToSmall;
	}
	else if(SelItem == 36)
	{
		//缓曲线的源点;
		GetOrignCorOfSprial(InterItem.FDotY,InterItem.FDotX,InterItem.ItemStartR,
			InterItem.FDotOrient,InterItem.ItemPro,InterItem.ItemDef,InterItem.RadForWard,
			SprSY,SprSX,SprOA);
		SprSR = 1E50;
		RadForWard = LargeToSmall;
	}
	else
	{
		AfxMessageBox(L"当前缓曲线不宜再引出缓曲线!");
		return ;
	}	

	AcGePoint3d StaDot,EndDot,PickDot;
	int type;
	struct resbuf result;
	RouteDot dot;
	bool IsOk;
	ads_point PT;

	for( ; ; )
	{
		acedGetVar(L"autosnap",&resOSMODE);
		short OsnapValue=resOSMODE.resval.rint;


		if(ads_grread(1, &type, &result)!=RTNORM)
		{ 
			IsOk=false;
			break;               
		}
		short KEYCODE=result.resval.rint;
		if(KEYCODE==13 || KEYCODE==32 || type==25)
		{
			break;
		}
		if(type == 3) break;
		else if(type == 5)
		{


			if(OsnapValue>0)//捕捉方式打开
			{
				acedOsnap(		
					result.resval.rpoint,

					L"endp,int",

					result.resval.rpoint);

			}

			PickDot.x=result.resval.rpoint[X];
			PickDot.y=result.resval.rpoint[Y];
			PickDot.z=result.resval.rpoint[Z];




			//acedCommandS(RTSTR,"point",RT3DPOINT,result.resval.rpoint,0); 
			if (SelItem == 1|| SelItem == 31)
			{		
				ValueA = GetValueAOfSprial(SprSX,SprSY,SprOA,PickDot.x,PickDot.y,SPrialER,SPrialLen);				
				SprDef = GetDeflectOfDotToLine(SprSX,SprSY,SprOA,PickDot.x,PickDot.y);
				if (ValueA != 1E50) 
				{
					ResultS = TRUE;
					//显示参数;
					m_qxcd = ChangeDataPrecision(ValueA*ValueA/SPrialER);
					m_qxzdbj = ChangeDataPrecision(SPrialER);
					m_qxqdbj = -1;//1E50;
					m_qxcsaz = ChangeDataPrecision(ValueA);
					m_qxpzj = ChangeDataPrecision(ValueA*ValueA/SPrialER/SPrialER/2*180/PI);
					UpdateData(FALSE);
				}	
				else
				{	
					ResultS = FALSE;
				}
			}
			else if (SelItem == 2|| SelItem == 32
				|| SelItem == 33 || SelItem == 35) 
			{
				SprDef = GetDeflectOfDotToLine(SprSX,SprSY,SprOA,PickDot.x,PickDot.y);
				if (SprSR > 8000)
				{						
					// 视SprSR为无穷大;
					ValueA = GetValueAOfSprial(SprSX,SprSY,SprOA,PickDot.x,PickDot.y,SPrialER,SPrialLen);
					if (fabs(ValueA - 1E50) > DataPrecision) 
					{
						ResultS = TRUE;
						// 显示参数;
						m_qxcd = ChangeDataPrecision(ValueA*ValueA/SPrialER);
						m_qxzdbj = ChangeDataPrecision(SPrialER);
						m_qxqdbj = -1;//1E50;
						m_qxcsaz = ChangeDataPrecision(ValueA);
						m_qxpzj = ChangeDataPrecision(ValueA*ValueA/SPrialER/SPrialER/2*180/PI);
						UpdateData(FALSE);
					}
					else 
					{
						ResultS = FALSE;
						continue ;
					}
				}
				else
				{					
					SprDef = GetDeflectOfDotToLine(SprSX,SprSY,SprOA,PickDot.x,PickDot.y);
					ResultS = GetValueAOfSprial(SprSX,SprSY,SprSR,SprOA,PickDot.x,PickDot.y,SPrialER,
						OrX,OrY,OrO,SPrialLen,ValueA,SmallToLarge,SprDef);					
					if (ResultS) 
					{
						// 显示参数;
						m_qxcd = ChangeDataPrecision(ValueA*ValueA*fabs(1/SprSR - 1/SPrialER));
						m_qxzdbj = -1;//ChangeDataPrecision(SPrialER);
						m_qxqdbj = ChangeDataPrecision(SprSR);
						m_qxcsaz = ChangeDataPrecision(ValueA);
						m_qxpzj = ChangeDataPrecision(fabs(ValueA*ValueA/SPrialER/SPrialER/2-ValueA*ValueA/SprSR/SprSR/2)*180/PI);
						UpdateData(FALSE);
					}
				}
			}
			else
			{
				if (SelItem == 36|| SelItem == 34) 
				{
					SprDef = GetDeflectOfDotToLine(SprSX,SprSY,SprOA,PickDot.x,PickDot.y);
					ValueA = GetValueAOfSprial(SprSX,SprSY,SprOA,PickDot.x,PickDot.y,SPrialER,SPrialLen);									
					if (ValueA != 1E50) 
					{
						ResultS = TRUE;
						//显示参数;
						m_qxcd = ChangeDataPrecision(ValueA*ValueA/SPrialER);
						m_qxzdbj = ChangeDataPrecision(SPrialER);
						m_qxqdbj = -1;//1E50;
						m_qxcsaz = ChangeDataPrecision(ValueA);
						m_qxpzj = ChangeDataPrecision(ValueA*ValueA/SPrialER/SPrialER/2*180/PI);
						UpdateData(FALSE);
					}
				}
			}
			if (ResultS)
			{
				ads_printf(L"当前缓曲线布设成功!\n");
				//设置缓曲实际偏向
				if (fabs(SprDef-1.0) < DataPrecision)
				{
					m_pazypian = 1;
					Vpazypian = 1;
				}
				else
				{
					m_pazypian = 0;
					Vpazypian = -1;
				}
				UpdateData(FALSE);
				ShowSpiralCirOnScreen(SprSY,SprSX,SprSR,SprOA,SPrialER,SprDef,ValueA,SPrialLen,entid);			
			}
		}
		//	acedCommandS(0); 
	}
	if (ResultS)
	{					
		//被加入线元;
		if (SelItem == 1||SelItem == 31)
		{	
			arrxy[1][0] = 3;
			arrxy[1][2] = SPrialER;
			arrxy[1][7] = 1e40;
			SPrialLen = ValueA*ValueA/SPrialER;
		}
		else if (SelItem == 2||SelItem == 32
			|| SelItem == 33 || SelItem == 35)
		{
			//如果缓曲线布设时圆缓反向且圆曲半径较大;
			if (SprSR > 8000)
			{
				arrxy[1][0] = 3;
				arrxy[1][2] = SPrialER;
				arrxy[1][7] = 1e40;
				SPrialLen = ValueA*ValueA/SPrialER;
			}
			else
			{				
				arrxy[1][0] = 4;
				arrxy[1][2] = SprSR;
				arrxy[1][7] = SprSR;
				SPrialLen = ValueA*ValueA/SprSR;						
			}
		}
		else if (SelItem == 34||SelItem == 36) 
		{
			arrxy[1][0] = 3;
			arrxy[1][2] = SPrialER;
			arrxy[1][7] = 1e40;
			SPrialLen = ValueA*ValueA/SPrialER;
		}
		arrxy[1][1] = ValueA;	
		arrxy[1][3] = SPrialER;
		arrxy[1][4] = SprDef;
		arrxy[1][5] = SprOA;
		arrxy[1][6] = 0;
		arrxy[1][8] = SprSY;
		arrxy[1][9] = SprSX;

		//被代替线元;
		if (SelItem == 1||SelItem == 2)
		{
			arrxy[0][0] = 0;
			arrxy[0][1] = arrxy[1][8];
			arrxy[0][2] = arrxy[1][9];
			arrxy[0][3] = arrxy[1][5];
			arrxy[0][4] = arrxy[1][6];
			arrxy[0][5] = 0;
			arrxy[0][6] = 0;
			arrxy[0][8] = 0;
			arrxy[0][7] = 0;
			arrxy[0][9] = 0;			
		}
		else			
		{
			JD_CENTER *JdCenObj = NULL;
			if (acdbOpenObject(JdCenObj,GottnEntId,AcDb::kForWrite)!=Acad::eOk) 
			{
				return;
			}
			for(int i=0; i <10;i++)
			{
				arrxy[0][i] = JdCenObj->XYArray[JdCenObj->XYNum-1][i];
			}
			if (fabs(arrxy[0][0]-6)<DataPrecision)
			{
				arrxy[0][0] = 4;
				arrxy[0][3] = 1e40;
			}
			JdCenObj->close();
		}
		if (fabs(arrxy[0][0]-1)<DataPrecision)
		{
			arrxy[1][6] = arrxy[0][6] + arrxy[0][1]; 
		}
		if (fabs(arrxy[0][0]-2)<DataPrecision)
		{
			arrxy[1][6] = arrxy[0][6] + arrxy[0][2]; 
		}
		if (fabs(arrxy[0][0]-4)<DataPrecision||fabs(arrxy[0][0]-6)<DataPrecision)
		{
			arrxy[1][6] = arrxy[0][6] + arrxy[0][1]*arrxy[0][1]/arrxy[0][2]; 
		}
		//终点线元;	
		xlpoint SSDot,SEDot;
		double InputData[5];
		for(int jj=0; jj<5; jj++)
			InputData[jj] = arrxy[1][jj];
		SSDot.lc = arrxy[1][6]; 
		SSDot.x = arrxy[1][8];
		SSDot.y = arrxy[1][9];
		SSDot.a = arrxy[1][5];
		SSDot.r = arrxy[1][7];
		xlzbxy(&SSDot,&SEDot,SPrialLen,InputData,0);
		arrxy[2][0] = 0;
		arrxy[2][1] = 0;
		arrxy[2][2] = 0;
		arrxy[2][3] = 0;
		arrxy[2][4] = 0;
		arrxy[2][6] = SEDot.lc;
		arrxy[2][5] = SEDot.a;
		arrxy[2][7] = SEDot.r;
		arrxy[2][8] = SEDot.x;
		arrxy[2][9] = SEDot.y;		
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	//SetFocus();
	ads_printf(L"\nCommand:");

}

void SetSprialdlg::OnButtonUpdate()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	double StartX,StartY,StartOA;
	StartX = SprSX;
	StartY = SprSY;
	StartOA = SprOA;
	//如果缓曲线布设成功;

	if (ResultS) 
	{				
		//获取新数据;
		UpdateData(TRUE);
		//重置起点数据;
		ModifyStartDot(StartX,StartY,Vqdzyce,m_qdpianjv,StartOA,Vqdzypian,m_qdpj);
		//重置缓曲的参数;
		ValueA = m_qxcsaz;
		SPrialER = m_qxzdbj;	
		//如果终点半径为-1则;
		if (SPrialER < 0)
		{
			if (fabs(SPrialER+1) < DataPrecision) 
			{
				SPrialER = 1E50;
			}
			else 
			{
				ads_printf(L"\n终点半径输入有误!\n");
				return ;
			}
		}
		//判断给出的终点半径是否有效;
		if (RadForWard == LargeToSmall) 
		{ 
			if (SprSR<SPrialER || fabs(SprSR-SPrialER)<DataPrecision || SPrialER<DataPrecision) 
			{
				ads_printf(L"\n终点半径不合理!\n");
				UpdateData(FALSE);
				return ;
			}
		}
		SprDef = Vpazypian;
		SPrialLen = m_qxcd;
		//判断获得值的有效性;
		if (SPrialLen < DataPrecision||ValueA < DataPrecision||
			SPrialER < DataPrecision)
		{
			return ;
		}
		//起点半径与曲线偏转角是不能被修改的(暂时规定);
		ShowSpiralCirOnScreen(StartY,StartX,SprSR,StartOA,SPrialER,SprDef,ValueA,SPrialLen,entid);
		UpdateData(FALSE);
		//修改线元数据;

		//被加入线元;
		SPIRAL pSpir;
		if (SelItem == 1||SelItem == 31)
		{	
			if (SPrialER > 1E50 - DataPrecision) 
			{
				ads_printf(L"终点半径不合理!\n");
				return ;
			}
			arrxy[1][0] = 3;
			arrxy[1][2] = SPrialER;
			arrxy[1][7] = 1e40;
		}
		else if (SelItem == 2||SelItem == 32
			|| SelItem == 33 || SelItem == 35) 
		{
			//如果缓曲线布设时圆曲半径较大;
			if (SprSR > 8000)
			{
				if (SPrialER > 1E50 - DataPrecision) 
				{
					AfxMessageBox(L"终点半径不合理!");
					return ;
				}
				arrxy[1][0] = 3;
				arrxy[1][2] = SPrialER;
				arrxy[1][7] = SPrialER;
				SPrialLen = ValueA*ValueA/SPrialER;
			}
			else
			{								
				if (SPrialER < 1E50 - DataPrecision && SPrialER > SprSR) 
				{
					arrxy[1][0] = 6;
					SPrialLen = ValueA*ValueA*fabs(1/SPrialER-1/SprSR);
				}
				if (SPrialER < SprSR) 
				{
					arrxy[1][0] = 5;
					SPrialLen = ValueA*ValueA*fabs(1/SPrialER-1/SprSR);
				}
				arrxy[1][2] = SprSR;
				arrxy[1][7] = SprSR;
			}
		}
		else if (SelItem == 34||SelItem == 36) 
		{
			arrxy[1][0] = 3;
			arrxy[1][2] = SPrialER;
			arrxy[1][7] = 1e40;
			SPrialLen = ValueA*ValueA/SPrialER;
		}

		arrxy[1][1] = ValueA;	
		arrxy[1][3] = SPrialER;
		arrxy[1][4] = SprDef;
		arrxy[1][5] = StartOA;
		//起点里程不变;
		arrxy[1][8] = StartY;
		arrxy[1][9] = StartX;
		//终点线元;	
		xlpoint SSDot,SEDot;
		double InputData[5];
		for(int jj=0; jj<5; jj++)
			InputData[jj] = arrxy[1][jj];
		SSDot.lc = arrxy[1][6]; 
		SSDot.x = arrxy[1][8];
		SSDot.y = arrxy[1][9];
		SSDot.a = arrxy[1][5];
		SSDot.r = arrxy[1][7];
		xlzbxy(&SSDot,&SEDot,SPrialLen,InputData,0);
		arrxy[2][0] = 0;
		arrxy[2][1] = 0;
		arrxy[2][2] = 0;
		arrxy[2][3] = 0;
		arrxy[2][4] = 0;
		arrxy[2][6] = SEDot.lc;
		arrxy[2][5] = SEDot.a;
		arrxy[2][7] = SEDot.r;
		arrxy[2][8] = SEDot.x;
		arrxy[2][9] = SEDot.y;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();

	return ;
}

BOOL SetSprialdlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	struct resbuf resOSMODE;//设置OSNAP
	resOSMODE.restype = RTSHORT;
	// 激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	// 获取新数据;
	UpdateData(TRUE);
	//打开所获取的实体;
	AcDbEntity* pEntity = NULL;
	if (acdbOpenObject(pEntity, GottnEntId, AcDb::kForRead) != Acad::eOk)
	{
		return FALSE;
	}
	if (pEntity == NULL)
	{
		return FALSE;
	}

	// 判断选中实体是否为匝道实体;
	BOOL IsRampRoad = FALSE;
	BAS_DRAW_FUN BAS1;
	AcDbObjectId lineId;
	if (_tcscmp(pEntity->isA()->name(), L"JD_CENTER") == 0)
	{
		BAS1.resetjd_centerid(point, pEntity, lineId);
		IsRampRoad = TRUE;
		pEntity->close();
		GottnEntId = lineId;
	}
	//若打开的是一般的缓和曲线，则将其变成直线，进行之后的求解2019.11.4zl
	if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0) //缓和曲线
	{
		XYarray xray;
		int res = GetXdataFromEntity(GottnEntId, xray);
		if (res == 0)
		{
			GottnEntId = NULL;
			AcDbPolyline* m_polyline = AcDbPolyline::cast(pEntity);
			int numverts = 0;
			int i = 0;
			int trow = 0;
			numverts = m_polyline->numVerts();
			AcGePoint3d  location, location1;
			for (i = 0; i < numverts; i++)
			{  //寻找的某顶点
				trow = i;
				m_polyline->getPointAt(i, location);
				m_polyline->getPointAt(i + 1, location1);
				BAS_DRAW_FUN_RAIL bas;
				double dXchuizu, dYchuizu;
				bas.ChuizuToZx(dXchuizu, dYchuizu, point.x, point.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y) < 0)
				{
					AcDbLine* pl1 = new AcDbLine(location, location1);
					GottnEntId = AddEntityToDbs(pl1);
					break;
				}
			}
			/*ads_printf(L"选中实体无效!\n");
			pEntity->close();*/
		}
	}
	else
	{
		pEntity->close();
	}





	// TODO: Add extra initialization here
	if (!CreatOrnot) 
	{
		//获取计算单元;
		GetComItemFromSelEntity(GottnEntId,InterItem);
		GetComItemFromSelEntity(GottnEntId,ComItem,InterItem.ItemDef,FALSE);
		CreatOrnot = TRUE;
	}
	//获取选择的线元类型;
	SelItem = int(InterItem.ItemType);
	//定义圆引缓时圆的偏向变量;
	double CirDef = 0;
	//直引缓时;
	if (SelItem == 1 || SelItem == 31)
	{
		//直线或JD直线实体的终点;
		SprSX = InterItem.FDotX;
		SprSY = InterItem.FDotY;
		SprOA = InterItem.FDotOrient;
		SprSR = 1E50;
		RadForWard = LargeToSmall;
	}
	else if (SelItem == 2 || SelItem == 32 
		|| SelItem == 33 || SelItem == 35)
	{
		//圆弧或JD圆弧实体的终点;
		SprSX = ComItem.FDotX;
		SprSY = ComItem.FDotY;
		SprOA = ComItem.FDotOrient;
		SprSR = ComItem.ItemPro;
		RadForWard = SmallToLarge;

		CirDef = ComItem.ItemDef;
	}
	else if(SelItem == 34)
	{
		//缓曲线的源点;
		SprSX = ComItem.FDotX;
		SprSY = ComItem.FDotY;
		SprOA = ComItem.FDotOrient;
		SprSR = ComItem.ItemEndR;
		RadForWard = LargeToSmall;
	}
	else if(SelItem == 36)
	{
		//缓曲线的源点;
		GetOrignCorOfSprial(InterItem.FDotY,InterItem.FDotX,InterItem.ItemStartR,
			InterItem.FDotOrient,InterItem.ItemPro,InterItem.ItemDef,InterItem.RadForWard,
			SprSY,SprSX,SprOA);
		SprSR = 1E50;
		RadForWard = LargeToSmall;
	}
	else
	{
		AfxMessageBox(L"当前缓曲线不宜再引出缓曲线!");
		return TRUE;
	}	



	if (SprSR>99999)
		m_qxqdbj = -1;//1E50;
	else
		m_qxqdbj = ChangeDataPrecision(SprSR);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*
void SetSprialdlg::OnChangeEditssre()
{
	UpdateData(TRUE);
	// 约定1:当终点半径发生改变,A值不变,曲长发生变化;
	double StartR,A,EndR,Len;
	int RadGo;
	//获得起点半径值;	
	StartR = m_qxqdbj;
	if (StartR<0 && fabs(StartR+1)<DataPrecision)
	{
		StartR = 1E50;
	}
	//获得此时的A值;
	A = m_qxcsaz;
	//获得终点此时的半径值;
	EndR = m_qxzdbj;
	//如果终点半径为-1则:
	if (EndR < 0)
	{
		if (fabs(EndR+1) < DataPrecision) 
		{
			EndR = 1E50;
		}
		else 
		{
			ResultS = FALSE;
			ads_printf(L"\n终点半径输入有误!\n");
			return ;
		}
	}
	//判断获得值的有效性;
	if (StartR < DataPrecision||A < DataPrecision||
		EndR < DataPrecision)
	{
		ads_printf(L"\n数据有误!\n");
		ResultS = FALSE;
		return ;
	}
	//判断给出的终点半径是否有效;
	RadGo = RadForWard;
	if (RadGo == LargeToSmall) 
	{ 
		if (StartR<EndR || fabs(StartR-EndR)<DataPrecision || EndR<DataPrecision) 
		{
			ResultS = FALSE;
			ads_printf(L"\n终点半径不合理!\n");
			UpdateData(FALSE);
			return ;
		}
	}
	//根据实际半径大小确定半径趋势;
	RadGo = GetRadiusForward(StartR,EndR);
	RadForWard = RadGo;
	//重算曲长;
	if (RadGo == LargeToSmall) 
	{
		m_qxcd  = A*A/EndR;
		m_qxpzj = 180*m_qxcd/(2*EndR*PI);
	}
	else if (RadGo == SmallToLarge) 
	{
		m_qxcd  = A*A/StartR;
		m_qxpzj = 180*m_qxcd/(2*StartR*PI);
	}
	else
	{
		m_qxcd = A*A*fabs(1/StartR-1/EndR);	
		m_qxpzj = 180/PI*A*A*fabs(1/(2*StartR*StartR)-1/(2*EndR*EndR));
	}
	ResultS = TRUE;
	//显示计算结果;
	UpdateData(FALSE);
}*/


/*
void SetSprialdlg::OnChangeEditssva()
{
	UpdateData(TRUE);
	// 约定2:当终点半径发生改变,A值不变,曲长发生变化;
	double StartR,A,EndR,Len;
	int RadGo;
	//获得起点半径值;	
	StartR = m_qxqdbj;
	if (StartR<0 && fabs(StartR+1)<DataPrecision)
	{
		StartR = 1E50;
	}
	//获得此时的A值;
	A = m_qxcsaz;
	if (A > 2000) 
	{
		ads_printf(L"数据修改不合理!\n");
		return ;
	}
	//获得终点此时的半径值;
	EndR = m_qxzdbj;
	//如果终点半径为-1则;
	if (EndR < 0)
	{
		if (fabs(EndR+1) < DataPrecision) 
		{
			EndR = 1E50;
		}
		else 
		{
			ResultS = FALSE;
			ads_printf(L"\n终点半径输入有误!\n");
			return ;
		}
	}		
	//判断获得值的有效性;
	if (StartR < DataPrecision||A < DataPrecision||
		EndR < DataPrecision)
	{
		ads_printf(L"\n数据有误!\n");
		ResultS = FALSE;
		return ;
	}
	RadGo = RadForWard;
	//重算曲长;
	if (RadGo == LargeToSmall) 
	{
		m_qxcd  = A*A/EndR;
		m_qxpzj = 180*m_qxcd/(2*EndR*PI);
	}
	else if (RadGo == SmallToLarge) 
	{
		m_qxcd  = A*A/StartR;
		m_qxpzj = 180*m_qxcd/(2*StartR*PI);
	}
	else
	{
		m_qxcd = A*A*fabs(1/StartR-1/EndR);	 
		m_qxpzj = 180/PI*A*A*fabs(1/(2*StartR*StartR)-1/(2*EndR*EndR));
	}
	ResultS = TRUE;
	//显示计算结果;
	UpdateData(FALSE);
}

void SetSprialdlg::OnChangeEditsslen()
{
	UpdateData(TRUE);
	// 约定3:当曲长发生改变,A值不变,终点半径发生变化;
	double StartR,A;
	int RadGo;
	//获得起点半径值;	
	StartR = m_qxqdbj;
	if (StartR<0 && fabs(StartR+1)<DataPrecision)
	{
		StartR = 1E50;
	}
	//获得此时的A值;
	A = m_qxcsaz;
	//判断获得值的有效性;
	if (StartR < DataPrecision||A < DataPrecision)
	{
		ResultS = FALSE;
		return ;
	}	
	RadGo = RadForWard;
	if (m_qxcd <DataPrecision) 
	{
		ResultS = FALSE;
		return ;
	}
	if (m_qxcd > 2000) 
	{
		ResultS = FALSE;
		return ;
	}
	//重算曲终半径;
	if (RadGo == LargeToSmall) 
	{
		m_qxzdbj  = A*A/m_qxcd;
		m_qxpzj = 180*m_qxcd/(2*m_qxzdbj*PI);
	}
	else if (RadGo == SmallToBig)		
	{
		m_qxzdbj = A*A/(A*A/StartR-m_qxcd);	
		m_qxpzj = 180/PI*A*A*fabs(1/(2*StartR*StartR)-1/(2*m_qxzdbj*m_qxzdbj));
	}
	else if (RadGo == BigToSmall)
	{
		m_qxzdbj = A*A/(A*A/StartR+m_qxcd);	
		m_qxpzj = 180/PI*A*A*fabs(1/(2*StartR*StartR)-1/(2*m_qxzdbj*m_qxzdbj));
	}
	else 
	{
		ResultS = FALSE;
		return ;
	}
	if (m_qxzdbj < DataPrecision) 
	{
		ads_printf(L"\n数据输入出错!\n");
		ResultS = FALSE;
		return ;
	}
	if (fabs(m_qxzdbj-1E50) < DataPrecision)
	{
		m_qxzdbj = -1;
	}
	ResultS = TRUE;
	//显示计算结果;
	UpdateData(FALSE);
}*/









































