// XLDataBase.cpp: implementation of the XLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "raildesign.h"
#include "XLDataBase_JYX.h"
#include "GTZX2_JYX.h"
#include "PmPointer.h"
#include "BAS_DRAW_FUN.h"

//#include "BasicDataOfZDM.h"
//#include "GlobalZDMData_JYX.h"
//extern CBasicDataOfZDM GlobalBasicDataOfZDM;//存基本参数

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

extern ACHAR SysPath[1024];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XLDataBase_JYX::XLDataBase_JYX()
{
	//NDL=0;
	//NQX=0;

	//QXB = NULL;
	//NJD=0;


	//PDB = NULL;
	//NPD=0;

	//DMXB = NULL;
	//NDMX=0;

	//Bri = NULL;
	//BriNum = 0;

	//Tun = NULL;
	//TunNum = 0;

	//TWB = NULL;
	//NTW = 0;

	//QXB2x=NULL;
	//NQX2x=0;

	//DLB2x=NULL;
	//NDL2x=0;

	//HDMB =NULL;
	//NHDM = 0;

	//SBri = NULL;
	//SBriNum = 0;

	//Sta = NULL;
	//StaNum = 0;

	//QXJKB = NULL;
	//NQXJK = 0; 

	//JYBoLiangTable = NULL;  //拨量计算表
	//NBLNum = 0;

	////JYJDDataTable = NULL; //交点数据表
	////NJDDateNum = 0;

	//JYOrigMeaTable = NULL;   //既有曲线测量原始资料表
	//NOrigMeaNum = 0;

	//BZLCTable = NULL;  //标志里程表
	//BZLCNum=0;

	//GJDAllInforTable = NULL;  //改建段汇总表
	//GJDAllInforNum =0;

	//ConnectLineTable = NULL;  //联络线表
	//ConnectLineInforNum =0;

	//SGBXTable = NULL ;
	//SGBXInforNum = 0 ;

	//AllZJDOrCDArray = NULL;
	//AllZJDOrCDNum = 0;

	////改建段分段信息表
	//pEXGJDFDInf = NULL ; 
	//NEXGJDFDInf = 0;

	////用地设计
	//pXZYD=NULL;
	//NXZYD=0;
	//pYDType=NULL;
	//NTypeFD=0;


	////水准点表
	//BM = NULL ;
	//BmNum = 0 ;

	//GDSTRU = NULL;
	//NGDSTRU = 0;

	GDSTRU = NULL;
	pBasicDataOfZDM = NULL;
	Tun = NULL;
	Bri = NULL;
	PDB = NULL;
	DMXB = NULL;
	JiYouLuJianGC = NULL;
	RoadBed = NULL;
	ZMControlPT = NULL;
	QXB = NULL;
	TWB = NULL;
	Sta = NULL;
	SBri = NULL;
	QXB2x = NULL;
	DLB2x = NULL;
	QXJKB = NULL;
	ZuoJiYouXianZhanDiKuan = NULL;
	YouJiYouXianZhanDiKuan = NULL;
	LXB = NULL;
	JYBoLiangTable = NULL;
	JYOrigMeaTable = NULL;
	MulFDTable = NULL;
	BZLCTable = NULL;
	pGjdInf = NULL;
	pEXGJDFDInf = NULL;
	ConnectLineTable = NULL;
	SGBXTable = NULL;
	JBJXTable = NULL;
	GJDAllInforTable = NULL;
	OneGJDJDarray = NULL;
	AllZJDOrCDArray = NULL;
	STATION = NULL;
	BM = NULL;
	DX = NULL;
	SJQXB = NULL;
	BC = NULL;
	pXZYD = NULL;
	pYDType = NULL;

	m_pZMTZArr = NULL;
	m_pXZQHArr = NULL;

	JiYouLuJianGC = NULL;
	NJiYouLuJianGC = 0;

	NGDSTRU = 0;
	NBasicDataOfZDM = 0;
	TunNum = 0;
	BriNum = 0;
	NDL = 0;
	NJD = 0;
	JDarray.RemoveAll();
	NPD = 0;
	NDMX = 0;
	NJiYouLuJianGC = 0;
	NGMX = 0;
	RoadBedNum = 0;
	ZMControlPTNum = 0;
	NQX = 0;
	NTW = 0;
	NHDM = 0;
	StaNum = 0;
	SBriNum = 0;
	NQX2x = 0;
	NDL2x = 0;//断链数
	NQXJK = 0;
	NZuoJiYouXianZhanDiKuan = 0;
	NYouJiYouXianZhanDiKuan = 0;
	NYDLX = 0;//用地类型分段总数
	NBLNum = 0;
	NOrigMeaNum = 0;
	MulFDNum = 0;
	BZLCNum = 0;
	NGjdInf = 0;
	NEXGJDFDInf = 0;
	ConnectLineInforNum = 0;
	SGBXInforNum = 0;
	JBJXInforNum = 0;
	GJDAllInforNum = 0;
	OneGJDNum = 0;
	AllZJDOrCDNum = 0;
	StaTionNum = 0;
	BmNum = 0;
	DXNum = 0;
	NSJQXB = 0;
	NBC = 0;
	NXZYD = 0;//行政用地分段数目
	NTypeFD = 0;//用地类型分段数目


}

XLDataBase_JYX::~XLDataBase_JYX()
{
	////plh07.19a
	//if(Tun)delete []Tun;
	//if(Bri)delete []Bri;
	//if(PDB)delete []PDB;
	//if(DMXB)delete []DMXB;
	////	if(QXB)delete []QXB;
	//if(QXB2x)delete []QXB2x;
	//if(TWB)delete []TWB;
	//if(DLB2x)delete []DLB2x;
	//if(GDSTRU) delete []GDSTRU;

	//if(JYBoLiangTable) 
	//{
	//	JYBoLiangTable = NULL ;
	//	delete []JYBoLiangTable;
	//}

	//if(BM)delete []BM;
	//BmNum = 0;

	//Tun=NULL;
	//TunNum=0;

	//Bri=NULL;
	//BriNum=0;

	//NDL=0;

	//NJD=0;

	//PDB=NULL;
	//NPD=0;

	//DMXB=NULL;//地面线表
	//NDMX=0;

	//QXB = NULL;//曲线表
	//NQX=0;

	//QXB2x=NULL;
	//NQX2x=0;

	//TWB=NULL;//填挖表
	//NTW=0;

	//DLB2x=NULL;//二线断链表
	//NDL2x=0;//断链数

	//JYBoLiangTable = NULL;  //拨量计算表
	//NBLNum = 0;

	//JYOrigMeaTable = NULL;  //既有曲线原始测量资料
	//NOrigMeaNum = 0;

	//MulFDTable = NULL;  //复曲线分段信息表
	//MulFDNum = 0;

	//ConnectLineTable = NULL; //联络线表
	//ConnectLineInforNum = 0;

	//SGBXTable = NULL ; 
	//SGBXInforNum = 0 ;

	//AllZJDOrCDArray = NULL;  //任意点坐标置镜　数组
	//AllZJDOrCDNum = 0;

	//pEXGJDFDInf = NULL ; //改建段分段信息表
	//NEXGJDFDInf = 0;

	if (GDSTRU)delete[]GDSTRU; GDSTRU = NULL;
	if (pBasicDataOfZDM)delete[]pBasicDataOfZDM; pBasicDataOfZDM = NULL;
	if (Tun)delete[]Tun; Tun = NULL;
	if (Bri)delete[]Bri; Bri = NULL;
	if (PDB)delete[]PDB; PDB = NULL;
	if (DMXB)delete[]DMXB; DMXB = NULL;
	if (JiYouLuJianGC)delete[]JiYouLuJianGC; JiYouLuJianGC = NULL;
	if (RoadBed)delete[]RoadBed; RoadBed = NULL;
	if (ZMControlPT)delete[]ZMControlPT; ZMControlPT = NULL;
	if (QXB)delete[]QXB; QXB = NULL;
	if (TWB)delete[]TWB; TWB = NULL;
	if (Sta)delete[]Sta; Sta = NULL;
	if (SBri)delete[]SBri; SBri = NULL;
	if (QXB2x)delete[]QXB2x; QXB2x = NULL;
	if (DLB2x)delete[]DLB2x; DLB2x = NULL;
	if (QXJKB)delete[]QXJKB; QXJKB = NULL;
	if (ZuoJiYouXianZhanDiKuan)delete[]ZuoJiYouXianZhanDiKuan; ZuoJiYouXianZhanDiKuan = NULL;
	if (YouJiYouXianZhanDiKuan)delete[]YouJiYouXianZhanDiKuan; YouJiYouXianZhanDiKuan = NULL;
	if (LXB) delete[]LXB; LXB = NULL;
	if (JYBoLiangTable)delete[]JYBoLiangTable; JYBoLiangTable = NULL;
	if (JYOrigMeaTable)delete[]JYOrigMeaTable; JYOrigMeaTable = NULL;
	if (MulFDTable)delete[]MulFDTable; MulFDTable = NULL;
	if (BZLCTable)delete[]BZLCTable; BZLCTable = NULL;
	if (pGjdInf)delete[]pGjdInf; pGjdInf = NULL;
	if (pEXGJDFDInf)delete[]pEXGJDFDInf; pEXGJDFDInf = NULL;
	if (ConnectLineTable)delete[]ConnectLineTable; ConnectLineTable = NULL;
	if (SGBXTable)delete[]SGBXTable; SGBXTable = NULL;
	if (JBJXTable)delete[]JBJXTable; JBJXTable = NULL;
	if (GJDAllInforTable)delete[]GJDAllInforTable; GJDAllInforTable = NULL;
	if (OneGJDJDarray)delete[]OneGJDJDarray; OneGJDJDarray = NULL;
	if (AllZJDOrCDArray)delete[]AllZJDOrCDArray; AllZJDOrCDArray = NULL;
	if (STATION)delete[]STATION; STATION = NULL;
	if (BM)delete[]BM; BM = NULL;
	if (DX)delete[]DX; DX = NULL;
	if (SJQXB)delete[]SJQXB; SJQXB = NULL;
	if (BC)delete[]BC; BC = NULL;
	if (pXZYD)delete[]pXZYD; pXZYD = NULL;
	if (pYDType)delete[]pYDType; pYDType = NULL;

	NGDSTRU = 0;
	NBasicDataOfZDM = 0;
	TunNum = 0;
	BriNum = 0;
	NDL = 0;
	NJD = 0;
	JDarray.RemoveAll();
	NPD = 0;
	NDMX = 0;
	NJiYouLuJianGC = 0;
	NGMX = 0;
	RoadBedNum = 0;
	ZMControlPTNum = 0;
	NQX = 0;
	NTW = 0;
	NHDM = 0;
	StaNum = 0;
	SBriNum = 0;
	NQX2x = 0;
	NDL2x = 0;//断链数
	NQXJK = 0;
	NZuoJiYouXianZhanDiKuan = 0;
	NYouJiYouXianZhanDiKuan = 0;
	NYDLX = 0;//用地类型分段总数
	NBLNum = 0;
	NOrigMeaNum = 0;
	MulFDNum = 0;
	BZLCNum = 0;
	NGjdInf = 0;
	NEXGJDFDInf = 0;
	ConnectLineInforNum = 0;
	SGBXInforNum = 0;
	JBJXInforNum = 0;
	GJDAllInforNum = 0;
	OneGJDNum = 0;
	AllZJDOrCDNum = 0;
	StaTionNum = 0;
	BmNum = 0;
	DXNum = 0;
	NSJQXB = 0;
	NBC = 0;
	NXZYD = 0;//行政用地分段数目
	NTypeFD = 0;//用地类型分段数目

}

void XLDataBase_JYX::Realease()
{

	if (BZLCTable)
	{
		delete[]BZLCTable;
		BZLCTable = NULL;
	}
	BZLCNum = 0;

	if (JYBoLiangTable)
	{
		delete[]JYBoLiangTable;
		JYBoLiangTable = NULL;
	}
	NBLNum = 0;

	if (QXB)
	{
		delete[]QXB;
		QXB = NULL;
	}
	NQX = 0;

	if (Bri)
	{
		delete[]Bri;
		Bri = NULL;
	}
	BriNum = 0;

	if (SBri)
	{
		delete[]SBri;
		SBri = NULL;
	}
	SBriNum = 0;

	if (BM)
	{
		delete[]BM;
		BM = NULL;
	}
	BmNum = 0;

	if (Tun)
	{
		delete[]Tun;
		Tun = NULL;
	}
	TunNum = 0;

	if (Sta)
	{
		delete[]Sta;
		Sta = NULL;
	}
	StaNum = 0;

	if (DX)
	{
		delete[]DX;
		DX = NULL;
	}
	DXNum = 0;


	JDarray.RemoveAll();
	NJD = 0;

	NDL = 0;

}

int XLDataBase_JYX::Read_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//读取线路数据库中的数据
{
	CString strEXName;
	CString RXDName;
	if (wlcstring == "")
	{
		m_strSQL = L"select * from " + TabFname;
	}
	else if ((TabFname == "大中桥" || TabFname == "小桥涵" || TabFname == "隧道" || TabFname == "车站") && (wlcstring != "线路名=\"既有基线\"" && wlcstring != "线路名=\"贯通设计基线\"" && wlcstring != "线路名=\"贯通设计Ⅱ线\""))
	{
		//得到插旗数据的补充数据（由于二线插旗数据的线路名可能是绕行段名或者二线名（如既有左线））

		int iStart;
		CString strTemp1 = wlcstring;
		strTemp1.Replace(L"线路名=", NULL);
		RXDName = strTemp1.Mid(1, strTemp1.GetLength() - 2);
		strEXName = RXDInWhichXL(mdbFname, RXDName);

		m_strSQL = L"select * from " + TabFname;
		m_strSQL += " where ";
		m_strSQL += wlcstring;
		if (!strEXName.IsEmpty())
		{
			strTemp1 = L"线路名=\"" + strEXName + "\"";
			m_strSQL += " or ";
			m_strSQL += strTemp1;
		}

	}
	else
	{
		//	m_strSQL.Format(L"select * from %s where %s",TabFname,wlcstring);

		m_strSQL = L"select * from " + TabFname;

		m_strSQL += " where ";
		m_strSQL += wlcstring;
	}

	//m_strSQL += " Order By 编号";
	m_strSQL += " Order By ID";
	///////////////////////


	CString strText, GH;
	CAdoRecordSet Report_input;
	AfxOleInit();

	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64



		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			//ads_printf(L"连接%s数据库失败!\n",mdbFname);

			//			AfxMessageBox(L"连接数据库失败!");			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"数据库没有打开或已经关闭!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			//if(!_tcscmp(TabFname,"外移桩导线点表"))
			//{
			//	CString sErr;
			//	sErr.Format(L"不能打开%s，因为数据库%s中不存该表，请您参照系统安装程序lib目录下的标准mdb在数据库中创建该表",TabFname,mdbFname);
			//	AfxMessageBox(sErr);
			//}


			m_adoConnection.Disconnect();
			return 0;

		}

		//////////////////////////////////////////////////////////////////////////
		////小桥涵增加了左右涵口标高两列，如果数据库中没有这两列数据程序自动增加。
		//if (TabFname == "小桥涵" && Report_input.GetFieldName(7) == "交角")//数据库中第八列是否为“交角”判断是否要增加两列
		//{
		//	Report_input.Close();
		//	CString AddField;
		//	AddField.Format(L"ALTER   TABLE   %s   ADD  左涵口标高   DOUBLE ",TabFname);//插入左涵口标高
		//	m_adoConnection.Execute(AddField);
		//	AddField.Format(L"ALTER   TABLE   %s   ADD  右涵口标高   DOUBLE ",TabFname);//插入右涵口标高
		//	m_adoConnection.Execute(AddField);
		//	AddField.Format(L"select 编号, 线路名, 中心里程冠号, 中心里程数, 孔跨式样, 左横向长度, 右横向长度, 左涵口标高, 右涵口标高, 交角, 用途, 插旗方式, 改建性质 from %s",TabFname);//重新定义顺序
		//	m_adoConnection.Execute(AddField);
		//	Report_input.SetAdoConnection(&m_adoConnection);
		//	Report_input.Open(m_strSQL,adCmdText);
		//}

		//	Report_input.Open(m_strSQL);

		//   Report_input.MoveFirst();

		/////////////////////////////////////////////////////////////////////////////
		////既有铁路改建   //  读数据
		/////////////////////////////////////////////////////////////////////////////
		if (TabFname == "交点数据表")
		{  ///有问题

			JDarrayStruct TempJDarray;
			JDarray.RemoveAll();

			int i = 0;

			/*
			NJDDateNum = Report_input.GetRecordCount();

			if(JYJDDataTable) delete []JYJDDataTable;
			if(NJDDateNum>0)
			JYJDDataTable = new JDDataTable[NJDDateNum];
			*/
			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"坐标N");
				TempJDarray.N = _wtof(strText);


				Report_input.GetValueString(strText, L"坐标E");
				TempJDarray.E = _wtof(strText);


				Report_input.GetValueString(strText, L"前缓长");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"半径");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓长");
				TempJDarray.l2 = _wtof(strText);

				JDarray.Add(TempJDarray);

				Report_input.MoveNext();
				i++;
			}
			NJD = i;

			if (NJD > 0)
			{
				JDarray[0].R = JDarray[NJD - 1].R = 0.0;
				JDarray[0].l2 = JDarray[NJD - 1].l2 = 0.0;
			}


		}
		else if (TabFname == L"既有基线交点数据表")
		{
			JDarrayStruct TempJDarray;
			JDarray.RemoveAll();

			NJD = Report_input.GetRowCount();
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())				//从每一行中获取每个交点的各种信息
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"坐标N");
				TempJDarray.N = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				TempJDarray.E = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线长");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线长");
				TempJDarray.l2 = _wtof(strText);

				JDarray.Add(TempJDarray);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"设计基线交点数据表")
		{
			JDarrayStruct TempJDarray;
			JDarray.RemoveAll();

			int i = 0;

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"坐标N");
				TempJDarray.N = _wtof(strText);


				Report_input.GetValueString(strText, L"坐标E");
				TempJDarray.E = _wtof(strText);


				Report_input.GetValueString(strText, L"前缓和曲线长");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线长");
				TempJDarray.l2 = _wtof(strText);

				JDarray.Add(TempJDarray);

				Report_input.MoveNext();
				i++;
			}
			NJD = i;

			if (NJD > 0)
			{
				JDarray[0].R = JDarray[NJD - 1].R = 0.0;
				JDarray[0].l2 = JDarray[NJD - 1].l2 = 0.0;
			}
		}


		else if (TabFname == "车站")
		{
			int i = 0;
			StaNum = Report_input.GetRowCount();
			if (Sta) delete[]Sta;
			Sta = NULL;
			if (StaNum > 0)
				Sta = new STATAB[StaNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//Report_input.GetValueString(strText,5);	
				//if(strText==""||strText=="NULL")//起始里程为空，不记录
				//{
				// Report_input.MoveNext();
				// continue;
				//}
				//Report_input.GetValueString(strText,7);				
				//if(strText==""||strText=="NULL")//终止里程为空，不记录
				//{
				//   Report_input.MoveNext();
				// continue;
				//}
				Report_input.GetValueString(Sta[i].NameOfWay, L"线路名");
				Report_input.GetValueString(Sta[i].StaName, L"车站名称");
				Report_input.GetValueString(Sta[i].CGH, L"站房中心里程冠号");	//中心里程冠号			
				Report_input.GetValueString(strText, L"站房中心里程数");
				Sta[i].CenDml = _wtof(strText);


				Report_input.GetValueString(Sta[i].BGH, L"起点里程冠号");	//起始里程冠号	
				Report_input.GetValueString(strText, L"起点里程数");
				Sta[i].StDml = _wtof(strText);
				//
				Report_input.GetValueString(Sta[i].EGH, L"终点里程冠号");	//终止里程冠号			
				Report_input.GetValueString(strText, L"终点里程数");	//终止里程			
				Sta[i].EdDml = _wtof(strText);

				Report_input.GetValueString(strText, L"车站类型");//车站类型
				Sta[i].CZLX = _wtof(strText);
				Report_input.GetValueString(Sta[i].CQTKM, L"插旗图块名");//插旗图块名
				//				Sta[i].CQTKM=_wtof(strText);
				Report_input.GetValueString(Sta[i].GJXZ, L"改建性质");

				dmltockml(Sta[i].CGH, Sta[i].CenDml, Sta[i].CKml);
				dmltockml(Sta[i].BGH, Sta[i].StDml, Sta[i].SKml);
				dmltockml(Sta[i].EGH, Sta[i].EdDml, Sta[i].EKml);
				i++;
				Report_input.MoveNext();
			}
			StaNum = i;
		}
		else if (TabFname == "既有曲线拨量计算结果表")
		{

			NBLNum = Report_input.GetRecordCount();

			if (JYBoLiangTable) delete[]JYBoLiangTable;

			if (NBLNum > 0)
				JYBoLiangTable = new BoLiangTable[NBLNum];
			else
			{
				Report_input.Close();
				m_adoConnection.Disconnect();
				return 0;
			}
			Report_input.MoveFirst();

			for (int i = 0; i < NBLNum; i++)
			{

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(JYBoLiangTable[i].JDNum, strText);

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(JYBoLiangTable[i].XLName, strText);


				Report_input.GetValueString(strText, L"曲线偏角");

				JYBoLiangTable[i].alfa = FromCStringTodouble(strText);


				Report_input.GetValueString(strText, L"曲线半径");
				JYBoLiangTable[i].R = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"前缓长");
				JYBoLiangTable[i].l1 = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"后缓长");
				JYBoLiangTable[i].l2 = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"ZH里程");
				JYBoLiangTable[i].KZH = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"HY里程");
				JYBoLiangTable[i].KHY = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"YH里程");
				JYBoLiangTable[i].KYH = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"HZ里程");
				JYBoLiangTable[i].KHZ = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"冠号");
				_tcscpy(JYBoLiangTable[i].GH, strText);

				Report_input.GetValueString(strText, L"是否外业实测");
				if (strText == "是")
					JYBoLiangTable[i].IsMeasure = true;
				else
					JYBoLiangTable[i].IsMeasure = false;

				Report_input.MoveNext();
			}

		}
		else if (TabFname == "既有曲线测量原始资料表")
		{
			int i = 0;

			NOrigMeaNum = Report_input.GetRecordCount();
			if (JYOrigMeaTable) delete[]JYOrigMeaTable;

			if (NOrigMeaNum > 0)
				JYOrigMeaTable = new OriginalMeasureTable[NOrigMeaNum];
			Report_input.MoveFirst();

			for (i = 0; i < NOrigMeaNum; i++)
			{
				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(JYOrigMeaTable[i].jdno, strText);

				Report_input.GetValueString(strText, L"置镜点");
				JYOrigMeaTable[i].ZJPoint = _wtoi(strText);

				Report_input.GetValueString(strText, L"测点里程");
				//	JYOrigMeaTable[i].Landmark=_wtof(strText);
				if (strText == "-100000")strText = "";
				_tcscpy(JYOrigMeaTable[i].Landmark, strText);



				Report_input.GetValueString(strText, L"偏角");

				//if (strText.IsEmpty())
				//	JYOrigMeaTable[i].fj = -10000 ;
				//else
				//    JYOrigMeaTable[i].fj=_wtof(strText);	
				JYOrigMeaTable[i].fj = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"置测距");
				JYOrigMeaTable[i].ZhiCeJv = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"右角");
				JYOrigMeaTable[i].Youjiao = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"既有Ej");
				JYOrigMeaTable[i].Ej = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"设计Es");

				JYOrigMeaTable[i].Es = FromCStringTodouble(strText);


				Report_input.GetValueString(strText, L"拨量");

				JYOrigMeaTable[i].deE = FromCStringTodouble(strText);

				// 06.09.20
				Report_input.GetValueString(strText, L"拨量正域值");

				JYOrigMeaTable[i].BLTerritoryPlus = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"拨量负域值");

				JYOrigMeaTable[i].BLTerritoryMinus = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"属性");
				if (strText != "NULL")
					_tcscpy(JYOrigMeaTable[i].ShuXing, strText);
				else
					_tcscpy(JYOrigMeaTable[i].ShuXing, L" ");

				Report_input.GetValueString(strText, L"里程差");

				JYOrigMeaTable[i].LCcha = FromCStringTodouble(strText);


				Report_input.MoveNext();

			}
		}
		else if (TabFname == "断链表")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"等号左里程冠号");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"等号右里程冠号");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"等号左里程数");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"断链长度");
				DLB[i].DL = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				DLB[i].N = _wtof(strText);
				Report_input.GetValueString(strText, L"坐标E");
				DLB[i].E = _wtof(strText);

				//同时赋予断后里程
				DLB[i].ELC = DLB[i].BLC + DLB[i].DL;

				Report_input.MoveNext();
				i++;
			}
			NDL = i;

		}
		else if (TabFname == L"既有基线断链表")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"断前冠号");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"断后冠号");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"断前里程");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"断后里程");
				DLB[i].ELC = _wtof(strText);
				//Report_input.GetValueString(strText, L"断链长度");
				DLB[i].DL = DLB[i].ELC - DLB[i].BLC;

				//Report_input.GetValueString(strText, L"坐标N");
				//DLB[i].N = _wtof(strText);
				//Report_input.GetValueString(strText, L"坐标E");
				//DLB[i].E = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			NDL = i;
		}
		else if (TabFname == L"设计基线断链表")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"断前冠号");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"断后冠号");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"断前里程");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"断后里程");
				DLB[i].ELC = _wtof(strText);
				//Report_input.GetValueString(strText, L"断链长度");
				DLB[i].DL = DLB[i].ELC - DLB[i].BLC;

				//Report_input.GetValueString(strText, L"坐标N");
				//DLB[i].N = _wtof(strText);
				//Report_input.GetValueString(strText, L"坐标E");
				//DLB[i].E = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			NDL = i;
		}

		else if (TabFname == "水准点表")
		{
			BmNum = Report_input.GetRecordCount();
			if (BM) delete[]BM;

			if (BmNum > 0)
				BM = new BMSTRUCT[BmNum];
			Report_input.MoveFirst();


			for (int i = 0; i < BmNum; i++)
			{
				Report_input.GetValueString(strText, L"水准点编号");
				BM[i].BianHao = strText;
				Report_input.GetValueString(strText, L"高程");
				BM[i].GaoCheng = _wtof(strText);

				Report_input.GetValueString(strText, L"里程冠号");
				BM[i].GH = strText;

				Report_input.GetValueString(strText, L"里程数");
				BM[i].XLC = _wtof(strText);


				Report_input.GetValueString(strText, L"距中线距离");
				BM[i].DistZX = _wtof(strText);


				Report_input.GetValueString(strText, L"坐标N");
				BM[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				BM[i].E = _wtof(strText);


				Report_input.GetValueString(strText, L"位置材料的描述");
				BM[i].CLMS = strText;

				Report_input.GetValueString(strText, L"附注");
				BM[i].FZ = strText;

				Report_input.MoveNext();

			}

		}


		else if (TabFname == "导线表")
		{
			DXNum = Report_input.GetRecordCount();
			if (DX) delete[]DX;

			if (DXNum > 0)
				DX = new DXSTRUCT[DXNum];
			Report_input.MoveFirst();


			for (int i = 0; i < DXNum; i++)
			{
				Report_input.GetValueString(strText, L"桩号");
				DX[i].ZhuangHao = strText;

				Report_input.GetValueString(strText, L"右角");
				DX[i].RightAng = _wtof(strText);

				Report_input.GetValueString(strText, L"方位角");
				DX[i].FWJ = _wtof(strText);

				Report_input.GetValueString(strText, L"距离");
				DX[i].Dist = _wtof(strText);

				Report_input.GetValueString(strText, L"纬距");
				DX[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"经距");
				DX[i].E = _wtof(strText);

				Report_input.GetValueString(strText, L"高程");
				DX[i].GaoCheng = _wtof(strText);

				Report_input.GetValueString(strText, L"里程");
				DX[i].cml = _wtof(strText);

				Report_input.MoveNext();

			}

		}

		/*
		///////////////////////
		else if(TabFname=="Ⅱ线改建分段表")
		{
		NEXGJDFDInf = Report_input.GetRowCount();
		if(pEXGJDFDInf) delete pEXGJDFDInf;
		if(NEXGJDFDInf>0)
		pEXGJDFDInf = new EXGJDFDInf[NEXGJDFDInf];
		Report_input.MoveFirst();
		int i=0;
		while(!Report_input.IsEOF())
		{
		Report_input.GetValueString(strText,L"起点N");
		pEXGJDFDInf[i].spt[0] = _wtof(strText);
		Report_input.GetValueString(strText,L"起点E");
		pEXGJDFDInf[i].spt[1] = _wtof(strText);

		Report_input.GetValueString(strText,L"终点N");
		pEXGJDFDInf[i].ept[0] = _wtof(strText);
		Report_input.GetValueString(strText,L"终点E");
		pEXGJDFDInf[i].ept[1] = _wtof(strText);
		Report_input.GetValueString(strText,L"既有线路名");
		_tcscpy(pEXGJDFDInf[i].XLName , strText);
		i++;
		Report_input.MoveNext();
		}
		}
		*/
		else if (TabFname == "二线曲线表")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(QXB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"交点属性");
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"二线交点号");
				//	QXB2x[i].jdno=int(_wtof(strText)+0.001);
				_tcscpy(QXB2x[i].JDNum, strText);
				QXB2x[i].jdno = i;

				//	ads_printf(L"S1  QXB2x[i].JDNum==%s\n ",QXB2x[i].JDNum);

				Report_input.GetValueString(strText, L"一线交点号");
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"线间距");
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线长");
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线长");
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"纵面设计二线交点数据表")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(QXB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"交点属性");
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"交点号");
				//	QXB2x[i].jdno=int(_wtof(strText)+0.001);
				_tcscpy(QXB2x[i].JDNum, strText);
				QXB2x[i].jdno = i;

				//	ads_printf(L"S1  QXB2x[i].JDNum==%s\n ",QXB2x[i].JDNum);

				Report_input.GetValueString(strText, L"一线交点号");
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"线间距");
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线长");
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线长");
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "二线断链表")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(DLB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"绕行段名");
				DLB2x[i].RXDName = strText;

				Report_input.GetValueString(strText, L"线路编号");
				DLB2x[i].no = int(_wtof(strText)); // +0.001

				Report_input.GetValueString(DLB2x[i].BGH, L"等号左里程冠号");

				Report_input.GetValueString(strText, L"等号左里程数");
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, L"等号右里程冠号");

				Report_input.GetValueString(strText, L"等号右里程数");
				DLB2x[i].ELC = _wtof(strText);


				//ads_printf(L"内　XLName=%s,RXDName=%s,no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n",DLB2x[i].XLName,
				//	DLB2x[i].RXDName,DLB2x[i].no,DLB2x[i].BGH,
				//	DLB2x[i].BLC,DLB2x[i].EGH,DLB2x[i].ELC);


				i++;
				Report_input.MoveNext();
			}

		}
		else if (TabFname == L"纵面设计二线断链表")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(DLB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"绕行段名");
				DLB2x[i].RXDName = strText;

				Report_input.GetValueString(strText, L"编号");
				DLB2x[i].no = int(_wtof(strText)); // +0.001

				Report_input.GetValueString(DLB2x[i].BGH, L"断前冠号");

				Report_input.GetValueString(strText, L"断前里程");
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, L"断后冠号");

				Report_input.GetValueString(strText, L"断后里程");
				DLB2x[i].ELC = _wtof(strText);


				//ads_printf(L"内　XLName=%s,RXDName=%s,no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n",DLB2x[i].XLName,
				//	DLB2x[i].RXDName,DLB2x[i].no,DLB2x[i].BGH,
				//	DLB2x[i].BLC,DLB2x[i].EGH,DLB2x[i].ELC);


				i++;
				Report_input.MoveNext();
			}
		}

		////////////////////////////////
		else if (TabFname == "标志里程表")
		{
			int i = 0;
			BZLCNum = Report_input.GetRecordCount();

			if (BZLCTable) delete[]BZLCTable;

			if (BZLCNum > 0)
				BZLCTable = new BZLCStructTable[BZLCNum];
			Report_input.MoveFirst();

			for (i = 0; i < BZLCNum; i++)
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(BZLCTable[i].GJDNum, strText);

				Report_input.GetValueString(strText, L"标志里程");

				_tcscpy(BZLCTable[i].kml, strText);

				Report_input.GetValueString(strText, L"X坐标");
				BZLCTable[i].X = _wtof(strText);

				Report_input.GetValueString(strText, L"Y坐标");
				BZLCTable[i].Y = _wtof(strText);


				Report_input.MoveNext();

			}
		}
		else if (TabFname == L"既有基线标志里程表")
		{
			int i = 0;
			BZLCNum = Report_input.GetRecordCount();

			if (BZLCTable) delete[]BZLCTable;

			if (BZLCNum > 0)
				BZLCTable = new BZLCStructTable[BZLCNum];
			Report_input.MoveFirst();

			for (i = 0; i < BZLCNum; i++)
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(BZLCTable[i].GJDNum, strText);

				Report_input.GetValueString(strText, L"标志里程");

				_tcscpy(BZLCTable[i].kml, strText);

				Report_input.GetValueString(strText, L"坐标N");
				BZLCTable[i].X = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				BZLCTable[i].Y = _wtof(strText);

				Report_input.MoveNext();

			}
		}

		/////////////////////////////////
		else if (TabFname == "复曲线分段表")
		{
			int i = 0;

			MulFDNum = Report_input.GetRecordCount();
			if (MulFDTable) delete[]MulFDTable;

			if (MulFDNum > 0)
				MulFDTable = new BAS_DRAW_FUN_JYX::MulFDData[MulFDNum];

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(MulFDTable[i].jdno, strText);

				Report_input.GetValueString(strText, L"HY里程");
				MulFDTable[i].KHY = _wtof(strText);

				Report_input.GetValueString(strText, L"YH里程");
				MulFDTable[i].KYH = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			MulFDNum = i;
		}
		if (TabFname == "改建段汇总表")
		{
			int i = 0;
			GJDAllInforNum = Report_input.GetRecordCount();

			if (GJDAllInforTable) delete[]GJDAllInforTable;

			if (GJDAllInforNum > 0)
				GJDAllInforTable = new GJDAllInforStruct[GJDAllInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < GJDAllInforNum; i++)
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(GJDAllInforTable[i].ToOutSGJJDNum, strText);

				Report_input.GetValueString(strText, L"改建名");
				_tcscpy(GJDAllInforTable[i].ToInSGJJDNum, strText);


				Report_input.GetValueString(strText, L"起点N坐标");
				GJDAllInforTable[i].BpointToN = _wtof(strText);

				Report_input.GetValueString(strText, L"起点E坐标");
				GJDAllInforTable[i].BpointToE = _wtof(strText);

				Report_input.GetValueString(strText, L"终点N坐标");
				GJDAllInforTable[i].EpointToN = _wtof(strText);

				Report_input.GetValueString(strText, L"终点E坐标");
				GJDAllInforTable[i].EpointToE = _wtof(strText);

				Report_input.GetValueString(strText, L"起始线路名");
				_tcscpy(GJDAllInforTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"终止线路名");
				_tcscpy(GJDAllInforTable[i].EXLName, strText);

				Report_input.GetValueString(strText, L"改建性质");
				_tcscpy(GJDAllInforTable[i].GJXZ, strText);

				if (strText == "并行改建")
					GJDAllInforTable[i].GJDType = 0;
				else
					GJDAllInforTable[i].GJDType = 2;

				Report_input.GetValueString(strText, L"起点里程");
				_tcscpy(GJDAllInforTable[i].GJDBLC, strText);

				Report_input.GetValueString(strText, L"终点里程");
				_tcscpy(GJDAllInforTable[i].GJDELC, strText);

				//if(strText == "并行改建") GJDAllInforTable[i].GJDType = 0;
				//else if( strText == "绕行改建") GJDAllInforTable[i].GJDType = 2;
				//else if( strText == "绕行改建") GJDAllInforTable[i].GJDType = 2;

				Report_input.MoveNext();
			}
		}
		if (TabFname == "联接信息表")
		{
			int i = 0;
			ConnectLineInforNum = Report_input.GetRecordCount();

			if (ConnectLineTable) delete[]ConnectLineTable;

			if (ConnectLineInforNum > 0)
				ConnectLineTable = new ConnectLineTableStruct[ConnectLineInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < ConnectLineInforNum; i++)
			{

				Report_input.GetValueString(strText, L"联接线线路名");
				_tcscpy(ConnectLineTable[i].ToOutSGJJDNum, strText);

				Report_input.GetValueString(strText, L"内部联接线线路名");
				_tcscpy(ConnectLineTable[i].ToInSGJJDNum, strText);


				Report_input.GetValueString(strText, L"起始线路名");
				_tcscpy(ConnectLineTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"终止线路名");
				_tcscpy(ConnectLineTable[i].EXLName, strText);

				Report_input.MoveNext();
			}
		}

		if (TabFname == "施工便线信息表")
		{
			int i = 0;
			SGBXInforNum = Report_input.GetRecordCount();

			if (SGBXTable) delete[]SGBXTable;

			if (SGBXInforNum > 0)
				SGBXTable = new SGBXTableStruct[SGBXInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < SGBXInforNum; i++)
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(SGBXTable[i].SGBXName, strText);


				Report_input.GetValueString(strText, L"内部线路名");
				_tcscpy(SGBXTable[i].ToInSGBXName, strText);


				Report_input.GetValueString(strText, L"起始线路名");
				_tcscpy(SGBXTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"终止线路名");
				_tcscpy(SGBXTable[i].EXLName, strText);

				Report_input.MoveNext();
			}
		}
		if (TabFname == "局部基线信息表")
		{
			int i = 0;
			JBJXInforNum = Report_input.GetRecordCount();

			if (JBJXTable) delete[]JBJXTable;

			if (JBJXInforNum > 0)
				JBJXTable = new JBJXTableStruct[JBJXInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < JBJXInforNum; i++)
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(JBJXTable[i].JBJXName, strText);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "任意点置镜拨量计算测量表")
		{   //

			int i = 0;
			AllZJDOrCDNum = Report_input.GetRecordCount();

			if (AllZJDOrCDArray) delete[]AllZJDOrCDArray;

			if (AllZJDOrCDNum > 0)
				AllZJDOrCDArray = new SZJDOrCD[AllZJDOrCDNum];
			Report_input.MoveFirst();

			for (i = 0; i < AllZJDOrCDNum; i++)
			{

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(AllZJDOrCDArray[i].JDNum, strText);

				Report_input.GetValueString(strText, L"置镜点");
				AllZJDOrCDArray[i].iZJDOrCD = _wtoi(strText);

				Report_input.GetValueString(strText, L"测点里程");
				AllZJDOrCDArray[i].dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"置测距");
				AllZJDOrCDArray[i].dLZJDtoCD = _wtof(strText);

				Report_input.GetValueString(strText, L"右角");
				AllZJDOrCDArray[i].dYoujiao = _wtof(strText);

				Report_input.MoveNext();

			}

		}

		else if (TabFname == "外移桩导线点表")
		{   //

			int i = 0;
			m_iNWYZ = Report_input.GetRecordCount();

			if (m_pWYZArr) delete[]m_pWYZArr;

			if (m_iNWYZ > 0)
				m_pWYZArr = new WaiYiZhuang[m_iNWYZ];
			Report_input.MoveFirst();

			for (i = 0; i < m_iNWYZ; i++)
			{

				Report_input.GetValueString(strText, L"冠号");
				_tcscpy(m_pWYZArr[i].m_cGH, strText);

				Report_input.GetValueString(strText, L"里程");
				m_pWYZArr[i].m_dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				m_pWYZArr[i].m_dN = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				m_pWYZArr[i].m_dE = _wtof(strText);

				Report_input.GetValueString(strText, L"外移距");
				m_pWYZArr[i].m_dOffset = _wtof(strText);

				Report_input.GetValueString(strText, L"属性");
				if (strText == "直线始")
					m_pWYZArr[i].m_iZXOrQX = -1;
				else if (strText == "直线止")
					m_pWYZArr[i].m_iZXOrQX = 1;
				else if (strText == "直线中")
					m_pWYZArr[i].m_iZXOrQX = 0;
				else if (strText == "直线点")
					m_pWYZArr[i].m_iZXOrQX = 3;
				else if (strText == "曲线点")
					m_pWYZArr[i].m_iZXOrQX = 2;
				else
					m_pWYZArr[i].m_iZXOrQX = 4;

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(m_pWYZArr[i].XLName, strText);



				Report_input.MoveNext();

			}

		}


		else if (TabFname == "纵面台帐表")
		{

			int i = 0;
			m_iNZMTZ = Report_input.GetRecordCount();

			if (m_pZMTZArr)
			{
				delete[]m_pZMTZArr;
				m_pZMTZArr = NULL;
			}

			if (m_iNZMTZ > 0)
				m_pZMTZArr = new ZMTaiZhang[m_iNZMTZ];
			Report_input.MoveFirst();

			for (i = 0; i < m_iNZMTZ; i++)
			{

				Report_input.GetValueString(strText, L"冠号");
				_tcscpy(m_pZMTZArr[i].m_cGH, strText);

				Report_input.GetValueString(strText, L"里程");
				m_pZMTZArr[i].m_dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(m_pZMTZArr[i].XLName, strText);

				Report_input.MoveNext();

			}

		}

		else if (TabFname == "行政区划表")
		{

			int i = 0;
			m_iXZQH = Report_input.GetRecordCount();

			if (m_pXZQHArr)
			{
				delete[]m_pXZQHArr;
				m_pXZQHArr = NULL;
			}

			if (m_iXZQH > 0)
				m_pXZQHArr = new XingZhengQuHua[m_iXZQH];
			Report_input.MoveFirst();

			for (i = 0; i < m_iXZQH; i++)
			{

				Report_input.GetValueString(strText, L"起始里程");
				m_pXZQHArr[i].m_sSml = strText;

				Report_input.GetValueString(strText, L"终止里程");
				m_pXZQHArr[i].m_sEml = strText;

				Report_input.GetValueString(strText, L"名称");
				m_pXZQHArr[i].m_sName = strText;


				Report_input.MoveNext();

			}

		}





		//////////////////////////////////////////////////////
		//------------------------纵面----------------------//
		else if (TabFname == "设计曲线结果表")
		{

			NSJQXB = Report_input.GetRecordCount();

			if (SJQXB) delete[]SJQXB;

			if (NSJQXB > 0)
				SJQXB = new QXBStruct[NSJQXB];
			else
			{
				Report_input.Close();
				m_adoConnection.Disconnect();
				return 0;
			}
			Report_input.MoveFirst();

			for (int i = 0; i < NSJQXB; i++)
			{

				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(SJQXB[i].XLName, strText);

				Report_input.GetValueString(strText, L"交点号");
				_tcscpy(SJQXB[i].JDNum, strText);

				Report_input.GetValueString(strText, L"曲线偏角");
				SJQXB[i].a = fabs(_wtof(strText));

				Report_input.GetValueString(strText, L"曲线半径");
				SJQXB[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓长");
				SJQXB[i].l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓长");
				SJQXB[i].l2 = _wtof(strText);

				Report_input.GetValueString(strText, L"CKZH里程");
				_tcscpy(SJQXB[i].CKZH, strText);

				Report_input.GetValueString(strText, L"CKHY里程");
				_tcscpy(SJQXB[i].CKHY, strText);

				Report_input.GetValueString(strText, L"CKYH里程");
				_tcscpy(SJQXB[i].CKYH, strText);

				Report_input.GetValueString(strText, L"CKHZ里程");
				_tcscpy(SJQXB[i].CKHZ, strText);

				//ads_printf(L"i=%d,HY=%s\n",i,SJQXB[i].CKHY);


				Report_input.MoveNext();
			}
		}


		else if (TabFname == "地面线")
		{
			int i = 0;
			NDMX = Report_input.GetRecordCount();
			if (DMXB) delete[]DMXB;
			if (NDMX > 0)
				DMXB = new DMXTAB[NDMX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(DMXB[i].GH, L"中桩里程冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				DMXB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"地面高程");
				DMXB[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NDMX = i;
		}

		else if (TabFname == "既有路肩高程表")
		{
			int i = 0;
			NJiYouLuJianGC = Report_input.GetRecordCount();
			if (JiYouLuJianGC) delete[]JiYouLuJianGC;
			if (NJiYouLuJianGC > 0)
				JiYouLuJianGC = new DMXTAB[NJiYouLuJianGC];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(JiYouLuJianGC[i].GH, "中桩冠号");
				Report_input.GetValueString(strText, L"中桩里程");
				JiYouLuJianGC[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"既有路肩高程");
				JiYouLuJianGC[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NJiYouLuJianGC = i;
		}

		else if (TabFname == "坡度表")
		{
			int i = 0;
			NPD = Report_input.GetRecordCount();
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].NameOfWay, L"线路名");
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _tstof(strText);
				Report_input.GetValueString(strText, L"坡度");
				PDB[i].degree = _tstof(strText);
				Report_input.GetValueString(strText, L"坡长");
				PDB[i].length = _tstof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _tstof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _tstof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"附注");

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"基线坡度表" || TabFname == L"辅助纵断面坡度表")
		{
			int i = 0;
			NPD = Report_input.GetRecordCount();
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].NameOfWay, L"线路名");
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _tstof(strText);
				Report_input.GetValueString(strText, L"坡度");
				PDB[i].degree = _tstof(strText);
				Report_input.GetValueString(strText, L"坡长");
				PDB[i].length = _tstof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _tstof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _tstof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"附注");

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"辅助纵断面段落表")
		{
			int i = 0;
			FZZDMNum = Report_input.GetRecordCount();

			if (FZZDMArray) delete[]FZZDMArray;

			if (FZZDMNum > 0)
				FZZDMArray = new FZZDM[FZZDMNum];
			Report_input.MoveFirst();

			for (i = 0; i < FZZDMNum; i++)
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(FZZDMArray[i].XLName, strText);

				Report_input.GetValueString(strText, L"段落名");
				_tcscpy(FZZDMArray[i].DLName, strText);

				Report_input.GetValueString(strText, L"段落类型");
				FZZDMArray[i].Type = strText;

				Report_input.GetValueString(strText, L"起点N坐标");
				FZZDMArray[i].spt[0] = _wtof(strText);

				Report_input.GetValueString(strText, L"起点E坐标");
				FZZDMArray[i].spt[1] = _wtof(strText);

				Report_input.GetValueString(strText, L"终点N坐标");
				FZZDMArray[i].ept[0] = _wtof(strText);

				Report_input.GetValueString(strText, L"终点E坐标");
				FZZDMArray[i].ept[1] = _wtof(strText);

				Report_input.GetValueString(strText, L"起点里程");
				_tcscpy(FZZDMArray[i].StCKml, strText);

				Report_input.GetValueString(strText, L"终点里程");
				_tcscpy(FZZDMArray[i].EdCKml, strText);

				Report_input.GetValueString(strText, L"依赖平面");
				_tcscpy(FZZDMArray[i].BLName, strText);

				Report_input.MoveNext();
			}
		}


		else if (TabFname == "既有轨面线")
		{
			int i = 0;
			NGMX = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NGMX > 0)
				GMXB = new GMXTAB[NGMX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(GMXB[i].CAP, L"中桩里程冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				GMXB[i].ML = _tstof(strText);
				dmltockml(GMXB[i].CAP, GMXB[i].ML, GMXB[i].CKml);
				Report_input.GetValueString(strText, L"既有轨面高程");
				GMXB[i].H = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NGMX = i;
		}
		else if (TabFname == "既有道床厚度表")
		{
			int i = 0;
			RoadBedNum = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (RoadBedNum > 0)
				RoadBed = new RoadBedTAB[RoadBedNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(RoadBed[i].GH, L"中桩冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				RoadBed[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"道床厚度");
				RoadBed[i].H = _tstof(strText);
				Report_input.GetValueString(strText, L"轨道结构高度");
				RoadBed[i].H1 = _tstof(strText);
				dmltockml(RoadBed[i].GH, RoadBed[i].ml, RoadBed[i].CKml);
				Report_input.MoveNext();
				i++;
			}
			RoadBedNum = i;
		}
		else if (TabFname == "纵面控制点表")
		{
			int i = 0;
			ZMControlPTNum = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (ZMControlPTNum > 0)
				ZMControlPT = new RoadBedTAB[ZMControlPTNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(ZMControlPT[i].GH, L"中桩冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				ZMControlPT[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"上限高程");
				ZMControlPT[i].H = _tstof(strText);
				Report_input.GetValueString(strText, L"下限高程");
				ZMControlPT[i].H1 = _tstof(strText);
				Report_input.GetValueString(ZMControlPT[i].sRemark, L"备注");

				Report_input.MoveNext();
				i++;
			}
			ZMControlPTNum = i;
		}
		else if (TabFname == "左既有线占地宽")
		{
			int i = 0;
			NZuoJiYouXianZhanDiKuan = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NZuoJiYouXianZhanDiKuan > 0)
				ZuoJiYouXianZhanDiKuan = new JiYouXianZhanDiKuan[NZuoJiYouXianZhanDiKuan];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(ZuoJiYouXianZhanDiKuan[i].GH, L"中桩冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				ZuoJiYouXianZhanDiKuan[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"路基边界宽度");
				ZuoJiYouXianZhanDiKuan[i].LuJiBianJieKuan = _tstof(strText);
				Report_input.GetValueString(strText, L"占地边界宽度");
				ZuoJiYouXianZhanDiKuan[i].ZhanDiKuan = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == "右既有线占地宽")
		{
			int i = 0;
			NYouJiYouXianZhanDiKuan = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NYouJiYouXianZhanDiKuan > 0)
				YouJiYouXianZhanDiKuan = new JiYouXianZhanDiKuan[NYouJiYouXianZhanDiKuan];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(YouJiYouXianZhanDiKuan[i].GH, L"中桩冠号");
				Report_input.GetValueString(strText, L"中桩里程数");
				YouJiYouXianZhanDiKuan[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"路基边界宽度");
				YouJiYouXianZhanDiKuan[i].LuJiBianJieKuan = _tstof(strText);
				Report_input.GetValueString(strText, L"占地边界宽度");
				YouJiYouXianZhanDiKuan[i].ZhanDiKuan = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == "用地类型表")
		{
			int i = 0;
			NYDLX = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NYDLX > 0)
				LXB = new YongDiLXB[NYDLX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"线路名");
				_tcscpy(LXB[i].XLName, strText);
				Report_input.GetValueString(strText, L"分段终里程冠号");
				_tcscpy(LXB[i].GH, strText);
				Report_input.GetValueString(strText, L"分段终里程数");
				LXB[i].dml = _tstof(strText);
				Report_input.GetValueString(strText, L"用地类型");
				_tcscpy(LXB[i].YDLX, strText);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == "隧道")
		{
			int i = 0;
			TunNum = Report_input.GetRecordCount();
			//			if(Tun) delete []Tun;
			if (TunNum > 0)
				Tun = new TUNTAB[TunNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Tun[i].NameOfWay, L"线路名");

				Report_input.GetValueString(Tun[i].TUNName, L"隧道名称");
				Report_input.GetValueString(Tun[i].BGH, L"起点里程冠号");
				Report_input.GetValueString(strText, L"起点里程数");
				Tun[i].StDml = _tstof(strText);
				Report_input.GetValueString(Tun[i].EGH, L"终点里程冠号");
				Report_input.GetValueString(strText, L"终点里程数");
				Tun[i].EdDml = _tstof(strText);
				Report_input.GetValueString(Tun[i].flag, L"插旗方式");//插旗方式，李伟添加
				if (Tun[i].flag == " " || Tun[i].flag == "NULL")//如果没记录默认为1-一般隧道
				{
					Tun[i].flag.Format(L"%d", 1);
				}
				Report_input.GetValueString(Tun[i].GJZT, L"改建状态");//改建状态

				Report_input.GetValueString(strText, L"附注");//附注，里面记录线数(1-单线2-双线)
				Tun[i].NumOfLine = _wtoi(strText);
				if (Tun[i].NumOfLine > 2)
				{
					Tun[i].NumOfLine = 1;
				}

				dmltockml(Tun[i].BGH, Tun[i].StDml, Tun[i].SKml);
				dmltockml(Tun[i].EGH, Tun[i].EdDml, Tun[i].EKml);
				i++;
				Report_input.MoveNext();
			}
		}

		else if (TabFname == "小桥涵" || TabFname == "左线绕行段小桥涵" || TabFname == "右线绕行段小桥涵")
		{
			int i = 0;
			SBriNum = Report_input.GetRecordCount();
			if (SBri) { delete[]SBri; SBri = NULL; }
			if (SBriNum > 0)
				SBri = new SMALLBRIDGETAB[SBriNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(SBri[i].NameOfWay, L"线路名");
				Report_input.GetValueString(SBri[i].GH, L"中心里程冠号");
				Report_input.GetValueString(strText, L"中心里程数");
				SBri[i].ml = _wtof(strText);

				dmltockml(SBri[i].GH, SBri[i].ml, SBri[i].CKml);

				Report_input.GetValueString(SBri[i].KKStyle, L"孔跨式样");
				Report_input.GetValueString(strText, L"左横向长度");
				SBri[i].dwL = _wtof(strText);
				Report_input.GetValueString(strText, L"右横向长度");
				SBri[i].dwR = _wtof(strText);
				Report_input.GetValueString(strText, L"左涵口标高");
				SBri[i].LLevel = _wtof(strText);
				Report_input.GetValueString(strText, L"右涵口标高");
				SBri[i].RLevel = _wtof(strText);
				Report_input.GetValueString(strText, L"交角");
				SBri[i].JJiao = _wtof(strText);
				if (SBri[i].JJiao < 0.1)//如果没输入，就认为是垂直
				{
					SBri[i].JJiao = 90;
				}
				Report_input.GetValueString(strText, L"用途");
				SBri[i].ToUse = _wtof(strText);
				Report_input.GetValueString(strText, L"插旗方式");
				SBri[i].InsertStyle = _wtof(strText);
				if (SBri[i].InsertStyle < 0 || SBri[i].InsertStyle>7)//不能越界
				{
					SBri[i].InsertStyle = 0;
				}

				Report_input.GetValueString(SBri[i].GJXZ, L"改建性质");
				if (Report_input.GetValueString(strText, L"线数"))
					SBri[i].LineNum = _wtoi(strText);
				else
					SBri[i].LineNum = 1;
				if (TabFname == "左线绕行段小桥涵" || TabFname == "右线绕行段小桥涵")
				{
					Report_input.GetValueString(strText, L"绕行段编号");//???????????				
					SBri[i].iRXD = (int)(_wtof(strText) + 0.001);
				}

				i++;
				Report_input.MoveNext();
			}
		}


		else if (TabFname == "大中桥")
		{
			int i = 0;
			BriNum = Report_input.GetRecordCount();
			if (BriNum > 0)
			{
				Bri = new BRITAB[BriNum];
			}
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Bri[i].NameOfWay, L"线路名");
				Report_input.GetValueString(Bri[i].BriName, L"桥梁名称");
				Report_input.GetValueString(Bri[i].CGH, L"中心里程冠号");	//中心里程冠号			
				Report_input.GetValueString(strText, L"中心里程数");
				Bri[i].CenDml = _tstof(strText);//中心里程
				Report_input.GetValueString(Bri[i].BGH, L"起点里程冠号");	//起始里程冠号			
				Report_input.GetValueString(strText, L"起点里程数");
				Bri[i].StDml = _tstof(strText);//起点里程
				Report_input.GetValueString(Bri[i].EGH, L"终点里程冠号");	//终止里程冠号			
				Report_input.GetValueString(strText, L"终点里程数");
				Bri[i].EdDml = _tstof(strText);//终止里程
				Report_input.GetValueString(Bri[i].KKSY, L"孔跨式样");//孔跨样式
				Report_input.GetValueString(strText, L"线数");
				Bri[i].LineNum = _tstoi(strText);//线数 
				Report_input.GetValueString(Bri[i].SWHt, L"100年水位高程");//百年水位
				Report_input.GetValueString(Bri[i].GJZT, L"改建状态");//改建状态
				Report_input.GetValueString(strText, L"桥全长");//改建状态
				Bri[i].BriLen = _tstof(strText);
				int pos = Bri[i].SWHt.Find(_T("."));
				if (pos > 0)
					Bri[i].SWHt = Bri[i].SWHt.Left(pos + 3);

				dmltockml(Bri[i].CGH, Bri[i].CenDml, Bri[i].CKml);
				dmltockml(Bri[i].BGH, Bri[i].StDml, Bri[i].SKml);
				dmltockml(Bri[i].EGH, Bri[i].EdDml, Bri[i].EKml);

				i++;
				Report_input.MoveNext();
			}
		}

		else if (TabFname == "曲线加宽")
		{
			int i = 0;
			NQXJK = Report_input.GetRowCount();
			if (QXJKB)
				delete[]QXJKB;
			QXJKB = NULL;
			if (NQXJK > 0)
				QXJKB = new QXJKTAB[NQXJK];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"r1");
				QXJKB[i].Rmin = _wtof(strText);
				Report_input.GetValueString(strText, L"r2");
				QXJKB[i].Rmax = _wtof(strText);
				Report_input.GetValueString(strText, L"jk");
				QXJKB[i].JK = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}
			NQXJK = i;
		}

		//////////////////////////////////////////////////////////
		//横断面
		else if (TabFname == "填挖表")
		{
			int i = 0;
			NTW = Report_input.GetRowCount();
			if (TWB) delete[]TWB;
			TWB = NULL;
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(TWB[i].NameOfWay, L"线路名");
				Report_input.GetValueString(TWB[i].GH, L"加桩里程冠号");
				Report_input.GetValueString(strText, L"加桩里程数");
				TWB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"地面高程");
				TWB[i].DMGC = _wtof(strText);
				Report_input.GetValueString(strText, L"设计坡度");
				TWB[i].SJPD = _wtof(strText);
				Report_input.GetValueString(strText, L"路肩设计高程");
				TWB[i].LJSGG = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线纵距");
				TWB[i].SQXZJ = _wtof(strText);
				Report_input.GetValueString(strText, L"中心抬降值");
				TWB[i].ZXTJ = _wtof(strText);
				Report_input.GetValueString(strText, L"中心填挖高度");
				TWB[i].ZXTWG = _wtof(strText);
				Report_input.GetValueString(strText, L"路肩施工高程");
				TWB[i].LJSGG = _wtof(strText);
				Report_input.GetValueString(strText, L"曲线加宽");
				TWB[i].ZQXJK = _wtof(strText);
				Report_input.GetValueString(strText, L"右曲线加宽");
				TWB[i].YQXJK = _wtof(strText);
				Report_input.GetValueString(strText, L"路肩施工宽度");
				TWB[i].LJSGK = _wtof(strText);
				Report_input.GetValueString(strText, L"左线间距");
				TWB[i].ZXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"右线间距");
				TWB[i].YXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"路肩分离限宽");
				TWB[i].LJFLK = _wtof(strText);
				Report_input.GetValueString(strText, L"断面信息");
				TWB[i].DMinf = _wtoi(strText);
				Report_input.GetValueString(strText, L"贯通基线线间距");
				TWB[i].GTJXXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"贯通二线线间距");
				TWB[i].GTEXXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"既有轨面高程");
				TWB[i].JYGMGC = _wtof(strText);
				Report_input.GetValueString(strText, L"设计轨面高程");
				TWB[i].SJGMGC = _wtof(strText);

				dmltockml(TWB[i].GH, TWB[i].ml, TWB[i].CKml);
				Report_input.MoveNext();
				i++;
			}
			NTW = i;
		}

		else if (TabFname == "轨道结构")
		{
			int i = 0;
			NGDSTRU = Report_input.GetRowCount();
			if (GDSTRU) delete[]GDSTRU;
			if (NGDSTRU > 0)
				GDSTRU = new RAILSTU[NGDSTRU];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(GDSTRU[i].XLName, L"线路名");
				Report_input.GetValueString(GDSTRU[i].FdKml, L"分段终里程");
				Report_input.GetValueString(GDSTRU[i].StruType, L"结构类型");
				Report_input.GetValueString(strText, L"结构高度");
				GDSTRU[i].StruHeight = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}
			NGDSTRU = i;
		}

		else if (TabFname == "标尺")
		{
			int i = 0;
			NBC = Report_input.GetRecordCount();
			if (BC) delete[]BC;
			if (NBC > 0)
				BC = new BCTAB[NBC];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"里程");
				_tcscpy(BC[i].ckml, strText);
				Report_input.GetValueString(strText, L"高程");
				BC[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NBC = i;
		}
		else if (TabFname == "横断面数量表")
		{
			int i = 0;
			NHDM = Report_input.GetRowCount();
			if (HDMB) delete[]HDMB;
			HDMB = NULL;
			if (NTW > 0)
				HDMB = new HdmAreaData[NHDM];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(HDMB[i].NameOfWay, L"线路名");
				Report_input.GetValueString(HDMB[i].GH, L"中桩里程冠号");//冠号			
				Report_input.GetValueString(strText, L"中桩里程数");//里程数
				HDMB[i].dml = _wtof(strText);
				Report_input.GetValueString(strText, L"断面性质");//断面性质
				HDMB[i].BTinf = _wtoi(strText);
				Report_input.GetValueString(strText, L"填方面积");//填方面积
				HDMB[i].at = _wtof(strText);
				Report_input.GetValueString(strText, L"改良土面积");//改良土面积
				HDMB[i].aglt = _wtof(strText);
				Report_input.GetValueString(strText, L"渗水土面积");//渗水土面积
				HDMB[i].asst = _wtof(strText);
				Report_input.GetValueString(strText, L"清表土面积");//清表土面积
				HDMB[i].aqbt = _wtof(strText);
				Report_input.GetValueString(strText, L"挖方面积");//挖方面积
				HDMB[i].acut = _wtof(strText);
				Report_input.GetValueString(strText, L"I类挖方面积");//挖方面积
				HDMB[i].aw[0] = _wtof(strText);
				Report_input.GetValueString(strText, L"II类挖方面积");//挖方面积
				HDMB[i].aw[1] = _wtof(strText);
				Report_input.GetValueString(strText, L"III类挖方面积");//挖方面积
				HDMB[i].aw[2] = _wtof(strText);
				Report_input.GetValueString(strText, L"IV类挖方面积");//挖方面积
				HDMB[i].aw[3] = _wtof(strText);
				Report_input.GetValueString(strText, L"V类挖方面积");//挖方面积
				HDMB[i].aw[4] = _wtof(strText);
				Report_input.GetValueString(strText, L"VI类挖方面积");//挖方面积
				HDMB[i].aw[5] = _wtof(strText);
				Report_input.GetValueString(strText, L"左用地宽");//左用地宽
				HDMB[i].LZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"右用地宽");//右用地宽
				HDMB[i].RZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"护肩面积");//护肩面积
				HDMB[i].ahj = _wtof(strText);
				Report_input.GetValueString(strText, L"水沟浆砌面积");//水沟浆砌面积
				HDMB[i].ajq = _wtof(strText);
				Report_input.GetValueString(strText, L"水沟填面积");//水沟填面积
				HDMB[i].asgt = _wtof(strText);
				Report_input.GetValueString(strText, L"水沟挖面积");// 水沟挖面积
				HDMB[i].asgw = _wtof(strText);
				Report_input.GetValueString(strText, L"草皮边坡斜长");//草皮边坡斜长			  
				HDMB[i].cpbpxc = _wtof(strText);
				Report_input.GetValueString(strText, L"挡墙边坡斜长");//挡墙边坡斜长			  
				HDMB[i].dqbpxc = _wtof(strText);
				i++;
				Report_input.MoveNext();
			}
			NHDM = i;
		}



	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	if ((TabFname == "大中桥" || TabFname == "小桥涵" || TabFname == "隧道" || TabFname == "车站") && (wlcstring != "线路名=\"既有基线\"" && wlcstring != "线路名=\"贯通设计基线\"" && wlcstring != "线路名=\"贯通设计Ⅱ线\""))
	{
		//检查二线插旗数据是否在绕行段内，剔除不在的、
		GetRXDChaQi(mdbFname, strEXName, RXDName, TabFname);
	}

	return 1;
	}
int XLDataBase_JYX::Delete_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//写入线路数据库中的数据
{

	CString strText, GH, strSQL, strSQL2;

	CFileFind Find;         //先定义类对象,才能用里面的涵数
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		//		GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
		File.Format(L"%s\\lib\\标准.mdb", SysPath);
		CopyFile(File, mdbFname, false);         //file 已存在,Path 不存在复制文件
	}

	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			ads_printf(L"连接%s数据库失败!", mdbFname);
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	_variant_t str;
	InitTableList();

	if (FindTable(TabFname))//表存在
	{
		if (wlcstring == "")
		{
			strSQL2 = L"delete  from " + TabFname;
		}
		else
		{

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
	}

		m_adoConnection.Execute(strSQL2);   //???????????
		//	Report_input.Open(strSQL);
}
	m_adoConnection.Disconnect();
	return 1;
}
//-----------------------------------------------------------------------------------------
int XLDataBase_JYX::WR_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//写入线路数据库中的数据
{
	CString strText, GH, strSQL, strSQL2;

	CFileFind Find;         //先定义类对象,才能用里面的涵数
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		//    GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
		File.Format(L"%s\\lib\\标准.mdb", SysPath);
		CopyFile(File, mdbFname, false);         //file 已存在,Path 不存在复制文件

	}

	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			//ads_printf(L"连接%s数据库失败!",mdbFname);			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	_variant_t str;
	InitTableList();

	if (FindTable(TabFname))//表存在
	{
		if (wlcstring == "")
		{
			strSQL2 = L"delete from " + TabFname;
		}
		else
		{
			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
		}

		m_adoConnection.Execute(strSQL2);   //???????????
		//	Report_input.Open(strSQL);
	}


	/////////////////////////////////////////////////////////////////////////////////
	//既有线

	if (TabFname == "交点数据表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "交点号 VARCHAR,";
			strSQL += "坐标N DOUBLE PRECISION,";//建立新表     
			strSQL += "坐标E DOUBLE PRECISION,";
			//	strSQL +="偏角 DOUBLE PRECISION,";
			strSQL += "前缓长 DOUBLE PRECISION,";
			strSQL += "半径 DOUBLE PRECISION,";
			strSQL += "后缓长 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		// strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL2);

		m_strSQL = L"select * from " + TabFname;
		//	 int iCols=0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int i = 0; i < JDarray.GetSize(); i++)
			{

				Report_input.AddNew();

				/*
				strText.Format(L"%d",i+1);//ID
				str = strText;
				Report_input.PutCollect(0,str);
				*/

				strText.Format(L"%s", JDarray[i].GJDNum);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", JDarray[i].JDNum);  //交点
				str = strText;
				Report_input.PutCollect(2, str);


				strText.Format(L"%lf", JDarray[i].N);  //y

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JDarray[i].E);  //x

				str = strText;
				Report_input.PutCollect(4, str);

				if (i == 0 || i == NQX - 1)//起终点半径为0
					JDarray[i].R = JDarray[i].l2 = 0.0;

				strText.Format(L"%0.3lf", JDarray[i].l1);  //l1
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%0.3lf", JDarray[i].R);  //R
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%0.3lf", JDarray[i].l2);  //l2
				str = strText;
				Report_input.PutCollect(7, str);
			}
			Report_input.Update();


		}
		catch (_com_error e)
		{
		}

		if (JDarray.GetSize() > 0) JDarray.RemoveAll();
		Report_input.Close();

	}
	else if (TabFname == "既有曲线测量原始资料表")
	{

		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "交点号 VARCHAR,";
			strSQL += "置镜点 integer,";//建立新表 
			//	strSQL +="冠号 VARCHAR,";
			strSQL += "测点里程 VARCHAR,";
			strSQL += "偏角 DOUBLE PRECISION,";

			strSQL += "置测距 DOUBLE PRECISION,";
			strSQL += "右角 DOUBLE PRECISION,";
			strSQL += "既有Ej DOUBLE PRECISION,";
			strSQL += "设计Es DOUBLE PRECISION,";
			strSQL += "拨量 DOUBLE PRECISION,";
			strSQL += "拨量正域值 DOUBLE PRECISION,";
			strSQL += "拨量负域值 DOUBLE PRECISION,";
			//	 strSQL +="属性 VARCHAR"; 
			strSQL += "属性 VARCHAR,";
			strSQL += "里程差 DOUBLE PRECISION";

			/*		strSQL +="坐标x DOUBLE PRECISION,";
			strSQL +="坐标y DOUBLE PRECISION";*/

			strSQL += ")";

			m_adoConnection.Execute(strSQL);//创建表    //????

		}

		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL2);
		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				// return 0;
			}

			for (int i = 0; i < NOrigMeaNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", JYOrigMeaTable[i].jdno);  //交点号
				str = strText;
				Report_input.PutCollect(1, str);


				strText.Format(L"%d", JYOrigMeaTable[i].ZJPoint);  //置镜点
				str = strText;
				Report_input.PutCollect(2, str);

				/*
				strText.Format(L"%s",JYOrigMeaTable[i].GH);  //冠号
				str = strText;
				Report_input.PutCollect(3,str);
				*/

				strText = JYOrigMeaTable[i].Landmark;  //测点里程
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].fj);  //偏角
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].ZhiCeJv);  //测距
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Youjiao);  //右角
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Ej);  //既有Ej
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Es);  //既有Es
				str = strText;
				Report_input.PutCollect(8, str);


				strText.Format(L"%lf", JYOrigMeaTable[i].deE);  //拨量
				str = strText;
				Report_input.PutCollect(9, str);


				strText.Format(L"%lf", JYOrigMeaTable[i].BLTerritoryPlus);  //拨量区域(+)
				str = strText;
				Report_input.PutCollect(10, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].BLTerritoryMinus);  //拨量区域(-)
				str = strText;
				Report_input.PutCollect(11, str);


				str = JYOrigMeaTable[i].ShuXing;
				Report_input.PutCollect(12, str);


				strText.Format(L"%lf", JYOrigMeaTable[i].LCcha);
				str = strText;
				Report_input.PutCollect(13, str);

				//	 Report_input.PutCollect(8,str); 
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		/*
		if(JYJDDataTable) delete []JYJDDataTable;
		JYJDDataTable = NULL;*/
		Report_input.Close();

	}


	///////////////////
	if (TabFname == "标志里程表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "标志里程 VARCHAR,";
			strSQL += "X坐标 DOUBLE PRECISION,";//建立新表     
			strSQL += "Y坐标 DOUBLE PRECISION";
			//	strSQL +="连续里程 DOUBLE PRECISION,";
			//	strSQL +="系数K DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		// strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL2);

		m_strSQL = L"select * from " + TabFname;
		//	 int iCols=0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < BZLCNum; i++)
			{
				Report_input.AddNew();

				//线路名
				strText = BZLCTable[i].GJDNum;
				str = strText;
				Report_input.PutCollect(1, str);

				strText = BZLCTable[i].kml;
				str = strText;     //标志里程
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", BZLCTable[i].X);  //x
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", BZLCTable[i].Y);  //Y
				str = strText;
				Report_input.PutCollect(4, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (BZLCTable) delete[]BZLCTable;
		BZLCTable = NULL;
		Report_input.Close();
	}





	if (TabFname == "外移桩导线点表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "冠号 VARCHAR,";
			strSQL += "里程 VARCHAR,";
			strSQL += "坐标N DOUBLE PRECISION,";//建立新表     
			strSQL += "坐标E DOUBLE PRECISION,";

			strSQL += "外移距 DOUBLE PRECISION,";
			strSQL += "属性 VARCHAR,";
			strSQL += "线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < m_iNWYZ; i++)
			{
				Report_input.AddNew();

				//线路名
				strText = m_pWYZArr[i].m_cGH;
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", m_pWYZArr[i].m_dLC);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", m_pWYZArr[i].m_dN);
				str = strText;
				// 				AfxMessageBox(strText);
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", m_pWYZArr[i].m_dE);
				str = strText;
				Report_input.PutCollect(4, str);


				strText.Format(L"%lf", m_pWYZArr[i].m_dOffset);
				str = strText;
				Report_input.PutCollect(5, str);


				if (m_pWYZArr[i].m_iZXOrQX == 0)
					str = "直线中";
				else if (m_pWYZArr[i].m_iZXOrQX == -1)
					str = "直线始";
				else if (m_pWYZArr[i].m_iZXOrQX == 1)
					str = "直线止";
				else if (m_pWYZArr[i].m_iZXOrQX == 2)
					str = "曲线点";
				else if (m_pWYZArr[i].m_iZXOrQX == 3)
					str = "直线点";
				else
					str = "";


				Report_input.PutCollect(6, str);

				strText = m_pWYZArr[i].XLName;
				str = strText;
				Report_input.PutCollect(7, str);


			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (m_pWYZArr)
		{
			delete[]m_pWYZArr;
			m_pWYZArr = NULL;
		}
		Report_input.Close();
	}




	if (TabFname == "纵面台帐表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "冠号 VARCHAR,";
			strSQL += "里程 VARCHAR,";
			strSQL += "线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < m_iNZMTZ; i++)
			{
				Report_input.AddNew();

				//线路名
				strText = m_pZMTZArr[i].m_cGH;
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", m_pZMTZArr[i].m_dLC);
				str = strText;
				Report_input.PutCollect(2, str);


				strText = m_pZMTZArr[i].XLName;
				str = strText;
				Report_input.PutCollect(3, str);


			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (m_pZMTZArr)
		{
			delete[]m_pZMTZArr;
			m_pZMTZArr = NULL;
		}
		Report_input.Close();
	}



	if (TabFname == "行政区划表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "起始里程 VARCHAR,";
			strSQL += "终止里程 VARCHAR,";
			strSQL += "名称 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < m_iXZQH; i++)
			{
				Report_input.AddNew();

				strText = m_pXZQHArr[i].m_sSml;
				str = strText;
				Report_input.PutCollect(1, str);

				strText = m_pXZQHArr[i].m_sEml;
				str = strText;
				Report_input.PutCollect(2, str);

				strText = m_pXZQHArr[i].m_sName;
				str = strText;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (m_pXZQHArr)
		{
			delete[]m_pXZQHArr;
			m_pXZQHArr = NULL;
		}
		Report_input.Close();
	}











	//////////////////////
	else if (TabFname == "既有曲线拨量计算结果表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "交点号 VARCHAR,";
			strSQL += "曲线偏角 DOUBLE PRECISION,";//建立新表     
			strSQL += "前缓长 DOUBLE PRECISION,";
			strSQL += "曲线半径 DOUBLE PRECISION,";
			strSQL += "后缓长 DOUBLE PRECISION,";
			strSQL += "ZH里程 DOUBLE PRECISION,";
			strSQL += "HY里程 DOUBLE PRECISION,";
			strSQL += "YH里程 DOUBLE PRECISION,";
			strSQL += "HZ里程 DOUBLE PRECISION,";
			strSQL += "冠号 VARCHAR,";
			strSQL += "是否外业实测 VARCHAR,";
			strSQL += "线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			for (int i = 0; i < NBLNum; i++)
			{

				//	 ads_printf(L"i=%d,JYBoLiangTable[i].l1=%lf\n",i,JYBoLiangTable[i].l1);
				//	 Report_input.MoveLast();
				Report_input.AddNew();



				strText = JYBoLiangTable[i].JDNum;  //交点号
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", JYBoLiangTable[i].alfa);  //曲线偏角

				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", JYBoLiangTable[i].l1);  //前缓长

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JYBoLiangTable[i].R);  //半径
				str = strText;
				Report_input.PutCollect(4, str);


				strText.Format(L"%lf", JYBoLiangTable[i].l2);  //后缓长
				str = strText;
				Report_input.PutCollect(5, str);


				strText.Format(L"%lf", JYBoLiangTable[i].KZH);   //KZH里程
				str = strText;
				Report_input.PutCollect(6, str);


				strText.Format(L"%lf", JYBoLiangTable[i].KHY);  //KHY里程
				str = strText;
				Report_input.PutCollect(7, str);

				//strText.Format(L"%lf",JYBoLiangTable[i].KQZ);  //KQZ里程
				//str = strText;
				//Report_input.PutCollect(8,str);

				strText.Format(L"%lf", JYBoLiangTable[i].KYH);  //KYH里程
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%lf", JYBoLiangTable[i].KHZ);  //KHZ里程
				str = strText;
				Report_input.PutCollect(9, str);


				strText = JYBoLiangTable[i].GH;  //冠号
				str = strText;
				Report_input.PutCollect(10, str);

				if (JYBoLiangTable[i].IsMeasure)//是否外业实测
					Report_input.PutCollect(11, L"是");
				else
					Report_input.PutCollect(11, L"否");

				strText = JYBoLiangTable[i].XLName;  //线路名
				str = strText;
				Report_input.PutCollect(12, str);


			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}
	////////////////////////////////

	else if (TabFname == "设计曲线结果表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "交点号 VARCHAR,";
			strSQL += "曲线偏角 DOUBLE PRECISION,";//建立新表     
			strSQL += "前缓长 DOUBLE PRECISION,";
			strSQL += "曲线半径 DOUBLE PRECISION,";
			strSQL += "后缓长 DOUBLE PRECISION,";
			strSQL += "CKZH里程 VARCHAR,";

			strSQL += "CKHY里程 VARCHAR,";

			strSQL += "CKYH里程 VARCHAR,";
			strSQL += "CKHZ里程 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表

		}
		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			for (int i = 0; i < NSJQXB; i++)
			{

				//	 ads_printf(L"i=%d,JYBoLiangTable[i].l1=%lf\n",i,JYBoLiangTable[i].l1);
				//	 Report_input.MoveLast();
				Report_input.AddNew();


				strText = SJQXB[i].XLName;
				str = strText;
				Report_input.PutCollect(1, str);

				strText = SJQXB[i].JDNum;  //交点号
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%.12lf", SJQXB[i].LorR * SJQXB[i].a);  //曲线偏角

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", SJQXB[i].l1);  //前缓长

				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", SJQXB[i].R);  //半径
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", SJQXB[i].l2);  //后缓长
				str = strText;
				Report_input.PutCollect(6, str);


				str = SJQXB[i].CKZH;   //CKZH里程
				Report_input.PutCollect(7, str);

				str = SJQXB[i].CKHY;  //CKHY里程
				Report_input.PutCollect(8, str);


				str = SJQXB[i].CKYH;  //CKYH里程
				Report_input.PutCollect(9, str);

				str = SJQXB[i].CKHZ;  //CKHZ里程
				Report_input.PutCollect(10, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}


	else if (TabFname == "改建段汇总表")
	{
		if (!FindTable(TabFname))//表不存在
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";

			strSQL += "起点里程 VARCHAR,";
			strSQL += "起点N坐标 DOUBLE PRECISION,";
			strSQL += "起点E坐标 DOUBLE PRECISION,";

			strSQL += "终点里程 VARCHAR,";
			strSQL += "终点N坐标 DOUBLE PRECISION,";
			strSQL += "终点E坐标 DOUBLE PRECISION,";
			strSQL += "起始线路名 VARCHAR,";
			strSQL += "终止线路名 VARCHAR,";
			strSQL += "改建性质 VARCHAR,";
			strSQL += "改建名 VARCHAR";

			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			for (int i = 0; i < GJDAllInforNum; i++)
			{
				Report_input.AddNew();

				strText = GJDAllInforTable[i].ToOutSGJJDNum;  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText = GJDAllInforTable[i].GJDBLC;
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", GJDAllInforTable[i].BpointToN);
				str = strText;
				Report_input.PutCollect(3, str);


				strText.Format(L"%lf", GJDAllInforTable[i].BpointToE);
				str = strText;
				Report_input.PutCollect(4, str);


				strText = GJDAllInforTable[i].GJDELC;
				str = strText;
				Report_input.PutCollect(5, str);


				strText.Format(L"%lf", GJDAllInforTable[i].EpointToN);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", GJDAllInforTable[i].EpointToE);
				str = strText;
				Report_input.PutCollect(7, str);


				//起始线路性质
				strText = GJDAllInforTable[i].BXLName;
				str = strText;
				Report_input.PutCollect(8, str);

				strText = GJDAllInforTable[i].EXLName;  //改建段终止线路名
				str = strText;
				Report_input.PutCollect(9, str);

				if (_tcscmp(GJDAllInforTable[i].ToOutSGJJDNum, L"贯通设计基线") && _tcscmp(GJDAllInforTable[i].ToOutSGJJDNum, L"贯通设计Ⅱ线"))//仅纵面用
				{
					strText = GJDAllInforTable[i].GJXZ;
					str = strText;
					Report_input.PutCollect(10, str);
				}
				else
				{
					if (GJDAllInforTable[i].GJDType == 0)str = "并行改建";
					else str = "绕行改建";
					Report_input.PutCollect(10, str);
				}

				strText = GJDAllInforTable[i].ToInSGJJDNum;
				str = strText;
				Report_input.PutCollect(11, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}

	else if (TabFname == "联接信息表")
	{

		if (!FindTable(TabFname))//表不存在
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "联接线线路名 VARCHAR,";
			strSQL += "起始线路名 VARCHAR,";
			strSQL += "终止线路名 VARCHAR,";//建立新表     
			strSQL += "内部联接线线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		for (int i = 0; i < ConnectLineInforNum; i++)
		{
			wlcstring = ConnectLineTable[i].ToInSGJJDNum;
			wlcstring = L"内部联接线线路名=\"" + wlcstring + "\"";

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
			m_adoConnection.Execute(strSQL2);
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}



			for (int i = 0; i < ConnectLineInforNum; i++)
			{
				Report_input.AddNew();

				strText = ConnectLineTable[i].ToOutSGJJDNum;  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText = ConnectLineTable[i].BXLName;
				str = strText;
				Report_input.PutCollect(2, str);

				strText = ConnectLineTable[i].EXLName;
				str = strText;
				Report_input.PutCollect(3, str);

				strText = ConnectLineTable[i].ToInSGJJDNum;
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}

	else if (TabFname == "施工便线信息表")
	{

		if (!FindTable(TabFname))//表不存在
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "起始线路名 VARCHAR,";
			strSQL += "终止线路名 VARCHAR,";//建立新表     
			strSQL += "内部线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		for (int i = 0; i < SGBXInforNum; i++)
		{
			wlcstring = SGBXTable[i].SGBXName;
			wlcstring = L"线路名=\"" + wlcstring + "\"";

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
			m_adoConnection.Execute(strSQL2);
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < SGBXInforNum; i++)
			{
				Report_input.AddNew();

				strText = SGBXTable[i].SGBXName;  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText = SGBXTable[i].BXLName;
				str = strText;
				Report_input.PutCollect(2, str);

				strText = SGBXTable[i].EXLName;
				str = strText;
				Report_input.PutCollect(3, str);

				strText = SGBXTable[i].ToInSGBXName;
				str = strText;
				Report_input.PutCollect(4, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}
	else if (TabFname == "局部基线信息表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		for (int i = 0; i < JBJXInforNum; i++)
		{
			wlcstring = JBJXTable[i].JBJXName;
			wlcstring = L"线路名=\"" + wlcstring + "\"";

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
			m_adoConnection.Execute(strSQL2);
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < JBJXInforNum; i++)
			{
				Report_input.AddNew();

				strText = JBJXTable[i].JBJXName;  //线路名
				str = strText;
				Report_input.PutCollect(1, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		// 		if(JYJDDataTable) delete []JYJDDataTable;
		// 		JYJDDataTable = NULL;
		Report_input.Close();
	}
	else if (TabFname == "断链表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名  VARCHAR,";
			strSQL += "等号左里程冠号 VARCHAR,";
			strSQL += "等号左里程数 DOUBLE PRECISION,";
			strSQL += "等号右里程冠号 VARCHAR,";  //DOUBLE PRECISION -> VARCHAR
			strSQL += "等号右里程数 DOUBLE PRECISION,";
			strSQL += "断链长度 DOUBLE PRECISION,";
			strSQL += "坐标N DOUBLE PRECISION,";
			strSQL += "坐标E DOUBLE PRECISION";
			//	strSQL +="附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		//	strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int i = 0; i < NDL; i++)
			{
				Report_input.AddNew();

				/*
				strText.Format(L"%d",i);//ID
				str = strText;
				Report_input.PutCollect(0,str);
				*/

				str = DLB[i].GJDNum;
				Report_input.PutCollect(1, str);

				str = DLB[i].BGH;
				Report_input.PutCollect(2, str);


				Report_input.PutCollect(3, DLB[i].BLC);

				str = DLB[i].EGH;
				Report_input.PutCollect(4, str);

				double Eml = DLB[i].BLC + DLB[i].DL;
				Report_input.PutCollect(5, Eml);

				Report_input.PutCollect(6, DLB[i].DL);
				Report_input.PutCollect(7, DLB[i].N);
				Report_input.PutCollect(8, DLB[i].E);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}

	//二线左线曲线表
	///////////////

	/*
	else if(TabFname=="Ⅱ线改建分段表")
	{
	if(!FindTable(TabFname))//表不存在
	{
	CString strSQL;
	strSQL = "create table " ;
	strSQL += TabFname;
	strSQL += "(";
	strSQL += "编号 integer identity,";
	strSQL += "既有线路名 VARCHAR,";
	strSQL +="起点N DOUBLE PRECISION,";
	strSQL +="起点E DOUBLE PRECISION,";
	strSQL +="终点N DOUBLE PRECISION,";
	strSQL +="终点E DOUBLE PRECISION";
	strSQL += ")";
	m_adoConnection.Execute(strSQL);//创建表
	}

	m_strSQL ="select * from "+TabFname;
	int iCols=0;
	try
	{
	if (!m_adoConnection.IsOpen())
	{
	ads_alert(L"数据库没有打开或已经关闭!");
	return 0;
	}

	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
	//	 ads_printf(L"没有记录!");
	//		return 0;
	}
	int j=0;

	for(int i=0;i<NEXGJDFDInf;i++)
	{
	Report_input.AddNew();

	strText = pEXGJDFDInf[i].XLName;
	str=strText;
	Report_input.PutCollect(1,str);
	Report_input.PutCollect(2,pEXGJDFDInf[i].spt[0]);
	Report_input.PutCollect(3,pEXGJDFDInf[i].spt[1]);
	Report_input.PutCollect(4,pEXGJDFDInf[i].ept[0]);
	Report_input.PutCollect(5,pEXGJDFDInf[i].ept[1]);
	}
	Report_input.Update();
	}
	catch(_com_error e)
	{
	}
	if(QXB) delete []QXB;
	QXB = NULL;
	Report_input.Close();
	}
	*/

	else if (TabFname == "二线曲线表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "交点属性 VARCHAR,";
			strSQL += "二线交点号 VARCHAR,";
			strSQL += "一线交点号 VARCHAR,";
			strSQL += "线间距 DOUBLE PRECISION,";
			strSQL += "坐标N DOUBLE PRECISION,";
			strSQL += "坐标E DOUBLE PRECISION,";
			strSQL += "前缓和曲线长 DOUBLE PRECISION,";
			strSQL += "曲线半径 DOUBLE PRECISION,";
			strSQL += "后缓和曲线长 DOUBLE PRECISION,";
			strSQL += "内业断链 DOUBLE PRECISION,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			//	int j=0;

			for (int i = 0; i < NQX2x; i++)
			{
				Report_input.AddNew();

				strText = QXB2x[i].XLName;
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%d", QXB2x[i].JDXZ);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%s", QXB2x[i].JDNum);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%d", QXB2x[i].jdno1);
				str = strText;
				Report_input.PutCollect(4, str);

				Report_input.PutCollect(5, QXB2x[i].xjj);

				Report_input.PutCollect(6, QXB2x[i].x);
				Report_input.PutCollect(7, QXB2x[i].y);
				Report_input.PutCollect(8, QXB2x[i].Lo1);
				Report_input.PutCollect(9, QXB2x[i].R);
				Report_input.PutCollect(10, QXB2x[i].Lo2);
				Report_input.PutCollect(11, QXB2x[i].NYDL);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}

		if (QXB) delete[]QXB;
		QXB = NULL;
		Report_input.Close();
	}
	else if (TabFname == "二线断链表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "绕行段名 VARCHAR,";
			strSQL += "线路编号 VARCHAR,";
			strSQL += "等号左里程冠号 VARCHAR,";
			strSQL += "等号左里程数 DOUBLE PRECISION,";
			strSQL += "等号右里程冠号 VARCHAR,";
			strSQL += "等号右里程数 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int i = 0; i < NDL2x; i++)
			{
				Report_input.AddNew();

				/*
				strText.Format(L"%d",i);//ID
				str = strText;
				Report_input.PutCollect(0,str);
				*/

				//	str=DLB2x[i].RXDName;
				str = DLB2x[i].XLName;
				Report_input.PutCollect(1, str);

				str = DLB2x[i].RXDName;
				Report_input.PutCollect(2, str);

				strText.Format(L"%d", DLB2x[i].no);//ID
				str = strText;
				Report_input.PutCollect(3, str);

				str = DLB2x[i].BGH;
				Report_input.PutCollect(4, str);

				Report_input.PutCollect(5, DLB2x[i].BLC);

				str = DLB2x[i].EGH;
				Report_input.PutCollect(6, str);
				Report_input.PutCollect(7, DLB2x[i].ELC);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}

	//////////////////
	//------------------------纵面-----------------------------//
	//////////////////////
	else if (TabFname == "纵断面基本参数表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";//建立新表 
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "横向比例 DOUBLE PRECISION,";
			strSQL += "纵向比例 DOUBLE PRECISION,";
			strSQL += "里程取整 VARCHAR,";
			strSQL += "坡度位数 VARCHAR,";
			strSQL += "高程位数 VARCHAR";
			strSQL += ")";

			m_adoConnection.Execute(strSQL);//创建表 
		}

		//	 strSQL.Format(L"delete from %s",TabFname);//删除全部
		//	 m_adoConnection.Execute(strSQL2);
		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//ads_printf(L"没有记录!");
				//return 0;
			}

			for (int i = 0; i < 1; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", pBasicDataOfZDM[i].WayName);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", pBasicDataOfZDM[i].XScale);  //X比例
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", pBasicDataOfZDM[i].YScale);  //Y比例
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].MLDecimal);  //里程取整
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].PDDeciamal);  //坡度取整
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].HDecimal);  //坡度取整
				str = strText;
				Report_input.PutCollect(6, str);

			}
			Report_input.Update();
		}



		catch (_com_error e)
		{
		}
		if (pBasicDataOfZDM) delete[]pBasicDataOfZDM;
		pBasicDataOfZDM = NULL;
		Report_input.Close();
	}

	//////////////...
	if (TabFname == "坡度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "变坡点里程冠号 VARCHAR,";
			strSQL += "变坡点里程数 DOUBLE PRECISION,";
			strSQL += "设计高程 DOUBLE PRECISION,";
			strSQL += "坡度 DOUBLE PRECISION,";
			strSQL += "坡长 DOUBLE PRECISION,";
			strSQL += "竖曲线半径 DOUBLE PRECISION,";
			strSQL += "竖曲线长度 DOUBLE PRECISION,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NPD; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", PDB[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", PDB[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", PDB[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", PDB[i].Level);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", PDB[i].degree);
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", PDB[i].length);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", PDB[i].Rshu);
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", PDB[i].RLen);
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%s", PDB[i].Notes);
				str = strText;
				Report_input.PutCollect(9, str);

				if (i == 0)
				{
					//if (GlobalBasicDataOfZDM.m_HDecimal=="保留零位小数")//BasicDataOfZDM-1
					//{
					//	strText.Format(_T("%d"),0);
					//}
					//else if (GlobalBasicDataOfZDM.m_HDecimal=="保留一位小数")
					//{
					//	strText.Format(_T("%d"),1);
					//}
					//else if (GlobalBasicDataOfZDM.m_HDecimal=="保留两位小数")
					//{
					//	strText.Format(_T("%d"),2);
					//}
					//else
					//{
					//	strText.Format(_T("%d"),3);
					//}
					strText.Format(_T("%d"), 2);
					str = strText;
					Report_input.PutCollect(7, str);

					//strText.Format(L"%d",GlobalBasicDataOfZDM.m_LJorGM); //去BasicDataOfZDM-2
					//str = strText;
					//Report_input.PutCollect(8,str);

					//if (GlobalBasicDataOfZDM.m_PDDeciamal=="保留零位小数")
					//{
					//	strText.Format(_T("%d"),0);
					//}
					//else if (GlobalBasicDataOfZDM.m_PDDeciamal=="保留一位小数")
					//{
					//	strText.Format(_T("%d"),1);
					//}
					//else if (GlobalBasicDataOfZDM.m_PDDeciamal=="保留两位小数")
					//{
					//	strText.Format(_T("%d"),2);
					//}
					//else
					//{
					//	strText.Format(_T("%d"),3);
					//}
					strText.Format(_T("%d"), 2);
					str = strText;
					Report_input.PutCollect(9, str);
				}

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (PDB) delete[]PDB;
		PDB = NULL;
		Report_input.Close();
	}

	//////////////...
	if (TabFname == "大中桥")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "桥梁名称 VARCHAR,";

			strSQL += "中心里程冠号 VARCHAR,";
			strSQL += "中心里程数 DOUBLE PRECISION,";
			strSQL += "起点里程冠号 VARCHAR,";
			strSQL += "起点里程数 DOUBLE PRECISION,";
			strSQL += "终点里程冠号 VARCHAR,";
			strSQL += "终点里程数 DOUBLE PRECISION,";

			strSQL += "孔跨式样 VARCHAR,";
			strSQL += "线数 INTEGER,";
			strSQL += "桥全长 DOUBLE PRECISION,";
			strSQL += "100年水位高程 DOUBLE PRECISION,";
			strSQL += "改建状态 VARCHAR,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", Bri[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", Bri[i].BriName);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%s", Bri[i].CGH);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", Bri[i].CenDml);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%s", Bri[i].BGH);
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", Bri[i].StDml);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%s", Bri[i].EGH);
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", Bri[i].EdDml);
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%s", Bri[i].KKSY);
				str = strText;
				Report_input.PutCollect(9, str);

				strText.Format(L"%d", Bri[i].LineNum);
				str = strText;
				Report_input.PutCollect(10, str);

				strText.Format(L"%lf", Bri[i].BriLen);
				str = strText;
				Report_input.PutCollect(11, str);

				strText.Format(L"%s", Bri[i].SWHt);
				str = strText;
				Report_input.PutCollect(12, str);

				strText.Format(L"%s", Bri[i].GJZT);
				str = strText;
				Report_input.PutCollect(13, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (Bri) delete[]Bri;
		Bri = NULL;
		Report_input.Close();
	}

	//////////////...
	if (TabFname == "隧道")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "隧道名称 VARCHAR,";
			strSQL += "起点里程冠号 VARCHAR,";
			strSQL += "起点里程数 DOUBLE PRECISION,";
			strSQL += "终点里程冠号 VARCHAR,";
			strSQL += "终点里程数 DOUBLE PRECISION,";
			strSQL += "插旗方式 VARCHAR,";
			strSQL += "改建状态 VARCHAR,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < TunNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", Tun[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", Tun[i].TUNName);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%s", Tun[i].BGH);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", Tun[i].StDml);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%s", Tun[i].EGH);
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", Tun[i].EdDml);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%s", Tun[i].flag);
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%s", Tun[i].GJZT);
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%d", Tun[i].NumOfLine);
				str = strText;
				Report_input.PutCollect(9, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (Tun) delete[]Tun;
		Tun = NULL;
		Report_input.Close();
	}

	//////////////...
	if (TabFname == "地面线")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "地面高程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NDMX; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", DMXB[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", DMXB[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", DMXB[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", DMXB[i].Level);
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (DMXB) delete[]DMXB;
		DMXB = NULL;
		Report_input.Close();
	}

	//////////////...
	if (TabFname == "既有轨面线")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "既有轨面高程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NGMX; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", GMXB[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", GMXB[i].CAP);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", GMXB[i].ML);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", GMXB[i].H);
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (GMXB) delete[]GMXB;
		GMXB = NULL;
		Report_input.Close();
	}



	else if (TabFname == "车站")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "车站名称 VARCHAR,";
			strSQL += "站房中心里程冠号 VARCHAR,";
			strSQL += "站房中心里程数 DOUBLE PRECISION,";
			strSQL += "起点里程冠号 VARCHAR,";
			strSQL += "起点里程数 DOUBLE PRECISION,";
			strSQL += "终点里程冠号 VARCHAR,";
			strSQL += "终点里程数 DOUBLE PRECISION,";
			strSQL += "车站类型 VARCHAR,";
			strSQL += "插旗图块名 VARCHAR,";
			strSQL += "改建性质 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		//strSQL.Format(L"delete from %s",TabFname);//删除全部
		//m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < StaNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				str = Sta[i].NameOfWay;
				Report_input.PutCollect(1, str);

				str = Sta[i].StaName;
				Report_input.PutCollect(2, str);
				str = Sta[i].CGH;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", Sta[i].CenDml);
				str = strText;

				Report_input.PutCollect(4, str);

				str = Sta[i].BGH;
				Report_input.PutCollect(5, str);
				Report_input.PutCollect(6, Sta[i].StDml);
				str = Sta[i].EGH;
				Report_input.PutCollect(7, str);
				Report_input.PutCollect(8, Sta[i].EdDml);

				strText.Format(L"%lf", Sta[i].CZLX);
				str = strText;
				Report_input.PutCollect(9, str);
				strText.Format(L"%s", Sta[i].CQTKM);
				str = strText;
				Report_input.PutCollect(10, str);
				strText.Format(L"%s", Sta[i].GJXZ);
				str = strText;
				Report_input.PutCollect(11, str);
			}

			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (Sta) delete[]Sta;
		Sta = NULL;
		Report_input.Close();
	}


	if (TabFname == "小桥涵")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString m_SQL;
			m_SQL = "CREATE TABLE 小桥涵 ";
			m_SQL += "(";
			m_SQL += "编号 INTEGER identity,";
			m_SQL += "线路名 VARCHAR,";
			m_SQL += "中心里程冠号 VARCHAR,";
			m_SQL += "中心里程数 DOUBLE PRECISION,";
			m_SQL += "孔跨式样 VARCHAR,";
			m_SQL += "左横向长度 DOUBLE PRECISION,";
			m_SQL += "右横向长度 DOUBLE PRECISION,";
			m_SQL += "左涵口标高 DOUBLE PRECISION,";
			m_SQL += "右涵口标高 DOUBLE PRECISION,";
			m_SQL += "交角 DOUBLE PRECISION,";
			m_SQL += "用途 INTEGER,";
			m_SQL += "插旗方式 VARCHAR,";
			m_SQL += "改建性质 VARCHAR,";
			m_SQL += "线数 INTEGER";
			m_SQL += ")";
			m_adoConnection.Execute(m_SQL);
		}
		//strSQL.Format(L"delete from %s",TabFname);//删除全部
		//m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < SBriNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%s", SBri[i].NameOfWay);
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", SBri[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", SBri[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%s", SBri[i].KKStyle);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", SBri[i].dwL);
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", SBri[i].dwR);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", SBri[i].LLevel);
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", SBri[i].RLevel);
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%lf", SBri[i].JJiao);
				str = strText;
				Report_input.PutCollect(9, str);

				strText.Format(L"%lf", SBri[i].ToUse);
				str = strText;
				Report_input.PutCollect(10, str);

				strText.Format(L"%lf", SBri[i].InsertStyle);
				str = strText;
				Report_input.PutCollect(11, SBri[i].InsertStyle);

				strText.Format(L"%s", SBri[i].GJXZ);
				str = strText;
				Report_input.PutCollect(12, str);

				strText.Format(L"%d", SBri[i].LineNum);
				str = strText;
				Report_input.PutCollect(13, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (SBri) delete[]SBri;
		SBri = NULL;
		Report_input.Close();
	}




	if (TabFname == "既有道床厚度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "中桩冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "道床厚度 DOUBLE PRECISION,";
			strSQL += "轨道结构高度 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < RoadBedNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", RoadBed[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", RoadBed[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", RoadBed[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", RoadBed[i].H);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", RoadBed[i].H1);
				str = strText;
				Report_input.PutCollect(5, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (RoadBed) delete[]RoadBed;
		RoadBed = NULL;
		Report_input.Close();
	}
	if (TabFname == "既有路肩高程表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "中桩冠号 VARCHAR,";
			strSQL += "中桩里程 DOUBLE PRECISION,";
			strSQL += "既有路肩高程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NJiYouLuJianGC; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", JiYouLuJianGC[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", JiYouLuJianGC[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", JiYouLuJianGC[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JiYouLuJianGC[i].Level);
				str = strText;
				Report_input.PutCollect(4, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (JiYouLuJianGC) delete[]JiYouLuJianGC;
		JiYouLuJianGC = NULL;
		Report_input.Close();
	}
	if (TabFname == "纵面控制点表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "中桩冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "上限高程 DOUBLE PRECISION,";
			strSQL += "下限高程 DOUBLE PRECISION,";
			strSQL += "备注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < ZMControlPTNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", ZMControlPT[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", ZMControlPT[i].GH);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", ZMControlPT[i].ml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", ZMControlPT[i].H);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", ZMControlPT[i].H1);
				str = strText;
				Report_input.PutCollect(5, str);


				str = ZMControlPT[i].sRemark;
				Report_input.PutCollect(6, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (ZMControlPT) delete[]ZMControlPT;
		ZMControlPT = NULL;
		Report_input.Close();
	}
	if (TabFname == "左既有线占地宽")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "中桩冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "路基边界宽度 DOUBLE PRECISION,";
			strSQL += "占地边界宽度 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NZuoJiYouXianZhanDiKuan; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", ZuoJiYouXianZhanDiKuan[i].GH);
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", ZuoJiYouXianZhanDiKuan[i].ml);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", ZuoJiYouXianZhanDiKuan[i].LuJiBianJieKuan);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", ZuoJiYouXianZhanDiKuan[i].ZhanDiKuan);
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (ZuoJiYouXianZhanDiKuan) delete[]ZuoJiYouXianZhanDiKuan;
		ZuoJiYouXianZhanDiKuan = NULL;
		Report_input.Close();
	}
	if (TabFname == "右既有线占地宽")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "中桩冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "路基边界宽度 DOUBLE PRECISION,";
			strSQL += "占地边界宽度 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NYouJiYouXianZhanDiKuan; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", YouJiYouXianZhanDiKuan[i].GH);
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", YouJiYouXianZhanDiKuan[i].ml);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", YouJiYouXianZhanDiKuan[i].LuJiBianJieKuan);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", YouJiYouXianZhanDiKuan[i].ZhanDiKuan);
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (YouJiYouXianZhanDiKuan) delete[]YouJiYouXianZhanDiKuan;
		YouJiYouXianZhanDiKuan = NULL;
		Report_input.Close();
	}

	if (TabFname == "用地类型表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "分段终里程冠号 VARCHAR,";
			strSQL += "分段终里程数 DOUBLE PRECISION,";
			strSQL += "用地类型 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NYDLX; i++)
			{
				Report_input.AddNew();

				str = LXB[i].XLName;
				Report_input.PutCollect(1, str);
				str = LXB[i].GH;
				Report_input.PutCollect(2, str);
				strText.Format(L"%lf", LXB[i].dml);
				str = strText;
				Report_input.PutCollect(3, str);
				str = LXB[i].YDLX;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (LXB) delete[]LXB;
		LXB = NULL;
		Report_input.Close();
	}

	if (TabFname == "水准点表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "水准点表 ";
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "水准点编号 VARCHAR,";
			strSQL += "高程 DOUBLE PRECISION,";
			strSQL += "里程冠号 VARCHAR,";
			strSQL += "里程数 DOUBLE PRECISION,";
			strSQL += "距中线距离 DOUBLE PRECISION,";
			strSQL += "坐标N DOUBLE PRECISION,";
			strSQL += "坐标E DOUBLE PRECISION,";
			strSQL += "位置材料的描述 VARCHAR,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < BmNum; i++)
			{
				Report_input.AddNew();

				str = BM[i].BianHao; //水准点编号
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", BM[i].GaoCheng); //高程
				str = strText;
				Report_input.PutCollect(2, str);

				str = BM[i].GH; // 里程冠号
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", BM[i].XLC);  //里程数
				str = strText;
				Report_input.PutCollect(4, str);


				strText.Format(L"%lf", BM[i].DistZX);  // 距中线距离
				str = strText;
				Report_input.PutCollect(5, str);


				strText.Format(L"%lf", BM[i].N);  //坐标N
				str = strText;
				Report_input.PutCollect(6, str);


				strText.Format(L"%lf", BM[i].E);  //坐标E
				str = strText;
				Report_input.PutCollect(7, str);

				str = BM[i].CLMS;  //位置材料的描述
				Report_input.PutCollect(8, str);

				str = BM[i].FZ;  //附注
				Report_input.PutCollect(9, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{

		}

		if (BM) delete[]BM;
		BM = NULL;
		Report_input.Close();
	}


	if (TabFname == "导线表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "导线表 ";
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "桩号 VARCHAR,";
			strSQL += "右角 VARCHAR,";
			strSQL += "方位角 VARCHAR,";
			strSQL += "距离 DOUBLE PRECISION,";
			strSQL += "纬距 DOUBLE PRECISION,";
			strSQL += "经距 DOUBLE PRECISION,";
			strSQL += "高程 DOUBLE PRECISION,";
			strSQL += "里程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < DXNum; i++)
			{
				Report_input.AddNew();

				str = DX[i].ZhuangHao; //桩号
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", DX[i].RightAng); //右角
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", DX[i].FWJ);//方位角
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", DX[i].Dist);  //距离
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", DX[i].N);  // 纬距
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", DX[i].E);  //经距
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", DX[i].GaoCheng); //高程
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", DX[i].cml); //里程
				str = strText;
				Report_input.PutCollect(8, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{

		}

		if (DX) delete[]DX;
		DX = NULL;
		Report_input.Close();
	}

	if (TabFname == "标尺")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "里程 VARCHAR,";
			strSQL += "高程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NBC; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", BC[i].NameOfWay);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", BC[i].ckml);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", BC[i].Level);
				str = strText;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (BC) delete[]BC;
		BC = NULL;
		Report_input.Close();
	}


	if (TabFname == "轨道结构")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 integer identity,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "分段终里程 VARCHAR,";
			strSQL += "结构类型 VARCHAR,";
			strSQL += "结构高度 DOUBLE PRECISION ";

			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NGDSTRU; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", GDSTRU[i].XLName);  //线路名
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", GDSTRU[i].FdKml);
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%s", GDSTRU[i].FdKml);
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", GDSTRU[i].StruHeight);
				str = strText;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (GDSTRU) delete[]GDSTRU;
		GDSTRU = NULL;
		Report_input.Close();
	}


	else if (TabFname == "填挖表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "编号 INTEGER IDENTITY,";
			strSQL += "线路名 VARCHAR,";
			strSQL += "加桩里程冠号 VARCHAR,";
			strSQL += "加桩里程数 DOUBLE PRECISION,";
			strSQL += "地面高程 DOUBLE PRECISION, ";
			strSQL += "设计坡度  DOUBLE PRECISION, ";
			strSQL += "路肩设计高程 DOUBLE PRECISION,";
			strSQL += "竖曲线纵距 DOUBLE PRECISION,";
			strSQL += "中心抬降值 DOUBLE PRECISION,";
			strSQL += "中心填挖高度 DOUBLE PRECISION,";
			strSQL += "路肩施工高程 DOUBLE PRECISION,";
			strSQL += "曲线加宽 DOUBLE PRECISION,";
			strSQL += "右曲线加宽 DOUBLE PRECISION,";
			strSQL += "路肩施工宽度 DOUBLE PRECISION,";
			strSQL += "左线间距 DOUBLE PRECISION,";
			strSQL += "右线间距 DOUBLE PRECISION,";
			strSQL += "路肩分离限宽 DOUBLE PRECISION,";
			strSQL += "断面信息 VARCHAR,";
			strSQL += "附注 VARCHAR,";
			strSQL += "贯通基线线间距 DOUBLE PRECISION,";
			strSQL += "贯通二线线间距 DOUBLE PRECISION,";
			strSQL += "既有轨面高程 DOUBLE PRECISION,";
			strSQL += "设计轨面高程 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NTW; i++)
			{
				Report_input.AddNew();
				wlcstring.Replace(L"线路名=\"", L"");
				wlcstring.Replace(L"\"", L"");
				str = wlcstring;
				Report_input.PutCollect(1, str);
				str = TWB[i].GH;
				Report_input.PutCollect(2, str);
				//	AfxMessageBox(TWB[i].GH);
				Report_input.PutCollect(3, TWB[i].ml);
				Report_input.PutCollect(4, TWB[i].DMGC);
				Report_input.PutCollect(5, TWB[i].SJPD);
				Report_input.PutCollect(6, TWB[i].SJGC);
				Report_input.PutCollect(7, TWB[i].SQXZJ);
				Report_input.PutCollect(8, TWB[i].ZXTJ);
				Report_input.PutCollect(9, TWB[i].ZXTWG);
				Report_input.PutCollect(10, TWB[i].LJSGG);
				Report_input.PutCollect(11, TWB[i].ZQXJK);
				Report_input.PutCollect(12, TWB[i].YQXJK);
				if (fabs(TWB[i].LJSGK) > 0.00001)
					Report_input.PutCollect(13, TWB[i].LJSGK);
				Report_input.PutCollect(14, TWB[i].ZXJJ);
				Report_input.PutCollect(15, TWB[i].YXJJ);
				Report_input.PutCollect(16, TWB[i].LJFLK);
				strText.Format(L"%d", TWB[i].DMinf);//ID
				str = strText;
				Report_input.PutCollect(17, str);
				str = TWB[i].FZ;
				Report_input.PutCollect(18, str);
				Report_input.PutCollect(19, TWB[i].GTJXXJJ);
				Report_input.PutCollect(20, TWB[i].GTEXXJJ);
				Report_input.PutCollect(21, TWB[i].JYGMGC);
				Report_input.PutCollect(22, TWB[i].SJGMGC);


				//	 ads_printf(L"i=%d %lf %lf\n",i,TWB[i].ml,TWB[i].DMGC);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (TWB)
		{
			delete[]TWB;
			TWB = NULL;
		}

		Report_input.Close();
	}

	m_adoConnection.Disconnect();
	return 1;
}

bool XLDataBase_JYX::InitTableList(void)
{
	CAdoRecordSet rsetTable;
	_bstr_t Value;
	CString strTablename = "";
	try
	{
		if (m_adoConnection.GetConnection()->State != adStateOpen)
			return false;
		rsetTable = m_adoConnection.OpenSchema(adSchemaColumns);
		while (!rsetTable.IsEOF())
		{
			CString strValue;
			rsetTable.GetValueString(strValue, "TABLE_NAME");
			if (strValue != strTablename)
			{
				strTablename = strValue;
				m_strTableNameList.Add(strTablename);
			}
			rsetTable.MoveNext();
		}
	}
	catch (_com_error e)
	{
	}
	return TRUE;
}


CString XLDataBase_JYX::isNULLToCSting(CString strTempName)
{
	if (_wtof(strTempName) + 100000 < 1.0e-3)strTempName = "";
	return strTempName;
}

bool XLDataBase_JYX::FindTable(CString strTempName)
{
	CString str;
	for (int i = 0; i < m_strTableNameList.GetSize(); i++)
	{
		str = m_strTableNameList.GetAt(i);
		if (str == strTempName)
			return TRUE;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
int XLDataBase_JYX::WR_EX_XLDbs(CString mdbFname, CString TabFname, int irxd)//写入线路数据库中的数据
{
	CString strText, GH, strSQL;

	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"失败  3!");
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}
	_variant_t str;
	InitTableList();
	if (TabFname == "左线绕行段坡度表" || TabFname == "右线绕行段坡度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "绕行段编号 INTEGER,";
			strSQL += "变坡点里程冠号 VARCHAR,";
			strSQL += "变坡点里程数 DOUBLE PRECISION,";
			strSQL += "设计高程 DOUBLE PRECISION,";
			strSQL += "坡度 DOUBLE PRECISION,";
			strSQL += "坡长 DOUBLE PRECISION,";
			strSQL += "竖曲线半径 DOUBLE PRECISION,";
			strSQL += "竖曲线长度 DOUBLE PRECISION,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		strSQL.Format(L"delete from %s where 绕行段编号=%d", TabFname, irxd);//删除irxd全部
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}
			for (int i = 0; i < NPD; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//绕行段编号
				str = strText;
				Report_input.PutCollect(1, str);

				str = PDB[i].GH;
				Report_input.PutCollect(2, str);
				Report_input.PutCollect(3, PDB[i].ml);
				Report_input.PutCollect(4, PDB[i].Level);
				Report_input.PutCollect(5, PDB[i].degree);
				Report_input.PutCollect(6, PDB[i].length);
				Report_input.PutCollect(7, PDB[i].Rshu);
				Report_input.PutCollect(8, PDB[i].RLen);
				str = PDB[i].Notes;
				Report_input.PutCollect(9, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (PDB) delete[]PDB;
		PDB = NULL;
		Report_input.Close();
	}
	else if (TabFname == "左线绕行段大中桥" || TabFname == "右线绕行段大中桥")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "绕行段编号 INTEGER,";
			strSQL += "桥梁名称 VARCHAR,";
			strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "起点里程冠号 VARCHAR,";
			strSQL += "起点里程数 DOUBLE PRECISION,";
			strSQL += "终点里程冠号 VARCHAR,";
			strSQL += "终点里程数 DOUBLE PRECISION,";
			strSQL += "孔跨式样 VARCHAR,";
			strSQL += "线数 VARCHAR,";
			strSQL += "桥全长 DOUBLE PRECISION,";
			strSQL += "100年水位高程 VARCHAR,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 绕行段编号=%d", TabFname, irxd);//删除irxd全部
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//绕行段编号
				str = strText;
				Report_input.PutCollect(1, str);

				str = Bri[i].BriName;
				Report_input.PutCollect(2, str);
				str = Bri[i].CGH;
				Report_input.PutCollect(3, str);
				Report_input.PutCollect(4, Bri[i].CenDml);
				str = Bri[i].BGH;
				Report_input.PutCollect(5, str);
				Report_input.PutCollect(6, Bri[i].StDml);
				str = Bri[i].EGH;
				Report_input.PutCollect(7, str);
				Report_input.PutCollect(8, Bri[i].EdDml);
				str = Bri[i].KKSY;
				Report_input.PutCollect(9, str);

				str = "1";//线数(战时)
				Report_input.PutCollect(10, str);
				Report_input.PutCollect(11, Bri[i].BriLen);
				str = Bri[i].SWHt;
				Report_input.PutCollect(12, str);
			}

			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (Bri) delete[]Bri;
		Bri = NULL;
		Report_input.Close();
	}
	else if (TabFname == "左线绕行段隧道" || TabFname == "右线绕行段隧道")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "绕行段编号 INTEGER,";
			strSQL += "隧道名称 VARCHAR,";
			strSQL += "起点里程冠号 VARCHAR,";
			strSQL += "起点里程数 DOUBLE PRECISION,";
			strSQL += "终点里程冠号 VARCHAR,";
			strSQL += "终点里程数 DOUBLE PRECISION,";
			strSQL += "插旗方式 VARCHAR,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 绕行段编号=%d", TabFname, irxd);//删除irxd全部
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}

			for (int i = 0; i < TunNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//绕行段编号
				str = strText;
				Report_input.PutCollect(1, str);

				str = Tun[i].TUNName;
				Report_input.PutCollect(2, str);
				str = Tun[i].BGH;
				Report_input.PutCollect(3, str);
				Report_input.PutCollect(4, Tun[i].StDml);
				str = Tun[i].EGH;
				Report_input.PutCollect(5, str);
				Report_input.PutCollect(6, Tun[i].EdDml);
				str = Tun[i].flag;
				Report_input.PutCollect(7, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (Tun) delete[]Tun;
		Tun = NULL;
		Report_input.Close();
	}
	else if (TabFname == "左线绕行段填挖表" || TabFname == "右线绕行段填挖表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "绕行段编号 INTEGER,";
			strSQL += "加桩里程冠号 VARCHAR,";
			strSQL += "加桩里程数 DOUBLE PRECISION,";
			strSQL += "地面高程 DOUBLE PRECISION, ";
			strSQL += "设计坡度 DOUBLE PRECISION, ";
			strSQL += "路肩设计高程 DOUBLE PRECISION,";
			strSQL += "竖曲线纵距 DOUBLE PRECISION,";
			strSQL += "中心抬降值 DOUBLE PRECISION,";
			strSQL += "中心填挖高度 DOUBLE PRECISION,";
			strSQL += "路肩施工高程 DOUBLE PRECISION,";
			strSQL += "曲线加宽 DOUBLE PRECISION,";
			strSQL += "路肩施工宽度 DOUBLE PRECISION,";
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 绕行段编号=%d", TabFname, irxd);//删除irxd全部
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"数据库没有打开或已经关闭!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"没有记录!");
				//		return 0;
			}


			for (int i = 0; i < NTW; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//绕行段编号
				str = strText;
				Report_input.PutCollect(1, str);

				str = TWB[i].GH;
				Report_input.PutCollect(2, str);
				Report_input.PutCollect(3, TWB[i].ml);
				Report_input.PutCollect(4, TWB[i].DMGC);
				Report_input.PutCollect(5, TWB[i].SJPD);
				Report_input.PutCollect(6, TWB[i].SJGC);
				Report_input.PutCollect(7, TWB[i].SQXZJ);
				Report_input.PutCollect(8, TWB[i].ZXTJ);
				Report_input.PutCollect(9, TWB[i].ZXTWG);
				Report_input.PutCollect(10, TWB[i].LJSGG);
				Report_input.PutCollect(11, TWB[i].ZQXJK);
				Report_input.PutCollect(12, TWB[i].YQXJK);
				Report_input.PutCollect(13, TWB[i].LJSGK);
				Report_input.PutCollect(14, TWB[i].ZXJJ);
				Report_input.PutCollect(15, TWB[i].YXJJ);
				Report_input.PutCollect(16, TWB[i].LJFLK);
				Report_input.PutCollect(17, TWB[i].DMinf);
				str = TWB[i].FZ;
				Report_input.PutCollect(18, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		if (TWB) delete[]TWB;
		TWB = NULL;
		Report_input.Close();
	}

	m_adoConnection.Disconnect();
	return 1;
}


int XLDataBase_JYX::Read_EX_XLDbs(CString mdbFname, CString TabFname, int irxd)//读取线路数据库中的二线数据
{
	CString strText, GH;
	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"失败  4!");
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	//	m_strSQL ="select * from "+TabFname;
	m_strSQL.Format(L"select * from %s where 绕行段编号=%d order by ID", TabFname, irxd);//删除irxd全部
	//		 m_adoConnection.Execute(strSQL);
	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"数据库没有打开或已经关闭!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			ads_printf(L"%s没有记录!\n", TabFname);
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();
		if (TabFname == "左线绕行段隧道" || TabFname == "右线绕行段隧道")
		{
			int i = 0;
			TunNum = Report_input.GetRowCount();

			if (Tun) delete[]Tun;
			if (TunNum > 0)
				Tun = new TUNTAB[TunNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Tun[i].TUNName, 2);
				Report_input.GetValueString(Tun[i].BGH, 3);
				Report_input.GetValueString(strText, 4);
				Tun[i].StDml = _wtof(strText);
				Report_input.GetValueString(Tun[i].EGH, 5);
				Report_input.GetValueString(strText, 6);
				Tun[i].EdDml = _wtof(strText);

				dmltockml(Tun[i].BGH, Tun[i].StDml, Tun[i].SKml);
				dmltockml(Tun[i].EGH, Tun[i].EdDml, Tun[i].EKml);
				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "左线绕行段大中桥" || TabFname == "右线绕行段大中桥")
		{
			int i = 0;
			BriNum = Report_input.GetRowCount();
			if (Bri) delete[]Bri;
			if (BriNum > 0)
				Bri = new BRITAB[BriNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Bri[i].BriName, 2);
				Report_input.GetValueString(Bri[i].CGH, 3);
				Report_input.GetValueString(strText, 4);
				Bri[i].CenDml = _wtof(strText);
				Report_input.GetValueString(Bri[i].BGH, 5);
				Report_input.GetValueString(strText, 6);
				Bri[i].StDml = _wtof(strText);
				Report_input.GetValueString(Bri[i].EGH, 7);
				Report_input.GetValueString(strText, 8);
				Bri[i].EdDml = _wtof(strText);
				Report_input.GetValueString(Bri[i].KKSY, 9);
				Report_input.GetValueString(Bri[i].SWHt, 10);
				int pos = Bri[i].SWHt.Find(L".");
				if (pos > 0)
					Bri[i].SWHt = Bri[i].SWHt.Left(pos + 3);

				dmltockml(Bri[i].CGH, Bri[i].CenDml, Bri[i].CKml);
				dmltockml(Bri[i].BGH, Bri[i].StDml, Bri[i].SKml);
				dmltockml(Bri[i].EGH, Bri[i].EdDml, Bri[i].EKml);
				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "左线绕行段坡度表" || TabFname == "右线绕行段坡度表")
		{
			int i = 0;
			NPD = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].GH, "变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == "左线绕行段填挖表" || TabFname == "右线绕行段填挖表")
		{
			int i = 0;
			NTW = Report_input.GetRowCount();
			if (TWB) delete[]TWB;
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(TWB[i].GH, "加桩里程冠号");
				Report_input.GetValueString(strText, L"加桩里程数");
				TWB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"地面高程");
				TWB[i].DMGC = _wtof(strText);
				Report_input.MoveNext();
				//			ads_printf(L"i=%d,ml=%lf Level=%lf \n",i,TWB[i].ml,TWB[i].DMGC);
				i++;
			}
			NTW = i;
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	return 1;

}


void XLDataBase_JYX::GetJDNumArrayFromBLCalTab(CString mdbFname, CString TabFname, CArray<CString, CString>& GetJDNumArray)//读取线路数据库中的数据
{
	CString strText, GH;
	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64

		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			//         ads_printf(L"连接%s数据库失败!\n",mdbFname);
			//			AfxMessageBox(L"连接数据库失败!");			
			//	return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from " + TabFname;
	m_strSQL += L" order by 编号";
	///////////////////////

	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"数据库没有打开或已经关闭!");
			//	return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		//		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		//	    {
		//          ads_printf(L"没有记录!");
		//			NPD=NHDM=0;
		//			return 0;
		//return false;
		//Report_input.Close();
		//       }
		Report_input.Open(m_strSQL);

		//   Report_input.MoveFirst();


		/////////////////////////////////////////////////////////////////////////////
		////既有铁路改建   //  读数据
		/////////////////////////////////////////////////////////////////////////////

		CArray<CString, CString>TempGetJDNumArray;

		TempGetJDNumArray.RemoveAll();
		GetJDNumArray.RemoveAll();

		int i = 0;
		NOrigMeaNum = Report_input.GetRowCount();

		Report_input.MoveFirst();
		_variant_t vt;
		for (i = 0; i < NOrigMeaNum; i++)
		{
			Report_input.GetValueString(strText, L"交点号");

			vt = strText;
			Report_input.PutCollect(1, vt);
			TempGetJDNumArray.Add(strText);
			Report_input.MoveNext();
		}


		int nCurrentRow;
		bool flag;
		for (nCurrentRow = 0; nCurrentRow < NOrigMeaNum; nCurrentRow++)
		{
			flag = FALSE;
			for (int j = 0; j < GetJDNumArray.GetSize(); j++)
			{
				if (TempGetJDNumArray[nCurrentRow] == GetJDNumArray[j])
				{
					flag = TRUE;
					break;
				}
			}
			if (flag)continue;
			else GetJDNumArray.Add(TempGetJDNumArray[nCurrentRow]);
		}


	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	//	return 1;

	}


double XLDataBase_JYX::FromCStringTodouble(CString strText)
{
	double a;

	strText.Trim();
	if (strText.IsEmpty() || strText == "NULL")
		a = -100000;
	else
		a = _wtof(strText);
	return a;
}


int XLDataBase_JYX::WR_EX_TWB(CString mdbFname, CString TabFname, int irxd)//写入线路数据库中的数据
{
	CString strText, GH, strSQL;

	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"失败 5!");
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}
	_variant_t str;
	InitTableList();

	if (!FindTable(TabFname))//表不存在
	{
		CString strSQL;
		strSQL = "create table ";
		strSQL += TabFname;
		strSQL += "(";
		strSQL += "ID INTEGER IDENTITY,";
		strSQL += "绕行段编号 INTEGER,";
		strSQL += "加桩里程冠号 VARCHAR,";
		strSQL += "加桩里程数 DOUBLE PRECISION,";
		strSQL += "地面高程 DOUBLE PRECISION, ";
		strSQL += "设计坡度 DOUBLE PRECISION, ";
		strSQL += "路肩设计高程 DOUBLE PRECISION,";
		strSQL += "竖曲线纵距 DOUBLE PRECISION,";
		strSQL += "中心抬降值 DOUBLE PRECISION,";
		strSQL += "中心填挖高度 DOUBLE PRECISION,";
		strSQL += "路肩施工高程 DOUBLE PRECISION,";
		strSQL += "曲线加宽 DOUBLE PRECISION,";
		strSQL += "右曲线加宽 DOUBLE PRECISION,";
		strSQL += "路肩施工宽度 DOUBLE PRECISION,";
		strSQL += "左线间距 DOUBLE PRECISION,";
		strSQL += "右线间距 DOUBLE PRECISION,";
		strSQL += "路肩分离限宽 DOUBLE PRECISION,";
		strSQL += "断面信息 VARCHAR,";
		strSQL += "附注 VARCHAR";
		strSQL += ")";
		m_adoConnection.Execute(strSQL);//创建表
	}
	strSQL.Format(L"delete from %s where 绕行段编号=%d", TabFname, irxd);//删除irxd全部
	m_adoConnection.Execute(strSQL);

	m_strSQL = L"select * from " + TabFname;
	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"数据库没有打开或已经关闭!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);
		Report_input.Open(m_strSQL, adCmdText);
		for (int i = 0; i < NTW; i++)
		{
			Report_input.AddNew();
			strText.Format(L"%d", i);//ID
			str = strText;
			Report_input.PutCollect(0, str);

			strText.Format(L"%d", irxd);//绕行段编号
			str = strText;
			Report_input.PutCollect(1, str);

			str = TWB[i].GH;
			Report_input.PutCollect(2, str);
			Report_input.PutCollect(3, TWB[i].ml);
			Report_input.PutCollect(4, TWB[i].DMGC);
			Report_input.PutCollect(5, TWB[i].SJPD);
			Report_input.PutCollect(6, TWB[i].SJGC);
			Report_input.PutCollect(7, TWB[i].SQXZJ);
			Report_input.PutCollect(8, TWB[i].ZXTJ);
			Report_input.PutCollect(9, TWB[i].ZXTWG);
			Report_input.PutCollect(10, TWB[i].LJSGG);
			Report_input.PutCollect(11, TWB[i].ZQXJK);
			Report_input.PutCollect(12, TWB[i].YQXJK);
			if (fabs(TWB[i].LJSGK) > 0.00001)
				Report_input.PutCollect(13, TWB[i].LJSGK);
			Report_input.PutCollect(14, TWB[i].ZXJJ);
			Report_input.PutCollect(15, TWB[i].YXJJ);
			Report_input.PutCollect(16, TWB[i].LJFLK);
			strText.Format(L"%d", TWB[i].DMinf);//ID
			str = strText;
			Report_input.PutCollect(17, str);
			str = TWB[i].FZ;
			Report_input.PutCollect(18, str);

			//	 ads_printf(L"i=%d %lf %lf\n",i,TWB[i].ml,TWB[i].DMGC);
		}
		Report_input.Update();
	}
	catch (_com_error e)
	{
	}
	if (TWB)
	{
		delete[]TWB;
		TWB = NULL;
	}

	Report_input.Close();
	m_adoConnection.Disconnect();
	return 1;
}


//void XLDataBase_JYX::GetSlopeData(CString NameOfMDB,GTZX_JYX *pGTZX,CDrawSlope * pLeftSlope,CDrawSlope * pRightSlope,CString XLName)
//{
//	//1.读贯通设计基线的坡度，构建贯通设计基线指针
//
//	CString strNameOfWay1="线路名=\""+XLName+"\"";
//
//	if(Read_XLDbs(NameOfMDB,"坡度表",strNameOfWay1))
//	{
//		PDTAB temp;
//		char GH[8];
//		for (int i=0;i<NPD;i++)
//		{
//			_tcscpy(GH,PDB[i].GH);
//			_tcscpy(temp.CKml,LCchr(GH,PDB[i].ml,3,1));
//
//			temp.ml=pGTZX->TYLC1(temp.CKml,pGTZX->NDL,pGTZX->DLB);
//			temp.Level=PDB[i].Level;
//			temp.degree=PDB[i].degree/1000.0;
//			temp.length=PDB[i].length;
//			temp.Rshu=PDB[i].Rshu;
//			temp.RLen=PDB[i].RLen;
//			temp.Notes=PDB[i].Notes;
//			pLeftSlope->m_SlopeValue.Add(temp);
//			pRightSlope->m_SlopeValue.Add(temp);
//		}
//	}
//
//	XLName="贯通设计基线";
//	//2.读改建坡度
//	CString strNameOfDel="(改建性质=\"";
//	strNameOfDel += "并行等高改坡\"" ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp="改建性质=\"";
//	strTemp += "贯通设计基线改坡\"" ;
//	strNameOfDel += strTemp ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp1="改建性质=\"";
//	strTemp1 += "贯通设计二线改坡\")" ;
//	strNameOfDel += strTemp1 ;
//
//	strNameOfDel += " AND " ;
//	CString  strNameOfWay="改建名=\""+ XLName +"\"";;
//	strNameOfDel += strNameOfWay ;
//
//	CArray<CString,CString>strZPGJ;
//	CString strXLName;
//	if(Read_XLDbs(NameOfMDB,"改建段汇总表",strNameOfDel))//读改建段数据
//	{
//		for(int i=0;i<GJDAllInforNum;i++)
//		{
//			strXLName.Format(L"%s",GJDAllInforTable[i].ToOutSGJJDNum);
//			strZPGJ.Add(strXLName);
//		}
//	}
//
//
//
//	MPDTAB m_MPD;
//	APDTAB m_APD;
//
//	m_MPD.SetSize(strZPGJ.GetSize());
//	for (int i=0;i<strZPGJ.GetSize();i++)
//	{
//		CString strNameOfWay = strZPGJ.GetAt(i); 
//		strNameOfWay="线路名=\""+strNameOfWay+"\"";
//		Read_XLDbs(NameOfMDB,"坡度表",strNameOfWay);
//		PDTAB tempPDTAB;
//
//		char strTemp[80];
//		char GH[8];
//		for (int j=0;j<NPD;j++)
//		{
//			tempPDTAB.NameOfWay=PDB[j].NameOfWay;
//			//_stprintf(strTemp,"%s%lf",PDB[j].GH,PDB[j].ml);
//			_tcscpy(GH,PDB[j].GH);
//			_tcscpy(tempPDTAB.CKml,LCchr(GH,PDB[j].ml,3,1));
//			tempPDTAB.ml=TYLC1(tempPDTAB.CKml,pGTZX->NDL,pGTZX->DLB);
//			tempPDTAB.Level=PDB[j].Level;
//			tempPDTAB.degree=PDB[j].degree/1000.0;
//			tempPDTAB.length=PDB[j].length;
//			tempPDTAB.Rshu=PDB[j].Rshu;
//			tempPDTAB.RLen=PDB[j].RLen;
//			tempPDTAB.Notes=PDB[j].Notes;
//			m_APD.Add(tempPDTAB);
//		}
//		m_MPD.GetAt(i) = m_APD;
//		m_APD.RemoveAll();
//	}
//
//
//
//	//3.修正左、右线坡度
//	for (int i=0;i<m_MPD.GetSize();i++)
//	{
//		m_APD.RemoveAll();
//		m_APD = m_MPD.GetAt(i);
//
//
//		//3.1找到要替换的起始坡度号
//
//		double dtmp1 = m_APD[0].ml;
//		double dtmp2 = m_APD[m_APD.GetSize()-1].ml;
//
//		int StNumLeft,EdNumLeft;
//		int StNumRight,EdNumRight;
//
//		int flag;
//		if (m_APD[1].Notes=="并行等高改坡")
//		{
//			flag=0;
//			StNumLeft = pLeftSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumLeft = pLeftSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//			StNumRight = pRightSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumRight = pRightSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		}
//		else if (m_APD[1].Notes=="贯通设计基线改坡")
//		{
//			flag=1;
//			StNumLeft = pLeftSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumLeft = pLeftSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		}
//		else 
//		{
//			flag=2;
//			StNumRight = pRightSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumRight = pRightSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		}
//
//
//		//3.2.remove StNum<Num<EdNum+1 的变坡点aa
//		int len=0;
//		if (flag==0)
//		{
//			len = EdNumLeft - StNumLeft;
//			pLeftSlope->m_SlopeValue.RemoveAt(StNumLeft+1,len);
//			len = EdNumRight - StNumRight;
//			pRightSlope->m_SlopeValue.RemoveAt(StNumRight+1,len);
//		}
//		else if (flag==1)
//		{
//			len = EdNumLeft - StNumLeft;
//			pLeftSlope->m_SlopeValue.RemoveAt(StNumLeft+1,len);
//		}
//		else
//		{
//			len = EdNumRight - StNumRight;
//			pRightSlope->m_SlopeValue.RemoveAt(StNumRight+1,len);
//		}
//
//
//		//3.3.add m_APD的0~Size-1的变坡点
//		for (int k=0;k<m_APD.GetSize();k++)
//		{
//			if (flag==0)
//			{
//				pLeftSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//				pRightSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//			else if (flag==1)
//			{
//				pLeftSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//			else
//			{
//				pRightSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//		}
//
//		//3.4按里程排序
//		pLeftSlope->Ordel(pLeftSlope->m_SlopeValue);
//		pRightSlope->Ordel(pRightSlope->m_SlopeValue);
//
//
//		//Zhujiang-剔除重复的坡度数据 20090618
//		pLeftSlope->DeleteRepeatData(pLeftSlope->m_SlopeValue);
//		pRightSlope->DeleteRepeatData(pRightSlope->m_SlopeValue);
//	}
//
//
//	for (int i=0;i<pLeftSlope->m_SlopeValue.GetSize();i++)
//	{
//		pLeftSlope->m_SlopeValue[i].NameOfWay = "贯通设计基线";
//	}
//
//	for (int i=0;i<pRightSlope->m_SlopeValue.GetSize();i++)
//	{
//		pRightSlope->m_SlopeValue[i].NameOfWay = "贯通设计Ⅱ线";
//	}
//
//
//
//}


//void XLDataBase::GetSlopeData(CString NameOfMDB,GTZX *pGTZX,CDrawSlope * pLeftSlope,CDrawSlope * pRightSlope,CString XLName)
//{
//	GetSlopeData_New(NameOfMDB,pGTZX,pLeftSlope,pRightSlope,XLName);
//	return;
//	//1.读贯通设计基线的坡度，构建贯通设计基线指针
//
//	CString strNameOfWay1="线路名=\""+XLName+"\"";
//
//	if(Read_XLDbs(NameOfMDB,"坡度表",strNameOfWay1))
//	{
//		PDTAB temp;
//		char GH[8];
//		for (int i=0;i<NPD;i++)
//		{
//			_tcscpy(GH,PDB[i].GH);
//			_tcscpy(temp.CKml,LCchr(GH,PDB[i].ml,3,1));
//
//			temp.ml=pGTZX->TYLC1(temp.CKml,pGTZX->NDL,pGTZX->DLB);
//			temp.Level=PDB[i].Level;
//			temp.degree=PDB[i].degree/1000.0;
//			temp.length=PDB[i].length;
//			temp.Rshu=PDB[i].Rshu;
//			temp.RLen=PDB[i].RLen;
//			temp.Notes=PDB[i].Notes;
//			pLeftSlope->m_SlopeValue.Add(temp);
//			pRightSlope->m_SlopeValue.Add(temp);
//		}
//	}
//
//    XLName="贯通设计基线";
//	//2.读改建坡度
//	CString strNameOfDel="(改建性质=\"";
//	strNameOfDel += "并行等高改坡\"" ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp="改建性质=\"";
//	strTemp += "贯通设计基线改坡\"" ;
//	strNameOfDel += strTemp ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp1="改建性质=\"";
//	strTemp1 += "贯通设计二线改坡\")" ;
//	strNameOfDel += strTemp1 ;
//
//	strNameOfDel += " AND " ;
//	CString  strNameOfWay="改建名=\""+ XLName +"\"";;
//	strNameOfDel += strNameOfWay ;
//
//	CArray<CString,CString>strZPGJ;
//	CString strXLName;
//	if(Read_XLDbs(NameOfMDB,"改建段汇总表",strNameOfDel))//读改建段数据
//	{
//		for(int i=0;i<GJDAllInforNum;i++)
//		{
//			strXLName.Format(L"%s",GJDAllInforTable[i].ToOutSGJJDNum);
//			strZPGJ.Add(strXLName);
//		}
//	}
//
//	
//
//	MPDTAB m_MPD;
//	APDTAB m_APD;
//
//	m_MPD.SetSize(strZPGJ.GetSize());
//	for (int i=0;i<strZPGJ.GetSize();i++)
//	{
//		CString strNameOfWay = strZPGJ.GetAt(i); 
//		strNameOfWay="线路名=\""+strNameOfWay+"\"";
//		Read_XLDbs(NameOfMDB,"坡度表",strNameOfWay);
//		PDTAB tempPDTAB;
//
//		char strTemp[80];
//		char GH[8];
//		for (int j=0;j<NPD;j++)
//		{
//			tempPDTAB.NameOfWay=PDB[j].NameOfWay;
//			//_stprintf(strTemp,"%s%lf",PDB[j].GH,PDB[j].ml);
//			_tcscpy(GH,PDB[j].GH);
//			_tcscpy(tempPDTAB.CKml,LCchr(GH,PDB[j].ml,3,1));
//			tempPDTAB.ml=TYLC1(tempPDTAB.CKml,pGTZX->NDL,pGTZX->DLB);
//			tempPDTAB.Level=PDB[j].Level;
//			tempPDTAB.degree=PDB[j].degree/1000.0;
//			tempPDTAB.length=PDB[j].length;
//			tempPDTAB.Rshu=PDB[j].Rshu;
//			tempPDTAB.RLen=PDB[j].RLen;
//			tempPDTAB.Notes=PDB[j].Notes;
//			m_APD.Add(tempPDTAB);
//		}
//		m_MPD.GetAt(i) = m_APD;
//		m_APD.RemoveAll();
//	}
//
//
//
//	//3.修正左、右线坡度
//	for (int i=0;i<m_MPD.GetSize();i++)
//	{
//		m_APD.RemoveAll();
//		m_APD = m_MPD.GetAt(i);
//
//
//		//3.1找到要替换的起始坡度号
//
//		double dtmp1 = m_APD[0].ml;
//		double dtmp2 = m_APD[m_APD.GetSize()-1].ml;
//
//
//
//		int StNum=pLeftSlope->FindSlopeNum(m_APD[0].ml);
//		int EdNum=pLeftSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		int flag;
//		if (m_APD[1].Notes=="并行等高改坡")
//		{
//			flag=0;
//		}
//		else if (m_APD[1].Notes=="贯通设计基线改坡")
//		{
//			flag=1;
//		}
//		else 
//		{
//			flag=2;
//		}
//
//		
//		//3.2.remove StNum<Num<EdNum+1 的变坡点aa
//		int len=EdNum-StNum;
//		if (flag==0)
//		{
//			pLeftSlope->m_SlopeValue.RemoveAt(StNum+1,len);
//			pRightSlope->m_SlopeValue.RemoveAt(StNum+1,len);
//		}
//		else if (flag==1)
//		{
//			pLeftSlope->m_SlopeValue.RemoveAt(StNum+1,len);
//		}
//		else
//		{
//
//			//pRightSlope->m_SlopeValue.RemoveAt(StNum+1,len + 1);
//			//Zhujiang-多替换一个坡度
//			//if (StNum + len + 2 < pRightSlope->m_SlopeValue.GetSize())
//			//	pRightSlope->m_SlopeValue.RemoveAt(StNum + 1,len + 2);
//			//else
//				pRightSlope->m_SlopeValue.RemoveAt(StNum+1,len);
//
//		}
//
//				
//
//
//		//3.3.add m_APD的0~Size-1的变坡点
//		for (int k=0;k<m_APD.GetSize();k++)
//		{
//			if (flag==0)
//			{
//				pLeftSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//				pRightSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//			else if (flag==1)
//			{
//				pLeftSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//			else
//			{
//				pRightSlope->m_SlopeValue.Add(m_APD.GetAt(k));
//			}
//		}
//
//		//3.4按里程排序
//		pLeftSlope->Ordel(pLeftSlope->m_SlopeValue);
//		pRightSlope->Ordel(pRightSlope->m_SlopeValue);
//
//
//		//Zhujiang-剔除重复的坡度数据 20090618
//		pLeftSlope->DeleteRepeatData(pLeftSlope->m_SlopeValue);
//		pRightSlope->DeleteRepeatData(pRightSlope->m_SlopeValue);
//	}
//
//
//	for (int i=0;i<pLeftSlope->m_SlopeValue.GetSize();i++)
//	{
//		pLeftSlope->m_SlopeValue[i].NameOfWay = "贯通设计基线";
//	}
//
//	for (int i=0;i<pRightSlope->m_SlopeValue.GetSize();i++)
//	{
//		pRightSlope->m_SlopeValue[i].NameOfWay = "贯通设计Ⅱ线";
//	}
//
//
//
//}


void XLDataBase_JYX::GetRxdXLNameArray(CString mdbFname, bool IsJY, int ZorY, CArray<CString, CString>& RxdXLNameArray)//获取绕行段线路名数组
{
	CString strTemp;
	CString strNameOfWay;
	CString SQLNameOfWay;
	if (IsJY)
	{
		if (ZorY == -1)
			strNameOfWay = "既有左线";
		else
			strNameOfWay = "既有右线";
	}
	else
		strNameOfWay = "贯通设计Ⅱ线";

	SQLNameOfWay = L"线路名=\"" + strNameOfWay + "\"";

	if (Read_XLDbs(mdbFname, "二线断链表", SQLNameOfWay))//读改建段数据
	{
		for (int i = 0; i < NDL2x; i++)
		{
			strTemp.Format(L"%s", DLB2x[i].RXDName);
			if ((strTemp != "") && (strTemp != "NULL"))
			{
				if (i > 0)
				{
					if (_tcscmp(strTemp, DLB2x[i - 1].RXDName) != 0)
					{
						RxdXLNameArray.Add(strTemp);
					}
				}
				else
				{
					RxdXLNameArray.Add(strTemp);
				}
			}
		}
	}
}

void XLDataBase_JYX::GetRxdStartAndEndCKMLByRXDName(CString mdbFname, bool IsJY, int ZorY, CString RxdXLName, CString& StartCKML, CString& EndCKML)//获取绕行段起终现场里程
{
	CString strTemp;
	CString strNameOfWay;
	CString SQLNameOfWay;
	if (IsJY)
	{
		if (ZorY == -1)
			strNameOfWay = "既有左线";
		else
			strNameOfWay = "既有右线";
	}
	else
		strNameOfWay = "贯通设计Ⅱ线";

	SQLNameOfWay = L"线路名=\"" + strNameOfWay + "\"";
	SQLNameOfWay += L" and ";
	SQLNameOfWay += L"绕行段名=\"";
	SQLNameOfWay += RxdXLName;
	SQLNameOfWay += "\"";

	if (Read_XLDbs(mdbFname, "二线断链表", SQLNameOfWay))//读改建段数据
	{
		CString str;
		str.Format(L"%lf", DLB2x[0].ELC);
		LCChr(DLB2x[0].EGH, str, 3, StartCKML);

		str.Format(L"%lf", DLB2x[NDL2x - 1].BLC);
		LCChr(DLB2x[NDL2x - 1].BGH, str, 3, EndCKML);
	}
}

//////////////////////////////////////////////////////////////////////////
//读取用地面积表
int XLDataBase_JYX::Read_YD_XLDbs(CString mdbFname, CString TabName, CString XLName)//读取线路数据库中的用地数据
{
	CString rcd;
	AfxOleInit();

	if (AttachYDMdb(mdbFname, TabName, XLName) == 0)
		return 0;

	if (TabName == "用地面积汇总分段表")
	{
		int i = 0;
		if (pXZYD) delete[]pXZYD;
		pXZYD = NULL;
		NXZYD = Report_input.GetRecordCount();

		if (NXZYD > 0)
			pXZYD = new CXZYDFD[NXZYD];
		Report_input.MoveFirst();
		while (!Report_input.IsEOF())
		{

			Report_input.GetValueString(pXZYD[i].GH, "分段终里程冠号");
			Report_input.GetValueString(rcd, "分段终里程数");
			pXZYD[i].dml = _wtof(rcd);
			Report_input.GetValueString(pXZYD[i].BZText, "备注");
			Report_input.MoveNext();
			i++;
		}
	}
	else if (TabName == "用地类型表")
	{
		int i = 0;
		if (pYDType) delete[]pYDType;
		pYDType = NULL;
		NTypeFD = Report_input.GetRecordCount();

		if (NTypeFD > 0)
			pYDType = new CYDLXFD[NTypeFD];
		Report_input.MoveFirst();
		while (!Report_input.IsEOF())
		{
			Report_input.GetValueString(pYDType[i].GH, "分段终里程冠号");
			Report_input.GetValueString(rcd, "分段终里程数");
			pYDType[i].dml = _wtof(rcd);
			Report_input.GetValueString(pYDType[i].YDType, "用地类型");
			Report_input.MoveNext();
			i++;
		}
}

	Report_input.Close();
	m_adoConnection.Disconnect();

	return 1;
}


int XLDataBase_JYX::AttachYDMdb(CString mdbFname, CString TabName, CString XLName)
{
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="数据库.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"失败   6!");
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认Pm.mdb是否在当前路径下!");
	}

	m_strSQL = L"select * from " + TabName;
	m_strSQL += " where 线路名=\"";
	m_strSQL += XLName;
	m_strSQL += "\"";

	if (!m_adoConnection.IsOpen())
	{
		AfxMessageBox(L"数据库没有打开或已经关闭!");
		return 0;
	}
	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
		//        ads_printf(L"%s没有记录!\n",TabName);		
		Report_input.Close();
		m_adoConnection.Disconnect();
		return 0;
	}
	return 1;

}

CString XLDataBase_JYX::RXDInWhichXL(CString mdbFname, CString RXDName)//绕行段找到在某个线路上
{
	CString XLName = "";
	CString SQLNameOfWay = L"绕行段名=\"" + RXDName + "\"";
	CString strTemp = "";
	if (Read_XLDbs(mdbFname, "二线断链表", SQLNameOfWay))//读改建段数据
	{
		for (int i = 0; i < NDL2x; i++)
		{
			strTemp.Format(L"%s", DLB2x[i].XLName);
			return strTemp;
		}
	}

	return XLName;
}

int XLDataBase_JYX::GetRXDChaQi(CString mdbFname, CString strEXName, CString RXDName, CString TabFname)//检查二线插旗数据是否在绕行段内，剔除不在的、
{
	//1.构建二线指针，得到二线里程系统
	//if(strEXName=="")
	GTZX2_JYX_ROAD* p2x = PmPointer::CreateGTSJEXPointer(mdbFname);
	if (p2x == NULL)
	{
		return 0;
	}
	//GTZX *pGTZX = p2x->CreateGtzx();

	//2.得到绕行段的起终点统一里程
	int RXDNum = p2x->GetiRxdNoFromRXDName(RXDName);
	if (RXDNum < 0)
	{
		return 0;
	}
	int FirstDL, EndDL;
	p2x->GetiRXDDLinfo(RXDNum, FirstDL, EndDL);
	ACHAR Ckml[80];
	ACHAR GH[8];
	_tcscpy(GH, p2x->DLB2x[FirstDL].EGH);
	_tcscpy(Ckml, LCchr(GH, p2x->DLB2x[FirstDL].ELC, 3, 1));

	double sml = p2x->TYLC2x(Ckml);
	//double sml=p2x->TYLC2x(Ckml,RXDNum);

	_tcscpy(GH, p2x->DLB2x[EndDL].BGH);
	_tcscpy(Ckml, LCchr(GH, p2x->DLB2x[EndDL].BLC, 3, 1));


	//朱江 20090605修改,若不传入绕行段编号，则可能导致算出来的统一里程是个错值，导致桥梁画不出来
	double eml = p2x->TYLC2x(Ckml);
	//double eml=p2x->TYLC2x(Ckml,RXDNum);

	//3.得到插旗的统一里程,并判断是否在绕行段范围内,是则加入

	if (TabFname == "大中桥")
	{
		CArray<BRITAB, BRITAB>TempBri;
		BRITAB Temp;
		for (int i = 0; i < BriNum; i++)
		{
			Temp.NameOfWay = Bri[i].NameOfWay;

			_tcscpy(Temp.CKml, Bri[i].CKml);
			_tcscpy(Temp.SKml, Bri[i].SKml);
			_tcscpy(Temp.EKml, Bri[i].EKml);

			Temp.CGH = Bri[i].CGH;
			Temp.CenDml = Bri[i].CenDml;

			Temp.BriLen = Bri[i].BriLen;

			Temp.LineNum = Bri[i].LineNum;
			Temp.BriName = Bri[i].BriName;
			Temp.SWHt.Format(L"%s", Bri[i].SWHt);//百年水位
			Temp.KKSY.Format(L"%s", Bri[i].KKSY);//孔跨样式
			Temp.GJZT = Bri[i].GJZT;
			Temp.StDml = p2x->TYLC2x(Bri[i].SKml);
			Temp.EdDml = p2x->TYLC2x(Bri[i].EKml);
			if ((Temp.StDml > sml && Temp.StDml < eml) || (Temp.EdDml > sml && Temp.EdDml < eml))
			{
				TempBri.Add(Temp);
			}
		}
		if (Bri)
		{
			delete[]Bri;
			Bri = NULL;
			BriNum = 0;
		}
		if (TempBri.GetSize() > 0)
		{
			BriNum = TempBri.GetSize();
			Bri = new BRITAB[BriNum];
			for (int i = 0; i < BriNum; i++)
			{
				Bri[i].NameOfWay = TempBri[i].NameOfWay;
				_tcscpy(Bri[i].SKml, TempBri[i].SKml);
				_tcscpy(Bri[i].EKml, TempBri[i].EKml);
				Bri[i].LineNum = TempBri[i].LineNum;
				Bri[i].BriName = TempBri[i].BriName;
				Bri[i].SWHt.Format(L"%s", TempBri[i].SWHt);//百年水位
				Bri[i].KKSY.Format(L"%s", TempBri[i].KKSY);//孔跨样式
				Bri[i].GJZT = TempBri[i].GJZT;
				Bri[i].CGH = TempBri[i].CGH;
				Bri[i].CenDml = TempBri[i].CenDml;
				Bri[i].BriLen = TempBri[i].BriLen;
				_tcscpy(Bri[i].CKml, TempBri[i].CKml);
			}
		}

	}
	else if (TabFname == "隧道")
	{
		CArray<TUNTAB, TUNTAB>ExsitTun;
		TUNTAB TempTUNTAB;
		for (int i = 0; i < TunNum; i++)
		{
			TempTUNTAB.NameOfWay = Tun[i].NameOfWay;
			_tcscpy(TempTUNTAB.SKml, Tun[i].SKml);
			_tcscpy(TempTUNTAB.EKml, Tun[i].EKml);
			TempTUNTAB.TUNName = Tun[i].TUNName;
			TempTUNTAB.flag = Tun[i].flag;
			if (TempTUNTAB.flag == " " || TempTUNTAB.flag == "NULL")//如果没记录默认为1-一般隧道
			{
				Tun[i].flag.Format(L"%d", 1);
			}
			TempTUNTAB.NumOfLine = Tun[i].NumOfLine;
			TempTUNTAB.GJZT = Tun[i].GJZT;
			TempTUNTAB.StDml = p2x->TYLC2x(Tun[i].SKml);
			TempTUNTAB.EdDml = p2x->TYLC2x(Tun[i].EKml);
			if ((TempTUNTAB.StDml > sml && TempTUNTAB.StDml < eml) || (TempTUNTAB.EdDml > sml && TempTUNTAB.EdDml < eml))
			{
				ExsitTun.Add(TempTUNTAB);
			}
		}

		if (Tun)
		{
			delete[]Tun;
			Tun = NULL;
			TunNum = 0;
		}
		if (ExsitTun.GetSize() > 0)
		{
			TunNum = ExsitTun.GetSize();
			Tun = new TUNTAB[TunNum];
			for (int i = 0; i < TunNum; i++)
			{
				Tun[i].NameOfWay = ExsitTun[i].NameOfWay;
				_tcscpy(Tun[i].SKml, ExsitTun[i].SKml);
				_tcscpy(Tun[i].EKml, ExsitTun[i].EKml);
				Tun[i].TUNName = ExsitTun[i].TUNName;
				Tun[i].flag = ExsitTun[i].flag;
				Tun[i].NumOfLine = ExsitTun[i].NumOfLine;
				Tun[i].GJZT = ExsitTun[i].GJZT;
			}
		}
	}
	else if (TabFname == "小桥涵")
	{
		CArray<SMALLBRIDGETAB, SMALLBRIDGETAB>SmallBri;
		SMALLBRIDGETAB Temp;
		CString temp;
		for (int i = 0; i < SBriNum; i++)
		{
			Temp.NameOfWay = SBri[i].NameOfWay;
			Temp.GH = SBri[i].GH;
			Temp.ml = SBri[i].ml;
			//temp.Format(L"%s%lf",Temp.GH,Temp.ml);
			ACHAR GH[8];
			_tcscpy(GH, SBri[i].GH);
			_tcscpy(Temp.CKml, LCchr(GH, SBri[i].ml, 3, 1));
			Temp.KKStyle = SBri[i].KKStyle;
			Temp.dwL = SBri[i].dwL;
			Temp.dwR = SBri[i].dwR;
			Temp.LLevel = SBri[i].LLevel;
			Temp.RLevel = SBri[i].RLevel;
			Temp.JJiao = SBri[i].JJiao;
			Temp.ToUse = SBri[i].ToUse;
			Temp.InsertStyle = SBri[i].InsertStyle;
			Temp.GJXZ = SBri[i].GJXZ;
			double ml = p2x->TYLC2x(Temp.CKml);
			if (ml > sml && ml < eml)
			{
				SmallBri.Add(Temp);
			}
		}

		if (SBri)
		{
			delete[]SBri;
			SBri = NULL;
			SBriNum = 0;
		}
		if (SmallBri.GetSize() > 0)
		{
			SBriNum = SmallBri.GetSize();
			SBri = new SMALLBRIDGETAB[SBriNum];
			for (int i = 0; i < SBriNum; i++)
			{
				SBri[i].NameOfWay = SmallBri[i].NameOfWay;
				SBri[i].GH = SmallBri[i].GH;
				SBri[i].ml = SmallBri[i].ml;
				ACHAR GH[8];
				_tcscpy(GH, SmallBri[i].GH);
				_tcscpy(SBri[i].CKml, LCchr(GH, SmallBri[i].ml, 3, 1));
				SBri[i].KKStyle = SmallBri[i].KKStyle;
				SBri[i].dwL = SmallBri[i].dwL;
				SBri[i].dwR = SmallBri[i].dwR;
				SBri[i].LLevel = SmallBri[i].LLevel;
				SBri[i].RLevel = SmallBri[i].RLevel;
				SBri[i].JJiao = SmallBri[i].JJiao;
				SBri[i].ToUse = SmallBri[i].ToUse;
				SBri[i].InsertStyle = SmallBri[i].InsertStyle;
				SBri[i].GJXZ = SmallBri[i].GJXZ;
			}
		}

	}
	else if (TabFname == "车站")
	{
		CArray<STATAB, STATAB>Station;
		STATAB Temp;
		for (int i = 0; i < StaNum; i++)
		{
			Temp.NameOfWay = Sta[i].NameOfWay;
			Temp.StaName = Sta[i].StaName;
			Temp.CGH = Sta[i].CGH;
			Temp.CenDml = Sta[i].CenDml;
			ACHAR GH[8];
			_tcscpy(GH, Sta[i].CGH);
			_tcscpy(Temp.CKml, LCchr(GH, Temp.CenDml, 3, 1));
			Temp.CZLX = Sta[i].CZLX;
			Temp.CQTKM = Sta[i].CQTKM;
			Temp.GJXZ = Sta[i].GJXZ;
			double ml = p2x->TYLC2x(Temp.CKml);
			if (ml > sml && ml < eml)
			{
				Station.Add(Temp);
			}

			if (Sta)
			{
				delete[]Sta;
				Sta = NULL;
				StaNum = 0;
			}
			if (Station.GetSize() > 0)
			{
				StaNum = Station.GetSize();
				Sta = new STATAB[StaNum];
				for (int i = 0; i < StaNum; i++)
				{
					Sta[i].NameOfWay = Station[i].NameOfWay;
					Sta[i].StaName = Station[i].StaName;
					Sta[i].CGH = Station[i].CGH;
					Sta[i].CenDml = Station[i].CenDml;
					ACHAR GH[8];
					_tcscpy(GH, Station[i].CGH);
					_tcscpy(Sta[i].CKml, LCchr(GH, Station[i].CenDml, 3, 1));
					Sta[i].CZLX = Station[i].CZLX;
					Sta[i].CQTKM = Station[i].CQTKM;
					Sta[i].GJXZ = Station[i].GJXZ;
				}
			}
		}
	}


	if (p2x)delete p2x;
	p2x = NULL;

	return 1;
}

/*
读数据库曲线表判断线路是设计线还是既有线，既有线直接返回isEX=-1
如果是设计线，继续通过读左曲线表、右曲线表判断是不是二线
设计基线返回isEX=0 左线返回isEX=1 右线返回isEX=2
*/
int  XLDataBase_JYX::IsJYXMdb2(CString MdbName, int& isEX)
{
	int result = -1;
	CString ProMDB;
	CString Temp1;
	ACHAR fname[1024];
	isEX = 0;
	BAS_DRAW_FUN ob;
	//if(!ob.ReadWorkdir())
	//{
	//	AfxMessageBox(L"请指定工作目录和方案名称!");
	//	return false;
	//}
	// 	CString FilePath;
	// 	ProMDB = MdbName;
	// 	ProMDB.TrimRight();
	// 	ProMDB.MakeLower();	
	// 
	// 	FilePath = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + ProMDB;		
	// 	if(!m_adoConnection.Connect(FilePath))
	// 	{
	// 		AfxMessageBox(L"连接数据库"+ProMDB+"失败!");
	// 
	// 	}
	// 	Report_input.SetAdoConnection(&m_adoConnection);
	// 	InitTableList();
	// 	Temp1 = "贯通设计Ⅱ线";
	// 	Temp1.Trim();
	// 	Temp1="线路名=\"" + Temp1 + "\"";
	// 	if (Read_XLDbs(FilePath,"二线曲线表",Temp1))
	// 	{
	// 		if (NQX2x>0)
	// 		{
	// 
	// 			isEX=2;
	// 		}
	// 		result= 1;
	// 	}
	// 	else
	// 	{
	// 		result= 0;
	// 		isEX=0;
	// 	}
	isEX = 2;


	return result;

}