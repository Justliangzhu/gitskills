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
		CString Type;//��·����//���жΡ����в��ȸ߶�
		ACHAR StCKml[80];//����ֳ����
		ACHAR EdCKml[80];//�յ��ֳ����

		ACHAR XLName[50];//��·��SJ2X,JY2X
		ACHAR DLName[100];//���ж��� //���ж�127.7,���в��ȸ߶�135.5
		ACHAR BLName[50];//����ƽ����


		double spt[2];//������� spt[0]--N spt[1]--E
		double ept[2];//�յ�����

		double BCml;//�������ƶ����ϵ�ͳһ���(cml)
		double ECml;//�յ�����ƶ����ϵ�ͳһ���

		double BL_BCml;//�������ƻ����ϵ�ͳһ���(cml)�����=-100˵����ƶ����������
		double BL_ECml;//�յ�����ƻ����ϵ�ͳһ���(cml)�����=-100˵����ƶ����յ�����

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


	
	RAILSTU *GDSTRU;//����ṹ
	int	NGDSTRU;

	BasicDataOfZDM *pBasicDataOfZDM;//����������
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

	DMXTAB *DMXB;//�����߱�
	int	NDMX;

	DMXTAB *JiYouLuJianGC;//����·��̱߳�
	int	NJiYouLuJianGC;

	GMXTAB *GMXB;//�����߱�
	int NGMX;

	RoadBedTAB *RoadBed;//������
	int RoadBedNum;

	RoadBedTAB *ZMControlPT;//������Ƶ��
	int ZMControlPTNum;

	QXTAB  *QXB;//���߱�
	int	NQX;

	TWTAB *TWB;//���ڱ�
	int	NTW;

	HdmAreaData	*HDMB;//���������
	int	NHDM;

	STATAB *Sta;
	int	StaNum;

	SMALLBRIDGETAB *SBri;
	int	SBriNum;

	DoubleLineData *QXB2x;
	int	NQX2x;

	EXDLB *DLB2x;//���߶�����
	int	NDL2x;//������

	QXJKTAB *QXJKB;//���߼ӿ��
	int NQXJK;

	JiYouXianZhanDiKuan *ZuoJiYouXianZhanDiKuan;//�������ռ�ؿ�
	int NZuoJiYouXianZhanDiKuan;
	
	JiYouXianZhanDiKuan *YouJiYouXianZhanDiKuan;//�Ҽ�����ռ�ؿ�
	int NYouJiYouXianZhanDiKuan;;

	int NYDLX;//�õ����ͷֶ�����
	YongDiLXB * LXB;//�õ����ͱ�
	///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////
	//������
	BoLiangTable *JYBoLiangTable; //������
	int	NBLNum;

	//JDDataTable	*JYJDDataTable;	//�������ݱ�
	//int	NJDDateNum;

	OriginalMeasureTable *JYOrigMeaTable; //��������ԭʼ��������
	int	NOrigMeaNum;

	MulFDData	*MulFDTable;  //�����߷ֶ���Ϣ��
	int	MulFDNum;

	BZLCStructTable	 *BZLCTable;  //��־���
	int	BZLCNum;

	GJDInf *pGjdInf;
	int NGjdInf;

	EXGJDFDInf *pEXGJDFDInf; //2�߸Ľ��ηֶ���Ϣ��
	int NEXGJDFDInf;

	ConnectLineTableStruct *ConnectLineTable;  //�����߱�
	int ConnectLineInforNum;

	SGBXTableStruct *SGBXTable; 
	int SGBXInforNum;


	JBJXTableStruct *JBJXTable; 
	int JBJXInforNum;

	GJDAllInforStruct *GJDAllInforTable;  //�Ľ��λ��ܱ�
	int GJDAllInforNum;

	OneGJDJDStruct *OneGJDJDarray;//�Ľ��μ��β�����
	int OneGJDNum;

	//������������þ���������
	SZJDOrCD   *AllZJDOrCDArray;  //����������þ�������
	int AllZJDOrCDNum;

	STATIONSTRUCT *STATION;
	int StaTionNum;

	BMSTRUCT *BM;
	int BmNum;

	DXSTRUCT *DX;
	int DXNum;

	QXBStruct *SJQXB;  //������߽����
	int NSJQXB;

	BCTAB *BC;//
	int	NBC;


	//���ݿ�������׮����͸���
	WaiYiZhuang *m_pWYZArr;
	int m_iNWYZ;


	//���ݿ�������̨������͸���
	ZMTaiZhang *m_pZMTZArr;
	int m_iNZMTZ;

	//���ݿ���������������͸���
	XingZhengQuHua *m_pXZQHArr;
	int m_iXZQH;



	//////////////////////////////////////////////////////////////////////////
	//�õ����
	CXZYDFD *pXZYD;//�����õطֶ�
	int NXZYD;//�����õطֶ���Ŀ

	CYDLXFD *pYDType;//�õ����ͷֶ�
	int NTypeFD;//�õ����ͷֶ���Ŀ
	//////////////////////////////////////////////////////////////////////////

	FZZDM* FZZDMArray;//�����ݶ�������	20191113����
	int FZZDMNum;

	ZYJSBZTAB JSBZ;//������׼
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
	CStringArray m_strTableNameList;

	int  IsJYXMdb2(CString MdbName, int &isEX);
	CString isNULLToCSting(CString strTempName);
	int	Read_XLDbs(CString mdbFname,CString	TabFname,CString wlcstring );//��ȡ��·���ݿ��е�����
	int	WR_XLDbs(CString mdbFname,CString TabFname,CString wlcstring);//д����·���ݿ�
	bool InitTableList(void);//ȡ��mdb�е����б���
	bool FindTable(CString strTempName);//���ұ��Ƿ������mdb��
	int	WR_EX_XLDbs(CString	mdbFname,CString TabFname,int irxd);//д����·���ݿ��е���
	int	Read_EX_XLDbs(CString mdbFname,CString TabFname,int	irxd);//��ȡ��·���ݿ��еĶ�������

	void Realease();
	int	Delete_XLDbs(CString mdbFname,CString TabFname,CString wlcstring);//ɾ����·���ݿ�
	void GetJDNumArrayFromBLCalTab(CString mdbFname,CString TabFname,CArray<CString,CString> &  GetJDNumArray);
	void GetJDNumArray(CAdoRecordSet m_RecSet, CArray<CString,CString> &a );
	double FromCStringTodouble(CString strText);

	int WR_EX_TWB(CString mdbFname,CString TabFname,int irxd);//д����·���ݿ��е�����

//	void GetSlopeData(CString NameOfMDB,GTZX_JYX *pGTZX,CDrawSlope *pLeftSlope,CDrawSlope *pRightSlope,CString XLName="��ͨ��ƻ���");

	void GetRxdXLNameArray(CString mdbFname, bool IsJY, int ZorY, CArray<CString,CString>& RxdXLNameArray);//��ȡ���ж���·������

	//��ȡ���ж������ֳ����
	void GetRxdStartAndEndCKMLByRXDName(CString mdbFname, bool IsJY, int ZorY, CString RxdXLName, CString& StartCKML, CString& EndCKML);

	//��ȡ��·���ݿ��е��õ�����
	int Read_YD_XLDbs(CString mdbFname,CString TabName,CString XLName);

	int AttachYDMdb(CString mdbFname,CString TabName,CString XLName);

	CString RXDInWhichXL(CString mdbFname,CString RXDName);

	int GetRXDChaQi(CString mdbFname,CString strEXName,CString RXDName,CString TabFname);	//�����߲��������Ƿ������ж��ڣ��޳����ڵġ�

};

#endif // !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
