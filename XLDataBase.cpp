// XLDataBase.cpp: implementation of the XLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLDataBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XLDataBase::XLDataBase()
{
	pModifyRoad = NULL;
	ModifyRoadSum = 0;

	QXB2x = NULL;
	NQX2x = 0;

	DLB2x = NULL;
	NDL2x = 0;

	YDLXB = NULL;
	YDLXSum = 0;

	QXB = NULL;
	NQX = 0;

	JdCenArray = NULL;
	NJdCen = 0;

	DLArray = NULL;
	NDL = 0;

	ZDLArray = NULL;
	NZDL = 0;

	XLMan = NULL;//线路管理
	NXL = 0;

	ZQZJB = NULL;
	NDMX = 0;
	m_RoadSum = 0;//道路数
	for (int i = 0; i < 50; i++)
	{
		Road[i].RoadName = "";
		Road[i].BGH = "K";
		Road[i].EGH = "K";
		Road[i].BLC = Road[i].ELC = -100.0;
	} //最多50条路
	PDB = NULL;
	NPD = 0;
	JPB = NULL;
	JPnum = 0;
	Bri = NULL;
	BriNum = 0;
	Tun = NULL;
	TunNum = 0;
	TWB = NULL;
	NTW = 0;
	lmk = NULL;
	NLMK = 0;
	ppcd = NULL;
	NPPCD = 0;
	NuphData = 0;
	NJK = 0;
	STA = NULL;
	NSTA = 0;
	CUL = NULL;
	NCU = 0;
	BM = NULL;
	BmNum = 0;
	DX = NULL;
	DXNum = 0;
	pCtrlHt = NULL;
	CRNUM = 0;
	XYArray = NULL;
	XYNum = 0;
	ZDMBZ.MAXPD = L"";
	ZDMBZ.TRshu1 = L"";
	ZDMBZ.TRshu2 = L"";
	ZDMBZ.ARshu1 = L"";
	ZDMBZ.ARshu2 = L"";
	ZDMBZ.Lshu1 = L"";
	ZDMBZ.Lshu2 = L"";
	ZDMBZ.PCMin1 = L"";
	ZDMBZ.PCMin2 = L"";
	CalCGCs.CgLenMode = -1;
	CalCGCs.CgMode = -1;
	CalCGCs.m_HxCgJbl = L"";
	CalCGCs.m_ZxCgJbl = L"";
	Scml = -9999999;
	Ecml = 99999999;
	Hmin = 999999999;
	Hmax = -99999999;

	NCG = 0;	//20190710添加
}

XLDataBase::~XLDataBase()
{
	Release();
}
void XLDataBase::Release()
{
	if (pModifyRoad) { delete[]pModifyRoad; pModifyRoad = NULL; }
	ModifyRoadSum = 0;

	if (QXB2x) { delete[]QXB2x; QXB2x = NULL; }
	NQX2x = 0;

	if (DLB2x) { delete[]DLB2x; DLB2x = NULL; }
	NDL2x = 0;

	if (YDLXB) { delete[]YDLXB; YDLXB = NULL; }
	YDLXSum = 0;

	if (QXB) { delete[]QXB; QXB = NULL; }
	NQX = 0;

	if (JdCenArray) { delete[]JdCenArray; JdCenArray = NULL; }
	NJdCen = 0;

	if (DLArray) { delete[]DLArray; DLArray = NULL; }
	NDL = 0;

	if (XLMan) { delete[]XLMan; XLMan = NULL; }
	NXL = 0;

	if (ZQZJB) { delete[]ZQZJB; ZQZJB = NULL; }
	if (XYArray)
	{
		delete[]XYArray;
		XYArray = NULL;
	}

}

int XLDataBase::Read_XLDbs(CString mdbFname, CString TabFname)//读取线路数据库中的数据
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
			//	AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from " + TabFname;
	//m_strSQL += L" Order By ID";
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
			//   ads_printf(L"没有记录!");
			return 0;
			//return false;
			//Report_input.Close();
		}

		Report_input.MoveFirst();

		//////////////////////////////////////////////////////////////////////////
		//改移道路
		if (TabFname == L"改移道路表")
		{
			if (pModifyRoad) { delete[]pModifyRoad; pModifyRoad = NULL; }
			ModifyRoadSum = Report_input.GetRecordCount();
			pModifyRoad = new ModifyRoadTab[ModifyRoadSum];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"道路名");
				pModifyRoad[i].RoadName = strText;
				Report_input.GetValueString(strText, L"相交铁路名");
				pModifyRoad[i].RailName = strText;
				Report_input.GetValueString(strText, L"起点里程");
				pModifyRoad[i].SCkml = strText;
				Report_input.GetValueString(strText, L"铁路交叉里程");
				pModifyRoad[i].RailCkml = strText;
				Report_input.GetValueString(strText, L"铁路交叉高程");
				pModifyRoad[i].Raillevel = _wtof(strText);
				Report_input.GetValueString(strText, L"终点里程");
				pModifyRoad[i].ECkml = strText;
				Report_input.GetValueString(strText, L"公路交叉里程");
				pModifyRoad[i].RoadDml = _wtof(strText);
				Report_input.GetValueString(strText, L"改移道路长度");
				pModifyRoad[i].RoadLen = _wtof(strText);
				Report_input.GetValueString(strText, L"交叉角");
				pModifyRoad[i].InterAng = _wtof(strText);
				Report_input.GetValueString(strText, L"高差");
				pModifyRoad[i].DetaH = _wtof(strText);
				Report_input.GetValueString(strText, L"铁路在其上下方");
				pModifyRoad[i].UpDown = strText;
				Report_input.GetValueString(strText, L"孔跨结构");
				pModifyRoad[i].KKSY = strText;
				Report_input.GetValueString(strText, L"等级种类");
				pModifyRoad[i].RandType = strText;
				Report_input.GetValueString(strText, L"路基面宽");
				pModifyRoad[i].LjWide = _wtof(strText);
				Report_input.GetValueString(strText, L"路面宽");
				pModifyRoad[i].LmWide = _wtof(strText);
				Report_input.GetValueString(strText, L"路面厚");
				pModifyRoad[i].LmThick = _wtof(strText);
				Report_input.GetValueString(strText, L"路面材料");
				pModifyRoad[i].LmType = strText;
				Report_input.GetValueString(strText, L"路面工程数量");
				pModifyRoad[i].LmArea = _wtof(strText);
				Report_input.GetValueString(strText, L"填方");
				pModifyRoad[i].FillVol = _wtof(strText);
				Report_input.GetValueString(strText, L"挖方");
				pModifyRoad[i].CutVol = _wtof(strText);
				Report_input.GetValueString(strText, L"圬工");
				pModifyRoad[i].MasonryVol = _wtof(strText);
				Report_input.GetValueString(strText, L"用地");
				pModifyRoad[i].LandAreaStr = strText;
				Report_input.GetValueString(strText, L"附注");
				pModifyRoad[i].Note = strText;

				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == L"坡度表")//铁路坡度表
		{
			int i = 0;
			NPD = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();
			for (int i = 0; i < NPD; i++)
			{
				PDB[i].cml = 0.0;
				PDB[i].degree = 3;
				PDB[i].GH = L" ";
				PDB[i].length = 0;
				PDB[i].Level = 0;
				PDB[i].ml = 0;
				PDB[i].Notes = L" ";
				PDB[i].RLen = 0;
				PDB[i].Rshu = 0;
				_tcscpy(PDB[i].CKml, L" ");
			}
			i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _wtof(strText);
				Report_input.GetValueString(strText, L"坡度");
				PDB[i].degree = _wtof(strText);
				Report_input.GetValueString(strText, L"坡长");
				PDB[i].length = _wtof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"附注");
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == L"左线曲线表" || TabFname == L"右线曲线表")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, 1);
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 2);
				QXB2x[i].jdno = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 3);
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 4);
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, 5);
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, 6);
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, 7);
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, 8);
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, 9);
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左线断链表" || TabFname == L"右线断链表")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, 1);
				DLB2x[i].no = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(DLB2x[i].BGH, 2);

				Report_input.GetValueString(strText, 3);
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, 4);

				Report_input.GetValueString(strText, 5);
				DLB2x[i].ELC = _wtof(strText);

				Report_input.GetValueString(strText, 6);
				DLB2x[i].TYLC = _wtof(strText);

				Report_input.GetValueString(strText, 7);
				DLB2x[i].RXDName = strText;

				i++;
				Report_input.MoveNext();
			}

		}

		else if (TabFname == L"曲线表")
		{
			int i = 0;
			NQX = Report_input.GetRecordCount();
			if (QXB) { delete[]QXB; QXB = NULL; }
			if (NQX > 0)
				QXB = new QXTAB[NQX];

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"交点号");
				QXB[i].jdno = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				QXB[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				QXB[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"偏角");
				QXB[i].zj = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				QXB[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线");
				QXB[i].l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线");
				QXB[i].l2 = _wtof(strText);

				Report_input.GetValueString(strText, L"前切线长");
				QXB[i].T1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后切线长");
				QXB[i].T2 = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线长");
				QXB[i].L = _wtof(strText);

				Report_input.GetValueString(strText, L"起点里程冠号");
				QXB[i].BGH = strText;

				Report_input.GetValueString(strText, L"起点里程");
				QXB[i].Bml = _wtof(strText);

				Report_input.GetValueString(strText, L"终点里程冠号");
				QXB[i].EGH = strText;

				Report_input.GetValueString(strText, L"终点里程");
				QXB[i].Eml = _wtof(strText);

				Report_input.GetValueString(strText, L"内业断链");
				QXB[i].InDL = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"断链表")
		{
			int i = 0;
			NDL = Report_input.GetRecordCount();
			if (DLArray) { delete[]DLArray; DLArray = NULL; }
			if (NDL > 0)
				DLArray = new DLBZ[NDL];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"等号左里程冠号");
				DLArray[i].BGH = strText;
				Report_input.GetValueString(strText, L"等号右里程冠号");
				DLArray[i].EGH = strText;
				Report_input.GetValueString(strText, L"等号左里程数");
				DLArray[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"等号右里程数");
				DLArray[i].ELC = _wtof(strText);
				Report_input.GetValueString(strText, L"断链长度");
				DLArray[i].DL = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}

			double LCB = 0;  // LCB 断链的统一里程
			int iDL = 0;
			for (iDL = 0; iDL < NZDL; iDL++)
			{
				//该断链点统一里程
				if (iDL == 0)LCB = DLArray[iDL].ELC;
				else  LCB = LCB + DLArray[iDL].BLC - (DLArray[iDL - 1].ELC);
				DLArray[iDL].TYLC = LCB;
			}
		}

		else if (TabFname == L"线路属性")
		{

			Report_input.GetValueString(strText, L"公路等级");
			RoadXZ.Grade = strText;

			Report_input.GetValueString(strText, L"设计车速");
			RoadXZ.DV = _wtoi(strText);

			Report_input.GetValueString(strText, L"地段类别");
			RoadXZ.TerrainType = strText;

			Report_input.GetValueString(strText, L"地形类别");
			RoadXZ.DXLB = strText;

		}
		else if (TabFname == L"纵面设计标准")
		{
			Report_input.GetValueString(strText, L"最大纵坡");
			ZDMBZ.MAXPD = strText;

			Report_input.GetValueString(strText, L"凸竖曲线半径一般最小值");
			ZDMBZ.TRshu1 = strText;

			Report_input.GetValueString(strText, L"凸竖曲线半径极限最小值");
			ZDMBZ.TRshu2 = strText;

			Report_input.GetValueString(strText, L"凹竖曲线半径一般最小值");
			ZDMBZ.ARshu1 = strText;

			Report_input.GetValueString(strText, L"凹竖曲线半径极限最小值");
			ZDMBZ.ARshu2 = strText;

			Report_input.GetValueString(strText, L"竖曲线长度一般最小值");
			ZDMBZ.Lshu1 = strText;

			Report_input.GetValueString(strText, L"竖曲线长度极限最小值");
			ZDMBZ.Lshu2 = strText;

			Report_input.GetValueString(strText, L"坡长一般最小值");
			ZDMBZ.PCMin1 = strText;

			Report_input.GetValueString(strText, L"坡长极限最小值");
			ZDMBZ.PCMin2 = strText;

		}
		else if (TabFname == L"超高计算标准")
		{
			NuphData = Report_input.GetRecordCount();
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"半径");
				SuphData[i][0] = _wtof(strText);

				Report_input.GetValueString(strText, L"横坡");
				SuphData[i][1] = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}

		}
		else if (TabFname == L"加宽计算标准")
		{
			NJK = Report_input.GetRecordCount();
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"半径");
				JKData[i][0] = _wtof(strText);

				Report_input.GetValueString(strText, L"加宽");
				JKData[i][1] = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}

		}
		else if (TabFname == L"超高计算参数")
		{
			Report_input.GetValueString(strText, L"直线段超高渐变率");
			CalCGCs.m_ZxCgJbl = strText;

			Report_input.GetValueString(strText, L"超高缓和段长度限制");
			CalCGCs.CgLenMode = _wtoi(strText);

			Report_input.GetValueString(strText, L"缓和段最小超高渐变率");
			CalCGCs.m_HxCgJbl = strText;

			Report_input.GetValueString(strText, L"超高方式");
			CalCGCs.CgMode = _wtoi(strText);

			Report_input.GetValueString(strText, L"加宽方式");
			CalCGCs.JkMode = _wtoi(strText);
		}
		else if (TabFname == L"桩号序列")
		{
			if (STA) { delete[]STA; STA = NULL; }
			NSTA = Report_input.GetRecordCount();
			if (NSTA > 0)
			{
				STA = new LCBdata[NSTA];
				int i = 0;
				Report_input.MoveFirst();
				while (!Report_input.IsEOF())
				{
					//acedSetStatusBarProgressMeterPos(i/NDL*100);
					Report_input.GetValueString(strText, L"桩号");
					STA[i].lc = _wtof(strText);

					Report_input.GetValueString(strText, L"关联桩号");
					STA[i].gldmlc = _wtof(strText);

					Report_input.GetValueString(strText, L"关联道路");
					STA[i].glRoadName = strText;

					Report_input.MoveNext();
					i++;
				}
			}

		}
		else if (TabFname == L"线路管理")
		{
			if (XLMan) { delete[]XLMan; XLMan = NULL; }
			NXL = Report_input.GetRecordCount();
			XLMan = new XLManager[NXL];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"线路编号");
				XLMan[i].No = (int)(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"线路名称");
				XLMan[i].Name = strText;

				Report_input.GetValueString(strText, L"引出线路编号");
				XLMan[i].PreNo = (int)(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"引入线路编号");
				XLMan[i].NextNo = (int)(_wtof(strText) + 0.001);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"断链表" || TabFname == L"分离段断链表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);

			if (DLArray) { delete[]DLArray; DLArray = NULL; }
			NDL = Report_input.GetRecordCount();
			DLArray = new DLBZ[NDL];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				if (TabFname == L"分离段断链表")
				{
					Report_input.GetValueString(strText, L"分离段编号");
					DLArray[i].FLDNo = (int)(_wtof(strText));
				}
				else	DLArray[i].FLDNo = -1;
				Report_input.GetValueString(DLArray[i].BGH, L"等号左里程冠号");
				Report_input.GetValueString(DLArray[i].EGH, L"等号右里程冠号");
				Report_input.GetValueString(strText, L"等号左里程数");
				DLArray[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"等号右里程数");
				DLArray[i].ELC = _wtof(strText);
				Report_input.GetValueString(strText, L"断链长度");
				DLArray[i].DL = _wtof(strText);
				if (TabFname == L"分离段断链表")
				{
					Report_input.GetValueString(DLArray[i].FLDName, L"附注");
				}
				else DLArray[i].FLDName = L"";
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"控制点表" || TabFname == L"分离段控制点表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);

			if (JdCenArray) { delete[]JdCenArray; JdCenArray = NULL; }
			NJdCen = Report_input.GetRecordCount();
			JdCenArray = new JdCenter[NJdCen];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NJdCen*100);
				if (TabFname == L"分离段控制点表")
				{
					Report_input.GetValueString(strText, L"分离段编号");
					JdCenArray[i].FLDNo = (int)(_wtof(strText) + 0.1);
				}
				else JdCenArray[i].FLDNo = -1;

				Report_input.GetValueString(strText, L"控制点属性");
				if (strText == L"交点")JdCenArray[i].JDXZ = IsJiaoDian;
				else JdCenArray[i].JDXZ = IsCenter;

				Report_input.GetValueString(strText, L"坐标N");
				JdCenArray[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				JdCenArray[i].E = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线A1");
				JdCenArray[i].A1 = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径R");
				JdCenArray[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线A2");
				JdCenArray[i].A2 = _wtof(strText);

				Report_input.GetValueString(strText, L"转向");
				if (JdCenArray[i].JDXZ == IsJiaoDian)
				{
					if (strText == L"顺向左转回头")JdCenArray[i].IsOrNotHuiTou = 1;
					else if (strText == L"顺向右转回头")JdCenArray[i].IsOrNotHuiTou = 2;
					else if (strText == L"逆向左转回头")JdCenArray[i].IsOrNotHuiTou = 3;
					else if (strText == L"逆向右转回头")JdCenArray[i].IsOrNotHuiTou = 4;
					else JdCenArray[i].IsOrNotHuiTou = false;
				}
				else
				{
					if (strText == L"左转")JdCenArray[i].LeftOrRight = -1;
					else JdCenArray[i].LeftOrRight = 1;
				}

				Report_input.GetValueString(strText, L"后接关系");
				if (strText == L"S型")JdCenArray[i].LinkAfter = S_Link;
				else if (strText == L"C型")JdCenArray[i].LinkAfter = C_Link;
				else if (strText == L"卵型")JdCenArray[i].LinkAfter = Luan_Link;
				else JdCenArray[i].LinkAfter = NotRelation;

				Report_input.MoveNext();
				i++;
			}
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	//acedRestoreStatusBar();	
	return 1;
}

int XLDataBase::Read_XLDbs_Railway(CString mdbFname, CString TabFname)
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
			//	AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from " + TabFname;
	m_strSQL += L" Order By ID";
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
			//   ads_printf(L"没有记录!");
			return 0;
			//return false;
			//Report_input.Close();
		}

		Report_input.MoveFirst();

		//////////////////////////////////////////////////////////////////////////
		if (TabFname == L"坡度表")//铁路坡度表
		{
			int i = 0;
			NPD = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();
			for (int i = 0; i < NPD; i++)
			{
				PDB[i].cml = 0.0;
				PDB[i].degree = 3;
				PDB[i].GH = L" ";
				PDB[i].length = 0;
				PDB[i].Level = 0;
				PDB[i].ml = 0;
				PDB[i].Notes = L" ";
				PDB[i].RLen = 0;
				PDB[i].Rshu = 0;
				_tcscpy(PDB[i].CKml, L" ");
			}
			i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _wtof(strText);
				Report_input.GetValueString(strText, L"坡度");
				PDB[i].degree = _wtof(strText);
				Report_input.GetValueString(strText, L"坡长");
				PDB[i].length = _wtof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"附注");
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == L"左线曲线表" || TabFname == L"右线曲线表")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, 1);
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 2);
				QXB2x[i].jdno = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 3);
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, 4);
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, 5);
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, 6);
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, 7);
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, 8);
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, 9);
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左线断链表" || TabFname == L"右线断链表")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, 1);
				DLB2x[i].no = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(DLB2x[i].BGH, 2);

				Report_input.GetValueString(strText, 3);
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, 4);

				Report_input.GetValueString(strText, 5);
				DLB2x[i].ELC = _wtof(strText);

				Report_input.GetValueString(strText, 6);
				DLB2x[i].TYLC = _wtof(strText);

				Report_input.GetValueString(strText, 7);
				DLB2x[i].RXDName = strText;

				i++;
				Report_input.MoveNext();
			}

		}

		else if (TabFname == L"曲线表")
		{
			int i = 0;
			NQX = Report_input.GetRecordCount();
			if (QXB) { delete[]QXB; QXB = NULL; }
			if (NQX > 0)
				QXB = new QXTAB[NQX];

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"交点号");
				QXB[i].jdno = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标N");
				QXB[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				QXB[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"偏角");
				QXB[i].zj = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径");
				QXB[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线");
				QXB[i].l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线");
				QXB[i].l2 = _wtof(strText);

				Report_input.GetValueString(strText, L"前切线长");
				QXB[i].T1 = _wtof(strText);

				Report_input.GetValueString(strText, L"后切线长");
				QXB[i].T2 = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线长");
				QXB[i].L = _wtof(strText);

				Report_input.GetValueString(strText, L"起点里程冠号");
				QXB[i].BGH = strText;

				Report_input.GetValueString(strText, L"起点里程");
				QXB[i].Bml = _wtof(strText);

				Report_input.GetValueString(strText, L"终点里程冠号");
				QXB[i].EGH = strText;

				Report_input.GetValueString(strText, L"终点里程");
				QXB[i].Eml = _wtof(strText);

				Report_input.GetValueString(strText, L"内业断链");
				QXB[i].InDL = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"断链表")
		{
			int i = 0;
			NDL = Report_input.GetRecordCount();
			if (DLArray) { delete[]DLArray; DLArray = NULL; }
			if (NDL > 0)
				DLArray = new DLBZ[NDL];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"等号左里程冠号");
				DLArray[i].BGH = strText;
				Report_input.GetValueString(strText, L"等号右里程冠号");
				DLArray[i].EGH = strText;
				Report_input.GetValueString(strText, L"等号左里程数");
				DLArray[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"等号右里程数");
				DLArray[i].ELC = _wtof(strText);
				Report_input.GetValueString(strText, L"断链长度");
				DLArray[i].DL = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}

			double LCB = 0;  // LCB 断链的统一里程
			int iDL = 0;
			for (iDL = 0; iDL < NZDL; iDL++)
			{
				//该断链点统一里程
				if (iDL == 0)LCB = DLArray[iDL].ELC;
				else  LCB = LCB + DLArray[iDL].BLC - (DLArray[iDL - 1].ELC);
				DLArray[iDL].TYLC = LCB;
			}
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	return 1;
}


int XLDataBase::Read_EX_XLDbs(CString mdbFname, CString TabFname, int irxd)//读取线路数据库中的二线数据
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
		//if(TabFname=="左线绕行段隧道"||TabFname=="右线绕行段隧道")
		//{
		//	int i=0;
		//	TunNum = Report_input.GetRowCount();

		//	if(Tun) delete []Tun;
		//	if(TunNum >0)
		//		Tun  = new TUNTAB[TunNum];
		//	Report_input.MoveFirst();
		//	while(!Report_input.IsEOF())
		//	{
		//		Report_input.GetValueString(Tun[i].TunName,2);				
		//		Report_input.GetValueString(Tun[i].BGH,3);				
		//		Report_input.GetValueString(strText,4);				
		//		Tun[i].StDml = _wtof(strText);
		//		Report_input.GetValueString(Tun[i].EGH,5);				
		//		Report_input.GetValueString(strText,6);
		//		Tun[i].EdDml = _wtof(strText);

		//		Report_input.GetValueString(Tun[i].flag,7);
		//		Report_input.GetValueString(strText,8);
		//		Tun[i].xs = _wtoi(strText);
		//		Report_input.GetValueString(Tun[i].Note,9);

		//		dmltockml(Tun[i].BGH,Tun[i].StDml,Tun[i].SKml);
		//		dmltockml(Tun[i].EGH,Tun[i].EdDml,Tun[i].EKml);
		//		i++;
		//		Report_input.MoveNext();
		//	}		
		//}
		//else if(TabFname=="左线绕行段大中桥"||TabFname=="右线绕行段大中桥")
		//{
		//	int i=0;
		//	BriNum = Report_input.GetRowCount();
		//	if(Bri) delete []Bri;
		//	if(BriNum >0)
		//		Bri  = new BRITAB[BriNum];

		//	Report_input.MoveFirst();
		//	while(!Report_input.IsEOF())
		//	{
		//		Report_input.GetValueString(Bri[i].BriName,2);
		//		Report_input.GetValueString(Bri[i].CGH,3);				
		//		Report_input.GetValueString(strText,4);				
		//		Bri[i].CenDml = _wtof(strText);
		//		Report_input.GetValueString(Bri[i].BGH,5);				
		//		Report_input.GetValueString(strText,6);				
		//		Bri[i].StDml = _wtof(strText);
		//		Report_input.GetValueString(Bri[i].EGH,7);				
		//		Report_input.GetValueString(strText,8);				
		//		Bri[i].EdDml = _wtof(strText);
		//		Report_input.GetValueString(Bri[i].KKSY,9);				
		//		int pos=Bri[i].SWHt.Find(L".");
		//		if(pos>0)
		//			Bri[i].SWHt=Bri[i].SWHt.Left(pos+3);
		//		Report_input.GetValueString(strText,10);
		//		Bri[i].xs = _wtoi(strText);
		//		Report_input.GetValueString(strText,11);
		//		Bri[i].BriLen = _wtof(strText);
		//		Report_input.GetValueString(Bri[i].SWHt,12);
		//		Report_input.GetValueString(Bri[i].Note,13);


		//		dmltockml(Bri[i].CGH,Bri[i].CenDml,Bri[i].CKml);
		//		dmltockml(Bri[i].BGH,Bri[i].StDml,Bri[i].SKml);
		//		dmltockml(Bri[i].EGH,Bri[i].EdDml,Bri[i].EKml);
		//		i++;
		//		Report_input.MoveNext();
		//	}
		//}
		if (TabFname == L"左线绕行段坡度表" || TabFname == L"右线绕行段坡度表")
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
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"附注");
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"左线绕行段填挖表" || TabFname == L"右线绕行段填挖表")
		{
			int i = 0;
			NTW = Report_input.GetRowCount();
			if (TWB) delete[]TWB;
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(TWB[i].GH, L"加桩里程冠号");
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

//-----------------------------------------------------------------------------------------
int XLDataBase::Write_XLDbs(CString mdbFname, CString TabFname, CString RoadName)//写入线路数据库中的数据
{

	CString strText, GH, strSQL;

	CFileFind Find;         //先定义类对象,才能用里面的涵数
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		
		File.Format(L"%s\\lib\\公路标准.mdb", SysPath);

		CopyFileW(File, mdbFname, false);         //file 已存在,Path 不存在复制文件
		///////////////////////////////////////////////////////////////////////////
		//HRESULT hr = S_OK;

		////Set ActiveConnection of Catalog to this string
		//CString strcnn(_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname));
		//try
		//{
		//	ADOX::_CatalogPtr m_pCatalog = NULL;
		//	hr = m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
		//	if(FAILED(hr))
		//	{
		//		_com_issue_error(hr);
		//		/*AfxMessageBox(e.ErrorMessage());*/
		//	}
		//	else
		//	{
		//		m_pCatalog->Create(_bstr_t(strcnn)); //Create MDB
		//	}
		//}
		//catch(_com_error &e)
		//{
		//}

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
	//acedSetStatusBarProgressMeter("正在写入数据库",0,100);

	//////////////////////////////////////////////////////////////////////////
	//改移道路
	if (TabFname == L"改移道路表")
	{

		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER identity,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"相交铁路名 VARCHAR,";
			strSQL += L"起点里程 VARCHAR,";
			strSQL += L"铁路交叉里程 VARCHAR,";
			strSQL += L"终点里程 VARCHAR,";
			strSQL += L"公路交叉里程 VARCHAR,";
			strSQL += L"改移道路长度 DOUBLE PRECISION,";
			strSQL += L"交叉角 DOUBLE PRECISION,";
			strSQL += L"高差 DOUBLE PRECISION,";
			strSQL += L"铁路在其上下方 VARCHAR,";
			strSQL += L"孔跨结构 VARCHAR,";
			strSQL += L"等级种类 VARCHAR,";
			strSQL += L"路基面宽 DOUBLE PRECISION,";
			strSQL += L"路面宽 DOUBLE PRECISION,";
			strSQL += L"路面厚 DOUBLE PRECISION,";
			strSQL += L"路面材料 VARCHAR,";
			strSQL += L"路面工程数量 DOUBLE PRECISION,";
			strSQL += L"填方 DOUBLE PRECISION,";
			strSQL += L"挖方 DOUBLE PRECISION,";
			strSQL += L"圬工 DOUBLE PRECISION,";
			strSQL += L"用地 VARCHAR,";
			strSQL += L"附注 VARCHAR)";

			m_adoConnection.Execute(strSQL);
		}
		RoadName.Trim();
		CString  m_strSQL;
		if (RoadName.IsEmpty())
		{

			m_strSQL = L"delete * from " + TabFname;
		}
		else
		{
			m_strSQL = L"delete * from " + TabFname + L" where 道路名 = '" + RoadName + L"' ";
		}
		m_adoConnection.Execute(m_strSQL);

		m_strSQL = L"select * from " + TabFname;

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

		for (int i = 0; i < ModifyRoadSum; i++)
		{
			Report_input.AddNew();

			str = pModifyRoad[i].RoadName.Trim();//道路名
			Report_input.PutCollect(1, str);

			str = pModifyRoad[i].RailName;//相交铁路名

			Report_input.PutCollect(2, str);

			str = pModifyRoad[i].SCkml;//起点里程
			Report_input.PutCollect(3, str);

			str = pModifyRoad[i].RailCkml;//铁路交叉里程
			Report_input.PutCollect(4, str);

			//str = pModifyRoad[i].Raillevel;//铁路交叉高程
			//Report_input.PutCollect(5, str);

			str = pModifyRoad[i].ECkml;//终点里程
			Report_input.PutCollect(5, str);

			strText.Format(L"%lf", pModifyRoad[i].RoadDml);//公路交叉里程
			str = strText;
			Report_input.PutCollect(6, str);


			strText.Format(L"%lf", pModifyRoad[i].RoadLen);//改移道路长度
			str = strText;
			Report_input.PutCollect(7, str);

			strText.Format(L"%lf", pModifyRoad[i].InterAng);//交叉角
			str = strText;
			Report_input.PutCollect(8, str);

			strText.Format(L"%lf", pModifyRoad[i].DetaH);//高差
			str = strText;
			Report_input.PutCollect(9, str);

			str = pModifyRoad[i].UpDown;//铁路在其上下方
			Report_input.PutCollect(10, str);

			str = pModifyRoad[i].KKSY;//孔跨结构
			Report_input.PutCollect(11, str);

			str = pModifyRoad[i].RandType;//等级种类
			Report_input.PutCollect(12, str);

			strText.Format(L"%lf", pModifyRoad[i].LjWide);//路基面宽
			str = strText;
			Report_input.PutCollect(13, str);

			strText.Format(L"%lf", pModifyRoad[i].LmWide);//路面宽
			str = strText;
			Report_input.PutCollect(14, str);

			strText.Format(L"%lf", pModifyRoad[i].LmThick);//路面厚
			str = strText;
			Report_input.PutCollect(15, str);

			str = pModifyRoad[i].LmType;
			Report_input.PutCollect(16, str);//路面类型

			strText.Format(L"%lf", pModifyRoad[i].LmArea);//路面工程数量
			str = strText;
			Report_input.PutCollect(17, str);

			strText.Format(L"%lf", pModifyRoad[i].FillVol);//填方
			str = strText;
			Report_input.PutCollect(18, str);

			strText.Format(L"%lf", pModifyRoad[i].CutVol);//挖方
			str = strText;
			Report_input.PutCollect(19, str);

			strText.Format(L"%lf", pModifyRoad[i].MasonryVol);//圬工
			str = strText;
			Report_input.PutCollect(20, str);

			str = pModifyRoad[i].LandAreaStr;//用地
			Report_input.PutCollect(21, str);

			str = pModifyRoad[i].Note;//附注
			Report_input.PutCollect(22, str);

		}
		Report_input.Update();
		Report_input.Close();
	}
	//////////////////////////////////////////////////////////////////////////


	else if (TabFname == "道路名")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER identity,";
			strSQL += L"道路名称 VARCHAR)";
			m_adoConnection.Execute(strSQL);
		}

		m_strSQL = L"select * from " + TabFname;
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

		int i = Report_input.GetRecordCount();
		Report_input.MoveFirst();
		while (!Report_input.IsEOF())
		{
			CString strText;
			Report_input.GetValueString(strText, 1); // 获取道路名
			if (RoadName == strText)
			{
				//				   AfxMessageBox(L"道路名已存在,请重新命名!");
				return 0;
			}
			Report_input.MoveNext();
		}

		Report_input.AddNew();
		_variant_t str;
		CString no;
		no.Format(L"%d", i + 1);
		str = no;
		Report_input.PutCollect(0, str);
		str = RoadName;
		Report_input.PutCollect(1, str);
		Report_input.Update();
		Report_input.Close();
	}

	else if (TabFname == L"线路属性")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER identity,";
			strSQL += L"线路属性 VARCHAR,";
			strSQL += L"公路等级 VARCHAR,";
			strSQL += L"设计车速 VARCHAR,";
			strSQL += L"地段类别 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			Report_input.AddNew();

			str = L"1";
			Report_input.PutCollect(0, str);//线路等级			 
			str = RoadXZ.Grade;
			Report_input.PutCollect(1, str);//线路等级			 
			CString dvstr;
			dvstr.Format(L"%d", RoadXZ.DV);
			str = dvstr;
			Report_input.PutCollect(2, str);//地段类别				 
			str = RoadXZ.TerrainType;
			Report_input.PutCollect(3, str);//地形类别				 
			str = RoadXZ.DXLB;
			Report_input.PutCollect(4, str);//地形类别				 
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"超高计算标准")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"半径 VARCHAR,";
			strSQL += L"横坡 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int Row = 0; Row < NuphData; Row++)
			{
				//acedSetStatusBarProgressMeterPos(Row/NXL*100);
				Report_input.AddNew();

				strText.Format(L"%d", Row);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				strText.Format(L"%0.1lf", SuphData[Row][0]);
				str = strText;
				Report_input.PutCollect(1, str);//线路编号				 
				strText.Format(L"%0.1lf", SuphData[Row][1]);
				str = strText;
				Report_input.PutCollect(2, str);//线路编号
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"加宽计算标准")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"半径 VARCHAR,";
			strSQL += L"加宽 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int Row = 0; Row < NJK; Row++)
			{
				//acedSetStatusBarProgressMeterPos(Row/NXL*100);
				Report_input.AddNew();

				strText.Format(L"%d", Row);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				strText.Format(L"%0.1lf", JKData[Row][0]);
				str = strText;
				Report_input.PutCollect(1, str);//线路编号				 
				strText.Format(L"%0.1lf", JKData[Row][1]);
				str = strText;
				Report_input.PutCollect(2, str);//线路编号
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"超高计算参数")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"直线段超高渐变率 VARCHAR,";
			strSQL += L"超高缓和段长度限制 VARCHAR,";
			strSQL += L"缓和段最小超高渐变率 VARCHAR,";
			strSQL += L"超高方式 VARCHAR,";
			strSQL += L"加宽方式 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			Report_input.AddNew();

			str = L"1";
			Report_input.PutCollect(0, str);//序号
			str = CalCGCs.m_ZxCgJbl;
			Report_input.PutCollect(1, str);//线路属性
			ACHAR tmp[30];
			_stprintf(tmp, L"%d", CalCGCs.CgLenMode);
			str = tmp;
			Report_input.PutCollect(2, str);//线路属性

			str = CalCGCs.m_HxCgJbl;
			Report_input.PutCollect(3, str);//线路属性

			_stprintf(tmp, L"%d", CalCGCs.CgMode);
			str = tmp;
			Report_input.PutCollect(4, str);//线路属性
			_stprintf(tmp, L"%d", CalCGCs.JkMode);
			str = tmp;
			Report_input.PutCollect(5, str);//线路属性
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"纵面设计标准")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"最大纵坡 VARCHAR,";
			strSQL += L"凸竖曲线半径一般最小值 VARCHAR,";
			strSQL += L"凸竖曲线半径极限最小值 VARCHAR,";
			strSQL += L"凹竖曲线半径一般最小值 VARCHAR,";
			strSQL += L"凹竖曲线半径极限最小值 VARCHAR,";
			strSQL += L"竖曲线长度一般最小值 VARCHAR,";
			strSQL += L"竖曲线长度极限最小值 VARCHAR,";
			strSQL += L"坡长一般最小值 VARCHAR,";
			strSQL += L"坡长极限最小值 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}


			Report_input.AddNew();
			str = "1";
			Report_input.PutCollect(0, str);//线路等级

			str = ZDMBZ.MAXPD;
			Report_input.PutCollect(1, str);//线路属性

			str = ZDMBZ.TRshu1;
			Report_input.PutCollect(2, str);//线路等级

			str = ZDMBZ.TRshu2;
			Report_input.PutCollect(3, str);//线路等级

			str = ZDMBZ.ARshu1;
			Report_input.PutCollect(4, str);//线路等级

			str = ZDMBZ.ARshu2;
			Report_input.PutCollect(5, str);//线路等级

			str = ZDMBZ.Lshu1;
			Report_input.PutCollect(6, str);//线路等级

			str = ZDMBZ.Lshu2;
			Report_input.PutCollect(7, str);//线路等级

			str = ZDMBZ.PCMin1;
			Report_input.PutCollect(8, str);//线路等级

			str = ZDMBZ.PCMin2;
			Report_input.PutCollect(9, str);//线路等级


			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"桩号序列")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"桩号 VARCHAR,";
			strSQL += L"关联桩号 VARCHAR,";
			strSQL += L"关联道路 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表

			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;

			for (int i = 0; i < NSTA; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = STA[i].lc;
				Report_input.PutCollect(1, str);

				str = STA[i].gldmlc;
				Report_input.PutCollect(2, str);

				str = STA[i].glRoadName;
				Report_input.PutCollect(3, str);

				str = RoadName;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"控制标高")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"里程 VARCHAR,";
			strSQL += L"控制标高 VARCHAR,";
			strSQL += L"属性 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表

			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;

			for (int i = 0; i < CRNUM; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = pCtrlHt[i].dml;
				Report_input.PutCollect(1, str);

				str = pCtrlHt[i].Ht;
				Report_input.PutCollect(2, str);

				str = pCtrlHt[i].Att;
				Report_input.PutCollect(3, str);

				str = RoadName;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"左超高分段表" || TabFname == L"右超高分段表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"分段终里程 VARCHAR,";
			strSQL += L"路面横坡 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表

			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;
			for (int i = 0; i < NCG; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = CG[i].xlc;
				Report_input.PutCollect(1, str);

				str = CG[i].cg;
				Report_input.PutCollect(2, str);

				str = RoadName;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"左加宽分段表" || TabFname == L"右加宽分段表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"分段终里程 VARCHAR,";
			strSQL += L"路面加宽 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表

			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;
			for (int i = 0; i < NCG; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = CG[i].xlc;
				Report_input.PutCollect(1, str);

				str = CG[i].cg;
				Report_input.PutCollect(2, str);

				str = RoadName;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();

	}
	else if (TabFname == L"左路面宽分段表" || TabFname == L"右路面宽分段表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"分段终里程 VARCHAR,";
			strSQL += L"分隔带半宽 VARCHAR,";
			strSQL += L"分隔带横坡 VARCHAR,";
			strSQL += L"路缘带宽 VARCHAR,";
			strSQL += L"车行道宽 VARCHAR,";
			strSQL += L"路面横坡 VARCHAR,";
			strSQL += L"硬路肩宽 VARCHAR,";
			strSQL += L"硬路肩横坡 VARCHAR,";
			strSQL += L"土路肩宽 VARCHAR,";
			strSQL += L"土路肩横坡 VARCHAR,";//建立新表
			strSQL += L"变宽方式 VARCHAR,";//建立新表
			strSQL += L"设计标高位置 VARCHAR,";//建立新表
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"路基中线偏移量 VARCHAR)";//建立新表


			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;

			int FieldSum = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum - 1) != L"路基中线偏移量")//数据库中没有“沟底半径”列，动态添加该列
			{
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  路基中线偏移量   DOUBLE ", TabFname);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				Report_input.SetAdoConnection(&m_adoConnection);
				strSQL = L"select * from " + TabFname;
				Report_input.Open(strSQL, adCmdText);
			}
			for (int i = 0; i < NLMK; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = lmk[i].xlc;
				Report_input.PutCollect(1, str);

				str = lmk[i].fgdk;
				Report_input.PutCollect(2, str);

				str = lmk[i].fgdhp;
				Report_input.PutCollect(3, str);

				str = lmk[i].lydk;
				Report_input.PutCollect(4, str);

				str = lmk[i].cxdk;
				Report_input.PutCollect(5, str);

				str = lmk[i].lmhp;
				Report_input.PutCollect(6, str);

				str = lmk[i].yljk;
				Report_input.PutCollect(7, str);

				str = lmk[i].yljhp;
				Report_input.PutCollect(8, str);

				str = lmk[i].tljk;
				Report_input.PutCollect(9, str);

				str = lmk[i].tljhp;
				Report_input.PutCollect(10, str);

				strText.Format(L"%d", lmk[i].bkmode);
				str = strText;
				Report_input.PutCollect(11, str);

				strText.Format(L"%d", lmk[i].sjbgmode);
				str = strText;
				Report_input.PutCollect(12, str);

				str = lmk[i].OffsetByXLZX;
				Report_input.PutCollect(14, str);

				str = RoadName;
				Report_input.PutCollect(13, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();


	}

	else if (TabFname == L"左爬坡车道分段表" || TabFname == L"右爬坡车道分段表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"分段起始里程 VARCHAR,";
			strSQL += L"分段终止里程 VARCHAR,";
			strSQL += L"最大横坡 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表

			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;

			for (int i = 0; i < NPPCD; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = ppcd[i].sdml;
				Report_input.PutCollect(1, str);

				str = ppcd[i].edml;
				Report_input.PutCollect(2, str);

				str = ppcd[i].max_hp;
				Report_input.PutCollect(3, str);


				str = RoadName;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();


	}
	else if (TabFname == L"线路管理")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"线路编号 INTEGER,";
			strSQL += L"线路名称 VARCHAR,";
			strSQL += L"引出线路编号 INTEGER,";
			strSQL += L"引入线路编号 INTEGER)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;

			for (int Row = 0; Row < NXL; Row++)
			{
				//acedSetStatusBarProgressMeterPos(Row/NXL*100);
				Report_input.AddNew();

				strText.Format(L"%d", Row);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				strText.Format(L"%d", XLMan[Row].No);
				str = strText;
				Report_input.PutCollect(1, str);//线路编号

				str = XLMan[Row].Name;
				Report_input.PutCollect(2, str);//线路名称

				strText.Format(L"%d", XLMan[Row].PreNo);
				str = strText;
				Report_input.PutCollect(3, str);//引出线路编号

				strText.Format(L"%d", XLMan[Row].NextNo);
				str = strText;
				Report_input.PutCollect(4, str);//引入线路编号
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"控制点表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"控制点属性 VARCHAR,";
			strSQL += L"坐标N DOUBLE PRECISION,";
			strSQL += L"坐标E DOUBLE PRECISION,";
			strSQL += L"前缓和曲线A1 DOUBLE PRECISION,";
			strSQL += L"曲线半径R DOUBLE PRECISION,";
			strSQL += L"后缓和曲线A2 DOUBLE PRECISION,";
			strSQL += L"转向 VARCHAR,";
			strSQL += L"后接关系 VARCHAR,";
			strSQL += L"道路名 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;
			int add = 0;
			for (int Row = 0; Row < NJdCen; Row++)
			{
				//acedSetStatusBarProgressMeterPos(Row/NJdCen*100);
				Report_input.AddNew();

				strText.Format(L"%d", Row);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				if (JdCenArray[Row].JDXZ == IsJiaoDian) str = L"交点";
				else str = L"圆心";

				Report_input.PutCollect(1 + add, str);//控制点属性

				Report_input.PutCollect(2 + add, JdCenArray[Row].N);//N

				Report_input.PutCollect(3 + add, JdCenArray[Row].E);//E

				Report_input.PutCollect(4 + add, JdCenArray[Row].A1);//A1

				Report_input.PutCollect(5 + add, JdCenArray[Row].R);//R

				Report_input.PutCollect(6 + add, JdCenArray[Row].A2);//A2

				if (JdCenArray[Row].JDXZ == IsJiaoDian)
				{
					if (JdCenArray[Row].IsOrNotHuiTou == 1)str = L"顺向左转回头";
					else if (JdCenArray[Row].IsOrNotHuiTou == 2)str = L"顺向右转回头";
					else if (JdCenArray[Row].IsOrNotHuiTou == 3)str = L"逆向左转回头";
					else if (JdCenArray[Row].IsOrNotHuiTou == 4)str = L"逆向右转回头";
					else str = L"不回头";
				}
				else
				{
					if (JdCenArray[Row].LeftOrRight < 0)str = L"左转";
					else str = L"右转";
				}
				Report_input.PutCollect(7 + add, str);//转向　

				if (JdCenArray[Row].LinkAfter == S_Link)str = L"S型";
				else if (JdCenArray[Row].LinkAfter == C_Link)str = L"C型";
				else if (JdCenArray[Row].LinkAfter == Luan_Link)str = L"卵型";
				else str = L"无关";
				Report_input.PutCollect(8 + add, str);//后接关系
				str = RoadName;
				Report_input.PutCollect(9 + add, str);//后接关系

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"线元表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

			//(R1>R2)/////////////////////////////////////////////////////////////////////////
			//1:			L		R    A         A              A                A        //
			//2:					L	 R         R              R1               R2       //
			//3:                                                  R2               R1       //
			//////////////////////////////////////////////////////////////////////////////////
			//4: 左右转
			//5: 起始切线方位角
			//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
			//7: 半径 R
			//8，9：线元起点坐标 N,E
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"线元性质 DOUBLE PRECISION,";
			strSQL += L"LAR DOUBLE PRECISION,";
			strSQL += L"LR DOUBLE PRECISION,";
			strSQL += L"卵型后R DOUBLE PRECISION,";
			strSQL += L"左右转 DOUBLE PRECISION,";
			strSQL += L"起始方位角 DOUBLE PRECISION,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"半径R DOUBLE PRECISION,";
			strSQL += L"起始坐标N DOUBLE PRECISION,";
			strSQL += L"起始坐标E DOUBLE PRECISION,";
			strSQL += L"道路名 VARCHAR)";//建立新表
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
				// ads_printf(L"没有记录!");
				//		return 0;
			}
			int j = 0;
			int add = 0;
			for (int Row = 0; Row <= XYNum; Row++)
			{
				//acedSetStatusBarProgressMeterPos(Row/NJdCen*100);
				Report_input.AddNew();

				strText.Format(L"%d", Row);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				for (j = 0; j < 10; j++)
					Report_input.PutCollect(j + 1, XYArray[Row][j]);//

				str = RoadName;
				Report_input.PutCollect(11, str);//				 
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"断链表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"等号左里程冠号 VARCHAR,";
			strSQL += L"等号左里程数 DOUBLE PRECISION,";
			strSQL += L"等号右里程冠号 VARCHAR,";
			strSQL += L"等号右里程数 DOUBLE PRECISION,";
			strSQL += L"断链长度 DOUBLE PRECISION,";
			strSQL += L"道路名 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;
			for (int i = 0; i < NDL; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号


				str = DLArray[i].BGH;
				Report_input.PutCollect(1 + add, str);

				Report_input.PutCollect(2 + add, DLArray[i].BLC);

				str = DLArray[i].EGH;
				Report_input.PutCollect(3 + add, str);

				Report_input.PutCollect(4 + add, DLArray[i].ELC);

				Report_input.PutCollect(5 + add, DLArray[i].ELC - DLArray[i].BLC);

				str = RoadName;
				Report_input.PutCollect(6 + add, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"纵断链表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"等号左里程冠号 VARCHAR,";
			strSQL += L"等号左里程数 DOUBLE PRECISION,";
			strSQL += L"等号右里程冠号 VARCHAR,";
			strSQL += L"等号右里程数 DOUBLE PRECISION,";
			strSQL += L"断链长度 DOUBLE PRECISION,";
			strSQL += L"道路名 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + "'";
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
			int add = 0;
			for (int i = 0; i < NZDL; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号


				str = ZDLArray[i].BGH;
				Report_input.PutCollect(1 + add, str);

				Report_input.PutCollect(2 + add, ZDLArray[i].BLC);

				str = ZDLArray[i].EGH;
				Report_input.PutCollect(3 + add, str);

				Report_input.PutCollect(4 + add, ZDLArray[i].ELC);

				Report_input.PutCollect(5 + add, ZDLArray[i].ELC - ZDLArray[i].BLC);

				str = RoadName;
				Report_input.PutCollect(6 + add, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"坡度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER identity,";
			strSQL += L"变坡点里程数 DOUBLE PRECISION,";
			strSQL += L"设计高程 DOUBLE PRECISION,";
			strSQL += L"坡度 DOUBLE PRECISION,";
			strSQL += L"坡长 DOUBLE PRECISION,";
			strSQL += L"竖曲线半径 DOUBLE PRECISION,";
			strSQL += L"竖曲线长度 DOUBLE PRECISION,";
			strSQL += L"附注 VARCHAR,";
			strSQL += L"道路名 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + "'";
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
			int add = 0;
			int sno = Report_input.GetRecordCount();
			for (int i = 0; i < NPD; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();
				//	 strText.Format(L"%d",i);
				//	 str=strText;
				//	 Report_input.PutCollect(0,str);//序号

				Report_input.PutCollect(1, PDB[i].ml);
				Report_input.PutCollect(2, PDB[i].Level);
				Report_input.PutCollect(3, PDB[i].degree);
				Report_input.PutCollect(4, PDB[i].length);
				Report_input.PutCollect(5, PDB[i].Rshu);
				Report_input.PutCollect(6, PDB[i].RLen);
				str = PDB[i].Notes;
				Report_input.PutCollect(7, str);
				str = RoadName;
				Report_input.PutCollect(8, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"大中桥")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"桥梁名称 VARCHAR,";
			//			 strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += L"中桩里程数 DOUBLE PRECISION,";
			//             strSQL += "起点里程冠号 VARCHAR,";
			strSQL += L"起点里程数 DOUBLE PRECISION,";
			//			 strSQL += "终点里程冠号 VARCHAR,";
			strSQL += L"终点里程数 DOUBLE PRECISION,";
			strSQL += L"孔跨式样 VARCHAR,";
			strSQL += L"桥全长 DOUBLE PRECISION,";
			strSQL += L"100年水位高程 VARCHAR,";
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"性质 VARCHAR,";//建立新表
			strSQL += L"桥头交角 DOUBLE PRECISION,";
			strSQL += L"桥尾交角 DOUBLE PRECISION,";
			strSQL += L"左占地宽 DOUBLE PRECISION,";
			strSQL += L"右占地宽 DOUBLE PRECISION";
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			int add = 0;
			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				str = Bri[i].BriName;
				Report_input.PutCollect(1, str);
				//				 str = Bri[i].CGH;
				//				 Report_input.PutCollect(2,str);
				Report_input.PutCollect(2, Bri[i].CenDml);
				//				 str = Bri[i].BGH;
				//				 Report_input.PutCollect(4,str);
				Report_input.PutCollect(3, Bri[i].StDml);
				//				 str = Bri[i].EGH;
				//				 Report_input.PutCollect(6,str);
				Report_input.PutCollect(4, Bri[i].EdDml);
				str = Bri[i].KKSY;
				Report_input.PutCollect(5, str);
				Report_input.PutCollect(6, Bri[i].BriLen);
				str = Bri[i].SWHt;
				Report_input.PutCollect(7, str);
				str = RoadName;
				Report_input.PutCollect(8, str);
				str = Bri[i].XZ;
				Report_input.PutCollect(9, str);
				Report_input.PutCollect(10, Bri[i].HeadAng);
				Report_input.PutCollect(11, Bri[i].TailAng);
				Report_input.PutCollect(12, Bri[i].ZZDK);
				Report_input.PutCollect(13, Bri[i].YZDK);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"隧道")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"隧道名称 VARCHAR,";
			//			 strSQL += "起点里程冠号 VARCHAR,";
			strSQL += L"起点里程数 DOUBLE PRECISION,";
			//             strSQL += "终点里程冠号 VARCHAR,";
			strSQL += L"终点里程数 DOUBLE PRECISION,";
			strSQL += L"插旗方式 VARCHAR,";
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"性质 VARCHAR,";//建立新表
			strSQL += L"左占地宽 DOUBLE PRECISION,";
			strSQL += L"右占地宽 DOUBLE PRECISION";
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + "'";
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
				str = Tun[i].TunName;
				Report_input.PutCollect(1, str);
				//				 str = Tun[i].BGH;
				//				 Report_input.PutCollect(2,str);
				Report_input.PutCollect(2, Tun[i].StDml);
				//				 str = Tun[i].EGH;
				//				 Report_input.PutCollect(4,str);
				Report_input.PutCollect(3, Tun[i].EdDml);
				str = Tun[i].flag;
				Report_input.PutCollect(4, str);
				str = RoadName;
				Report_input.PutCollect(5, str);
				str = Tun[i].XZ;
				Report_input.PutCollect(6, str);
				Report_input.PutCollect(7, Tun[i].ZZDK);
				Report_input.PutCollect(8, Tun[i].YZDK);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"纵地面线表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			//			 strSQL += "加桩里程冠号 VARCHAR,";
			strSQL += L"加桩里程数 DOUBLE PRECISION,";
			strSQL += L"地面高程 DOUBLE PRECISION, ";
			//			 strSQL += "路肩设计高程 DOUBLE PRECISION,";  
			//			 strSQL += "中心填挖高度 DOUBLE PRECISION,";
			//			 strSQL += "竖曲线纵距 DOUBLE PRECISION,";
			strSQL += L"道路名 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
				//				 str = TWB[i].GH;
				//				 Report_input.PutCollect(1,str);
				Report_input.PutCollect(1, TWB[i].ml);
				Report_input.PutCollect(2, TWB[i].DMGC);
				//				 Report_input.PutCollect(3,TWB[i].SJGC);
				//				 Report_input.PutCollect(4,TWB[i].ZXTWG);
				//				 Report_input.PutCollect(5,TWB[i].SQXZJ);
				str = RoadName;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"直曲转角表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";

			strSQL += L"交点号 VARCHAR,";

			strSQL += L"交点桩号 VARCHAR,";
			strSQL += L"交点坐标N VARCHAR,";
			strSQL += L"交点坐标E VARCHAR,";
			strSQL += L"交点间距 VARCHAR,";
			strSQL += L"计算方位角 VARCHAR,";
			strSQL += L"曲线间直线长 VARCHAR,";
			strSQL += L"转角 VARCHAR,";
			strSQL += L"切线长T1 VARCHAR,";
			strSQL += L"切线长T2 VARCHAR,";

			strSQL += L"半径R1 VARCHAR,";
			strSQL += L"半径R2 VARCHAR,";
			strSQL += L"半径R3 VARCHAR,";

			strSQL += L"缓和曲线A1 VARCHAR,";
			strSQL += L"缓和曲线A2 VARCHAR,";

			strSQL += L"前缓和曲线长Ls1 VARCHAR,";
			strSQL += L"圆曲线长Lc VARCHAR,";
			strSQL += L"后缓和曲线长Ls2 VARCHAR,";
			strSQL += L"曲线总长L VARCHAR,";

			strSQL += L"外距 VARCHAR,";

			strSQL += L"前缓起点桩号 VARCHAR,";
			strSQL += L"前缓起点坐标N VARCHAR,";
			strSQL += L"前缓起点坐标E VARCHAR,";

			strSQL += L"前缓终点桩号 VARCHAR,";
			strSQL += L"前缓终点坐标N VARCHAR,";
			strSQL += L"前缓终点坐标E VARCHAR,";

			strSQL += L"圆曲线中点桩号 VARCHAR,";
			strSQL += L"圆曲线中点坐标N VARCHAR,";
			strSQL += L"圆曲线中点坐标E VARCHAR,";

			strSQL += L"后缓起点桩号 VARCHAR,";
			strSQL += L"后缓起点坐标N VARCHAR,";
			strSQL += L"后缓起点坐标E VARCHAR,";

			strSQL += L"后缓终点桩号 VARCHAR,";
			strSQL += L"后缓终点坐标N VARCHAR,";
			strSQL += L"后缓终点坐标E VARCHAR,";

			strSQL += L"附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
			for (int i = 0; i < NJdCen; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NJdCen*100);
				Report_input.AddNew();

				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);//序号

				str = ZQZJB[i].JDNO;
				if (i == 0 || i == NJdCen - 1)str = "";
				Report_input.PutCollect(1, str);//交点号

				str = ZQZJB[i].JDZH;
				Report_input.PutCollect(2, str);//交点桩号

				strText.Format(L"%.3lf", ZQZJB[i].JDN);
				str = strText;
				Report_input.PutCollect(3, str);//交点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].JDE);
				str = strText;
				Report_input.PutCollect(4, str);//交点坐标E

				strText.Format(L"%.3lf", ZQZJB[i].JDJJ);
				str = strText;
				Report_input.PutCollect(5, str);//交点间距

				strText.Format(L"%.4lf", dms_rad(ZQZJB[i].FWJ));
				str = strText;
				Report_input.PutCollect(6, str);//计算方位角

				strText.Format(L"%.3lf", ZQZJB[i].JIAZX);
				str = strText;
				Report_input.PutCollect(7, str);//曲线间直线长

				strText.Format(L"%.4lf", dms_rad(ZQZJB[i].AFA));
				str = strText;
				Report_input.PutCollect(8, str);//转角

				strText.Format(L"%.3lf", ZQZJB[i].T1);
				str = strText;
				Report_input.PutCollect(9, str);//切线长T1

				strText.Format(L"%.3lf", ZQZJB[i].T2);
				str = strText;
				Report_input.PutCollect(10, str);//切线长T2

				strText.Format(L"%.3lf", ZQZJB[i].R1);
				str = strText;
				Report_input.PutCollect(11, str);//半径R1

				strText.Format(L"%.3lf", ZQZJB[i].R2);
				str = strText;
				Report_input.PutCollect(12, str);//半径R2

				strText.Format(L"%.3lf", ZQZJB[i].R3);
				str = strText;
				Report_input.PutCollect(13, str);//半径R3

				strText.Format(L"%.3lf", ZQZJB[i].A1);
				str = strText;
				Report_input.PutCollect(14, str);//缓和曲线A1

				strText.Format(L"%.3lf", ZQZJB[i].A2);
				str = strText;
				Report_input.PutCollect(15, str);//缓和曲线A2

				strText.Format(L"%.3lf", ZQZJB[i].Ls1);
				str = strText;
				Report_input.PutCollect(16, str);//前缓和曲线长Ls1

				strText.Format(L"%.3lf", ZQZJB[i].Lc);
				str = strText;
				Report_input.PutCollect(17, str);//圆曲线长Lc

				strText.Format(L"%.3lf", ZQZJB[i].Ls2);
				str = strText;
				Report_input.PutCollect(18, str);//后缓和曲线长Ls2

				strText.Format(L"%.3lf", ZQZJB[i].L);
				str = strText;
				Report_input.PutCollect(19, str);//曲线总长L

				strText.Format(L"%.3lf", ZQZJB[i].OutDist);
				str = strText;
				Report_input.PutCollect(20, str);//外距

				str = ZQZJB[i].ZHZH;
				Report_input.PutCollect(21, str);//第一缓和曲线起点桩号

				strText.Format(L"%.3lf", ZQZJB[i].ZHN);
				str = strText;
				Report_input.PutCollect(22, str);//第一缓和曲线起点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].ZHE);
				str = strText;
				Report_input.PutCollect(23, str);//第一缓和曲线起点坐标E

				str = ZQZJB[i].HYZH;
				Report_input.PutCollect(24, str);//第一缓和曲线终点及圆曲线起点桩号

				strText.Format(L"%.3lf", ZQZJB[i].HYN);
				str = strText;
				Report_input.PutCollect(25, str);//第一缓和曲线终点及圆曲线起点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].HYE);
				str = strText;
				Report_input.PutCollect(26, str);//第一缓和曲线终点及圆曲线起点坐标E

				str = ZQZJB[i].YoZH;
				Report_input.PutCollect(27, str);//圆曲线中点桩号

				strText.Format(L"%.3lf", ZQZJB[i].YoN);
				str = strText;
				Report_input.PutCollect(28, str);//圆曲线中点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].YoE);
				str = strText;
				Report_input.PutCollect(29, str);//圆曲线中点坐标E

				str = ZQZJB[i].YHZH;
				Report_input.PutCollect(30, str);//第二缓和曲线起点及圆曲线终点桩号

				strText.Format(L"%.3lf", ZQZJB[i].YHN);
				str = strText;
				Report_input.PutCollect(31, str);//第二缓和曲线起点及圆曲线终点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].YHE);
				str = strText;
				Report_input.PutCollect(32, str);//第二缓和曲线起点及圆曲线终点坐标E

				str = ZQZJB[i].HZZH;
				Report_input.PutCollect(33, str);//第二缓和曲线终点桩号

				strText.Format(L"%.3lf", ZQZJB[i].HZN);
				str = strText;
				Report_input.PutCollect(34, str);//第二缓和曲线终点坐标N

				strText.Format(L"%.3lf", ZQZJB[i].HZE);
				str = strText;
				Report_input.PutCollect(35, str);//第二缓和曲线终点坐标E	 

				str = ZQZJB[i].Note;
				Report_input.PutCollect(36, str);//附注
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	//20190808添加	横断面基本参数系列数据库读写
	else if (TabFname == L"松方压实系数")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"松土 DOUBLE PRECISION,";
			strSQL += L"普通土 DOUBLE PRECISION,";
			strSQL += L"硬土 DOUBLE PRECISION,";
			strSQL += L"软石 DOUBLE PRECISION,";
			strSQL += L"次坚石 DOUBLE PRECISION,";
			strSQL += L"坚石 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s", TabFname);//删除全部
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
			}

			Report_input.AddNew();
			//序号
			strText.Format(L"%d", 1);
			str = strText;
			Report_input.PutCollect(0, str);
			//压实系数	只有一行？
			for (int i = 0; i < 6; i++)
			{
				strText.Format(L"%.3lf", YSXS[i]);
				str = strText;
				Report_input.PutCollect(i + 1, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"填方分类")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"填料类型 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_FillType.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//里程
				strText.Format(L"%.3lf", m_FillType[i].dml);
				str = strText;
				Report_input.PutCollect(2, str);

				//类型
				strText.Format(L"%ld", m_FillType[i].type);
				str = strText;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"用地面积汇总分段表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_FillType.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//里程
				strText.Format(L"%.3lf", m_EarthSection[i].dml);
				str = strText;
				Report_input.PutCollect(2, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"用地类型表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程数 DOUBLE PRECISION,";
			strSQL += L"用地类型 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < YDLXSum; i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//分段终里程数
				strText.Format(L"%.3lf", YDLXB[i].dml);
				str = strText;
				Report_input.PutCollect(2, str);

				//用地类型
				str = YDLXB[i].YDLX;
				Report_input.PutCollect(3, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"挖方分类")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"I百分比 DOUBLE PRECISION,";
			strSQL += L"II百分比 DOUBLE PRECISION,";
			strSQL += L"III百分比 DOUBLE PRECISION,";
			strSQL += L"IV百分比 DOUBLE PRECISION,";
			strSQL += L"V百分比 DOUBLE PRECISION,";
			strSQL += L"VI百分比 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_CutType.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//分段终里程
				strText.Format(L"%.3lf", m_CutType[i].dml);
				str = strText;
				Report_input.PutCollect(2, str);

				//各级百分比
				for (int j = 0; j < 6; j++)
				{
					strText.Format(L"%.3lf", m_CutType[i].sj[j]);
					str = strText;
					Report_input.PutCollect(3 + j, str);
				}
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"关联断面路肩拉直")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段起始里程 DOUBLE PRECISION,";
			strSQL += L"分段终止里程 DOUBLE PRECISION,";
			strSQL += L"水平距离 DOUBLE PRECISION,";
			strSQL += L"高差 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_GLDMLZ.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_GLDMLZ[i].Sdml);
				Report_input.PutCollect(3, m_GLDMLZ[i].Edml);
				Report_input.PutCollect(4, m_GLDMLZ[i].dX);
				Report_input.PutCollect(5, m_GLDMLZ[i].dH);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"关联断面路堑边坡推平")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段起始里程 DOUBLE PRECISION,";
			strSQL += L"分段终止里程 DOUBLE PRECISION,";
			strSQL += L"水平距离 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_GLDMLQBPTP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_GLDMLQBPTP[i].Sdml);
				Report_input.PutCollect(3, m_GLDMLQBPTP[i].Edml);
				Report_input.PutCollect(4, m_GLDMLQBPTP[i].dX);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"关联断面线间沟")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段起始里程 DOUBLE PRECISION,";
			strSQL += L"分段终止里程 DOUBLE PRECISION,";
			strSQL += L"沟宽 DOUBLE PRECISION,";
			strSQL += L"沟深 DOUBLE PRECISION,";
			strSQL += L"内边坡 DOUBLE PRECISION,";
			strSQL += L"外边坡 DOUBLE PRECISION,";
			strSQL += L"浆砌厚度 DOUBLE PRECISION,";
			strSQL += L"沟底半径 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_GLDMXJG.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_GLDMXJG[i].Sdml);
				Report_input.PutCollect(3, m_GLDMXJG[i].Edml);
				Report_input.PutCollect(4, m_GLDMXJG[i].GouWide);
				Report_input.PutCollect(5, m_GLDMXJG[i].GouHigh);
				Report_input.PutCollect(6, m_GLDMXJG[i].NBP);
				Report_input.PutCollect(7, m_GLDMXJG[i].WBP);
				Report_input.PutCollect(8, m_GLDMXJG[i].JQHD);
				Report_input.PutCollect(9, m_GLDMXJG[i].GDR);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左清表土厚度" || TabFname == L"右清表土厚度")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"清表土厚度 DOUBLE PRECISION,";
			strSQL += L"清表土性质 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_HTHD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_HTHD[i].dml);
				Report_input.PutCollect(3, m_HTHD[i].hthd);
				Report_input.PutCollect(4, m_HTHD[i].xz);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左沟底坡" || TabFname == L"右沟底坡")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始高程 DOUBLE PRECISION,";
			strSQL += L"终止里程 DOUBLE PRECISION,";
			strSQL += L"终止高程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_GouPD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_GouPD[i].Sdml);
				Report_input.PutCollect(3, m_GouPD[i].SH);
				Report_input.PutCollect(4, m_GouPD[i].Edml);
				Report_input.PutCollect(5, m_GouPD[i].EH);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左沟底坡" || TabFname == L"右沟底坡")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始高程 DOUBLE PRECISION,";
			strSQL += L"终止里程 DOUBLE PRECISION,";
			strSQL += L"终止高程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_GouPD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_GouPD[i].Sdml);
				Report_input.PutCollect(3, m_GouPD[i].SH);
				Report_input.PutCollect(4, m_GouPD[i].Edml);
				Report_input.PutCollect(5, m_GouPD[i].EH);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左超填宽度表" || TabFname == L"右超填宽度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始宽度 DOUBLE PRECISION,";
			strSQL += L"终止里程 DOUBLE PRECISION,";
			strSQL += L"终止宽度 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_CTKD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_CTKD[i].Sdml);
				Report_input.PutCollect(3, m_CTKD[i].Sctkd);
				Report_input.PutCollect(4, m_CTKD[i].Edml);
				Report_input.PutCollect(5, m_CTKD[i].Ectkd);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左弧形边坡表" || TabFname == L"右弧形边坡表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始半径 DOUBLE PRECISION,";
			strSQL += L"终止里程 DOUBLE PRECISION,";
			strSQL += L"终止半径 DOUBLE PRECISION,";
			strSQL += L"设置位置 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_ARCBP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_ARCBP[i].Sdml);
				Report_input.PutCollect(3, m_ARCBP[i].SR);
				Report_input.PutCollect(4, m_ARCBP[i].Edml);
				Report_input.PutCollect(5, m_ARCBP[i].ER);
				Report_input.PutCollect(6, m_ARCBP[i].ArcLocation);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左挡墙" || TabFname == L"右挡墙")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始支距 DOUBLE PRECISION,";
			strSQL += L"终点里程 DOUBLE PRECISION,";
			strSQL += L"终止半径 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_DQ.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_DQ[i].sdml);
				Report_input.PutCollect(3, m_DQ[i].szj);
				Report_input.PutCollect(4, m_DQ[i].edml);
				Report_input.PutCollect(5, m_DQ[i].ezj);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左低填路堤自动填平" || TabFname == L"右低填路堤自动填平")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"终点里程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LDTP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LDTP[i].sdml);
				Report_input.PutCollect(3, m_LDTP[i].edml);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左低挖路堑自动推平" || TabFname == L"右低挖路堑自动推平")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"终点里程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LQTP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LQTP[i].sdml);
				Report_input.PutCollect(3, m_LQTP[i].edml);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左低填路堤自动加宽土路肩" || TabFname == L"右低填路堤自动加宽土路肩")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"终点里程 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_TLJJK.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_TLJJK[i].sdml);
				Report_input.PutCollect(3, m_TLJJK[i].edml);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左路面厚度" || TabFname == L"右路面厚度")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程数 DOUBLE PRECISION,";
			strSQL += L"车行道厚 VARCHAR,";
			strSQL += L"硬路肩厚 VARCHAR,";
			strSQL += L"土路肩厚 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LMHD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LMHD[i].dml);
				Report_input.PutCollect(3, m_LMHD[i].cxdh);
				Report_input.PutCollect(4, m_LMHD[i].yljh);
				Report_input.PutCollect(5, m_LMHD[i].tljh);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左地面线内插宽" || TabFname == L"右地面线内插宽")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"内插宽 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_CRSWID.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_CRSWID[i].dml);
				Report_input.PutCollect(3, m_CRSWID[i].wid);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左横断面限制宽度" || TabFname == L"右横断面限制宽度")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"路基限宽 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_DM.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_DM[i].dml);
				Report_input.PutCollect(3, m_DM[i].xk);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左用地加宽" || TabFname == L"右用地加宽")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"路堤加宽 DOUBLE PRECISION,";
			strSQL += L"路堑加宽 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_PWD.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_PWD[i].dml);
				Report_input.PutCollect(3, m_PWD[i].ltjk);
				Report_input.PutCollect(4, m_PWD[i].lqjk);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左路堤边坡" || TabFname == L"右路堤边坡")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL, tempStr;

			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += "序号 INTEGER,";
			strSQL += "道路名 VARCHAR,";
			strSQL += "分段终里程 double precision,";
			strSQL += "边坡总级数 integer,";

			int i;
			for (i = 0; i < 29; i++)
			{
				tempStr.Format(L"%d", i + 1);
				strSQL += str;
				strSQL += "级边坡系数 double precision,";
				strSQL += str;
				strSQL += "级最大高度 double precision,";
				strSQL += str;
				strSQL += "级平台宽度 double precision,";
				strSQL += str;
				strSQL += "级平台横坡 double precision,";
			}

			tempStr.Format(L"%d", i + 1);
			strSQL += str;
			strSQL += "级边坡系数 double precision,";
			strSQL += str;
			strSQL += "级最大高度 double precision,";
			strSQL += str;
			strSQL += "级平台宽度 double precision,";
			strSQL += str;
			strSQL += "级平台横坡 double precision";
			strSQL += ")";

			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LDBP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LDBP[i].dml);
				Report_input.PutCollect(3, m_LDBP[i].BPJS);
				for (int j = 0; j < m_LDBP[i].BPJS * 4; j++)
				{
					Report_input.PutCollect(4 + j, m_LDBP[i].bpdata[j]);
				}
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左路堑边坡" || TabFname == L"右路堑边坡")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL, tempStr;

			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += "序号 INTEGER,";
			strSQL += "道路名 VARCHAR,";
			strSQL += "分段终里程 double precision,";
			strSQL += "边坡总级数 integer,";

			int i;
			for (i = 0; i < 29; i++)
			{
				tempStr.Format(L"%d", i + 1);
				strSQL += str;
				strSQL += "级边坡系数 double precision,";
				strSQL += str;
				strSQL += "级最大高度 double precision,";
				strSQL += str;
				strSQL += "级平台宽度 double precision,";
				strSQL += str;
				strSQL += "级平台横坡 double precision,";
			}

			tempStr.Format(L"%d", i + 1);
			strSQL += str;
			strSQL += "级边坡系数 double precision,";
			strSQL += str;
			strSQL += "级最大高度 double precision,";
			strSQL += str;
			strSQL += "级平台宽度 double precision,";
			strSQL += str;
			strSQL += "级平台横坡 double precision";
			strSQL += ")";

			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LQBP.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LQBP[i].dml);
				Report_input.PutCollect(3, m_LQBP[i].BPJS);
				for (int j = 0; j < m_LQBP[i].BPJS * 4; j++)
				{
					Report_input.PutCollect(4 + j, m_LQBP[i].bpdata[j]);
				}
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左排水沟" || TabFname == L"右排水沟")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"坡脚墙 DOUBLE PRECISION,";
			strSQL += L"浆砌厚度 DOUBLE PRECISION,";
			strSQL += L"起始里程 DOUBLE PRECISION,";
			strSQL += L"起始护坡道宽 DOUBLE PRECISION,";
			strSQL += L"起始护坡道厚 DOUBLE PRECISION,";
			strSQL += L"起始水沟底宽 DOUBLE PRECISION,";
			strSQL += L"起始水沟深 DOUBLE PRECISION,";
			strSQL += L"起始挡水堰宽 DOUBLE PRECISION,";
			strSQL += L"起始挡水堰厚 DOUBLE PRECISION,";
			strSQL += L"终止里程 DOUBLE PRECISION,";
			strSQL += L"终止护坡道宽 DOUBLE PRECISION,";
			strSQL += L"终止护坡道厚 DOUBLE PRECISION,";
			strSQL += L"终止水沟底宽 DOUBLE PRECISION,";
			strSQL += L"终止水沟深 DOUBLE PRECISION,";
			strSQL += L"终止挡水堰宽 DOUBLE PRECISION,";
			strSQL += L"终止挡水堰厚 DOUBLE PRECISION,";
			strSQL += L"内侧边坡 DOUBLE PRECISION,";
			strSQL += L"外侧边坡 DOUBLE PRECISION,";
			strSQL += L"挡水堰坡 DOUBLE PRECISION,";
			strSQL += L"沟底半径 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LDSG.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LDSG[i].WallorGou);
				Report_input.PutCollect(3, m_LDSG[i].jh);
				Report_input.PutCollect(4, m_LDSG[i].sdml);
				Report_input.PutCollect(5, m_LDSG[i].S_HPDK);
				Report_input.PutCollect(6, m_LDSG[i].S_HPDH);
				Report_input.PutCollect(7, m_LDSG[i].S_GS);
				Report_input.PutCollect(8, m_LDSG[i].S_GK);
				Report_input.PutCollect(9, m_LDSG[i].S_DSYK);
				Report_input.PutCollect(10, m_LDSG[i].S_DSYH);
				Report_input.PutCollect(11, m_LDSG[i].edml);
				Report_input.PutCollect(12, m_LDSG[i].E_HPDK);
				Report_input.PutCollect(13, m_LDSG[i].E_HPDH);
				Report_input.PutCollect(14, m_LDSG[i].E_GS);
				Report_input.PutCollect(15, m_LDSG[i].E_GK);
				Report_input.PutCollect(16, m_LDSG[i].E_DSYK);
				Report_input.PutCollect(17, m_LDSG[i].E_DSYH);
				Report_input.PutCollect(18, m_LDSG[i].nbp);
				Report_input.PutCollect(19, m_LDSG[i].wbp);
				Report_input.PutCollect(20, m_LDSG[i].DSYP);
				Report_input.PutCollect(21, m_LDSG[i].LDSG_GDR);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左侧沟" || TabFname == L"右侧沟")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"水沟底宽 DOUBLE PRECISION,";
			strSQL += L"沟深 DOUBLE PRECISION,";
			strSQL += L"左侧边坡 DOUBLE PRECISION,";
			strSQL += L"右侧边坡 DOUBLE PRECISION,";
			strSQL += L"浆砌厚度 DOUBLE PRECISION,";
			strSQL += L"沟底半径 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LQSG.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_LQSG[i].dml);
				Report_input.PutCollect(3, m_LQSG[i].gw);
				Report_input.PutCollect(4, m_LQSG[i].gs);
				Report_input.PutCollect(5, m_LQSG[i].nbp);
				Report_input.PutCollect(6, m_LQSG[i].wbp);
				Report_input.PutCollect(7, m_LQSG[i].Jh);
				Report_input.PutCollect(8, m_LQSG[i].GDR);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左天沟" || TabFname == L"右天沟")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"分段起里程 DOUBLE PRECISION,";
			strSQL += L"起始边距 DOUBLE PRECISION,";
			strSQL += L"起始水沟深 DOUBLE PRECISION,";
			strSQL += L"起始水沟底宽 DOUBLE PRECISION,";
			strSQL += L"分段终里程 DOUBLE PRECISION,";
			strSQL += L"终止边距 DOUBLE PRECISION,";
			strSQL += L"终止水沟深 DOUBLE PRECISION,";
			strSQL += L"终止水沟底宽 DOUBLE PRECISION,";
			strSQL += L"内侧边坡 DOUBLE PRECISION,";
			strSQL += L"外侧边坡 DOUBLE PRECISION,";
			strSQL += L"浆砌厚度 DOUBLE PRECISION,";
			strSQL += L"沟底半径 DOUBLE PRECISION";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_TG.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				Report_input.PutCollect(2, m_TG[i].sdml);
				Report_input.PutCollect(3, m_TG[i].sbj);
				Report_input.PutCollect(4, m_TG[i].sgs);
				Report_input.PutCollect(5, m_TG[i].sgw);
				Report_input.PutCollect(6, m_TG[i].edml);
				Report_input.PutCollect(7, m_TG[i].ebj);
				Report_input.PutCollect(8, m_TG[i].egs);
				Report_input.PutCollect(9, m_TG[i].egw);
				Report_input.PutCollect(10, m_TG[i].nbp);
				Report_input.PutCollect(11, m_TG[i].wbp);
				Report_input.PutCollect(12, m_TG[i].jqhd);
				Report_input.PutCollect(13, m_TG[i].TG_GDR);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"左路面分层表" || TabFname == L"右路面分层表")
	{
		if (!FindTable(TabFname))
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "左路面分层表 ";
			strSQL += "(";
			strSQL += "序号 INTEGER identity,";
			strSQL += "道路名 VARCHAR,";
			strSQL += "填料类型 VARCHAR,";
			strSQL += "厚度 double precision";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_RoadLayerArray.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				str = m_RoadLayerArray[i].LayerName;
				Report_input.PutCollect(2, str);
				Report_input.PutCollect(3, m_RoadLayerArray[i].LayerThickness);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"高程控制点表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"里程冠号 VARCHAR,";
			strSQL += L"里程数 DOUBLE PRECISION,";
			strSQL += L"高程上限 DOUBLE PRECISION,";
			strSQL += L"高程下限 DOUBLE PRECISION,";
			strSQL += L"净空 DOUBLE PRECISION,";
			strSQL += L"控制点性质 INTEGER,";
			strSQL += L"附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);
				//道路名
				str = m_LevelCtrlPtArray[i].RoadName;
				Report_input.PutCollect(1, str);
				//冠号
				str = m_LevelCtrlPtArray[i].GH;
				Report_input.PutCollect(2, str);
				//里程数
				Report_input.PutCollect(3, m_LevelCtrlPtArray[i].XCLc);
				//高程上限
				if (m_LevelCtrlPtArray[i].Style == 1)
				{
					Report_input.PutCollect(4, m_LevelCtrlPtArray[i].DesHSX);
				}
				else if (m_LevelCtrlPtArray[i].Style == 2)
				{
					Report_input.PutCollect(5, m_LevelCtrlPtArray[i].DesHXX);
				}
				else
				{
					Report_input.PutCollect(4, m_LevelCtrlPtArray[i].DesHSX);
					//高程下限
					Report_input.PutCollect(5, m_LevelCtrlPtArray[i].DesHXX);
				}
				//Report_input.PutCollect(4, m_LevelCtrlPtArray[i].DesHSX);
				////高程下限
				//Report_input.PutCollect(5, m_LevelCtrlPtArray[i].DesHXX);
				Report_input.PutCollect(6, m_LevelCtrlPtArray[i].clearrence);
				//控制点性质
				Report_input.PutCollect(7, m_LevelCtrlPtArray[i].Style);
				//附注
				str = m_LevelCtrlPtArray[i].Note;
				Report_input.PutCollect(8, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"接坡信息表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"道路名 VARCHAR,";
			strSQL += L"前接坡高程 DOUBLE PRECISION,";
			strSQL += L"前接坡坡度 DOUBLE PRECISION,";
			strSQL += L"前接坡坡长 DOUBLE PRECISION,";
			strSQL += L"后接坡高程 DOUBLE PRECISION,";
			strSQL += L"后接坡坡度 DOUBLE PRECISION,";
			strSQL += L"后接坡坡长 DOUBLE PRECISION,";
			strSQL += L"是否接坡 INTEGER,";
			strSQL += L"起始接坡 INTEGER,";
			strSQL += L"终止接坡 INTEGER";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}
			for (int i = 0; i < m_JPArray.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);
				//道路名
				str = m_JPArray[i].RoadName;
				Report_input.PutCollect(1, str);
				//前接坡高程
				str = m_JPArray[i].Bef_H;
				Report_input.PutCollect(2, str);
				//前接坡坡度
				str = m_JPArray[i].Bef_PD;
				Report_input.PutCollect(3, str);
				//前接坡坡长
				str = m_JPArray[i].Bef_PC;
				Report_input.PutCollect(4, str);
				//后接坡高程
				str = m_JPArray[i].Aft_H;
				Report_input.PutCollect(5, str);
				//后接坡坡度
				str = m_JPArray[i].Aft_PD;
				Report_input.PutCollect(6, str);
				//后接坡坡长
				str = m_JPArray[i].Aft_PC;
				Report_input.PutCollect(7, str);
				//是否接坡
				str = m_JPArray[i].Is_JP;
				Report_input.PutCollect(8, str);
				//起始是否接坡
				str = m_JPArray[i].Is_JP_Bef;
				Report_input.PutCollect(9, str);
				//终止是否接坡
				str = m_JPArray[i].Is_JP_Aft;
				Report_input.PutCollect(10, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"道铁交叉信息表")
	{
		if (!FindTable(TabFname))
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "道铁交叉信息表 ";
			strSQL += "(";
			strSQL += "序号 INTEGER identity,";
			strSQL += "道路名 VARCHAR,";
			strSQL += "连续里程 DOUBLE PRECISION,";
			strSQL += "现场里程 VARCHAR,";
			strSQL += "N坐标 DOUBLE PRECISION,";
			strSQL += "E坐标 DOUBLE PRECISION,";
			strSQL += "道路方位角 DOUBLE PRECISION,";
			strSQL += "铁路方位角 DOUBLE PRECISION,";
			strSQL += "夹角 DOUBLE PRECISION,";
			strSQL += "道路设计高程 DOUBLE PRECISION,";
			strSQL += "铁路设计高程 DOUBLE PRECISION,";
			strSQL += "高程差 DOUBLE PRECISION,";
			strSQL += "最小角度 DOUBLE PRECISION,";
			strSQL += "最小高差 DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
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
			}

			for (int i = 0; i < m_RoadNRailInterInforArray.size(); i++)
			{
				Report_input.AddNew();
				//序号
				strText.Format(L"%d", i);
				str = strText;
				Report_input.PutCollect(0, str);

				//道路名
				str = RoadName;
				Report_input.PutCollect(1, str);

				//其余信息
				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].CurDml);
				str = strText;
				Report_input.PutCollect(2, str);

				strText = m_RoadNRailInterInforArray[i].InterCkml;
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].InterN);
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].interE);
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].CurFwj);
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].InterFwj);
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].JiaJiao);
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].CurDesH);
				str = strText;
				Report_input.PutCollect(9, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].InterDesH);
				str = strText;
				Report_input.PutCollect(10, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].DetaH);
				str = strText;
				Report_input.PutCollect(11, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].MinAng);
				str = strText;
				Report_input.PutCollect(12, str);

				strText.Format(L"%lf", m_RoadNRailInterInforArray[i].MinDetaH);
				str = strText;
				Report_input.PutCollect(13, str);

			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}

	m_adoConnection.Disconnect();
	acedRestoreStatusBar();
	return 1;
}

int XLDataBase::Write_XLDbs(CString mdbFname, CString TabFname, CString RoadName, CString XZ)//写入线路数据库中的数据
{
	CString strText, GH, strSQL;

	CFileFind Find;         //先定义类对象,才能用里面的涵数
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		File.Format(L"%s\\lib\\公路标准.mdb", SysPath);
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
			AfxMessageBox(L"4 失败!");
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
	//acedSetStatusBarProgressMeter("正在写入数据库",0,100);

	if (TabFname == L"坡度表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER identity,";
			strSQL += L"变坡点里程数 DOUBLE PRECISION,";
			strSQL += L"设计高程 DOUBLE PRECISION,";
			strSQL += L"坡度 DOUBLE PRECISION,";
			strSQL += L"坡长 DOUBLE PRECISION,";
			strSQL += L"竖曲线半径 DOUBLE PRECISION,";
			strSQL += L"竖曲线长度 DOUBLE PRECISION,";
			strSQL += L"附注 VARCHAR,";
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"性质 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"' AND 性质='";
		strSQL += XZ + "'";
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
			int add = 0;
			for (int i = 0; i < NPD; i++)
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.AddNew();
				//				 strText.Format(L"%d",i);
				//				 str=strText;
				//				 Report_input.PutCollect(0,str);//序号

				Report_input.PutCollect(1, PDB[i].ml);
				Report_input.PutCollect(2, PDB[i].Level);
				Report_input.PutCollect(3, PDB[i].degree);
				Report_input.PutCollect(4, PDB[i].length);
				Report_input.PutCollect(5, PDB[i].Rshu);
				Report_input.PutCollect(6, PDB[i].RLen);
				str = PDB[i].Notes;
				Report_input.PutCollect(7, str);
				str = RoadName;
				Report_input.PutCollect(8, str);
				str = XZ;
				Report_input.PutCollect(9, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"大中桥")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"桥梁名称 VARCHAR,";
			//			 strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += L"中桩里程数 DOUBLE PRECISION,";
			//             strSQL += "起点里程冠号 VARCHAR,";
			strSQL += L"起点里程数 DOUBLE PRECISION,";
			//			 strSQL += "终点里程冠号 VARCHAR,";
			strSQL += L"终点里程数 DOUBLE PRECISION,";
			strSQL += L"孔跨式样 VARCHAR,";
			strSQL += L"桥全长 DOUBLE PRECISION,";
			strSQL += L"100年水位高程 VARCHAR,";
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"性质 VARCHAR,";//建立新表
			strSQL += L"桥头交角 DOUBLE PRECISION,";
			strSQL += L"桥尾交角 DOUBLE PRECISION,";
			strSQL += L"左占地宽 DOUBLE PRECISION,";
			strSQL += L"右占地宽 DOUBLE PRECISION";
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
		if (XZ == L"左幅" || XZ == L"右幅")
			m_strSQL += L"' AND 性质='" + XZ;
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
			int add = 0;
			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				str = Bri[i].BriName;
				Report_input.PutCollect(1, str);
				//				 str = Bri[i].CGH;
				//				 Report_input.PutCollect(2,str);
				Report_input.PutCollect(2, Bri[i].CenDml);
				//				 str = Bri[i].BGH;
				//				 Report_input.PutCollect(4,str);
				Report_input.PutCollect(3, Bri[i].StDml);
				//				 str = Bri[i].EGH;
				//				 Report_input.PutCollect(6,str);
				Report_input.PutCollect(4, Bri[i].EdDml);
				str = Bri[i].KKSY;
				Report_input.PutCollect(5, str);
				Report_input.PutCollect(6, Bri[i].BriLen);
				str = Bri[i].SWHt;
				Report_input.PutCollect(7, str);
				str = RoadName;
				Report_input.PutCollect(8, str);
				str = Bri[i].XZ;
				Report_input.PutCollect(9, str);
				Report_input.PutCollect(10, Bri[i].HeadAng);
				Report_input.PutCollect(11, Bri[i].TailAng);
				Report_input.PutCollect(12, Bri[i].ZZDK);
				Report_input.PutCollect(13, Bri[i].YZDK);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"隧道")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			strSQL += L"隧道名称 VARCHAR,";
			//			 strSQL += "起点里程冠号 VARCHAR,";
			strSQL += L"起点里程数 DOUBLE PRECISION,";
			//             strSQL += "终点里程冠号 VARCHAR,";
			strSQL += L"终点里程数 DOUBLE PRECISION,";
			strSQL += L"插旗方式 VARCHAR,";
			strSQL += L"道路名 VARCHAR,";//建立新表
			strSQL += L"性质 VARCHAR,";//建立新表
			strSQL += L"左占地宽 DOUBLE PRECISION,";
			strSQL += L"右占地宽 DOUBLE PRECISION";
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"'";
		if (XZ == L"左幅" || XZ == L"右幅")
			m_strSQL += L"' AND 性质='" + XZ;
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
				str = Tun[i].TunName;
				Report_input.PutCollect(1, str);
				//				 str = Tun[i].BGH;
				//				 Report_input.PutCollect(2,str);
				Report_input.PutCollect(2, Tun[i].StDml);
				//				 str = Tun[i].EGH;
				//				 Report_input.PutCollect(4,str);
				Report_input.PutCollect(3, Tun[i].EdDml);
				str = Tun[i].flag;
				Report_input.PutCollect(4, str);
				str = RoadName;
				Report_input.PutCollect(5, str);
				str = Tun[i].XZ;
				Report_input.PutCollect(6, str);
				Report_input.PutCollect(7, Tun[i].ZZDK);
				Report_input.PutCollect(8, Tun[i].YZDK);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	else if (TabFname == L"边线地面线表")
	{
		if (!FindTable(TabFname))//表不存在
		{
			CString strSQL;
			strSQL = L"create table ";
			strSQL += TabFname;
			strSQL += L"(";
			strSQL += L"序号 INTEGER,";
			//			 strSQL += "加桩里程冠号 VARCHAR,";
			strSQL += L"加桩里程数 DOUBLE PRECISION,";
			strSQL += L"地面高程 DOUBLE PRECISION, ";
			//			 strSQL += "路肩设计高程 DOUBLE PRECISION,";  
			//			 strSQL += "中心填挖高度 DOUBLE PRECISION,";
			//			 strSQL += "竖曲线纵距 DOUBLE PRECISION,";
			strSQL += L"支距 VARCHAR,";//建立新表
			strSQL += L"道路名 VARCHAR";//建立新表
			//			 strSQL +="附注 VARCHAR";
			strSQL += L")";
			m_adoConnection.Execute(strSQL);//创建表
		}
		strSQL.Format(L"delete from %s where 道路名=", TabFname);//删除全部
		strSQL += L"'" + RoadName + L"' AND 支距='";
		strSQL += XZ + L"'";
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
				Report_input.PutCollect(1, TWB[i].ml);
				Report_input.PutCollect(2, TWB[i].DMGC);
				str = XZ;
				Report_input.PutCollect(3, str);
				str = RoadName;
				Report_input.PutCollect(4, str);
			}
			Report_input.Update();
		}
		catch (_com_error e)
		{
		}
		Report_input.Close();
	}
	m_adoConnection.Disconnect();
	acedRestoreStatusBar();
	return 1;
}

bool XLDataBase::InitTableList(void)
{
	CAdoRecordSet rsetTable;
	_bstr_t Value;
	CString strTablename = L"";
	m_strTableNameList.RemoveAll();
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

bool XLDataBase::FindTable(CString strTempName)
{
	CString str;
	for (int i = 0; i < m_strTableNameList.GetSize(); i++)
	{
		str = m_strTableNameList.GetAt(i);
		if (str == strTempName)
			return true;
	}
	return false;
}

void XLDataBase::GetAllXLName(CStringArray* XLNameArray)
{
	if (NDL > 0)
	{
		int NXL = DLArray[NDL - 1].FLDNo;
		int iFLD = 1;
		for (int iDL = 0; iDL < NDL; iDL++)
		{
			if (DLArray[iDL].FLDNo == iFLD)
			{
				CString str;
				str.Format(L"%d-%s", iFLD, DLArray[iDL].FLDName);
				XLNameArray->Add(str);
				iFLD++;
			}
		}
	}
}

bool XLDataBase::GetiFLDDLInfo(int iFLD, int& sDLNo, int& eDLNo)
{
	int FLDNum = DLArray[NDL - 1].FLDNo;
	if (iFLD<1 || iFLD>FLDNum)return false;
	int i = 0;
	for (i = 0; i < NDL; i++)if (DLArray[i].FLDNo == iFLD)break;
	sDLNo = i;
	for (i = NDL - 1; i >= 0; i--)if (DLArray[i].FLDNo == iFLD)break;
	eDLNo = i;
	if (sDLNo >= 0 && eDLNo <= NDL && eDLNo >= 0 && eDLNo <= NDL && sDLNo < eDLNo)return true;
	else return false;
}
//---------------------------------------------------------------------------------------
int XLDataBase::Read_XLDbs(CString mdbFname, CString TabFname, CString RoadName, CString XZ)//读取线路数据库中的数据
{
	CString strText, GH;
	double xlc;
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
			ads_printf(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		ads_printf(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from " + TabFname;
	m_strSQL += L" where 道路名='" + RoadName;

	if (TabFname == L"边线地面线表")
		m_strSQL += L"' AND 支距='" + XZ;
	else
		m_strSQL += L"' AND 性质='" + XZ;

	//	if(TabFname!="坡度表")
	m_strSQL += L"' ORDER by 序号";
	//	else
	//		m_strSQL += "'";

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
			//            ads_printf(L"没有记录!");
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();
		if (TabFname == L"坡度表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			int i = 0;
			NPD = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (PDB) { delete[]PDB, PDB = NULL; }
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				if (pzLinep)
					PDB[i].cml = pzLinep->TYLC(PDB[i].ml);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _wtof(strText);
				//20190807	新增	读取所有数据
				Report_input.GetValueString(strText, L"坡度");
				PDB[i].degree = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].length = _wtof(strText);
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"大中桥")
		{
			int i = 0;
			BriNum = Report_input.GetRowCount();
			if (Bri) { delete[]Bri, Bri = NULL; }
			if (BriNum > 0)
				Bri = new BRITAB[BriNum];

			Report_input.MoveFirst();
			double cml;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Bri[i].BriName, 1);
				//				Report_input.GetValueString(Bri[i].CGH,2);	//中心里程冠号			
				Report_input.GetValueString(strText, 2);
				Bri[i].CenDml = _wtof(strText);
				//				Report_input.GetValueString(Bri[i].BGH,4);	//起始里程冠号			
				Report_input.GetValueString(strText, 3);
				Bri[i].StDml = _wtof(strText);
				//				Report_input.GetValueString(Bri[i].EGH,6);	//终止里程冠号			
				Report_input.GetValueString(strText, 4);
				Bri[i].EdDml = _wtof(strText);
				Report_input.GetValueString(Bri[i].KKSY, 5);
				Report_input.GetValueString(Bri[i].SWHt, 7);
				int pos = Bri[i].SWHt.Find(L".");
				if (pos > 0)
					Bri[i].SWHt = Bri[i].SWHt.Left(pos + 3);
				Report_input.GetValueString(Bri[i].XZ, L"性质");
				Report_input.GetValueString(strText, L"桥头交角");
				Bri[i].HeadAng = _wtof(strText);
				Report_input.GetValueString(strText, L"桥尾交角");
				Bri[i].TailAng = _wtof(strText);
				Report_input.GetValueString(strText, L"左占地宽");
				Bri[i].ZZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"右占地宽");
				Bri[i].YZDK = _wtof(strText);
				if (pzLinep)
				{
					cml = pzLinep->TYLC(Bri[i].CenDml);
					_tcscpy(Bri[i].CKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Bri[i].StDml);
					_tcscpy(Bri[i].SKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Bri[i].EdDml);
					_tcscpy(Bri[i].EKml, pzLinep->XLC(cml));
				}

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"隧道")
		{
			int i = 0;
			TunNum = Report_input.GetRowCount();

			if (Tun) { delete[]Tun, Tun = NULL; }
			if (TunNum > 0)
				Tun = new TUNTAB[TunNum];
			double cml;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Tun[i].TunName, 1);
				//				Report_input.GetValueString(Tun[i].BGH,2);				
				Report_input.GetValueString(strText, 2);
				Tun[i].StDml = _wtof(strText);
				//				Report_input.GetValueString(Tun[i].EGH,4);				
				Report_input.GetValueString(strText, 3);
				Tun[i].EdDml = _wtof(strText);
				Report_input.GetValueString(Tun[i].XZ, L"性质");
				Report_input.GetValueString(strText, L"左占地宽");
				Tun[i].ZZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"右占地宽");
				Tun[i].YZDK = _wtof(strText);
				if (pzLinep)
				{
					cml = pzLinep->TYLC(Tun[i].StDml);
					_tcscpy(Tun[i].SKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Tun[i].EdDml);
					_tcscpy(Tun[i].EKml, pzLinep->XLC(cml));
				}
				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"边线地面线表")
		{
			int i = 0;
			double dml, cml;
			NTW = Report_input.GetRowCount();
			if (TWB) { delete[]TWB, TWB = NULL; }
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"加桩里程数");
				dml = _wtof(strText);
				if (pzLinep)
					cml = pzLinep->TYLC(dml);
				else
					cml = 0;
				if (cml > Scml - 0.001 && cml < Ecml + 0.001)
				{
					TWB[i].ml = dml;
					TWB[i].cml = cml;
					Report_input.GetValueString(strText, L"地面高程");
					TWB[i].DMGC = _wtof(strText);
					i++;
				}
				Report_input.MoveNext();
			}
			NTW = i;
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	//acedRestoreStatusBar();	
	return 1;
}
//---------------------------------------------------------------------------------------
int XLDataBase::Read_XLDbs(CString mdbFname, CString TabFname, CString RoadName)//读取线路数据库中的数据
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
			strText.Format(L"连接%s数据库失败-R3!", strConnect);
			ads_printf(strText);
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from " + TabFname;
	m_strSQL += L" where 道路名= '" + RoadName;
	m_strSQL += L"' ORDER by 序号";

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
			if (TabFname == L"控制点表")
			{
				if (JdCenArray) { delete[]JdCenArray; JdCenArray = NULL; }
				NJdCen = 0;
			}

			if (TabFname == L"纵断链表")
			{
				Report_input.Close();
				m_adoConnection.Disconnect();
				int res = Read_XLDbs(mdbFname, L"断链表", RoadName);
				NZDL = NDL;
				if (NDL > 0)
				{
					if (ZDLArray) delete[]ZDLArray;
					ZDLArray = new DLBZ[NZDL];
					//acedSetStatusBarProgressMeterPos(i/NDL*100);
					for (int i = 0; i < NZDL; i++)
					{
						ZDLArray[i].BGH = DLArray[i].BGH;
						ZDLArray[i].EGH = DLArray[i].EGH;
						ZDLArray[i].BLC = DLArray[i].BLC;
						ZDLArray[i].ELC = DLArray[i].ELC;
					}
					double LCB = 0;  // LCB 断链的统一里程
					int iDL = 0;
					for (iDL = 0; iDL < NZDL; iDL++)
					{
						//该断链点统一里程
						if (iDL == 0)LCB = ZDLArray[iDL].ELC;
						else  LCB = LCB + ZDLArray[iDL].BLC - (ZDLArray[iDL - 1].ELC);
						ZDLArray[iDL].TYLC = LCB;
					}
				}
				return res;
			}
			else return 0;
			//return false;
			//Report_input.Close();
		}

		Report_input.MoveFirst();

		//////////////////////////////////////////////////////////////////////////
		//改移道路表
		if (TabFname == L"改移道路表")
		{
			if (pModifyRoad) { delete[]pModifyRoad; pModifyRoad = NULL; }
			ModifyRoadSum = Report_input.GetRecordCount();
			pModifyRoad = new ModifyRoadTab[ModifyRoadSum];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"道路名");
				pModifyRoad[i].RoadName = strText;
				Report_input.GetValueString(strText, L"相交铁路名");
				pModifyRoad[i].RailName = strText;
				Report_input.GetValueString(strText, L"起点里程");
				pModifyRoad[i].SCkml = strText;
				Report_input.GetValueString(strText, L"铁路交叉里程");
				pModifyRoad[i].RailCkml = strText;
				Report_input.GetValueString(strText, L"铁路交叉高程");
				pModifyRoad[i].Raillevel = _wtof(strText);

				Report_input.GetValueString(strText, L"终点里程");
				pModifyRoad[i].ECkml = strText;
				Report_input.GetValueString(strText, L"公路交叉里程");
				pModifyRoad[i].RoadDml = _wtof(strText);
				Report_input.GetValueString(strText, L"改移道路长度");
				pModifyRoad[i].RoadLen = _wtof(strText);
				Report_input.GetValueString(strText, L"交叉角");
				pModifyRoad[i].InterAng = _wtof(strText);
				Report_input.GetValueString(strText, L"高差");
				pModifyRoad[i].DetaH = _wtof(strText);
				Report_input.GetValueString(strText, L"铁路在其上下方");
				pModifyRoad[i].UpDown = strText;
				Report_input.GetValueString(strText, L"孔跨结构");
				pModifyRoad[i].KKSY = strText;
				Report_input.GetValueString(strText, L"等级种类");
				pModifyRoad[i].RandType = strText;
				Report_input.GetValueString(strText, L"路基面宽");
				pModifyRoad[i].LjWide = _wtof(strText);
				Report_input.GetValueString(strText, L"路面宽");
				pModifyRoad[i].LmWide = _wtof(strText);
				Report_input.GetValueString(strText, L"路面厚");
				pModifyRoad[i].LmThick = _wtof(strText);
				Report_input.GetValueString(strText, L"路面材料");
				pModifyRoad[i].LmType = strText;
				Report_input.GetValueString(strText, L"路面工程数量");
				pModifyRoad[i].LmArea = _wtof(strText);
				Report_input.GetValueString(strText, L"填方");
				pModifyRoad[i].FillVol = _wtof(strText);
				Report_input.GetValueString(strText, L"挖方");
				pModifyRoad[i].CutVol = _wtof(strText);
				Report_input.GetValueString(strText, L"圬工");
				pModifyRoad[i].MasonryVol = _wtof(strText);
				Report_input.GetValueString(strText, L"用地");
				pModifyRoad[i].LandAreaStr = strText;
				Report_input.GetValueString(strText, L"附注");
				pModifyRoad[i].Note = strText;

				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == L"用地类型表")
		{
			if (YDLXB) { delete[]YDLXB; YDLXB = NULL; }
			YDLXSum = Report_input.GetRecordCount();
			YDLXB = new YongDiLXB[YDLXSum];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程数");
				YDLXB[i].dml = _wtof(strText);
				Report_input.GetValueString(YDLXB[i].YDLX, L"用地类型");
				Report_input.MoveNext();
				i++;
			}
		}
		//////////////////////////////////////////////////////////////////////////

		else if (TabFname == L"线路管理")
		{
			//		   //acedSetStatusBarProgressMeter("读取数据",0,100);

			if (XLMan) { delete[]XLMan; XLMan = NULL; }
			NXL = Report_input.GetRecordCount();
			XLMan = new XLManager[NXL];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"线路编号");
				XLMan[i].No = (int)(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"线路名称");
				XLMan[i].Name = strText;

				Report_input.GetValueString(strText, L"引出线路编号");
				XLMan[i].PreNo = (int)(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"引入线路编号");
				XLMan[i].NextNo = (int)(_wtof(strText) + 0.001);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"断链表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);

			if (DLArray) { delete[]DLArray; DLArray = NULL; }
			NDL = Report_input.GetRecordCount();
			DLArray = new DLBZ[NDL];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(DLArray[i].BGH, L"等号左里程冠号");
				Report_input.GetValueString(DLArray[i].EGH, L"等号右里程冠号");
				Report_input.GetValueString(strText, L"等号左里程数");
				DLArray[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"等号右里程数");
				DLArray[i].ELC = _wtof(strText);
				Report_input.GetValueString(strText, L"断链长度");
				DLArray[i].DL = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}

			double LCB = 0;  // LCB 断链的统一里程
			int iDL = 0;
			for (iDL = 0; iDL < NDL; iDL++)
			{
				//该断链点统一里程
				if (iDL == 0)LCB = DLArray[iDL].ELC;
				else  LCB = LCB + DLArray[iDL].BLC - (DLArray[iDL - 1].ELC);
				DLArray[iDL].TYLC = LCB;
			}
		}
		else if (TabFname == L"纵断链表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			if (ZDLArray) { delete[]ZDLArray; ZDLArray = NULL; }
			NZDL = Report_input.GetRecordCount();
			if (NZDL > 0)
			{
				ZDLArray = new DLBZ[NZDL];
				int i = 0;
				Report_input.MoveFirst();
				while (!Report_input.IsEOF())
				{
					//acedSetStatusBarProgressMeterPos(i/NDL*100);
					Report_input.GetValueString(ZDLArray[i].BGH, L"等号左里程冠号");
					Report_input.GetValueString(ZDLArray[i].EGH, L"等号右里程冠号");
					Report_input.GetValueString(strText, L"等号左里程数");
					ZDLArray[i].BLC = _wtof(strText);
					Report_input.GetValueString(strText, L"等号右里程数");
					ZDLArray[i].ELC = _wtof(strText);
					ZDLArray[i].DL = ZDLArray[i].ELC - ZDLArray[i].BLC;
					Report_input.MoveNext();
					i++;
				}
				double LCB = 0;  // LCB 断链的统一里程
				int iDL = 0;
				for (iDL = 0; iDL < NZDL; iDL++)
				{
					//该断链点统一里程
					if (iDL == 0)LCB = ZDLArray[iDL].ELC;
					else  LCB = LCB + ZDLArray[iDL].BLC - (ZDLArray[iDL - 1].ELC);
					ZDLArray[iDL].TYLC = LCB;
				}
			}
		}
		else if (TabFname == L"边线地面线表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);			
			NDMX = 0;
			int i;
			Report_input.MoveFirst();
			double dml, cml, dmgc, ZJ;
			bool inf;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"加桩里程数");
				dml = _wtof(strText);
				if (pzLinep)
					cml = pzLinep->TYLC(dml);
				Report_input.GetValueString(strText, L"地面高程");
				dmgc = _wtof(strText);
				Report_input.GetValueString(strText, L"支距");
				ZJ = _wtof(strText);
				inf = true;
				for (i = 0; i < NDMX; i++)
				{
					if (fabs(DmxCsArr[i].ZJ - ZJ) < 0.01)//已经有该边线地面线记录
					{
						DmxCsArr[i].sdml = DmxCsArr[i].sdml < cml ? DmxCsArr[i].sdml : cml;
						DmxCsArr[i].edml = DmxCsArr[i].edml > cml ? DmxCsArr[i].edml : cml;
						inf = false;
						break;
					}
				}
				if (inf)//没有该边线地面线记录
				{
					DmxCsArr[NDMX].ZJ = ZJ;
					DmxCsArr[NDMX].sdml = cml;
					DmxCsArr[NDMX].edml = cml;
					DmxCsArr[NDMX].Color = 0;
					NDMX++;
				}
				Report_input.MoveNext();
			}
			for (i = 0; i < NDMX; i++)
			{
				if (pzLinep)
				{
					DmxCsArr[i].sdml = pzLinep->XLC1(DmxCsArr[i].sdml);
					DmxCsArr[i].edml = pzLinep->XLC1(DmxCsArr[i].edml);
				}
			}
		}
		else if (TabFname == L"控制标高")
		{
			if (pCtrlHt) { delete[]pCtrlHt; pCtrlHt = NULL; }
			CRNUM = Report_input.GetRecordCount();
			pCtrlHt = new CTRLHT[CRNUM];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(strText, L"里程");
				pCtrlHt[i].dml = _wtof(strText);
				Report_input.GetValueString(strText, L"控制标高");
				pCtrlHt[i].Ht = _wtof(strText);
				Report_input.GetValueString(strText, L"属性");
				pCtrlHt[i].Att = strText;
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"桩号序列")
		{
			if (STA) { delete[]STA; STA = NULL; }
			NSTA = Report_input.GetRecordCount();
			if (NSTA > 0)
			{
				STA = new LCBdata[NSTA];

				int i = 0;
				Report_input.MoveFirst();
				while (!Report_input.IsEOF())
				{
					//acedSetStatusBarProgressMeterPos(i/NDL*100);
					Report_input.GetValueString(strText, L"桩号");
					STA[i].lc = _wtof(strText);

					Report_input.GetValueString(strText, L"关联桩号");
					STA[i].gldmlc = _wtof(strText);

					Report_input.GetValueString(strText, L"关联道路");
					STA[i].glRoadName = strText;
					Report_input.MoveNext();
					i++;
				}
			}
		}
		else if (TabFname == L"左超高分段表" || TabFname == L"右超高分段表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);			
			NCG = Report_input.GetRecordCount();
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(strText, L"分段终里程");
				CG[i].xlc = _wtof(strText);

				Report_input.GetValueString(strText, L"路面横坡");
				CG[i].cg = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"左加宽分段表" || TabFname == L"右加宽分段表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);			

			NCG = Report_input.GetRecordCount();

			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(strText, L"分段终里程");
				CG[i].xlc = _wtof(strText);

				Report_input.GetValueString(strText, L"路面加宽");
				CG[i].cg = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"左路面宽分段表" || TabFname == L"右路面宽分段表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			bool IsAddField = false;//是否需要插入"路基中线偏移量"列
			int FieldSum = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum - 1) != L"路基中线偏移量")//数据库中没有“路基中线偏移量”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  路基中线偏移量   DOUBLE ", TabFname);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname, RoadName, TabFname);
			}
			if (lmk) { delete[]lmk; lmk = NULL; }
			NLMK = Report_input.GetRecordCount();
			lmk = new LMKdata[NLMK];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(strText, L"分段终里程");
				lmk[i].xlc = _wtof(strText);

				Report_input.GetValueString(strText, L"分隔带半宽");
				lmk[i].fgdk = _wtof(strText);

				Report_input.GetValueString(strText, L"分隔带横坡");
				lmk[i].fgdhp = _wtof(strText);

				Report_input.GetValueString(strText, L"路缘带宽");
				lmk[i].lydk = _wtof(strText);

				Report_input.GetValueString(strText, L"车行道宽");
				lmk[i].cxdk = _wtof(strText);

				Report_input.GetValueString(strText, L"路面横坡");
				lmk[i].lmhp = _wtof(strText);

				Report_input.GetValueString(strText, L"硬路肩宽");
				lmk[i].yljk = _wtof(strText);

				Report_input.GetValueString(strText, L"硬路肩横坡");
				lmk[i].yljhp = _wtof(strText);

				Report_input.GetValueString(strText, L"土路肩宽");
				lmk[i].tljk = _wtof(strText);

				Report_input.GetValueString(strText, L"土路肩横坡");
				lmk[i].tljhp = _wtof(strText);

				Report_input.GetValueString(strText, L"变宽方式");
				lmk[i].bkmode = _wtoi(strText);

				Report_input.GetValueString(strText, L"设计标高位置");
				lmk[i].sjbgmode = _wtoi(strText);

				Report_input.GetValueString(strText, L"路基中线偏移量");
				if (strText == L"NULL")
				{
					Report_input.PutCollect(14, 0.0);
					lmk[i].OffsetByXLZX = 0.0;
				}
				else
					lmk[i].OffsetByXLZX = _wtof(strText);
				// 				if (IsAddField)
				// 				{
				// 					Report_input.PutCollect(14,0.0);
				// 					lmk[i].OffsetByXLZX=0.0;
				// 				}
				// 				else
				// 				{
				// 					Report_input.GetValueString(strText,"路基中线偏移量");	
				// 					lmk[i].OffsetByXLZX = _wtof(strText);
				// 				}

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"左爬坡车道分段表" || TabFname == L"右爬坡车道分段表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);			
			if (ppcd) { delete[]ppcd; ppcd = NULL; }
			NPPCD = Report_input.GetRecordCount();
			ppcd = new PPCDdata[NPPCD];
			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NDL*100);
				Report_input.GetValueString(strText, L"分段起始里程");
				ppcd[i].sdml = _wtof(strText);

				Report_input.GetValueString(strText, L"分段终止里程");
				ppcd[i].edml = _wtof(strText);

				Report_input.GetValueString(strText, L"最大横坡");
				ppcd[i].max_hp = _wtof(strText);


				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"控制点表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);

			if (JdCenArray) { delete[]JdCenArray; JdCenArray = NULL; }
			NJdCen = Report_input.GetRecordCount();
			if (NJdCen > 0)
				JdCenArray = new JdCenter[NJdCen];

			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NJdCen*100);
				Report_input.GetValueString(strText, L"控制点属性");
				if (strText == L"交点")JdCenArray[i].JDXZ = IsJiaoDian;
				else JdCenArray[i].JDXZ = IsCenter;

				Report_input.GetValueString(strText, L"坐标N");
				JdCenArray[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"坐标E");
				JdCenArray[i].E = _wtof(strText);

				Report_input.GetValueString(strText, L"前缓和曲线A1");
				JdCenArray[i].A1 = _wtof(strText);

				Report_input.GetValueString(strText, L"曲线半径R");
				JdCenArray[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"后缓和曲线A2");
				JdCenArray[i].A2 = _wtof(strText);

				Report_input.GetValueString(strText, L"转向");
				JdCenArray[i].IsOrNotHuiTou = false;
				if (JdCenArray[i].JDXZ == IsJiaoDian)
				{
					if (strText == L"顺向左转回头")JdCenArray[i].IsOrNotHuiTou = 1;
					else if (strText == L"顺向右转回头")JdCenArray[i].IsOrNotHuiTou = 2;
					else if (strText == L"逆向左转回头")JdCenArray[i].IsOrNotHuiTou = 3;
					else if (strText == L"逆向右转回头")JdCenArray[i].IsOrNotHuiTou = 4;
					else JdCenArray[i].IsOrNotHuiTou = false;
				}
				else
				{
					if (strText == L"左转")JdCenArray[i].LeftOrRight = -1;
					else JdCenArray[i].LeftOrRight = 1;
				}

				Report_input.GetValueString(strText, L"后接关系");
				if (strText == L"S型")JdCenArray[i].LinkAfter = S_Link;
				else if (strText == L"C型")JdCenArray[i].LinkAfter = C_Link;
				else if (strText == L"卵型")JdCenArray[i].LinkAfter = Luan_Link;
				else JdCenArray[i].LinkAfter = NotRelation;

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"线元表")
		{
			if (XYArray) { delete[]XYArray; XYArray = NULL; }
			XYNum = Report_input.GetRecordCount();
			if (XYNum > 0)
				XYArray = new double[XYNum][10];

			int i = 0;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//acedSetStatusBarProgressMeterPos(i/NJdCen*100);				
				Report_input.GetValueString(strText, L"线元性质");
				XYArray[i][0] = _wtof(strText);
				Report_input.GetValueString(strText, L"LAR");
				XYArray[i][1] = _wtof(strText);
				Report_input.GetValueString(strText, L"LR");
				XYArray[i][2] = _wtof(strText);
				Report_input.GetValueString(strText, L"卵型后R");
				XYArray[i][3] = _wtof(strText);
				Report_input.GetValueString(strText, L"左右转");
				XYArray[i][4] = _wtof(strText);
				Report_input.GetValueString(strText, L"起始方位角");
				XYArray[i][5] = _wtof(strText);
				Report_input.GetValueString(strText, L"起始里程");
				XYArray[i][6] = _wtof(strText);
				Report_input.GetValueString(strText, L"半径R");
				XYArray[i][7] = _wtof(strText);
				Report_input.GetValueString(strText, L"起始坐标N");
				XYArray[i][8] = _wtof(strText);
				Report_input.GetValueString(strText, L"起始坐标E");
				XYArray[i][9] = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}
			XYNum = XYNum - 1;
		}
		else if (TabFname == L"水准点表")
		{
			int i = 0;
			BmNum = Report_input.GetRecordCount();
			if (BM) { delete[]BM; BM = NULL; }
			if (BmNum > 0)
				BM = new BMSTRUCT[BmNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(BM[i].BianHao, L"水准点编号");
				Report_input.GetValueString(strText, L"高程");
				BM[i].GaoCheng = _wtof(strText);
				Report_input.GetValueString(strText, L"纬距");
				BM[i].N = _wtof(strText);
				Report_input.GetValueString(strText, L"经距");
				BM[i].E = _wtof(strText);
				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"导线表")
		{
			int i = 0;
			DXNum = Report_input.GetRecordCount();
			if (DX) { delete[]DX; DX = NULL; }
			if (DXNum > 0)
				DX = new DXSTRUCT[DXNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(DX[i].BianHao, L"编号");
				Report_input.GetValueString(strText, L"高程");
				DX[i].GaoCheng = _wtof(strText);
				Report_input.GetValueString(strText, L"纬距");
				DX[i].N = _wtof(strText);
				Report_input.GetValueString(strText, L"经距");
				DX[i].E = _wtof(strText);
				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"坡度表")//铁路坡度表
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			int i = 0;
			NPD = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (PDB) { delete[]PDB, PDB = NULL; }
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].GH, L"变坡点里程冠号");
				Report_input.GetValueString(strText, L"变坡点里程数");
				PDB[i].ml = _wtof(strText);
				if (pzLinep)
					PDB[i].cml = pzLinep->TYLC(PDB[i].ml);
				Report_input.GetValueString(strText, L"设计高程");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线半径");
				PDB[i].Rshu = _wtof(strText);
				Report_input.GetValueString(strText, L"竖曲线长度");
				PDB[i].RLen = _wtof(strText);
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"小桥涵")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			int i = 0;
			NCU = Report_input.GetRowCount();
			//	ads_printf(L"1:NPD=%d\n",NPD);
			if (CUL) { delete[]CUL, CUL = NULL; }
			if (NCU > 0)
				CUL = new Culvet[NCU];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(CUL[i].QHType, L"桥涵类型");
				Report_input.GetValueString(strText, L"中心里程");
				CUL[i].CenDml = _wtof(strText);
				if (pzLinep)
					CUL[i].cml = pzLinep->TYLC(CUL[i].CenDml);
				Report_input.GetValueString(CUL[i].KKMS, L"孔跨数及孔跨径");
				Report_input.GetValueString(strText, L"标高");
				CUL[i].ht = _wtof(strText);
				Report_input.GetValueString(CUL[i].QHMS, L"桥涵名");
				Report_input.GetValueString(strText, L"夹角");
				CUL[i].ang = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"纵地面线表")
		{
			//acedSetStatusBarProgressMeter("读取数据",0,100);
			int i = 0;
			NTW = Report_input.GetRowCount();
			if (TWB) { delete[]TWB, TWB = NULL; }
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//Report_input.GetValueString(TWB[i].GH,L"加桩里程冠号");		//20190806暂时屏蔽
				Report_input.GetValueString(strText, L"加桩里程数");
				TWB[i].ml = _wtof(strText);
				//	          if(fmod(TWB[i].ml,100)==0)
				//			  {
				if (pzLinep)
					TWB[i].cml = pzLinep->TYLC(TWB[i].ml);
				Report_input.GetValueString(strText, L"地面高程");
				TWB[i].DMGC = _wtof(strText);
				Hmin = Hmin < TWB[i].DMGC ? Hmin : TWB[i].DMGC;
				Hmax = Hmax > TWB[i].DMGC ? Hmax : TWB[i].DMGC;
				i++;
				//			  }	
				Report_input.MoveNext();
			}
			NTW = i;
		}
		else if (TabFname == L"大中桥")
		{
			int i = 0;
			BriNum = Report_input.GetRowCount();
			if (Bri) { delete[]Bri, Bri = NULL; }
			if (BriNum > 0)
				Bri = new BRITAB[BriNum];

			Report_input.MoveFirst();
			double cml;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Bri[i].BriName, 1);
				//				Report_input.GetValueString(Bri[i].CGH,2);	//中心里程冠号			
				Report_input.GetValueString(strText, 2);
				Bri[i].CenDml = _wtof(strText);
				//				Report_input.GetValueString(Bri[i].BGH,4);	//起始里程冠号			
				Report_input.GetValueString(strText, 3);
				Bri[i].StDml = _wtof(strText);
				//				Report_input.GetValueString(Bri[i].EGH,6);	//终止里程冠号			
				Report_input.GetValueString(strText, 4);
				Bri[i].EdDml = _wtof(strText);
				Report_input.GetValueString(Bri[i].KKSY, 5);
				Report_input.GetValueString(Bri[i].SWHt, 7);
				int pos = Bri[i].SWHt.Find(L".");
				if (pos > 0)
					Bri[i].SWHt = Bri[i].SWHt.Left(pos + 3);
				Report_input.GetValueString(Bri[i].XZ, L"性质");
				Report_input.GetValueString(strText, L"桥头交角");
				Bri[i].HeadAng = _wtof(strText);
				Report_input.GetValueString(strText, L"桥尾交角");
				Bri[i].TailAng = _wtof(strText);
				Report_input.GetValueString(strText, L"左占地宽");
				Bri[i].ZZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"右占地宽");
				Bri[i].YZDK = _wtof(strText);
				if (pzLinep)
				{
					cml = pzLinep->TYLC(Bri[i].CenDml);
					_tcscpy(Bri[i].CKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Bri[i].StDml);
					_tcscpy(Bri[i].SKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Bri[i].EdDml);
					_tcscpy(Bri[i].EKml, pzLinep->XLC(cml));
				}

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"隧道")
		{
			int i = 0;
			TunNum = Report_input.GetRowCount();

			if (Tun) { delete[]Tun, Tun = NULL; }
			if (TunNum > 0)
				Tun = new TUNTAB[TunNum];

			double cml;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Tun[i].TunName, 1);
				//				Report_input.GetValueString(Tun[i].BGH,2);				
				Report_input.GetValueString(strText, 2);
				Tun[i].StDml = _wtof(strText);
				//				Report_input.GetValueString(Tun[i].EGH,4);				
				Report_input.GetValueString(strText, 3);
				Tun[i].EdDml = _wtof(strText);
				Report_input.GetValueString(Tun[i].XZ, L"性质");
				Report_input.GetValueString(strText, L"左占地宽");
				Tun[i].ZZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"右占地宽");
				Tun[i].YZDK = _wtof(strText);
				if (pzLinep)
				{
					cml = pzLinep->TYLC(Tun[i].StDml);
					_tcscpy(Tun[i].SKml, pzLinep->XLC(cml));

					cml = pzLinep->TYLC(Tun[i].EdDml);
					_tcscpy(Tun[i].EKml, pzLinep->XLC(cml));
				}
				i++;

				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"松方压实系数")
		{
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())	//按理应该只有一行
			{
				Report_input.GetValueString(strText, 1);
				YSXS[0] = _wtof(strText);
				Report_input.GetValueString(strText, 2);
				YSXS[1] = _wtof(strText);
				Report_input.GetValueString(strText, 3);
				YSXS[2] = _wtof(strText);
				Report_input.GetValueString(strText, 4);
				YSXS[3] = _wtof(strText);
				Report_input.GetValueString(strText, 5);
				YSXS[4] = _wtof(strText);
				Report_input.GetValueString(strText, 6);
				YSXS[5] = _wtof(strText);

				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"填方分类")
		{
			LDTLdata tempType;
			m_FillType.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())	//按理应该只有一行
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempType.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"填料类型");
				tempType.type = _wtoi(strText);

				m_FillType.push_back(tempType);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"用地面积汇总分段表")
		{
			YongDiFDB tempSection;
			m_EarthSection.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempSection.dml = _wtof(strText);

				m_EarthSection.push_back(tempSection);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"挖方分类")
		{
			WFFLdata tempCut;
			m_CutType.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempCut.dml = _wtof(strText);

				for (int j = 0; j < 6; j++)
				{
					Report_input.GetValueString(strText, j + 3);
					tempCut.sj[j] = _wtof(strText);
				}
				m_CutType.push_back(tempCut);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"关联断面路肩拉直")
		{
			GLDMLZdata tempGLDMLZ;
			m_GLDMLZ.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段起始里程");
				tempGLDMLZ.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"分段终止里程");
				tempGLDMLZ.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"水平距离");
				tempGLDMLZ.dX = _wtof(strText);
				Report_input.GetValueString(strText, L"高差");
				tempGLDMLZ.dH = _wtof(strText);

				m_GLDMLZ.push_back(tempGLDMLZ);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"关联断面路堑边坡推平")
		{
			GLDMLQBPTPdata tempGLDMLQBPTP;
			m_GLDMLQBPTP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段起始里程");
				tempGLDMLQBPTP.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"分段终止里程");
				tempGLDMLQBPTP.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"水平距离");
				tempGLDMLQBPTP.dX = _wtof(strText);

				m_GLDMLQBPTP.push_back(tempGLDMLQBPTP);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"关联断面线间沟")
		{
			GLDMXJGdata tempGLDMXJG;
			m_GLDMXJG.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段起始里程");
				tempGLDMXJG.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"分段终止里程");
				tempGLDMXJG.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"沟宽");
				tempGLDMXJG.GouWide = _wtof(strText);
				Report_input.GetValueString(strText, L"沟深");
				tempGLDMXJG.GouHigh = _wtof(strText);
				Report_input.GetValueString(strText, L"内边坡");
				tempGLDMXJG.NBP = _wtof(strText);
				Report_input.GetValueString(strText, L"外边坡");
				tempGLDMXJG.WBP = _wtof(strText);
				Report_input.GetValueString(strText, L"浆砌厚度");
				tempGLDMXJG.JQHD = _wtof(strText);
				Report_input.GetValueString(strText, L"沟底半径");
				tempGLDMXJG.GDR = _wtof(strText);

				m_GLDMXJG.push_back(tempGLDMXJG);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左清表土厚度" || TabFname == L"右清表土厚度")
		{
			hthddata temphthd;
			m_HTHD.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				temphthd.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"清表土厚度");
				temphthd.hthd = _wtof(strText);
				Report_input.GetValueString(strText, L"清表土性质");
				temphthd.xz = _wtof(strText);

				m_HTHD.push_back(temphthd);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左沟底坡" || TabFname == L"右沟底坡")
		{
			GouPDdata tempGouPD;
			m_GouPD.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempGouPD.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始高程");
				tempGouPD.SH = _wtof(strText);
				Report_input.GetValueString(strText, L"终止里程");
				tempGouPD.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止高程");
				tempGouPD.EH = _wtof(strText);

				m_GouPD.push_back(tempGouPD);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左超填宽度表" || TabFname == L"右超填宽度表")
		{
			ctkddata tempctkd;
			m_CTKD.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempctkd.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始宽度");
				tempctkd.Sctkd = _wtof(strText);
				Report_input.GetValueString(strText, L"终止里程");
				tempctkd.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止宽度");
				tempctkd.Ectkd = _wtof(strText);

				m_CTKD.push_back(tempctkd);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左弧形边坡表" || TabFname == L"右弧形边坡表")
		{
			arcbpata temparcbp;
			m_ARCBP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				temparcbp.Sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始半径");
				temparcbp.SR = _wtof(strText);
				Report_input.GetValueString(strText, L"终止里程");
				temparcbp.Edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止半径");
				temparcbp.ER = _wtof(strText);
				Report_input.GetValueString(strText, L"设置位置");
				temparcbp.ArcLocation = _wtof(strText);

				m_ARCBP.push_back(temparcbp);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左挡墙" || TabFname == L"右挡墙")
		{
			dqdata tempdq;
			m_DQ.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempdq.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始支距");
				tempdq.szj = _wtof(strText);
				Report_input.GetValueString(strText, L"终点里程");
				tempdq.edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止支距");
				tempdq.ezj = _wtof(strText);

				m_DQ.push_back(tempdq);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左低填路堤自动填平" || TabFname == L"右低填路堤自动填平")
		{
			LDTPData tempLDTP;
			m_LDTP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempLDTP.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"终点里程");
				tempLDTP.edml = _wtof(strText);

				m_LDTP.push_back(tempLDTP);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左低挖路堑自动推平" || TabFname == L"右低挖路堑自动推平")
		{
			LQTPData tempLQTP;
			m_LQTP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempLQTP.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"终点里程");
				tempLQTP.edml = _wtof(strText);

				m_LQTP.push_back(tempLQTP);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左低填路堤自动加宽土路肩" || TabFname == L"右低填路堤自动加宽土路肩")
		{
			TLJJKData tempTLJJK;
			m_TLJJK.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"起始里程");
				tempTLJJK.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"终点里程");
				tempTLJJK.edml = _wtof(strText);

				m_TLJJK.push_back(tempTLJJK);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左路面厚度" || TabFname == L"右路面厚度")
		{
			LMHdata tempLMH;
			m_LMHD.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				//20191022	老版本数据库由于从公路标准模板里得来是“分段终里程”，这里考虑兼容性读取（公路标准模板现已更新）
				strText.Empty();
				Report_input.GetValueString(strText, L"分段终里程数");
				if (strText == L"")
					Report_input.GetValueString(strText, L"分段终里程");
				tempLMH.dml = _wtof(strText);

				Report_input.GetValueString(strText, L"车行道厚");
				tempLMH.cxdh = _wtof(strText);
				Report_input.GetValueString(strText, L"硬路肩厚");
				tempLMH.yljh = _wtof(strText);
				Report_input.GetValueString(strText, L"土路肩厚");
				tempLMH.tljh = _wtof(strText);

				m_LMHD.push_back(tempLMH);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左地面线内插宽" || TabFname == L"右地面线内插宽")
		{
			crswiddata tempcrswid;
			m_CRSWID.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempcrswid.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"内插宽");
				tempcrswid.wid = _wtof(strText);

				m_CRSWID.push_back(tempcrswid);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左横断面限制宽度" || TabFname == L"右横断面限制宽度")
		{
			dmdata tempdm;
			m_DM.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempdm.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"路基限宽");
				tempdm.xk = _wtof(strText);

				m_DM.push_back(tempdm);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左用地加宽" || TabFname == L"右用地加宽")
		{
			pwddata temppwd;
			m_PWD.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				temppwd.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"路堤加宽");
				temppwd.ltjk = _wtof(strText);
				Report_input.GetValueString(strText, L"路堑加宽");
				temppwd.lqjk = _wtof(strText);

				m_PWD.push_back(temppwd);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左路堤边坡" || TabFname == L"右路堤边坡")
		{
			LDBPdata tempLDBP;
			m_LDBP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempLDBP.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"边坡总级数");
				tempLDBP.BPJS = _wtof(strText);

				for (int j = 0; j < tempLDBP.BPJS * 4; j++)
				{
					Report_input.GetValueString(strText, 4 + j);
					tempLDBP.bpdata[j] = _wtof(strText);
				}

				m_LDBP.push_back(tempLDBP);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左路堑边坡" || TabFname == L"右路堑边坡")
		{
			LDBPdata tempLDBP;
			m_LQBP.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempLDBP.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"边坡总级数");
				tempLDBP.BPJS = _wtof(strText);

				for (int j = 0; j < tempLDBP.BPJS * 4; j++)
				{
					Report_input.GetValueString(strText, 4 + j);
					tempLDBP.bpdata[j] = _wtof(strText);
				}

				m_LQBP.push_back(tempLDBP);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左排水沟" || TabFname == L"右排水沟")
		{
			LDSGdata tempLDSG;
			m_LDSG.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"坡脚墙");
				tempLDSG.WallorGou = _wtof(strText);
				Report_input.GetValueString(strText, L"浆砌厚度");
				tempLDSG.jh = _wtof(strText);
				Report_input.GetValueString(strText, L"起始里程");
				tempLDSG.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始护坡道宽");
				tempLDSG.S_HPDK = _wtof(strText);
				Report_input.GetValueString(strText, L"起始护坡道厚");
				tempLDSG.S_HPDH = _wtof(strText);
				Report_input.GetValueString(strText, L"起始水沟底宽");
				tempLDSG.S_GS = _wtof(strText);
				Report_input.GetValueString(strText, L"起始水沟深");
				tempLDSG.S_GK = _wtof(strText);
				Report_input.GetValueString(strText, L"起始挡水堰宽");
				tempLDSG.S_DSYK = _wtof(strText);
				Report_input.GetValueString(strText, L"起始挡水堰厚");
				tempLDSG.S_DSYH = _wtof(strText);
				Report_input.GetValueString(strText, L"终止里程");
				tempLDSG.edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止护坡道宽");
				tempLDSG.E_HPDK = _wtof(strText);
				Report_input.GetValueString(strText, L"终止护坡道厚");
				tempLDSG.E_HPDH = _wtof(strText);
				Report_input.GetValueString(strText, L"终止水沟底宽");
				tempLDSG.E_GS = _wtof(strText);
				Report_input.GetValueString(strText, L"终止水沟深");
				tempLDSG.E_GK = _wtof(strText);
				Report_input.GetValueString(strText, L"终止挡水堰宽");
				tempLDSG.E_DSYK = _wtof(strText);
				Report_input.GetValueString(strText, L"终止挡水堰厚");
				tempLDSG.E_DSYH = _wtof(strText);
				Report_input.GetValueString(strText, L"内侧边坡");
				tempLDSG.nbp = _wtof(strText);
				Report_input.GetValueString(strText, L"外侧边坡");
				tempLDSG.wbp = _wtof(strText);
				Report_input.GetValueString(strText, L"挡水堰坡");
				tempLDSG.DSYP = _wtof(strText);
				Report_input.GetValueString(strText, L"沟底半径");
				tempLDSG.LDSG_GDR = _wtof(strText);

				m_LDSG.push_back(tempLDSG);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左侧沟" || TabFname == L"右侧沟")
		{
			LQSGdata tempLQSG;
			m_LQSG.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段终里程");
				tempLQSG.dml = _wtof(strText);
				Report_input.GetValueString(strText, L"水沟底宽");
				tempLQSG.gw = _wtof(strText);
				Report_input.GetValueString(strText, L"沟深");
				tempLQSG.gs = _wtof(strText);
				if (TabFname == L"右侧沟")
				{
					Report_input.GetValueString(strText, L"内侧边坡");
					tempLQSG.nbp = _wtof(strText);
					Report_input.GetValueString(strText, L"外侧边坡");
					tempLQSG.wbp = _wtof(strText);
				}
				else if (TabFname == L"左侧沟")
				{
					Report_input.GetValueString(strText, L"左侧边坡");
					tempLQSG.nbp = _wtof(strText);
					Report_input.GetValueString(strText, L"右侧边坡");
					tempLQSG.wbp = _wtof(strText);
				}
				Report_input.GetValueString(strText, L"浆砌厚度");
				tempLQSG.Jh = _wtof(strText);
				Report_input.GetValueString(strText, L"沟底半径");
				tempLQSG.GDR = _wtof(strText);

				m_LQSG.push_back(tempLQSG);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左天沟" || TabFname == L"右天沟")
		{
			TGdata tempTG;
			m_TG.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"分段起里程");
				tempTG.sdml = _wtof(strText);
				Report_input.GetValueString(strText, L"起始边距");
				tempTG.sbj = _wtof(strText);
				Report_input.GetValueString(strText, L"起始水沟深");
				tempTG.sgs = _wtof(strText);
				Report_input.GetValueString(strText, L"起始水沟底宽");
				tempTG.sgw = _wtof(strText);
				Report_input.GetValueString(strText, L"分段终里程");
				tempTG.edml = _wtof(strText);
				Report_input.GetValueString(strText, L"终止边距");
				tempTG.ebj = _wtof(strText);
				Report_input.GetValueString(strText, L"终止水沟深");
				tempTG.egs = _wtof(strText);
				Report_input.GetValueString(strText, L"终止水沟底宽");
				tempTG.egw = _wtof(strText);
				Report_input.GetValueString(strText, L"内侧边坡");
				tempTG.nbp = _wtof(strText);
				Report_input.GetValueString(strText, L"外侧边坡");
				tempTG.wbp = _wtof(strText);
				Report_input.GetValueString(strText, L"浆砌厚度");
				tempTG.jqhd = _wtof(strText);
				Report_input.GetValueString(strText, L"沟底半径");
				tempTG.TG_GDR = _wtof(strText);

				m_TG.push_back(tempTG);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"左路面分层表" || TabFname == L"右路面分层表")
		{
			RoadLayerData tempRoadLayer;
			m_RoadLayerArray.clear();
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"填料类型");
				tempRoadLayer.LayerName = strText;
				Report_input.GetValueString(strText, L"厚度");
				tempRoadLayer.LayerThickness = _wtof(strText);

				m_RoadLayerArray.push_back(tempRoadLayer);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"高程控制点表")
		{
			if (m_LevelCtrlPtArray.size())
				m_LevelCtrlPtArray.clear();
			GCKZPt tempLevelCtrlPt;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"里程冠号");
				_tcscpy(tempLevelCtrlPt.GH, strText);

				Report_input.GetValueString(strText, L"道路名");
				tempLevelCtrlPt.RoadName = strText;

				Report_input.GetValueString(strText, L"里程数");
				tempLevelCtrlPt.XCLc = _wtof(strText);

				Report_input.GetValueString(strText, L"高程上限");
				tempLevelCtrlPt.DesHSX = _wtof(strText);

				Report_input.GetValueString(strText, L"高程下限");
				tempLevelCtrlPt.DesHXX = _wtof(strText);

				Report_input.GetValueString(strText, L"净空");
				tempLevelCtrlPt.clearrence = _wtof(strText);

				Report_input.GetValueString(strText, L"控制点性质");
				tempLevelCtrlPt.Style = _wtoi(strText);

				Report_input.GetValueString(tempLevelCtrlPt.Note, L"附注");

				m_LevelCtrlPtArray.push_back(tempLevelCtrlPt);

				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"接坡信息表")
		{
			if (m_JPArray.size())
				m_JPArray.clear();
			JPInfor tempJPInfor;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"道路名");
				tempJPInfor.RoadName = strText;

				Report_input.GetValueString(strText, L"前接坡高程");
				tempJPInfor.Bef_H = _wtof(strText);

				Report_input.GetValueString(strText, L"前接坡坡度");
				tempJPInfor.Bef_PD = _wtof(strText);

				Report_input.GetValueString(strText, L"前接坡坡长");
				tempJPInfor.Bef_PC = _wtof(strText);

				Report_input.GetValueString(strText, L"后接坡高程");
				tempJPInfor.Aft_H = _wtof(strText);

				Report_input.GetValueString(strText, L"后接坡坡度");
				tempJPInfor.Aft_PD = _wtof(strText);

				Report_input.GetValueString(strText, L"后接坡坡长");
				tempJPInfor.Aft_PC = _wtof(strText);

				Report_input.GetValueString(strText, L"是否接坡");
				tempJPInfor.Is_JP = _tstoi(strText);

				Report_input.GetValueString(strText, L"起始接坡");
				tempJPInfor.Is_JP_Bef = _tstoi(strText);

				Report_input.GetValueString(strText, L"终止接坡");
				tempJPInfor.Is_JP_Aft = _tstoi(strText);

				m_JPArray.push_back(tempJPInfor);

				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"道铁交叉信息表")
		{
			if (m_RoadNRailInterInforArray.size())
				m_RoadNRailInterInforArray.clear();
			ConstraintErr tempInfor;
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"连续里程");
				tempInfor.CurDml = _wtof(strText);

				Report_input.GetValueString(strText, L"现场里程");
				_tcscpy(tempInfor.InterCkml, strText);

				Report_input.GetValueString(strText, L"N坐标");
				tempInfor.InterN = _wtof(strText);

				Report_input.GetValueString(strText, L"E坐标");
				tempInfor.interE = _wtof(strText);

				Report_input.GetValueString(strText, L"道路方位角");
				tempInfor.CurFwj = _wtof(strText);

				Report_input.GetValueString(strText, L"铁路方位角");
				tempInfor.InterFwj = _wtof(strText);

				Report_input.GetValueString(strText, L"夹角");
				tempInfor.JiaJiao = _wtof(strText);

				Report_input.GetValueString(strText, L"道路设计高程");
				tempInfor.CurDesH = _wtof(strText);

				Report_input.GetValueString(strText, L"铁路设计高程");
				tempInfor.InterDesH = _wtof(strText);

				Report_input.GetValueString(strText, L"高程差");
				tempInfor.DetaH = _wtof(strText);

				Report_input.GetValueString(strText, L"最小角度");
				tempInfor.MinAng = _wtof(strText);

				Report_input.GetValueString(strText, L"最小高差");
				tempInfor.MinDetaH = _wtof(strText);

				m_RoadNRailInterInforArray.push_back(tempInfor);

				Report_input.MoveNext();
			}
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();
	//acedRestoreStatusBar();	
	return 1;
}

int XLDataBase::ReadAllRoadName(CString mdbFname)//读取线路数据库中断链表的数据
{
	CString strText, GH;
	m_RoadSum = 0;
	AfxOleInit();
	try
	{
		// 打开本地Access库
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"连接数据库失败!打开道路");
			return 0;
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
	}

	m_strSQL = L"select * from 断链表";
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
			return 0;
		}
		Report_input.MoveFirst();
		CString rname = L"";
		while (!Report_input.IsEOF())
		{
			Report_input.GetValueString(rname, L"道路名");
			PutRnameInArray(rname);//将道路名入库
			Report_input.MoveNext();
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	//ads_printf(L"%s中共%d条道路\n",mdbFname,m_RoadSum);
	for (int i = 0; i < m_RoadSum; i++)
	{
		//	if(Road[i].RoadName != "主线")
		SetRoadSEml(mdbFname, Road[i].RoadName, i);
	}
	SortRoad();
	return 1;
	}

int XLDataBase::ReadAllXZ(CString mdbFname, CString RoadName)//读取线路数据库中坡度表的数据
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
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
}
	RoadZ = RoadY = L"";
	m_strSQL = L"select * from 坡度表";
	m_strSQL += L" where 道路名='" + RoadName;
	m_strSQL += L"'";
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
			//     ads_printf(L"没有记录!");
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();
		CString rname = L"";
		while (!Report_input.IsEOF())
		{
			Report_input.GetValueString(rname, L"性质");
			if (rname == L"左幅")
				RoadZ = L"左幅";
			if (rname == L"右幅")
				RoadY = L"右幅";
			Report_input.MoveNext();
		}
	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	return 1;
}

bool XLDataBase::PutRnameInArray(CString rname)//读取线路数据库中断链表的数据
{
	for (int i = 0; i < m_RoadSum; i++)
	{
		if (Road[i].RoadName == rname)//已存在
			return false;
	}
	//不存在
	m_RoadSum++;
	Road[m_RoadSum - 1].RoadName = rname;
	return true;
}

int XLDataBase::SetRoadSEml(CString mdbFname, CString RoadName, int iRoad)
//读取线路数据库中的断链数据，输出Road[i].SKml,EKml
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
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");
}

	m_strSQL = L"select * from 断链表";
	m_strSQL += L" where 道路名='" + RoadName;
	m_strSQL += L"'";
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
			//        ads_printf(L"没有记录!");
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();

		if (DLArray) { delete[]DLArray; DLArray = NULL; }
		NDL = Report_input.GetRecordCount();
		if (NDL > 0)
			DLArray = new DLBZ[NDL];
		int i = 0;
		Report_input.MoveFirst();
		while (!Report_input.IsEOF())
		{
			//acedSetStatusBarProgressMeterPos(i/NDL*100);
			Report_input.GetValueString(DLArray[i].BGH, L"等号左里程冠号");
			Report_input.GetValueString(DLArray[i].EGH, L"等号右里程冠号");
			Report_input.GetValueString(strText, L"等号左里程数");
			DLArray[i].BLC = _wtof(strText);
			Report_input.GetValueString(strText, L"等号右里程数");
			DLArray[i].ELC = _wtof(strText);
			Report_input.MoveNext();
			i++;
		}

	}
	catch (_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	if (NDL > 0)
	{
		//起点里程
		Road[iRoad].BGH = DLArray[0].BGH;
		Road[iRoad].BLC = DLArray[0].BLC;

		//终点里程
		Road[iRoad].EGH = DLArray[NDL - 1].EGH;
		Road[iRoad].ELC = DLArray[NDL - 1].ELC;

	}

	return 1;
}

void XLDataBase::SortRoad()
{
	int i, j, k;
	double t;
	struct RoadData tmp;
	//找出主线,保证主线为0
	for (i = 0; i < m_RoadSum; i++)
	{
		if (i > 0 && Road[i].RoadName == L"主线")
		{
			tmp = Road[0];
			Road[0] = Road[i];
			Road[i] = tmp;
		}
	}

	for (i = 1; i < m_RoadSum; i++)
		for (j = i + 1; j < m_RoadSum; j++)
		{
			if (Road[i].BLC > Road[j].BLC)
			{
				tmp = Road[i];
				Road[i] = Road[j];
				Road[j] = tmp;
			}
		}

}

//-----------------------------------------------------------------------------------------

int XLDataBase::DelAllRecords(CString mdbFname, CString TabFname)//删除表中所有记录
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
		return 0;

	strSQL.Format(L"delete from %s ", TabFname);//删除全部

	m_adoConnection.Execute(strSQL);
	m_adoConnection.Disconnect();
	return 1;
}


int XLDataBase::AttachMdb(CString HdmMdbname, CString RoadName, CString TabName)
{
	ACHAR mes[300];
	try
	{
		// 打开本地Access库
#ifdef X64
		_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", HdmMdbname);
#else
		_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", HdmMdbname);
#endif // X64

		CString strConnect = mes;
		//strConnect+="数据库.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"10 失败!");
			AfxMessageBox(L"连接数据库失败!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
		}
	catch (_com_error e)
	{
		AfxMessageBox(L"数据库连接失败，确认mdb是否在当前路径下!");
	}

	m_strSQL = L"select * from " + TabName;
	m_strSQL += L" where 道路名=";
	m_strSQL += L"'" + RoadName + L"' ORDER by 序号";
	//	m_strSQL+=" and";
	//	m_strSQL+=" 绕行段编号=";
	//	m_strSQL+=rbh;

	if (!m_adoConnection.IsOpen())
	{
		AfxMessageBox(L"数据库没有打开或已经关闭!");
		return 0;
	}
	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
		Report_input.Close();
		m_adoConnection.Disconnect();
		return 0;
	}
	return 1;
	}

int  XLDataBase::IsMdb2(CString MdbName, int& isEX)
{
	int result = -1;
	CString ProMDB;
	ACHAR fname[1024];
	//if(!ReadWorkdir())
	//{
	//	AfxMessageBox(L"请指定工作目录和方案名称!");
	//	return false;
	//}
	CString FilePath;
	ProMDB = MdbName;
	ProMDB.TrimRight();
	ProMDB.MakeLower();
	FilePath = ProMDB;

//#ifdef X64
//	FilePath = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + ProMDB;
//#else
//	FilePath = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + ProMDB;
//#endif // X64
//
//	if (!m_adoConnection.Connect(FilePath))
//	{
//
//		//	AfxMessageBox(L"连接数据库"+ProMDB+L"失败!");
//
//	}
//	Report_input.SetAdoConnection(&m_adoConnection);
//	InitTableList();
	if (Read_XLDbs(FilePath, L"曲线表"))
	{
		if (NQX > 0)
		{
			Read_XLDbs(FilePath, L"左线曲线表");
			if (NQX2x > 0)
			{

				isEX = 1;
			}
			else
			{
				Read_XLDbs(FilePath, L"右线曲线表");
				if (NQX2x > 0)
				{
					isEX = 2;
				}
			}
			result = 1;
		}
		else
		{
			result = 0;
			isEX = 3;
		}
}
	else
	{
		result = 0;
		isEX = 3;
	}


	return result;

}

















