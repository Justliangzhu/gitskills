// XLDataBase.h: interface for the XLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if	!defined(AFX_XLDATABASE_JYX_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
#define	AFX_XLDATABASE_JYX_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_

#if	_MSC_VER > 1000
#pragma	once
#endif // _MSC_VER > 1000

#include "ado.h"
#include "BAS_DRAW_FUN_JYX.h"
//#include "DrawSlope.h"
class XLDataBase_JYX  :	public BAS_DRAW_FUN_JYX
{
public:
	struct FZZDM
	{
		CString Type;//线路类型//绕行段、并行不等高段
		ACHAR StCKml[80];//起点现场里程
		ACHAR EdCKml[80];//终点现场里程

		ACHAR XLName[50];//线路名SJ2X,JY2X
		ACHAR DLName[100];//绕行段名 //绕行段127.7,并行不等高段135.5
		ACHAR BLName[50];//依赖平面名


		double spt[2];//起点坐标 spt[0]--N spt[1]--E
		double ept[2];//终点坐标

		double BCml;//起点在设计二线上的统一里程(cml)
		double ECml;//终点在设计二线上的统一里程

		double BL_BCml;//起点在设计基线上的统一里程(cml)，如果=-100说明设计二线起点绕行
		double BL_ECml;//终点在设计基线上的统一里程(cml)，如果=-100说明设计二线终点绕行

		FZZDM()
		{
			_tcscpy(StCKml, L"");
			_tcscpy(EdCKml, L"");
			_tcscpy(XLName, L"");
			_tcscpy(DLName, L"");
			_tcscpy(BLName, L"");
			spt[1] = 0.0;
			spt[0] = 0.0;
			ept[0] = 0.0;
			ept[1] = 0.0;
			BCml = 0.0;
			ECml = 0.0;
			BL_BCml = 0.0;
			BL_ECml = 0.0;

		}


	};

	XLDataBase_JYX();
	virtual	~XLDataBase_JYX();


	
	RAILSTU *GDSTRU;//轨道结构
	int	NGDSTRU;

	BasicDataOfZDM *pBasicDataOfZDM;//基本参数表
	int NBasicDataOfZDM;

	TUNTAB *Tun;
	int	TunNum;

	BRITAB *Bri;
	int	BriNum;

	DLBZ DLB[MAXDLNUM];
	int	NDL;

	//    double JDarray[MAXJDNUM][6];

	int	NJD;

	CArray<JDarrayStruct,JDarrayStruct>JDarray;

	PDTAB *PDB;
	int	NPD;

	DMXTAB *DMXB;//地面线表
	int	NDMX;

	DMXTAB *JiYouLuJianGC;//既有路肩高程表
	int	NJiYouLuJianGC;

	GMXTAB *GMXB;//轨面线表
	int NGMX;

	RoadBedTAB *RoadBed;//道床表
	int RoadBedNum;

	RoadBedTAB *ZMControlPT;//纵面控制点表
	int ZMControlPTNum;

	QXTAB  *QXB;//曲线表
	int	NQX;

	TWTAB *TWB;//填挖表
	int	NTW;

	HdmAreaData	*HDMB;//横段面数据
	int	NHDM;

	STATAB *Sta;
	int	StaNum;

	SMALLBRIDGETAB *SBri;
	int	SBriNum;

	DoubleLineData *QXB2x;
	int	NQX2x;

	EXDLB *DLB2x;//二线断链表
	int	NDL2x;//断链数

	QXJKTAB *QXJKB;//曲线加宽表
	int NQXJK;

	JiYouXianZhanDiKuan *ZuoJiYouXianZhanDiKuan;//左既有线占地宽
	int NZuoJiYouXianZhanDiKuan;
	
	JiYouXianZhanDiKuan *YouJiYouXianZhanDiKuan;//右既有线占地宽
	int NYouJiYouXianZhanDiKuan;;

	int NYDLX;//用地类型分段总数
	YongDiLXB * LXB;//用地类型表
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//既有线
	BoLiangTable *JYBoLiangTable; //拨量表
	int	NBLNum;

	//JDDataTable	*JYJDDataTable;	//交点数据表
	//int	NJDDateNum;

	OriginalMeasureTable *JYOrigMeaTable; //既有曲线原始测量资料
	int	NOrigMeaNum;

	MulFDData	*MulFDTable;  //复曲线分段信息表
	int	MulFDNum;

	BZLCStructTable	 *BZLCTable;  //标志里程
	int	BZLCNum;

	GJDInf *pGjdInf;
	int NGjdInf;

	EXGJDFDInf *pEXGJDFDInf; //2线改建段分段信息表
	int NEXGJDFDInf;

	ConnectLineTableStruct *ConnectLineTable;  //联络线表
	int ConnectLineInforNum;

	SGBXTableStruct *SGBXTable; 
	int SGBXInforNum;


	JBJXTableStruct *JBJXTable; 
	int JBJXInforNum;

	GJDAllInforStruct *GJDAllInforTable;  //改建段汇总表
	int GJDAllInforNum;

	OneGJDJDStruct *OneGJDJDarray;//改建段几何参数表
	int OneGJDNum;

	//　任意点坐标置镜拨量计算
	SZJDOrCD   *AllZJDOrCDArray;  //任意点坐标置镜　数组
	int AllZJDOrCDNum;

	STATIONSTRUCT *STATION;
	int StaTionNum;

	BMSTRUCT *BM;
	int BmNum;

	DXSTRUCT *DX;
	int DXNum;

	QXBStruct *SJQXB;  //设计曲线结果表
	int NSJQXB;

	BCTAB *BC;//
	int	NBC;


	//数据库中外移桩数组和个数
	WaiYiZhuang *m_pWYZArr;
	int m_iNWYZ;


	//数据库中纵面台帐数组和个数
	ZMTaiZhang *m_pZMTZArr;
	int m_iNZMTZ;

	//数据库中行政区划数组和个数
	XingZhengQuHua *m_pXZQHArr;
	int m_iXZQH;



	//////////////////////////////////////////////////////////////////////////
	//用地设计
	CXZYDFD *pXZYD;//行政用地分段
	int NXZYD;//行政用地分段数目

	CYDLXFD *pYDType;//用地类型分段
	int NTypeFD;//用地类型分段数目
	//////////////////////////////////////////////////////////////////////////

	FZZDM* FZZDMArray;//辅助纵断面数组	20191113新增
	int FZZDMNum;

	ZYJSBZTAB JSBZ;//技术标准
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
	CStringArray m_strTableNameList;

	int  IsJYXMdb2(CString MdbName, int &isEX);
	CString isNULLToCSting(CString strTempName);
	int	Read_XLDbs(CString mdbFname,CString	TabFname,CString wlcstring );//读取线路数据库中的数据
	int	WR_XLDbs(CString mdbFname,CString TabFname,CString wlcstring);//写入线路数据库
	bool InitTableList(void);//取出mdb中的所有表名
	bool FindTable(CString strTempName);//查找表是否存在于mdb中
	int	WR_EX_XLDbs(CString	mdbFname,CString TabFname,int irxd);//写入线路数据库中的数
	int	Read_EX_XLDbs(CString mdbFname,CString TabFname,int	irxd);//读取线路数据库中的二线数据

	void Realease();
	int	Delete_XLDbs(CString mdbFname,CString TabFname,CString wlcstring);//删除线路数据库
	void GetJDNumArrayFromBLCalTab(CString mdbFname,CString TabFname,CArray<CString,CString> &  GetJDNumArray);
	void GetJDNumArray(CAdoRecordSet m_RecSet, CArray<CString,CString> &a );
	double FromCStringTodouble(CString strText);

	int WR_EX_TWB(CString mdbFname,CString TabFname,int irxd);//写入线路数据库中的数据

//	void GetSlopeData(CString NameOfMDB,GTZX_JYX *pGTZX,CDrawSlope *pLeftSlope,CDrawSlope *pRightSlope,CString XLName="贯通设计基线");

	void GetRxdXLNameArray(CString mdbFname, bool IsJY, int ZorY, CArray<CString,CString>& RxdXLNameArray);//获取绕行段线路名数组

	//获取绕行段起终现场里程
	void GetRxdStartAndEndCKMLByRXDName(CString mdbFname, bool IsJY, int ZorY, CString RxdXLName, CString& StartCKML, CString& EndCKML);

	//读取线路数据库中的用地数据
	int Read_YD_XLDbs(CString mdbFname,CString TabName,CString XLName);

	int AttachYDMdb(CString mdbFname,CString TabName,CString XLName);

	CString RXDInWhichXL(CString mdbFname,CString RXDName);

	int GetRXDChaQi(CString mdbFname,CString strEXName,CString RXDName,CString TabFname);	//检查二线插旗数据是否在绕行段内，剔除不在的、

};

#endif // !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
