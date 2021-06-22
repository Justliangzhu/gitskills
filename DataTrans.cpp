// DataTrans.cpp : 实现文件
//

#include "stdafx.h"
#include "DataTrans.h"
#include "afxdialogex.h"


// DataTrans 对话框

IMPLEMENT_DYNAMIC(DataTrans, CDialog)

DataTrans::DataTrans(CWnd* pParent /*=NULL*/)
	: CDialog(DataTrans::IDD, pParent)
	, m_WDfilepath(_T(""))
	, m_DRfilepath(_T(""))
{

}

DataTrans::~DataTrans()
{
}

void DataTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WDfilepath, m_WDfilepath);
	DDX_Text(pDX, IDC_EDIT_DRfilepath, m_DRfilepath);
}


BEGIN_MESSAGE_MAP(DataTrans, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_browseWD, &DataTrans::OnBnClickedButtonbrowsewd)
	ON_BN_CLICKED(IDC_BUTTON_browseDR, &DataTrans::OnBnClickedButtonbrowsedr)
	ON_BN_CLICKED(IDC_BUTTON_trans, &DataTrans::OnBnClickedButtontrans)
END_MESSAGE_MAP()


// DataTrans 消息处理程序


void DataTrans::OnBnClickedButtonbrowsewd()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fname(true,NULL);
	fname.m_ofn.lpstrDefExt = L"jd";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"纬地交点设计线形文件(*.jd)\0*.jd\0";

	if(fname.DoModal()==IDOK)
	{
		m_WDfilepath = fname.GetPathName();			
	}
	UpdateData(FALSE);
}


void DataTrans::OnBnClickedButtonbrowsedr()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fname(true,NULL);
	fname.m_ofn.lpstrDefExt = L"jd";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"数据库(*.mdb)\0*.mdb\0";

	if(fname.DoModal()==IDOK)
	{
		m_DRfilepath = fname.GetPathName();			
	}
	UpdateData(FALSE);
}


void DataTrans::OnBnClickedButtontrans()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!PathFileExists(m_WDfilepath))
		return ;
	FILE* ReadData;		//文件类
	ReadData = _wfopen(m_WDfilepath, L"r");

	double EmptyReadDouble;//空读double
	int EmptyReadint;//空读int
	ACHAR EmptyReadChar[1024];//空读字符
	double tmpdouble;//临时double值
	int tmpint;//临时int值

	//空读纬地txt开头一行的文件类型名称
	fwscanf(ReadData, L"%s", EmptyReadChar);

	//读取起点数据
	int JDnum;//交点数
	fwscanf(ReadData, L"%d", &JDnum);

	BAS_DRAW_FUN::JdCenter *JCArray=new BAS_DRAW_FUN::JdCenter[JDnum];

	fwscanf(ReadData, L"%lf", &JCArray[0].A1);
	JCArray[0].JDXZ=IsJiaoDian;
	JCArray[0].LinkAfter=NotRelation;
	JCArray[0].IsOrNotHuiTou=false;

	for (int i=0;i<3;i++)
	{
		fwscanf(ReadData, L"%d", &EmptyReadint);
	}

	fwscanf(ReadData, L"%lf", &JCArray[0].N);
	fwscanf(ReadData, L"%lf", &JCArray[0].E);

	for (int i=0;i<22;i++)
	{
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
	}

	JCArray[0].R=0.0;
	JCArray[0].A2=0.0;

	//读取交点数据
	for (int i=1; i<JDnum; i++)
	{ 
		//寻找的某顶点
		JCArray[i].JDXZ=IsJiaoDian;
		JCArray[i].LinkAfter=NotRelation;
		JCArray[i].IsOrNotHuiTou=false;

		fwscanf(ReadData, L"%d", &EmptyReadint);
		for (int j=0;j<3;j++)
		{
			Empty3Rows(ReadData);
		}
		for (int j=0;j<3;j++)
		{
			fwscanf(ReadData, L"%d", &EmptyReadint);
		}

		fwscanf(ReadData, L"%lf", &JCArray[i].N);
		fwscanf(ReadData, L"%lf", &JCArray[i].E);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &JCArray[i].A1);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &JCArray[i].R);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		fwscanf(ReadData, L"%lf", &JCArray[i].A2);

		for (int j=0;j<13;j++)
		{
			fwscanf(ReadData, L"%lf", &EmptyReadDouble);
		}
	}

	//创建JD_CENTER实体，计算线路数据
	JD_CENTER *pJC=new JD_CENTER;
	pJC->SetJdCenterArray(JCArray,JDnum);
	pJC->setRoadName("主线");

	int DV;
	CString grade;
	pJC->ReadRoadDV(DV,grade);
	pJC->DV=DV;
	pJC->Grade=grade;

	delete []JCArray;	

	//下面把实体数据写入数据库
	pJC->setMdbName(m_DRfilepath);


	//if(mode!=0)//非主线，则休正起始与终止断链
	//{
	//	if(BLC>-0.1)
	//	{
	//		pJC->setDLM(0,BLC,pJC->DLArray[0].ELC,BGH,pJC->DLArray[0].EGH);
	//		if(pJC->JdCenNum>0)
	//			pJC->JdCenArray[0].A1 = BLC;
	//	}
	//	if(ELC>-0.1)
	//	{
	//		pJC->setDLM(0,pJC->DLArray[pJC->DLNum-1].BLC,ELC,pJC->DLArray[pJC->DLNum-1].BGH,EGH);
	//	}
	//}
	pJC->close();


	//写入数据库
	XLDataBase DBS;
	DBS.NDL=pJC->DLNum;
	DBS.DLArray=new BAS_DRAW_FUN::DLBZ[DBS.NDL];

	int i;
	for( i=0;i<DBS.NDL;i++)
		DBS.DLArray[i]=pJC->DLArray[i];

	DBS.NZDL=pJC->DLNum;
	DBS.ZDLArray=new BAS_DRAW_FUN::DLBZ[DBS.NZDL];
	//	acutPrintf(L"NDL=%d\n ",DBS.NDL);
	for(i=0;i<DBS.NZDL;i++)
		DBS.ZDLArray[i]=pJC->DLArray[i];//纵断面断链表

	DBS.XYNum = pJC->XYNum;
	DBS.XYArray = new double[DBS.XYNum+1][10];
	int j;

	for(i=0;i<=DBS.XYNum;i++)
	{
		for(j=0;j<10;j++)
			DBS.XYArray[i][j]=pJC->XYArray[i][j];
	}
	DBS.XYArray[0][5]=pJC->JD0;
	DBS.NJdCen=pJC->JdCenNum;
	if(DBS.NJdCen>0)
	{
		DBS.JdCenArray=new BAS_DRAW_FUN::JdCenter[DBS.NJdCen];
		for(i=0;i<DBS.NJdCen;i++)
		{
			DBS.JdCenArray[i]=pJC->JdCenArray[i];
		}
		DBS.JdCenArray[0].A2=pJC->JD0;
	}

	CString	m_RoadName=L"主线";
	if(m_DRfilepath != "")
	{
		DBS.Write_XLDbs(m_DRfilepath, L"道路名",m_RoadName);
		if(DBS.NJdCen>0)
			DBS.Write_XLDbs(m_DRfilepath, L"控制点表",m_RoadName);

		DBS.Write_XLDbs(m_DRfilepath, L"断链表",m_RoadName);
		DBS.Write_XLDbs(m_DRfilepath, L"纵断链表",m_RoadName);
		DBS.Write_XLDbs(m_DRfilepath, L"线元表",m_RoadName);
	}
	ACHAR mes[256];
	_stprintf(mes, L"已将数据存入%s数据库,道路名: %s",m_DRfilepath,m_RoadName);
	ads_alert(mes);	
}

void DataTrans::Empty3Rows(FILE *ReadData)
{
	double EmptyReadDouble;//空读double
	int EmptyReadint;//空读int
	ACHAR EmptyReadChar[1024];//空读字符

	fwscanf(ReadData, L"%d", &EmptyReadint);
	for (int i=0;i<5;i++)
	{
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
	}
	fwscanf(ReadData, L"%d", &EmptyReadint);
	for (int i=0;i<12;i++)
	{
		fwscanf(ReadData, L"%lf", &EmptyReadDouble);
	}
}
