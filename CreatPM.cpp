#include "StdAfx.h"
#include "CreatPM.h"
#include "XLDataBase.h"
#include "PaletteSet.h"
#include "Palette.h"
#include "BCCreateXLEntity.h"

extern CMyPaletteSet  *pPaletteSet;
extern CMyPalette *pPalette1;
extern void LoadPalette(CMyPalette *pPalette1);
extern void LoadPaletteSet(CMyPaletteSet *pPaletteSet);
extern GuiFanCanShu mGJGuiFanCS;
extern GuiFanCanShu mJYGuiFanCS;

//vector<RHA*> g_DocManageAry[iDoc].XLAry;//�����ߺ�����ߵ�ȫ������
vector<CDocManage> g_DocManageAry; 

//nan
#include "string"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

CCreatPM::CCreatPM(void)
{
}

CCreatPM::~CCreatPM(void)
{
}

/*************************************************************
��������: ������·ƽ���Զ���ʵ�壨ȫ�ֻ�ֲ���
�������:
mdbName     			���ݿ���������·����
xlShuXing    			��·���ԣ���ʱ�ߡ�ʩ������
xlName                  ��·��
isAddtoDbs              �Ƿ���ͼ��            
��������ֵ:��·ƽ��ʵ��ָ�� 
��д����˧ 2016
***************************************************************/
RHA* CCreatPM::CreatRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs)
{
	if (xlShuXing == SGBX || xlShuXing == LSX)
		return CreatPartRHA(mdbName, xlShuXing, xlName, isAddtoDbs);//������ʱ�ߡ�ʩ������
	else
		return CreatGlobalRHA(mdbName, xlName, isAddtoDbs);//�������л��ߡ����ж��ߡ���ƻ��ߡ���ƶ���

}

/*************************************************************
��������: �����ֲ�����·ƽ��ʵ��ָ�룬��������ʱ�ߡ�ʩ������
�������:
mdbName     			���ݿ���������·����
xlShuXing    			��·���ԣ���ʱ�ߡ�ʩ������
xlName                  ��·��
isAddtoDbs              �Ƿ���ͼ��            
��������ֵ:�ֲ�����·ƽ��ʵ��ָ�� 
��д����˧ 2016
***************************************************************/
RHA* CCreatPM::CreatPartRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs)
{
	CString temp;
	XLDataBase xlmdb;
	RHA *pRHA=NULL;

	xlName.Trim();	
	temp = L"��·��=\"" + xlName + L"\"";
	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"�������ݱ�", temp))//������
	{

		pRHA = new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD"
			, xlName, mdbName, xlShuXing);
	}
	else
	{
		ads_printf(L"���������ݱ�ʧ��!\n");
		return NULL;
	}

	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"������", temp))//������
	{
		if(xlmdb.NDL>1)
		{
			pRHA->DLarray.clear();
			pRHA->DLarray.resize(xlmdb.NDL);
			for(int k=0;k<xlmdb.NDL;k++)
				pRHA->DLarray[k]=xlmdb.DLB[k];
		}
	}
	else
	{
		ads_printf(L"��������ʧ��!\n");
	}

	//�Զ����ɱ�־���
	pRHA->GenBZLCbyDL();

	//��־��̱�
	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"��־��̱�", temp))
	{

		if (xlmdb.BZLCNum>0)
		{
			//�����û��ı�־���
			for (int k=0;k<xlmdb.BZLCNum;k++)
				pRHA->BZLCarray.push_back(xlmdb.BZLCTable[k]);
			//�����־�����Ϣ
			pRHA->CalBZLC();
		}

	}

	//��ͼ
	pRHA->setColorLineW();
	if(isAddtoDbs)
		BAS_DRAW_FUN::AddEntityToDbs(pRHA);

	
	return pRHA;
}


/*************************************************************
��������: �ͷ�ƽ����·ָ�루ȫ�֣�
�������: pRHA     	ƽ��ָ��
�㷨�����������·��ȫ�������У���û����CADͼ�л����������ͷŸ�ָ��
��Ȼ������ݿ��������ɵ���·��ֵ��ָ��.
��������ֵ: 
��ע��
��д����˧ 2017.6
***************************************************************/
void CCreatPM::FreeRHA(RHA* pRHA)
{
	if (pRHA)
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

		if (Acad::eOk == acdbOpenObject(pRHA, pRHA->id(), AcDb::kForWrite))
		{
			pRHA->close();
		}
		else
		{//û����CADͼ�л�����
			delete []pRHA;
			pRHA=NULL;
		}

		acDocManager->unlockDocument(acDocManager->curDocument());
	}
}

	
	
//818 ���˴��ڴ������Ǹô��ڴ������ٶ��ô���
/*************************************************************
��������: ����ȫ�ֵ���·�Զ���ʵ�壬���������л��ߡ����ж��ߡ���ƻ��ߡ���ƶ���
�������:
mdbName     			���ݿ���������·����
xlName                  ��·��
isAddtoDbs              �Ƿ���ͼ��            
��������ֵ:ȫ����·ƽ��ʵ��ָ�� 
��ע�������·��ȫ�������У���û����CADͼ�л����������ͷŸ�ָ��
	  ��Ȼ������ݿ��������ɵ���·��ֵ��ָ��
��д����˧ 2016
***************************************************************/
RHA* CCreatPM::CreatGlobalRHA(CString mdbName, CString xlName, bool isAddtoDbs)//818 �������ֻ��ͨ��IsGlobalXLHaveAndCreat���� �Ժ�
{
	CString temp;
	CStringArray newXLNameAry;
	AcApDocument *doc=acDocManager->curDocument();
	int iDoc=-1,j=-1;
	RHA *pRHA=NULL;
	XLDataBase xlmdb1;

	//��ȡ��ǰ����
	if(!CurDocIndx(iDoc))
		return NULL;
	
	CString XLShuxing = xlmdb1.GetXLAttriStr(xlName);
	if (XLShuxing == SGBX || XLShuxing == LSX)
	{
		temp.Trim();
		temp = L"��·��=\"" + xlName + L"\"";
		if(xlmdb1.Read_XLDbs(mdbName, XLShuxing + L"�������ݱ�", temp))//������
		{
			//j=IsIncluded(iDoc, mdbName, xlName);
			//if (j<0)
			//{
				//��·û�й�����������·
				j=g_DocManageAry[iDoc].XLAry.size();
				pRHA = new RHA(xlmdb1.NJD, xlmdb1.JDB, L"AK", L"JD"
					, xlName, mdbName);

				if(xlmdb1.Read_XLDbs(mdbName, XLShuxing + L"������", temp))//������
				{
					if(xlmdb1.NDL>1)
					{
						pRHA->DLarray.clear();
						pRHA->DLarray.resize(xlmdb1.NDL);
						for(int k=0;k<xlmdb1.NDL;k++)
							pRHA->DLarray[k]=xlmdb1.DLB[k];
					}
				}
				else
				{
					ads_printf(L"��������ʧ��!\n");
					FreeRHA(pRHA);
				}

				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
				pRHA->m_iDoc=iDoc;
				pRHA->setColorLineW();
			//}
			//else
			//	pRHA = g_DocManageAry[iDoc].XLAry[j];
		}
		else
		{
			ads_printf(xlName+L"�������ݱ�ȱʧ��������·ʧ��!\n");
			return NULL;
		}

		return pRHA;
	}


	//һ����ȡ����·��xlName�������л�׼����·��������m_JYXNames��m_SJXNames��
	newXLNameAry.RemoveAll();
	//�����������ƶ��ߣ�������֮ǰ������ƻ���Ϊ��ƶ������жΣ����棩�ļ����ṩ��ƻ���ָ��
	if(xlName.Find(L"���") >= 0 && xlName != SJJX)
		newXLNameAry.Add(SJJX);

	xlName.Trim();
	
	if (!xlmdb1.ReadBLNames(mdbName ,L"�������ݱ�", xlName, newXLNameAry))
		return NULL;

	
	//������ȡ��׼�ߵĻ�׼����·����ѭ�������ռ��л��ߡ����ж��ߡ�����ƻ��ߡ���ƶ�������
	int change=0;
	int len=0;
	int oldXLNum;
	vector<FZZDM> rxdAry;

	do 
	{
		change=0;
		len=newXLNameAry.GetSize();

		for (int i=0; i<len; i++)        //�������һ���洢����·�����ظ����洢��·����
		{			                    //�ٶ�����·�Ĳ����ߣ�ֱ���ҳ���������·��xlName����Ҫ��������صĲ�����·����
			
			if (newXLNameAry[i] != L"")
			{
				XLDataBase xlmdb2;

				oldXLNum=newXLNameAry.GetSize();
				xlmdb2.ReadBLNames(mdbName ,L"�������ݱ�", newXLNameAry[i], newXLNameAry);
				change += newXLNameAry.GetSize()-oldXLNum;
			}
		}

	} while (change);


	newXLNameAry.Add(xlName);
	SortXLNames(newXLNameAry);//818 ͨ�����չ�ϵ������ã�ע����ж�����ͬ�ȼ��ģ�����������
	int newXLNum=newXLNameAry.GetSize();

	//nan
	vector<CString> tmpNmAry;
	for(int i=0; i<newXLNum; i++)
		tmpNmAry.push_back(newXLNameAry[i]);


	//�������ι����µ���·RHAʵ��
	for(int i=0; i<newXLNum; i++)
	{
		XLDataBase xlmdb;//attention: ÿ��ѭ�������XLDataBaseָ�뿪�Ŀռ�
		pRHA=NULL;

		temp = newXLNameAry[i];
		temp.Trim();
		temp = L"��·��=\"" + temp + L"\"";
		if(xlmdb.Read_XLDbs(mdbName, L"�������ݱ�", temp))//������
		{
			j=IsIncluded(iDoc, mdbName, newXLNameAry[i]);
			if (j<0)
			{
				//��·û�й�����������·
				j=g_DocManageAry[iDoc].XLAry.size();
				pRHA = new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD", newXLNameAry[i], mdbName);
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
				pRHA->m_iDoc=iDoc;
				pRHA->setColorLineW();
			}
			else 
			{
				//�������·���У��������� 818 ���ǲ������ɣ�������������·��
				if (i == newXLNum-1)
				{//��Ҫ���ɵ���·���У������ݿ�����������

					//�ͷ�ָ��
					FreeRHA(g_DocManageAry[iDoc].XLAry[j]);
					//������·
					pRHA= new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD"
						, newXLNameAry[i], mdbName);					//��RHA�Ĺ��캯��������·�Ļ���
					g_DocManageAry[iDoc].XLAry[j] = pRHA;				//����g_DocManageAry������·����
				}
				else
					//���������У�����Ҫ������
					continue;
				
			}

		}
		else
		{
			ads_printf(newXLNameAry[i]+L"�������ݱ�ȱʧ��������·ʧ��!\n");
			return NULL;
		}

		if(xlmdb.Read_XLDbs(mdbName, L"������", temp))//������
		{
			if(xlmdb.NDL>1)
			{
				pRHA->DLarray.clear();
				pRHA->DLarray.resize(xlmdb.NDL);
				for(int k=0;k<xlmdb.NDL;k++)
					pRHA->DLarray[k]=xlmdb.DLB[k];
			}
		}
		else
		{
			ads_printf(L"��������ʧ��!\n");
		}

		//�γɹ�ͨ������ 818 ע���Ƿ��еط�û�øú������ɼ����߻�����ߣ������γɹ�ͨ������
		if (pRHA->XLShuXing == JY2X 
			|| pRHA->XLShuXing == SJJX 
			|| pRHA->XLShuXing == SJ2X)
		{
			//ɾ�����жζ���
			pRHA->EarseDLofBLonBXD();
			//�γɹ�ͨ������
			pRHA->Create2XGTDLB();
		}
					
		//818 ��ʱ�ߺ�ʩ�������б�־��̣�
		//��־��̱�
		if (pRHA->XLShuXing != SJJX && pRHA->XLShuXing != SJ2X)
		{
			if(xlmdb.Read_XLDbs(mdbName, L"��־��̱�", temp)/* && xlmdb.BZLCNum>0*/)
			{
				//�����û��ı�־���
				pRHA->BZLCarray.resize(xlmdb.BZLCNum);
				for (int k=0;k<xlmdb.BZLCNum;k++)
					pRHA->BZLCarray[k]=xlmdb.BZLCTable[k];

				//��Ӷ��������ı�־���
				pRHA->GenBZLCbyDL(false);

				//�����־�����Ϣ
				pRHA->CalBZLC();
			}
			else
				//�Զ����ɱ�־���
				pRHA->GenBZLCbyDL(true);
		}
		else
			pRHA->BZLCarray.clear();


		//�Ľ�����Ϣ
		pRHA->GJDInforArray.clear();
		if (pRHA->XLShuXing == JYJX 
			|| pRHA->XLShuXing == JY2X)
		{
			//�������γ����ö����� 818 �����߼�¼���ö�ʲôʱ���ã�
			pRHA->CreatLYDArrayOfJYX();
		}
		else if (pRHA->XLShuXing == SJJX 
			|| pRHA->XLShuXing == SJ2X)
		{
			//�γ�ȫ�ߵķֶ�����
			if(xlmdb.Read_XLDbs(mdbName, L"�Ľ��λ��ܱ�", temp))
			{

				if (xlmdb.GJDAllInforNum>0)
				{
					//��ӸĽ�����Ϣ //818 �ڸĽ��������γ�ʱ��Ӧ�����򣬲���
					pRHA->GJDInforArray.resize(xlmdb.GJDAllInforNum);
					for (int k=0;k<xlmdb.GJDAllInforNum;k++)
						pRHA->GJDInforArray[k]=xlmdb.GJDAllInforTable[k];
						

					//������Ľ������յ�ͶӰ���
					pRHA->GJDInforArray[0].BProjml=pRHA->QXB[0].KZH;

					for (int k=0;k<xlmdb.GJDAllInforNum;k++)
					{

						pRHA->GJDInforArray[k].EProjml=pRHA->PROJ_ML(pRHA->array, pRHA->tarray
							, pRHA->GJDInforArray[k].ept[0], pRHA->GJDInforArray[k].ept[1]
						, pRHA->GJDInforArray[k].ept[0], pRHA->GJDInforArray[k].ept[1]);

						if(k<xlmdb.GJDAllInforNum-1)
							pRHA->GJDInforArray[k+1].BProjml=pRHA->GJDInforArray[k].EProjml;

					}

					//������Ľ������յ�ͳһ���
					for (int k=0;k<xlmdb.GJDAllInforNum;k++)
					{
						pRHA->TrsCkmltoCml(xlmdb.GJDAllInforTable[k].BLC
							, pRHA->GJDInforArray[k].GJDSml);

						pRHA->TrsCkmltoCml(xlmdb.GJDAllInforTable[k].ELC
						, pRHA->GJDInforArray[k].GJDEml);
					}

				}

			}

		}

		////NAN
		//if (pRHA->XLShuXing == JYJX)
		//{
		//	CString t, k;
		//	t=pRHA->XLC(61900, pRHA->DLarray);
		//	k=pRHA->XLC(61300, pRHA->DLarray);
		//}
		//�������ж����飨��������ã�
		pRHA->FZZDMAry.clear();
		rxdAry.clear();

		if (pRHA->XLShuXing == JY2X
			|| pRHA->XLShuXing == SJ2X)
		{
			if(xlmdb.Read_XLDbs(mdbName, L"�����ݶ�������", temp))
			{
				if (xlmdb.FZZDMNum>0)
				{
					//������ж���Ϣ
					rxdAry.resize(xlmdb.FZZDMNum);
					for (int k=0;k<xlmdb.FZZDMNum;k++)
						rxdAry[k]=xlmdb.FZZDMArray[k];
				}
			}
		
			RHA *pJX_RHA=NULL;
			int n=-1;
			if (pRHA->XLShuXing == JY2X)
				n=IsIncluded(iDoc, mdbName, JYJX);
			else
				n=IsIncluded(iDoc, mdbName, SJJX);
			if (n>0)
				pJX_RHA=g_DocManageAry[iDoc].XLAry[n];
			else
				ads_printf(L"δ���ɻ���ƽ�棡\n");

			double stProjml1x, endProjml1x;
			int jdNum=pRHA->JDarray.size();
			double dist;

			for (int k=0;k<rxdAry.size();k++)
			{
				dist=100.0;
				//�������յ��ڱ����ϵ�ͳһ���
				pRHA->TrsCkmltoCml(rxdAry[k].StCKml, rxdAry[k].BCml);
				pRHA->TrsCkmltoCml(rxdAry[k].EdCKml, rxdAry[k].ECml);
				
				//�������յ�����ƻ����ϵ�ͶӰ���
				stProjml1x=pJX_RHA->PROJ_ML(pJX_RHA->array, pJX_RHA->tarray
					, rxdAry[k].spt[0], rxdAry[k].spt[1]
				, rxdAry[k].spt[0], rxdAry[k].spt[1]);

				endProjml1x=pJX_RHA->PROJ_ML(pJX_RHA->array, pJX_RHA->tarray
					, rxdAry[k].ept[0], rxdAry[k].ept[1]
				, rxdAry[k].ept[0], rxdAry[k].ept[1]);

				//������ж��Ƿ����յ�����
				if (k == 0)
					dist=sqrt(pow(rxdAry[k].spt[0]-pRHA->JDarray[0].N, 2)
					+pow(rxdAry[k].spt[1]-pRHA->JDarray[0].E, 2));
				else if(k+1 == rxdAry.size())
					dist=sqrt(pow(rxdAry[k].ept[0]-pRHA->JDarray[jdNum-1].N, 2)
					+pow(rxdAry[k].ept[1]-pRHA->JDarray[jdNum-1].E, 2));
				

				//�������յ�������ƽ���ϵ�ͳһ���
				if((k>0 && stProjml1x>=0 )|| (k == 0 && dist>0.5))
					rxdAry[k].BL_BCml=pJX_RHA->TrsProjmltoCml(stProjml1x);
				else
					rxdAry[k].BL_BCml=-100;

				if((k+1<rxdAry.size() && endProjml1x>=0 )|| (k+1 == rxdAry.size() && dist>0.5))
					rxdAry[k].BL_ECml=pJX_RHA->TrsProjmltoCml(endProjml1x);
				else
					rxdAry[k].BL_ECml=-100;
			}

			//��ֵ
			pRHA->FZZDMAry.assign(rxdAry.begin(), rxdAry.end());
			pRHA->ZMRXDArray.clear();
			for (int iFZZDM=0; iFZZDM<pRHA->FZZDMAry.size(); iFZZDM++)
			{
				if (pRHA->FZZDMAry[iFZZDM].Type == L"���ж�")
					pRHA->ZMRXDArray.push_back(pRHA->FZZDMAry[iFZZDM]);
			}
		}
	}

	//nan
	//RHA *tmp=g_DocManageAry[iDoc].XLAry[j];
	//if (tmp->m_XLName == SJ2X)
	//{
	//	vector<double> PROJARY;
	//	vector<double> PROJARYResult;
	//	ACHAR ckml[50];
	//	CStringArray ckmlaryResult;

	//	double projml=97700.0;
	//	for (projml=97700.0; projml<159500; projml += 50.0)
	//	{
	//		PROJARY.push_back(projml);
	//	}

	//	ckmlaryResult.RemoveAll();
	//	for(int i=0; i<PROJARY.size(); i++)
	//	{
	//		tmp->TrsProjmltoCkml(PROJARY[i], ckml);
	//		ckmlaryResult.Add(ckml);
	//	}

	//	PROJARYResult.resize(PROJARY.size());
	//	for(int i=0; i<PROJARYResult.size(); i++)
	//	{
	//		tmp->TrsCkmltoProjml(ckmlaryResult[i], PROJARYResult[i]);
	//	}


	//	ofstream outfile;
	//	char ckml2[50];
	//	outfile.open("d:\\projmltockml.txt");
	//	outfile << "********************array********************" << endl;
	//	outfile << left<<setw(10) << "projml" << setw(10) << "ckml" << setw(10) << "2"
	//	<< setw(10)<<"3"<< setw(10) << "4" <<setw(10) << "5" << setw(10) << "6" 
	//	<< setw(10) << "7"<<setw(10) << "8" << setw(10) << "9"<<endl;
	//	for (int i = 0;i<PROJARY.size();i++)
	//	{ 
	//		USES_CONVERSION;
	//		strcpy(ckml2, T2A(ckmlaryResult[i]));
	//		outfile << left<<setw(10) << PROJARY[i] << setw(10) << /*ckmlaryResult[i]*/ckml2 <<endl;
	//	}

	//	outfile << "\n" << endl;
	//	outfile.close();

	//	outfile.open("d:\\ckmltoprojml.txt");
	//	outfile << "********************array********************" << endl;
	//	outfile << left<<setw(30) << "ckml" << setw(10) << "projml" << setw(10) << "2"
	//		<< setw(10)<<"3"<< setw(10) << "4" <<setw(10) << "5" << setw(10) << "6" 
	//		<< setw(10) << "7"<<setw(10) << "8" << setw(10) << "9"<<endl;
	//	for (int i = 0;i<PROJARYResult.size();i++)
	//	{ 
	//		USES_CONVERSION;
	//		strcpy(ckml2, T2A(ckmlaryResult[i]));
	//		outfile << left<<setw(30) << /*ckmlaryResult[i]*/ckml2 << setw(10) <<PROJARYResult[i]  <<endl;
	//	}

	//	outfile << "\n" << endl;
	//	outfile.close();


	//}

	

	//��ͼ
	if (pRHA)
	{
		pRHA->m_iDoc=iDoc;
		pRHA->setColorLineW();
		if(isAddtoDbs)
			BAS_DRAW_FUN::AddEntityToDbs(pRHA);
	}

	return pRHA;
}


//��·�����ռ����ߡ����������
void CCreatPM::SortXLNames(CStringArray &xlNameAry)
{
	CStringArray xlNameAryCopy;

	for (int i=0; i<xlNameAry.GetSize(); i++)
		xlNameAryCopy.Add(xlNameAry[i]);
	
	xlNameAry.RemoveAll();
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"���л���") >= 0)
		xlNameAry.Add(L"���л���");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"���ж���") >= 0)
		xlNameAry.Add(L"���ж���");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"��������") >= 0)
		xlNameAry.Add(L"��������");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"��������") >= 0)
		xlNameAry.Add(L"��������");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"��ƻ���") >= 0)
		xlNameAry.Add(L"��ƻ���");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"��ƶ���") >= 0)
		xlNameAry.Add(L"��ƶ���");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"�������") >= 0)
		xlNameAry.Add(L"�������");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"�������") >= 0)
		xlNameAry.Add(L"�������");
}

/*===================================================================
������:IsIncluded   
------------------------------------------------------------------------
��������:ȫ����·�������Ƿ��������·��
------------------------------------------------------------------------
����:
iDoc           Ҫ�������ĵ������±�
mdbName        ����·���ݿ���
xlName         ��·��
------------------------------------------------------------------------
���: ������������������е��±�,���򷵻�-1
------------------------------------------------------------------------
��д: ��˧ 2016.10.17  
=====================================================================*/
int CCreatPM::IsIncluded(int iDoc, CString mdbName, CString xlName)
{
	if (iDoc<0 || iDoc>=g_DocManageAry.size() || xlName.IsEmpty()) 
		return -1;

	mdbName.MakeUpper();
	mdbName.Trim();
	/*xlName.MakeUpper();
	xlName.Trim();*/

	int xlNum=g_DocManageAry[iDoc].XLAry.size();
	for(int j=0; j<xlNum; j++)
	{
		g_DocManageAry[iDoc].XLAry[j]->m_mdbName.MakeUpper();
		g_DocManageAry[iDoc].XLAry[j]->m_mdbName.Trim();

		if (g_DocManageAry[iDoc].XLAry[j]->m_mdbName == mdbName 
			&& g_DocManageAry[iDoc].XLAry[j]->m_XLName == xlName) 
			return j;
	}

	return -1;
}

/*===================================================================
������:IsIncluded   
------------------------------------------------------------------------
��������:ȫ����·�������Ƿ��������·��
------------------------------------------------------------------------
����:
doc            Ҫ�����Ĵ���
mdbName        ����·���ݿ���
xlName         ��·��
------------------------------------------------------------------------
���: 
iDoc           Ҫ�������ĵ������±꣬û�ҵ���ֵΪ-1
iXL            ����·�ڸ��ĵ�����·�����е��±꣬û�ҵ���ֵΪ-1
------------------------------------------------------------------------
��д: ��˧ 2016.10.17  
=====================================================================*/
bool CCreatPM::IsIncluded(AcApDocument *doc, CString mdbName, CString xlName, int &iDoc, int &iXL)
{
	iDoc=-1;
	iXL=-1;

	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();
	
	for (int i=0; i<g_DocManageAry.size(); i++)
	{
		if (g_DocManageAry[i].Doc == doc)
		{
			iDoc=i;
			int xlNum=g_DocManageAry[i].XLAry.size();
			for(int j=0; j<xlNum; j++)
			{
				if (g_DocManageAry[i].XLAry[j]->m_mdbName == mdbName 
					&& g_DocManageAry[i].XLAry[j]->m_XLName == xlName) 
					iXL=j;
			}

			break;
		}
	}

	if (iDoc == -1 || iXL == -1)
		return false;
	else 
		return true;
	
}

/*===================================================================
��������:ȫ����·�������Ƿ��������·��
------------------------------------------------------------------------
����:
mdbName        ����·���ݿ���
xlName         ��·��
------------------------------------------------------------------------
���: 
iDoc           Ҫ�������ĵ������±꣬û�ҵ���ֵΪ-1
iXL            ����·�ڸ��ĵ�����·�����е��±꣬û�ҵ���ֵΪ-1
------------------------------------------------------------------------
��д: ��˧ 2016.10.17  
=====================================================================*/
bool CCreatPM::IsIncluded(CString mdbName, CString xlName , int &iDoc, int &iXL)
{
	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	iDoc=-1;
	iXL=-1;
	if (CurDocIndx(iDoc))
	{
		int xlNum=g_DocManageAry[iDoc].XLAry.size();
		for(int j=0; j<xlNum; j++)
		{
			if (g_DocManageAry[iDoc].XLAry[j]->m_mdbName == mdbName 
				&& g_DocManageAry[iDoc].XLAry[j]->m_XLName == xlName)
			{
				iXL=j;
				break;
			}
				
		}
	}

	if (iDoc == -1 || iXL == -1)
		return false;
	else 
		return true;
}

/*************************************************************
��������: ȫ����·�������Ƿ��������·�������û�о����ɸ���·
�������:
mdbName     			���ݿ���������·����
xlShuXing    			��·���ԣ���ʱ�ߡ�ʩ������
xlName                  ��·��
isAddtoDbs              �Ƿ���ͼ��  
�������:
ix                      ����·��ȫ�������е��±�
��������ֵ:������������������˸���·����true, ��֮����false
��ע���ú���ֻ���ڼ����ߺ������
��д����˧ 2016
***************************************************************/
bool CCreatPM::IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, int &ix, bool isAddtoDbs)
{
	//��ǰ�ĵ�ָ��
	int i=-1;
	AcApDocument *doc;
	doc=acDocManager->curDocument();
	ix=-1;

	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	for (i=0; i<g_DocManageAry.size(); i++)
	{
		if (g_DocManageAry[i].Doc == doc)
		{
			for(int j=0; j<g_DocManageAry[i].XLAry.size(); j++)
			{
				if (g_DocManageAry[i].XLAry[j]->m_mdbName == mdbName 
					&& g_DocManageAry[i].XLAry[j]->m_XLName == xlName)
				{
					ix = j;
					return true;
				}	
			}

			break;
		}
	}

	ix=g_DocManageAry[i].XLAry.size();

	if (CreatGlobalRHA(mdbName, /*xlShuXing,*/ xlName, isAddtoDbs) == NULL)
	{
		ix=-1;
		return false;
	}
	else
		return true;

}


/*************************************************************
��������: ȫ����·�������Ƿ��������·��,����������������ɸ���·ָ�룬���ظ�ָ��
�������:
mdbName     			���ݿ���������·����
xlName                  ��·��
isAddtoDbs              �Ƿ���ͼ��  
��������ֵ:���ظ���·ָ��
��ע���ú���ֻ���ڼ����ߺ������
��д����˧ 2016
***************************************************************/
RHA* CCreatPM::IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, bool isAddtoDbs)
{
	//��ǰ�ĵ�ָ��
	AcApDocument *doc;
	doc=acDocManager->curDocument();
	/*ix=-1;
	iDoc=-1;*/

	mdbName.MakeUpper();
	mdbName.Trim();
	xlName.MakeUpper();
	xlName.Trim();

	for (int i=0; i<g_DocManageAry.size(); i++)
	{
		if (g_DocManageAry[i].Doc == doc)
		{
			/*iDoc=i;*/
			for(int j=0; j<g_DocManageAry[i].XLAry.size(); j++)
			{
				if (g_DocManageAry[i].XLAry[j]->m_mdbName == mdbName 
					&& g_DocManageAry[i].XLAry[j]->m_XLName == xlName)
				{
					return g_DocManageAry[i].XLAry[j];
				}	
			}

			break;
		}
	}

	RHA *pRHA;
	
	pRHA=CreatGlobalRHA(mdbName, /*xlShuXing,*/ xlName, isAddtoDbs);

	return pRHA;

}




//��������XLSize
//�������ܣ���ȡg_DocManageAry[iDoc].XLAry����·����
//��ע��������·ʱ���������ӣ�����g_DocManageAry[iDoc].XLAry[j]Ϊ�գ�g_DocManageAry[iDoc].XLAry[j+1]��Ϊ�յ����
//��д����˧ 16.5.23
//int CCreatPM::XLSize()
//{
//	for(int j=0; j<MaxXLNum; j++)
//	{
//		if(g_DocManageAry[iDoc].XLAry[j] == NULL)
//				return j;
//	}
//}

//int GetXLNameLever(CString XLName)   818 liwei
//{
//	if ()
//	{
//	}
//	CStringArray XLNameAry;//��·��
//	XLNameAry.SetSize(8);
//	XLNameAry[0]="���л���";
//	XLNameAry[1]="���ж���";
//	XLNameAry[2]="��������";
//	XLNameAry[3]="��������";
//	XLNameAry[4]="��ƻ���";
//	XLNameAry[5]="��ƶ���";
//	XLNameAry[6]="�������";
//	XLNameAry[7]="�������";
//}

/*
��������GetAllXLNames
�������ܣ���ȡ�����ݿ���xlName֮ǰ��������·��  //818 Ӧ���Ƕ���һ��
��ע��
��д����˧ 16.6.15 
*/
void CCreatPM::GetAllXLNames(CString mdbName, CString xlName, CStringArray &allXLNameAry)
{
	  
	allXLNameAry.RemoveAll();

	CStringArray XLNameAry;//��·��
	XLNameAry.SetSize(8);
	XLNameAry[0]="���л���";
	XLNameAry[1]="���ж���";
	XLNameAry[2]="��������";
	XLNameAry[3]="��������";
	XLNameAry[4]="��ƻ���";
	XLNameAry[5]="��ƶ���";
	XLNameAry[6]="�������";
	XLNameAry[7]="�������";

	//xlName֮ǰ����·Ϊ���п��ܵĲ�����·
	int j;
	for(j=0;j<8;j++)
	{
		if (_tcscmp(xlName, XLNameAry[j]) == 0)
			break;
	}


	//�Ƿ�������,����������ӽ�allXLNameAry
	XLDataBase xlmdb;
	CString temp;
	for(int i=0;i<j;i++)
	{
		temp = L"��·��=\"" + XLNameAry[i] + L"\"";
		if( xlmdb.IsXLHasData(mdbName, L"�������ݱ�", temp) )
			allXLNameAry.Add(XLNameAry[i]);
	}

	allXLNameAry.Add(XLNameAry[j]);
}

/*
��������CmpXLPriority
�������ܣ��Ƚ�������·�������ȼ�
��ע��
����������·1���ȣ�����true����֮������false
��д����˧ 16.6.15 
*/
bool CCreatPM::CmpXLPriority(CString xlName1, CString xlName2)//818 ��BAS_CALC_FUN::CmpShuXing�滻
{
	CStringArray XLNameAry;//��·��
	XLNameAry.SetSize(8);
	XLNameAry[0]="���л���";
	XLNameAry[1]="���ж���";
	XLNameAry[2]="��������";
	XLNameAry[3]="��������";
	XLNameAry[4]="��ƻ���";
	XLNameAry[5]="��ƶ���";
	XLNameAry[6]="�������";
	XLNameAry[7]="�������";

	int i, j;
	for (i=0;i<8;i++)
	{
		if(_tcscmp(xlName1, XLNameAry[i]) == 0)
			break;
	}

	for (j=0;j<8;j++)
	{
		if(_tcscmp(xlName2, XLNameAry[j]) == 0)
			break;
	}

	if(i<j)
		return true;
	else
		return false;

}

/*===================================================================
������:ChgXLByXLAttrName   
------------------------------------------------------------
��������:������·���Ժ���·���޸���·��eg.�����л��߸�Ϊ��ƻ���
------------------------------------------------------------
����: 
pRHA        ��·ָ��
xlShuXing   Ŀ����·����
xlName      Ŀ����·��
------------------------------------------------------------
����: ��ʼ�����ߺ��б�༭ʱʹ�ô˺���
------------------------------------------------------------
��д: ��˧ 2016.8.28 
=====================================================================*/
RHA* CCreatPM::ChgXLByXLAttrName(RHA *pRHA, CString xlShuXing, CString xlName)
{//818 ���ߺ�һ�ߵ��б�༭��Ҫ��
	if (pRHA->m_XLName == xlName && pRHA->XLShuXing == xlShuXing)
		return pRHA;

	int ix=-1;
	int ix2=-1;
	ACHAR mes[128];
	CString oldXLName, oldXLShuXing;
	int iDoc=-1;

	//��ȡ��ǰ����
	if(!CurDocIndx(iDoc))
		return NULL;

	
	//���תΪ�����߻�����ߣ������·�Ѵ��ڣ������Ƿ񸲸�
	if(xlShuXing != LSX && xlShuXing != SGBX)
	{
		IsIncluded(pRHA->m_mdbName, xlName, iDoc, ix);
		if(ix >= 0)
		{
			_stprintf(mes, L"ϵͳ��[%s]��[%s]�Ѵ��ڣ��Ƿ���%s�滻?\n˵�����滻�󵱽��в�����̼���ʱ�������µ�%s������㡣"
				,pRHA->m_mdbName, xlName, pRHA->m_XLName, xlName);
				//818 ���ܴ����ݿ������ɣ���δ��ͼ��ʲôʱ������ݿ������ɣ�
			if (AfxMessageBox(mes, MB_YESNO) == IDNO)
				return pRHA;

			//��·ʵ������,�򸳿գ�������g_DocManageAry[iDoc].XLAry��λ�ò��䣬Ϊ����ĸ���׼��
			NullRHA(iDoc, ix);
			//g_DocManageAry[iDoc].XLAry[ix]=NULL;
		}
	}

	oldXLShuXing=pRHA->XLShuXing;//ԭʼ��·������·����
	oldXLName=pRHA->m_XLName;
	if(oldXLShuXing != SGBX || oldXLShuXing != LSX)
		ix2=IsIncluded(iDoc, pRHA->m_mdbName, pRHA->m_XLName);

	pRHA->m_XLName=xlName;
	pRHA->XLShuXing=xlShuXing;

	for (int i=0; i<pRHA->JDarray.size(); i++)
		_tcscpy(pRHA->JDarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->DLarray.size(); i++)
		_tcscpy(pRHA->DLarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->BZLCarray.size(); i++)
		_tcscpy(pRHA->BZLCarray[i].XLName, pRHA->m_XLName);

	pRHA->SaveData();
	pRHA->UpdateAllData();

	//818 ����֮����·��һ�º�SaveData()
	for (int i=0; i<pRHA->JDarray.size(); i++)
		_tcscpy(pRHA->JDarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->DLarray.size(); i++)
		_tcscpy(pRHA->DLarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->BZLCarray.size(); i++)
		_tcscpy(pRHA->BZLCarray[i].XLName, pRHA->m_XLName);


	if(xlShuXing != LSX && xlShuXing != SGBX)
	{
		//ԭ������·��ʩ�����߻���ʱ��
		//��Ŀ����תΪ�����߻�����ߣ�ȫ��ָ�룩ʱ
		if(oldXLShuXing == LSX || oldXLShuXing == SGBX)
		{
			//
			if (ix<0)//��·û�й���
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
			else			
				g_DocManageAry[iDoc].XLAry[ix]=pRHA;
		}
	}
	else
	{
		//ԭ������·�Ǽ����߻�����ߣ�ȫ��ָ�룩
		//��Ŀ����תΪ��ʱ�߻�ʩ������ʱ
		if(oldXLShuXing != LSX && oldXLShuXing != SGBX)
		{
			RHA *pNewRHA;

			pNewRHA = new RHA(pRHA->JDarray,L"AK", L"JD"
						, xlName, pRHA->m_mdbName, xlShuXing);

			pNewRHA->DLarray.clear();
			pNewRHA->DLarray.assign(pRHA->DLarray.begin(), pRHA->DLarray.end());

			pNewRHA->BZLCarray.clear();
			pNewRHA->BZLCarray=pRHA->BZLCarray;

			pNewRHA->setColorLineW();
			BAS_DRAW_FUN::AddEntityToDbs(pNewRHA);

			//pRHA->erase();//818 ע�⣬pRHA���д

			//if(ix2 >= 0)
			//	//g_DocManageAry[iDoc].XLAry[ix2]=NULL;
			//	DelRHA(iDoc, ix2);

			return pNewRHA;

		}
		

	}

	if (oldXLShuXing != xlShuXing)
		pRHA->setColorLineW();

	return pRHA;
}


/*************************************************************
��������: �����ת��·ʵ�壨�����ߡ�����ߡ���ʱ�ߡ�ʩ������)
�������:
xlName                  Ŀ����·��
eIdToIn                 ʰȡ�Ķ����ID
xlShuXing    			Ŀ����·����
mdbName     			���ݿ���������·����
��������ֵ: 
��д��ls 2016.11.7
***************************************************************/
void CCreatPM::PLinetoRHA(CString xlName , AcDbObjectId eIdToIn , CString xlShuXing, CString mdbName)
{
	double R;
	double L1;
	double L2;

	BAS_CALC_FUN BS;
	BS.ReadGuifanFile();
	if (xlName.Find(L"�Ľ�")>-1)
	{
		R = mGJGuiFanCS.MinR1;
		L1 = L2 = mGJGuiFanCS.L[mGJGuiFanCS.RXiangShu-1];
	}
	else
	{
		R = mJYGuiFanCS.MinR1;
		L1 = L2 = mJYGuiFanCS.L[mJYGuiFanCS.RXiangShu-1];
	}

	//int NLC=3,NH=3,texth=8;

	AcGePoint3d location;
	CString JDNumCString;
	int i=0,/*j=0,*/trow;
	CArray<JDarrayStruct, JDarrayStruct> JDarray;
	JDarrayStruct TempJD;

	_tcscpy(TempJD.XLName, xlName);
	_tcscpy(TempJD.BLName, L"");
	_tcscpy(TempJD.BLJDName, L"");
	_tcscpy(TempJD.Draw_JDName, L"");
	_tcscpy(TempJD.ZHLC, L"");
	TempJD.XJJ=0.0;


	AcDbObject *pObj = NULL ;
	if(acdbOpenObject(pObj, eIdToIn, AcDb::kForRead)==Acad::eOk)
	{
		pObj->close();

		if( pObj->isKindOf(AcDb2dPolyline::desc())
			|| pObj->isKindOf(AcDbPolyline::desc())
			|| pObj->isKindOf(AcDbLine::desc())
			|| pObj->isKindOf(AcDbArc::desc()) )
		{   
			//  ployline
			if (pObj->isKindOf(AcDbPolyline::desc())) 
			{  
				AcDbPolyline *pPline1;
				AcGePoint3d location;
				int numverts;
				pPline1=AcDbPolyline::cast(pObj);
				numverts=pPline1->numVerts();
				for ( i= 0; i<numverts; i++)
				{  //Ѱ�ҵ�ĳ����
					pPline1->getPointAt( i, location);
					JDNumCString.Format(L"JD%d",i);
					_tcscpy(TempJD.JDName ,JDNumCString) ;
					TempJD.N = location[Y];
					TempJD.E = location[X];
					TempJD.l1 = L1;
					TempJD.R = R;
					TempJD.l2 = L2;
					JDarray.Add(TempJD);
				}
			}
			else if (pObj->isKindOf(AcDb2dPolyline::desc())) 
			{  
				AcDb2dPolyline *pPline;

				AcDb2dVertex *pVertex;
				AcGePoint3d location;
				AcDbObjectId vertexObjId;

				pPline=AcDb2dPolyline::cast(pObj);
				AcDbObjectIterator *pVertIter= pPline->vertexIterator();

				for ( i= 0; !pVertIter->done(); i++, pVertIter->step())
				{  //Ѱ�� ��ĳ����
					vertexObjId = pVertIter->objectId();
					if(acdbOpenObject(pObj, vertexObjId, AcDb::kForRead)==Acad::eOk)
					{
						pObj->close();
						if(pObj->isKindOf(AcDb2dVertex::desc()))
						{
							pVertex = AcDb2dVertex::cast(pObj);
							location = pVertex->position();
						}
					}

					JDNumCString.Format(L"JD%d",i);
					_tcscpy(TempJD.JDName ,JDNumCString) ;
					TempJD.N = location[Y];
					TempJD.E = location[X];
					TempJD.l1 = L1;
					TempJD.R = R;
					TempJD.l2 = L2;
					JDarray.Add(TempJD);
				};
			}
			else if  (pObj->isKindOf(AcDbLine::desc())) 
			{  
				AcDbLine *pLine;
				AcGePoint3d PT1,PT2;

				pLine = AcDbLine::cast(pObj);

				PT1=pLine->startPoint();
				PT2=pLine->endPoint();        
				pLine->close(); 
				trow=0;
				JDNumCString = L"JD0";
				_tcscpy(TempJD.JDName ,JDNumCString) ;
				TempJD.N=PT1.y; 
				TempJD.E=PT1.x;
				TempJD.l1=0;
				TempJD.R=0;
				TempJD.l2=0;
				JDarray.Add(TempJD);

				JDNumCString = L"JD1";
				_tcscpy(TempJD.JDName ,JDNumCString) ;		
				TempJD.N=PT2.y; 
				TempJD.E=PT2.x;
				TempJD.l1=0;
				TempJD.R=0;
				TempJD.l2=0;
				JDarray.Add(TempJD);
			}
			else if(pObj->isKindOf(AcDbArc::desc()))//plh07.17
			{

				pObj->close();
				AcDbArc *pArc;

				pArc = AcDbArc::cast(pObj);

				BCCreateXLEntity OB;
				double CenN,CenE,R,SA,EA;
				CenN=pArc->center().y;
				CenE=pArc->center().x;
				R=pArc->radius();
				SA=pArc->startAngle();
				EA=pArc->endAngle();
				OB.SetARC(CenN,CenE,R,SA,EA);
				OB.UpdateData(true, xlName);

				/*if(acdbOpenObject(pObj , eIdToIn , AcDb::kForWrite)==Acad::eOk)
				{
					if(pObj->isKindOf(AcDbArc::desc()))
					{
						pArc = AcDbArc::cast(pObj);
						pArc->assertWriteEnabled();

						pArc->erase(Adesk::kTrue);

						pArc->close();
						return;
					}

				}*/


			}
		}

	}
	else
	{
		pObj->close();
		ads_printf(L"\nSelected entity is not a Polyline!");
		return;
	} 	


	ACHAR GH[8],JDGH[20];
	_tcscpy(GH,L"K"); 
	_tcscpy(JDGH,L"JD");

	trow = JDarray.GetSize(); 

	if(trow<1)return;
	JDarray[0].l1=JDarray[0].R=JDarray[0].l2=0.0;
	if(trow>0)
		JDarray[trow-1].l1=JDarray[trow-1].R=JDarray[trow-1].l2=0.0;

	if(mdbName == L"")
	{
		BAS_CALC_FUN ob;
		mdbName=ob.GetCurProjName();
	}

	RHA *pRHA=NULL;
	int j;
	AcDbObjectId eId;
	int iDoc=-1;
	//�����·���ڣ�ɾ��(818 erase??)

	if (xlShuXing != LSX && xlShuXing != SGBX)
	{
		if (IsIncluded(mdbName, xlName, iDoc, j))
		{
			g_DocManageAry[iDoc].XLAry[j]=NULL;
			g_DocManageAry[iDoc].XLAry[j] = new RHA(JDarray, GH, JDGH, xlName, mdbName, xlShuXing);

		}
		else
		{
			j=g_DocManageAry[iDoc].XLAry.size();
			pRHA = new RHA(JDarray, GH, JDGH, xlName, mdbName, xlShuXing);
			g_DocManageAry[iDoc].XLAry.push_back(pRHA);

		}

		g_DocManageAry[iDoc].XLAry[j]->m_iDoc=iDoc;
		//g_DocManageAry[iDoc].XLAry[j]->setColorLineW(/*fileName, xlName*/);
		eId=BAS_DRAW_FUN::AddEntityToDbs(g_DocManageAry[iDoc].XLAry[j]);//��ͼ
	}
	else
	{
		if (xlShuXing == LSX)
			pRHA = new RHA(JDarray, GH, JDGH, xlName, L"C:\\δ����.mdb", xlShuXing);
		else if (xlShuXing == SGBX)
			pRHA = new RHA(JDarray, GH, JDGH, xlName, mdbName, xlShuXing);
		
		pRHA->setColorLineW();
		eId=BAS_DRAW_FUN::AddEntityToDbs(pRHA);
	}

	//���б�༭
	CPMSJ::ListXLInfor(FALSE, eId);

	//ɾ��ʵ��
	AcDbEntity *pEnt;
	if(acdbOpenObject(pEnt, eIdToIn, AcDb::kForWrite)==Acad::eOk)
	{
		pEnt->erase(Adesk::kTrue);
		pEnt->close();
		return;  
	}
	else
		ads_printf(L"ɾ��ʵ��ʧ��!\n");

	return;
}

//��ȡ��ǰ����,���������g_DocManageAry�е��±�
bool CCreatPM::CurDocIndx(int &iDoc)//818 ��Щ�ط����û�ȡiDoc�ˣ����һ��
{
	iDoc=-1;
	AcApDocument *doc=acDocManager->curDocument();

	for(int i=0; i<g_DocManageAry.size(); i++)
	{
		if (g_DocManageAry[i].Doc == doc)
		{
			iDoc=i;break;
		}
	}

	if (iDoc<0)
		return false;
	else
		return true;
}

//�����·��ȫ���������Ѿ�����,��ɾ��ͼ�Σ�����·����
bool CCreatPM::NullRHA(int iDoc, int iXL) 
{
	if(iDoc>=g_DocManageAry.size()
		|| iXL>=g_DocManageAry[iDoc].XLAry.size())
		return false;

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if (Acad::eOk == acdbOpenObject(g_DocManageAry[iDoc].XLAry[iXL]
	, g_DocManageAry[iDoc].XLAry[iXL]->id(), AcDb::kForWrite))
	{
		g_DocManageAry[iDoc].XLAry[iXL]->assertWriteEnabled();
		g_DocManageAry[iDoc].XLAry[iXL]->erase();
		g_DocManageAry[iDoc].XLAry[iXL]->close();
	}

	acDocManager->unlockDocument(acDocManager->curDocument());

	g_DocManageAry[iDoc].XLAry[iXL]=NULL;
	
	return true;
}


//�����·��ȫ���������Ѿ�����,��ɾ��ͼ�Σ�ɾ����·
bool CCreatPM::DelRHA(int iDoc, int iXL)
{
	if(iDoc>=g_DocManageAry.size()
		|| iXL>=g_DocManageAry[iDoc].XLAry.size())
		return false;

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if (Acad::eOk == acdbOpenObject(g_DocManageAry[iDoc].XLAry[iXL]
	, g_DocManageAry[iDoc].XLAry[iXL]->id(), AcDb::kForWrite))
	{
		g_DocManageAry[iDoc].XLAry[iXL]->assertWriteEnabled();
		g_DocManageAry[iDoc].XLAry[iXL]->erase();
		g_DocManageAry[iDoc].XLAry[iXL]->close();
	}

	acDocManager->unlockDocument(acDocManager->curDocument());

	g_DocManageAry[iDoc].XLAry.erase(g_DocManageAry[iDoc].XLAry.begin()+iXL);

	return true;
}


/*************************************************************
��������: �������еļ����ߺ�����ߵ�ʵ��
�������:
mdbName     			���ݿ���������·����
iDoc    			    �����±�
�������:
g_DocManageAry          ȫ����·����
��д����˧ 2016
***************************************************************/
void CCreatPM::CreatAllJYXandSJX(CString mdbName, int iDoc)
{
	XLDataBase xlmdb;
	RHA *pRHA;
	CString tmpStr ;
	CArray<XLInforTableStruct, XLInforTableStruct> xlInforAry;//��·��Ϣ 

	//��ȡ������·��
	xlInforAry.RemoveAll();
	if(xlmdb.Read_XLDbs(mdbName,L"��·��Ϣ��",L""))	
	{
		if(xlmdb.XLInforNum>0)
		{
			for (int i=0;i<xlmdb.XLInforNum;i++)
				xlInforAry.Add(xlmdb.XLInforTable[i]);
		}
	}

	//�γ���·��Ϣ����
	for (int i=0; i<xlmdb.XLInforNum; i++)
	{
		if(xlmdb.XLInforTable[i].XLShuXing != LSX 
			&& xlmdb.XLInforTable[i].XLShuXing != SGBX)
		{
			xlInforAry.Add(xlmdb.XLInforTable[i]);
		}

	}
	if(xlmdb.XLInforTable)
		delete []xlmdb.XLInforTable;
	xlmdb.XLInforTable=NULL;
	
	//����������·
	if (iDoc<0)
		CurDocIndx(iDoc);

	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		if (IsIncluded(iDoc, mdbName, xlInforAry[i].XLName)<0)
		{
			pRHA=CreatGlobalRHA(mdbName, xlInforAry[i].XLName, false);
			if(pRHA == NULL)
				ads_printf(L"������·ʧ�ܣ�\n");
			else
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
		}
	}
}


//���ܣ��ӵ�ǰ������ ��ȡ��ϵͳ��ȫ�������� ĳ���ݿ� ��ȫ����·��Ϣ
//���룺���ݿ���
//�������·��Ϣ����
//��д��17.8.18
void CCreatPM::GetSysXLInfbyMdbName(CString mdbName, CArray<XLInforTableStruct, XLInforTableStruct> &xlInfAry)
{
	xlInfAry.RemoveAll();
	//��ǰ�ĵ�ָ��
	AcApDocument *doc;
	doc=acDocManager->curDocument();

	mdbName.MakeUpper();
	mdbName.Trim();

	for (int i=0; i<g_DocManageAry.size(); i++)
	{
		if (g_DocManageAry[i].Doc == doc)
		{
			XLInforTableStruct tmpInf;
			for(int j=0; j<g_DocManageAry[i].XLAry.size(); j++)
			{
				if (g_DocManageAry[i].XLAry[j]->m_mdbName == mdbName)
				{
					_tcscpy(tmpInf.XLName, g_DocManageAry[i].XLAry[j]->m_XLName);
					_tcscpy(tmpInf.XLShuXing, g_DocManageAry[i].XLAry[j]->XLShuXing);
					xlInfAry.Add(tmpInf);
				}	
			}

			break;
		}
	}
}