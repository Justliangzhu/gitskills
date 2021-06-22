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
//extern CBasicDataOfZDM GlobalBasicDataOfZDM;//���������

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

	//JYBoLiangTable = NULL;  //���������
	//NBLNum = 0;

	////JYJDDataTable = NULL; //�������ݱ�
	////NJDDateNum = 0;

	//JYOrigMeaTable = NULL;   //�������߲���ԭʼ���ϱ�
	//NOrigMeaNum = 0;

	//BZLCTable = NULL;  //��־��̱�
	//BZLCNum=0;

	//GJDAllInforTable = NULL;  //�Ľ��λ��ܱ�
	//GJDAllInforNum =0;

	//ConnectLineTable = NULL;  //�����߱�
	//ConnectLineInforNum =0;

	//SGBXTable = NULL ;
	//SGBXInforNum = 0 ;

	//AllZJDOrCDArray = NULL;
	//AllZJDOrCDNum = 0;

	////�Ľ��ηֶ���Ϣ��
	//pEXGJDFDInf = NULL ; 
	//NEXGJDFDInf = 0;

	////�õ����
	//pXZYD=NULL;
	//NXZYD=0;
	//pYDType=NULL;
	//NTypeFD=0;


	////ˮ׼���
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
	NDL2x = 0;//������
	NQXJK = 0;
	NZuoJiYouXianZhanDiKuan = 0;
	NYouJiYouXianZhanDiKuan = 0;
	NYDLX = 0;//�õ����ͷֶ�����
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
	NXZYD = 0;//�����õطֶ���Ŀ
	NTypeFD = 0;//�õ����ͷֶ���Ŀ


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

	//DMXB=NULL;//�����߱�
	//NDMX=0;

	//QXB = NULL;//���߱�
	//NQX=0;

	//QXB2x=NULL;
	//NQX2x=0;

	//TWB=NULL;//���ڱ�
	//NTW=0;

	//DLB2x=NULL;//���߶�����
	//NDL2x=0;//������

	//JYBoLiangTable = NULL;  //���������
	//NBLNum = 0;

	//JYOrigMeaTable = NULL;  //��������ԭʼ��������
	//NOrigMeaNum = 0;

	//MulFDTable = NULL;  //�����߷ֶ���Ϣ��
	//MulFDNum = 0;

	//ConnectLineTable = NULL; //�����߱�
	//ConnectLineInforNum = 0;

	//SGBXTable = NULL ; 
	//SGBXInforNum = 0 ;

	//AllZJDOrCDArray = NULL;  //����������þ�������
	//AllZJDOrCDNum = 0;

	//pEXGJDFDInf = NULL ; //�Ľ��ηֶ���Ϣ��
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
	NDL2x = 0;//������
	NQXJK = 0;
	NZuoJiYouXianZhanDiKuan = 0;
	NYouJiYouXianZhanDiKuan = 0;
	NYDLX = 0;//�õ����ͷֶ�����
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
	NXZYD = 0;//�����õطֶ���Ŀ
	NTypeFD = 0;//�õ����ͷֶ���Ŀ

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

int XLDataBase_JYX::Read_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//��ȡ��·���ݿ��е�����
{
	CString strEXName;
	CString RXDName;
	if (wlcstring == "")
	{
		m_strSQL = L"select * from " + TabFname;
	}
	else if ((TabFname == "������" || TabFname == "С�ź�" || TabFname == "���" || TabFname == "��վ") && (wlcstring != "��·��=\"���л���\"" && wlcstring != "��·��=\"��ͨ��ƻ���\"" && wlcstring != "��·��=\"��ͨ��Ƣ���\""))
	{
		//�õ��������ݵĲ������ݣ����ڶ��߲������ݵ���·�����������ж������߶���������������ߣ���

		int iStart;
		CString strTemp1 = wlcstring;
		strTemp1.Replace(L"��·��=", NULL);
		RXDName = strTemp1.Mid(1, strTemp1.GetLength() - 2);
		strEXName = RXDInWhichXL(mdbFname, RXDName);

		m_strSQL = L"select * from " + TabFname;
		m_strSQL += " where ";
		m_strSQL += wlcstring;
		if (!strEXName.IsEmpty())
		{
			strTemp1 = L"��·��=\"" + strEXName + "\"";
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

	//m_strSQL += " Order By ���";
	m_strSQL += " Order By ID";
	///////////////////////


	CString strText, GH;
	CAdoRecordSet Report_input;
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
			//ads_printf(L"����%s���ݿ�ʧ��!\n",mdbFname);

			//			AfxMessageBox(L"�������ݿ�ʧ��!");			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}

	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			//if(!_tcscmp(TabFname,"����׮���ߵ��"))
			//{
			//	CString sErr;
			//	sErr.Format(L"���ܴ�%s����Ϊ���ݿ�%s�в���ñ���������ϵͳ��װ����libĿ¼�µı�׼mdb�����ݿ��д����ñ�",TabFname,mdbFname);
			//	AfxMessageBox(sErr);
			//}


			m_adoConnection.Disconnect();
			return 0;

		}

		//////////////////////////////////////////////////////////////////////////
		////С�ź����������Һ��ڱ�����У�������ݿ���û�����������ݳ����Զ����ӡ�
		//if (TabFname == "С�ź�" && Report_input.GetFieldName(7) == "����")//���ݿ��еڰ����Ƿ�Ϊ�����ǡ��ж��Ƿ�Ҫ��������
		//{
		//	Report_input.Close();
		//	CString AddField;
		//	AddField.Format(L"ALTER   TABLE   %s   ADD  �󺭿ڱ��   DOUBLE ",TabFname);//�����󺭿ڱ��
		//	m_adoConnection.Execute(AddField);
		//	AddField.Format(L"ALTER   TABLE   %s   ADD  �Һ��ڱ��   DOUBLE ",TabFname);//�����Һ��ڱ��
		//	m_adoConnection.Execute(AddField);
		//	AddField.Format(L"select ���, ��·��, ������̹ں�, ���������, �׿�ʽ��, ����򳤶�, �Һ��򳤶�, �󺭿ڱ��, �Һ��ڱ��, ����, ��;, ���췽ʽ, �Ľ����� from %s",TabFname);//���¶���˳��
		//	m_adoConnection.Execute(AddField);
		//	Report_input.SetAdoConnection(&m_adoConnection);
		//	Report_input.Open(m_strSQL,adCmdText);
		//}

		//	Report_input.Open(m_strSQL);

		//   Report_input.MoveFirst();

		/////////////////////////////////////////////////////////////////////////////
		////������·�Ľ�   //  ������
		/////////////////////////////////////////////////////////////////////////////
		if (TabFname == "�������ݱ�")
		{  ///������

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

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"����N");
				TempJDarray.N = _wtof(strText);


				Report_input.GetValueString(strText, L"����E");
				TempJDarray.E = _wtof(strText);


				Report_input.GetValueString(strText, L"ǰ����");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"�뾶");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺳�");
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
		else if (TabFname == L"���л��߽������ݱ�")
		{
			JDarrayStruct TempJDarray;
			JDarray.RemoveAll();

			NJD = Report_input.GetRowCount();
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())				//��ÿһ���л�ȡÿ������ĸ�����Ϣ
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"����N");
				TempJDarray.N = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				TempJDarray.E = _wtof(strText);

				Report_input.GetValueString(strText, L"���߰뾶");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"ǰ�������߳�");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺺����߳�");
				TempJDarray.l2 = _wtof(strText);

				JDarray.Add(TempJDarray);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"��ƻ��߽������ݱ�")
		{
			JDarrayStruct TempJDarray;
			JDarray.RemoveAll();

			int i = 0;

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(TempJDarray.GJDNum, strText);

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(TempJDarray.JDNum, strText);

				Report_input.GetValueString(strText, L"����N");
				TempJDarray.N = _wtof(strText);


				Report_input.GetValueString(strText, L"����E");
				TempJDarray.E = _wtof(strText);


				Report_input.GetValueString(strText, L"ǰ�������߳�");
				TempJDarray.l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"���߰뾶");
				TempJDarray.R = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺺����߳�");
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


		else if (TabFname == "��վ")
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
				//if(strText==""||strText=="NULL")//��ʼ���Ϊ�գ�����¼
				//{
				// Report_input.MoveNext();
				// continue;
				//}
				//Report_input.GetValueString(strText,7);				
				//if(strText==""||strText=="NULL")//��ֹ���Ϊ�գ�����¼
				//{
				//   Report_input.MoveNext();
				// continue;
				//}
				Report_input.GetValueString(Sta[i].NameOfWay, L"��·��");
				Report_input.GetValueString(Sta[i].StaName, L"��վ����");
				Report_input.GetValueString(Sta[i].CGH, L"վ��������̹ں�");	//������̹ں�			
				Report_input.GetValueString(strText, L"վ�����������");
				Sta[i].CenDml = _wtof(strText);


				Report_input.GetValueString(Sta[i].BGH, L"�����̹ں�");	//��ʼ��̹ں�	
				Report_input.GetValueString(strText, L"��������");
				Sta[i].StDml = _wtof(strText);
				//
				Report_input.GetValueString(Sta[i].EGH, L"�յ���̹ں�");	//��ֹ��̹ں�			
				Report_input.GetValueString(strText, L"�յ������");	//��ֹ���			
				Sta[i].EdDml = _wtof(strText);

				Report_input.GetValueString(strText, L"��վ����");//��վ����
				Sta[i].CZLX = _wtof(strText);
				Report_input.GetValueString(Sta[i].CQTKM, L"����ͼ����");//����ͼ����
				//				Sta[i].CQTKM=_wtof(strText);
				Report_input.GetValueString(Sta[i].GJXZ, L"�Ľ�����");

				dmltockml(Sta[i].CGH, Sta[i].CenDml, Sta[i].CKml);
				dmltockml(Sta[i].BGH, Sta[i].StDml, Sta[i].SKml);
				dmltockml(Sta[i].EGH, Sta[i].EdDml, Sta[i].EKml);
				i++;
				Report_input.MoveNext();
			}
			StaNum = i;
		}
		else if (TabFname == "�������߲�����������")
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

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(JYBoLiangTable[i].JDNum, strText);

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(JYBoLiangTable[i].XLName, strText);


				Report_input.GetValueString(strText, L"����ƫ��");

				JYBoLiangTable[i].alfa = FromCStringTodouble(strText);


				Report_input.GetValueString(strText, L"���߰뾶");
				JYBoLiangTable[i].R = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"ǰ����");
				JYBoLiangTable[i].l1 = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"�󻺳�");
				JYBoLiangTable[i].l2 = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"ZH���");
				JYBoLiangTable[i].KZH = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"HY���");
				JYBoLiangTable[i].KHY = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"YH���");
				JYBoLiangTable[i].KYH = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"HZ���");
				JYBoLiangTable[i].KHZ = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"�ں�");
				_tcscpy(JYBoLiangTable[i].GH, strText);

				Report_input.GetValueString(strText, L"�Ƿ���ҵʵ��");
				if (strText == "��")
					JYBoLiangTable[i].IsMeasure = true;
				else
					JYBoLiangTable[i].IsMeasure = false;

				Report_input.MoveNext();
			}

		}
		else if (TabFname == "�������߲���ԭʼ���ϱ�")
		{
			int i = 0;

			NOrigMeaNum = Report_input.GetRecordCount();
			if (JYOrigMeaTable) delete[]JYOrigMeaTable;

			if (NOrigMeaNum > 0)
				JYOrigMeaTable = new OriginalMeasureTable[NOrigMeaNum];
			Report_input.MoveFirst();

			for (i = 0; i < NOrigMeaNum; i++)
			{
				Report_input.GetValueString(strText, L"�����");
				_tcscpy(JYOrigMeaTable[i].jdno, strText);

				Report_input.GetValueString(strText, L"�þ���");
				JYOrigMeaTable[i].ZJPoint = _wtoi(strText);

				Report_input.GetValueString(strText, L"������");
				//	JYOrigMeaTable[i].Landmark=_wtof(strText);
				if (strText == "-100000")strText = "";
				_tcscpy(JYOrigMeaTable[i].Landmark, strText);



				Report_input.GetValueString(strText, L"ƫ��");

				//if (strText.IsEmpty())
				//	JYOrigMeaTable[i].fj = -10000 ;
				//else
				//    JYOrigMeaTable[i].fj=_wtof(strText);	
				JYOrigMeaTable[i].fj = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"�ò��");
				JYOrigMeaTable[i].ZhiCeJv = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"�ҽ�");
				JYOrigMeaTable[i].Youjiao = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"����Ej");
				JYOrigMeaTable[i].Ej = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"���Es");

				JYOrigMeaTable[i].Es = FromCStringTodouble(strText);


				Report_input.GetValueString(strText, L"����");

				JYOrigMeaTable[i].deE = FromCStringTodouble(strText);

				// 06.09.20
				Report_input.GetValueString(strText, L"��������ֵ");

				JYOrigMeaTable[i].BLTerritoryPlus = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"��������ֵ");

				JYOrigMeaTable[i].BLTerritoryMinus = FromCStringTodouble(strText);

				Report_input.GetValueString(strText, L"����");
				if (strText != "NULL")
					_tcscpy(JYOrigMeaTable[i].ShuXing, strText);
				else
					_tcscpy(JYOrigMeaTable[i].ShuXing, L" ");

				Report_input.GetValueString(strText, L"��̲�");

				JYOrigMeaTable[i].LCcha = FromCStringTodouble(strText);


				Report_input.MoveNext();

			}
		}
		else if (TabFname == "������")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"�Ⱥ�����̹ں�");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"�Ⱥ�����̹ں�");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"�Ⱥ��������");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"��������");
				DLB[i].DL = _wtof(strText);

				Report_input.GetValueString(strText, L"����N");
				DLB[i].N = _wtof(strText);
				Report_input.GetValueString(strText, L"����E");
				DLB[i].E = _wtof(strText);

				//ͬʱ����Ϻ����
				DLB[i].ELC = DLB[i].BLC + DLB[i].DL;

				Report_input.MoveNext();
				i++;
			}
			NDL = i;

		}
		else if (TabFname == L"���л��߶�����")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"��ǰ�ں�");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"�Ϻ�ں�");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"��ǰ���");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"�Ϻ����");
				DLB[i].ELC = _wtof(strText);
				//Report_input.GetValueString(strText, L"��������");
				DLB[i].DL = DLB[i].ELC - DLB[i].BLC;

				//Report_input.GetValueString(strText, L"����N");
				//DLB[i].N = _wtof(strText);
				//Report_input.GetValueString(strText, L"����E");
				//DLB[i].E = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			NDL = i;
		}
		else if (TabFname == L"��ƻ��߶�����")
		{
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"��ǰ�ں�");
				_tcscpy(DLB[i].BGH, strText);
				Report_input.GetValueString(strText, L"�Ϻ�ں�");
				_tcscpy(DLB[i].EGH, strText);
				Report_input.GetValueString(strText, L"��ǰ���");
				DLB[i].BLC = _wtof(strText);
				Report_input.GetValueString(strText, L"�Ϻ����");
				DLB[i].ELC = _wtof(strText);
				//Report_input.GetValueString(strText, L"��������");
				DLB[i].DL = DLB[i].ELC - DLB[i].BLC;

				//Report_input.GetValueString(strText, L"����N");
				//DLB[i].N = _wtof(strText);
				//Report_input.GetValueString(strText, L"����E");
				//DLB[i].E = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			NDL = i;
		}

		else if (TabFname == "ˮ׼���")
		{
			BmNum = Report_input.GetRecordCount();
			if (BM) delete[]BM;

			if (BmNum > 0)
				BM = new BMSTRUCT[BmNum];
			Report_input.MoveFirst();


			for (int i = 0; i < BmNum; i++)
			{
				Report_input.GetValueString(strText, L"ˮ׼����");
				BM[i].BianHao = strText;
				Report_input.GetValueString(strText, L"�߳�");
				BM[i].GaoCheng = _wtof(strText);

				Report_input.GetValueString(strText, L"��̹ں�");
				BM[i].GH = strText;

				Report_input.GetValueString(strText, L"�����");
				BM[i].XLC = _wtof(strText);


				Report_input.GetValueString(strText, L"�����߾���");
				BM[i].DistZX = _wtof(strText);


				Report_input.GetValueString(strText, L"����N");
				BM[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				BM[i].E = _wtof(strText);


				Report_input.GetValueString(strText, L"λ�ò��ϵ�����");
				BM[i].CLMS = strText;

				Report_input.GetValueString(strText, L"��ע");
				BM[i].FZ = strText;

				Report_input.MoveNext();

			}

		}


		else if (TabFname == "���߱�")
		{
			DXNum = Report_input.GetRecordCount();
			if (DX) delete[]DX;

			if (DXNum > 0)
				DX = new DXSTRUCT[DXNum];
			Report_input.MoveFirst();


			for (int i = 0; i < DXNum; i++)
			{
				Report_input.GetValueString(strText, L"׮��");
				DX[i].ZhuangHao = strText;

				Report_input.GetValueString(strText, L"�ҽ�");
				DX[i].RightAng = _wtof(strText);

				Report_input.GetValueString(strText, L"��λ��");
				DX[i].FWJ = _wtof(strText);

				Report_input.GetValueString(strText, L"����");
				DX[i].Dist = _wtof(strText);

				Report_input.GetValueString(strText, L"γ��");
				DX[i].N = _wtof(strText);

				Report_input.GetValueString(strText, L"����");
				DX[i].E = _wtof(strText);

				Report_input.GetValueString(strText, L"�߳�");
				DX[i].GaoCheng = _wtof(strText);

				Report_input.GetValueString(strText, L"���");
				DX[i].cml = _wtof(strText);

				Report_input.MoveNext();

			}

		}

		/*
		///////////////////////
		else if(TabFname=="���߸Ľ��ֶα�")
		{
		NEXGJDFDInf = Report_input.GetRowCount();
		if(pEXGJDFDInf) delete pEXGJDFDInf;
		if(NEXGJDFDInf>0)
		pEXGJDFDInf = new EXGJDFDInf[NEXGJDFDInf];
		Report_input.MoveFirst();
		int i=0;
		while(!Report_input.IsEOF())
		{
		Report_input.GetValueString(strText,L"���N");
		pEXGJDFDInf[i].spt[0] = _wtof(strText);
		Report_input.GetValueString(strText,L"���E");
		pEXGJDFDInf[i].spt[1] = _wtof(strText);

		Report_input.GetValueString(strText,L"�յ�N");
		pEXGJDFDInf[i].ept[0] = _wtof(strText);
		Report_input.GetValueString(strText,L"�յ�E");
		pEXGJDFDInf[i].ept[1] = _wtof(strText);
		Report_input.GetValueString(strText,L"������·��");
		_tcscpy(pEXGJDFDInf[i].XLName , strText);
		i++;
		Report_input.MoveNext();
		}
		}
		*/
		else if (TabFname == "�������߱�")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(QXB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"��������");
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"���߽����");
				//	QXB2x[i].jdno=int(_wtof(strText)+0.001);
				_tcscpy(QXB2x[i].JDNum, strText);
				QXB2x[i].jdno = i;

				//	ads_printf(L"S1  QXB2x[i].JDNum==%s\n ",QXB2x[i].JDNum);

				Report_input.GetValueString(strText, L"һ�߽����");
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"�߼��");
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, L"����N");
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"���߰뾶");
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"ǰ�������߳�");
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺺����߳�");
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == L"������ƶ��߽������ݱ�")
		{
			NQX2x = Report_input.GetRowCount();
			if (QXB2x) delete[]QXB2x;
			if (NQX2x > 0)
				QXB2x = new DoubleLineData[NQX2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(QXB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"��������");
				QXB2x[i].JDXZ = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"�����");
				//	QXB2x[i].jdno=int(_wtof(strText)+0.001);
				_tcscpy(QXB2x[i].JDNum, strText);
				QXB2x[i].jdno = i;

				//	ads_printf(L"S1  QXB2x[i].JDNum==%s\n ",QXB2x[i].JDNum);

				Report_input.GetValueString(strText, L"һ�߽����");
				QXB2x[i].jdno1 = int(_wtof(strText) + 0.001);

				Report_input.GetValueString(strText, L"�߼��");
				QXB2x[i].xjj = _wtof(strText);

				Report_input.GetValueString(strText, L"����N");
				QXB2x[i].x = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				QXB2x[i].y = _wtof(strText);

				Report_input.GetValueString(strText, L"���߰뾶");
				QXB2x[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"ǰ�������߳�");
				QXB2x[i].Lo1 = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺺����߳�");
				QXB2x[i].Lo2 = _wtof(strText);

				i++;
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "���߶�����")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(DLB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"���ж���");
				DLB2x[i].RXDName = strText;

				Report_input.GetValueString(strText, L"��·���");
				DLB2x[i].no = int(_wtof(strText)); // +0.001

				Report_input.GetValueString(DLB2x[i].BGH, L"�Ⱥ�����̹ں�");

				Report_input.GetValueString(strText, L"�Ⱥ��������");
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, L"�Ⱥ�����̹ں�");

				Report_input.GetValueString(strText, L"�Ⱥ��������");
				DLB2x[i].ELC = _wtof(strText);


				//ads_printf(L"�ڡ�XLName=%s,RXDName=%s,no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n",DLB2x[i].XLName,
				//	DLB2x[i].RXDName,DLB2x[i].no,DLB2x[i].BGH,
				//	DLB2x[i].BLC,DLB2x[i].EGH,DLB2x[i].ELC);


				i++;
				Report_input.MoveNext();
			}

		}
		else if (TabFname == L"������ƶ��߶�����")
		{
			NDL2x = Report_input.GetRowCount();
			if (DLB2x) delete[]DLB2x;
			if (NDL2x > 0)
				DLB2x = new EXDLB[NDL2x];
			Report_input.MoveFirst();
			int i = 0;
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(DLB2x[i].XLName, strText);

				Report_input.GetValueString(strText, L"���ж���");
				DLB2x[i].RXDName = strText;

				Report_input.GetValueString(strText, L"���");
				DLB2x[i].no = int(_wtof(strText)); // +0.001

				Report_input.GetValueString(DLB2x[i].BGH, L"��ǰ�ں�");

				Report_input.GetValueString(strText, L"��ǰ���");
				DLB2x[i].BLC = _wtof(strText);

				Report_input.GetValueString(DLB2x[i].EGH, L"�Ϻ�ں�");

				Report_input.GetValueString(strText, L"�Ϻ����");
				DLB2x[i].ELC = _wtof(strText);


				//ads_printf(L"�ڡ�XLName=%s,RXDName=%s,no=%d,BGH=%s,BLC=%lf,EGH=%s,ELC=%lf\n",DLB2x[i].XLName,
				//	DLB2x[i].RXDName,DLB2x[i].no,DLB2x[i].BGH,
				//	DLB2x[i].BLC,DLB2x[i].EGH,DLB2x[i].ELC);


				i++;
				Report_input.MoveNext();
			}
		}

		////////////////////////////////
		else if (TabFname == "��־��̱�")
		{
			int i = 0;
			BZLCNum = Report_input.GetRecordCount();

			if (BZLCTable) delete[]BZLCTable;

			if (BZLCNum > 0)
				BZLCTable = new BZLCStructTable[BZLCNum];
			Report_input.MoveFirst();

			for (i = 0; i < BZLCNum; i++)
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(BZLCTable[i].GJDNum, strText);

				Report_input.GetValueString(strText, L"��־���");

				_tcscpy(BZLCTable[i].kml, strText);

				Report_input.GetValueString(strText, L"X����");
				BZLCTable[i].X = _wtof(strText);

				Report_input.GetValueString(strText, L"Y����");
				BZLCTable[i].Y = _wtof(strText);


				Report_input.MoveNext();

			}
		}
		else if (TabFname == L"���л��߱�־��̱�")
		{
			int i = 0;
			BZLCNum = Report_input.GetRecordCount();

			if (BZLCTable) delete[]BZLCTable;

			if (BZLCNum > 0)
				BZLCTable = new BZLCStructTable[BZLCNum];
			Report_input.MoveFirst();

			for (i = 0; i < BZLCNum; i++)
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(BZLCTable[i].GJDNum, strText);

				Report_input.GetValueString(strText, L"��־���");

				_tcscpy(BZLCTable[i].kml, strText);

				Report_input.GetValueString(strText, L"����N");
				BZLCTable[i].X = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				BZLCTable[i].Y = _wtof(strText);

				Report_input.MoveNext();

			}
		}

		/////////////////////////////////
		else if (TabFname == "�����߷ֶα�")
		{
			int i = 0;

			MulFDNum = Report_input.GetRecordCount();
			if (MulFDTable) delete[]MulFDTable;

			if (MulFDNum > 0)
				MulFDTable = new BAS_DRAW_FUN_JYX::MulFDData[MulFDNum];

			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(MulFDTable[i].jdno, strText);

				Report_input.GetValueString(strText, L"HY���");
				MulFDTable[i].KHY = _wtof(strText);

				Report_input.GetValueString(strText, L"YH���");
				MulFDTable[i].KYH = _wtof(strText);

				Report_input.MoveNext();
				i++;
			}
			MulFDNum = i;
		}
		if (TabFname == "�Ľ��λ��ܱ�")
		{
			int i = 0;
			GJDAllInforNum = Report_input.GetRecordCount();

			if (GJDAllInforTable) delete[]GJDAllInforTable;

			if (GJDAllInforNum > 0)
				GJDAllInforTable = new GJDAllInforStruct[GJDAllInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < GJDAllInforNum; i++)
			{

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(GJDAllInforTable[i].ToOutSGJJDNum, strText);

				Report_input.GetValueString(strText, L"�Ľ���");
				_tcscpy(GJDAllInforTable[i].ToInSGJJDNum, strText);


				Report_input.GetValueString(strText, L"���N����");
				GJDAllInforTable[i].BpointToN = _wtof(strText);

				Report_input.GetValueString(strText, L"���E����");
				GJDAllInforTable[i].BpointToE = _wtof(strText);

				Report_input.GetValueString(strText, L"�յ�N����");
				GJDAllInforTable[i].EpointToN = _wtof(strText);

				Report_input.GetValueString(strText, L"�յ�E����");
				GJDAllInforTable[i].EpointToE = _wtof(strText);

				Report_input.GetValueString(strText, L"��ʼ��·��");
				_tcscpy(GJDAllInforTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"��ֹ��·��");
				_tcscpy(GJDAllInforTable[i].EXLName, strText);

				Report_input.GetValueString(strText, L"�Ľ�����");
				_tcscpy(GJDAllInforTable[i].GJXZ, strText);

				if (strText == "���иĽ�")
					GJDAllInforTable[i].GJDType = 0;
				else
					GJDAllInforTable[i].GJDType = 2;

				Report_input.GetValueString(strText, L"������");
				_tcscpy(GJDAllInforTable[i].GJDBLC, strText);

				Report_input.GetValueString(strText, L"�յ����");
				_tcscpy(GJDAllInforTable[i].GJDELC, strText);

				//if(strText == "���иĽ�") GJDAllInforTable[i].GJDType = 0;
				//else if( strText == "���иĽ�") GJDAllInforTable[i].GJDType = 2;
				//else if( strText == "���иĽ�") GJDAllInforTable[i].GJDType = 2;

				Report_input.MoveNext();
			}
		}
		if (TabFname == "������Ϣ��")
		{
			int i = 0;
			ConnectLineInforNum = Report_input.GetRecordCount();

			if (ConnectLineTable) delete[]ConnectLineTable;

			if (ConnectLineInforNum > 0)
				ConnectLineTable = new ConnectLineTableStruct[ConnectLineInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < ConnectLineInforNum; i++)
			{

				Report_input.GetValueString(strText, L"��������·��");
				_tcscpy(ConnectLineTable[i].ToOutSGJJDNum, strText);

				Report_input.GetValueString(strText, L"�ڲ���������·��");
				_tcscpy(ConnectLineTable[i].ToInSGJJDNum, strText);


				Report_input.GetValueString(strText, L"��ʼ��·��");
				_tcscpy(ConnectLineTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"��ֹ��·��");
				_tcscpy(ConnectLineTable[i].EXLName, strText);

				Report_input.MoveNext();
			}
		}

		if (TabFname == "ʩ��������Ϣ��")
		{
			int i = 0;
			SGBXInforNum = Report_input.GetRecordCount();

			if (SGBXTable) delete[]SGBXTable;

			if (SGBXInforNum > 0)
				SGBXTable = new SGBXTableStruct[SGBXInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < SGBXInforNum; i++)
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(SGBXTable[i].SGBXName, strText);


				Report_input.GetValueString(strText, L"�ڲ���·��");
				_tcscpy(SGBXTable[i].ToInSGBXName, strText);


				Report_input.GetValueString(strText, L"��ʼ��·��");
				_tcscpy(SGBXTable[i].BXLName, strText);

				Report_input.GetValueString(strText, L"��ֹ��·��");
				_tcscpy(SGBXTable[i].EXLName, strText);

				Report_input.MoveNext();
			}
		}
		if (TabFname == "�ֲ�������Ϣ��")
		{
			int i = 0;
			JBJXInforNum = Report_input.GetRecordCount();

			if (JBJXTable) delete[]JBJXTable;

			if (JBJXInforNum > 0)
				JBJXTable = new JBJXTableStruct[JBJXInforNum];
			Report_input.MoveFirst();

			for (i = 0; i < JBJXInforNum; i++)
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(JBJXTable[i].JBJXName, strText);
				Report_input.MoveNext();
			}
		}
		else if (TabFname == "������þ��������������")
		{   //

			int i = 0;
			AllZJDOrCDNum = Report_input.GetRecordCount();

			if (AllZJDOrCDArray) delete[]AllZJDOrCDArray;

			if (AllZJDOrCDNum > 0)
				AllZJDOrCDArray = new SZJDOrCD[AllZJDOrCDNum];
			Report_input.MoveFirst();

			for (i = 0; i < AllZJDOrCDNum; i++)
			{

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(AllZJDOrCDArray[i].JDNum, strText);

				Report_input.GetValueString(strText, L"�þ���");
				AllZJDOrCDArray[i].iZJDOrCD = _wtoi(strText);

				Report_input.GetValueString(strText, L"������");
				AllZJDOrCDArray[i].dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"�ò��");
				AllZJDOrCDArray[i].dLZJDtoCD = _wtof(strText);

				Report_input.GetValueString(strText, L"�ҽ�");
				AllZJDOrCDArray[i].dYoujiao = _wtof(strText);

				Report_input.MoveNext();

			}

		}

		else if (TabFname == "����׮���ߵ��")
		{   //

			int i = 0;
			m_iNWYZ = Report_input.GetRecordCount();

			if (m_pWYZArr) delete[]m_pWYZArr;

			if (m_iNWYZ > 0)
				m_pWYZArr = new WaiYiZhuang[m_iNWYZ];
			Report_input.MoveFirst();

			for (i = 0; i < m_iNWYZ; i++)
			{

				Report_input.GetValueString(strText, L"�ں�");
				_tcscpy(m_pWYZArr[i].m_cGH, strText);

				Report_input.GetValueString(strText, L"���");
				m_pWYZArr[i].m_dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"����N");
				m_pWYZArr[i].m_dN = _wtof(strText);

				Report_input.GetValueString(strText, L"����E");
				m_pWYZArr[i].m_dE = _wtof(strText);

				Report_input.GetValueString(strText, L"���ƾ�");
				m_pWYZArr[i].m_dOffset = _wtof(strText);

				Report_input.GetValueString(strText, L"����");
				if (strText == "ֱ��ʼ")
					m_pWYZArr[i].m_iZXOrQX = -1;
				else if (strText == "ֱ��ֹ")
					m_pWYZArr[i].m_iZXOrQX = 1;
				else if (strText == "ֱ����")
					m_pWYZArr[i].m_iZXOrQX = 0;
				else if (strText == "ֱ�ߵ�")
					m_pWYZArr[i].m_iZXOrQX = 3;
				else if (strText == "���ߵ�")
					m_pWYZArr[i].m_iZXOrQX = 2;
				else
					m_pWYZArr[i].m_iZXOrQX = 4;

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(m_pWYZArr[i].XLName, strText);



				Report_input.MoveNext();

			}

		}


		else if (TabFname == "����̨�ʱ�")
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

				Report_input.GetValueString(strText, L"�ں�");
				_tcscpy(m_pZMTZArr[i].m_cGH, strText);

				Report_input.GetValueString(strText, L"���");
				m_pZMTZArr[i].m_dLC = _wtof(strText);

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(m_pZMTZArr[i].XLName, strText);

				Report_input.MoveNext();

			}

		}

		else if (TabFname == "����������")
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

				Report_input.GetValueString(strText, L"��ʼ���");
				m_pXZQHArr[i].m_sSml = strText;

				Report_input.GetValueString(strText, L"��ֹ���");
				m_pXZQHArr[i].m_sEml = strText;

				Report_input.GetValueString(strText, L"����");
				m_pXZQHArr[i].m_sName = strText;


				Report_input.MoveNext();

			}

		}





		//////////////////////////////////////////////////////
		//------------------------����----------------------//
		else if (TabFname == "������߽����")
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

				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(SJQXB[i].XLName, strText);

				Report_input.GetValueString(strText, L"�����");
				_tcscpy(SJQXB[i].JDNum, strText);

				Report_input.GetValueString(strText, L"����ƫ��");
				SJQXB[i].a = fabs(_wtof(strText));

				Report_input.GetValueString(strText, L"���߰뾶");
				SJQXB[i].R = _wtof(strText);

				Report_input.GetValueString(strText, L"ǰ����");
				SJQXB[i].l1 = _wtof(strText);

				Report_input.GetValueString(strText, L"�󻺳�");
				SJQXB[i].l2 = _wtof(strText);

				Report_input.GetValueString(strText, L"CKZH���");
				_tcscpy(SJQXB[i].CKZH, strText);

				Report_input.GetValueString(strText, L"CKHY���");
				_tcscpy(SJQXB[i].CKHY, strText);

				Report_input.GetValueString(strText, L"CKYH���");
				_tcscpy(SJQXB[i].CKYH, strText);

				Report_input.GetValueString(strText, L"CKHZ���");
				_tcscpy(SJQXB[i].CKHZ, strText);

				//ads_printf(L"i=%d,HY=%s\n",i,SJQXB[i].CKHY);


				Report_input.MoveNext();
			}
		}


		else if (TabFname == "������")
		{
			int i = 0;
			NDMX = Report_input.GetRecordCount();
			if (DMXB) delete[]DMXB;
			if (NDMX > 0)
				DMXB = new DMXTAB[NDMX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(DMXB[i].GH, L"��׮��̹ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				DMXB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"����߳�");
				DMXB[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NDMX = i;
		}

		else if (TabFname == "����·��̱߳�")
		{
			int i = 0;
			NJiYouLuJianGC = Report_input.GetRecordCount();
			if (JiYouLuJianGC) delete[]JiYouLuJianGC;
			if (NJiYouLuJianGC > 0)
				JiYouLuJianGC = new DMXTAB[NJiYouLuJianGC];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(JiYouLuJianGC[i].GH, "��׮�ں�");
				Report_input.GetValueString(strText, L"��׮���");
				JiYouLuJianGC[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"����·��߳�");
				JiYouLuJianGC[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NJiYouLuJianGC = i;
		}

		else if (TabFname == "�¶ȱ�")
		{
			int i = 0;
			NPD = Report_input.GetRecordCount();
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].NameOfWay, L"��·��");
				Report_input.GetValueString(PDB[i].GH, L"���µ���̹ں�");
				Report_input.GetValueString(strText, L"���µ������");
				PDB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"��Ƹ߳�");
				PDB[i].Level = _tstof(strText);
				Report_input.GetValueString(strText, L"�¶�");
				PDB[i].degree = _tstof(strText);
				Report_input.GetValueString(strText, L"�³�");
				PDB[i].length = _tstof(strText);
				Report_input.GetValueString(strText, L"�����߰뾶");
				PDB[i].Rshu = _tstof(strText);
				Report_input.GetValueString(strText, L"�����߳���");
				PDB[i].RLen = _tstof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"��ע");

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"�����¶ȱ�" || TabFname == L"�����ݶ����¶ȱ�")
		{
			int i = 0;
			NPD = Report_input.GetRecordCount();
			if (PDB) delete[]PDB;
			if (NPD > 0)
				PDB = new PDTAB[NPD];
			Report_input.MoveFirst();

			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(PDB[i].NameOfWay, L"��·��");
				Report_input.GetValueString(PDB[i].GH, L"���µ���̹ں�");
				Report_input.GetValueString(strText, L"���µ������");
				PDB[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"��Ƹ߳�");
				PDB[i].Level = _tstof(strText);
				Report_input.GetValueString(strText, L"�¶�");
				PDB[i].degree = _tstof(strText);
				Report_input.GetValueString(strText, L"�³�");
				PDB[i].length = _tstof(strText);
				Report_input.GetValueString(strText, L"�����߰뾶");
				PDB[i].Rshu = _tstof(strText);
				Report_input.GetValueString(strText, L"�����߳���");
				PDB[i].RLen = _tstof(strText);
				Report_input.GetValueString(PDB[i].Notes, L"��ע");

				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == L"�����ݶ�������")
		{
			int i = 0;
			FZZDMNum = Report_input.GetRecordCount();

			if (FZZDMArray) delete[]FZZDMArray;

			if (FZZDMNum > 0)
				FZZDMArray = new FZZDM[FZZDMNum];
			Report_input.MoveFirst();

			for (i = 0; i < FZZDMNum; i++)
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(FZZDMArray[i].XLName, strText);

				Report_input.GetValueString(strText, L"������");
				_tcscpy(FZZDMArray[i].DLName, strText);

				Report_input.GetValueString(strText, L"��������");
				FZZDMArray[i].Type = strText;

				Report_input.GetValueString(strText, L"���N����");
				FZZDMArray[i].spt[0] = _wtof(strText);

				Report_input.GetValueString(strText, L"���E����");
				FZZDMArray[i].spt[1] = _wtof(strText);

				Report_input.GetValueString(strText, L"�յ�N����");
				FZZDMArray[i].ept[0] = _wtof(strText);

				Report_input.GetValueString(strText, L"�յ�E����");
				FZZDMArray[i].ept[1] = _wtof(strText);

				Report_input.GetValueString(strText, L"������");
				_tcscpy(FZZDMArray[i].StCKml, strText);

				Report_input.GetValueString(strText, L"�յ����");
				_tcscpy(FZZDMArray[i].EdCKml, strText);

				Report_input.GetValueString(strText, L"����ƽ��");
				_tcscpy(FZZDMArray[i].BLName, strText);

				Report_input.MoveNext();
			}
		}


		else if (TabFname == "���й�����")
		{
			int i = 0;
			NGMX = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NGMX > 0)
				GMXB = new GMXTAB[NGMX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(GMXB[i].CAP, L"��׮��̹ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				GMXB[i].ML = _tstof(strText);
				dmltockml(GMXB[i].CAP, GMXB[i].ML, GMXB[i].CKml);
				Report_input.GetValueString(strText, L"���й���߳�");
				GMXB[i].H = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NGMX = i;
		}
		else if (TabFname == "���е�����ȱ�")
		{
			int i = 0;
			RoadBedNum = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (RoadBedNum > 0)
				RoadBed = new RoadBedTAB[RoadBedNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(RoadBed[i].GH, L"��׮�ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				RoadBed[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"�������");
				RoadBed[i].H = _tstof(strText);
				Report_input.GetValueString(strText, L"����ṹ�߶�");
				RoadBed[i].H1 = _tstof(strText);
				dmltockml(RoadBed[i].GH, RoadBed[i].ml, RoadBed[i].CKml);
				Report_input.MoveNext();
				i++;
			}
			RoadBedNum = i;
		}
		else if (TabFname == "������Ƶ��")
		{
			int i = 0;
			ZMControlPTNum = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (ZMControlPTNum > 0)
				ZMControlPT = new RoadBedTAB[ZMControlPTNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(ZMControlPT[i].GH, L"��׮�ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				ZMControlPT[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"���޸߳�");
				ZMControlPT[i].H = _tstof(strText);
				Report_input.GetValueString(strText, L"���޸߳�");
				ZMControlPT[i].H1 = _tstof(strText);
				Report_input.GetValueString(ZMControlPT[i].sRemark, L"��ע");

				Report_input.MoveNext();
				i++;
			}
			ZMControlPTNum = i;
		}
		else if (TabFname == "�������ռ�ؿ�")
		{
			int i = 0;
			NZuoJiYouXianZhanDiKuan = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NZuoJiYouXianZhanDiKuan > 0)
				ZuoJiYouXianZhanDiKuan = new JiYouXianZhanDiKuan[NZuoJiYouXianZhanDiKuan];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(ZuoJiYouXianZhanDiKuan[i].GH, L"��׮�ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				ZuoJiYouXianZhanDiKuan[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"·���߽���");
				ZuoJiYouXianZhanDiKuan[i].LuJiBianJieKuan = _tstof(strText);
				Report_input.GetValueString(strText, L"ռ�ر߽���");
				ZuoJiYouXianZhanDiKuan[i].ZhanDiKuan = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == "�Ҽ�����ռ�ؿ�")
		{
			int i = 0;
			NYouJiYouXianZhanDiKuan = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NYouJiYouXianZhanDiKuan > 0)
				YouJiYouXianZhanDiKuan = new JiYouXianZhanDiKuan[NYouJiYouXianZhanDiKuan];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(YouJiYouXianZhanDiKuan[i].GH, L"��׮�ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				YouJiYouXianZhanDiKuan[i].ml = _tstof(strText);
				Report_input.GetValueString(strText, L"·���߽���");
				YouJiYouXianZhanDiKuan[i].LuJiBianJieKuan = _tstof(strText);
				Report_input.GetValueString(strText, L"ռ�ر߽���");
				YouJiYouXianZhanDiKuan[i].ZhanDiKuan = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == "�õ����ͱ�")
		{
			int i = 0;
			NYDLX = Report_input.GetRecordCount();
			//if(DMXB) delete []DMXB;
			if (NYDLX > 0)
				LXB = new YongDiLXB[NYDLX];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"��·��");
				_tcscpy(LXB[i].XLName, strText);
				Report_input.GetValueString(strText, L"�ֶ�����̹ں�");
				_tcscpy(LXB[i].GH, strText);
				Report_input.GetValueString(strText, L"�ֶ��������");
				LXB[i].dml = _tstof(strText);
				Report_input.GetValueString(strText, L"�õ�����");
				_tcscpy(LXB[i].YDLX, strText);
				Report_input.MoveNext();
				i++;
			}
		}

		else if (TabFname == "���")
		{
			int i = 0;
			TunNum = Report_input.GetRecordCount();
			//			if(Tun) delete []Tun;
			if (TunNum > 0)
				Tun = new TUNTAB[TunNum];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(Tun[i].NameOfWay, L"��·��");

				Report_input.GetValueString(Tun[i].TUNName, L"�������");
				Report_input.GetValueString(Tun[i].BGH, L"�����̹ں�");
				Report_input.GetValueString(strText, L"��������");
				Tun[i].StDml = _tstof(strText);
				Report_input.GetValueString(Tun[i].EGH, L"�յ���̹ں�");
				Report_input.GetValueString(strText, L"�յ������");
				Tun[i].EdDml = _tstof(strText);
				Report_input.GetValueString(Tun[i].flag, L"���췽ʽ");//���췽ʽ����ΰ���
				if (Tun[i].flag == " " || Tun[i].flag == "NULL")//���û��¼Ĭ��Ϊ1-һ�����
				{
					Tun[i].flag.Format(L"%d", 1);
				}
				Report_input.GetValueString(Tun[i].GJZT, L"�Ľ�״̬");//�Ľ�״̬

				Report_input.GetValueString(strText, L"��ע");//��ע�������¼����(1-����2-˫��)
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

		else if (TabFname == "С�ź�" || TabFname == "�������ж�С�ź�" || TabFname == "�������ж�С�ź�")
		{
			int i = 0;
			SBriNum = Report_input.GetRecordCount();
			if (SBri) { delete[]SBri; SBri = NULL; }
			if (SBriNum > 0)
				SBri = new SMALLBRIDGETAB[SBriNum];

			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(SBri[i].NameOfWay, L"��·��");
				Report_input.GetValueString(SBri[i].GH, L"������̹ں�");
				Report_input.GetValueString(strText, L"���������");
				SBri[i].ml = _wtof(strText);

				dmltockml(SBri[i].GH, SBri[i].ml, SBri[i].CKml);

				Report_input.GetValueString(SBri[i].KKStyle, L"�׿�ʽ��");
				Report_input.GetValueString(strText, L"����򳤶�");
				SBri[i].dwL = _wtof(strText);
				Report_input.GetValueString(strText, L"�Һ��򳤶�");
				SBri[i].dwR = _wtof(strText);
				Report_input.GetValueString(strText, L"�󺭿ڱ��");
				SBri[i].LLevel = _wtof(strText);
				Report_input.GetValueString(strText, L"�Һ��ڱ��");
				SBri[i].RLevel = _wtof(strText);
				Report_input.GetValueString(strText, L"����");
				SBri[i].JJiao = _wtof(strText);
				if (SBri[i].JJiao < 0.1)//���û���룬����Ϊ�Ǵ�ֱ
				{
					SBri[i].JJiao = 90;
				}
				Report_input.GetValueString(strText, L"��;");
				SBri[i].ToUse = _wtof(strText);
				Report_input.GetValueString(strText, L"���췽ʽ");
				SBri[i].InsertStyle = _wtof(strText);
				if (SBri[i].InsertStyle < 0 || SBri[i].InsertStyle>7)//����Խ��
				{
					SBri[i].InsertStyle = 0;
				}

				Report_input.GetValueString(SBri[i].GJXZ, L"�Ľ�����");
				if (Report_input.GetValueString(strText, L"����"))
					SBri[i].LineNum = _wtoi(strText);
				else
					SBri[i].LineNum = 1;
				if (TabFname == "�������ж�С�ź�" || TabFname == "�������ж�С�ź�")
				{
					Report_input.GetValueString(strText, L"���жα��");//???????????				
					SBri[i].iRXD = (int)(_wtof(strText) + 0.001);
				}

				i++;
				Report_input.MoveNext();
			}
		}


		else if (TabFname == "������")
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
				Report_input.GetValueString(Bri[i].NameOfWay, L"��·��");
				Report_input.GetValueString(Bri[i].BriName, L"��������");
				Report_input.GetValueString(Bri[i].CGH, L"������̹ں�");	//������̹ں�			
				Report_input.GetValueString(strText, L"���������");
				Bri[i].CenDml = _tstof(strText);//�������
				Report_input.GetValueString(Bri[i].BGH, L"�����̹ں�");	//��ʼ��̹ں�			
				Report_input.GetValueString(strText, L"��������");
				Bri[i].StDml = _tstof(strText);//������
				Report_input.GetValueString(Bri[i].EGH, L"�յ���̹ں�");	//��ֹ��̹ں�			
				Report_input.GetValueString(strText, L"�յ������");
				Bri[i].EdDml = _tstof(strText);//��ֹ���
				Report_input.GetValueString(Bri[i].KKSY, L"�׿�ʽ��");//�׿���ʽ
				Report_input.GetValueString(strText, L"����");
				Bri[i].LineNum = _tstoi(strText);//���� 
				Report_input.GetValueString(Bri[i].SWHt, L"100��ˮλ�߳�");//����ˮλ
				Report_input.GetValueString(Bri[i].GJZT, L"�Ľ�״̬");//�Ľ�״̬
				Report_input.GetValueString(strText, L"��ȫ��");//�Ľ�״̬
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

		else if (TabFname == "���߼ӿ�")
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
		//�����
		else if (TabFname == "���ڱ�")
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
				Report_input.GetValueString(TWB[i].NameOfWay, L"��·��");
				Report_input.GetValueString(TWB[i].GH, L"��׮��̹ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				TWB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"����߳�");
				TWB[i].DMGC = _wtof(strText);
				Report_input.GetValueString(strText, L"����¶�");
				TWB[i].SJPD = _wtof(strText);
				Report_input.GetValueString(strText, L"·����Ƹ߳�");
				TWB[i].LJSGG = _wtof(strText);
				Report_input.GetValueString(strText, L"�������ݾ�");
				TWB[i].SQXZJ = _wtof(strText);
				Report_input.GetValueString(strText, L"����̧��ֵ");
				TWB[i].ZXTJ = _wtof(strText);
				Report_input.GetValueString(strText, L"�������ڸ߶�");
				TWB[i].ZXTWG = _wtof(strText);
				Report_input.GetValueString(strText, L"·��ʩ���߳�");
				TWB[i].LJSGG = _wtof(strText);
				Report_input.GetValueString(strText, L"���߼ӿ�");
				TWB[i].ZQXJK = _wtof(strText);
				Report_input.GetValueString(strText, L"�����߼ӿ�");
				TWB[i].YQXJK = _wtof(strText);
				Report_input.GetValueString(strText, L"·��ʩ�����");
				TWB[i].LJSGK = _wtof(strText);
				Report_input.GetValueString(strText, L"���߼��");
				TWB[i].ZXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"���߼��");
				TWB[i].YXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"·������޿�");
				TWB[i].LJFLK = _wtof(strText);
				Report_input.GetValueString(strText, L"������Ϣ");
				TWB[i].DMinf = _wtoi(strText);
				Report_input.GetValueString(strText, L"��ͨ�����߼��");
				TWB[i].GTJXXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"��ͨ�����߼��");
				TWB[i].GTEXXJJ = _wtof(strText);
				Report_input.GetValueString(strText, L"���й���߳�");
				TWB[i].JYGMGC = _wtof(strText);
				Report_input.GetValueString(strText, L"��ƹ���߳�");
				TWB[i].SJGMGC = _wtof(strText);

				dmltockml(TWB[i].GH, TWB[i].ml, TWB[i].CKml);
				Report_input.MoveNext();
				i++;
			}
			NTW = i;
		}

		else if (TabFname == "����ṹ")
		{
			int i = 0;
			NGDSTRU = Report_input.GetRowCount();
			if (GDSTRU) delete[]GDSTRU;
			if (NGDSTRU > 0)
				GDSTRU = new RAILSTU[NGDSTRU];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(GDSTRU[i].XLName, L"��·��");
				Report_input.GetValueString(GDSTRU[i].FdKml, L"�ֶ������");
				Report_input.GetValueString(GDSTRU[i].StruType, L"�ṹ����");
				Report_input.GetValueString(strText, L"�ṹ�߶�");
				GDSTRU[i].StruHeight = _wtof(strText);
				Report_input.MoveNext();
				i++;
			}
			NGDSTRU = i;
		}

		else if (TabFname == "���")
		{
			int i = 0;
			NBC = Report_input.GetRecordCount();
			if (BC) delete[]BC;
			if (NBC > 0)
				BC = new BCTAB[NBC];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(strText, L"���");
				_tcscpy(BC[i].ckml, strText);
				Report_input.GetValueString(strText, L"�߳�");
				BC[i].Level = _tstof(strText);
				Report_input.MoveNext();
				i++;
			}
			NBC = i;
		}
		else if (TabFname == "�����������")
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
				Report_input.GetValueString(HDMB[i].NameOfWay, L"��·��");
				Report_input.GetValueString(HDMB[i].GH, L"��׮��̹ں�");//�ں�			
				Report_input.GetValueString(strText, L"��׮�����");//�����
				HDMB[i].dml = _wtof(strText);
				Report_input.GetValueString(strText, L"��������");//��������
				HDMB[i].BTinf = _wtoi(strText);
				Report_input.GetValueString(strText, L"����");//����
				HDMB[i].at = _wtof(strText);
				Report_input.GetValueString(strText, L"���������");//���������
				HDMB[i].aglt = _wtof(strText);
				Report_input.GetValueString(strText, L"��ˮ�����");//��ˮ�����
				HDMB[i].asst = _wtof(strText);
				Report_input.GetValueString(strText, L"��������");//��������
				HDMB[i].aqbt = _wtof(strText);
				Report_input.GetValueString(strText, L"�ڷ����");//�ڷ����
				HDMB[i].acut = _wtof(strText);
				Report_input.GetValueString(strText, L"I���ڷ����");//�ڷ����
				HDMB[i].aw[0] = _wtof(strText);
				Report_input.GetValueString(strText, L"II���ڷ����");//�ڷ����
				HDMB[i].aw[1] = _wtof(strText);
				Report_input.GetValueString(strText, L"III���ڷ����");//�ڷ����
				HDMB[i].aw[2] = _wtof(strText);
				Report_input.GetValueString(strText, L"IV���ڷ����");//�ڷ����
				HDMB[i].aw[3] = _wtof(strText);
				Report_input.GetValueString(strText, L"V���ڷ����");//�ڷ����
				HDMB[i].aw[4] = _wtof(strText);
				Report_input.GetValueString(strText, L"VI���ڷ����");//�ڷ����
				HDMB[i].aw[5] = _wtof(strText);
				Report_input.GetValueString(strText, L"���õؿ�");//���õؿ�
				HDMB[i].LZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"���õؿ�");//���õؿ�
				HDMB[i].RZDK = _wtof(strText);
				Report_input.GetValueString(strText, L"�������");//�������
				HDMB[i].ahj = _wtof(strText);
				Report_input.GetValueString(strText, L"ˮ���������");//ˮ���������
				HDMB[i].ajq = _wtof(strText);
				Report_input.GetValueString(strText, L"ˮ�������");//ˮ�������
				HDMB[i].asgt = _wtof(strText);
				Report_input.GetValueString(strText, L"ˮ�������");// ˮ�������
				HDMB[i].asgw = _wtof(strText);
				Report_input.GetValueString(strText, L"��Ƥ����б��");//��Ƥ����б��			  
				HDMB[i].cpbpxc = _wtof(strText);
				Report_input.GetValueString(strText, L"��ǽ����б��");//��ǽ����б��			  
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

	if ((TabFname == "������" || TabFname == "С�ź�" || TabFname == "���" || TabFname == "��վ") && (wlcstring != "��·��=\"���л���\"" && wlcstring != "��·��=\"��ͨ��ƻ���\"" && wlcstring != "��·��=\"��ͨ��Ƣ���\""))
	{
		//�����߲��������Ƿ������ж��ڣ��޳����ڵġ�
		GetRXDChaQi(mdbFname, strEXName, RXDName, TabFname);
	}

	return 1;
	}
int XLDataBase_JYX::Delete_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//д����·���ݿ��е�����
{

	CString strText, GH, strSQL, strSQL2;

	CFileFind Find;         //�ȶ��������,����������ĺ���
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		//		GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
		File.Format(L"%s\\lib\\��׼.mdb", SysPath);
		CopyFile(File, mdbFname, false);         //file �Ѵ���,Path �����ڸ����ļ�
	}

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
			ads_printf(L"����%s���ݿ�ʧ��!", mdbFname);
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}

	_variant_t str;
	InitTableList();

	if (FindTable(TabFname))//�����
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
int XLDataBase_JYX::WR_XLDbs(CString mdbFname, CString TabFname, CString wlcstring)//д����·���ݿ��е�����
{
	CString strText, GH, strSQL, strSQL2;

	CFileFind Find;         //�ȶ��������,����������ĺ���
	if (!Find.FindFile(mdbFname, 0))
	{
		CString File, Path;
		//    GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
		File.Format(L"%s\\lib\\��׼.mdb", SysPath);
		CopyFile(File, mdbFname, false);         //file �Ѵ���,Path �����ڸ����ļ�

	}

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
			//ads_printf(L"����%s���ݿ�ʧ��!",mdbFname);			
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}

	_variant_t str;
	InitTableList();

	if (FindTable(TabFname))//�����
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
	//������

	if (TabFname == "�������ݱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "����� VARCHAR,";
			strSQL += "����N DOUBLE PRECISION,";//�����±�     
			strSQL += "����E DOUBLE PRECISION,";
			//	strSQL +="ƫ�� DOUBLE PRECISION,";
			strSQL += "ǰ���� DOUBLE PRECISION,";
			strSQL += "�뾶 DOUBLE PRECISION,";
			strSQL += "�󻺳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		// strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL2);

		m_strSQL = L"select * from " + TabFname;
		//	 int iCols=0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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

				strText.Format(L"%s", JDarray[i].GJDNum);  //��·��
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%s", JDarray[i].JDNum);  //����
				str = strText;
				Report_input.PutCollect(2, str);


				strText.Format(L"%lf", JDarray[i].N);  //y

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JDarray[i].E);  //x

				str = strText;
				Report_input.PutCollect(4, str);

				if (i == 0 || i == NQX - 1)//���յ�뾶Ϊ0
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
	else if (TabFname == "�������߲���ԭʼ���ϱ�")
	{

		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "����� VARCHAR,";
			strSQL += "�þ��� integer,";//�����±� 
			//	strSQL +="�ں� VARCHAR,";
			strSQL += "������ VARCHAR,";
			strSQL += "ƫ�� DOUBLE PRECISION,";

			strSQL += "�ò�� DOUBLE PRECISION,";
			strSQL += "�ҽ� DOUBLE PRECISION,";
			strSQL += "����Ej DOUBLE PRECISION,";
			strSQL += "���Es DOUBLE PRECISION,";
			strSQL += "���� DOUBLE PRECISION,";
			strSQL += "��������ֵ DOUBLE PRECISION,";
			strSQL += "��������ֵ DOUBLE PRECISION,";
			//	 strSQL +="���� VARCHAR"; 
			strSQL += "���� VARCHAR,";
			strSQL += "��̲� DOUBLE PRECISION";

			/*		strSQL +="����x DOUBLE PRECISION,";
			strSQL +="����y DOUBLE PRECISION";*/

			strSQL += ")";

			m_adoConnection.Execute(strSQL);//������    //????

		}

		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL2);
		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				// return 0;
			}

			for (int i = 0; i < NOrigMeaNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", JYOrigMeaTable[i].jdno);  //�����
				str = strText;
				Report_input.PutCollect(1, str);


				strText.Format(L"%d", JYOrigMeaTable[i].ZJPoint);  //�þ���
				str = strText;
				Report_input.PutCollect(2, str);

				/*
				strText.Format(L"%s",JYOrigMeaTable[i].GH);  //�ں�
				str = strText;
				Report_input.PutCollect(3,str);
				*/

				strText = JYOrigMeaTable[i].Landmark;  //������
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].fj);  //ƫ��
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].ZhiCeJv);  //���
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Youjiao);  //�ҽ�
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Ej);  //����Ej
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].Es);  //����Es
				str = strText;
				Report_input.PutCollect(8, str);


				strText.Format(L"%lf", JYOrigMeaTable[i].deE);  //����
				str = strText;
				Report_input.PutCollect(9, str);


				strText.Format(L"%lf", JYOrigMeaTable[i].BLTerritoryPlus);  //��������(+)
				str = strText;
				Report_input.PutCollect(10, str);

				strText.Format(L"%lf", JYOrigMeaTable[i].BLTerritoryMinus);  //��������(-)
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
	if (TabFname == "��־��̱�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��־��� VARCHAR,";
			strSQL += "X���� DOUBLE PRECISION,";//�����±�     
			strSQL += "Y���� DOUBLE PRECISION";
			//	strSQL +="������� DOUBLE PRECISION,";
			//	strSQL +="ϵ��K DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		// strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL2);

		m_strSQL = L"select * from " + TabFname;
		//	 int iCols=0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < BZLCNum; i++)
			{
				Report_input.AddNew();

				//��·��
				strText = BZLCTable[i].GJDNum;
				str = strText;
				Report_input.PutCollect(1, str);

				strText = BZLCTable[i].kml;
				str = strText;     //��־���
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





	if (TabFname == "����׮���ߵ��")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "�ں� VARCHAR,";
			strSQL += "��� VARCHAR,";
			strSQL += "����N DOUBLE PRECISION,";//�����±�     
			strSQL += "����E DOUBLE PRECISION,";

			strSQL += "���ƾ� DOUBLE PRECISION,";
			strSQL += "���� VARCHAR,";
			strSQL += "��·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < m_iNWYZ; i++)
			{
				Report_input.AddNew();

				//��·��
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
					str = "ֱ����";
				else if (m_pWYZArr[i].m_iZXOrQX == -1)
					str = "ֱ��ʼ";
				else if (m_pWYZArr[i].m_iZXOrQX == 1)
					str = "ֱ��ֹ";
				else if (m_pWYZArr[i].m_iZXOrQX == 2)
					str = "���ߵ�";
				else if (m_pWYZArr[i].m_iZXOrQX == 3)
					str = "ֱ�ߵ�";
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




	if (TabFname == "����̨�ʱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "�ں� VARCHAR,";
			strSQL += "��� VARCHAR,";
			strSQL += "��·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			//		 int j=0;

			for (int i = 0; i < m_iNZMTZ; i++)
			{
				Report_input.AddNew();

				//��·��
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



	if (TabFname == "����������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��ʼ��� VARCHAR,";
			strSQL += "��ֹ��� VARCHAR,";
			strSQL += "���� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}


		m_strSQL = L"select * from " + TabFname;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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
	else if (TabFname == "�������߲�����������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "����� VARCHAR,";
			strSQL += "����ƫ�� DOUBLE PRECISION,";//�����±�     
			strSQL += "ǰ���� DOUBLE PRECISION,";
			strSQL += "���߰뾶 DOUBLE PRECISION,";
			strSQL += "�󻺳� DOUBLE PRECISION,";
			strSQL += "ZH��� DOUBLE PRECISION,";
			strSQL += "HY��� DOUBLE PRECISION,";
			strSQL += "YH��� DOUBLE PRECISION,";
			strSQL += "HZ��� DOUBLE PRECISION,";
			strSQL += "�ں� VARCHAR,";
			strSQL += "�Ƿ���ҵʵ�� VARCHAR,";
			strSQL += "��·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
				//		return 0;
			}


			for (int i = 0; i < NBLNum; i++)
			{

				//	 ads_printf(L"i=%d,JYBoLiangTable[i].l1=%lf\n",i,JYBoLiangTable[i].l1);
				//	 Report_input.MoveLast();
				Report_input.AddNew();



				strText = JYBoLiangTable[i].JDNum;  //�����
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", JYBoLiangTable[i].alfa);  //����ƫ��

				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", JYBoLiangTable[i].l1);  //ǰ����

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", JYBoLiangTable[i].R);  //�뾶
				str = strText;
				Report_input.PutCollect(4, str);


				strText.Format(L"%lf", JYBoLiangTable[i].l2);  //�󻺳�
				str = strText;
				Report_input.PutCollect(5, str);


				strText.Format(L"%lf", JYBoLiangTable[i].KZH);   //KZH���
				str = strText;
				Report_input.PutCollect(6, str);


				strText.Format(L"%lf", JYBoLiangTable[i].KHY);  //KHY���
				str = strText;
				Report_input.PutCollect(7, str);

				//strText.Format(L"%lf",JYBoLiangTable[i].KQZ);  //KQZ���
				//str = strText;
				//Report_input.PutCollect(8,str);

				strText.Format(L"%lf", JYBoLiangTable[i].KYH);  //KYH���
				str = strText;
				Report_input.PutCollect(8, str);

				strText.Format(L"%lf", JYBoLiangTable[i].KHZ);  //KHZ���
				str = strText;
				Report_input.PutCollect(9, str);


				strText = JYBoLiangTable[i].GH;  //�ں�
				str = strText;
				Report_input.PutCollect(10, str);

				if (JYBoLiangTable[i].IsMeasure)//�Ƿ���ҵʵ��
					Report_input.PutCollect(11, L"��");
				else
					Report_input.PutCollect(11, L"��");

				strText = JYBoLiangTable[i].XLName;  //��·��
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

	else if (TabFname == "������߽����")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "����� VARCHAR,";
			strSQL += "����ƫ�� DOUBLE PRECISION,";//�����±�     
			strSQL += "ǰ���� DOUBLE PRECISION,";
			strSQL += "���߰뾶 DOUBLE PRECISION,";
			strSQL += "�󻺳� DOUBLE PRECISION,";
			strSQL += "CKZH��� VARCHAR,";

			strSQL += "CKHY��� VARCHAR,";

			strSQL += "CKYH��� VARCHAR,";
			strSQL += "CKHZ��� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������

		}
		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
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

				strText = SJQXB[i].JDNum;  //�����
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%.12lf", SJQXB[i].LorR * SJQXB[i].a);  //����ƫ��

				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", SJQXB[i].l1);  //ǰ����

				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", SJQXB[i].R);  //�뾶
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", SJQXB[i].l2);  //�󻺳�
				str = strText;
				Report_input.PutCollect(6, str);


				str = SJQXB[i].CKZH;   //CKZH���
				Report_input.PutCollect(7, str);

				str = SJQXB[i].CKHY;  //CKHY���
				Report_input.PutCollect(8, str);


				str = SJQXB[i].CKYH;  //CKYH���
				Report_input.PutCollect(9, str);

				str = SJQXB[i].CKHZ;  //CKHZ���
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


	else if (TabFname == "�Ľ��λ��ܱ�")
	{
		if (!FindTable(TabFname))//������
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";

			strSQL += "������ VARCHAR,";
			strSQL += "���N���� DOUBLE PRECISION,";
			strSQL += "���E���� DOUBLE PRECISION,";

			strSQL += "�յ���� VARCHAR,";
			strSQL += "�յ�N���� DOUBLE PRECISION,";
			strSQL += "�յ�E���� DOUBLE PRECISION,";
			strSQL += "��ʼ��·�� VARCHAR,";
			strSQL += "��ֹ��·�� VARCHAR,";
			strSQL += "�Ľ����� VARCHAR,";
			strSQL += "�Ľ��� VARCHAR";

			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
				//		return 0;
			}


			for (int i = 0; i < GJDAllInforNum; i++)
			{
				Report_input.AddNew();

				strText = GJDAllInforTable[i].ToOutSGJJDNum;  //��·��
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


				//��ʼ��·����
				strText = GJDAllInforTable[i].BXLName;
				str = strText;
				Report_input.PutCollect(8, str);

				strText = GJDAllInforTable[i].EXLName;  //�Ľ�����ֹ��·��
				str = strText;
				Report_input.PutCollect(9, str);

				if (_tcscmp(GJDAllInforTable[i].ToOutSGJJDNum, L"��ͨ��ƻ���") && _tcscmp(GJDAllInforTable[i].ToOutSGJJDNum, L"��ͨ��Ƣ���"))//��������
				{
					strText = GJDAllInforTable[i].GJXZ;
					str = strText;
					Report_input.PutCollect(10, str);
				}
				else
				{
					if (GJDAllInforTable[i].GJDType == 0)str = "���иĽ�";
					else str = "���иĽ�";
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

	else if (TabFname == "������Ϣ��")
	{

		if (!FindTable(TabFname))//������
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��������·�� VARCHAR,";
			strSQL += "��ʼ��·�� VARCHAR,";
			strSQL += "��ֹ��·�� VARCHAR,";//�����±�     
			strSQL += "�ڲ���������·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		for (int i = 0; i < ConnectLineInforNum; i++)
		{
			wlcstring = ConnectLineTable[i].ToInSGJJDNum;
			wlcstring = L"�ڲ���������·��=\"" + wlcstring + "\"";

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
			m_adoConnection.Execute(strSQL2);
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
				//		return 0;
			}



			for (int i = 0; i < ConnectLineInforNum; i++)
			{
				Report_input.AddNew();

				strText = ConnectLineTable[i].ToOutSGJJDNum;  //��·��
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

	else if (TabFname == "ʩ��������Ϣ��")
	{

		if (!FindTable(TabFname))//������
		{

			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��ʼ��·�� VARCHAR,";
			strSQL += "��ֹ��·�� VARCHAR,";//�����±�     
			strSQL += "�ڲ���·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		for (int i = 0; i < SGBXInforNum; i++)
		{
			wlcstring = SGBXTable[i].SGBXName;
			wlcstring = L"��·��=\"" + wlcstring + "\"";

			strSQL2 = "delete from ";
			strSQL2 += TabFname;
			strSQL2 += " where ";
			strSQL2 += wlcstring;
			m_adoConnection.Execute(strSQL2);
		}
		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
				//		return 0;
			}

			for (int i = 0; i < SGBXInforNum; i++)
			{
				Report_input.AddNew();

				strText = SGBXTable[i].SGBXName;  //��·��
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
	else if (TabFname == "�ֲ�������Ϣ��")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		for (int i = 0; i < JBJXInforNum; i++)
		{
			wlcstring = JBJXTable[i].JBJXName;
			wlcstring = L"��·��=\"" + wlcstring + "\"";

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
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				// ads_printf(L"û�м�¼!");
				//		return 0;
			}

			for (int i = 0; i < JBJXInforNum; i++)
			{
				Report_input.AddNew();

				strText = JBJXTable[i].JBJXName;  //��·��
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
	else if (TabFname == "������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·��  VARCHAR,";
			strSQL += "�Ⱥ�����̹ں� VARCHAR,";
			strSQL += "�Ⱥ�������� DOUBLE PRECISION,";
			strSQL += "�Ⱥ�����̹ں� VARCHAR,";  //DOUBLE PRECISION -> VARCHAR
			strSQL += "�Ⱥ�������� DOUBLE PRECISION,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "����N DOUBLE PRECISION,";
			strSQL += "����E DOUBLE PRECISION";
			//	strSQL +="��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		//	strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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

	//�����������߱�
	///////////////

	/*
	else if(TabFname=="���߸Ľ��ֶα�")
	{
	if(!FindTable(TabFname))//������
	{
	CString strSQL;
	strSQL = "create table " ;
	strSQL += TabFname;
	strSQL += "(";
	strSQL += "��� integer identity,";
	strSQL += "������·�� VARCHAR,";
	strSQL +="���N DOUBLE PRECISION,";
	strSQL +="���E DOUBLE PRECISION,";
	strSQL +="�յ�N DOUBLE PRECISION,";
	strSQL +="�յ�E DOUBLE PRECISION";
	strSQL += ")";
	m_adoConnection.Execute(strSQL);//������
	}

	m_strSQL ="select * from "+TabFname;
	int iCols=0;
	try
	{
	if (!m_adoConnection.IsOpen())
	{
	ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
	return 0;
	}

	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
	//	 ads_printf(L"û�м�¼!");
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

	else if (TabFname == "�������߱�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "�������� VARCHAR,";
			strSQL += "���߽���� VARCHAR,";
			strSQL += "һ�߽���� VARCHAR,";
			strSQL += "�߼�� DOUBLE PRECISION,";
			strSQL += "����N DOUBLE PRECISION,";
			strSQL += "����E DOUBLE PRECISION,";
			strSQL += "ǰ�������߳� DOUBLE PRECISION,";
			strSQL += "���߰뾶 DOUBLE PRECISION,";
			strSQL += "�󻺺����߳� DOUBLE PRECISION,";
			strSQL += "��ҵ���� DOUBLE PRECISION,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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
	else if (TabFname == "���߶�����")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "���ж��� VARCHAR,";
			strSQL += "��·��� VARCHAR,";
			strSQL += "�Ⱥ�����̹ں� VARCHAR,";
			strSQL += "�Ⱥ�������� DOUBLE PRECISION,";
			strSQL += "�Ⱥ�����̹ں� VARCHAR,";
			strSQL += "�Ⱥ�������� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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
	//------------------------����-----------------------------//
	//////////////////////
	else if (TabFname == "�ݶ������������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";//�����±� 
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "������� DOUBLE PRECISION,";
			strSQL += "������� DOUBLE PRECISION,";
			strSQL += "���ȡ�� VARCHAR,";
			strSQL += "�¶�λ�� VARCHAR,";
			strSQL += "�߳�λ�� VARCHAR";
			strSQL += ")";

			m_adoConnection.Execute(strSQL);//������ 
		}

		//	 strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//	 m_adoConnection.Execute(strSQL2);
		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//ads_printf(L"û�м�¼!");
				//return 0;
			}

			for (int i = 0; i < 1; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", pBasicDataOfZDM[i].WayName);  //��·��
				str = strText;
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", pBasicDataOfZDM[i].XScale);  //X����
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", pBasicDataOfZDM[i].YScale);  //Y����
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].MLDecimal);  //���ȡ��
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].PDDeciamal);  //�¶�ȡ��
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%s", pBasicDataOfZDM[i].HDecimal);  //�¶�ȡ��
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
	if (TabFname == "�¶ȱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "���µ���̹ں� VARCHAR,";
			strSQL += "���µ������ DOUBLE PRECISION,";
			strSQL += "��Ƹ߳� DOUBLE PRECISION,";
			strSQL += "�¶� DOUBLE PRECISION,";
			strSQL += "�³� DOUBLE PRECISION,";
			strSQL += "�����߰뾶 DOUBLE PRECISION,";
			strSQL += "�����߳��� DOUBLE PRECISION,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NPD; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", PDB[i].NameOfWay);  //��·��
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
					//if (GlobalBasicDataOfZDM.m_HDecimal=="������λС��")//BasicDataOfZDM-1
					//{
					//	strText.Format(_T("%d"),0);
					//}
					//else if (GlobalBasicDataOfZDM.m_HDecimal=="����һλС��")
					//{
					//	strText.Format(_T("%d"),1);
					//}
					//else if (GlobalBasicDataOfZDM.m_HDecimal=="������λС��")
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

					//strText.Format(L"%d",GlobalBasicDataOfZDM.m_LJorGM); //ȥBasicDataOfZDM-2
					//str = strText;
					//Report_input.PutCollect(8,str);

					//if (GlobalBasicDataOfZDM.m_PDDeciamal=="������λС��")
					//{
					//	strText.Format(_T("%d"),0);
					//}
					//else if (GlobalBasicDataOfZDM.m_PDDeciamal=="����һλС��")
					//{
					//	strText.Format(_T("%d"),1);
					//}
					//else if (GlobalBasicDataOfZDM.m_PDDeciamal=="������λС��")
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
	if (TabFname == "������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "�������� VARCHAR,";

			strSQL += "������̹ں� VARCHAR,";
			strSQL += "��������� DOUBLE PRECISION,";
			strSQL += "�����̹ں� VARCHAR,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "�յ���̹ں� VARCHAR,";
			strSQL += "�յ������ DOUBLE PRECISION,";

			strSQL += "�׿�ʽ�� VARCHAR,";
			strSQL += "���� INTEGER,";
			strSQL += "��ȫ�� DOUBLE PRECISION,";
			strSQL += "100��ˮλ�߳� DOUBLE PRECISION,";
			strSQL += "�Ľ�״̬ VARCHAR,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", Bri[i].NameOfWay);  //��·��
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
	if (TabFname == "���")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "������� VARCHAR,";
			strSQL += "�����̹ں� VARCHAR,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "�յ���̹ں� VARCHAR,";
			strSQL += "�յ������ DOUBLE PRECISION,";
			strSQL += "���췽ʽ VARCHAR,";
			strSQL += "�Ľ�״̬ VARCHAR,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < TunNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", Tun[i].NameOfWay);  //��·��
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
	if (TabFname == "������")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "����߳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NDMX; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", DMXB[i].NameOfWay);  //��·��
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
	if (TabFname == "���й�����")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "���й���߳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NGMX; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", GMXB[i].NameOfWay);  //��·��
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



	else if (TabFname == "��վ")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��վ���� VARCHAR,";
			strSQL += "վ��������̹ں� VARCHAR,";
			strSQL += "վ����������� DOUBLE PRECISION,";
			strSQL += "�����̹ں� VARCHAR,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "�յ���̹ں� VARCHAR,";
			strSQL += "�յ������ DOUBLE PRECISION,";
			strSQL += "��վ���� VARCHAR,";
			strSQL += "����ͼ���� VARCHAR,";
			strSQL += "�Ľ����� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		//strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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


	if (TabFname == "С�ź�")
	{
		if (!FindTable(TabFname))//������
		{
			CString m_SQL;
			m_SQL = "CREATE TABLE С�ź� ";
			m_SQL += "(";
			m_SQL += "��� INTEGER identity,";
			m_SQL += "��·�� VARCHAR,";
			m_SQL += "������̹ں� VARCHAR,";
			m_SQL += "��������� DOUBLE PRECISION,";
			m_SQL += "�׿�ʽ�� VARCHAR,";
			m_SQL += "����򳤶� DOUBLE PRECISION,";
			m_SQL += "�Һ��򳤶� DOUBLE PRECISION,";
			m_SQL += "�󺭿ڱ�� DOUBLE PRECISION,";
			m_SQL += "�Һ��ڱ�� DOUBLE PRECISION,";
			m_SQL += "���� DOUBLE PRECISION,";
			m_SQL += "��; INTEGER,";
			m_SQL += "���췽ʽ VARCHAR,";
			m_SQL += "�Ľ����� VARCHAR,";
			m_SQL += "���� INTEGER";
			m_SQL += ")";
			m_adoConnection.Execute(m_SQL);
		}
		//strSQL.Format(L"delete from %s",TabFname);//ɾ��ȫ��
		//m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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




	if (TabFname == "���е�����ȱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮�ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "������� DOUBLE PRECISION,";
			strSQL += "����ṹ�߶� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < RoadBedNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", RoadBed[i].NameOfWay);  //��·��
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
	if (TabFname == "����·��̱߳�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮�ں� VARCHAR,";
			strSQL += "��׮��� DOUBLE PRECISION,";
			strSQL += "����·��߳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NJiYouLuJianGC; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", JiYouLuJianGC[i].NameOfWay);  //��·��
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
	if (TabFname == "������Ƶ��")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮�ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "���޸߳� DOUBLE PRECISION,";
			strSQL += "���޸߳� DOUBLE PRECISION,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < ZMControlPTNum; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", ZMControlPT[i].NameOfWay);  //��·��
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
	if (TabFname == "�������ռ�ؿ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��׮�ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "·���߽��� DOUBLE PRECISION,";
			strSQL += "ռ�ر߽��� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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
	if (TabFname == "�Ҽ�����ռ�ؿ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��׮�ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "·���߽��� DOUBLE PRECISION,";
			strSQL += "ռ�ر߽��� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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

	if (TabFname == "�õ����ͱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "�ֶ�����̹ں� VARCHAR,";
			strSQL += "�ֶ�������� DOUBLE PRECISION,";
			strSQL += "�õ����� VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
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

	if (TabFname == "ˮ׼���")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "ˮ׼��� ";
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "ˮ׼���� VARCHAR,";
			strSQL += "�߳� DOUBLE PRECISION,";
			strSQL += "��̹ں� VARCHAR,";
			strSQL += "����� DOUBLE PRECISION,";
			strSQL += "�����߾��� DOUBLE PRECISION,";
			strSQL += "����N DOUBLE PRECISION,";
			strSQL += "����E DOUBLE PRECISION,";
			strSQL += "λ�ò��ϵ����� VARCHAR,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < BmNum; i++)
			{
				Report_input.AddNew();

				str = BM[i].BianHao; //ˮ׼����
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", BM[i].GaoCheng); //�߳�
				str = strText;
				Report_input.PutCollect(2, str);

				str = BM[i].GH; // ��̹ں�
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", BM[i].XLC);  //�����
				str = strText;
				Report_input.PutCollect(4, str);


				strText.Format(L"%lf", BM[i].DistZX);  // �����߾���
				str = strText;
				Report_input.PutCollect(5, str);


				strText.Format(L"%lf", BM[i].N);  //����N
				str = strText;
				Report_input.PutCollect(6, str);


				strText.Format(L"%lf", BM[i].E);  //����E
				str = strText;
				Report_input.PutCollect(7, str);

				str = BM[i].CLMS;  //λ�ò��ϵ�����
				Report_input.PutCollect(8, str);

				str = BM[i].FZ;  //��ע
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


	if (TabFname == "���߱�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += "���߱� ";
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "׮�� VARCHAR,";
			strSQL += "�ҽ� VARCHAR,";
			strSQL += "��λ�� VARCHAR,";
			strSQL += "���� DOUBLE PRECISION,";
			strSQL += "γ�� DOUBLE PRECISION,";
			strSQL += "���� DOUBLE PRECISION,";
			strSQL += "�߳� DOUBLE PRECISION,";
			strSQL += "��� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������

		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < DXNum; i++)
			{
				Report_input.AddNew();

				str = DX[i].ZhuangHao; //׮��
				Report_input.PutCollect(1, str);

				strText.Format(L"%lf", DX[i].RightAng); //�ҽ�
				str = strText;
				Report_input.PutCollect(2, str);

				strText.Format(L"%lf", DX[i].FWJ);//��λ��
				str = strText;
				Report_input.PutCollect(3, str);

				strText.Format(L"%lf", DX[i].Dist);  //����
				str = strText;
				Report_input.PutCollect(4, str);

				strText.Format(L"%lf", DX[i].N);  // γ��
				str = strText;
				Report_input.PutCollect(5, str);

				strText.Format(L"%lf", DX[i].E);  //����
				str = strText;
				Report_input.PutCollect(6, str);

				strText.Format(L"%lf", DX[i].GaoCheng); //�߳�
				str = strText;
				Report_input.PutCollect(7, str);

				strText.Format(L"%lf", DX[i].cml); //���
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

	if (TabFname == "���")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��� VARCHAR,";
			strSQL += "�߳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NBC; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", BC[i].NameOfWay);  //��·��
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


	if (TabFname == "����ṹ")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� integer identity,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "�ֶ������ VARCHAR,";
			strSQL += "�ṹ���� VARCHAR,";
			strSQL += "�ṹ�߶� DOUBLE PRECISION ";

			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NGDSTRU; i++)
			{
				Report_input.AddNew();

				strText.Format(L"%s", GDSTRU[i].XLName);  //��·��
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


	else if (TabFname == "���ڱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "��� INTEGER IDENTITY,";
			strSQL += "��·�� VARCHAR,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "����߳� DOUBLE PRECISION, ";
			strSQL += "����¶�  DOUBLE PRECISION, ";
			strSQL += "·����Ƹ߳� DOUBLE PRECISION,";
			strSQL += "�������ݾ� DOUBLE PRECISION,";
			strSQL += "����̧��ֵ DOUBLE PRECISION,";
			strSQL += "�������ڸ߶� DOUBLE PRECISION,";
			strSQL += "·��ʩ���߳� DOUBLE PRECISION,";
			strSQL += "���߼ӿ� DOUBLE PRECISION,";
			strSQL += "�����߼ӿ� DOUBLE PRECISION,";
			strSQL += "·��ʩ����� DOUBLE PRECISION,";
			strSQL += "���߼�� DOUBLE PRECISION,";
			strSQL += "���߼�� DOUBLE PRECISION,";
			strSQL += "·������޿� DOUBLE PRECISION,";
			strSQL += "������Ϣ VARCHAR,";
			strSQL += "��ע VARCHAR,";
			strSQL += "��ͨ�����߼�� DOUBLE PRECISION,";
			strSQL += "��ͨ�����߼�� DOUBLE PRECISION,";
			strSQL += "���й���߳� DOUBLE PRECISION,";
			strSQL += "��ƹ���߳� DOUBLE PRECISION";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NTW; i++)
			{
				Report_input.AddNew();
				wlcstring.Replace(L"��·��=\"", L"");
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
int XLDataBase_JYX::WR_EX_XLDbs(CString mdbFname, CString TabFname, int irxd)//д����·���ݿ��е�����
{
	CString strText, GH, strSQL;

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
			AfxMessageBox(L"ʧ��  3!");
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}
	_variant_t str;
	InitTableList();
	if (TabFname == "�������ж��¶ȱ�" || TabFname == "�������ж��¶ȱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "���жα�� INTEGER,";
			strSQL += "���µ���̹ں� VARCHAR,";
			strSQL += "���µ������ DOUBLE PRECISION,";
			strSQL += "��Ƹ߳� DOUBLE PRECISION,";
			strSQL += "�¶� DOUBLE PRECISION,";
			strSQL += "�³� DOUBLE PRECISION,";
			strSQL += "�����߰뾶 DOUBLE PRECISION,";
			strSQL += "�����߳��� DOUBLE PRECISION,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}

		strSQL.Format(L"delete from %s where ���жα��=%d", TabFname, irxd);//ɾ��irxdȫ��
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}
			for (int i = 0; i < NPD; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//���жα��
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
	else if (TabFname == "�������жδ�����" || TabFname == "�������жδ�����")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "���жα�� INTEGER,";
			strSQL += "�������� VARCHAR,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "�����̹ں� VARCHAR,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "�յ���̹ں� VARCHAR,";
			strSQL += "�յ������ DOUBLE PRECISION,";
			strSQL += "�׿�ʽ�� VARCHAR,";
			strSQL += "���� VARCHAR,";
			strSQL += "��ȫ�� DOUBLE PRECISION,";
			strSQL += "100��ˮλ�߳� VARCHAR,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		strSQL.Format(L"delete from %s where ���жα��=%d", TabFname, irxd);//ɾ��irxdȫ��
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}

			for (int i = 0; i < BriNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//���жα��
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

				str = "1";//����(սʱ)
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
	else if (TabFname == "�������ж����" || TabFname == "�������ж����")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "���жα�� INTEGER,";
			strSQL += "������� VARCHAR,";
			strSQL += "�����̹ں� VARCHAR,";
			strSQL += "�������� DOUBLE PRECISION,";
			strSQL += "�յ���̹ں� VARCHAR,";
			strSQL += "�յ������ DOUBLE PRECISION,";
			strSQL += "���췽ʽ VARCHAR,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		strSQL.Format(L"delete from %s where ���жα��=%d", TabFname, irxd);//ɾ��irxdȫ��
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}

			for (int i = 0; i < TunNum; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//���жα��
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
	else if (TabFname == "�������ж����ڱ�" || TabFname == "�������ж����ڱ�")
	{
		if (!FindTable(TabFname))//������
		{
			CString strSQL;
			strSQL = "create table ";
			strSQL += TabFname;
			strSQL += "(";
			strSQL += "ID integer identity,";
			strSQL += "���жα�� INTEGER,";
			strSQL += "��׮��̹ں� VARCHAR,";
			strSQL += "��׮����� DOUBLE PRECISION,";
			strSQL += "����߳� DOUBLE PRECISION, ";
			strSQL += "����¶� DOUBLE PRECISION, ";
			strSQL += "·����Ƹ߳� DOUBLE PRECISION,";
			strSQL += "�������ݾ� DOUBLE PRECISION,";
			strSQL += "����̧��ֵ DOUBLE PRECISION,";
			strSQL += "�������ڸ߶� DOUBLE PRECISION,";
			strSQL += "·��ʩ���߳� DOUBLE PRECISION,";
			strSQL += "���߼ӿ� DOUBLE PRECISION,";
			strSQL += "·��ʩ����� DOUBLE PRECISION,";
			strSQL += "��ע VARCHAR";
			strSQL += ")";
			m_adoConnection.Execute(strSQL);//������
		}
		strSQL.Format(L"delete from %s where ���жα��=%d", TabFname, irxd);//ɾ��irxdȫ��
		m_adoConnection.Execute(strSQL);

		m_strSQL = L"select * from " + TabFname;
		int iCols = 0;
		try
		{
			if (!m_adoConnection.IsOpen())
			{
				ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
				return 0;
			}

			Report_input.SetAdoConnection(&m_adoConnection);
			if (Report_input.Open(m_strSQL, adCmdText) != 1)
			{
				//	 ads_printf(L"û�м�¼!");
				//		return 0;
			}


			for (int i = 0; i < NTW; i++)
			{
				Report_input.AddNew();
				strText.Format(L"%d", i);//ID
				str = strText;
				Report_input.PutCollect(0, str);

				strText.Format(L"%d", irxd);//���жα��
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


int XLDataBase_JYX::Read_EX_XLDbs(CString mdbFname, CString TabFname, int irxd)//��ȡ��·���ݿ��еĶ�������
{
	CString strText, GH;
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
			AfxMessageBox(L"ʧ��  4!");
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}

	//	m_strSQL ="select * from "+TabFname;
	m_strSQL.Format(L"select * from %s where ���жα��=%d order by ID", TabFname, irxd);//ɾ��irxdȫ��
	//		 m_adoConnection.Execute(strSQL);
	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		{
			ads_printf(L"%sû�м�¼!\n", TabFname);
			return 0;
			//return false;
			//Report_input.Close();
		}
		Report_input.MoveFirst();
		if (TabFname == "�������ж����" || TabFname == "�������ж����")
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
		else if (TabFname == "�������жδ�����" || TabFname == "�������жδ�����")
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
		else if (TabFname == "�������ж��¶ȱ�" || TabFname == "�������ж��¶ȱ�")
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
				Report_input.GetValueString(PDB[i].GH, "���µ���̹ں�");
				Report_input.GetValueString(strText, L"���µ������");
				PDB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"��Ƹ߳�");
				PDB[i].Level = _wtof(strText);
				Report_input.GetValueString(strText, L"�����߰뾶");
				PDB[i].Rshu = _wtof(strText);
				//	ads_printf(L"i=%d,ml=%lf Level=%lf Rshu=%lf\n",i,PDB[i].ml,PDB[i].Level,PDB[i].Rshu);
				Report_input.MoveNext();
				i++;
			}
		}
		else if (TabFname == "�������ж����ڱ�" || TabFname == "�������ж����ڱ�")
		{
			int i = 0;
			NTW = Report_input.GetRowCount();
			if (TWB) delete[]TWB;
			if (NTW > 0)
				TWB = new TWTAB[NTW];
			Report_input.MoveFirst();
			while (!Report_input.IsEOF())
			{
				Report_input.GetValueString(TWB[i].GH, "��׮��̹ں�");
				Report_input.GetValueString(strText, L"��׮�����");
				TWB[i].ml = _wtof(strText);
				Report_input.GetValueString(strText, L"����߳�");
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


void XLDataBase_JYX::GetJDNumArrayFromBLCalTab(CString mdbFname, CString TabFname, CArray<CString, CString>& GetJDNumArray)//��ȡ��·���ݿ��е�����
{
	CString strText, GH;
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
			//         ads_printf(L"����%s���ݿ�ʧ��!\n",mdbFname);
			//			AfxMessageBox(L"�������ݿ�ʧ��!");			
			//	return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}

	m_strSQL = L"select * from " + TabFname;
	m_strSQL += L" order by ���";
	///////////////////////

	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
			//	return 0;
		}

		Report_input.SetAdoConnection(&m_adoConnection);

		//		if (Report_input.Open(m_strSQL, adCmdText) != 1)
		//	    {
		//          ads_printf(L"û�м�¼!");
		//			NPD=NHDM=0;
		//			return 0;
		//return false;
		//Report_input.Close();
		//       }
		Report_input.Open(m_strSQL);

		//   Report_input.MoveFirst();


		/////////////////////////////////////////////////////////////////////////////
		////������·�Ľ�   //  ������
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
			Report_input.GetValueString(strText, L"�����");

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


int XLDataBase_JYX::WR_EX_TWB(CString mdbFname, CString TabFname, int irxd)//д����·���ݿ��е�����
{
	CString strText, GH, strSQL;

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
			AfxMessageBox(L"ʧ�� 5!");
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ�����ݿ��Ƿ����!");
	}
	_variant_t str;
	InitTableList();

	if (!FindTable(TabFname))//������
	{
		CString strSQL;
		strSQL = "create table ";
		strSQL += TabFname;
		strSQL += "(";
		strSQL += "ID INTEGER IDENTITY,";
		strSQL += "���жα�� INTEGER,";
		strSQL += "��׮��̹ں� VARCHAR,";
		strSQL += "��׮����� DOUBLE PRECISION,";
		strSQL += "����߳� DOUBLE PRECISION, ";
		strSQL += "����¶� DOUBLE PRECISION, ";
		strSQL += "·����Ƹ߳� DOUBLE PRECISION,";
		strSQL += "�������ݾ� DOUBLE PRECISION,";
		strSQL += "����̧��ֵ DOUBLE PRECISION,";
		strSQL += "�������ڸ߶� DOUBLE PRECISION,";
		strSQL += "·��ʩ���߳� DOUBLE PRECISION,";
		strSQL += "���߼ӿ� DOUBLE PRECISION,";
		strSQL += "�����߼ӿ� DOUBLE PRECISION,";
		strSQL += "·��ʩ����� DOUBLE PRECISION,";
		strSQL += "���߼�� DOUBLE PRECISION,";
		strSQL += "���߼�� DOUBLE PRECISION,";
		strSQL += "·������޿� DOUBLE PRECISION,";
		strSQL += "������Ϣ VARCHAR,";
		strSQL += "��ע VARCHAR";
		strSQL += ")";
		m_adoConnection.Execute(strSQL);//������
	}
	strSQL.Format(L"delete from %s where ���жα��=%d", TabFname, irxd);//ɾ��irxdȫ��
	m_adoConnection.Execute(strSQL);

	m_strSQL = L"select * from " + TabFname;
	int iCols = 0;
	try
	{
		if (!m_adoConnection.IsOpen())
		{
			ads_alert(L"���ݿ�û�д򿪻��Ѿ��ر�!");
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

			strText.Format(L"%d", irxd);//���жα��
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
//	//1.����ͨ��ƻ��ߵ��¶ȣ�������ͨ��ƻ���ָ��
//
//	CString strNameOfWay1="��·��=\""+XLName+"\"";
//
//	if(Read_XLDbs(NameOfMDB,"�¶ȱ�",strNameOfWay1))
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
//	XLName="��ͨ��ƻ���";
//	//2.���Ľ��¶�
//	CString strNameOfDel="(�Ľ�����=\"";
//	strNameOfDel += "���еȸ߸���\"" ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp="�Ľ�����=\"";
//	strTemp += "��ͨ��ƻ��߸���\"" ;
//	strNameOfDel += strTemp ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp1="�Ľ�����=\"";
//	strTemp1 += "��ͨ��ƶ��߸���\")" ;
//	strNameOfDel += strTemp1 ;
//
//	strNameOfDel += " AND " ;
//	CString  strNameOfWay="�Ľ���=\""+ XLName +"\"";;
//	strNameOfDel += strNameOfWay ;
//
//	CArray<CString,CString>strZPGJ;
//	CString strXLName;
//	if(Read_XLDbs(NameOfMDB,"�Ľ��λ��ܱ�",strNameOfDel))//���Ľ�������
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
//		strNameOfWay="��·��=\""+strNameOfWay+"\"";
//		Read_XLDbs(NameOfMDB,"�¶ȱ�",strNameOfWay);
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
//	//3.�����������¶�
//	for (int i=0;i<m_MPD.GetSize();i++)
//	{
//		m_APD.RemoveAll();
//		m_APD = m_MPD.GetAt(i);
//
//
//		//3.1�ҵ�Ҫ�滻����ʼ�¶Ⱥ�
//
//		double dtmp1 = m_APD[0].ml;
//		double dtmp2 = m_APD[m_APD.GetSize()-1].ml;
//
//		int StNumLeft,EdNumLeft;
//		int StNumRight,EdNumRight;
//
//		int flag;
//		if (m_APD[1].Notes=="���еȸ߸���")
//		{
//			flag=0;
//			StNumLeft = pLeftSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumLeft = pLeftSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//			StNumRight = pRightSlope->FindSlopeNum(m_APD[0].ml);
//			EdNumRight = pRightSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		}
//		else if (m_APD[1].Notes=="��ͨ��ƻ��߸���")
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
//		//3.2.remove StNum<Num<EdNum+1 �ı��µ�aa
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
//		//3.3.add m_APD��0~Size-1�ı��µ�
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
//		//3.4���������
//		pLeftSlope->Ordel(pLeftSlope->m_SlopeValue);
//		pRightSlope->Ordel(pRightSlope->m_SlopeValue);
//
//
//		//Zhujiang-�޳��ظ����¶����� 20090618
//		pLeftSlope->DeleteRepeatData(pLeftSlope->m_SlopeValue);
//		pRightSlope->DeleteRepeatData(pRightSlope->m_SlopeValue);
//	}
//
//
//	for (int i=0;i<pLeftSlope->m_SlopeValue.GetSize();i++)
//	{
//		pLeftSlope->m_SlopeValue[i].NameOfWay = "��ͨ��ƻ���";
//	}
//
//	for (int i=0;i<pRightSlope->m_SlopeValue.GetSize();i++)
//	{
//		pRightSlope->m_SlopeValue[i].NameOfWay = "��ͨ��Ƣ���";
//	}
//
//
//
//}


//void XLDataBase::GetSlopeData(CString NameOfMDB,GTZX *pGTZX,CDrawSlope * pLeftSlope,CDrawSlope * pRightSlope,CString XLName)
//{
//	GetSlopeData_New(NameOfMDB,pGTZX,pLeftSlope,pRightSlope,XLName);
//	return;
//	//1.����ͨ��ƻ��ߵ��¶ȣ�������ͨ��ƻ���ָ��
//
//	CString strNameOfWay1="��·��=\""+XLName+"\"";
//
//	if(Read_XLDbs(NameOfMDB,"�¶ȱ�",strNameOfWay1))
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
//    XLName="��ͨ��ƻ���";
//	//2.���Ľ��¶�
//	CString strNameOfDel="(�Ľ�����=\"";
//	strNameOfDel += "���еȸ߸���\"" ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp="�Ľ�����=\"";
//	strTemp += "��ͨ��ƻ��߸���\"" ;
//	strNameOfDel += strTemp ;
//
//	strNameOfDel += " OR " ;
//	CString strTemp1="�Ľ�����=\"";
//	strTemp1 += "��ͨ��ƶ��߸���\")" ;
//	strNameOfDel += strTemp1 ;
//
//	strNameOfDel += " AND " ;
//	CString  strNameOfWay="�Ľ���=\""+ XLName +"\"";;
//	strNameOfDel += strNameOfWay ;
//
//	CArray<CString,CString>strZPGJ;
//	CString strXLName;
//	if(Read_XLDbs(NameOfMDB,"�Ľ��λ��ܱ�",strNameOfDel))//���Ľ�������
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
//		strNameOfWay="��·��=\""+strNameOfWay+"\"";
//		Read_XLDbs(NameOfMDB,"�¶ȱ�",strNameOfWay);
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
//	//3.�����������¶�
//	for (int i=0;i<m_MPD.GetSize();i++)
//	{
//		m_APD.RemoveAll();
//		m_APD = m_MPD.GetAt(i);
//
//
//		//3.1�ҵ�Ҫ�滻����ʼ�¶Ⱥ�
//
//		double dtmp1 = m_APD[0].ml;
//		double dtmp2 = m_APD[m_APD.GetSize()-1].ml;
//
//
//
//		int StNum=pLeftSlope->FindSlopeNum(m_APD[0].ml);
//		int EdNum=pLeftSlope->FindSlopeNum(m_APD[m_APD.GetSize()-1].ml);
//		int flag;
//		if (m_APD[1].Notes=="���еȸ߸���")
//		{
//			flag=0;
//		}
//		else if (m_APD[1].Notes=="��ͨ��ƻ��߸���")
//		{
//			flag=1;
//		}
//		else 
//		{
//			flag=2;
//		}
//
//		
//		//3.2.remove StNum<Num<EdNum+1 �ı��µ�aa
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
//			//Zhujiang-���滻һ���¶�
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
//		//3.3.add m_APD��0~Size-1�ı��µ�
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
//		//3.4���������
//		pLeftSlope->Ordel(pLeftSlope->m_SlopeValue);
//		pRightSlope->Ordel(pRightSlope->m_SlopeValue);
//
//
//		//Zhujiang-�޳��ظ����¶����� 20090618
//		pLeftSlope->DeleteRepeatData(pLeftSlope->m_SlopeValue);
//		pRightSlope->DeleteRepeatData(pRightSlope->m_SlopeValue);
//	}
//
//
//	for (int i=0;i<pLeftSlope->m_SlopeValue.GetSize();i++)
//	{
//		pLeftSlope->m_SlopeValue[i].NameOfWay = "��ͨ��ƻ���";
//	}
//
//	for (int i=0;i<pRightSlope->m_SlopeValue.GetSize();i++)
//	{
//		pRightSlope->m_SlopeValue[i].NameOfWay = "��ͨ��Ƣ���";
//	}
//
//
//
//}


void XLDataBase_JYX::GetRxdXLNameArray(CString mdbFname, bool IsJY, int ZorY, CArray<CString, CString>& RxdXLNameArray)//��ȡ���ж���·������
{
	CString strTemp;
	CString strNameOfWay;
	CString SQLNameOfWay;
	if (IsJY)
	{
		if (ZorY == -1)
			strNameOfWay = "��������";
		else
			strNameOfWay = "��������";
	}
	else
		strNameOfWay = "��ͨ��Ƣ���";

	SQLNameOfWay = L"��·��=\"" + strNameOfWay + "\"";

	if (Read_XLDbs(mdbFname, "���߶�����", SQLNameOfWay))//���Ľ�������
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

void XLDataBase_JYX::GetRxdStartAndEndCKMLByRXDName(CString mdbFname, bool IsJY, int ZorY, CString RxdXLName, CString& StartCKML, CString& EndCKML)//��ȡ���ж������ֳ����
{
	CString strTemp;
	CString strNameOfWay;
	CString SQLNameOfWay;
	if (IsJY)
	{
		if (ZorY == -1)
			strNameOfWay = "��������";
		else
			strNameOfWay = "��������";
	}
	else
		strNameOfWay = "��ͨ��Ƣ���";

	SQLNameOfWay = L"��·��=\"" + strNameOfWay + "\"";
	SQLNameOfWay += L" and ";
	SQLNameOfWay += L"���ж���=\"";
	SQLNameOfWay += RxdXLName;
	SQLNameOfWay += "\"";

	if (Read_XLDbs(mdbFname, "���߶�����", SQLNameOfWay))//���Ľ�������
	{
		CString str;
		str.Format(L"%lf", DLB2x[0].ELC);
		LCChr(DLB2x[0].EGH, str, 3, StartCKML);

		str.Format(L"%lf", DLB2x[NDL2x - 1].BLC);
		LCChr(DLB2x[NDL2x - 1].BGH, str, 3, EndCKML);
	}
}

//////////////////////////////////////////////////////////////////////////
//��ȡ�õ������
int XLDataBase_JYX::Read_YD_XLDbs(CString mdbFname, CString TabName, CString XLName)//��ȡ��·���ݿ��е��õ�����
{
	CString rcd;
	AfxOleInit();

	if (AttachYDMdb(mdbFname, TabName, XLName) == 0)
		return 0;

	if (TabName == "�õ�������ֶܷα�")
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

			Report_input.GetValueString(pXZYD[i].GH, "�ֶ�����̹ں�");
			Report_input.GetValueString(rcd, "�ֶ��������");
			pXZYD[i].dml = _wtof(rcd);
			Report_input.GetValueString(pXZYD[i].BZText, "��ע");
			Report_input.MoveNext();
			i++;
		}
	}
	else if (TabName == "�õ����ͱ�")
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
			Report_input.GetValueString(pYDType[i].GH, "�ֶ�����̹ں�");
			Report_input.GetValueString(rcd, "�ֶ��������");
			pYDType[i].dml = _wtof(rcd);
			Report_input.GetValueString(pYDType[i].YDType, "�õ�����");
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
		// �򿪱���Access��
#ifdef X64
		CString strConnect = L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + mdbFname;
#else
		CString strConnect = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + mdbFname;
#endif // X64
		//strConnect+="���ݿ�.mdb";
		//m_adoConnection.Disconnect();
		if (!m_adoConnection.Connect(strConnect))
		{
			AfxMessageBox(L"ʧ��   6!");
			AfxMessageBox(L"�������ݿ�ʧ��!");
			return 0;
		}
		//m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=���ݿ�.mdb","","",adModeUnknown);		
	}
	catch (_com_error e)
	{
		AfxMessageBox(L"���ݿ�����ʧ�ܣ�ȷ��Pm.mdb�Ƿ��ڵ�ǰ·����!");
	}

	m_strSQL = L"select * from " + TabName;
	m_strSQL += " where ��·��=\"";
	m_strSQL += XLName;
	m_strSQL += "\"";

	if (!m_adoConnection.IsOpen())
	{
		AfxMessageBox(L"���ݿ�û�д򿪻��Ѿ��ر�!");
		return 0;
	}
	Report_input.SetAdoConnection(&m_adoConnection);
	if (Report_input.Open(m_strSQL, adCmdText) != 1)
	{
		//        ads_printf(L"%sû�м�¼!\n",TabName);		
		Report_input.Close();
		m_adoConnection.Disconnect();
		return 0;
	}
	return 1;

}

CString XLDataBase_JYX::RXDInWhichXL(CString mdbFname, CString RXDName)//���ж��ҵ���ĳ����·��
{
	CString XLName = "";
	CString SQLNameOfWay = L"���ж���=\"" + RXDName + "\"";
	CString strTemp = "";
	if (Read_XLDbs(mdbFname, "���߶�����", SQLNameOfWay))//���Ľ�������
	{
		for (int i = 0; i < NDL2x; i++)
		{
			strTemp.Format(L"%s", DLB2x[i].XLName);
			return strTemp;
		}
	}

	return XLName;
}

int XLDataBase_JYX::GetRXDChaQi(CString mdbFname, CString strEXName, CString RXDName, CString TabFname)//�����߲��������Ƿ������ж��ڣ��޳����ڵġ�
{
	//1.��������ָ�룬�õ��������ϵͳ
	//if(strEXName=="")
	GTZX2_JYX_ROAD* p2x = PmPointer::CreateGTSJEXPointer(mdbFname);
	if (p2x == NULL)
	{
		return 0;
	}
	//GTZX *pGTZX = p2x->CreateGtzx();

	//2.�õ����жε����յ�ͳһ���
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


	//�콭 20090605�޸�,�����������жα�ţ�����ܵ����������ͳһ����Ǹ���ֵ������������������
	double eml = p2x->TYLC2x(Ckml);
	//double eml=p2x->TYLC2x(Ckml,RXDNum);

	//3.�õ������ͳһ���,���ж��Ƿ������жη�Χ��,�������

	if (TabFname == "������")
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
			Temp.SWHt.Format(L"%s", Bri[i].SWHt);//����ˮλ
			Temp.KKSY.Format(L"%s", Bri[i].KKSY);//�׿���ʽ
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
				Bri[i].SWHt.Format(L"%s", TempBri[i].SWHt);//����ˮλ
				Bri[i].KKSY.Format(L"%s", TempBri[i].KKSY);//�׿���ʽ
				Bri[i].GJZT = TempBri[i].GJZT;
				Bri[i].CGH = TempBri[i].CGH;
				Bri[i].CenDml = TempBri[i].CenDml;
				Bri[i].BriLen = TempBri[i].BriLen;
				_tcscpy(Bri[i].CKml, TempBri[i].CKml);
			}
		}

	}
	else if (TabFname == "���")
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
			if (TempTUNTAB.flag == " " || TempTUNTAB.flag == "NULL")//���û��¼Ĭ��Ϊ1-һ�����
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
	else if (TabFname == "С�ź�")
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
	else if (TabFname == "��վ")
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
�����ݿ����߱��ж���·������߻��Ǽ����ߣ�������ֱ�ӷ���isEX=-1
���������ߣ�����ͨ���������߱������߱��ж��ǲ��Ƕ���
��ƻ��߷���isEX=0 ���߷���isEX=1 ���߷���isEX=2
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
	//	AfxMessageBox(L"��ָ������Ŀ¼�ͷ�������!");
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
	// 		AfxMessageBox(L"�������ݿ�"+ProMDB+"ʧ��!");
	// 
	// 	}
	// 	Report_input.SetAdoConnection(&m_adoConnection);
	// 	InitTableList();
	// 	Temp1 = "��ͨ��Ƣ���";
	// 	Temp1.Trim();
	// 	Temp1="��·��=\"" + Temp1 + "\"";
	// 	if (Read_XLDbs(FilePath,"�������߱�",Temp1))
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