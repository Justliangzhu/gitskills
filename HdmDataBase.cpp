// HdmDataBase.cpp: implementation of the HdmDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "hdmdata.h"
#include "HdmDataBase.h"
#include "RoadDesignWorkingDatabase.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库
HdmDataBase::HdmDataBase()
{

	DLArray=NULL;
	NDL=0;
	NZDM=NYDM=0; //路基限宽
	ZDM=YDM=NULL;

	NZPWD=NYPWD=0;
	ZPWD=YPWD=NULL;//路基本体外宽

	NZHTHD=NYHTHD=0;
	ZHTHD=YHTHD=NULL;//清表土厚度

	NZCRS=NYCRS=0;
	ZCRSWID=YCRSWID=NULL;
	NZLDBP=NYLDBP=NZLQBP=NYLQBP=0;
	/////////路堑水沟
	NZLQSG=NYLQSG=0;
	ZLQSG=YLQSG=NULL;

	NZLDSG=NYLDSG=0;
	ZLDSG=YLDSG=NULL;

	NZTG=NYTG=0;
	ZTG=YTG=NULL;

	///////路堤填料
	NLDTL=0;
	LDTL=NULL;

	NWFFL=0;
	WFFL=NULL;

	NGLDMLZ = 0;
	GLDMLZ = NULL;

	NGLDMLQBPTP = 0;
	GLDMLQBPTP = NULL;



	NGLDMXJG = 0;
	GLDMXJG = NULL;

	NYD=0;
	FDB=NULL;

	NYDLX=0;
	LXB=NULL;

	NDMX=0;
	DMX=NULL;

	NZLMHD=NYLMHD=0;
	ZLMHD=YLMHD=NULL;

	NZDQ=NYDQ=0;
	ZDQ=YDQ=NULL;

	NZGouPD=0;
	ZGouPD = NULL;
	NYGouPD=0;
	YGouPD = NULL;


	NZCTKD=0;
	ZCTKD = NULL;
	NYCTKD=0;
	YCTKD = NULL;

	NZARCBP=0;
	ZARCBP = NULL;
	NYARCBP=0;
	YARCBP = NULL;

	ZLDTP = NULL;
	YLDTP = NULL;
	ZLQTP = NULL;
	YLQTP = NULL;
	ZTLJJK = NULL;
	YTLJJK = NULL;
	NZLDTP = NYLDTP = 0;
	NZLQTP = NYLQTP = 0;
	NZTLJJK = NYTLJJK = 0;
	//	Res=NULL;
	//    NRES=0;
	//    Area=NULL;
	//    NArea=0;
	//	
}

HdmDataBase::~HdmDataBase()
{
	if(DLArray) delete []DLArray; DLArray=NULL;
	NDL=0;
	if(LDTL) delete []LDTL; LDTL=NULL;
	if(WFFL) delete []WFFL; WFFL=NULL;
	if(FDB) delete []FDB; FDB=NULL;
	if(LXB) delete []LXB; LXB=NULL;
	//	if(Res) delete []Res; Res=NULL;
	//	if(Area) delete []Area; Area=NULL;

	if(ZDM) delete []ZDM; ZDM=NULL;
	if(ZPWD) delete []ZPWD; ZPWD=NULL;
	if(ZHTHD) delete []ZHTHD; ZHTHD=NULL;
	if(ZCRSWID) delete []ZCRSWID; ZCRSWID=NULL;
	if(ZTG) delete []ZTG; ZTG=NULL;
	if(ZLDSG) delete []ZLDSG; ZLDSG=NULL;
	if(ZLMHD) delete []ZLMHD; ZLMHD=NULL;
	if(ZDQ) delete []ZDQ; ZDQ=NULL;

	if(YDM) delete []YDM; YDM=NULL;
	if(YPWD) delete []YPWD; YPWD=NULL;
	if(YHTHD) delete []YHTHD; YHTHD=NULL;
	if(YCRSWID) delete []YCRSWID; YCRSWID=NULL;
	if(YTG) delete []YTG; YTG=NULL;
	if(YLDSG) delete []YLDSG; YLDSG=NULL;
	if(YLMHD) delete []YLMHD; YLMHD=NULL;
	if(YDQ) delete []YDQ; YDQ=NULL;

	NZCTKD=0;
	if(ZCTKD) delete []ZCTKD; ZCTKD= NULL;
	NYCTKD=0;
	if(YCTKD) delete []YCTKD; YCTKD= NULL;

	NZARCBP=0;
	if(ZARCBP) delete []ZARCBP; ZARCBP= NULL;
	NYARCBP=0;
	if(YARCBP) delete []YARCBP; YARCBP= NULL;


	NZPWD=NYPWD=0;
	NZHTHD=NYHTHD=0;
	NZCRS=NYCRS=0;
	NZLDBP=NYLDBP=NZLQBP=NYLQBP=0;
	NZLQSG=NYLQSG=0;
	NZLDSG=NYLDSG=0;
	NZTG=NYTG=0;
	NLDTL=0;
	NWFFL=0;
	NGLDMLZ = 0;
	NGLDMLQBPTP = 0;
	if (GLDMLZ) delete []GLDMLZ; GLDMLZ = NULL;
	if (GLDMXJG) delete []GLDMXJG; GLDMXJG = NULL;
	if (GLDMLQBPTP) delete []GLDMLQBPTP; GLDMLQBPTP = NULL;

	NGLDMXJG = 0;
	NYD=0;
	NYDLX=0;
	NDMX=0;
	NZLMHD=NYLMHD=0;
	//   NRES=0;
	//   NArea=0;
	NZDQ=0;
	NYDQ=0;

	if (ZLDTP) delete []ZLDTP; ZLDTP = NULL;
	if (YLDTP) delete []YLDTP; YLDTP = NULL;
	if (ZLQTP) delete []ZLQTP; ZLQTP = NULL;
	if (YLQTP) delete []YLQTP; YLQTP = NULL;
	if (ZTLJJK) delete []ZTLJJK; ZTLJJK = NULL;
	if (YTLJJK) delete []YTLJJK; YTLJJK = NULL;
	NZLDTP = NYLDTP = 0;
	NZLQTP = NYLQTP = 0;
	NZTLJJK = NYTLJJK = 0;

}

//输入线路性质和绕行段编号
//m_xlxz-- 线路性质 m_rbh--绕行段编号
//形成与之对应的横短面标准数据记录集
int HdmDataBase::AttachMdb(CString HdmMdbname,CString RoadName,CString TabName)
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
			AfxMessageBox(L"连接数据库失败!");			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"数据库连接失败，确认mdb是否在当前路径下!");		
	} 

	m_strSQL =L"select * from "+TabName;
	m_strSQL+=L" where 道路名=";
	m_strSQL+= L"'"+RoadName+"' ORDER by 序号";
	//	m_strSQL+=" and";
	//	m_strSQL+=" 绕行段编号=";
	//	m_strSQL+=rbh;

	if (!m_adoConnection.IsOpen())
	{
		AfxMessageBox(L"数据库没有打开或已经关闭!");
		return 0;
	}
	Report_input.SetAdoConnection(&m_adoConnection);
	if(Report_input.Open(m_strSQL, adCmdText)!=1)
	{
		Report_input.Close();
		m_adoConnection.Disconnect();
		return 0;
	}
	return 1;
}
int HdmDataBase::GetRoadSEml(CString mdbFname,CString RoadName,double &Sxlc,double &Exlc)
	//读取线路数据库中的断链数据，输出Road[i].SKml,EKml
{
	CString strText,GH;
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
	catch(_com_error e)
	{		
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");		
	} 	

	m_strSQL ="select * from 断链表";
	m_strSQL += L" where 道路名='"+ RoadName;
	m_strSQL += "'";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"数据库没有打开或已经关闭!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();

		if(DLArray){delete []DLArray;DLArray=NULL;}
		NDL=Report_input.GetRecordCount();
		if(NDL>0)
			DLArray=new DLBZ[NDL];
		int i=0;
		Report_input.MoveFirst();
		while(!Report_input.IsEOF())
		{
			//acedSetStatusBarProgressMeterPos(i/NDL*100);
			Report_input.GetValueString(DLArray[i].BGH,L"等号左里程冠号");	
			Report_input.GetValueString(DLArray[i].EGH,L"等号右里程冠号");	
			Report_input.GetValueString(strText,L"等号左里程数");	
			DLArray[i].BLC=_wtof(strText);			
			Report_input.GetValueString(strText,L"等号右里程数");	
			DLArray[i].ELC=_wtof(strText);
			Report_input.MoveNext();
			i++;
		}

	}
	catch(_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	if(NDL>0)
	{
		//起点里程
		Sxlc = DLArray[0].BLC;
		Exlc = DLArray[NDL-1].ELC;		
	}	

	return 1;
}



/*
int HdmDataBase::Attach_ZYJSMdb(CString TabName)//与数据库连接并连接数据库，形成一个非空记录集
{
if(!ConnectMdb())
return 0;

m_strSQL ="select * from "+TabName;
if (!m_adoConnection.IsOpen())
{
AfxMessageBox(L"数据库没有打开或已经关闭!");
return 0;
}
Report_input.SetAdoConnection(&m_adoConnection);
if(Report_input.Open(m_strSQL, adCmdText)!=1)
return 0; 
Report_input.MoveFirst();
while(!Report_input.IsEOF())
{
CString str;
Report_input.GetValueString(str,"速度目标");
m_SDMB=_wtof(str);
Report_input.MoveNext();
}
Report_input.Close();
m_adoConnection.Disconnect();
return 1;
}*/


//删除所有记录
void HdmDataBase::deleteDataFrmMdb(CString tabName,CString RoadName)

{
	CString SQL;
	SQL="delete * from ";
	SQL+=tabName;
	SQL+=" where 道路名=";
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);
}

//将第i个绕行段的标准数据写入数据库
void  HdmDataBase::WriteHdmMdb(CString HdmMdbname,CString RoadName,int xh)
{	
	int i=0,j,k=1;
	CString SQL;
	_variant_t var;
	CString  bzFrame;

	bzFrame="填改良土";

	if(!ConnectMdb())
		return;

	//	if(!ConnectMdb())//连接数据库
	//		return ;
	InitTableList();
	if(!FindTable(bzFrame))
	{
		CString SQL;
		SQL="create table ";
		SQL+="填方分类 ";
		SQL+="(";
		SQL += "序号 INTEGER,";
		SQL+="道路名 VARCHAR,";
		SQL+="分段终里程 double precision,";
		SQL+="填料类型 INTEGER";
		SQL+=")";
		m_adoConnection.Execute(SQL);	
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	//int  
	for(i=0;i<NLDTL;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,LDTL[i].dml);
		Report_input.PutCollect(3,(long)LDTL[i].type);

		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="用地面积汇总分段表";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		SQL="create table ";
		SQL+="用地面积汇总分段表 ";
		SQL+="(";
		SQL+= "序号 INTEGER,";
		SQL+="道路名 VARCHAR,";
		SQL+="分段终里程 double precision";
		SQL+=")";
		m_adoConnection.Execute(SQL);
	}

	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i<NYD;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2, FDB[i].dml);
		var=FDB[i].BZ;
		Report_input.PutCollect(3, var);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="用地类型表";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="用地类型表 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程数 double,";
		m_SQL+="用地类型 varchar";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i<NYDLX;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2, LXB[i].dml);
		var=LXB[i].YDLX;
		Report_input.PutCollect(3, var);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="挖方分类";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="挖方分类 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";

		m_SQL+="I百分比 DOUBLE,";
		m_SQL+="II百分比 DOUBLE,";
		m_SQL+="III百分比  DOUBLE,";
		m_SQL+="IV百分比  DOUBLE,";
		m_SQL+="V百分比  DOUBLE,";
		m_SQL+="VI百分比  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL +=L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NWFFL;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,WFFL[i].dml);
		for(j=0;j<6;j++)
			Report_input.PutCollect(j+3, WFFL[i].sj[j]);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	//////////////////////////////////////////////////////////////////////////
	//关联断面拉直与线间沟
	bzFrame="关联断面路肩拉直";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="关联断面路肩拉直 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段起始里程 double,";
		m_SQL+="分段终止里程 double,";
		m_SQL+="水平距离 DOUBLE,";
		m_SQL+="高差  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NGLDMLZ;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,GLDMLZ[i].Sdml);
		Report_input.PutCollect(3,GLDMLZ[i].Edml);
		Report_input.PutCollect(4,GLDMLZ[i].dX);
		Report_input.PutCollect(5,GLDMLZ[i].dH);

		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="关联断面路堑边坡推平";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="关联断面路堑边坡推平 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段起始里程 double,";
		m_SQL+="分段终止里程 double,";
		m_SQL+="水平距离 DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NGLDMLQBPTP;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,GLDMLQBPTP[i].Sdml);
		Report_input.PutCollect(3,GLDMLQBPTP[i].Edml);
		Report_input.PutCollect(4,GLDMLQBPTP[i].dX);

		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="关联断面线间沟";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="关联断面线间沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段起始里程 double,";
		m_SQL+="分段终止里程 double,";
		m_SQL+="沟宽 DOUBLE,";
		m_SQL+="沟深 DOUBLE,";
		m_SQL+="内边坡  DOUBLE,";
		m_SQL+="外边坡  DOUBLE,";
		m_SQL+="浆砌厚度  DOUBLE,";
		m_SQL+="沟底半径  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NGLDMXJG;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,GLDMXJG[i].Sdml);
		Report_input.PutCollect(3,GLDMXJG[i].Edml);
		Report_input.PutCollect(4,GLDMXJG[i].GouWide);
		Report_input.PutCollect(5,GLDMXJG[i].GouHigh);
		Report_input.PutCollect(6,GLDMXJG[i].NBP);
		Report_input.PutCollect(7,GLDMXJG[i].WBP);
		Report_input.PutCollect(8,GLDMXJG[i].JQHD);
		Report_input.PutCollect(9,GLDMXJG[i].GDR);

		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	//////////////////////////////////////////////////////////////////////////

	bzFrame="清表土厚度";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="清表土厚度 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="清表土厚度 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NZHTHD;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);

		Report_input.PutCollect(2,ZHTHD[i].dml);
		Report_input.PutCollect(3, ZHTHD[i].hthd);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	//------------------------------------------------------
	//以下数据分左右侧	
	bzFrame="左挡墙";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="左挡墙 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="起始里程 double,";
		m_SQL+="起始支距 double precision,";
		m_SQL+="终点里程 double precision,";
		m_SQL+="终止支距 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NZDQ;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,ZDQ[i].sdml);
		Report_input.PutCollect(3,ZDQ[i].szj);
		Report_input.PutCollect(4,ZDQ[i].edml);
		Report_input.PutCollect(5,ZDQ[i].ezj);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();


	bzFrame="左路面厚度";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="左路面厚度 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程数 double,";
		m_SQL+="车行道厚 double precision,";
		m_SQL+="硬路肩厚 double precision,";
		m_SQL+="土路肩厚 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NZLMHD;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,ZLMHD[i].dml);
		Report_input.PutCollect(3,ZLMHD[i].cxdh);
		Report_input.PutCollect(4,ZLMHD[i].yljh);
		Report_input.PutCollect(5,ZLMHD[i].tljh);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="左地面线内插宽";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左地面线内插宽 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="内插宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NZCRS;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,ZCRSWID[i].dml);
		Report_input.PutCollect(3, ZCRSWID[i].wid);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="左横断面限制宽度";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左横断面限制宽度 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="路基限宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);


	for(i=0;i< NZDM;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,ZDM[i].dml);
		Report_input.PutCollect(3,ZDM[i].xk);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="左用地加宽";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左用地加宽 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="路堤加宽 double precision,";
		m_SQL+="路堑加宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NZPWD;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2, ZPWD[i].dml);
		Report_input.PutCollect(3, ZPWD[i].ltjk);
		Report_input.PutCollect(4, ZPWD[i].lqjk);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="左路堤边坡";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="左路堤边坡 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double precision,";
		m_SQL+="边坡总级数 integer,";

		for(i=0;i<29;i++)
		{

			str.Format(L"%d",i+1);
			m_SQL+=str;
			m_SQL+="级边坡系数 double precision,";
			m_SQL+=str;
			m_SQL+="级最大高度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台宽度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台横坡 double precision,";
		}

		str.Format(L"%d",i+1);
		m_SQL+=str;
		m_SQL+="级边坡系数 double precision,";
		m_SQL+=str;
		m_SQL+="级最大高度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台宽度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台横坡 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText); 


	for(i=0;i< NZLDBP;i++)
	{
		ZLDBP[i].bpdata[(ZLDBP[i].BPJS-1)*4+1] = 100.0;
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,ZLDBP[i].dml);
		Report_input.PutCollect(3,(long) ZLDBP[i].BPJS);

		for(j=0;j< ZLDBP[i].BPJS*4;j++)
		{

			var= ZLDBP[i].bpdata[j];
			Report_input.PutCollect(j+4,var);

		}
		xh++;	
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="左路堑边坡";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="左路堑边坡 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double precision,";
		m_SQL+="边坡总级数 integer,";
		for(i=0;i<29;i++)
		{
			str.Format(L"%d",i+1);
			//m_SQL+=str;
			//m_SQL+="级边坡  integer,";
			m_SQL+=str;
			m_SQL+="级边坡系数 double precision,";
			m_SQL+=str;
			m_SQL+="级最大高度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台宽度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台横坡 double precision,";
		}

		str.Format(L"%d",i+1);
		//m_SQL+=str;
		//m_SQL+="级边坡  integer,";
		m_SQL+=str;
		m_SQL+="级边坡系数 double precision,";
		m_SQL+=str;
		m_SQL+="级最大高度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台宽度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台横坡 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NZLQBP;i++)
	{
		ZLQBP[i].bpdata[(ZLQBP[i].BPJS-1)*4+1] = 100.0;
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);

		Report_input.PutCollect(2,ZLQBP[i].dml);
		Report_input.PutCollect(3,(long) ZLQBP[i].BPJS);
		for(j=0;j< ZLQBP[i].BPJS*4;j++)
		{

			var= ZLQBP[i].bpdata[j];
			Report_input.PutCollect(j+4,var);

		}
		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="左排水沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左排水沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="坡脚墙 integer,";
		m_SQL+="浆砌厚度 double precision,";
		m_SQL+="起始里程 double precision,";
		m_SQL+="起始护坡道宽 double precision,";
		m_SQL+="起始护坡道厚 double precision,";
		m_SQL+="起始水沟底宽 double precision,";
		m_SQL+="起始水沟深 double precision,";
		m_SQL+="起始挡水堰宽 double precision,";
		m_SQL+="起始挡水堰厚 double precision,";
		m_SQL+="终止里程 double precision,";
		m_SQL+="终止护坡道宽 double precision,";
		m_SQL+="终止护坡道厚 double precision,";
		m_SQL+="终止水沟底宽 double precision,";
		m_SQL+="终止水沟深 double precision,";
		m_SQL+="终止挡水堰宽 double precision,";
		m_SQL+="终止挡水堰厚 double precision,";
		m_SQL+="内侧边坡 double precision,";
		m_SQL+="外侧边坡 double precision,";
		m_SQL+="挡水堰坡 double precision";
		m_SQL+="沟底半径 double precision";
		m_SQL+=")";

		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL +=L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	int FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",bzFrame);//插入沟底半径列
		m_adoConnection.Execute(AddField);
		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
	}

	for(i=0;i< NZLDSG;i++)
	{
		Report_input.AddNew();
		//	Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,(long) ZLDSG[i].WallorGou);
		Report_input.PutCollect(3, ZLDSG[i].jh);
		Report_input.PutCollect(4,ZLDSG[i].sdml);
		Report_input.PutCollect(5,ZLDSG[i].S_HPDK);
		Report_input.PutCollect(6,ZLDSG[i].S_HPDH);
		Report_input.PutCollect(7,ZLDSG[i].S_GS);
		Report_input.PutCollect(8,ZLDSG[i].S_GK);
		Report_input.PutCollect(9,ZLDSG[i].S_DSYK);
		Report_input.PutCollect(10,ZLDSG[i].S_DSYH);
		Report_input.PutCollect(11,ZLDSG[i].edml);
		Report_input.PutCollect(12,ZLDSG[i].E_HPDK);
		Report_input.PutCollect(13,ZLDSG[i].E_HPDH);
		Report_input.PutCollect(14,ZLDSG[i].E_GS);
		Report_input.PutCollect(15,ZLDSG[i].E_GK);
		Report_input.PutCollect(16,ZLDSG[i].E_DSYK);
		Report_input.PutCollect(17,ZLDSG[i].E_DSYH);
		Report_input.PutCollect(18, ZLDSG[i].nbp);
		Report_input.PutCollect(19, ZLDSG[i].wbp);
		Report_input.PutCollect(20, ZLDSG[i].DSYP);
		Report_input.PutCollect(21, ZLDSG[i].LDSG_GDR);


		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="左侧沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左侧沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="水沟底宽 double precision,";
		m_SQL+="沟深 double precision,";
		m_SQL+="左侧边坡 double precision,";
		m_SQL+="右侧边坡 double precision,";
		m_SQL+="浆砌厚度 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NZLQSG;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1,var);
		Report_input.PutCollect(2, ZLQSG[i].dml);
		Report_input.PutCollect(3, ZLQSG[i].gw);
		Report_input.PutCollect(4, ZLQSG[i].gs);
		Report_input.PutCollect(5, ZLQSG[i].nbp);
		Report_input.PutCollect(6, ZLQSG[i].wbp);
		Report_input.PutCollect(7, ZLQSG[i].Jh);
		Report_input.PutCollect(8, ZLQSG[i].GDR);

		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="左天沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="左天沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段起里程 double,";
		m_SQL+="起始边距 double precision,";
		m_SQL+="起始水沟深 double precision,";
		m_SQL+="起始水沟底宽 double precision,";

		m_SQL+="分段终里程 double,";
		m_SQL+="终止边距 double precision,";
		m_SQL+="终止水沟深 double precision,";
		m_SQL+="终止水沟底宽 double precision,";

		m_SQL+="内侧边坡 double precision,";
		m_SQL+="外侧边坡 double precision,";
		m_SQL+="浆砌厚度 double precision,";
		m_SQL+="沟底半径 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}

	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",bzFrame);//插入沟底半径列
		m_adoConnection.Execute(AddField);
		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
	}


	for(i=0;i< NZTG;i++)
	{
		Report_input.AddNew();
		//	Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1,var);
		Report_input.PutCollect(2,ZTG[i].sdml);
		Report_input.PutCollect(3,ZTG[i].sbj);
		Report_input.PutCollect(4, ZTG[i].sgs);
		Report_input.PutCollect(5, ZTG[i].sgw);
		Report_input.PutCollect(6,ZTG[i].edml);

		Report_input.PutCollect(7, ZTG[i].ebj);
		Report_input.PutCollect(8, ZTG[i].egs);
		Report_input.PutCollect(9, ZTG[i].egw);
		Report_input.PutCollect(10, ZTG[i].nbp);
		Report_input.PutCollect(11, ZTG[i].wbp);
		Report_input.PutCollect(12, ZTG[i].jqhd);
		Report_input.PutCollect(13, ZTG[i].TG_GDR);
		xh++;		
	}

	Report_input.Update();
	Report_input.Close();
	//---------------------------------------------------------------
	bzFrame="右挡墙";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="右挡墙 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="起始里程 double,";
		m_SQL+="起始支距 double precision,";
		m_SQL+="终点里程 double precision,";
		m_SQL+="终止支距 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NYDQ;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YDQ[i].sdml);
		Report_input.PutCollect(3,YDQ[i].szj);
		Report_input.PutCollect(4,YDQ[i].edml);
		Report_input.PutCollect(5,YDQ[i].ezj);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();


	bzFrame="右低填路堤自动填平";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="右低填路堤自动填平 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="起始里程 double precision,";
		m_SQL+="终点里程 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NYLDTP;i++)
	{
		Report_input.AddNew();

		//	Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YLDTP[i].sdml);
		Report_input.PutCollect(3,YLDTP[i].edml);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="右低挖路堑自动推平";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="右低挖路堑自动推平 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="起始里程 double precision,";
		m_SQL+="终点里程 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NYLQTP;i++)
	{
		Report_input.AddNew();

		//	Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YLQTP[i].sdml);
		Report_input.PutCollect(3,YLQTP[i].edml);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="右低填路堤自动加宽土路肩";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="右低填路堤自动加宽土路肩 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="起始里程 double precision,";
		m_SQL+="终点里程 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NYTLJJK;i++)
	{
		Report_input.AddNew();

		//	Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YTLJJK[i].sdml);
		Report_input.PutCollect(3,YTLJJK[i].edml);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="右路面厚度";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右路面厚度 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double precision,";
		m_SQL+="车行道厚 double precision,";
		m_SQL+="硬路肩厚 double precision,";
		m_SQL+="土路肩厚 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);
	for(i=0;i< NYLMHD;i++)
	{
		Report_input.AddNew();

		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YLMHD[i].dml);
		Report_input.PutCollect(3,YLMHD[i].cxdh);
		Report_input.PutCollect(4,YLMHD[i].yljh);
		Report_input.PutCollect(5,YLMHD[i].tljh);
		xh++;
	}
	Report_input.Update();
	Report_input.Close();

	bzFrame="右地面线内插宽";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右地面线内插宽 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="内插宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NYCRS;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YCRSWID[i].dml);
		Report_input.PutCollect(3, YCRSWID[i].wid);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="右横断面限制宽度";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右横断面限制宽度 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="路基限宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);


	for(i=0;i< NYDM;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YDM[i].dml);
		Report_input.PutCollect(3,YDM[i].xk);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="右用地加宽";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右用地加宽 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="路堤加宽 double precision,";
		m_SQL+="路堑加宽 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);

	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);


	for(i=0;i< NYPWD;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2, YPWD[i].dml);
		Report_input.PutCollect(3, YPWD[i].ltjk);
		Report_input.PutCollect(4, YPWD[i].lqjk);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="右路堤边坡";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="右路堤边坡 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double precision,";
		m_SQL+="边坡总级数 integer,";

		for(i=0;i<29;i++)
		{

			str.Format(L"%d",i+1);
			m_SQL+=str;
			m_SQL+="级边坡系数 double precision,";
			m_SQL+=str;
			m_SQL+="级最大高度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台宽度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台横坡 double precision,";
		}

		str.Format(L"%d",i+1);
		m_SQL+=str;
		m_SQL+="级边坡系数 double precision,";
		m_SQL+=str;
		m_SQL+="级最大高度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台宽度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台横坡 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NYLDBP;i++)
	{
		YLDBP[i].bpdata[(YLDBP[i].BPJS-1)*4+1] = 100.0;
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,YLDBP[i].dml);
		Report_input.PutCollect(3,(long) YLDBP[i].BPJS);

		for(j=0;j< YLDBP[i].BPJS*4;j++)
		{

			var= YLDBP[i].bpdata[j];
			Report_input.PutCollect(j+4,var);

		}
		xh++;	
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="右路堑边坡";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="右路堑边坡 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double precision,";
		m_SQL+="边坡总级数 integer,";
		for(i=0;i<29;i++)
		{
			str.Format(L"%d",i+1);
			//m_SQL+=str;
			//m_SQL+="级边坡  integer,";
			m_SQL+=str;
			m_SQL+="级边坡系数 double precision,";
			m_SQL+=str;
			m_SQL+="级最大高度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台宽度 double precision,";
			m_SQL+=str;
			m_SQL+="级平台横坡 double precision,";
		}

		str.Format(L"%d",i+1);
		//m_SQL+=str;
		//m_SQL+="级边坡  integer,";
		m_SQL+=str;
		m_SQL+="级边坡系数 double precision,";
		m_SQL+=str;
		m_SQL+="级最大高度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台宽度 double precision,";
		m_SQL+=str;
		m_SQL+="级平台横坡 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL); 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NYLQBP;i++)
	{
		YLQBP[i].bpdata[(YLQBP[i].BPJS-1)*4+1] = 100.0;
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);

		Report_input.PutCollect(2,YLQBP[i].dml);
		Report_input.PutCollect(3,(long) YLQBP[i].BPJS);
		for(j=0;j< YLQBP[i].BPJS*4;j++)
		{

			var= YLQBP[i].bpdata[j];
			Report_input.PutCollect(j+4,var);

		}
		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="右排水沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右排水沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="坡脚墙 integer,";
		m_SQL+="浆砌厚度 double precision,";
		m_SQL+="起始里程 double precision,";
		m_SQL+="起始护坡道宽 double precision,";
		m_SQL+="起始护坡道厚 double precision,";
		m_SQL+="起始水沟底宽 double precision,";
		m_SQL+="起始水沟深 double precision,";
		m_SQL+="起始挡水堰宽 double precision,";
		m_SQL+="起始挡水堰厚 double precision,";
		m_SQL+="终止里程 double precision,";
		m_SQL+="终止护坡道宽 double precision,";
		m_SQL+="终止护坡道厚 double precision,";
		m_SQL+="终止水沟底宽 double precision,";
		m_SQL+="终止水沟深 double precision,";
		m_SQL+="终止挡水堰宽 double precision,";
		m_SQL+="终止挡水堰厚 double precision,";
		m_SQL+="内侧边坡 double precision,";
		m_SQL+="外侧边坡 double precision,";
		m_SQL+="挡水堰坡 double precision";
		m_SQL+="沟底半径 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",bzFrame);//插入沟底半径列
		m_adoConnection.Execute(AddField);
		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
	}

	for(i=0;i< NYLDSG;i++)
	{
		Report_input.AddNew();
		//		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1, var);
		Report_input.PutCollect(2,(long) YLDSG[i].WallorGou);
		Report_input.PutCollect(3, YLDSG[i].jh);
		Report_input.PutCollect(4,YLDSG[i].sdml);
		Report_input.PutCollect(5,YLDSG[i].S_HPDK);
		Report_input.PutCollect(6,YLDSG[i].S_HPDH);
		Report_input.PutCollect(7,YLDSG[i].S_GS);
		Report_input.PutCollect(8,YLDSG[i].S_GK);
		Report_input.PutCollect(9,YLDSG[i].S_DSYK);
		Report_input.PutCollect(10,YLDSG[i].S_DSYH);
		Report_input.PutCollect(11,YLDSG[i].edml);
		Report_input.PutCollect(12,YLDSG[i].E_HPDK);
		Report_input.PutCollect(13,YLDSG[i].E_HPDH);
		Report_input.PutCollect(14,YLDSG[i].E_GS);
		Report_input.PutCollect(15,YLDSG[i].E_GK);
		Report_input.PutCollect(16,YLDSG[i].E_DSYK);
		Report_input.PutCollect(17,YLDSG[i].E_DSYH);
		Report_input.PutCollect(18, YLDSG[i].nbp);
		Report_input.PutCollect(19, YLDSG[i].wbp);
		Report_input.PutCollect(20, YLDSG[i].DSYP);
		Report_input.PutCollect(21, YLDSG[i].LDSG_GDR);	
		xh++;
	}

	Report_input.Update();
	Report_input.Close();

	bzFrame="右侧沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右侧沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段终里程 double,";
		m_SQL+="水沟底宽 double precision,";
		m_SQL+="沟深 double precision,";
		m_SQL+="内侧边坡 double precision,";
		m_SQL+="外侧边坡 double precision,";
		m_SQL+="浆砌厚度 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	for(i=0;i< NYLQSG;i++)
	{
		Report_input.AddNew();
		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1,var);
		Report_input.PutCollect(2, YLQSG[i].dml);
		Report_input.PutCollect(3, YLQSG[i].gw);
		Report_input.PutCollect(4, YLQSG[i].gs);
		Report_input.PutCollect(5, YLQSG[i].nbp);
		Report_input.PutCollect(6, YLQSG[i].wbp);
		Report_input.PutCollect(7, YLQSG[i].Jh);
		Report_input.PutCollect(8, YLQSG[i].GDR);
		xh++;
	}

	Report_input.Update();
	Report_input.Close();


	bzFrame="右天沟";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="右天沟 ";
		m_SQL+="(";
		m_SQL+= "序号 INTEGER identity,";
		m_SQL+="道路名 VARCHAR,";
		m_SQL+="分段起里程 double,";
		m_SQL+="起始边距 double precision,";
		m_SQL+="起始水沟深 double precision,";
		m_SQL+="起始水沟底宽 double precision,";

		m_SQL+="分段终里程 double,";
		m_SQL+="终止边距 double precision,";
		m_SQL+="终止水沟深 double precision,";
		m_SQL+="终止水沟底宽 double precision,";

		m_SQL+="内侧边坡 double precision,";
		m_SQL+="外侧边坡 double precision,";
		m_SQL+="浆砌厚度 double precision,";
		m_SQL+="沟底半径 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",bzFrame);//插入沟底半径列
		m_adoConnection.Execute(AddField);
		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
	}


	for(i=0;i< NYTG;i++)
	{
		Report_input.AddNew();
		//		Report_input.PutCollect(0,(long)xh);
		var = RoadName;
		Report_input.PutCollect(1,var);
		Report_input.PutCollect(2,YTG[i].sdml);
		Report_input.PutCollect(3,YTG[i].sbj);
		Report_input.PutCollect(4, YTG[i].sgs);
		Report_input.PutCollect(5, YTG[i].sgw);
		Report_input.PutCollect(6,YTG[i].edml);

		Report_input.PutCollect(7, YTG[i].ebj);
		Report_input.PutCollect(8, YTG[i].egs);
		Report_input.PutCollect(9, YTG[i].egw);
		Report_input.PutCollect(10, YTG[i].nbp);
		Report_input.PutCollect(11, YTG[i].wbp);
		Report_input.PutCollect(12, YTG[i].jqhd);
		Report_input.PutCollect(13, YTG[i].TG_GDR);
		xh++;		
	}
	Report_input.Update();
	Report_input.Close();
	m_adoConnection.Disconnect();
	return ;
}
//读取所有横断面数据
int HdmDataBase::Read_AllHdmDataFromDbs(CString mdbFname,CString roadname)
{
	int i=0,j,k;
	CString rcd;
	char sckml[40];
	CString field;
	double Sxlc,Exlc;
	CString TabName;

	GetRoadSEml(mdbFname,roadname,Sxlc,Exlc);
	//  MyPro->SetPosgress(0);
	TabName="松方压实系数";

	if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
	{
		YSXS[0]=1.23,YSXS[1]=1.16,YSXS[2]=1.09,YSXS[3]=0.92,YSXS[4]=0.92,YSXS[5]=0.92;
	}
	else //从数据库取值
	{
		Report_input.MoveFirst();
		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rcd,1);
			YSXS[0]=_wtof(rcd);
			Report_input.GetValueString(rcd,2);
			YSXS[1]=_wtof(rcd);
			Report_input.GetValueString(rcd,3);
			YSXS[2]=_wtof(rcd);
			Report_input.GetValueString(rcd,4);
			YSXS[3]=_wtof(rcd);
			Report_input.GetValueString(rcd,5);
			YSXS[4]=_wtof(rcd);
			Report_input.GetValueString(rcd,6);
			YSXS[5]=_wtof(rcd);				
		}
		Report_input.Close();
		m_adoConnection.Disconnect();

	}

	TabName="填方分类";

	if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
	{
		NLDTL=1;
		if(LDTL) {delete []LDTL;LDTL=NULL;}
		LDTL=new LDTLdata[NLDTL];
		LDTL[0].dml=Exlc;//////////////改良土厚度
		LDTL[0].type = 0;
	}
	else //从数据库取值
	{
		NLDTL=Report_input.GetRecordCount();

		if(LDTL) {delete []LDTL;LDTL=NULL;}
		LDTL=(LDTLdata *)new LDTLdata[NLDTL];//动态声请一个空间
		Report_input.MoveFirst();
		//Report_input.GetValueString()
		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rcd,2);
			LDTL[i].dml=_wtof(rcd);

			Report_input.GetValueString(rcd,3);
			LDTL[i].type=_wtoi(rcd);

			i++;
			Report_input.MoveNext();
		}
		Report_input.Close();
		m_adoConnection.Disconnect();

	}


	//	MyPro->SetPosgress(2);
	//	MyPro->SetPosgress(4);
	CString GH;
	i=0;
	TabName="用地面积汇总分段表";

	if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
	{

		NYD=1;
		if(FDB) {delete []FDB;FDB=NULL;}
		FDB=(YongDiFDB *)new YongDiFDB[NYD];//动态声请一个空间
		FDB[0].dml = Exlc;	
		FDB[0].BZ="";
	}
	else
	{

		NYD=Report_input.GetRecordCount();

		if(FDB) {delete []FDB;FDB=NULL;}
		FDB=(YongDiFDB *)new YongDiFDB[NYD];//动态声请一个空间
		Report_input.MoveFirst();
		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rcd,2);
			FDB[i].dml=_wtof(rcd);

			Report_input.GetValueString(FDB[i].BZ,3);
			Report_input.MoveNext();
			i++;
		}
		Report_input.Close();
		m_adoConnection.Disconnect();

	}

	//	MyPro->SetPosgress(6);
	//	MyPro->SetPosgress(8);

	TabName="用地类型表";
	i=0;
	if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
	{
		NYDLX=1;
		if(LXB){delete []LXB;LXB=NULL;}
		LXB=new YongDiLXB[NYDLX];
		LXB[0].dml = Exlc;	
		LXB[0].YDLX="水稻田";
	}
	else
	{		
		NYDLX=Report_input.GetRecordCount();
		if(LXB){delete []LXB;LXB=NULL;}
		LXB=new YongDiLXB[NYDLX];
		Report_input.MoveFirst();

		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rcd,2);
			LXB[i].dml=_wtof(rcd);
			Report_input.GetValueString(LXB[i].YDLX,3);
			Report_input.MoveNext();
			i++;
		}
		Report_input.Close();
		m_adoConnection.Disconnect();
	}

	//	MyPro->SetPosgress(10);
	//	MyPro->SetPosgress(12);

	TabName="挖方分类";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NWFFL=1;
			if(WFFL) {delete []WFFL;WFFL=NULL;}
			WFFL=new WFFLdata[NWFFL];

			WFFL[0].dml = Exlc;	
			for(int i=0; i<6; i++)
			{
				WFFL[0].sj[i] = 0;
			}
			WFFL[0].sj[2] = 100;//III
		}
		else
		{

			NWFFL=Report_input.GetRecordCount();

			if(WFFL) {delete []WFFL;WFFL=NULL;}
			WFFL=(WFFLdata *)new WFFLdata[NWFFL];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				WFFL[i].dml=_wtof(rcd);

				for(j=0;j<6;j++)
				{
					Report_input.GetValueString(field,j+3);
					WFFL[i].sj[j]=_wtof(field);
				}
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}


	//	MyPro->SetPosgress(14);
	//	MyPro->SetPosgress(16);

	//////////////////////////////////////////////////////////////////////////
	TabName="关联断面路肩拉直";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMLZ=1;
			if(GLDMLZ) {delete []GLDMLZ;GLDMLZ=NULL;}
			GLDMLZ=new GLDMLZdata[NGLDMLZ];

			GLDMLZ[0].Sdml = Sxlc;
			GLDMLZ[0].Edml = Exlc;
			GLDMLZ[0].dX = 5.0;
			GLDMLZ[0].dH = 1.0;

		}
		else
		{			
			NGLDMLZ=Report_input.GetRecordCount();

			if(GLDMLZ) {delete []GLDMLZ;GLDMLZ=NULL;}
			GLDMLZ=new GLDMLZdata[NGLDMLZ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMLZ[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMLZ[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMLZ[i].dX=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				GLDMLZ[i].dH=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="关联断面路堑边坡推平";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMLQBPTP=1;
			if(GLDMLQBPTP) {delete []GLDMLQBPTP;GLDMLQBPTP=NULL;}
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];

			GLDMLQBPTP[0].Sdml = Sxlc;
			GLDMLQBPTP[0].Edml = Exlc;
			GLDMLQBPTP[0].dX = 5.0;
		}
		else
		{			
			NGLDMLQBPTP=Report_input.GetRecordCount();

			if(GLDMLQBPTP) {delete []GLDMLQBPTP;GLDMLQBPTP=NULL;}
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMLQBPTP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMLQBPTP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMLQBPTP[i].dX=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	// 	MyPro->SetPosgress(14);
	// 	MyPro->SetPosgress(15);



	TabName="关联断面线间沟";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMXJG=1;
			if(GLDMXJG) {delete []GLDMXJG;GLDMXJG=NULL;}
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];

			GLDMXJG[0].Sdml = Sxlc;
			GLDMXJG[0].Edml = Exlc;
			GLDMXJG[0].GouWide = 0.5;
			GLDMXJG[0].GouHigh = 0.7;
			GLDMXJG[0].NBP = 0.0;
			GLDMXJG[0].WBP = 0.0;
			GLDMXJG[0].JQHD = 0.3;
			GLDMXJG[0].GDR = 0.0;

		}
		else
		{			
			NGLDMXJG=Report_input.GetRecordCount();

			if(GLDMXJG) {delete []GLDMXJG;GLDMXJG=NULL;}
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMXJG[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMXJG[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMXJG[i].GouWide=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				GLDMXJG[i].GouHigh=_wtof(rcd);
				Report_input.GetValueString(rcd,6);
				GLDMXJG[i].NBP=_wtof(rcd);
				Report_input.GetValueString(rcd,7);
				GLDMXJG[i].WBP=_wtof(rcd);
				Report_input.GetValueString(rcd,8);
				GLDMXJG[i].JQHD=_wtof(rcd);
				Report_input.GetValueString(rcd,9);
				GLDMXJG[i].GDR=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}

	// 	MyPro->SetPosgress(16);
	// 	MyPro->SetPosgress(17);

	//////////////////////////////////////////////////////////////////////////

	TabName="左清表土厚度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZHTHD=1;
			if(ZHTHD) {delete []ZHTHD;ZHTHD=NULL;}
			ZHTHD=new hthddata[NZHTHD];

			ZHTHD[0].dml=Exlc;	
			ZHTHD[0].hthd=0.0;
			ZHTHD[0].xz=0;
		}
		else
		{
			NZHTHD=Report_input.GetRecordCount();

			if(ZHTHD) {delete []ZHTHD;ZHTHD=NULL;}
			ZHTHD=(hthddata*)new hthddata[NZHTHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZHTHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZHTHD[i].hthd=_wtof(field);
				Report_input.GetValueString(field,4);
				ZHTHD[i].xz=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	MyPro->SetPosgress(18);
	//	MyPro->SetPosgress(20);

	TabName="左沟底坡";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZGouPD=0;
			ZGouPD = NULL;
		}
		else
		{
			NZGouPD=Report_input.GetRecordCount();

			if(ZGouPD) {delete []ZGouPD;ZGouPD=NULL;}
			ZGouPD=(GouPDdata*)new GouPDdata[NZGouPD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZGouPD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZGouPD[i].SH=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZGouPD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZGouPD[i].EH=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="左超填宽度表";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZCTKD=0;
			ZCTKD = NULL;
		}
		else
		{
			NZCTKD=Report_input.GetRecordCount();

			if(ZCTKD) {delete []ZCTKD;ZCTKD=NULL;}
			ZCTKD=(ctkddata*)new ctkddata[NZCTKD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZCTKD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZCTKD[i].Sctkd=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZCTKD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZCTKD[i].Ectkd=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="左弧形边坡表";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZARCBP=0;
			ZARCBP = NULL;
		}
		else
		{
			NZARCBP=Report_input.GetRecordCount();

			if(ZARCBP) {delete []ZARCBP;ZARCBP=NULL;}
			ZARCBP=(arcbpata*)new arcbpata[NZARCBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZARCBP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZARCBP[i].SR=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZARCBP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZARCBP[i].ER=_wtof(field);
				Report_input.GetValueString(field,6);
				ZARCBP[i].ArcLocation=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//以下数据分左右	///////////////////////////////////////////////////////////
	TabName="左挡墙";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZDQ=0;
			ZDQ=NULL;
			/*
			if(ZDQ) {delete []ZDQ;ZDQ=NULL;}
			ZDQ=new dqdata[NZDQ];
			ZDQ[0].sdml = Sxlc;
			ZDQ[0].edml = Exlc;
			ZDQ[0].szj = 100.0;
			ZDQ[0].ezj = 100.0;	*/

		}
		else
		{
			NZDQ=Report_input.GetRecordCount();

			if(ZDQ) {delete []ZDQ;ZDQ=NULL;}
			ZDQ=(dqdata *)new dqdata[NZDQ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZDQ[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZDQ[i].szj=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				ZDQ[i].edml=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				ZDQ[i].ezj=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	TabName="左低填路堤自动填平";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDTP=0;
		}
		else
		{
			NZLDTP=Report_input.GetRecordCount();

			if(ZLDTP) {delete []ZLDTP;ZLDTP=NULL;}
			ZLDTP=(LDTPData *)new LDTPData[NZLDTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLDTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLDTP[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="左低挖路堑自动推平";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLQTP=0;
		}
		else
		{
			NZLQTP=Report_input.GetRecordCount();

			if(ZLQTP) {delete []ZLQTP;ZLQTP=NULL;}
			ZLQTP=(LQTPData *)new LQTPData[NZLQTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLQTP[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="左低填路堤自动加宽土路肩";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZTLJJK=0;
		}
		else
		{
			NZTLJJK=Report_input.GetRecordCount();

			if(ZTLJJK) {delete []ZTLJJK;ZTLJJK=NULL;}
			ZTLJJK=(TLJJKData *)new TLJJKData[NZTLJJK];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZTLJJK[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZTLJJK[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	TabName="左路面厚度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLMHD=1;
			if(ZLMHD) {delete []ZLMHD;ZLMHD=NULL;}
			ZLMHD=new LMHdata[NZLMHD];
			ZLMHD[0].dml = Exlc;
			ZLMHD[0].cxdh = 0.0;
			ZLMHD[0].yljh = 0.0;
			ZLMHD[0].tljh = 0.0;
		}
		else
		{
			NZLMHD=Report_input.GetRecordCount();

			if(ZLMHD) {delete []ZLMHD;ZLMHD=NULL;}
			ZLMHD=(LMHdata*)new LMHdata[NZLMHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLMHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLMHD[i].cxdh=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				ZLMHD[i].yljh=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				ZLMHD[i].tljh=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//	MyPro->SetPosgress(22);
	//	MyPro->SetPosgress(24);

	//------------------------------------------------------
	TabName="左地面线内插宽";
	{
		//以下数据分左右侧	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZCRS=1;
			if(ZCRSWID) {delete []ZCRSWID;ZCRSWID=NULL;}
			ZCRSWID=new crswiddata[NZCRS];

			ZCRSWID[0].dml = Exlc;
			ZCRSWID[0].wid=100.0;		   
		}
		else
		{
			NZCRS=Report_input.GetRecordCount();

			if(ZCRSWID) {delete []ZCRSWID;ZCRSWID=NULL;}
			ZCRSWID=(crswiddata* )new crswiddata[NZCRS];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZCRSWID[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZCRSWID[i].wid=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(30);
	//	   MyPro->SetPosgress(32);

	TabName="左横断面限制宽度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZDM=1;
			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=new dmdata[NZDM];

			ZDM[0].dml = Exlc;	
			ZDM[0].xk=100.0;//限制宽
		}
		else
		{
			NZDM=Report_input.GetRecordCount();

			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=(dmdata* )new dmdata[NZDM];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZDM[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZDM[i].xk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//   MyPro->SetPosgress(34);
	//  MyPro->SetPosgress(36);
	TabName="左用地加宽";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZPWD=1;
			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=new pwddata[NZPWD];
			ZPWD[0].dml = Exlc;	
			ZPWD[0].ltjk=ZPWD[0].lqjk=1.0;//外宽
		}
		else
		{
			NZPWD=Report_input.GetRecordCount();

			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=(pwddata* )new pwddata[NZPWD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZPWD[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZPWD[i].ltjk=_wtof(field);
				Report_input.GetValueString(field,4);
				ZPWD[i].lqjk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//   MyPro->SetPosgress(38);
	//   MyPro->SetPosgress(40);
	TabName="左路堤边坡";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDBP=1;
			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//动态声请一个空间
			ZLDBP[0].dml = Exlc;	

			ZLDBP[0].BPJS  = 3;
			ZLDBP[0].bpdata[0]  = 1.5;
			ZLDBP[0].bpdata[1]  = 8;
			ZLDBP[0].bpdata[2]  = 1.0;
			ZLDBP[0].bpdata[3]  = 0.0;

			ZLDBP[0].bpdata[4]  = 1.75;
			ZLDBP[0].bpdata[5]  = 10;
			ZLDBP[0].bpdata[6]  = 1.0;
			ZLDBP[0].bpdata[7]  = 0.0;

			ZLDBP[0].bpdata[8]  = 2.0;
			ZLDBP[0].bpdata[9]  = 100;
			ZLDBP[0].bpdata[10]  = 1.0;
			ZLDBP[0].bpdata[11]  = 0.0;
		}
		else
		{
			NZLDBP=Report_input.GetRecordCount();

			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//动态声请一个空间
			Report_input.MoveFirst();
			k=1;
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLDBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLDBP[i].BPJS=_wtoi(field);
				for( j=0;j<ZLDBP[i].BPJS*4;j++)
				{
					Report_input.GetValueString(field,4+j);
					ZLDBP[i].bpdata[j]=_wtof(field);


				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	// ZLDBP[i-1].bpdata[(ZLDBP[i-1].BPJS-1)*4+1]=100;
	//	   MyPro->SetPosgress(42);
	//	   MyPro->SetPosgress(44);
	TabName="左路堑边坡";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLQBP=1;
			// if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//动态声请一个空间
			ZLQBP[0].dml = Exlc;	

			ZLQBP[0].BPJS = 3;
			ZLQBP[0].bpdata[0]  = 1;
			ZLQBP[0].bpdata[1]  = 8;
			ZLQBP[0].bpdata[2]  = 1.0;
			ZLQBP[0].bpdata[3] = 0.0;

			ZLQBP[0].bpdata[4]  = 1.25;
			ZLQBP[0].bpdata[5]  = 10;
			ZLQBP[0].bpdata[6]  = 1.0;
			ZLQBP[0].bpdata[7]  = 0.0;

			ZLQBP[0].bpdata[8] =  1.5;
			ZLQBP[0].bpdata[9] =  100;
			ZLQBP[0].bpdata[10] =  1.0;
			ZLQBP[0].bpdata[11] =  0.0;

		}
		else
		{
			NZLQBP=Report_input.GetRecordCount();

			//if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLQBP[i].BPJS=_wtoi(field);
				for( j=0;j<ZLQBP[i].BPJS*4;j++)
				{					   
					Report_input.GetValueString(field,4+j);
					ZLQBP[i].bpdata[j]=_wtof(field);					   
				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//ZLQBP[i-1].bpdata[(ZLQBP[i-1].BPJS-1)*4+1]=100;
	//	   MyPro->SetPosgress(46);
	//	   MyPro->SetPosgress(48);
	TabName="左排水沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDSG=0;
			/*
			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//动态声请一个空间
			ZLDSG[0].sdml = Sxlc;	
			ZLDSG[0].edml = Exlc;	

			ZLDSG[0].WallorGou  = 1;//默认的是水沟
			ZLDSG[0].S_HPDK=ZLDSG[0].E_HPDK=2;//护坡道宽
			ZLDSG[0].S_HPDH=ZLDSG[0].E_HPDH=0.5;//护坡道宽
			ZLDSG[0].S_DSYK=ZLDSG[0].E_DSYK=1;//护坡道宽
			ZLDSG[0].S_DSYH=ZLDSG[0].E_DSYH=0.5;//护坡道宽
			ZLDSG[0].S_GK=ZLDSG[0].E_GK=0.4;//横坡
			ZLDSG[0].S_GS=ZLDSG[0].E_GS=0.6;//横坡
			ZLDSG[0].nbp  = 1;//内侧边坡
			ZLDSG[0].wbp  = 1;//外侧边坡
			ZLDSG[0].DSYP  = 1;//外侧边坡
			ZLDSG[0].jh   = 0.2;//浆砌厚度*/

		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NZLDSG=Report_input.GetRecordCount();

			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{

				Report_input.GetValueString(field,2);
				ZLDSG[i].WallorGou=_wtoi(field);
				Report_input.GetValueString(field,3);
				ZLDSG[i].jh=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZLDSG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,5);
				ZLDSG[i].S_HPDK=_wtof(field);
				Report_input.GetValueString(field,6);
				ZLDSG[i].S_HPDH=_wtof(field);
				Report_input.GetValueString(field,7);
				ZLDSG[i].S_GS=_wtof(field);
				Report_input.GetValueString(field,8);
				ZLDSG[i].S_GK=_wtof(field);
				Report_input.GetValueString(field,9);
				ZLDSG[i].S_DSYK=_wtof(field);
				Report_input.GetValueString(field,10);
				ZLDSG[i].S_DSYH=_wtof(field);

				Report_input.GetValueString(field,11);
				ZLDSG[i].edml=_wtof(field);
				Report_input.GetValueString(field,12);
				ZLDSG[i].E_HPDK=_wtof(field);
				Report_input.GetValueString(field,13);
				ZLDSG[i].E_HPDH=_wtof(field);
				Report_input.GetValueString(field,14);
				ZLDSG[i].E_GS=_wtof(field);
				Report_input.GetValueString(field,15);
				ZLDSG[i].E_GK=_wtof(field);
				Report_input.GetValueString(field,16);
				ZLDSG[i].E_DSYK=_wtof(field);
				Report_input.GetValueString(field,17);
				ZLDSG[i].E_DSYH=_wtof(field);
				Report_input.GetValueString(field,18);
				ZLDSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,19);
				ZLDSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,20);
				ZLDSG[i].DSYP=_wtof(field);
				Report_input.GetValueString(field,21);
				if (field=="NULL")
				{
					Report_input.PutCollect(21,0.0);
					ZLDSG[i].LDSG_GDR=0.0;
				}
				else
					ZLDSG[i].LDSG_GDR=_wtof(field);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	//	   MyPro->SetPosgress(50);
	//	   MyPro->SetPosgress(52);
	TabName="左侧沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{

			NZLQSG=1;
			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			ZLQSG[0].dml = Exlc;	

			ZLQSG[0].gw=0.4;
			ZLQSG[0].gs = 0.6;
			ZLQSG[0].nbp= 1;
			ZLQSG[0].wbp = 1;
			ZLQSG[0].Jh=0.3;////浆砌厚度
			ZLQSG[0].GDR=0.0;
		}
		else
		{

			NZLQSG=Report_input.GetRecordCount();

			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQSG[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLQSG[i].gw=_wtof(field);
				Report_input.GetValueString(field,4);
				ZLQSG[i].gs=_wtof(field);
				Report_input.GetValueString(field,5);
				ZLQSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,6);
				ZLQSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,7);
				ZLQSG[i].Jh=_wtof(field);//浆砌厚度
				Report_input.GetValueString(field,8);
				ZLQSG[i].GDR=_wtof(field);//浆砌厚度

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//	   MyPro->SetPosgress(54);
	//	   MyPro->SetPosgress(56);
	TabName="左天沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZTG=0;
			/*
			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			ZTG[0].sdml = Sxlc;
			ZTG[0].edml = Exlc;	

			ZTG[0].sbj=2;//起始边距（坡顶离水沟内侧距离）
			ZTG[0].sgw=0.4;//沟宽
			ZTG[0].sgs=0.6;//沟深
			ZTG[0].ebj=2;//终止边距
			ZTG[0].egw=0.4;//沟宽
			ZTG[0].egs=0.6;//沟深
			ZTG[0].nbp=1;//内侧边坡
			ZTG[0].wbp=1;//外侧边坡
			ZTG[0].jqhd=0.3;//浆切厚度*/

		}
		else
		{

			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}

			NZTG=Report_input.GetRecordCount();

			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZTG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZTG[i].sbj=_wtof(field);
				Report_input.GetValueString(field,4);
				ZTG[i].sgs=_wtof(field);
				Report_input.GetValueString(field,5);
				ZTG[i].sgw =_wtof(field);

				Report_input.GetValueString(rcd,6);
				ZTG[i].edml=_wtof(rcd);

				Report_input.GetValueString(field,7);
				ZTG[i].ebj=_wtof(field);
				Report_input.GetValueString(field,8);//3列后是起始边距
				ZTG[i].egs =_wtof(field);
				Report_input.GetValueString(field,9);//3列后是起始边距
				ZTG[i].egw=_wtof(field);
				Report_input.GetValueString(field,10);//3列后是起始边距
				ZTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3列后是起始边距
				ZTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3列后是起始边距
				ZTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3列后是起始边距
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3列后是起始边距
					ZTG[i].TG_GDR=0.0;
				}
				else
					ZTG[i].TG_GDR=_wtof(field);

				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3列后是起始边距
				// 					  ZTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3列后是起始边距
				// 					  ZTG[i].TG_GDR=_wtof(field);
				// 				  }

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(58);
	//	   MyPro->SetPosgress(60);

	//---------------------------------------------------------------
	TabName="右挡墙";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYDQ=0;
			YDQ=NULL;
			/*
			if(YDQ) {delete []YDQ;YDQ=NULL;}
			YDQ=new dqdata[NYDQ];
			YDQ[0].sdml = Sxlc;
			YDQ[0].edml = Exlc;
			YDQ[0].szj = 100.0;
			YDQ[0].ezj = 100.0;	*/

		}
		else
		{
			NYDQ=Report_input.GetRecordCount();

			if(YDQ) {delete []YDQ;YDQ=NULL;}
			YDQ=(dqdata *)new dqdata[NYDQ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YDQ[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YDQ[i].szj=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				YDQ[i].edml=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				YDQ[i].ezj=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="右低填路堤自动填平";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDTP=0;
		}
		else
		{
			NYLDTP=Report_input.GetRecordCount();

			if(YLDTP) {delete []YLDTP;YLDTP=NULL;}
			YLDTP=(LDTPData *)new LDTPData[NYLDTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLDTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLDTP[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="右低挖路堑自动推平";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQTP=0;
		}
		else
		{
			NYLQTP=Report_input.GetRecordCount();

			if(YLQTP) {delete []YLQTP;YLQTP=NULL;}
			YLQTP=(LQTPData *)new LQTPData[NYLQTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLQTP[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="右低填路堤自动加宽土路肩";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYTLJJK=0;
		}
		else
		{
			NYTLJJK=Report_input.GetRecordCount();

			if(YTLJJK) {delete []YTLJJK;YTLJJK=NULL;}
			YTLJJK=(TLJJKData *)new TLJJKData[NYTLJJK];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YTLJJK[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YTLJJK[i].edml=_wtof(rcd);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	TabName="右清表土厚度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYHTHD=1;
			if(YHTHD) {delete []YHTHD;YHTHD=NULL;}
			YHTHD=new hthddata[NYHTHD];

			YHTHD[0].dml=Exlc;	
			YHTHD[0].hthd=0.0;
			YHTHD[0].xz=0.0;
		}
		else
		{
			NYHTHD=Report_input.GetRecordCount();

			if(YHTHD) {delete []YHTHD;YHTHD=NULL;}
			YHTHD=(hthddata*)new hthddata[NYHTHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YHTHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YHTHD[i].hthd=_wtof(field);
				Report_input.GetValueString(field,4);
				YHTHD[i].xz=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	TabName="右沟底坡";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYGouPD=0;
			YGouPD = NULL;
		}
		else
		{
			NYGouPD=Report_input.GetRecordCount();

			if(YGouPD) {delete []YGouPD;YGouPD=NULL;}
			YGouPD=(GouPDdata*)new GouPDdata[NYGouPD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YGouPD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YGouPD[i].SH=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YGouPD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YGouPD[i].EH=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	TabName="右超填宽度表";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYCTKD=0;
			YCTKD = NULL;
		}
		else
		{
			NYCTKD=Report_input.GetRecordCount();

			if(YCTKD) {delete []YCTKD;YCTKD=NULL;}
			YCTKD=(ctkddata*)new ctkddata[NYCTKD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YCTKD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YCTKD[i].Sctkd=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YCTKD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YCTKD[i].Ectkd=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	TabName="右弧形边坡表";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYARCBP=0;
			YARCBP = NULL;
		}
		else
		{
			NYARCBP=Report_input.GetRecordCount();

			if(YARCBP) {delete []YARCBP;YARCBP=NULL;}
			YARCBP=(arcbpata*)new arcbpata[NYARCBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YARCBP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YARCBP[i].SR=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YARCBP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YARCBP[i].ER=_wtof(field);
				Report_input.GetValueString(field,6);
				YARCBP[i].ArcLocation=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//右侧
	TabName="右路面厚度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLMHD=1;
			if(YLMHD) {delete []YLMHD;YLMHD=NULL;}
			YLMHD=new LMHdata[NYLMHD];
			YLMHD[0].dml = Exlc;
			YLMHD[0].cxdh = 0.0;
			YLMHD[0].yljh = 0.0;
			YLMHD[0].tljh = 0.0;
		}
		else
		{
			NYLMHD=Report_input.GetRecordCount();

			if(YLMHD) {delete []YLMHD;YLMHD=NULL;}
			YLMHD=(LMHdata*)new LMHdata[NYLMHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLMHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLMHD[i].cxdh=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				YLMHD[i].yljh=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				YLMHD[i].tljh=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	
	//	MyPro->SetPosgress(62);
	//	MyPro->SetPosgress(64);
	//------------------------------------------------------
	TabName="右地面线内插宽";
	{
		//以下数据分右右侧	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYCRS=1;
			if(YCRSWID) {delete []YCRSWID;YCRSWID=NULL;}
			YCRSWID=new crswiddata[NYCRS];

			YCRSWID[0].dml = Exlc;
			YCRSWID[0].wid=100.0;		   
		}
		else
		{
			NYCRS=Report_input.GetRecordCount();

			if(YCRSWID) {delete []YCRSWID;YCRSWID=NULL;}
			YCRSWID=(crswiddata* )new crswiddata[NYCRS];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YCRSWID[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YCRSWID[i].wid=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(68);
	//	   MyPro->SetPosgress(70);

	TabName="右横断面限制宽度";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYDM=1;
			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=new dmdata[NYDM];

			YDM[0].dml = Exlc;	
			YDM[0].xk=100.0;//限制宽
		}
		else
		{
			NYDM=Report_input.GetRecordCount();

			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=(dmdata* )new dmdata[NYDM];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YDM[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YDM[i].xk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(74);
	//	   MyPro->SetPosgress(76);
	TabName="右用地加宽";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYPWD=1;
			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=new pwddata[NYPWD];
			YPWD[0].dml = Exlc;	
			YPWD[0].ltjk=YPWD[0].lqjk=1.0;//外宽
		}
		else
		{
			NYPWD=Report_input.GetRecordCount();

			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=(pwddata* )new pwddata[NYPWD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YPWD[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YPWD[i].ltjk=_wtof(field);
				Report_input.GetValueString(field,4);
				YPWD[i].lqjk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(78);
	//	   MyPro->SetPosgress(80);
	TabName="右路堤边坡";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDBP=1;
			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//动态声请一个空间
			YLDBP[0].dml = Exlc;	

			YLDBP[0].BPJS  = 3;
			YLDBP[0].bpdata[0]  = 1.5;
			YLDBP[0].bpdata[1]  = 8;
			YLDBP[0].bpdata[2]  = 1.0;
			YLDBP[0].bpdata[3]  = 0.0;

			YLDBP[0].bpdata[4]  = 1.75;
			YLDBP[0].bpdata[5]  = 10;
			YLDBP[0].bpdata[6]  = 1.0;
			YLDBP[0].bpdata[7]  = 0.0;

			YLDBP[0].bpdata[8]  = 2.0;
			YLDBP[0].bpdata[9]  = 100;
			YLDBP[0].bpdata[10]  = 1.0;
			YLDBP[0].bpdata[11]  = 0.0;
		}
		else
		{
			NYLDBP=Report_input.GetRecordCount();

			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//动态声请一个空间
			Report_input.MoveFirst();
			k=1;
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLDBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLDBP[i].BPJS=_wtoi(field);
				for( j=0;j<YLDBP[i].BPJS*4;j++)
				{
					Report_input.GetValueString(field,4+j);
					YLDBP[i].bpdata[j]=_wtof(field);


				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	// ZLDBP[i-1].bpdata[(ZLDBP[i-1].BPJS-1)*4+1]=100;
	//	   MyPro->SetPosgress(82);
	//	   MyPro->SetPosgress(84);
	TabName="右路堑边坡";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQBP=1;
			// if(YLQBP) {delete []YLQBP;YLQBP=NULL;}
			//YLQBP=(LDBPdata* )new LDBPdata[NYLQBP];//动态声请一个空间
			YLQBP[0].dml = Exlc;	

			YLQBP[0].BPJS = 3;
			YLQBP[0].bpdata[0]  = 1;
			YLQBP[0].bpdata[1]  = 8;
			YLQBP[0].bpdata[2]  = 1.0;
			YLQBP[0].bpdata[3] = 0.0;

			YLQBP[0].bpdata[4]  = 1.25;
			YLQBP[0].bpdata[5]  = 10;
			YLQBP[0].bpdata[6]  = 1.0;
			YLQBP[0].bpdata[7]  = 0.0;

			YLQBP[0].bpdata[8] =  1.5;
			YLQBP[0].bpdata[9] =  100;
			YLQBP[0].bpdata[10] =  1.0;
			YLQBP[0].bpdata[11] =  0.0;

		}
		else
		{
			NYLQBP=Report_input.GetRecordCount();
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLQBP[i].BPJS=_wtoi(field);
				for( j=0;j<YLQBP[i].BPJS*4;j++)
				{					   
					Report_input.GetValueString(field,4+j);
					YLQBP[i].bpdata[j]=_wtof(field);					   
				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//ZLQBP[i-1].bpdata[(ZLQBP[i-1].BPJS-1)*4+1]=100;
	//	   MyPro->SetPosgress(86);
	//	   MyPro->SetPosgress(88);
	TabName="右排水沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDSG=0;
			/*
			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//动态声请一个空间
			YLDSG[0].sdml = Sxlc;	
			YLDSG[0].edml = Exlc;	

			YLDSG[0].WallorGou  = 1;//默认的是水沟
			YLDSG[0].S_HPDK=YLDSG[0].E_HPDK=2;//护坡道宽
			YLDSG[0].S_HPDH=YLDSG[0].E_HPDH=0.5;//护坡道宽
			YLDSG[0].S_DSYK=YLDSG[0].E_DSYK=1;//护坡道宽
			YLDSG[0].S_DSYH=YLDSG[0].E_DSYH=0.5;//护坡道宽
			YLDSG[0].S_GK=YLDSG[0].E_GK=0.4;//横坡
			YLDSG[0].S_GS=YLDSG[0].E_GS=0.6;//横坡
			YLDSG[0].nbp  = 1;//内侧边坡
			YLDSG[0].wbp  = 1;//外侧边坡
			YLDSG[0].DSYP  = 1;//外侧边坡
			YLDSG[0].jh   = 0.2;//浆砌厚度*/

		}
		else
		{

			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}

			NYLDSG=Report_input.GetRecordCount();

			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(field,2);
				YLDSG[i].WallorGou=_wtoi(field);
				Report_input.GetValueString(field,3);
				YLDSG[i].jh=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YLDSG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,5);
				YLDSG[i].S_HPDK=_wtof(field);
				Report_input.GetValueString(field,6);
				YLDSG[i].S_HPDH=_wtof(field);
				Report_input.GetValueString(field,7);
				YLDSG[i].S_GS=_wtof(field);
				Report_input.GetValueString(field,8);
				YLDSG[i].S_GK=_wtof(field);
				Report_input.GetValueString(field,9);
				YLDSG[i].S_DSYK=_wtof(field);
				Report_input.GetValueString(field,10);
				YLDSG[i].S_DSYH=_wtof(field);

				Report_input.GetValueString(field,11);
				YLDSG[i].edml=_wtof(field);
				Report_input.GetValueString(field,12);
				YLDSG[i].E_HPDK=_wtof(field);
				Report_input.GetValueString(field,13);
				YLDSG[i].E_HPDH=_wtof(field);
				Report_input.GetValueString(field,14);
				YLDSG[i].E_GS=_wtof(field);
				Report_input.GetValueString(field,15);
				YLDSG[i].E_GK=_wtof(field);
				Report_input.GetValueString(field,16);
				YLDSG[i].E_DSYK=_wtof(field);
				Report_input.GetValueString(field,17);
				YLDSG[i].E_DSYH=_wtof(field);
				Report_input.GetValueString(field,18);
				YLDSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,19);
				YLDSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,20);
				YLDSG[i].DSYP=_wtof(field);
				Report_input.GetValueString(field,21);
				if (field=="NULL")
				{
					Report_input.PutCollect(21,0.0);
					YLDSG[i].LDSG_GDR=0.0;
				}
				else
					YLDSG[i].LDSG_GDR=_wtof(field);
				// 			   if (IsAddField)
				// 			   {
				// 				   Report_input.PutCollect(21,0.0);
				// 				   YLDSG[i].LDSG_GDR=0.0;
				// 			   }
				// 			   else
				// 			   {
				// 				   Report_input.GetValueString(field,21);
				// 				   YLDSG[i].LDSG_GDR=_wtof(field);
				// 			   }
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//	   MyPro->SetPosgress(92);
	TabName="右侧沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQSG=1;
			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			YLQSG[0].dml = Exlc;	

			YLQSG[0].gw=0.4;
			YLQSG[0].gs = 0.6;
			YLQSG[0].nbp= 1;
			YLQSG[0].wbp = 1;
			YLQSG[0].Jh=0.3;////浆砌厚度
			YLQSG[0].GDR=0.0;////浆砌厚度
		}
		else
		{
			NYLQSG=Report_input.GetRecordCount();

			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQSG[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLQSG[i].gw=_wtof(field);
				Report_input.GetValueString(field,4);
				YLQSG[i].gs=_wtof(field);
				Report_input.GetValueString(field,5);
				YLQSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,6);
				YLQSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,7);
				YLQSG[i].Jh=_wtof(field);//浆砌厚度
				Report_input.GetValueString(field,8);
				YLQSG[i].GDR=_wtof(field);//浆砌厚度

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//	   MyPro->SetPosgress(94);
	//	   MyPro->SetPosgress(96);
	TabName="右天沟";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYTG=0;
			/*
			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			YTG[0].sdml = Sxlc;
			YTG[0].edml = Exlc;	

			YTG[0].sbj=2;//起始边距（坡顶离水沟内侧距离）
			YTG[0].sgw=0.4;//沟宽
			YTG[0].sgs=0.6;//沟深
			YTG[0].ebj=2;//终止边距
			YTG[0].egw=0.4;//沟宽
			YTG[0].egs=0.6;//沟深
			YTG[0].nbp=1;//内侧边坡
			YTG[0].wbp=1;//外侧边坡
			YTG[0].jqhd=0.3;//浆切厚度*/

		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NYTG=Report_input.GetRecordCount();

			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YTG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YTG[i].sbj=_wtof(field);
				Report_input.GetValueString(field,4);
				YTG[i].sgs =_wtof(field);
				Report_input.GetValueString(field,5);
				YTG[i].sgw=_wtof(field);

				Report_input.GetValueString(rcd,6);
				YTG[i].edml=_wtof(rcd);

				Report_input.GetValueString(field,7);
				YTG[i].ebj=_wtof(field);
				Report_input.GetValueString(field,8);//3列后是起始边距
				YTG[i].egs =_wtof(field);
				Report_input.GetValueString(field,9);//3列后是起始边距
				YTG[i].egw=_wtof(field);
				Report_input.GetValueString(field,10);//3列后是起始边距
				YTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3列后是起始边距
				YTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3列后是起始边距
				YTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3列后是起始边距
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3列后是起始边距
					YTG[i].TG_GDR=0.0;
				}
				else
					YTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3列后是起始边距
				// 					  YTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3列后是起始边距
				// 					  YTG[i].TG_GDR=_wtof(field);
				// 				  }
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//20191017	读取路面分层数据
	g_RoadWorkingDatabase.GetRoadLayer(mdbFname, roadname, LEFT, m_RoadLayerArray_L);
	g_RoadWorkingDatabase.GetRoadLayer(mdbFname, roadname, RIGHT, m_RoadLayerArray_R);

	//	  MyPro->SetPosgress(98);
	//	  MyPro->SetPosgress(100);
	return 1;
}

int HdmDataBase::Read_HdmDbs(CString mdbFname,CString roadname,CString TabName)
{
	int i=0,j,k;
	CString rcd;
	CString field;
	double Sxlc,Exlc;


	GetRoadSEml(mdbFname,roadname,Sxlc,Exlc);

	if(TabName=="松方压实系数")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			YSXS[0]=1.23,YSXS[1]=1.16,YSXS[2]=1.09,YSXS[3]=0.92,YSXS[4]=0.92,YSXS[5]=0.92;
		}
		else //从数据库取值
		{
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,1);
				YSXS[0]=_wtof(rcd);
				Report_input.GetValueString(rcd,2);
				YSXS[1]=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YSXS[2]=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				YSXS[3]=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				YSXS[4]=_wtof(rcd);
				Report_input.GetValueString(rcd,6);
				YSXS[5]=_wtof(rcd);				
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}
	if(TabName=="填方分类")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NLDTL=1;
			if(LDTL) {delete []LDTL;LDTL=NULL;}
			LDTL=new LDTLdata[NLDTL];
			LDTL[0].dml=Exlc;//////////////改良土厚度
			LDTL[0].type = 0;				
		}
		else //从数据库取值
		{
			NLDTL=Report_input.GetRecordCount();

			if(LDTL) {delete []LDTL;LDTL=NULL;}
			LDTL=(LDTLdata *)new LDTLdata[NLDTL];//动态声请一个空间
			Report_input.MoveFirst();
			//Report_input.GetValueString()
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				LDTL[i].dml=_wtof(rcd);

				Report_input.GetValueString(rcd,3);
				LDTL[i].type=_wtoi(rcd);

				i++;
				Report_input.MoveNext();
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}
	CString GH;
	i=0;
	if(TabName=="用地面积汇总分段表")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYD=1;
			if(FDB) {delete []FDB;FDB=NULL;}
			FDB=(YongDiFDB *)new YongDiFDB[NYD];//动态声请一个空间
			FDB[0].dml = Exlc;	
			FDB[0].BZ="";
		}
		else
		{

			NYD=Report_input.GetRecordCount();

			if(FDB) {delete []FDB;FDB=NULL;}
			FDB=(YongDiFDB *)new YongDiFDB[NYD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				FDB[i].dml=_wtof(rcd);

				Report_input.GetValueString(FDB[i].BZ,3);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}

	if(TabName=="用地类型表")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYDLX=1;
			if(LXB){delete []LXB;LXB=NULL;}
			LXB=new YongDiLXB[NYDLX];
			LXB[0].dml = Exlc;	
			LXB[0].YDLX="水稻田";
		}
		else
		{		
			NYDLX=Report_input.GetRecordCount();
			if(LXB){delete []LXB;LXB=NULL;}
			LXB=new YongDiLXB[NYDLX];
			Report_input.MoveFirst();

			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				LXB[i].dml=_wtof(rcd);
				Report_input.GetValueString(LXB[i].YDLX,3);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	if(TabName=="挖方分类")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NWFFL=1;
			if(WFFL) {delete []WFFL;WFFL=NULL;}
			WFFL=new WFFLdata[NWFFL];

			WFFL[0].dml = Exlc;	
			for(int i=0; i<6; i++)
			{
				WFFL[0].sj[i] = 0;
			}
			WFFL[0].sj[2] = 100;//III
		}
		else
		{

			NWFFL=Report_input.GetRecordCount();

			if(WFFL) {delete []WFFL;WFFL=NULL;}
			WFFL=(WFFLdata *)new WFFLdata[NWFFL];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				WFFL[i].dml=_wtof(rcd);

				for(j=0;j<6;j++)
				{
					Report_input.GetValueString(field,j+3);
					WFFL[i].sj[j]=_wtof(field);
				}
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (TabName=="关联断面路肩拉直")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMLZ=1;
			if(GLDMLZ) {delete []GLDMLZ;GLDMLZ=NULL;}
			GLDMLZ=new GLDMLZdata[NGLDMLZ];

			GLDMLZ[0].Sdml = Sxlc;
			GLDMLZ[0].Edml = Exlc;
			GLDMLZ[0].dX = 5.0;
			GLDMLZ[0].dH = 1.0;

		}
		else
		{			
			NGLDMLZ=Report_input.GetRecordCount();

			if(GLDMLZ) {delete []GLDMLZ;GLDMLZ=NULL;}
			GLDMLZ=new GLDMLZdata[NGLDMLZ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMLZ[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMLZ[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMLZ[i].dX=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				GLDMLZ[i].dH=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	if (TabName=="关联断面路堑边坡推平")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMLQBPTP=1;
			if(GLDMLQBPTP) {delete []GLDMLQBPTP;GLDMLQBPTP=NULL;}
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];

			GLDMLQBPTP[0].Sdml = Sxlc;
			GLDMLQBPTP[0].Edml = Exlc;
			GLDMLQBPTP[0].dX = 5.0;
		}
		else
		{			
			NGLDMLQBPTP=Report_input.GetRecordCount();

			if(GLDMLQBPTP) {delete []GLDMLQBPTP;GLDMLQBPTP=NULL;}
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMLQBPTP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMLQBPTP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMLQBPTP[i].dX=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	if (TabName=="关联断面线间沟")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NGLDMXJG=1;
			if(GLDMXJG) {delete []GLDMXJG;GLDMXJG=NULL;}
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];

			GLDMXJG[0].Sdml = Sxlc;
			GLDMXJG[0].Edml = Exlc;
			GLDMXJG[0].GouWide = 0.5;
			GLDMXJG[0].GouHigh = 0.7;
			GLDMXJG[0].NBP = 0.0;
			GLDMXJG[0].WBP = 0.0;
			GLDMXJG[0].JQHD = 0.3;
			GLDMXJG[0].GDR = 0.0;
		}
		else
		{			
			NGLDMXJG=Report_input.GetRecordCount();

			if(GLDMXJG) {delete []GLDMXJG;GLDMXJG=NULL;}
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				GLDMXJG[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				GLDMXJG[i].Edml=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				GLDMXJG[i].GouWide=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				GLDMXJG[i].GouHigh=_wtof(rcd);
				Report_input.GetValueString(rcd,6);
				GLDMXJG[i].NBP=_wtof(rcd);
				Report_input.GetValueString(rcd,7);
				GLDMXJG[i].WBP=_wtof(rcd);
				Report_input.GetValueString(rcd,8);
				GLDMXJG[i].JQHD=_wtof(rcd);
				Report_input.GetValueString(rcd,9);
				GLDMXJG[i].GDR=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}
	}

	//////////////////////////////////////////////////////////////////////////

	if(TabName=="左沟底坡")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZGouPD=0;
			ZGouPD = NULL;
		}
		else
		{
			NZGouPD=Report_input.GetRecordCount();

			if(ZGouPD) {delete []ZGouPD;ZGouPD=NULL;}
			ZGouPD=(GouPDdata*)new GouPDdata[NZGouPD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZGouPD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZGouPD[i].SH=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZGouPD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZGouPD[i].EH=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左超填宽度表")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZCTKD=0;
			ZCTKD = NULL;
		}
		else
		{
			NZCTKD=Report_input.GetRecordCount();

			if(ZCTKD) {delete []ZCTKD;ZCTKD=NULL;}
			ZCTKD=(ctkddata*)new ctkddata[NZCTKD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZCTKD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZCTKD[i].Sctkd=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZCTKD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZCTKD[i].Ectkd=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	if(TabName=="左弧形边坡表")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZARCBP=0;
			ZARCBP = NULL;
		}
		else
		{
			NZARCBP=Report_input.GetRecordCount();

			if(ZARCBP) {delete []ZARCBP;ZARCBP=NULL;}
			ZARCBP=(arcbpata*)new arcbpata[NZARCBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZARCBP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZARCBP[i].SR=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZARCBP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				ZARCBP[i].ER=_wtof(field);
				Report_input.GetValueString(field,6);
				ZARCBP[i].ArcLocation=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左挡墙")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZDQ=1;
			if(ZDQ) {delete []ZDQ;ZDQ=NULL;}
			ZDQ=new dqdata[NZDQ];
			ZDQ[0].sdml = Sxlc;
			ZDQ[0].edml = Exlc;
			ZDQ[0].szj = 100.0;
			ZDQ[0].ezj = 100.0;	
		}
		else
		{
			NZDQ=Report_input.GetRecordCount();

			if(ZDQ) {delete []ZDQ;ZDQ=NULL;}
			ZDQ=(dqdata *)new dqdata[NZDQ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZDQ[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZDQ[i].szj=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				ZDQ[i].edml=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				ZDQ[i].ezj=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="左低填路堤自动填平")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDTP=1;
			if(ZLDTP) {delete []ZLDTP;ZLDTP=NULL;}
			ZLDTP=new LDTPData[NZLDTP];
			ZLDTP[0].sdml = Sxlc;
			ZLDTP[0].edml = Exlc;
		}
		else
		{
			NZLDTP=Report_input.GetRecordCount();

			if(ZLDTP) {delete []ZLDTP;ZLDTP=NULL;}
			ZLDTP=(LDTPData *)new LDTPData[NZLDTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLDTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLDTP[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="左低挖路堑自动推平")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLQTP=1;
			if(ZLQTP) {delete []ZLQTP;ZLQTP=NULL;}
			ZLQTP=new LQTPData[NZLQTP];
			ZLQTP[0].sdml = Sxlc;
			ZLQTP[0].edml = Exlc;
		}
		else
		{
			NZLQTP=Report_input.GetRecordCount();

			if(ZLQTP) {delete []ZLQTP;ZLQTP=NULL;}
			ZLQTP=(LQTPData *)new LQTPData[NZLQTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLQTP[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="左低填路堤自动加宽土路肩")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZTLJJK=1;
			if(ZTLJJK) {delete []ZTLJJK;ZTLJJK=NULL;}
			ZTLJJK=new TLJJKData[NZTLJJK];
			ZTLJJK[0].sdml = Sxlc;
			ZTLJJK[0].edml = Exlc;
		}
		else
		{
			NZTLJJK=Report_input.GetRecordCount();

			if(ZTLJJK) {delete []ZTLJJK;ZTLJJK=NULL;}
			ZTLJJK=(TLJJKData *)new TLJJKData[NZTLJJK];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZTLJJK[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZTLJJK[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="左清表土厚度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZHTHD=1;
			if(ZHTHD) {delete []ZHTHD;ZHTHD=NULL;}
			ZHTHD=new hthddata[NZHTHD];

			ZHTHD[0].dml=Exlc;	
			ZHTHD[0].hthd=0.0;
			ZHTHD[0].xz=0;
		}
		else
		{
			NZHTHD=Report_input.GetRecordCount();

			if(ZHTHD) {delete []ZHTHD;ZHTHD=NULL;}
			ZHTHD=(hthddata*)new hthddata[NZHTHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZHTHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZHTHD[i].hthd=_wtof(field);
				Report_input.GetValueString(field,4);
				ZHTHD[i].xz=_wtoi(field);					
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//以下数据分左右	///////////////////////////////////////////////////////////
	if(TabName=="左路面厚度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLMHD=1;
			if(ZLMHD) {delete []ZLMHD;ZLMHD=NULL;}
			ZLMHD=new LMHdata[NZLMHD];
			ZLMHD[0].dml = Exlc;
			ZLMHD[0].cxdh = 0.5;
			ZLMHD[0].yljh = 0.25;
			ZLMHD[0].tljh = 0.1;
		}
		else
		{
			NZLMHD=Report_input.GetRecordCount();

			if(ZLMHD) {delete []ZLMHD;ZLMHD=NULL;}
			ZLMHD=(LMHdata*)new LMHdata[NZLMHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLMHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				ZLMHD[i].cxdh=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				ZLMHD[i].yljh=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				ZLMHD[i].tljh=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//------------------------------------------------------
	if(TabName=="左地面线内插宽")
	{
		//以下数据分左右侧	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZCRS=1;
			if(ZCRSWID) {delete []ZCRSWID;ZCRSWID=NULL;}
			ZCRSWID=new crswiddata[NZCRS];

			ZCRSWID[0].dml = Exlc;
			ZCRSWID[0].wid=100.0;		   
		}
		else
		{
			NZCRS=Report_input.GetRecordCount();

			if(ZCRSWID) {delete []ZCRSWID;ZCRSWID=NULL;}
			ZCRSWID=(crswiddata* )new crswiddata[NZCRS];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZCRSWID[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				ZCRSWID[i].wid=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左横断面限制宽度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZDM=1;
			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=new dmdata[NZDM];

			ZDM[0].dml = Exlc;	
			ZDM[0].xk=100.0;//限制宽
		}
		else
		{
			NZDM=Report_input.GetRecordCount();

			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=(dmdata* )new dmdata[NZDM];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZDM[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZDM[i].xk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左用地加宽")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZPWD=1;
			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=new pwddata[NZPWD];
			ZPWD[0].dml = Exlc;	
			ZPWD[0].ltjk=ZPWD[0].lqjk=1.0;//外宽
		}
		else
		{
			NZPWD=Report_input.GetRecordCount();

			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=(pwddata* )new pwddata[NZPWD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZPWD[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZPWD[i].ltjk=_wtof(field);
				Report_input.GetValueString(field,4);
				ZPWD[i].lqjk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左路堤边坡")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDBP=1;
			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//动态声请一个空间
			ZLDBP[0].dml = Exlc;	

			ZLDBP[0].BPJS  = 3;
			ZLDBP[0].bpdata[0]  = 1.5;
			ZLDBP[0].bpdata[1]  = 8;
			ZLDBP[0].bpdata[2]  = 1.0;
			ZLDBP[0].bpdata[3]  = 0.0;

			ZLDBP[0].bpdata[4]  = 1.75;
			ZLDBP[0].bpdata[5]  = 10;
			ZLDBP[0].bpdata[6]  = 1.0;
			ZLDBP[0].bpdata[7]  = 0.0;

			ZLDBP[0].bpdata[8]  = 2.0;
			ZLDBP[0].bpdata[9]  = 100;
			ZLDBP[0].bpdata[10]  = 1.0;
			ZLDBP[0].bpdata[11]  = 0.0;
		}
		else
		{
			NZLDBP=Report_input.GetRecordCount();

			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//动态声请一个空间
			Report_input.MoveFirst();
			k=1;
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLDBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLDBP[i].BPJS=_wtoi(field);
				for( j=0;j<ZLDBP[i].BPJS*4;j++)
				{
					Report_input.GetValueString(field,4+j);
					ZLDBP[i].bpdata[j]=_wtof(field);


				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	// ZLDBP[i-1].bpdata[(ZLDBP[i-1].BPJS-1)*4+1]=100;

	if(TabName=="左路堑边坡")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLQBP=1;
			// if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//动态声请一个空间
			ZLQBP[0].dml = Exlc;	

			ZLQBP[0].BPJS = 3;
			ZLQBP[0].bpdata[0]  = 1;
			ZLQBP[0].bpdata[1]  = 8;
			ZLQBP[0].bpdata[2]  = 1.0;
			ZLQBP[0].bpdata[3] = 0.0;

			ZLQBP[0].bpdata[4]  = 1.25;
			ZLQBP[0].bpdata[5]  = 10;
			ZLQBP[0].bpdata[6]  = 1.0;
			ZLQBP[0].bpdata[7]  = 0.0;

			ZLQBP[0].bpdata[8] =  1.5;
			ZLQBP[0].bpdata[9] =  100;
			ZLQBP[0].bpdata[10] =  1.0;
			ZLQBP[0].bpdata[11] =  0.0;

		}
		else
		{
			NZLQBP=Report_input.GetRecordCount();

			//if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLQBP[i].BPJS=_wtoi(field);
				for( j=0;j<ZLQBP[i].BPJS*4;j++)
				{					   
					Report_input.GetValueString(field,4+j);
					ZLQBP[i].bpdata[j]=_wtof(field);					   
				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//ZLQBP[i-1].bpdata[(ZLQBP[i-1].BPJS-1)*4+1]=100;

	if(TabName=="左排水沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLDSG=1;
			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//动态声请一个空间
			ZLDSG[0].sdml = Sxlc;	
			ZLDSG[0].edml = Exlc;	

			ZLDSG[0].WallorGou  = 1;//默认的是水沟
			ZLDSG[0].S_HPDK=ZLDSG[0].E_HPDK=2;//护坡道宽
			ZLDSG[0].S_HPDH=ZLDSG[0].E_HPDH=0.5;//护坡道宽
			ZLDSG[0].S_DSYK=ZLDSG[0].E_DSYK=1;//护坡道宽
			ZLDSG[0].S_DSYH=ZLDSG[0].E_DSYH=0.5;//护坡道宽
			ZLDSG[0].S_GK=ZLDSG[0].E_GK=0.4;//横坡
			ZLDSG[0].S_GS=ZLDSG[0].E_GS=0.6;//横坡
			ZLDSG[0].nbp  = 1;//内侧边坡
			ZLDSG[0].wbp  = 1;//外侧边坡
			ZLDSG[0].DSYP  = 1;//外侧边坡
			ZLDSG[0].jh   = 0.2;//浆砌厚度
			ZLDSG[0].LDSG_GDR   = 0.0;//沟底半径
		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}

			NZLDSG=Report_input.GetRecordCount();

			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(field,2);
				ZLDSG[i].WallorGou=_wtoi(field);
				Report_input.GetValueString(field,3);
				ZLDSG[i].jh=_wtof(field);
				Report_input.GetValueString(rcd,4);
				ZLDSG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,5);
				ZLDSG[i].S_HPDK=_wtof(field);
				Report_input.GetValueString(field,6);
				ZLDSG[i].S_HPDH=_wtof(field);
				Report_input.GetValueString(field,7);
				ZLDSG[i].S_GS=_wtof(field);
				Report_input.GetValueString(field,8);
				ZLDSG[i].S_GK=_wtof(field);
				Report_input.GetValueString(field,9);
				ZLDSG[i].S_DSYK=_wtof(field);
				Report_input.GetValueString(field,10);
				ZLDSG[i].S_DSYH=_wtof(field);

				Report_input.GetValueString(field,11);
				ZLDSG[i].edml=_wtof(field);
				Report_input.GetValueString(field,12);
				ZLDSG[i].E_HPDK=_wtof(field);
				Report_input.GetValueString(field,13);
				ZLDSG[i].E_HPDH=_wtof(field);
				Report_input.GetValueString(field,14);
				ZLDSG[i].E_GS=_wtof(field);
				Report_input.GetValueString(field,15);
				ZLDSG[i].E_GK=_wtof(field);
				Report_input.GetValueString(field,16);
				ZLDSG[i].E_DSYK=_wtof(field);
				Report_input.GetValueString(field,17);
				ZLDSG[i].E_DSYH=_wtof(field);
				Report_input.GetValueString(field,18);
				ZLDSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,19);
				ZLDSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,20);
				ZLDSG[i].DSYP=_wtof(field);
				Report_input.GetValueString(field,21);
				if (field=="NULL")
				{
					Report_input.PutCollect(21,0.0);
					ZLDSG[i].LDSG_GDR=0.0;
				}
				else
					ZLDSG[i].LDSG_GDR=_wtof(field);
				// 			   if (IsAddField)
				// 			   {
				// 				   Report_input.PutCollect(21,0.0);
				// 				   ZLDSG[i].LDSG_GDR=0.0;
				// 			   }
				// 			   else
				// 			   {
				// 				   Report_input.GetValueString(field,21);
				// 				   ZLDSG[i].LDSG_GDR=_wtof(field);
				// 			   }

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	if(TabName=="左侧沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZLQSG=1;
			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			ZLQSG[0].dml = Exlc;	

			ZLQSG[0].gw=0.4;
			ZLQSG[0].gs = 0.6;
			ZLQSG[0].nbp= 1;
			ZLQSG[0].wbp = 1;
			ZLQSG[0].Jh=0.3;////浆砌厚度
			ZLQSG[0].GDR=0.0;
		}
		else
		{
			NZLQSG=Report_input.GetRecordCount();

			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZLQSG[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZLQSG[i].gw=_wtof(field);
				Report_input.GetValueString(field,4);
				ZLQSG[i].gs=_wtof(field);
				Report_input.GetValueString(field,5);
				ZLQSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,6);
				ZLQSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,7);
				ZLQSG[i].Jh=_wtof(field);//浆砌厚度
				Report_input.GetValueString(field,8);
				ZLQSG[i].GDR=_wtof(field);//浆砌厚度

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="左天沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NZTG=1;
			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			ZTG[0].sdml = Sxlc;
			ZTG[0].edml = Exlc;	

			ZTG[0].sbj=2;//起始边距（坡顶离水沟内侧距离）
			ZTG[0].sgw=0.4;//沟宽
			ZTG[0].sgs=0.6;//沟深
			ZTG[0].ebj=2;//终止边距
			ZTG[0].egw=0.4;//沟宽
			ZTG[0].egs=0.6;//沟深
			ZTG[0].nbp=1;//内侧边坡
			ZTG[0].wbp=1;//外侧边坡
			ZTG[0].jqhd=0.3;//浆切厚度
			ZTG[0].TG_GDR = 0.0;//沟底半径
		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NZTG=Report_input.GetRecordCount();

			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				ZTG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				ZTG[i].sbj=_wtof(field);
				Report_input.GetValueString(field,4);
				ZTG[i].sgs =_wtof(field);
				Report_input.GetValueString(field,5);
				ZTG[i].sgw=_wtof(field);

				Report_input.GetValueString(rcd,6);
				ZTG[i].edml=_wtof(rcd);

				Report_input.GetValueString(field,7);
				ZTG[i].ebj=_wtof(field);
				Report_input.GetValueString(field,8);//3列后是起始边距
				ZTG[i].egs=_wtof(field);
				Report_input.GetValueString(field,9);//3列后是起始边距
				ZTG[i].egw =_wtof(field);
				Report_input.GetValueString(field,10);//3列后是起始边距
				ZTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3列后是起始边距
				ZTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3列后是起始边距
				ZTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3列后是起始边距
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3列后是起始边距
					ZTG[i].TG_GDR=0.0;
				}
				else
					ZTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3列后是起始边距
				// 					  ZTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3列后是起始边距
				// 					  ZTG[i].TG_GDR=_wtof(field);
				// 				  }
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//---------------------------------------------------------------
	if(TabName=="右沟底坡")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYGouPD=0;
			YGouPD = NULL;
		}
		else
		{
			NYGouPD=Report_input.GetRecordCount();

			if(YGouPD) {delete []YGouPD;YGouPD=NULL;}
			YGouPD=(GouPDdata*)new GouPDdata[NYGouPD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YGouPD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YGouPD[i].SH=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YGouPD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YGouPD[i].EH=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	if(TabName=="右超填宽度表")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYCTKD=0;
			YCTKD = NULL;
		}
		else
		{
			NYCTKD=Report_input.GetRecordCount();

			if(YCTKD) {delete []YCTKD;YCTKD=NULL;}
			YCTKD=(ctkddata*)new ctkddata[NYCTKD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YCTKD[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YCTKD[i].Sctkd=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YCTKD[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YCTKD[i].Ectkd=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右弧形边坡表")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYARCBP=0;
			YARCBP = NULL;
		}
		else
		{
			NYARCBP=Report_input.GetRecordCount();

			if(YARCBP) {delete []YARCBP;YARCBP=NULL;}
			YARCBP=(arcbpata*)new arcbpata[NYARCBP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YARCBP[i].Sdml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YARCBP[i].SR=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YARCBP[i].Edml=_wtof(rcd);
				Report_input.GetValueString(field,5);
				YARCBP[i].ER=_wtof(field);
				Report_input.GetValueString(field,6);
				YARCBP[i].ArcLocation=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右挡墙")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYDQ=1;
			if(YDQ) {delete []YDQ;YDQ=NULL;}
			YDQ=new dqdata[NYDQ];
			YDQ[0].sdml = Sxlc;
			YDQ[0].edml = Exlc;
			YDQ[0].szj = 100.0;
			YDQ[0].ezj = 100.0;	
		}
		else
		{
			NYDQ=Report_input.GetRecordCount();

			if(YDQ) {delete []YDQ;YDQ=NULL;}
			YDQ=(dqdata *)new dqdata[NYDQ];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YDQ[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YDQ[i].szj=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				YDQ[i].edml=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				YDQ[i].ezj=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();

		}	
	}


	if(TabName=="右低填路堤自动填平")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDTP=1;
			if(YLDTP) {delete []YLDTP;YLDTP=NULL;}
			YLDTP=new LDTPData[NYLDTP];
			YLDTP[0].sdml = Sxlc;
			YLDTP[0].edml = Exlc;
		}
		else
		{
			NYLDTP=Report_input.GetRecordCount();

			if(YLDTP) {delete []YLDTP;YLDTP=NULL;}
			YLDTP=(LDTPData *)new LDTPData[NYLDTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLDTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLDTP[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="右低挖路堑自动推平")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQTP=1;
			if(YLQTP) {delete []YLQTP;YLQTP=NULL;}
			YLQTP=new LQTPData[NYLQTP];
			YLQTP[0].sdml = Sxlc;
			YLQTP[0].edml = Exlc;
		}
		else
		{
			NYLQTP=Report_input.GetRecordCount();

			if(YLQTP) {delete []YLQTP;YLQTP=NULL;}
			YLQTP=(LQTPData *)new LQTPData[NYLQTP];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQTP[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLQTP[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="右低填路堤自动加宽土路肩")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYTLJJK=1;
			if(YTLJJK) {delete []YTLJJK;YTLJJK=NULL;}
			YTLJJK=new TLJJKData[NYTLJJK];
			YTLJJK[0].sdml = Sxlc;
			YTLJJK[0].edml = Exlc;
		}
		else
		{
			NYTLJJK=Report_input.GetRecordCount();

			if(YTLJJK) {delete []YTLJJK;YTLJJK=NULL;}
			YTLJJK=(TLJJKData *)new TLJJKData[NYTLJJK];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YTLJJK[i].sdml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YTLJJK[i].edml=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}	

	if(TabName=="右清表土厚度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYHTHD=1;
			if(YHTHD) {delete []YHTHD;YHTHD=NULL;}
			YHTHD=new hthddata[NYHTHD];

			YHTHD[0].dml=Exlc;	
			YHTHD[0].hthd=0.0;
			YHTHD[0].xz=0;
		}
		else
		{
			NYHTHD=Report_input.GetRecordCount();

			if(YHTHD) {delete []YHTHD;YHTHD=NULL;}
			YHTHD=(hthddata*)new hthddata[NYHTHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YHTHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YHTHD[i].hthd=_wtof(field);
				Report_input.GetValueString(field,4);
				YHTHD[i].xz=_wtoi(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	//右侧
	if(TabName=="右路面厚度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLMHD=1;
			if(YLMHD) {delete []YLMHD;YLMHD=NULL;}
			YLMHD=new LMHdata[NYLMHD];
			YLMHD[0].dml = Exlc;
			YLMHD[0].cxdh = 0.5;
			YLMHD[0].yljh = 0.25;
			YLMHD[0].tljh = 0.1;
		}
		else
		{
			NYLMHD=Report_input.GetRecordCount();

			if(YLMHD) {delete []YLMHD;YLMHD=NULL;}
			YLMHD=(LMHdata*)new LMHdata[NYLMHD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLMHD[i].dml=_wtof(rcd);
				Report_input.GetValueString(rcd,3);
				YLMHD[i].cxdh=_wtof(rcd);
				Report_input.GetValueString(rcd,4);
				YLMHD[i].yljh=_wtof(rcd);
				Report_input.GetValueString(rcd,5);
				YLMHD[i].tljh=_wtof(rcd);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}


	//------------------------------------------------------
	if(TabName=="右地面线内插宽")
	{
		//以下数据分右右侧	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYCRS=1;
			if(YCRSWID) {delete []YCRSWID;YCRSWID=NULL;}
			YCRSWID=new crswiddata[NYCRS];

			YCRSWID[0].dml = Exlc;
			YCRSWID[0].wid=100.0;		   
		}
		else
		{
			NYCRS=Report_input.GetRecordCount();

			if(YCRSWID) {delete []YCRSWID;YCRSWID=NULL;}
			YCRSWID=(crswiddata* )new crswiddata[NYCRS];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YCRSWID[i].dml=_wtof(rcd);
				Report_input.GetValueString(field,3);
				YCRSWID[i].wid=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右横断面限制宽度")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYDM=1;
			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=new dmdata[NYDM];

			YDM[0].dml = Exlc;	
			YDM[0].xk=100.0;//限制宽
		}
		else
		{
			NYDM=Report_input.GetRecordCount();

			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=(dmdata* )new dmdata[NYDM];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YDM[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YDM[i].xk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右用地加宽")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYPWD=1;
			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=new pwddata[NYPWD];
			YPWD[0].dml = Exlc;	
			YPWD[0].ltjk=YPWD[0].lqjk=1.0;//外宽
		}
		else
		{
			NYPWD=Report_input.GetRecordCount();

			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=(pwddata* )new pwddata[NYPWD];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YPWD[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YPWD[i].ltjk=_wtof(field);
				Report_input.GetValueString(field,4);
				YPWD[i].lqjk=_wtof(field);
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右路堤边坡")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDBP=1;
			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//动态声请一个空间
			YLDBP[0].dml = Exlc;	

			YLDBP[0].BPJS  = 3;
			YLDBP[0].bpdata[0]  = 1.5;
			YLDBP[0].bpdata[1]  = 8;
			YLDBP[0].bpdata[2]  = 1.0;
			YLDBP[0].bpdata[3]  = 0.0;

			YLDBP[0].bpdata[4]  = 1.75;
			YLDBP[0].bpdata[5]  = 10;
			YLDBP[0].bpdata[6]  = 1.0;
			YLDBP[0].bpdata[7]  = 0.0;

			YLDBP[0].bpdata[8]  = 2.0;
			YLDBP[0].bpdata[9]  = 100;
			YLDBP[0].bpdata[10]  = 1.0;
			YLDBP[0].bpdata[11]  = 0.0;
		}
		else
		{
			NYLDBP=Report_input.GetRecordCount();

			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//动态声请一个空间
			Report_input.MoveFirst();
			k=1;
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLDBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLDBP[i].BPJS=_wtoi(field);
				for( j=0;j<YLDBP[i].BPJS*4;j++)
				{
					Report_input.GetValueString(field,4+j);
					YLDBP[i].bpdata[j]=_wtof(field);


				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	// ZLDBP[i-1].bpdata[(ZLDBP[i-1].BPJS-1)*4+1]=100;

	if(TabName=="右路堑边坡")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQBP=1;
			// if(YLQBP) {delete []YLQBP;YLQBP=NULL;}
			//YLQBP=(LDBPdata* )new LDBPdata[NYLQBP];//动态声请一个空间
			YLQBP[0].dml = Exlc;	

			YLQBP[0].BPJS = 3;
			YLQBP[0].bpdata[0]  = 1;
			YLQBP[0].bpdata[1]  = 8;
			YLQBP[0].bpdata[2]  = 1.0;
			YLQBP[0].bpdata[3] = 0.0;

			YLQBP[0].bpdata[4]  = 1.25;
			YLQBP[0].bpdata[5]  = 10;
			YLQBP[0].bpdata[6]  = 1.0;
			YLQBP[0].bpdata[7]  = 0.0;

			YLQBP[0].bpdata[8] =  1.5;
			YLQBP[0].bpdata[9] =  100;
			YLQBP[0].bpdata[10] =  1.0;
			YLQBP[0].bpdata[11] =  0.0;

		}
		else
		{
			NYLQBP=Report_input.GetRecordCount();
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQBP[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLQBP[i].BPJS=_wtoi(field);
				for( j=0;j<YLQBP[i].BPJS*4;j++)
				{					   
					Report_input.GetValueString(field,4+j);
					YLQBP[i].bpdata[j]=_wtof(field);					   
				}
				Report_input.MoveNext();
				i++;

			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//ZLQBP[i-1].bpdata[(ZLQBP[i-1].BPJS-1)*4+1]=100;

	if(TabName=="右排水沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLDSG=1;
			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//动态声请一个空间
			YLDSG[0].sdml = Sxlc;	
			YLDSG[0].edml = Exlc;	

			YLDSG[0].WallorGou  = 1;//默认的是水沟
			YLDSG[0].S_HPDK=YLDSG[0].E_HPDK=2;//护坡道宽
			YLDSG[0].S_HPDH=YLDSG[0].E_HPDH=0.5;//护坡道宽
			YLDSG[0].S_DSYK=YLDSG[0].E_DSYK=1;//护坡道宽
			YLDSG[0].S_DSYH=YLDSG[0].E_DSYH=0.5;//护坡道宽
			YLDSG[0].S_GK=YLDSG[0].E_GK=0.4;//横坡
			YLDSG[0].S_GS=YLDSG[0].E_GS=0.6;//横坡
			YLDSG[0].nbp  = 1;//内侧边坡
			YLDSG[0].wbp  = 1;//外侧边坡
			YLDSG[0].DSYP  = 1;//外侧边坡
			YLDSG[0].jh   = 0.2;//浆砌厚度
			YLDSG[0].LDSG_GDR = 0.0;//沟底半径
		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NYLDSG=Report_input.GetRecordCount();

			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//动态声请一个空间
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(field,2);
				YLDSG[i].WallorGou=_wtoi(field);
				Report_input.GetValueString(field,3);
				YLDSG[i].jh=_wtof(field);
				Report_input.GetValueString(rcd,4);
				YLDSG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,5);
				YLDSG[i].S_HPDK=_wtof(field);
				Report_input.GetValueString(field,6);
				YLDSG[i].S_HPDH=_wtof(field);
				Report_input.GetValueString(field,7);
				YLDSG[i].S_GS=_wtof(field);
				Report_input.GetValueString(field,8);
				YLDSG[i].S_GK=_wtof(field);
				Report_input.GetValueString(field,9);
				YLDSG[i].S_DSYK=_wtof(field);
				Report_input.GetValueString(field,10);
				YLDSG[i].S_DSYH=_wtof(field);

				Report_input.GetValueString(field,11);
				YLDSG[i].edml=_wtof(field);
				Report_input.GetValueString(field,12);
				YLDSG[i].E_HPDK=_wtof(field);
				Report_input.GetValueString(field,13);
				YLDSG[i].E_HPDH=_wtof(field);
				Report_input.GetValueString(field,14);
				YLDSG[i].E_GS=_wtof(field);
				Report_input.GetValueString(field,15);
				YLDSG[i].E_GK=_wtof(field);
				Report_input.GetValueString(field,16);
				YLDSG[i].E_DSYK=_wtof(field);
				Report_input.GetValueString(field,17);
				YLDSG[i].E_DSYH=_wtof(field);
				Report_input.GetValueString(field,18);
				YLDSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,19);
				YLDSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,20);
				YLDSG[i].DSYP=_wtof(field);
				Report_input.GetValueString(field,21);
				if (field=="NULL")
				{   
					Report_input.PutCollect(21,0.0);
					YLDSG[i].LDSG_GDR=0.0;
				}
				else
					YLDSG[i].LDSG_GDR=_wtof(field);
				// 			   if (IsAddField)
				// 			   {
				// 				   
				// 				   Report_input.PutCollect(21,0.0);
				// 				   YLDSG[i].LDSG_GDR=0.0;
				// 			   }
				// 			   else
				// 			   {
				// 				   Report_input.GetValueString(field,21);
				// 				   YLDSG[i].LDSG_GDR=_wtof(field);
				// 			   }
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}
	if(TabName=="右侧沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYLQSG=1;
			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			YLQSG[0].dml = Exlc;	

			YLQSG[0].gw=0.4;
			YLQSG[0].gs = 0.6;
			YLQSG[0].nbp= 1;
			YLQSG[0].wbp = 1;
			YLQSG[0].Jh=0.3;////浆砌厚度
			YLQSG[0].GDR=0.0;
		}
		else
		{
			NYLQSG=Report_input.GetRecordCount();

			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YLQSG[i].dml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YLQSG[i].gw=_wtof(field);
				Report_input.GetValueString(field,4);
				YLQSG[i].gs=_wtof(field);
				Report_input.GetValueString(field,5);
				YLQSG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,6);
				YLQSG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,7);
				YLQSG[i].Jh=_wtof(field);//浆砌厚度
				Report_input.GetValueString(field,8);
				YLQSG[i].GDR=_wtof(field);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="右天沟")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//数据表里没有数据时则返回
		{
			NYTG=1;
			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			YTG[0].sdml = Sxlc;
			YTG[0].edml = Exlc;	

			YTG[0].sbj=2;//起始边距（坡顶离水沟内侧距离）
			YTG[0].sgw=0.4;//沟宽
			YTG[0].sgs=0.6;//沟深
			YTG[0].ebj=2;//终止边距
			YTG[0].egw=0.4;//沟宽
			YTG[0].egs=0.6;//沟深
			YTG[0].nbp=1;//内侧边坡
			YTG[0].wbp=1;//外侧边坡
			YTG[0].jqhd=0.3;//浆切厚度.
			YTG[0].TG_GDR=0.0;//沟底半径
		}
		else
		{
			bool IsAddField = false;//是否需要插入"沟底半径"列
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "沟底半径")//数据库中没有“沟底半径”列，动态添加该列
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  沟底半径   DOUBLE ",TabName);//插入沟底半径列
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NYTG=Report_input.GetRecordCount();

			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			Report_input.MoveFirst();
			while(!Report_input.IsEOF())
			{
				Report_input.GetValueString(rcd,2);
				YTG[i].sdml=_wtof(rcd);

				Report_input.GetValueString(field,3);
				YTG[i].sbj=_wtof(field);
				Report_input.GetValueString(field,4);
				YTG[i].sgs=_wtof(field);
				Report_input.GetValueString(field,5);
				YTG[i].sgw =_wtof(field);

				Report_input.GetValueString(rcd,6);
				YTG[i].edml=_wtof(rcd);

				Report_input.GetValueString(field,7);
				YTG[i].ebj=_wtof(field);
				Report_input.GetValueString(field,8);//3列后是起始边距
				YTG[i].egs = _wtof(field);
				Report_input.GetValueString(field,9);//3列后是起始边距
				YTG[i].egw = _wtof(field);
				Report_input.GetValueString(field,10);//3列后是起始边距
				YTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3列后是起始边距
				YTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3列后是起始边距
				YTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3列后是起始边距
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3列后是起始边距
					YTG[i].TG_GDR=0.0;
				}
				else
					YTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3列后是起始边距
				// 					  YTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3列后是起始边距
				// 					  YTG[i].TG_GDR=_wtof(field);
				// 				  }

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	return 1;
}


void  HdmDataBase::Write_DmxDb()
{
	CString SQL;
	int id=0;
	if(ConnectMdb())
	{
		int ptnum;
		ptnum=100;
		if(!FindTable("横断面地面线表"))
		{

			CString strSQL;
			strSQL = "create table " ;
			strSQL += "横断面地面线表";
			strSQL += "(";
			strSQL += "ID VARCHAR,";
			strSQL += "中桩里程冠号 VARCHAR,";
			strSQL += "中桩里程数 DOUBLE PRECISION,";
			strSQL += "地面高程 DOUBLE PRECISION,";
			strSQL +=  "左或右 VARCHAR,";
			strSQL += "地面点数 integer,";
			for(int i=1;i<=100;i++)
			{

				strSQL+="点";
				CString str;
				str.Format(L"%d",i);
				strSQL+=str;
				strSQL+="距中桩距离 DOUBLE PRECISION,";
				strSQL+="点";
				strSQL+=str;
				strSQL+="与中桩高程差 DOUBLE PRECISION,";
			}
			strSQL += "附注 VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//创建表
		}

		SQL="delete from 横断面地面线表";
		m_adoConnection.Execute(SQL);
		_variant_t field;

		Report_input.SetAdoConnection(&m_adoConnection);
		SQL="select * from 横断面地面线表";
		Report_input.Open(SQL,adCmdText);
		for(int i=0;i<NDMX;i++)
		{
			CString strText;
			Report_input.AddNew();
			strText.Format(L"%d",id++);
			field = strText;
			Report_input.PutCollect(0,field);//ID
			field = DMX[i].GhK;
			Report_input.PutCollect(1,field);//地面线冠号
			field=DMX[i].rcd;
			Report_input.PutCollect(2,field);//地面线里程
			field=DMX[i].m_dmgc;
			Report_input.PutCollect(3,field);//地面线高程
			field="左";
			Report_input.PutCollect(4,field);
			//field=SingleCrs.NumL;
			Report_input.PutCollect(5,(long)DMX[i].NumL);//地面线左右点数
			if(DMX[i].NumL>0)
			{
				for(int m=0;m<DMX[i].NumL;m++)
				{
					Report_input.PutCollect(6+m*2,DMX[i].Learpt[m*2]);//6、8、10列
					Report_input.PutCollect(7+m*2,DMX[i].Learpt[m*2+1]);//7、9、11列
				}

			}
			Report_input.AddNew();
			strText.Format(L"%d",id++);
			field = strText;
			Report_input.PutCollect(0,field);//ID
			field = DMX[i].GhK;
			Report_input.PutCollect(1,field);//地面线冠号
			field=DMX[i].rcd;
			Report_input.PutCollect(2,field);//地面线里程
			field=DMX[i].m_dmgc;
			Report_input.PutCollect(3,field);//地面线高程
			field="右";
			Report_input.PutCollect(4,field);
			Report_input.PutCollect(5,(long)DMX[i].NumR);//地面线左右点数
			if(DMX[i].NumR>0)
			{

				for(int m=0;m<DMX[i].NumR;m++)
				{
					Report_input.PutCollect(6+m*2,DMX[i].Rearpt[m*2]);
					Report_input.PutCollect(7+m*2,DMX[i].Rearpt[m*2+1]);
				}

			}	

		}
	}

	Report_input.Update();
	Report_input.Close();
	m_adoConnection.Disconnect();

}
bool HdmDataBase::ConnectMdb()
{
	ACHAR mes[300];
	//AfxOleInit();
	try                 
	{	
		// 打开本地Access库
#ifdef X64
		_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", mdbFname);
#else
		_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", mdbFname);
#endif // X64
		
		CString strConnect = mes;

		//strConnect+="数据库.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"连接数据库失败!");
			return false;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=数据库.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"数据库连接失败，确认Pm.mdb是否在当前路径下!");		
	} 	
	return true;
}
bool HdmDataBase::InitTableList(void)
{
	CAdoRecordSet rsetTable;
	_bstr_t Value;
	CString strTablename = "";
	try
	{
		if (m_adoConnection.GetConnection()->State != adStateOpen) 
			return false;
		rsetTable = m_adoConnection.OpenSchema (adSchemaColumns);
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
	catch(_com_error e)
	{
	}
	return TRUE;
}

bool HdmDataBase::FindTable(CString strTempName)
{
	CString str;
	for(int i=0;i<m_strTableNameList.GetSize();i++)
	{
		str=m_strTableNameList.GetAt(i);
		if(str==strTempName)
			return TRUE;
	}	
	return false;
}

//void HdmDataBase::Write_HdmDesresDb()
//{
//
//	CString SQL;
//	int id=0;
//	if(ConnectMdb())
//	{
//		if(!FindTable("横断面几何信息表"))
//		{
//			CString strSQL;
//			strSQL = "create table " ;
//			strSQL += "横断面几何信息表";
//			strSQL += "(";
//			strSQL += "桩号 VARCHAR,";
//			
//			strSQL += "中桩里程冠号 VARCHAR,";
//			strSQL += "中桩里程数 DOUBLE PRECISION,";
//			strSQL += "地面标高 DOUBLE PRECISION,";
//			strSQL += "设计标高 DOUBLE PRECISION,";
//			strSQL += "边侧 VARCHAR,";
//			
//			strSQL += "设计点数 integer,";
//			for(int i=1;i<=SJPT;i++)
//			{
//				CString str;
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="△X DOUBLE PRECISION,";
//				
//				str.Format(L"%d",i);
//				strSQL+=str;
//				strSQL+="△Y DOUBLE PRECISION,";
//			}
//			strSQL +="水沟信息 INTEGER,";
//			strSQL +="水沟点数 INTEGER,";
//			for( i=1;i<=SGPT-1;i++)
//			{
//				strSQL+="沟";
//				CString str;
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="△X DOUBLE PRECISION,";
//				
//				strSQL+="沟";
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="△Y DOUBLE PRECISION,";
//			}
//			strSQL+="沟";
//			CString str;
//			str.Format(L"%d",SGPT);
//			strSQL+=str;
//			strSQL+="△X DOUBLE PRECISION,";
//			
//			strSQL+="沟";
//			str.Format(L"%d",SGPT);
//			strSQL+=str;
//			strSQL+="△Y DOUBLE PRECISION";
//			strSQL += ")";
//			m_adoConnection.Execute(strSQL);//创建表
//		}
//		SQL="delete from 横断面几何信息表";
//		m_adoConnection.Execute(SQL);
//		Report_input.SetAdoConnection(&m_adoConnection);
//		SQL="select * from 横断面几何信息表";
//		Report_input.Open(SQL,adCmdText);
//		
//		for(int i=0;i<NRES;i++)
//		{
//			_variant_t field;
//			int m;
//			CString strText;
//			Report_input.AddNew();//增加一个记录
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Res[i].GH;
//			Report_input.PutCollect(1,field);//冠号
//			
//			field=Res[i].dml;
//			Report_input.PutCollect(2,field);//里程数
//		
//			field=Res[i].dmbg;
//			Report_input.PutCollect(3,field); //地面标高
//			field=Res[i].sjbg;
//			Report_input.PutCollect(4,field); //设计标高
//			field="左";
//			Report_input.PutCollect(5,field); //设计标高
//			
//			field=(long)Res[i].Lnpt;
//			Report_input.PutCollect(6,field); //设计点数
//			
//			//field=SingleCrs.NumL;
//			if(Res[i].Lnpt>0)//设计点坐标
//			{
//				for(m=0;m<Res[i].Lnpt;m++)
//				{
//					Report_input.PutCollect(7+m*2,Res[i].RPTL[m*2]);//7 9 11
//					Report_input.PutCollect(8+m*2,Res[i].RPTL[m*2+1]);//8 10 11 
//				}
//				
//			}
//            field=(long)Res[i].Lsg_inf;//水沟信息 0==水沟 1==天沟
//			Report_input.PutCollect(7+SJPT*2,field);
//			field=(long)Res[i].Lgpt;//左水沟点数
//			Report_input.PutCollect(8+SJPT*2,field);
//			if(Res[i].Lgpt>0)
//			{
//				for(m=0;m<Res[i].Lgpt;m++)//左侧水沟点
//				{
//					Report_input.PutCollect(9+SJPT*2+m*2,Res[i].Lgou[m*2]);
//					Report_input.PutCollect(10+SJPT*2+m*2,Res[i].Lgou[m*2+1]);
//				}
//			}
//			
//			//////////////////////////////////右面
//			Report_input.AddNew();//增加一个记录
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Res[i].GH;
//			Report_input.PutCollect(1,field);//冠号
//			
//			field=Res[i].dml;
//			Report_input.PutCollect(2,field);//里程数
//		
//			field=Res[i].dmbg;
//			Report_input.PutCollect(3,field); //地面标高
//			field=Res[i].sjbg;
//			Report_input.PutCollect(4,field); //设计标高
//			field="右";
//			Report_input.PutCollect(5,field); //边侧
//			
//			field=(long)Res[i].Rnpt;
//			Report_input.PutCollect(6,field); //设计点数
//			
//			
//			//field=SingleCrs.NumL;
//			if(Res[i].Rnpt>0)//设计点坐标
//			{
//				for(m=0;m<Res[i].Rnpt;m++)
//				{
//					Report_input.PutCollect(7+m*2,Res[i].RPTR[m*2]);//7 9 11
//					Report_input.PutCollect(8+m*2,Res[i].RPTR[m*2+1]);//8 10 11 
//				}
//				
//			}
//            field=(long)Res[i].Rsg_inf;//水沟信息
//            //ads_printf(L"%d",Res[i].sg_inf);
//			Report_input.PutCollect(7+SJPT*2,field);
//			field=(long)Res[i].Rgpt;//左水沟点数
//			Report_input.PutCollect(8+SJPT*2,field);
//			if(Res[i].Rgpt>0)
//			{
//				for(m=0;m<Res[i].Rgpt;m++)
//				{
//					Report_input.PutCollect(9+SJPT*2+m*2,Res[i].Rgou[m*2]);
//					Report_input.PutCollect(10+SJPT*2+m*2,Res[i].Rgou[m*2+1]);
//				}
//			}
//	   
//	   }
//    }
//	   Report_input.Update();
//	   Report_input.Close();
//	   m_adoConnection.Disconnect(); 
//}
//void HdmDataBase::Write_HdmAreaDb()
//{
//	CString SQL;
//	int id=0;
//    //int SJPT,SGPT;
//	//SJPT=20;SGPT=10;
//	if(ConnectMdb())
//	{
//		if(!FindTable("横断面数量表"))
//		{
//			CString strSQL;
//			strSQL = "create table " ;
//			strSQL += "横断面数量表";
//			strSQL += "(";
//			strSQL += "桩号 VARCHAR,";
//			
//			strSQL += "中桩里程冠号 VARCHAR,";
//			strSQL += "中桩里程数 DOUBLE PRECISION,";
//			strSQL += "断面性质 integer,";
//			strSQL += "填面积 DOUBLE PRECISION,";
//			strSQL += "改良土面积 DOUBLE PRECISION,";
//			strSQL += "渗水土面积 DOUBLE PRECISION,";
//			strSQL += "清表土面积 DOUBLE PRECISION,";
//			strSQL += "I类挖方面积 DOUBLE PRECISION,";
//			strSQL += "II类挖方面积 DOUBLE PRECISION,";
//			strSQL += "III类挖方面积 DOUBLE PRECISION,";
//			strSQL += "IV类挖方面积 DOUBLE PRECISION,";
//			strSQL += "V类挖方面积 DOUBLE PRECISION,";
//			strSQL += "VI类挖方面积 DOUBLE PRECISION,";
//			strSQL += "左用地宽 DOUBLE PRECISION,";
//			strSQL += "右用地宽 DOUBLE PRECISION,";
//			strSQL += "护肩面积 DOUBLE PRECISION,";
//			strSQL += "水沟浆砌面积 DOUBLE PRECISION,";
//			strSQL += "水沟填面积 DOUBLE PRECISION,";
//			strSQL += "水沟挖面积 DOUBLE PRECISION,";
//			strSQL += "草皮边坡斜长 DOUBLE PRECISION,";
//			strSQL += "挡墙边坡斜长 DOUBLE PRECISION";
//			strSQL += ")";
//			m_adoConnection.Execute(strSQL);//创建表
//		}
//		SQL="delete from 横断面数量表";
//		m_adoConnection.Execute(SQL);
//		Report_input.SetAdoConnection(&m_adoConnection);
//		SQL="select * from 横断面数量表";
//		Report_input.Open(SQL,adCmdText);
//		
//		for(int i=0;i<NArea;i++)
//		{
//			Report_input.AddNew();//增加一个记录
//			_variant_t field;
//			CString strText;
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Area[i].GH;
//			
//			Report_input.PutCollect(1,field);//冠号
//			
//			field=Area[i].dml;
//			Report_input.PutCollect(2,field);//里程数
//			field=(long)Area[i].BTinf; 
//			Report_input.PutCollect(3,field);//断面性质
//			field=Area[i].at; 
//			Report_input.PutCollect(4,field);//填方面积
//			field=Area[i].aglt; 
//			Report_input.PutCollect(5,field);//改良土面积
//			field=Area[i].asst; 
//			Report_input.PutCollect(6,field);//渗水土面积
//			field=Area[i].aqbt; 
//			Report_input.PutCollect(7,field);//清表土面积
//			field=Area[i].aw[0]; 
//			Report_input.PutCollect(8,field);//挖方面积
//			field=Area[i].aw[1]; 
//			Report_input.PutCollect(9,field);//挖方面积
//			field=Area[i].aw[2]; 
//			Report_input.PutCollect(10,field);//挖方面积
//			field=Area[i].aw[3]; 
//			Report_input.PutCollect(11,field);//挖方面积
//			field=Area[i].aw[4]; 
//			Report_input.PutCollect(12,field);//挖方面积
//			field=Area[i].aw[5]; 
//			Report_input.PutCollect(13,field);//挖方面积
//			field=Area[i].LZDK; 
//			Report_input.PutCollect(14,field);//左用地宽
//			field=Area[i].RZDK; 
//			Report_input.PutCollect(15,field);//右用地宽
//			field=Area[i].ahj; 
//			Report_input.PutCollect(16,field);//护肩面积
//			field=Area[i].ajq; 
//			Report_input.PutCollect(17,field);//浆砌面积
//			field=Area[i].asgt; 
//			Report_input.PutCollect(18,field);//水沟填面积
//			field=Area[i].asgw; 
//			Report_input.PutCollect(19,field);// 水沟挖面积
//
//			field=Area[i].cpbpxc; 
//			Report_input.PutCollect(20,field);//草皮边坡斜长
//			field=Area[i].dqbpxc; 
//			Report_input.PutCollect(21,field);//挡墙边坡斜长
//	   }
//	}
//	   
//	   Report_input.Update();
//	   Report_input.Close();
//	   m_adoConnection.Disconnect();
//}
int HdmDataBase::ReadAllRoadName(CString mdbFname)//读取线路数据库中断链表的数据
{
	CString strText,GH;
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
	catch(_com_error e)
	{		
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");		
	} 	

	m_strSQL ="select * from 断链表";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"数据库没有打开或已经关闭!");
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
		CString rname="";
		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rname,"道路名");	
			PutRnameInArray(rname);//将道路名入库
			Report_input.MoveNext();
		}
	} 
	catch(_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();	

	//	ads_printf(L"%s中共%d条道路\n",mdbFname,m_RoadSum);
	for(int i=0; i<m_RoadSum; i++)
	{
		if(Road[i].RoadName != "主线")
			SetRoadSEml(mdbFname,Road[i].RoadName,i);
	}
	SortRoad();	
	return 1;
}

bool HdmDataBase::PutRnameInArray(CString rname)//读取线路数据库中断链表的数据
{
	for(int i=0; i<m_RoadSum; i++)
	{
		if(Road[i].RoadName==rname)//已存在
			return false;
	}
	//不存在
	m_RoadSum++;
	Road[m_RoadSum-1].RoadName=rname;		
	return true;
}

int HdmDataBase::SetRoadSEml(CString mdbFname,CString RoadName,int iRoad)
	//读取线路数据库中的断链数据，输出Road[i].SKml,EKml
{
	CString strText,GH;
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
	catch(_com_error e)
	{		
		AfxMessageBox(L"数据库连接失败，确认数据库是否存在!");		
	} 	

	m_strSQL ="select * from 断链表";
	m_strSQL += L" where 道路名='"+ RoadName;
	m_strSQL += "'";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"数据库没有打开或已经关闭!");
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

		if(DLArray){delete []DLArray;DLArray=NULL;}
		NDL=Report_input.GetRecordCount();
		if(NDL>0)
			DLArray=new DLBZ[NDL];
		int i=0;
		Report_input.MoveFirst();
		while(!Report_input.IsEOF())
		{
			//acedSetStatusBarProgressMeterPos(i/NDL*100);
			Report_input.GetValueString(DLArray[i].BGH,"等号左里程冠号");	
			Report_input.GetValueString(DLArray[i].EGH,"等号右里程冠号");	
			Report_input.GetValueString(strText,"等号左里程数");	
			DLArray[i].BLC=_wtof(strText);			
			Report_input.GetValueString(strText,"等号右里程数");	
			DLArray[i].ELC=_wtof(strText);
			Report_input.MoveNext();
			i++;
		}

	}
	catch(_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();

	if(NDL>0)
	{
		//起点里程
		Road[iRoad].BGH  =  DLArray[0].BGH;
		Road[iRoad].BLC  =  DLArray[0].BLC;

		//终点里程
		Road[iRoad].EGH  =  DLArray[NDL-1].EGH;
		Road[iRoad].ELC  =  DLArray[NDL-1].ELC;

	}	

	return 1;
}

void HdmDataBase::SortRoad()
{
	int i,j;
	struct RoadData tmp;
	//找出主线,保证主线为0
	for(i=0; i<m_RoadSum; i++)
	{
		if(i>0 && Road[i].RoadName=="主线")
		{
			tmp = Road[0]; 
			Road[0] = Road[i];
			Road[i] = tmp;
		}
	}

	for(i=1; i<m_RoadSum; i++)
		for(j=i+1; j<m_RoadSum; j++)
		{
			if(Road[i].BLC > Road[j].BLC)
			{
				tmp = Road[i];
				Road[i] = Road[j];
				Road[j] = tmp;
			}
		}

		//  for(i=0; i<m_RoadSum; i++)
		//		AfxMessageBox(Road[i].RoadName);

}

void HdmDataBase::WriteGouPDB(CString HdmMdbname,CString RoadName,CString bzFrame)
{
	CString SQL,m_SQL;
	int i;	  
	_variant_t var;
	if(!ConnectMdb())
		return;

	//	if(!ConnectMdb())//连接数据库
	//		return ;
	InitTableList();
	if(bzFrame=="左沟底坡")
	{
		if(!FindTable(bzFrame))
		{
			CString m_SQL;
			m_SQL="create table ";

			m_SQL+="左沟底坡 ";
			m_SQL+="(";
			m_SQL+= "序号 INTEGER,";
			m_SQL+="道路名 VARCHAR,";
			m_SQL+="起始里程 double,";
			m_SQL+="起始高程 double precision,";
			m_SQL+="终止里程 double precision,";
			m_SQL+="终止高程 double precision";
			m_SQL+=")";
			m_adoConnection.Execute(m_SQL);
		}
		SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
		SQL += L"'"+RoadName+"'";
		m_adoConnection.Execute(SQL); 

		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
		for(i=0;i< NZGouPD;i++)
		{
			Report_input.AddNew();

			Report_input.PutCollect(0,(long)i);
			var = RoadName;
			Report_input.PutCollect(1, var);
			Report_input.PutCollect(2,ZGouPD[i].Sdml);
			Report_input.PutCollect(3,ZGouPD[i].SH);
			Report_input.PutCollect(4,ZGouPD[i].Edml);
			Report_input.PutCollect(5,ZGouPD[i].EH);
		}
	}
	if(bzFrame=="右沟底坡")
	{
		if(!FindTable(bzFrame))
		{
			CString m_SQL;
			m_SQL="create table ";

			m_SQL+="右沟底坡 ";
			m_SQL+="(";
			m_SQL+= "序号 INTEGER,";
			m_SQL+="道路名 VARCHAR,";
			m_SQL+="起始里程 double,";
			m_SQL+="起始高程 double precision,";
			m_SQL+="终止里程 double precision,";
			m_SQL+="终止高程 double precision";
			m_SQL+=")";
			m_adoConnection.Execute(m_SQL);
		}
		SQL.Format(L"delete from %s where 道路名=",bzFrame);//删除全部
		SQL += L"'"+RoadName+"'";
		m_adoConnection.Execute(SQL); 

		Report_input.SetAdoConnection(&m_adoConnection);
		SQL=L"select * from "+bzFrame;
		Report_input.Open(SQL,adCmdText);
		for(i=0;i< NYGouPD;i++)
		{
			Report_input.AddNew();

			Report_input.PutCollect(0,(long)i);
			var = RoadName;
			Report_input.PutCollect(1, var);
			Report_input.PutCollect(2,YGouPD[i].Sdml);
			Report_input.PutCollect(3,YGouPD[i].SH);
			Report_input.PutCollect(4,YGouPD[i].Edml);
			Report_input.PutCollect(5,YGouPD[i].EH);
		}

	}
	Report_input.Update();
	Report_input.Close();
	m_adoConnection.Disconnect();

}

