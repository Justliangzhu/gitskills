// XLDataBase.h: interface for the XLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
#define AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ado.h"
#include "bas_draw_fun.h"
#include "Struct.h"
#include <vector>

class XLDataBase  : public BAS_DRAW_FUN
{
public:
	XLDataBase();
	virtual ~XLDataBase();
    
	ModifyRoadTab* pModifyRoad;
	int ModifyRoadSum;

	DoubleLineData *QXB2x;//�������߱�
	int NQX2x;//����������Ŀ

	EXDLB *DLB2x;//���߶�����
	int NDL2x;//������

	YongDiLXB* YDLXB;//�õ����ͱ�
	int YDLXSum;//�õ����ͷֶ�����

	QXTAB *QXB;//���߱�
	int NQX;

	CTRLHT *pCtrlHt;
	int CRNUM;
	
	DXSTRUCT *DX;
	int DXNum;
	
	BMSTRUCT *BM;
	int BmNum;
	
	XLManager *XLMan;//��·����
	int NXL;

	JdCenter *JdCenArray;
	int NJdCen;
	
	DLBZ *DLArray;
	int NDL;

	DLBZ *ZDLArray;
	int NZDL;
	
	ZQZJTAB *ZQZJB;

	TUNTAB *Tun;
	int TunNum;
	
	BRITAB *Bri;
	int BriNum;

	PDTAB *PDB;
	int NPD;

	JPInfor *JPB;
	int JPnum;

	TWTAB *TWB;//�ݵ����߱�
	int NTW;

	Culvet *CUL;
	int NCU;
	
	double (*XYArray)[10];//  	
	int XYNum;//��Ԫ��

	struct DMXCS{
		double ZJ;//������ 0 Ϊ����
		double sdml;
		double edml;//�������
		int Color;//��õ����ߵ���ɫ
	} DmxCsArr[50];
	int NDMX;
	double Scml,Ecml;//�ݶ���ͼ�л��Ʊ��ߵ������������
	double Hmin,Hmax;
	int Color;//�ݶ���ͼ�л��Ʊ��ߵ����ߵ���ɫ
	double ZJ;

	RoadProperty RoadXZ;//��·����
	ZdmDesStandard ZDMBZ;//�ݶ�����Ʊ�׼
	CGJSCS CalCGCs;

	LMKdata *lmk;
	int NLMK;

	PPCDdata *ppcd;
	int NPPCD;

    CgSZ CG[500];
	int NCG;
	
	LCBdata *STA;
	int NSTA;

	//20190808	���������
	//�ɷ�ѹʵϵ��
	double YSXS[6];
	//�����
	std::vector<LDTLdata> m_FillType;
	//�õ�������ֶܷ�
	std::vector<YongDiFDB> m_EarthSection;
	//�ڷ�����
	std::vector<WFFLdata> m_CutType;
	//��������·����ֱ
	std::vector<GLDMLZdata> m_GLDMLZ;
	//��������·ǵ������ƽ
	std::vector<GLDMLQBPTPdata> m_GLDMLQBPTP;
	//���������߼乵
	std::vector<GLDMXJGdata> m_GLDMXJG;
	//��������
	std::vector<hthddata> m_HTHD;
	//������
	std::vector<GouPDdata> m_GouPD;
	//������
	std::vector<ctkddata> m_CTKD;
	//���α���
	std::vector<arcbpata> m_ARCBP;
	//��ǽ
	std::vector<dqdata> m_DQ;
	//����·���Զ���ƽ
	std::vector<LDTPData> m_LDTP;
	//����·ǵ�Զ���ƽ
	std::vector<LQTPData> m_LQTP;
	//����·���Զ��ӿ���·��
	std::vector<TLJJKData> m_TLJJK;
	//·����
	std::vector<LMHdata> m_LMHD;
	//�������ڲ��
	std::vector<crswiddata> m_CRSWID;
	//��������ƿ��
	std::vector<dmdata> m_DM;
	//�õؼӿ�
	std::vector<pwddata> m_PWD;
	//·�̱���
	std::vector<LDBPdata> m_LDBP;
	//·ǵ����
	std::vector<LDBPdata> m_LQBP;
	//��ˮ��
	std::vector<LDSGdata> m_LDSG;
	//�๵
	std::vector<LQSGdata> m_LQSG;
	//�칵
	std::vector<TGdata> m_TG;

	//20190918	���Ƶ���
	std::vector<GCKZPt> m_LevelCtrlPtArray;

	std::vector<JPInfor>m_JPArray;
	//20191017	·��ֲ�
	std::vector<RoadLayerData> m_RoadLayerArray;

	//20191030	����������Ϣ
	std::vector<ConstraintErr> m_RoadNRailInterInforArray;
	
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
	CStringArray m_strTableNameList;

	void Release();
	int  IsMdb2(CString MdbName, int &isEX);
	int ReadAllXZ(CString mdbFname,CString RoadName);//��ȡ��·���ݿ����¶ȱ������
    int Read_XLDbs(CString mdbFname,CString TabFname);//��ȡ��·���ݿ��е�����
	int Read_XLDbs_Railway(CString mdbFname, CString TabFname);	//20191029����	��ȡ��·���ݿ�������
//	int Write_XLDbs(CString mdbFname,CString TabFname);//д����·���ݿ��е�����
	bool InitTableList(void);//ȡ��mdb�е����б���
	bool FindTable(CString strTempName);//���ұ��Ƿ������mdb��

	void GetAllXLName(CStringArray *XLNameArray);
	bool GetiFLDDLInfo(int iFLD,int &sDLNo,int &eDLNo);
	int Read_EX_XLDbs(CString mdbFname,CString TabFname,int irxd);//��ȡ��·���ݿ��еĶ�������
	int Read_XLDbs(CString mdbFname,CString TabFname,CString RoadName);//��ȡ��·���ݿ��е�����
	int Read_XLDbs(CString mdbFname,CString TabFname,CString RoadName,CString XZ);
	int Write_XLDbs(CString mdbFname,CString TabFname,CString RoadName);//д����·���ݿ��е�����
    int Write_XLDbs(CString mdbFname,CString TabFname,CString RoadName,CString XZ);//д����·���ݿ��е�����
	int ReadAllRoadName();
	int m_RoadSum;//��·��
	struct RoadData
	{
       CString RoadName;
	   CString BGH,EGH;
	   double BLC,ELC;
	} Road[50];//���50��·

	CString RoadZ,RoadY;		

	double SuphData[10][2];
	int NuphData;
	double JKData[10][2];
	int NJK;

	bool PutRnameInArray(CString rname);//��ȡ��·���ݿ��ж����������
	int SetRoadSEml(CString mdbFname,CString RoadName,int iRoad);
	int ReadAllRoadName(CString mdbFname);//��ȡ��·���ݿ��ж����������
	void SortRoad();
	int DelAllRecords(CString mdbFname,CString TabFname);//ɾ���������м�¼
	int AttachMdb(CString HdmMdbname,CString RoadName,CString TabName);
};

#endif // !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
