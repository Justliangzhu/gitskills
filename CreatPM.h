#pragma once
#include "RHA.h"
#include "PMSJ.h"
#include "DocManage.h"

class CCreatPM 
{
public:
	CCreatPM(void);
	~CCreatPM(void);

	//������·ƽ���Զ���ʵ�壨ȫ�ֻ�ֲ���
	static RHA* CreatRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs=true);

	//����ȫ�ֵ���·�Զ���ʵ�壬���������л��ߡ����ж��ߡ���ƻ��ߡ���ƶ���
	static RHA* CreatGlobalRHA(CString mdbName, CString xlName, bool isAddtoDbs=true);

	//���ֲ�����·ƽ��ʵ��ָ�룬��������ʱ�ߡ�ʩ������
	static RHA* CreatPartRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs=true);

	//������·���Ժ���·���޸���·��eg.�����л��߸�Ϊ��ƻ���
	static RHA* ChgXLByXLAttrName(RHA *pRHA, CString xlShuXing, CString xlName);

	//ֱ��ת��·ʵ��
	static void PLinetoRHA(CString XLNameToIn , AcDbObjectId eIdToIn , CString xlShuXing, CString mdbName=L"");

	//��·�����ռ����ߡ����������
	static void SortXLNames(CStringArray &xlNameAry);

	//ȫ����·�������Ƿ��������·����������������������е��±�,���򷵻�-1
	static int IsIncluded(int iDoc, CString mdbName, CString xlName);

	//ȫ����·�������Ƿ��������·����������������ڴ����������·�����е��±�
	static bool IsIncluded(AcApDocument *doc, CString mdbName, CString xlName, int &iDoc, int &iXL);

	//ȫ����·�������Ƿ��������·����������������ڴ����������·�����е��±�
	static bool IsIncluded(CString mdbName, CString xlName , int &iDoc, int &iXL);

	//ȫ����·�������Ƿ��������·�������û�о����ɸ���·��������������������˸���·����true, ��֮����false�����ixΪ����·��ȫ�������е��±�
	static bool IsGlobalXLHaveAndCreat( CString mdbName, CString xlName, int &ix, bool isAddtoDbs=false);
	
	//ȫ����·�������Ƿ��������·��,����������������ɸ���·ָ�룬���ظ�ָ��
	static RHA* IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, bool isAddtoDbs=false);

	//��ȡ��ǰ����,���������g_DocManageAry�е��±�
	static bool CurDocIndx(int &iDoc);

	//�����·��ȫ���������Ѿ�����,��ɾ��ͼ�Σ�����·����
	static bool NullRHA(int iDoc, int iXL) ;

	//�����·��ȫ���������Ѿ�����,��ɾ��ͼ�Σ�ɾ����·
	static bool DelRHA(int iDoc, int iXL);

	//��ȡ�����ݿ���xlName֮ǰ��������·��
	static void GetAllXLNames(CString mdbName, CString xlName, CStringArray &allXLNameAry);

	//�Ƚ�������·�������ȼ�
	static bool CmpXLPriority(CString xlName1, CString xlName2);

	//�������еļ����ߺ�����ߵ�ʵ��
	static void CreatAllJYXandSJX(CString mdbName, int iDoc=-1);

	//�ͷ�ƽ����·ָ�루ȫ�֣�
	static void CCreatPM::FreeRHA(RHA* pRHA);

	//�ӵ�ǰ������ ��ȡ��ϵͳ��ȫ�������� ĳ���ݿ� ��ȫ����·��Ϣ
	static void CCreatPM::GetSysXLInfbyMdbName(CString mdbName, CArray<XLInforTableStruct, XLInforTableStruct> &xlInfAry);

	



};
