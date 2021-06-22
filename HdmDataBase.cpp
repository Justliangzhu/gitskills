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
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//���Ƶ�·��ƻ���ݿ�
HdmDataBase::HdmDataBase()
{

	DLArray=NULL;
	NDL=0;
	NZDM=NYDM=0; //·���޿�
	ZDM=YDM=NULL;

	NZPWD=NYPWD=0;
	ZPWD=YPWD=NULL;//·���������

	NZHTHD=NYHTHD=0;
	ZHTHD=YHTHD=NULL;//��������

	NZCRS=NYCRS=0;
	ZCRSWID=YCRSWID=NULL;
	NZLDBP=NYLDBP=NZLQBP=NYLQBP=0;
	/////////·ǵˮ��
	NZLQSG=NYLQSG=0;
	ZLQSG=YLQSG=NULL;

	NZLDSG=NYLDSG=0;
	ZLDSG=YLDSG=NULL;

	NZTG=NYTG=0;
	ZTG=YTG=NULL;

	///////·������
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

//������·���ʺ����жα��
//m_xlxz-- ��·���� m_rbh--���жα��
//�γ���֮��Ӧ�ĺ�����׼���ݼ�¼��
int HdmDataBase::AttachMdb(CString HdmMdbname,CString RoadName,CString TabName)
{
	ACHAR mes[300];
	try                 
	{	
		// �򿪱���Access��
#ifdef X64
		_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", HdmMdbname);
#else
		_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", HdmMdbname);
#endif // X64
		
		CString strConnect = mes;
		//strConnect+="���ݿ�.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"�������ݿ�ʧ��!");			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ��mdb�Ƿ��ڵ�ǰ·����!");		
	} 

	m_strSQL =L"select * from "+TabName;
	m_strSQL+=L" where ��·��=";
	m_strSQL+= L"'"+RoadName+"' ORDER by ���";
	//	m_strSQL+=" and";
	//	m_strSQL+=" ���жα��=";
	//	m_strSQL+=rbh;

	if (!m_adoConnection.IsOpen())
	{
		AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
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
	//��ȡ��·���ݿ��еĶ������ݣ����Road[i].SKml,EKml
{
	CString strText,GH;
	AfxOleInit();
	try                 
	{	
		// �򿪱���Access��
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="���ݿ�.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");		
	} 	

	m_strSQL ="select * from ������";
	m_strSQL += L" where ��·��='"+ RoadName;
	m_strSQL += "'";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
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
			Report_input.GetValueString(DLArray[i].BGH,L"�Ⱥ�����̹ں�");	
			Report_input.GetValueString(DLArray[i].EGH,L"�Ⱥ�����̹ں�");	
			Report_input.GetValueString(strText,L"�Ⱥ��������");	
			DLArray[i].BLC=_wtof(strText);			
			Report_input.GetValueString(strText,L"�Ⱥ��������");	
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
		//������
		Sxlc = DLArray[0].BLC;
		Exlc = DLArray[NDL-1].ELC;		
	}	

	return 1;
}



/*
int HdmDataBase::Attach_ZYJSMdb(CString TabName)//�����ݿ����Ӳ��������ݿ⣬�γ�һ���ǿռ�¼��
{
if(!ConnectMdb())
return 0;

m_strSQL ="select * from "+TabName;
if (!m_adoConnection.IsOpen())
{
AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
return 0;
}
Report_input.SetAdoConnection(&m_adoConnection);
if(Report_input.Open(m_strSQL, adCmdText)!=1)
return 0; 
Report_input.MoveFirst();
while(!Report_input.IsEOF())
{
CString str;
Report_input.GetValueString(str,"�ٶ�Ŀ��");
m_SDMB=_wtof(str);
Report_input.MoveNext();
}
Report_input.Close();
m_adoConnection.Disconnect();
return 1;
}*/


//ɾ�����м�¼
void HdmDataBase::deleteDataFrmMdb(CString tabName,CString RoadName)

{
	CString SQL;
	SQL="delete * from ";
	SQL+=tabName;
	SQL+=" where ��·��=";
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);
}

//����i�����жεı�׼����д�����ݿ�
void  HdmDataBase::WriteHdmMdb(CString HdmMdbname,CString RoadName,int xh)
{	
	int i=0,j,k=1;
	CString SQL;
	_variant_t var;
	CString  bzFrame;

	bzFrame="�������";

	if(!ConnectMdb())
		return;

	//	if(!ConnectMdb())//�������ݿ�
	//		return ;
	InitTableList();
	if(!FindTable(bzFrame))
	{
		CString SQL;
		SQL="create table ";
		SQL+="����� ";
		SQL+="(";
		SQL += "��� INTEGER,";
		SQL+="��·�� VARCHAR,";
		SQL+="�ֶ������ double precision,";
		SQL+="�������� INTEGER";
		SQL+=")";
		m_adoConnection.Execute(SQL);	
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�õ�������ֶܷα�";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		SQL="create table ";
		SQL+="�õ�������ֶܷα� ";
		SQL+="(";
		SQL+= "��� INTEGER,";
		SQL+="��·�� VARCHAR,";
		SQL+="�ֶ������ double precision";
		SQL+=")";
		m_adoConnection.Execute(SQL);
	}

	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�õ����ͱ�";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�õ����ͱ� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ�������� double,";
		m_SQL+="�õ����� varchar";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�ڷ�����";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�ڷ����� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";

		m_SQL+="I�ٷֱ� DOUBLE,";
		m_SQL+="II�ٷֱ� DOUBLE,";
		m_SQL+="III�ٷֱ�  DOUBLE,";
		m_SQL+="IV�ٷֱ�  DOUBLE,";
		m_SQL+="V�ٷֱ�  DOUBLE,";
		m_SQL+="VI�ٷֱ�  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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
	//����������ֱ���߼乵
	bzFrame="��������·����ֱ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="��������·����ֱ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ���ʼ��� double,";
		m_SQL+="�ֶ���ֹ��� double,";
		m_SQL+="ˮƽ���� DOUBLE,";
		m_SQL+="�߲�  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="��������·ǵ������ƽ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="��������·ǵ������ƽ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ���ʼ��� double,";
		m_SQL+="�ֶ���ֹ��� double,";
		m_SQL+="ˮƽ���� DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="���������߼乵";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���������߼乵 ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ���ʼ��� double,";
		m_SQL+="�ֶ���ֹ��� double,";
		m_SQL+="���� DOUBLE,";
		m_SQL+="���� DOUBLE,";
		m_SQL+="�ڱ���  DOUBLE,";
		m_SQL+="�����  DOUBLE,";
		m_SQL+="�������  DOUBLE,";
		m_SQL+="���װ뾶  DOUBLE";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="��������";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�������� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="�������� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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
	//�������ݷ����Ҳ�	
	bzFrame="��ǽ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="��ǽ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="��ʼ��� double,";
		m_SQL+="��ʼ֧�� double precision,";
		m_SQL+="�յ���� double precision,";
		m_SQL+="��ֹ֧�� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="��·����";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="��·���� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ�������� double,";
		m_SQL+="���е��� double precision,";
		m_SQL+="Ӳ·��� double precision,";
		m_SQL+="��·��� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="��������ڲ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="��������ڲ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="�ڲ�� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="���������ƿ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���������ƿ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="·���޿� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="���õؼӿ�";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���õؼӿ� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="·�̼ӿ� double precision,";
		m_SQL+="·ǵ�ӿ� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="��·�̱���";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="��·�̱��� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double precision,";
		m_SQL+="�����ܼ��� integer,";

		for(i=0;i<29;i++)
		{

			str.Format(L"%d",i+1);
			m_SQL+=str;
			m_SQL+="������ϵ�� double precision,";
			m_SQL+=str;
			m_SQL+="�����߶� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨��� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨���� double precision,";
		}

		str.Format(L"%d",i+1);
		m_SQL+=str;
		m_SQL+="������ϵ�� double precision,";
		m_SQL+=str;
		m_SQL+="�����߶� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨��� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="��·ǵ����";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="��·ǵ���� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double precision,";
		m_SQL+="�����ܼ��� integer,";
		for(i=0;i<29;i++)
		{
			str.Format(L"%d",i+1);
			//m_SQL+=str;
			//m_SQL+="������  integer,";
			m_SQL+=str;
			m_SQL+="������ϵ�� double precision,";
			m_SQL+=str;
			m_SQL+="�����߶� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨��� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨���� double precision,";
		}

		str.Format(L"%d",i+1);
		//m_SQL+=str;
		//m_SQL+="������  integer,";
		m_SQL+=str;
		m_SQL+="������ϵ�� double precision,";
		m_SQL+=str;
		m_SQL+="�����߶� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨��� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="����ˮ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="����ˮ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�½�ǽ integer,";
		m_SQL+="������� double precision,";
		m_SQL+="��ʼ��� double precision,";
		m_SQL+="��ʼ���µ��� double precision,";
		m_SQL+="��ʼ���µ��� double precision,";
		m_SQL+="��ʼˮ���׿� double precision,";
		m_SQL+="��ʼˮ���� double precision,";
		m_SQL+="��ʼ��ˮ�߿� double precision,";
		m_SQL+="��ʼ��ˮ�ߺ� double precision,";
		m_SQL+="��ֹ��� double precision,";
		m_SQL+="��ֹ���µ��� double precision,";
		m_SQL+="��ֹ���µ��� double precision,";
		m_SQL+="��ֹˮ���׿� double precision,";
		m_SQL+="��ֹˮ���� double precision,";
		m_SQL+="��ֹ��ˮ�߿� double precision,";
		m_SQL+="��ֹ��ˮ�ߺ� double precision,";
		m_SQL+="�ڲ���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="��ˮ���� double precision";
		m_SQL+="���װ뾶 double precision";
		m_SQL+=")";

		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
	SQL +=L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	int FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",bzFrame);//���빵�װ뾶��
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


	bzFrame="��๵";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="��๵ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="ˮ���׿� double precision,";
		m_SQL+="���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="�Ҳ���� double precision,";
		m_SQL+="������� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="���칵";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���칵 ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="��ʼ�߾� double precision,";
		m_SQL+="��ʼˮ���� double precision,";
		m_SQL+="��ʼˮ���׿� double precision,";

		m_SQL+="�ֶ������ double,";
		m_SQL+="��ֹ�߾� double precision,";
		m_SQL+="��ֹˮ���� double precision,";
		m_SQL+="��ֹˮ���׿� double precision,";

		m_SQL+="�ڲ���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="������� double precision,";
		m_SQL+="���װ뾶 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}

	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",bzFrame);//���빵�װ뾶��
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
	bzFrame="�ҵ�ǽ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="�ҵ�ǽ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="��ʼ��� double,";
		m_SQL+="��ʼ֧�� double precision,";
		m_SQL+="�յ���� double precision,";
		m_SQL+="��ֹ֧�� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="�ҵ���·���Զ���ƽ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="�ҵ���·���Զ���ƽ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="��ʼ��� double precision,";
		m_SQL+="�յ���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�ҵ���·ǵ�Զ���ƽ";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="�ҵ���·ǵ�Զ���ƽ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="��ʼ��� double precision,";
		m_SQL+="�յ���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�ҵ���·���Զ��ӿ���·��";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";

		m_SQL+="�ҵ���·���Զ��ӿ���·�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="��ʼ��� double precision,";
		m_SQL+="�յ���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="��·����";
	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="��·���� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double precision,";
		m_SQL+="���е��� double precision,";
		m_SQL+="Ӳ·��� double precision,";
		m_SQL+="��·��� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="�ҵ������ڲ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�ҵ������ڲ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="�ڲ�� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="�Һ�������ƿ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�Һ�������ƿ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="·���޿� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="���õؼӿ�";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���õؼӿ� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="·�̼ӿ� double precision,";
		m_SQL+="·ǵ�ӿ� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="��·�̱���";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="��·�̱��� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double precision,";
		m_SQL+="�����ܼ��� integer,";

		for(i=0;i<29;i++)
		{

			str.Format(L"%d",i+1);
			m_SQL+=str;
			m_SQL+="������ϵ�� double precision,";
			m_SQL+=str;
			m_SQL+="�����߶� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨��� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨���� double precision,";
		}

		str.Format(L"%d",i+1);
		m_SQL+=str;
		m_SQL+="������ϵ�� double precision,";
		m_SQL+=str;
		m_SQL+="�����߶� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨��� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="��·ǵ����";

	if(!FindTable(bzFrame))
	{
		CString m_SQL,str;
		m_SQL="create table ";
		m_SQL+="��·ǵ���� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double precision,";
		m_SQL+="�����ܼ��� integer,";
		for(i=0;i<29;i++)
		{
			str.Format(L"%d",i+1);
			//m_SQL+=str;
			//m_SQL+="������  integer,";
			m_SQL+=str;
			m_SQL+="������ϵ�� double precision,";
			m_SQL+=str;
			m_SQL+="�����߶� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨��� double precision,";
			m_SQL+=str;
			m_SQL+="��ƽ̨���� double precision,";
		}

		str.Format(L"%d",i+1);
		//m_SQL+=str;
		//m_SQL+="������  integer,";
		m_SQL+=str;
		m_SQL+="������ϵ�� double precision,";
		m_SQL+=str;
		m_SQL+="�����߶� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨��� double precision,";
		m_SQL+=str;
		m_SQL+="��ƽ̨���� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

	bzFrame="����ˮ��";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="����ˮ�� ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�½�ǽ integer,";
		m_SQL+="������� double precision,";
		m_SQL+="��ʼ��� double precision,";
		m_SQL+="��ʼ���µ��� double precision,";
		m_SQL+="��ʼ���µ��� double precision,";
		m_SQL+="��ʼˮ���׿� double precision,";
		m_SQL+="��ʼˮ���� double precision,";
		m_SQL+="��ʼ��ˮ�߿� double precision,";
		m_SQL+="��ʼ��ˮ�ߺ� double precision,";
		m_SQL+="��ֹ��� double precision,";
		m_SQL+="��ֹ���µ��� double precision,";
		m_SQL+="��ֹ���µ��� double precision,";
		m_SQL+="��ֹˮ���׿� double precision,";
		m_SQL+="��ֹˮ���� double precision,";
		m_SQL+="��ֹ��ˮ�߿� double precision,";
		m_SQL+="��ֹ��ˮ�ߺ� double precision,";
		m_SQL+="�ڲ���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="��ˮ���� double precision";
		m_SQL+="���װ뾶 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",bzFrame);//���빵�װ뾶��
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

	bzFrame="�Ҳ๵";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="�Ҳ๵ ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="ˮ���׿� double precision,";
		m_SQL+="���� double precision,";
		m_SQL+="�ڲ���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="������� double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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


	bzFrame="���칵";

	if(!FindTable(bzFrame))
	{
		CString m_SQL;
		m_SQL="create table ";
		m_SQL+="���칵 ";
		m_SQL+="(";
		m_SQL+= "��� INTEGER identity,";
		m_SQL+="��·�� VARCHAR,";
		m_SQL+="�ֶ������ double,";
		m_SQL+="��ʼ�߾� double precision,";
		m_SQL+="��ʼˮ���� double precision,";
		m_SQL+="��ʼˮ���׿� double precision,";

		m_SQL+="�ֶ������ double,";
		m_SQL+="��ֹ�߾� double precision,";
		m_SQL+="��ֹˮ���� double precision,";
		m_SQL+="��ֹˮ���׿� double precision,";

		m_SQL+="�ڲ���� double precision,";
		m_SQL+="������ double precision,";
		m_SQL+="������� double precision,";
		m_SQL+="���װ뾶 double precision";
		m_SQL+=")";
		m_adoConnection.Execute(m_SQL);
	}
	SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
	SQL += L"'"+RoadName+"'";
	m_adoConnection.Execute(SQL);	 
	Report_input.SetAdoConnection(&m_adoConnection);
	SQL=L"select * from "+bzFrame;
	Report_input.Open(SQL,adCmdText);

	FieldSum  = Report_input.GetFieldsCount();
	Report_input.GetFieldName(FieldSum-1);
	if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
	{
		Report_input.Close();
		CString AddField;
		AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",bzFrame);//���빵�װ뾶��
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
//��ȡ���к��������
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
	TabName="�ɷ�ѹʵϵ��";

	if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
	{
		YSXS[0]=1.23,YSXS[1]=1.16,YSXS[2]=1.09,YSXS[3]=0.92,YSXS[4]=0.92,YSXS[5]=0.92;
	}
	else //�����ݿ�ȡֵ
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

	TabName="�����";

	if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
	{
		NLDTL=1;
		if(LDTL) {delete []LDTL;LDTL=NULL;}
		LDTL=new LDTLdata[NLDTL];
		LDTL[0].dml=Exlc;//////////////���������
		LDTL[0].type = 0;
	}
	else //�����ݿ�ȡֵ
	{
		NLDTL=Report_input.GetRecordCount();

		if(LDTL) {delete []LDTL;LDTL=NULL;}
		LDTL=(LDTLdata *)new LDTLdata[NLDTL];//��̬����һ���ռ�
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
	TabName="�õ�������ֶܷα�";

	if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
	{

		NYD=1;
		if(FDB) {delete []FDB;FDB=NULL;}
		FDB=(YongDiFDB *)new YongDiFDB[NYD];//��̬����һ���ռ�
		FDB[0].dml = Exlc;	
		FDB[0].BZ="";
	}
	else
	{

		NYD=Report_input.GetRecordCount();

		if(FDB) {delete []FDB;FDB=NULL;}
		FDB=(YongDiFDB *)new YongDiFDB[NYD];//��̬����һ���ռ�
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

	TabName="�õ����ͱ�";
	i=0;
	if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
	{
		NYDLX=1;
		if(LXB){delete []LXB;LXB=NULL;}
		LXB=new YongDiLXB[NYDLX];
		LXB[0].dml = Exlc;	
		LXB[0].YDLX="ˮ����";
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

	TabName="�ڷ�����";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			WFFL=(WFFLdata *)new WFFLdata[NWFFL];//��̬����һ���ռ�
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
	TabName="��������·����ֱ";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMLZ=new GLDMLZdata[NGLDMLZ];//��̬����һ���ռ�
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

	TabName="��������·ǵ������ƽ";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];//��̬����һ���ռ�
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



	TabName="���������߼乵";
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];//��̬����һ���ռ�
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

	TabName="����������";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZHTHD=(hthddata*)new hthddata[NZHTHD];//��̬����һ���ռ�
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

	TabName="�󹵵���";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZGouPD=0;
			ZGouPD = NULL;
		}
		else
		{
			NZGouPD=Report_input.GetRecordCount();

			if(ZGouPD) {delete []ZGouPD;ZGouPD=NULL;}
			ZGouPD=(GouPDdata*)new GouPDdata[NZGouPD];//��̬����һ���ռ�
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

	TabName="�����ȱ�";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZCTKD=0;
			ZCTKD = NULL;
		}
		else
		{
			NZCTKD=Report_input.GetRecordCount();

			if(ZCTKD) {delete []ZCTKD;ZCTKD=NULL;}
			ZCTKD=(ctkddata*)new ctkddata[NZCTKD];//��̬����һ���ռ�
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

	TabName="���α��±�";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZARCBP=0;
			ZARCBP = NULL;
		}
		else
		{
			NZARCBP=Report_input.GetRecordCount();

			if(ZARCBP) {delete []ZARCBP;ZARCBP=NULL;}
			ZARCBP=(arcbpata*)new arcbpata[NZARCBP];//��̬����һ���ռ�
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
	//�������ݷ�����	///////////////////////////////////////////////////////////
	TabName="��ǽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZDQ=(dqdata *)new dqdata[NZDQ];//��̬����һ���ռ�
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


	TabName="�����·���Զ���ƽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLDTP=0;
		}
		else
		{
			NZLDTP=Report_input.GetRecordCount();

			if(ZLDTP) {delete []ZLDTP;ZLDTP=NULL;}
			ZLDTP=(LDTPData *)new LDTPData[NZLDTP];//��̬����һ���ռ�
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

	TabName="�����·ǵ�Զ���ƽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLQTP=0;
		}
		else
		{
			NZLQTP=Report_input.GetRecordCount();

			if(ZLQTP) {delete []ZLQTP;ZLQTP=NULL;}
			ZLQTP=(LQTPData *)new LQTPData[NZLQTP];//��̬����һ���ռ�
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

	TabName="�����·���Զ��ӿ���·��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZTLJJK=0;
		}
		else
		{
			NZTLJJK=Report_input.GetRecordCount();

			if(ZTLJJK) {delete []ZTLJJK;ZTLJJK=NULL;}
			ZTLJJK=(TLJJKData *)new TLJJKData[NZTLJJK];//��̬����һ���ռ�
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
	TabName="��·����";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZLMHD=(LMHdata*)new LMHdata[NZLMHD];//��̬����һ���ռ�
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
	TabName="��������ڲ��";
	{
		//�������ݷ����Ҳ�	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZCRSWID=(crswiddata* )new crswiddata[NZCRS];//��̬����һ���ռ�
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

	TabName="���������ƿ��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZDM=1;
			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=new dmdata[NZDM];

			ZDM[0].dml = Exlc;	
			ZDM[0].xk=100.0;//���ƿ�
		}
		else
		{
			NZDM=Report_input.GetRecordCount();

			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=(dmdata* )new dmdata[NZDM];//��̬����һ���ռ�
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
	TabName="���õؼӿ�";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZPWD=1;
			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=new pwddata[NZPWD];
			ZPWD[0].dml = Exlc;	
			ZPWD[0].ltjk=ZPWD[0].lqjk=1.0;//���
		}
		else
		{
			NZPWD=Report_input.GetRecordCount();

			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=(pwddata* )new pwddata[NZPWD];//��̬����һ���ռ�
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
	TabName="��·�̱���";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLDBP=1;
			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//��̬����һ���ռ�
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
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//��̬����һ���ռ�
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
	TabName="��·ǵ����";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLQBP=1;
			// if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//��̬����һ���ռ�
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
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//��̬����һ���ռ�
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
	TabName="����ˮ��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLDSG=0;
			/*
			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//��̬����һ���ռ�
			ZLDSG[0].sdml = Sxlc;	
			ZLDSG[0].edml = Exlc;	

			ZLDSG[0].WallorGou  = 1;//Ĭ�ϵ���ˮ��
			ZLDSG[0].S_HPDK=ZLDSG[0].E_HPDK=2;//���µ���
			ZLDSG[0].S_HPDH=ZLDSG[0].E_HPDH=0.5;//���µ���
			ZLDSG[0].S_DSYK=ZLDSG[0].E_DSYK=1;//���µ���
			ZLDSG[0].S_DSYH=ZLDSG[0].E_DSYH=0.5;//���µ���
			ZLDSG[0].S_GK=ZLDSG[0].E_GK=0.4;//����
			ZLDSG[0].S_GS=ZLDSG[0].E_GS=0.6;//����
			ZLDSG[0].nbp  = 1;//�ڲ����
			ZLDSG[0].wbp  = 1;//������
			ZLDSG[0].DSYP  = 1;//������
			ZLDSG[0].jh   = 0.2;//�������*/

		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NZLDSG=Report_input.GetRecordCount();

			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//��̬����һ���ռ�
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
	TabName="��๵";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{

			NZLQSG=1;
			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			ZLQSG[0].dml = Exlc;	

			ZLQSG[0].gw=0.4;
			ZLQSG[0].gs = 0.6;
			ZLQSG[0].nbp= 1;
			ZLQSG[0].wbp = 1;
			ZLQSG[0].Jh=0.3;////�������
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
				ZLQSG[i].Jh=_wtof(field);//�������
				Report_input.GetValueString(field,8);
				ZLQSG[i].GDR=_wtof(field);//�������

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//	   MyPro->SetPosgress(54);
	//	   MyPro->SetPosgress(56);
	TabName="���칵";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZTG=0;
			/*
			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			ZTG[0].sdml = Sxlc;
			ZTG[0].edml = Exlc;	

			ZTG[0].sbj=2;//��ʼ�߾ࣨ�¶���ˮ���ڲ���룩
			ZTG[0].sgw=0.4;//����
			ZTG[0].sgs=0.6;//����
			ZTG[0].ebj=2;//��ֹ�߾�
			ZTG[0].egw=0.4;//����
			ZTG[0].egs=0.6;//����
			ZTG[0].nbp=1;//�ڲ����
			ZTG[0].wbp=1;//������
			ZTG[0].jqhd=0.3;//���к��*/

		}
		else
		{

			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
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
				Report_input.GetValueString(field,8);//3�к�����ʼ�߾�
				ZTG[i].egs =_wtof(field);
				Report_input.GetValueString(field,9);//3�к�����ʼ�߾�
				ZTG[i].egw=_wtof(field);
				Report_input.GetValueString(field,10);//3�к�����ʼ�߾�
				ZTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3�к�����ʼ�߾�
				ZTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3�к�����ʼ�߾�
				ZTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
					ZTG[i].TG_GDR=0.0;
				}
				else
					ZTG[i].TG_GDR=_wtof(field);

				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
				// 					  ZTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
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
	TabName="�ҵ�ǽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YDQ=(dqdata *)new dqdata[NYDQ];//��̬����һ���ռ�
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

	TabName="�ҵ���·���Զ���ƽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLDTP=0;
		}
		else
		{
			NYLDTP=Report_input.GetRecordCount();

			if(YLDTP) {delete []YLDTP;YLDTP=NULL;}
			YLDTP=(LDTPData *)new LDTPData[NYLDTP];//��̬����һ���ռ�
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

	TabName="�ҵ���·ǵ�Զ���ƽ";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLQTP=0;
		}
		else
		{
			NYLQTP=Report_input.GetRecordCount();

			if(YLQTP) {delete []YLQTP;YLQTP=NULL;}
			YLQTP=(LQTPData *)new LQTPData[NYLQTP];//��̬����һ���ռ�
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

	TabName="�ҵ���·���Զ��ӿ���·��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYTLJJK=0;
		}
		else
		{
			NYTLJJK=Report_input.GetRecordCount();

			if(YTLJJK) {delete []YTLJJK;YTLJJK=NULL;}
			YTLJJK=(TLJJKData *)new TLJJKData[NYTLJJK];//��̬����һ���ռ�
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

	TabName="����������";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YHTHD=(hthddata*)new hthddata[NYHTHD];//��̬����һ���ռ�
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
	TabName="�ҹ�����";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYGouPD=0;
			YGouPD = NULL;
		}
		else
		{
			NYGouPD=Report_input.GetRecordCount();

			if(YGouPD) {delete []YGouPD;YGouPD=NULL;}
			YGouPD=(GouPDdata*)new GouPDdata[NYGouPD];//��̬����һ���ռ�
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
	TabName="�ҳ����ȱ�";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYCTKD=0;
			YCTKD = NULL;
		}
		else
		{
			NYCTKD=Report_input.GetRecordCount();

			if(YCTKD) {delete []YCTKD;YCTKD=NULL;}
			YCTKD=(ctkddata*)new ctkddata[NYCTKD];//��̬����һ���ռ�
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
	TabName="�һ��α��±�";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYARCBP=0;
			YARCBP = NULL;
		}
		else
		{
			NYARCBP=Report_input.GetRecordCount();

			if(YARCBP) {delete []YARCBP;YARCBP=NULL;}
			YARCBP=(arcbpata*)new arcbpata[NYARCBP];//��̬����һ���ռ�
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

	//�Ҳ�
	TabName="��·����";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YLMHD=(LMHdata*)new LMHdata[NYLMHD];//��̬����һ���ռ�
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
	TabName="�ҵ������ڲ��";
	{
		//�������ݷ����Ҳ�	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YCRSWID=(crswiddata* )new crswiddata[NYCRS];//��̬����һ���ռ�
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

	TabName="�Һ�������ƿ��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYDM=1;
			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=new dmdata[NYDM];

			YDM[0].dml = Exlc;	
			YDM[0].xk=100.0;//���ƿ�
		}
		else
		{
			NYDM=Report_input.GetRecordCount();

			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=(dmdata* )new dmdata[NYDM];//��̬����һ���ռ�
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
	TabName="���õؼӿ�";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYPWD=1;
			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=new pwddata[NYPWD];
			YPWD[0].dml = Exlc;	
			YPWD[0].ltjk=YPWD[0].lqjk=1.0;//���
		}
		else
		{
			NYPWD=Report_input.GetRecordCount();

			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=(pwddata* )new pwddata[NYPWD];//��̬����һ���ռ�
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
	TabName="��·�̱���";
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLDBP=1;
			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//��̬����һ���ռ�
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
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//��̬����һ���ռ�
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
	TabName="��·ǵ����";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLQBP=1;
			// if(YLQBP) {delete []YLQBP;YLQBP=NULL;}
			//YLQBP=(LDBPdata* )new LDBPdata[NYLQBP];//��̬����һ���ռ�
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
	TabName="����ˮ��";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLDSG=0;
			/*
			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//��̬����һ���ռ�
			YLDSG[0].sdml = Sxlc;	
			YLDSG[0].edml = Exlc;	

			YLDSG[0].WallorGou  = 1;//Ĭ�ϵ���ˮ��
			YLDSG[0].S_HPDK=YLDSG[0].E_HPDK=2;//���µ���
			YLDSG[0].S_HPDH=YLDSG[0].E_HPDH=0.5;//���µ���
			YLDSG[0].S_DSYK=YLDSG[0].E_DSYK=1;//���µ���
			YLDSG[0].S_DSYH=YLDSG[0].E_DSYH=0.5;//���µ���
			YLDSG[0].S_GK=YLDSG[0].E_GK=0.4;//����
			YLDSG[0].S_GS=YLDSG[0].E_GS=0.6;//����
			YLDSG[0].nbp  = 1;//�ڲ����
			YLDSG[0].wbp  = 1;//������
			YLDSG[0].DSYP  = 1;//������
			YLDSG[0].jh   = 0.2;//�������*/

		}
		else
		{

			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}

			NYLDSG=Report_input.GetRecordCount();

			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//��̬����һ���ռ�
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
	TabName="�Ҳ๵";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLQSG=1;
			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			YLQSG[0].dml = Exlc;	

			YLQSG[0].gw=0.4;
			YLQSG[0].gs = 0.6;
			YLQSG[0].nbp= 1;
			YLQSG[0].wbp = 1;
			YLQSG[0].Jh=0.3;////�������
			YLQSG[0].GDR=0.0;////�������
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
				YLQSG[i].Jh=_wtof(field);//�������
				Report_input.GetValueString(field,8);
				YLQSG[i].GDR=_wtof(field);//�������

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//	   MyPro->SetPosgress(94);
	//	   MyPro->SetPosgress(96);
	TabName="���칵";
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYTG=0;
			/*
			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			YTG[0].sdml = Sxlc;
			YTG[0].edml = Exlc;	

			YTG[0].sbj=2;//��ʼ�߾ࣨ�¶���ˮ���ڲ���룩
			YTG[0].sgw=0.4;//����
			YTG[0].sgs=0.6;//����
			YTG[0].ebj=2;//��ֹ�߾�
			YTG[0].egw=0.4;//����
			YTG[0].egs=0.6;//����
			YTG[0].nbp=1;//�ڲ����
			YTG[0].wbp=1;//������
			YTG[0].jqhd=0.3;//���к��*/

		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
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
				Report_input.GetValueString(field,8);//3�к�����ʼ�߾�
				YTG[i].egs =_wtof(field);
				Report_input.GetValueString(field,9);//3�к�����ʼ�߾�
				YTG[i].egw=_wtof(field);
				Report_input.GetValueString(field,10);//3�к�����ʼ�߾�
				YTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3�к�����ʼ�߾�
				YTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3�к�����ʼ�߾�
				YTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
					YTG[i].TG_GDR=0.0;
				}
				else
					YTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
				// 					  YTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
				// 					  YTG[i].TG_GDR=_wtof(field);
				// 				  }
				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	//20191017	��ȡ·��ֲ�����
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

	if(TabName=="�ɷ�ѹʵϵ��")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			YSXS[0]=1.23,YSXS[1]=1.16,YSXS[2]=1.09,YSXS[3]=0.92,YSXS[4]=0.92,YSXS[5]=0.92;
		}
		else //�����ݿ�ȡֵ
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
	if(TabName=="�����")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NLDTL=1;
			if(LDTL) {delete []LDTL;LDTL=NULL;}
			LDTL=new LDTLdata[NLDTL];
			LDTL[0].dml=Exlc;//////////////���������
			LDTL[0].type = 0;				
		}
		else //�����ݿ�ȡֵ
		{
			NLDTL=Report_input.GetRecordCount();

			if(LDTL) {delete []LDTL;LDTL=NULL;}
			LDTL=(LDTLdata *)new LDTLdata[NLDTL];//��̬����һ���ռ�
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
	if(TabName=="�õ�������ֶܷα�")
	{
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYD=1;
			if(FDB) {delete []FDB;FDB=NULL;}
			FDB=(YongDiFDB *)new YongDiFDB[NYD];//��̬����һ���ռ�
			FDB[0].dml = Exlc;	
			FDB[0].BZ="";
		}
		else
		{

			NYD=Report_input.GetRecordCount();

			if(FDB) {delete []FDB;FDB=NULL;}
			FDB=(YongDiFDB *)new YongDiFDB[NYD];//��̬����һ���ռ�
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

	if(TabName=="�õ����ͱ�")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYDLX=1;
			if(LXB){delete []LXB;LXB=NULL;}
			LXB=new YongDiLXB[NYDLX];
			LXB[0].dml = Exlc;	
			LXB[0].YDLX="ˮ����";
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


	if(TabName=="�ڷ�����")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			WFFL=(WFFLdata *)new WFFLdata[NWFFL];//��̬����һ���ռ�
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
	if (TabName=="��������·����ֱ")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMLZ=new GLDMLZdata[NGLDMLZ];//��̬����һ���ռ�
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


	if (TabName=="��������·ǵ������ƽ")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMLQBPTP=new GLDMLQBPTPdata[NGLDMLQBPTP];//��̬����һ���ռ�
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


	if (TabName=="���������߼乵")
	{		
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			GLDMXJG=new GLDMXJGdata[NGLDMXJG];//��̬����һ���ռ�
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

	if(TabName=="�󹵵���")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZGouPD=0;
			ZGouPD = NULL;
		}
		else
		{
			NZGouPD=Report_input.GetRecordCount();

			if(ZGouPD) {delete []ZGouPD;ZGouPD=NULL;}
			ZGouPD=(GouPDdata*)new GouPDdata[NZGouPD];//��̬����һ���ռ�
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

	if(TabName=="�����ȱ�")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZCTKD=0;
			ZCTKD = NULL;
		}
		else
		{
			NZCTKD=Report_input.GetRecordCount();

			if(ZCTKD) {delete []ZCTKD;ZCTKD=NULL;}
			ZCTKD=(ctkddata*)new ctkddata[NZCTKD];//��̬����һ���ռ�
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
	if(TabName=="���α��±�")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZARCBP=0;
			ZARCBP = NULL;
		}
		else
		{
			NZARCBP=Report_input.GetRecordCount();

			if(ZARCBP) {delete []ZARCBP;ZARCBP=NULL;}
			ZARCBP=(arcbpata*)new arcbpata[NZARCBP];//��̬����һ���ռ�
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

	if(TabName=="��ǽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZDQ=(dqdata *)new dqdata[NZDQ];//��̬����һ���ռ�
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

	if(TabName=="�����·���Զ���ƽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZLDTP=(LDTPData *)new LDTPData[NZLDTP];//��̬����һ���ռ�
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

	if(TabName=="�����·ǵ�Զ���ƽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZLQTP=(LQTPData *)new LQTPData[NZLQTP];//��̬����һ���ռ�
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

	if(TabName=="�����·���Զ��ӿ���·��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZTLJJK=(TLJJKData *)new TLJJKData[NZTLJJK];//��̬����һ���ռ�
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

	if(TabName=="����������")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZHTHD=(hthddata*)new hthddata[NZHTHD];//��̬����һ���ռ�
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

	//�������ݷ�����	///////////////////////////////////////////////////////////
	if(TabName=="��·����")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZLMHD=(LMHdata*)new LMHdata[NZLMHD];//��̬����һ���ռ�
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
	if(TabName=="��������ڲ��")
	{
		//�������ݷ����Ҳ�	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			ZCRSWID=(crswiddata* )new crswiddata[NZCRS];//��̬����һ���ռ�
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

	if(TabName=="���������ƿ��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZDM=1;
			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=new dmdata[NZDM];

			ZDM[0].dml = Exlc;	
			ZDM[0].xk=100.0;//���ƿ�
		}
		else
		{
			NZDM=Report_input.GetRecordCount();

			if(ZDM) {delete []ZDM;ZDM=NULL;}
			ZDM=(dmdata* )new dmdata[NZDM];//��̬����һ���ռ�
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

	if(TabName=="���õؼӿ�")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZPWD=1;
			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=new pwddata[NZPWD];
			ZPWD[0].dml = Exlc;	
			ZPWD[0].ltjk=ZPWD[0].lqjk=1.0;//���
		}
		else
		{
			NZPWD=Report_input.GetRecordCount();

			if(ZPWD) {delete []ZPWD;ZPWD=NULL;}
			ZPWD=(pwddata* )new pwddata[NZPWD];//��̬����һ���ռ�
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

	if(TabName=="��·�̱���")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLDBP=1;
			//if(ZLDBP) {delete []ZLDBP;ZLDBP=NULL;}
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//��̬����һ���ռ�
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
			//ZLDBP=(LDBPdata* )new LDBPdata[NZLDBP];//��̬����һ���ռ�
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

	if(TabName=="��·ǵ����")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLQBP=1;
			// if(ZLQBP) {delete []ZLQBP;ZLQBP=NULL;}
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//��̬����һ���ռ�
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
			//ZLQBP=(LDBPdata* )new LDBPdata[NZLQBP];//��̬����һ���ռ�
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

	if(TabName=="����ˮ��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLDSG=1;
			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//��̬����һ���ռ�
			ZLDSG[0].sdml = Sxlc;	
			ZLDSG[0].edml = Exlc;	

			ZLDSG[0].WallorGou  = 1;//Ĭ�ϵ���ˮ��
			ZLDSG[0].S_HPDK=ZLDSG[0].E_HPDK=2;//���µ���
			ZLDSG[0].S_HPDH=ZLDSG[0].E_HPDH=0.5;//���µ���
			ZLDSG[0].S_DSYK=ZLDSG[0].E_DSYK=1;//���µ���
			ZLDSG[0].S_DSYH=ZLDSG[0].E_DSYH=0.5;//���µ���
			ZLDSG[0].S_GK=ZLDSG[0].E_GK=0.4;//����
			ZLDSG[0].S_GS=ZLDSG[0].E_GS=0.6;//����
			ZLDSG[0].nbp  = 1;//�ڲ����
			ZLDSG[0].wbp  = 1;//������
			ZLDSG[0].DSYP  = 1;//������
			ZLDSG[0].jh   = 0.2;//�������
			ZLDSG[0].LDSG_GDR   = 0.0;//���װ뾶
		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}

			NZLDSG=Report_input.GetRecordCount();

			if(ZLDSG) {delete []ZLDSG;ZLDSG=NULL;}
			ZLDSG=(LDSGdata* )new LDSGdata[NZLDSG];//��̬����һ���ռ�
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


	if(TabName=="��๵")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZLQSG=1;
			if(ZLQSG) {delete []ZLQSG;ZLQSG=NULL;}
			ZLQSG=(LQSGdata* )new LQSGdata[NZLQSG];
			ZLQSG[0].dml = Exlc;	

			ZLQSG[0].gw=0.4;
			ZLQSG[0].gs = 0.6;
			ZLQSG[0].nbp= 1;
			ZLQSG[0].wbp = 1;
			ZLQSG[0].Jh=0.3;////�������
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
				ZLQSG[i].Jh=_wtof(field);//�������
				Report_input.GetValueString(field,8);
				ZLQSG[i].GDR=_wtof(field);//�������

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="���칵")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NZTG=1;
			if(ZTG) {delete []ZTG;ZTG=NULL;}
			ZTG=(TGdata* )new TGdata[NZTG];
			ZTG[0].sdml = Sxlc;
			ZTG[0].edml = Exlc;	

			ZTG[0].sbj=2;//��ʼ�߾ࣨ�¶���ˮ���ڲ���룩
			ZTG[0].sgw=0.4;//����
			ZTG[0].sgs=0.6;//����
			ZTG[0].ebj=2;//��ֹ�߾�
			ZTG[0].egw=0.4;//����
			ZTG[0].egs=0.6;//����
			ZTG[0].nbp=1;//�ڲ����
			ZTG[0].wbp=1;//������
			ZTG[0].jqhd=0.3;//���к��
			ZTG[0].TG_GDR = 0.0;//���װ뾶
		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
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
				Report_input.GetValueString(field,8);//3�к�����ʼ�߾�
				ZTG[i].egs=_wtof(field);
				Report_input.GetValueString(field,9);//3�к�����ʼ�߾�
				ZTG[i].egw =_wtof(field);
				Report_input.GetValueString(field,10);//3�к�����ʼ�߾�
				ZTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3�к�����ʼ�߾�
				ZTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3�к�����ʼ�߾�
				ZTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
					ZTG[i].TG_GDR=0.0;
				}
				else
					ZTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
				// 					  ZTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
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
	if(TabName=="�ҹ�����")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYGouPD=0;
			YGouPD = NULL;
		}
		else
		{
			NYGouPD=Report_input.GetRecordCount();

			if(YGouPD) {delete []YGouPD;YGouPD=NULL;}
			YGouPD=(GouPDdata*)new GouPDdata[NYGouPD];//��̬����һ���ռ�
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
	if(TabName=="�ҳ����ȱ�")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYCTKD=0;
			YCTKD = NULL;
		}
		else
		{
			NYCTKD=Report_input.GetRecordCount();

			if(YCTKD) {delete []YCTKD;YCTKD=NULL;}
			YCTKD=(ctkddata*)new ctkddata[NYCTKD];//��̬����һ���ռ�
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

	if(TabName=="�һ��α��±�")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYARCBP=0;
			YARCBP = NULL;
		}
		else
		{
			NYARCBP=Report_input.GetRecordCount();

			if(YARCBP) {delete []YARCBP;YARCBP=NULL;}
			YARCBP=(arcbpata*)new arcbpata[NYARCBP];//��̬����һ���ռ�
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

	if(TabName=="�ҵ�ǽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YDQ=(dqdata *)new dqdata[NYDQ];//��̬����һ���ռ�
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


	if(TabName=="�ҵ���·���Զ���ƽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YLDTP=(LDTPData *)new LDTPData[NYLDTP];//��̬����һ���ռ�
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

	if(TabName=="�ҵ���·ǵ�Զ���ƽ")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YLQTP=(LQTPData *)new LQTPData[NYLQTP];//��̬����һ���ռ�
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

	if(TabName=="�ҵ���·���Զ��ӿ���·��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YTLJJK=(TLJJKData *)new TLJJKData[NYTLJJK];//��̬����һ���ռ�
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

	if(TabName=="����������")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YHTHD=(hthddata*)new hthddata[NYHTHD];//��̬����һ���ռ�
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
	//�Ҳ�
	if(TabName=="��·����")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YLMHD=(LMHdata*)new LMHdata[NYLMHD];//��̬����һ���ռ�
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
	if(TabName=="�ҵ������ڲ��")
	{
		//�������ݷ����Ҳ�	
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
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
			YCRSWID=(crswiddata* )new crswiddata[NYCRS];//��̬����һ���ռ�
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

	if(TabName=="�Һ�������ƿ��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYDM=1;
			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=new dmdata[NYDM];

			YDM[0].dml = Exlc;	
			YDM[0].xk=100.0;//���ƿ�
		}
		else
		{
			NYDM=Report_input.GetRecordCount();

			if(YDM) {delete []YDM;YDM=NULL;}
			YDM=(dmdata* )new dmdata[NYDM];//��̬����һ���ռ�
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

	if(TabName=="���õؼӿ�")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYPWD=1;
			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=new pwddata[NYPWD];
			YPWD[0].dml = Exlc;	
			YPWD[0].ltjk=YPWD[0].lqjk=1.0;//���
		}
		else
		{
			NYPWD=Report_input.GetRecordCount();

			if(YPWD) {delete []YPWD;YPWD=NULL;}
			YPWD=(pwddata* )new pwddata[NYPWD];//��̬����һ���ռ�
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

	if(TabName=="��·�̱���")
	{  
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLDBP=1;
			//if(YLDBP) {delete []YLDBP;YLDBP=NULL;}
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//��̬����һ���ռ�
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
			//YLDBP=(LDBPdata* )new LDBPdata[NYLDBP];//��̬����һ���ռ�
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

	if(TabName=="��·ǵ����")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLQBP=1;
			// if(YLQBP) {delete []YLQBP;YLQBP=NULL;}
			//YLQBP=(LDBPdata* )new LDBPdata[NYLQBP];//��̬����һ���ռ�
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

	if(TabName=="����ˮ��")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLDSG=1;
			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//��̬����һ���ռ�
			YLDSG[0].sdml = Sxlc;	
			YLDSG[0].edml = Exlc;	

			YLDSG[0].WallorGou  = 1;//Ĭ�ϵ���ˮ��
			YLDSG[0].S_HPDK=YLDSG[0].E_HPDK=2;//���µ���
			YLDSG[0].S_HPDH=YLDSG[0].E_HPDH=0.5;//���µ���
			YLDSG[0].S_DSYK=YLDSG[0].E_DSYK=1;//���µ���
			YLDSG[0].S_DSYH=YLDSG[0].E_DSYH=0.5;//���µ���
			YLDSG[0].S_GK=YLDSG[0].E_GK=0.4;//����
			YLDSG[0].S_GS=YLDSG[0].E_GS=0.6;//����
			YLDSG[0].nbp  = 1;//�ڲ����
			YLDSG[0].wbp  = 1;//������
			YLDSG[0].DSYP  = 1;//������
			YLDSG[0].jh   = 0.2;//�������
			YLDSG[0].LDSG_GDR = 0.0;//���װ뾶
		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
				m_adoConnection.Execute(AddField);
				AttachMdb(mdbFname,roadname,TabName);
			}
			NYLDSG=Report_input.GetRecordCount();

			if(YLDSG) {delete []YLDSG;YLDSG=NULL;}
			YLDSG=(LDSGdata* )new LDSGdata[NYLDSG];//��̬����һ���ռ�
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
	if(TabName=="�Ҳ๵")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYLQSG=1;
			if(YLQSG) {delete []YLQSG;YLQSG=NULL;}
			YLQSG=(LQSGdata* )new LQSGdata[NYLQSG];
			YLQSG[0].dml = Exlc;	

			YLQSG[0].gw=0.4;
			YLQSG[0].gs = 0.6;
			YLQSG[0].nbp= 1;
			YLQSG[0].wbp = 1;
			YLQSG[0].Jh=0.3;////�������
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
				YLQSG[i].Jh=_wtof(field);//�������
				Report_input.GetValueString(field,8);
				YLQSG[i].GDR=_wtof(field);

				Report_input.MoveNext();
				i++;
			}
			Report_input.Close();
			m_adoConnection.Disconnect();
		}
	}

	if(TabName=="���칵")
	{
		i=0;
		if(!AttachMdb(mdbFname,roadname,TabName))//���ݱ���û������ʱ�򷵻�
		{
			NYTG=1;
			if(YTG) {delete []YTG;YTG=NULL;}
			YTG=(TGdata* )new TGdata[NYTG];
			YTG[0].sdml = Sxlc;
			YTG[0].edml = Exlc;	

			YTG[0].sbj=2;//��ʼ�߾ࣨ�¶���ˮ���ڲ���룩
			YTG[0].sgw=0.4;//����
			YTG[0].sgs=0.6;//����
			YTG[0].ebj=2;//��ֹ�߾�
			YTG[0].egw=0.4;//����
			YTG[0].egs=0.6;//����
			YTG[0].nbp=1;//�ڲ����
			YTG[0].wbp=1;//������
			YTG[0].jqhd=0.3;//���к��.
			YTG[0].TG_GDR=0.0;//���װ뾶
		}
		else
		{
			bool IsAddField = false;//�Ƿ���Ҫ����"���װ뾶"��
			int FieldSum  = Report_input.GetFieldsCount();
			if (Report_input.GetFieldName(FieldSum-1) != "���װ뾶")//���ݿ���û�С����װ뾶���У���̬��Ӹ���
			{
				IsAddField = true;
				Report_input.Close();
				CString AddField;
				AddField.Format(L"ALTER   TABLE   %s   ADD  ���װ뾶   DOUBLE ",TabName);//���빵�װ뾶��
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
				Report_input.GetValueString(field,8);//3�к�����ʼ�߾�
				YTG[i].egs = _wtof(field);
				Report_input.GetValueString(field,9);//3�к�����ʼ�߾�
				YTG[i].egw = _wtof(field);
				Report_input.GetValueString(field,10);//3�к�����ʼ�߾�
				YTG[i].nbp=_wtof(field);
				Report_input.GetValueString(field,11);//3�к�����ʼ�߾�
				YTG[i].wbp=_wtof(field);
				Report_input.GetValueString(field,12);//3�к�����ʼ�߾�
				YTG[i].jqhd=_wtof(field);
				Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
				if (field=="NULL")
				{
					Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
					YTG[i].TG_GDR=0.0;
				}
				else
					YTG[i].TG_GDR=_wtof(field);
				// 				  if (IsAddField)
				// 				  {
				// 					  Report_input.PutCollect(13,0.0);//3�к�����ʼ�߾�
				// 					  YTG[i].TG_GDR=0.0;
				// 				  }
				// 				  else
				// 				  {
				// 					  Report_input.GetValueString(field,13);//3�к�����ʼ�߾�
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
		if(!FindTable("���������߱�"))
		{

			CString strSQL;
			strSQL = "create table " ;
			strSQL += "���������߱�";
			strSQL += "(";
			strSQL += "ID VARCHAR,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "����߳� DOUBLE PRECISION,";
			strSQL +=  "����� VARCHAR,";
			strSQL += "������� integer,";
			for(int i=1;i<=100;i++)
			{

				strSQL+="��";
				CString str;
				str.Format(L"%d",i);
				strSQL+=str;
				strSQL+="����׮���� DOUBLE PRECISION,";
				strSQL+="��";
				strSQL+=str;
				strSQL+="����׮�̲߳� DOUBLE PRECISION,";
			}
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		SQL="delete from ���������߱�";
		m_adoConnection.Execute(SQL);
		_variant_t field;

		Report_input.SetAdoConnection(&m_adoConnection);
		SQL="select * from ���������߱�";
		Report_input.Open(SQL,adCmdText);
		for(int i=0;i<NDMX;i++)
		{
			CString strText;
			Report_input.AddNew();
			strText.Format(L"%d",id++);
			field = strText;
			Report_input.PutCollect(0,field);//ID
			field = DMX[i].GhK;
			Report_input.PutCollect(1,field);//�����߹ں�
			field=DMX[i].rcd;
			Report_input.PutCollect(2,field);//���������
			field=DMX[i].m_dmgc;
			Report_input.PutCollect(3,field);//�����߸߳�
			field="��";
			Report_input.PutCollect(4,field);
			//field=SingleCrs.NumL;
			Report_input.PutCollect(5,(long)DMX[i].NumL);//���������ҵ���
			if(DMX[i].NumL>0)
			{
				for(int m=0;m<DMX[i].NumL;m++)
				{
					Report_input.PutCollect(6+m*2,DMX[i].Learpt[m*2]);//6��8��10��
					Report_input.PutCollect(7+m*2,DMX[i].Learpt[m*2+1]);//7��9��11��
				}

			}
			Report_input.AddNew();
			strText.Format(L"%d",id++);
			field = strText;
			Report_input.PutCollect(0,field);//ID
			field = DMX[i].GhK;
			Report_input.PutCollect(1,field);//�����߹ں�
			field=DMX[i].rcd;
			Report_input.PutCollect(2,field);//���������
			field=DMX[i].m_dmgc;
			Report_input.PutCollect(3,field);//�����߸߳�
			field="��";
			Report_input.PutCollect(4,field);
			Report_input.PutCollect(5,(long)DMX[i].NumR);//���������ҵ���
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
		// �򿪱���Access��
#ifdef X64
		_stprintf(mes, L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", mdbFname);
#else
		_stprintf(mes, L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", mdbFname);
#endif // X64
		
		CString strConnect = mes;

		//strConnect+="���ݿ�.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return false;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ��Pm.mdb�Ƿ��ڵ�ǰ·����!");		
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
//		if(!FindTable("����漸����Ϣ��"))
//		{
//			CString strSQL;
//			strSQL = "create table " ;
//			strSQL += "����漸����Ϣ��";
//			strSQL += "(";
//			strSQL += "׮�� VARCHAR,";
//			
//			strSQL += "��׮��̹ں� VARCHAR,";
//			strSQL += "��׮����� DOUBLE PRECISION,";
//			strSQL += "������ DOUBLE PRECISION,";
//			strSQL += "��Ʊ�� DOUBLE PRECISION,";
//			strSQL += "�߲� VARCHAR,";
//			
//			strSQL += "��Ƶ��� integer,";
//			for(int i=1;i<=SJPT;i++)
//			{
//				CString str;
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="��X DOUBLE PRECISION,";
//				
//				str.Format(L"%d",i);
//				strSQL+=str;
//				strSQL+="��Y DOUBLE PRECISION,";
//			}
//			strSQL +="ˮ����Ϣ INTEGER,";
//			strSQL +="ˮ������ INTEGER,";
//			for( i=1;i<=SGPT-1;i++)
//			{
//				strSQL+="��";
//				CString str;
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="��X DOUBLE PRECISION,";
//				
//				strSQL+="��";
//				str.Format(L"%d",i);
//				strSQL+=str;
//				//strSQL+="-";
//				strSQL+="��Y DOUBLE PRECISION,";
//			}
//			strSQL+="��";
//			CString str;
//			str.Format(L"%d",SGPT);
//			strSQL+=str;
//			strSQL+="��X DOUBLE PRECISION,";
//			
//			strSQL+="��";
//			str.Format(L"%d",SGPT);
//			strSQL+=str;
//			strSQL+="��Y DOUBLE PRECISION";
//			strSQL += ")";
//			m_adoConnection.Execute(strSQL);//������
//		}
//		SQL="delete from ����漸����Ϣ��";
//		m_adoConnection.Execute(SQL);
//		Report_input.SetAdoConnection(&m_adoConnection);
//		SQL="select * from ����漸����Ϣ��";
//		Report_input.Open(SQL,adCmdText);
//		
//		for(int i=0;i<NRES;i++)
//		{
//			_variant_t field;
//			int m;
//			CString strText;
//			Report_input.AddNew();//����һ����¼
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Res[i].GH;
//			Report_input.PutCollect(1,field);//�ں�
//			
//			field=Res[i].dml;
//			Report_input.PutCollect(2,field);//�����
//		
//			field=Res[i].dmbg;
//			Report_input.PutCollect(3,field); //������
//			field=Res[i].sjbg;
//			Report_input.PutCollect(4,field); //��Ʊ��
//			field="��";
//			Report_input.PutCollect(5,field); //��Ʊ��
//			
//			field=(long)Res[i].Lnpt;
//			Report_input.PutCollect(6,field); //��Ƶ���
//			
//			//field=SingleCrs.NumL;
//			if(Res[i].Lnpt>0)//��Ƶ�����
//			{
//				for(m=0;m<Res[i].Lnpt;m++)
//				{
//					Report_input.PutCollect(7+m*2,Res[i].RPTL[m*2]);//7 9 11
//					Report_input.PutCollect(8+m*2,Res[i].RPTL[m*2+1]);//8 10 11 
//				}
//				
//			}
//            field=(long)Res[i].Lsg_inf;//ˮ����Ϣ 0==ˮ�� 1==�칵
//			Report_input.PutCollect(7+SJPT*2,field);
//			field=(long)Res[i].Lgpt;//��ˮ������
//			Report_input.PutCollect(8+SJPT*2,field);
//			if(Res[i].Lgpt>0)
//			{
//				for(m=0;m<Res[i].Lgpt;m++)//���ˮ����
//				{
//					Report_input.PutCollect(9+SJPT*2+m*2,Res[i].Lgou[m*2]);
//					Report_input.PutCollect(10+SJPT*2+m*2,Res[i].Lgou[m*2+1]);
//				}
//			}
//			
//			//////////////////////////////////����
//			Report_input.AddNew();//����һ����¼
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Res[i].GH;
//			Report_input.PutCollect(1,field);//�ں�
//			
//			field=Res[i].dml;
//			Report_input.PutCollect(2,field);//�����
//		
//			field=Res[i].dmbg;
//			Report_input.PutCollect(3,field); //������
//			field=Res[i].sjbg;
//			Report_input.PutCollect(4,field); //��Ʊ��
//			field="��";
//			Report_input.PutCollect(5,field); //�߲�
//			
//			field=(long)Res[i].Rnpt;
//			Report_input.PutCollect(6,field); //��Ƶ���
//			
//			
//			//field=SingleCrs.NumL;
//			if(Res[i].Rnpt>0)//��Ƶ�����
//			{
//				for(m=0;m<Res[i].Rnpt;m++)
//				{
//					Report_input.PutCollect(7+m*2,Res[i].RPTR[m*2]);//7 9 11
//					Report_input.PutCollect(8+m*2,Res[i].RPTR[m*2+1]);//8 10 11 
//				}
//				
//			}
//            field=(long)Res[i].Rsg_inf;//ˮ����Ϣ
//            //ads_printf(L"%d",Res[i].sg_inf);
//			Report_input.PutCollect(7+SJPT*2,field);
//			field=(long)Res[i].Rgpt;//��ˮ������
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
//		if(!FindTable("�����������"))
//		{
//			CString strSQL;
//			strSQL = "create table " ;
//			strSQL += "�����������";
//			strSQL += "(";
//			strSQL += "׮�� VARCHAR,";
//			
//			strSQL += "��׮��̹ں� VARCHAR,";
//			strSQL += "��׮����� DOUBLE PRECISION,";
//			strSQL += "�������� integer,";
//			strSQL += "����� DOUBLE PRECISION,";
//			strSQL += "��������� DOUBLE PRECISION,";
//			strSQL += "��ˮ����� DOUBLE PRECISION,";
//			strSQL += "�������� DOUBLE PRECISION,";
//			strSQL += "I���ڷ���� DOUBLE PRECISION,";
//			strSQL += "II���ڷ���� DOUBLE PRECISION,";
//			strSQL += "III���ڷ���� DOUBLE PRECISION,";
//			strSQL += "IV���ڷ���� DOUBLE PRECISION,";
//			strSQL += "V���ڷ���� DOUBLE PRECISION,";
//			strSQL += "VI���ڷ���� DOUBLE PRECISION,";
//			strSQL += "���õؿ� DOUBLE PRECISION,";
//			strSQL += "���õؿ� DOUBLE PRECISION,";
//			strSQL += "������� DOUBLE PRECISION,";
//			strSQL += "ˮ��������� DOUBLE PRECISION,";
//			strSQL += "ˮ������� DOUBLE PRECISION,";
//			strSQL += "ˮ������� DOUBLE PRECISION,";
//			strSQL += "��Ƥ����б�� DOUBLE PRECISION,";
//			strSQL += "��ǽ����б�� DOUBLE PRECISION";
//			strSQL += ")";
//			m_adoConnection.Execute(strSQL);//������
//		}
//		SQL="delete from �����������";
//		m_adoConnection.Execute(SQL);
//		Report_input.SetAdoConnection(&m_adoConnection);
//		SQL="select * from �����������";
//		Report_input.Open(SQL,adCmdText);
//		
//		for(int i=0;i<NArea;i++)
//		{
//			Report_input.AddNew();//����һ����¼
//			_variant_t field;
//			CString strText;
//			strText.Format(L"%d",id++);//ID
//			field = strText;
//			Report_input.PutCollect(0,field);
//			field =Area[i].GH;
//			
//			Report_input.PutCollect(1,field);//�ں�
//			
//			field=Area[i].dml;
//			Report_input.PutCollect(2,field);//�����
//			field=(long)Area[i].BTinf; 
//			Report_input.PutCollect(3,field);//��������
//			field=Area[i].at; 
//			Report_input.PutCollect(4,field);//����
//			field=Area[i].aglt; 
//			Report_input.PutCollect(5,field);//���������
//			field=Area[i].asst; 
//			Report_input.PutCollect(6,field);//��ˮ�����
//			field=Area[i].aqbt; 
//			Report_input.PutCollect(7,field);//��������
//			field=Area[i].aw[0]; 
//			Report_input.PutCollect(8,field);//�ڷ����
//			field=Area[i].aw[1]; 
//			Report_input.PutCollect(9,field);//�ڷ����
//			field=Area[i].aw[2]; 
//			Report_input.PutCollect(10,field);//�ڷ����
//			field=Area[i].aw[3]; 
//			Report_input.PutCollect(11,field);//�ڷ����
//			field=Area[i].aw[4]; 
//			Report_input.PutCollect(12,field);//�ڷ����
//			field=Area[i].aw[5]; 
//			Report_input.PutCollect(13,field);//�ڷ����
//			field=Area[i].LZDK; 
//			Report_input.PutCollect(14,field);//���õؿ�
//			field=Area[i].RZDK; 
//			Report_input.PutCollect(15,field);//���õؿ�
//			field=Area[i].ahj; 
//			Report_input.PutCollect(16,field);//�������
//			field=Area[i].ajq; 
//			Report_input.PutCollect(17,field);//�������
//			field=Area[i].asgt; 
//			Report_input.PutCollect(18,field);//ˮ�������
//			field=Area[i].asgw; 
//			Report_input.PutCollect(19,field);// ˮ�������
//
//			field=Area[i].cpbpxc; 
//			Report_input.PutCollect(20,field);//��Ƥ����б��
//			field=Area[i].dqbpxc; 
//			Report_input.PutCollect(21,field);//��ǽ����б��
//	   }
//	}
//	   
//	   Report_input.Update();
//	   Report_input.Close();
//	   m_adoConnection.Disconnect();
//}
int HdmDataBase::ReadAllRoadName(CString mdbFname)//��ȡ��·���ݿ��ж����������
{
	CString strText,GH;
	AfxOleInit();
	try                 
	{	
		// �򿪱���Access��
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		
		//strConnect+="���ݿ�.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");		
	} 	

	m_strSQL ="select * from ������";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			//            ads_printf(L"û�м�¼!");
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();
		CString rname="";
		while(!Report_input.IsEOF())
		{
			Report_input.GetValueString(rname,"��·��");	
			PutRnameInArray(rname);//����·�����
			Report_input.MoveNext();
		}
	} 
	catch(_com_error e)
	{
	}
	Report_input.Close();
	m_adoConnection.Disconnect();	

	//	ads_printf(L"%s�й�%d����·\n",mdbFname,m_RoadSum);
	for(int i=0; i<m_RoadSum; i++)
	{
		if(Road[i].RoadName != "����")
			SetRoadSEml(mdbFname,Road[i].RoadName,i);
	}
	SortRoad();	
	return 1;
}

bool HdmDataBase::PutRnameInArray(CString rname)//��ȡ��·���ݿ��ж����������
{
	for(int i=0; i<m_RoadSum; i++)
	{
		if(Road[i].RoadName==rname)//�Ѵ���
			return false;
	}
	//������
	m_RoadSum++;
	Road[m_RoadSum-1].RoadName=rname;		
	return true;
}

int HdmDataBase::SetRoadSEml(CString mdbFname,CString RoadName,int iRoad)
	//��ȡ��·���ݿ��еĶ������ݣ����Road[i].SKml,EKml
{
	CString strText,GH;
	AfxOleInit();
	try                 
	{	
		// �򿪱���Access��
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		
		//strConnect+="���ݿ�.mdb";
		m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch(_com_error e)
	{		
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");		
	} 	

	m_strSQL ="select * from ������";
	m_strSQL += L" where ��·��='"+ RoadName;
	m_strSQL += "'";
	int iCols=0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			//            ads_printf(L"û�м�¼!");
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
			Report_input.GetValueString(DLArray[i].BGH,"�Ⱥ�����̹ں�");	
			Report_input.GetValueString(DLArray[i].EGH,"�Ⱥ�����̹ں�");	
			Report_input.GetValueString(strText,"�Ⱥ��������");	
			DLArray[i].BLC=_wtof(strText);			
			Report_input.GetValueString(strText,"�Ⱥ��������");	
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
		//������
		Road[iRoad].BGH  =  DLArray[0].BGH;
		Road[iRoad].BLC  =  DLArray[0].BLC;

		//�յ����
		Road[iRoad].EGH  =  DLArray[NDL-1].EGH;
		Road[iRoad].ELC  =  DLArray[NDL-1].ELC;

	}	

	return 1;
}

void HdmDataBase::SortRoad()
{
	int i,j;
	struct RoadData tmp;
	//�ҳ�����,��֤����Ϊ0
	for(i=0; i<m_RoadSum; i++)
	{
		if(i>0 && Road[i].RoadName=="����")
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

	//	if(!ConnectMdb())//�������ݿ�
	//		return ;
	InitTableList();
	if(bzFrame=="�󹵵���")
	{
		if(!FindTable(bzFrame))
		{
			CString m_SQL;
			m_SQL="create table ";

			m_SQL+="�󹵵��� ";
			m_SQL+="(";
			m_SQL+= "��� INTEGER,";
			m_SQL+="��·�� VARCHAR,";
			m_SQL+="��ʼ��� double,";
			m_SQL+="��ʼ�߳� double precision,";
			m_SQL+="��ֹ��� double precision,";
			m_SQL+="��ֹ�߳� double precision";
			m_SQL+=")";
			m_adoConnection.Execute(m_SQL);
		}
		SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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
	if(bzFrame=="�ҹ�����")
	{
		if(!FindTable(bzFrame))
		{
			CString m_SQL;
			m_SQL="create table ";

			m_SQL+="�ҹ����� ";
			m_SQL+="(";
			m_SQL+= "��� INTEGER,";
			m_SQL+="��·�� VARCHAR,";
			m_SQL+="��ʼ��� double,";
			m_SQL+="��ʼ�߳� double precision,";
			m_SQL+="��ֹ��� double precision,";
			m_SQL+="��ֹ�߳� double precision";
			m_SQL+=")";
			m_adoConnection.Execute(m_SQL);
		}
		SQL.Format(L"delete from %s where ��·��=",bzFrame);//ɾ��ȫ��
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

