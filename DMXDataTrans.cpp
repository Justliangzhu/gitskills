// DMXDataTrans.cpp : 实现文件
//

#include "stdafx.h"
#include "DMXDataTrans.h"
#include "afxdialogex.h"


// DMXDataTrans 对话框

IMPLEMENT_DYNAMIC(DMXDataTrans, CDialog)

DMXDataTrans::DMXDataTrans(CWnd* pParent /*=NULL*/)
	: CDialog(DMXDataTrans::IDD, pParent)
	, m_WDdmxPath(_T(""))
	, m_DRmdbPath(_T(""))
	, m_WDzdmPath(_T(""))
{

}

DMXDataTrans::~DMXDataTrans()
{
}

void DMXDataTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WDdmxPath, m_WDdmxPath);
	DDX_Text(pDX, IDC_EDIT_DRmdbPath, m_DRmdbPath);
	DDX_Text(pDX, IDC_EDIT_ZDMpath, m_WDzdmPath);
}


BEGIN_MESSAGE_MAP(DMXDataTrans, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_browsWD, &DMXDataTrans::OnBnClickedButtonbrowswd)
	ON_BN_CLICKED(IDC_BUTTON_browsDR, &DMXDataTrans::OnBnClickedButtonbrowsdr)
	ON_BN_CLICKED(IDC_BUTTON_DMXtrans, &DMXDataTrans::OnBnClickedButtonDmxtrans)
	ON_BN_CLICKED(IDC_BUTTON_browsZDM, &DMXDataTrans::OnBnClickedButtonbrowszdm)
	ON_BN_CLICKED(IDC_BUTTON_ZDMtrans, &DMXDataTrans::OnBnClickedButtonZdmtrans)
END_MESSAGE_MAP()


// DMXDataTrans 消息处理程序


void DMXDataTrans::OnBnClickedButtonbrowswd()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fname(true,NULL);
	fname.m_ofn.lpstrDefExt = L"dmx";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"纬地地面线文件(*.dmx)\0*.dmx\0";

	if(fname.DoModal()==IDOK)
	{
		m_WDdmxPath = fname.GetPathName();			
	}
	UpdateData(FALSE);
}


void DMXDataTrans::OnBnClickedButtonbrowsdr()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fname(true,NULL);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"数据库(*.mdb)\0*.mdb\0";

	if(fname.DoModal()==IDOK)
	{
		m_DRmdbPath = fname.GetPathName();			
	}
	UpdateData(FALSE);
}


void DMXDataTrans::OnBnClickedButtonDmxtrans()
{
	// TODO: 在此添加控件通知处理程序代码

	////////////////////////先导入平面数据/////////////////////
	JD_CENTER *pJC=NULL;
	XLDataBase DBS;
	DBS.Read_XLDbs(m_DRmdbPath,L"控制点表",L"主线"/*DBS.Road[i].RoadName*/);
	DBS.Read_XLDbs(m_DRmdbPath,L"断链表",L"主线"/*DBS.Road[i].RoadName*/);

	if(DBS.NJdCen>0)
	{
		pJC=new JD_CENTER;

		pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		pJC->setRoadName(L"主线"/*DBS.Road[i].RoadName*/);
		pJC->setMdbName(m_DRmdbPath);

		if(DBS.NDL>0)
		{
			pJC->setDLB(DBS.DLArray,DBS.NDL);
		}
		DBS.Read_XLDbs(m_DRmdbPath,L"线路属性");
		pJC->DV = DBS.RoadXZ.DV;
	}
	pzLinep=pJC;


	//////////////////////导入纬地地面线文件///////////////////////
	if(!PathFileExists(m_WDdmxPath))
		return ;
	//获取地面点数num
	ACHAR *Path = (ACHAR*)m_WDdmxPath.GetBuffer(m_WDdmxPath.GetLength());
	BAS_DRAW_FUN bas;
	int num=bas.CountLines(Path);

	FILE* ReadData;		//文件类
	ReadData = _wfopen(m_WDdmxPath, L"r");

	//空读文件名
	ACHAR EmptyReadChar[1024];//空读字符
	fwscanf(ReadData, L"%s", EmptyReadChar);

	double cml,GC;
	double *dmx;

	if(num>0)
		dmx=new double [num*2];
	else
		return;

	for (int i=0;i<num;i++)
	{
		fwscanf(ReadData, L"%lf", &cml);
		dmx[i*2]=pzLinep->TYLC(cml);
		fwscanf(ReadData, L"%lf", &GC);
		dmx[i*2+1]=GC;
	}

	DMX_ROAD *pDmx=new DMX_ROAD(num,dmx,0/*GlobalZDMTK.CurZDM*/,1);

	if(dmx) delete []dmx;

	pDmx->get_axle_legth();


	/////////////////////////////写入数据库//////////////////////////
	//XLDataBase DBS;

	//写纵地面线
	int NCM;
	double CM[5*MAXBPDNUM];
	CString  GH,str;
	double ml;	   
	DBS.NTW = pDmx->NUM; 
	if(DBS.TWB) {delete []DBS.TWB; DBS.TWB=NULL;}
	if(DBS.NTW>0)
	{
		DBS.TWB = new XLDataBase::TWTAB[DBS.NTW];
		for(int i=0; i<pDmx->NUM;i++)
		{
			ml = pzLinep->XLC1(pDmx->DMXys[i].lc);
			str.Format(L"%0.2lf",ml);
			DBS.TWB[i].ml = _wtof(str);
			str.Format(L"%0.2lf",pDmx->DMXys[i].level);
			DBS.TWB[i].DMGC = _wtof(str);
		}
		DBS.Write_XLDbs(m_DRmdbPath,L"纵地面线表",L"主线");
	}	
	ACHAR mes[256];
	_stprintf(mes, L"已将数据存入%s数据库,道路名: %s",m_DRmdbPath,L"主线");
	ads_alert(mes);	
}


void DMXDataTrans::OnBnClickedButtonbrowszdm()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fname(true,NULL);
	fname.m_ofn.lpstrDefExt = L"zdm";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"纬地纵断面文件(*.zdm)\0*.zdm\0";

	if(fname.DoModal()==IDOK)
	{
		m_WDzdmPath = fname.GetPathName();		
	}

	UpdateData(FALSE);
}


void DMXDataTrans::OnBnClickedButtonZdmtrans()
{
	// TODO: 在此添加控件通知处理程序代码

	////////////////////////先导入平面数据/////////////////////
	JD_CENTER *pJC=NULL;
	XLDataBase DBS;
	DBS.Read_XLDbs(m_DRmdbPath,L"控制点表",L"主线"/*DBS.Road[i].RoadName*/);
	DBS.Read_XLDbs(m_DRmdbPath,L"断链表",L"主线"/*DBS.Road[i].RoadName*/);

	if(DBS.NJdCen>0)
	{
		pJC=new JD_CENTER;

		pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		pJC->setRoadName(L"主线"/*DBS.Road[i].RoadName*/);
		pJC->setMdbName(m_DRmdbPath);

		if(DBS.NDL>0)
		{
			pJC->setDLB(DBS.DLArray,DBS.NDL);
		}
		DBS.Read_XLDbs(m_DRmdbPath,L"线路属性");
		pJC->DV = DBS.RoadXZ.DV;
	}
	pzLinep=pJC;

	////////////////////////////////导入纵断面文件数据///////////////////////////
	//创建GTZDM_ROAD坡度线实体
	int NPD=1;
	double PDarray[MAXBPDNUM][4];
	ACHAR GH[8];
	_tcscpy(GH, L"K");
	int i;
	for(i=0; i<MAXBPDNUM; i++)
		for(int j=0; j<4; j++)
			PDarray[i][j] = 0.0;
	PDarray[0][0] = pzLinep->DLArray[0].ELC;
	GTZDM_ROAD *pZDMline;
	pZDMline = new GTZDM_ROAD(NPD,PDarray,&GH[0],0,3);	
	
	//打开文件读数据
	if(!PathFileExists(m_WDzdmPath))
		return ;

	FILE* ReadData;		//文件类
	ReadData = _wfopen(m_WDzdmPath, L"r");

	//空读文件名
	ACHAR EmptyReadChar[1024];//空读字符
	fwscanf(ReadData, L"%s", EmptyReadChar);

	int BPDnum;
	fwscanf(ReadData, L"%d", &BPDnum);

	pZDMline->NBPD= BPDnum;  
	//设置小数位
	pZDMline->DesHXSW = 6;
	pORI_SCALE.DesHXSW = pZDMline->DesHXSW;
	pZDMline->Xsw= 6;
	pORI_SCALE.XSW = pZDMline->Xsw;

	double cml,GC,Rshu,tylc,emptydouble;
	for (int iBPD=0;iBPD<BPDnum;iBPD++)
	{
		fwscanf(ReadData, L"%lf", &cml);
		tylc=pZDMline->TYLC(cml);//统一里程
		fwscanf(ReadData, L"%lf", &GC);
		fwscanf(ReadData, L"%lf", &Rshu);
		fwscanf(ReadData, L"%lf", &emptydouble);
		fwscanf(ReadData, L"%lf", &emptydouble);

		pZDMline->BPD_array[iBPD][0]=(tylc-pZDMline->K0)/pZDMline->HBVSCALE+pZDMline->X0;
		pZDMline->BPD_array[iBPD][1]=GC-pZDMline->H0+pZDMline->Y0;
		pZDMline->BPD_array[iBPD][2]=Rshu;
	}
	GlobalZDMTK.ZDMTK[GlobalZDMTK.CurZDM].zdm =pZDMline;

	///////////////////////////////////////////写入数据库//////////////////////////////////////
	CString str;

	double LL,II,HH,LC;
	GTZDM_ROAD *pzdm = pZDMline;
	if(pzdm)
	{
		DBS.NZDL=pzdm->DLNum;
		DBS.ZDLArray=new BAS_DRAW_FUN::DLBZ[DBS.NZDL];
		//	acutPrintf(L"NDL=%d\n ",DBS.NDL);
		for(i=0;i<DBS.NZDL;i++)
			DBS.ZDLArray[i]=pzdm->DLArray[i];
		DBS.Write_XLDbs(m_DRmdbPath,L"纵断链表",L"主线"/*dlg.m_RoadName*/);
	}
	if(pzdm)
	{
		//写坡度表
		DBS.NPD = pzdm->NBPD;
		if(DBS.PDB) delete []DBS.PDB;
		if(DBS.NPD>0)
		{
			DBS.PDB = new XLDataBase::PDTAB[DBS.NPD];
			for(i=0; i<DBS.NPD; i++)
			{
				double xlc;
				CString  GH;	
				if(i<DBS.NPD-1)
				{
					LL=(pzdm->BPD_array[i+1][0]-pzdm->BPD_array[i][0])*pzdm->HBVSCALE;
					if (fabs(LL)<0.001) LL=100.0;
					II=(pzdm->BPD_array[i+1][1]-pzdm->BPD_array[i][1])*100.0/LL;			
				}
				else
					LL=II=0.0;

				//坡长
				str.Format(L"%0.2lf",LL);
				DBS.PDB[i].length = _wtof(str);		

				//坡度
				if(II>0.00000000001)
					DBS.PDB[i].degree = ((int)(II*10.0+0.5))/10.0;
				else if(II<-0.00000000001)
					DBS.PDB[i].degree = -1.0*((int)(fabs(II)*10.0+0.5))/10.0;
				else
					DBS.PDB[i].degree = 0.0;          
				//标高
				//		if(i<xlmdb.NPD-1)
				//            pzdm->BPD_array[i+1][1] = pzdm->BPD_array[i][1]+xlmdb.PDB[i].degree*xlmdb.PDB[i].length/1000.0;//根据坡度调整变坡点标高

				LC=(pzdm->BPD_array[i][0]-pzdm->X0)*pzdm->HBVSCALE+pzdm->K0;
				xlc = pzLinep->XLC1(LC);
				//			DBS.PDB[i].GH = GH;
				str.Format(L"%0.3lf",xlc);
				DBS.PDB[i].ml = _wtof(str);

				HH=pzdm->BPD_array[i][1]-pzdm->Y0+pzdm->H0;    // 设计高程
				if(pzdm->DesHXSW==2)
					str.Format(L"%0.2lf",HH);
				else if(pzdm->DesHXSW==3)
					str.Format(L"%0.3lf",HH);
				else if(pzdm->DesHXSW==1)
					str.Format(L"%0.1lf",HH);
				else
					str.Format(L"%0.0lf",HH);

				DBS.PDB[i].Level = _wtof(str);
				DBS.PDB[i].Rshu = pzdm->BPD_array[i][2];
				//竖曲线长度
				double deltaI;//坡度代数差
				if(i>0 && i<DBS.NPD-1)
					deltaI = fabs(DBS.PDB[i].degree - DBS.PDB[i-1].degree);
				else
					deltaI = 0.0;

				DBS.PDB[i].RLen = 2*(DBS.PDB[i].Rshu/200.0)*deltaI;
				DBS.PDB[i].Notes = " ";
			}
			DBS.PDB[0].Rshu = pzdm->DesHXSW;
			DBS.PDB[0].RLen = pzdm->Xsw;
			DBS.PDB[0].Notes="半径:标高小数位数;长度:坡度小数位数";
			DBS.Write_XLDbs(m_DRmdbPath,L"坡度表",L"主线"/*dlg.m_RoadName*/,L"全幅"/*dlg.m_ZPXZ*/);
		}
	}
	ACHAR mes[256];
	_stprintf(mes, L"已将数据存入%s数据库,道路名: %s",m_DRmdbPath,L"主线");
	ads_alert(mes);	



}
