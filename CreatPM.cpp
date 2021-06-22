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

//vector<RHA*> g_DocManageAry[iDoc].XLAry;//既有线和设计线的全局数组
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
函数功能: 创建线路平面自定义实体（全局或局部）
输入参数:
mdbName     			数据库名（包括路径）
xlShuXing    			线路属性：临时线、施工便线
xlName                  线路名
isAddtoDbs              是否画在图上            
函数返回值:线路平面实体指针 
编写：李帅 2016
***************************************************************/
RHA* CCreatPM::CreatRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs)
{
	if (xlShuXing == SGBX || xlShuXing == LSX)
		return CreatPartRHA(mdbName, xlShuXing, xlName, isAddtoDbs);//创建临时线、施工便线
	else
		return CreatGlobalRHA(mdbName, xlName, isAddtoDbs);//创建既有基线、既有二线、设计基线、设计二线

}

/*************************************************************
函数功能: 创建局部的线路平面实体指针，包括：临时线、施工便线
输入参数:
mdbName     			数据库名（包括路径）
xlShuXing    			线路属性：临时线、施工便线
xlName                  线路名
isAddtoDbs              是否画在图上            
函数返回值:局部的线路平面实体指针 
编写：李帅 2016
***************************************************************/
RHA* CCreatPM::CreatPartRHA(CString mdbName, CString xlShuXing, CString xlName, bool isAddtoDbs)
{
	CString temp;
	XLDataBase xlmdb;
	RHA *pRHA=NULL;

	xlName.Trim();	
	temp = L"线路名=\"" + xlName + L"\"";
	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"交点数据表", temp))//读交点
	{

		pRHA = new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD"
			, xlName, mdbName, xlShuXing);
	}
	else
	{
		ads_printf(L"读交点数据表失败!\n");
		return NULL;
	}

	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"断链表", temp))//读断链
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
		ads_printf(L"读断链表失败!\n");
	}

	//自动生成标志里程
	pRHA->GenBZLCbyDL();

	//标志里程表
	if(xlmdb.Read_XLDbs(mdbName, xlShuXing+L"标志里程表", temp))
	{

		if (xlmdb.BZLCNum>0)
		{
			//插入用户的标志里程
			for (int k=0;k<xlmdb.BZLCNum;k++)
				pRHA->BZLCarray.push_back(xlmdb.BZLCTable[k]);
			//计算标志里程信息
			pRHA->CalBZLC();
		}

	}

	//绘图
	pRHA->setColorLineW();
	if(isAddtoDbs)
		BAS_DRAW_FUN::AddEntityToDbs(pRHA);

	
	return pRHA;
}


/*************************************************************
函数功能: 释放平面线路指针（全局）
输入参数: pRHA     	平面指针
算法描述：如果线路在全局数组中，但没有在CAD图中画出来，就释放该指针
，然后从数据库中新生成的线路赋值该指针.
函数返回值: 
备注：
编写：李帅 2017.6
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
		{//没有在CAD图中画出来
			delete []pRHA;
			pRHA=NULL;
		}

		acDocManager->unlockDocument(acDocManager->curDocument());
	}
}

	
	
//818 换了窗口创建还是该窗口创建，假定该窗口
/*************************************************************
函数功能: 创建全局的线路自定义实体，包括：既有基线、既有二线、设计基线、设计二线
输入参数:
mdbName     			数据库名（包括路径）
xlName                  线路名
isAddtoDbs              是否画在图上            
函数返回值:全局线路平面实体指针 
备注：如果线路在全局数组中，但没有在CAD图中画出来，就释放该指针
	  ，然后从数据库中新生成的线路赋值该指针
编写：李帅 2016
***************************************************************/
RHA* CCreatPM::CreatGlobalRHA(CString mdbName, CString xlName, bool isAddtoDbs)//818 这个函数只能通过IsGlobalXLHaveAndCreat调用 以后
{
	CString temp;
	CStringArray newXLNameAry;
	AcApDocument *doc=acDocManager->curDocument();
	int iDoc=-1,j=-1;
	RHA *pRHA=NULL;
	XLDataBase xlmdb1;

	//获取当前窗口
	if(!CurDocIndx(iDoc))
		return NULL;
	
	CString XLShuxing = xlmdb1.GetXLAttriStr(xlName);
	if (XLShuxing == SGBX || XLShuxing == LSX)
	{
		temp.Trim();
		temp = L"线路名=\"" + xlName + L"\"";
		if(xlmdb1.Read_XLDbs(mdbName, XLShuxing + L"交点数据表", temp))//读交点
		{
			//j=IsIncluded(iDoc, mdbName, xlName);
			//if (j<0)
			//{
				//线路没有构建，生成线路
				j=g_DocManageAry[iDoc].XLAry.size();
				pRHA = new RHA(xlmdb1.NJD, xlmdb1.JDB, L"AK", L"JD"
					, xlName, mdbName);

				if(xlmdb1.Read_XLDbs(mdbName, XLShuxing + L"断链表", temp))//读断链
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
					ads_printf(L"读断链表失败!\n");
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
			ads_printf(xlName+L"交点数据表缺失，生成线路失败!\n");
			return NULL;
		}

		return pRHA;
	}


	//一、读取该线路（xlName）的所有基准线线路名，存在m_JYXNames、m_SJXNames中
	newXLNameAry.RemoveAll();
	//如果是生成设计二线，必须在之前生成设计基线为设计二线绕行段（纵面）的计算提供设计基线指针
	if(xlName.Find(L"设计") >= 0 && xlName != SJJX)
		newXLNameAry.Add(SJJX);

	xlName.Trim();
	
	if (!xlmdb1.ReadBLNames(mdbName ,L"交点数据表", xlName, newXLNameAry))
		return NULL;

	
	//二、读取基准线的基准线线路名，循环。按照既有基线、既有二线……设计基线、设计二线排序
	int change=0;
	int len=0;
	int oldXLNum;
	vector<FZZDM> rxdAry;

	do 
	{
		change=0;
		len=newXLNameAry.GetSize();

		for (int i=0; i<len; i++)        //如果和上一步存储的线路名不重复，存储线路名。
		{			                    //再读该线路的参照线，直到找出构建该线路（xlName）需要的所有相关的参照线路名。
			
			if (newXLNameAry[i] != L"")
			{
				XLDataBase xlmdb2;

				oldXLNum=newXLNameAry.GetSize();
				xlmdb2.ReadBLNames(mdbName ,L"交点数据表", newXLNameAry[i], newXLNameAry);
				change += newXLNameAry.GetSize()-oldXLNum;
			}
		}

	} while (change);


	newXLNameAry.Add(xlName);
	SortXLNames(newXLNameAry);//818 通过参照关系排序最好，注意既有二线是同等级的，今后如何排序
	int newXLNum=newXLNameAry.GetSize();

	//nan
	vector<CString> tmpNmAry;
	for(int i=0; i<newXLNum; i++)
		tmpNmAry.push_back(newXLNameAry[i]);


	//三、依次构建新的线路RHA实体
	for(int i=0; i<newXLNum; i++)
	{
		XLDataBase xlmdb;//attention: 每次循环会回收XLDataBase指针开的空间
		pRHA=NULL;

		temp = newXLNameAry[i];
		temp.Trim();
		temp = L"线路名=\"" + temp + L"\"";
		if(xlmdb.Read_XLDbs(mdbName, L"交点数据表", temp))//读交点
		{
			j=IsIncluded(iDoc, mdbName, newXLNameAry[i]);
			if (j<0)
			{
				//线路没有构建，生成线路
				j=g_DocManageAry[iDoc].XLAry.size();
				pRHA = new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD", newXLNameAry[i], mdbName);
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
				pRHA->m_iDoc=iDoc;
				pRHA->setColorLineW();
			}
			else 
			{
				//如果该线路已有，重新生成 818 还是不用生成？？还有其他线路呢
				if (i == newXLNum-1)
				{//需要生成的线路已有，从数据库中重新生成

					//释放指针
					FreeRHA(g_DocManageAry[iDoc].XLAry[j]);
					//生成线路
					pRHA= new RHA(xlmdb.NJD, xlmdb.JDB, L"AK", L"JD"
						, newXLNameAry[i], mdbName);					//由RHA的构造函数进行线路的绘制
					g_DocManageAry[iDoc].XLAry[j] = pRHA;				//存入g_DocManageAry进行线路管理
				}
				else
					//参照线已有，不需要再生成
					continue;
				
			}

		}
		else
		{
			ads_printf(newXLNameAry[i]+L"交点数据表缺失，生成线路失败!\n");
			return NULL;
		}

		if(xlmdb.Read_XLDbs(mdbName, L"断链表", temp))//读断链
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
			ads_printf(L"读断链表失败!\n");
		}

		//形成贯通断链表 818 注意是否有地方没用该函数生成既有线或设计线，不能形成贯通断链表
		if (pRHA->XLShuXing == JY2X 
			|| pRHA->XLShuXing == SJJX 
			|| pRHA->XLShuXing == SJ2X)
		{
			//删除并行段断链
			pRHA->EarseDLofBLonBXD();
			//形成贯通断链表
			pRHA->Create2XGTDLB();
		}
					
		//818 临时线和施工便线有标志里程？
		//标志里程表
		if (pRHA->XLShuXing != SJJX && pRHA->XLShuXing != SJ2X)
		{
			if(xlmdb.Read_XLDbs(mdbName, L"标志里程表", temp)/* && xlmdb.BZLCNum>0*/)
			{
				//插入用户的标志里程
				pRHA->BZLCarray.resize(xlmdb.BZLCNum);
				for (int k=0;k<xlmdb.BZLCNum;k++)
					pRHA->BZLCarray[k]=xlmdb.BZLCTable[k];

				//添加断链产生的标志里程
				pRHA->GenBZLCbyDL(false);

				//计算标志里程信息
				pRHA->CalBZLC();
			}
			else
				//自动生成标志里程
				pRHA->GenBZLCbyDL(true);
		}
		else
			pRHA->BZLCarray.clear();


		//改建段信息
		pRHA->GJDInforArray.clear();
		if (pRHA->XLShuXing == JYJX 
			|| pRHA->XLShuXing == JY2X)
		{
			//既有线形成利用段数组 818 既有线记录利用段什么时候用？
			pRHA->CreatLYDArrayOfJYX();
		}
		else if (pRHA->XLShuXing == SJJX 
			|| pRHA->XLShuXing == SJ2X)
		{
			//形成全线的分段数组
			if(xlmdb.Read_XLDbs(mdbName, L"改建段汇总表", temp))
			{

				if (xlmdb.GJDAllInforNum>0)
				{
					//添加改建段信息 //818 在改建段数组形成时，应该排序，查重
					pRHA->GJDInforArray.resize(xlmdb.GJDAllInforNum);
					for (int k=0;k<xlmdb.GJDAllInforNum;k++)
						pRHA->GJDInforArray[k]=xlmdb.GJDAllInforTable[k];
						

					//计算各改建段起终点投影里程
					pRHA->GJDInforArray[0].BProjml=pRHA->QXB[0].KZH;

					for (int k=0;k<xlmdb.GJDAllInforNum;k++)
					{

						pRHA->GJDInforArray[k].EProjml=pRHA->PROJ_ML(pRHA->array, pRHA->tarray
							, pRHA->GJDInforArray[k].ept[0], pRHA->GJDInforArray[k].ept[1]
						, pRHA->GJDInforArray[k].ept[0], pRHA->GJDInforArray[k].ept[1]);

						if(k<xlmdb.GJDAllInforNum-1)
							pRHA->GJDInforArray[k+1].BProjml=pRHA->GJDInforArray[k].EProjml;

					}

					//计算各改建段起终点统一里程
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
		//纵面绕行段数组（纵面设计用）
		pRHA->FZZDMAry.clear();
		rxdAry.clear();

		if (pRHA->XLShuXing == JY2X
			|| pRHA->XLShuXing == SJ2X)
		{
			if(xlmdb.Read_XLDbs(mdbName, L"辅助纵断面段落表", temp))
			{
				if (xlmdb.FZZDMNum>0)
				{
					//添加绕行段信息
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
				ads_printf(L"未生成基线平面！\n");

			double stProjml1x, endProjml1x;
			int jdNum=pRHA->JDarray.size();
			double dist;

			for (int k=0;k<rxdAry.size();k++)
			{
				dist=100.0;
				//计算起终点在本线上的统一里程
				pRHA->TrsCkmltoCml(rxdAry[k].StCKml, rxdAry[k].BCml);
				pRHA->TrsCkmltoCml(rxdAry[k].EdCKml, rxdAry[k].ECml);
				
				//计算起终点在设计基线上的投影里程
				stProjml1x=pJX_RHA->PROJ_ML(pJX_RHA->array, pJX_RHA->tarray
					, rxdAry[k].spt[0], rxdAry[k].spt[1]
				, rxdAry[k].spt[0], rxdAry[k].spt[1]);

				endProjml1x=pJX_RHA->PROJ_ML(pJX_RHA->array, pJX_RHA->tarray
					, rxdAry[k].ept[0], rxdAry[k].ept[1]
				, rxdAry[k].ept[0], rxdAry[k].ept[1]);

				//求距离判断是否起终点绕行
				if (k == 0)
					dist=sqrt(pow(rxdAry[k].spt[0]-pRHA->JDarray[0].N, 2)
					+pow(rxdAry[k].spt[1]-pRHA->JDarray[0].E, 2));
				else if(k+1 == rxdAry.size())
					dist=sqrt(pow(rxdAry[k].ept[0]-pRHA->JDarray[jdNum-1].N, 2)
					+pow(rxdAry[k].ept[1]-pRHA->JDarray[jdNum-1].E, 2));
				

				//计算起终点在依赖平面上的统一里程
				if((k>0 && stProjml1x>=0 )|| (k == 0 && dist>0.5))
					rxdAry[k].BL_BCml=pJX_RHA->TrsProjmltoCml(stProjml1x);
				else
					rxdAry[k].BL_BCml=-100;

				if((k+1<rxdAry.size() && endProjml1x>=0 )|| (k+1 == rxdAry.size() && dist>0.5))
					rxdAry[k].BL_ECml=pJX_RHA->TrsProjmltoCml(endProjml1x);
				else
					rxdAry[k].BL_ECml=-100;
			}

			//赋值
			pRHA->FZZDMAry.assign(rxdAry.begin(), rxdAry.end());
			pRHA->ZMRXDArray.clear();
			for (int iFZZDM=0; iFZZDM<pRHA->FZZDMAry.size(); iFZZDM++)
			{
				if (pRHA->FZZDMAry[iFZZDM].Type == L"绕行段")
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

	

	//绘图
	if (pRHA)
	{
		pRHA->m_iDoc=iDoc;
		pRHA->setColorLineW();
		if(isAddtoDbs)
			BAS_DRAW_FUN::AddEntityToDbs(pRHA);
	}

	return pRHA;
}


//线路名按照既有线、设计线排序
void CCreatPM::SortXLNames(CStringArray &xlNameAry)
{
	CStringArray xlNameAryCopy;

	for (int i=0; i<xlNameAry.GetSize(); i++)
		xlNameAryCopy.Add(xlNameAry[i]);
	
	xlNameAry.RemoveAll();
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"既有基线") >= 0)
		xlNameAry.Add(L"既有基线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"既有二线") >= 0)
		xlNameAry.Add(L"既有二线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"既有三线") >= 0)
		xlNameAry.Add(L"既有三线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"既有四线") >= 0)
		xlNameAry.Add(L"既有四线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"设计基线") >= 0)
		xlNameAry.Add(L"设计基线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"设计二线") >= 0)
		xlNameAry.Add(L"设计二线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"设计三线") >= 0)
		xlNameAry.Add(L"设计三线");
	if (BAS_CALC_FUN::IsIncluded(xlNameAryCopy, L"设计四线") >= 0)
		xlNameAry.Add(L"设计四线");
}

/*===================================================================
函数名:IsIncluded   
------------------------------------------------------------------------
函数功能:全局线路数组中是否包含此线路名
------------------------------------------------------------------------
输入:
iDoc           要搜索的文档窗口下标
mdbName        该线路数据库名
xlName         线路名
------------------------------------------------------------------------
输出: 如果包含返回在数组中的下标,否则返回-1
------------------------------------------------------------------------
编写: 李帅 2016.10.17  
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
函数名:IsIncluded   
------------------------------------------------------------------------
函数功能:全局线路数组中是否包含此线路名
------------------------------------------------------------------------
输入:
doc            要搜索的窗口
mdbName        该线路数据库名
xlName         线路名
------------------------------------------------------------------------
输出: 
iDoc           要搜索的文档窗口下标，没找到赋值为-1
iXL            该线路在该文档的线路数组中的下标，没找到赋值为-1
------------------------------------------------------------------------
编写: 李帅 2016.10.17  
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
函数功能:全局线路数组中是否包含此线路名
------------------------------------------------------------------------
输入:
mdbName        该线路数据库名
xlName         线路名
------------------------------------------------------------------------
输出: 
iDoc           要搜索的文档窗口下标，没找到赋值为-1
iXL            该线路在该文档的线路数组中的下标，没找到赋值为-1
------------------------------------------------------------------------
编写: 李帅 2016.10.17  
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
函数功能: 全局线路数组中是否包含此线路名，如果没有就生成该线路
输入参数:
mdbName     			数据库名（包括路径）
xlShuXing    			线路属性：临时线、施工便线
xlName                  线路名
isAddtoDbs              是否画在图上  
输出参数:
ix                      该线路在全局数组中的下标
函数返回值:如果包含或重新生成了该线路返回true, 反之返回false
备注：该函数只用于既有线和设计线
编写：李帅 2016
***************************************************************/
bool CCreatPM::IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, int &ix, bool isAddtoDbs)
{
	//当前文档指针
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
函数功能: 全局线路数组中是否包含此线路名,如果不包含重新生成该线路指针，返回该指针
输入参数:
mdbName     			数据库名（包括路径）
xlName                  线路名
isAddtoDbs              是否画在图上  
函数返回值:返回该线路指针
备注：该函数只用于既有线和设计线
编写：李帅 2016
***************************************************************/
RHA* CCreatPM::IsGlobalXLHaveAndCreat(CString mdbName, CString xlName, bool isAddtoDbs)
{
	//当前文档指针
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




//函数名：XLSize
//函数功能：获取g_DocManageAry[iDoc].XLAry的线路个数
//备注：增加线路时是依次增加，避免g_DocManageAry[iDoc].XLAry[j]为空，g_DocManageAry[iDoc].XLAry[j+1]不为空的情况
//编写：李帅 16.5.23
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
//	CStringArray XLNameAry;//线路名
//	XLNameAry.SetSize(8);
//	XLNameAry[0]="既有基线";
//	XLNameAry[1]="既有二线";
//	XLNameAry[2]="既有三线";
//	XLNameAry[3]="既有四线";
//	XLNameAry[4]="设计基线";
//	XLNameAry[5]="设计二线";
//	XLNameAry[6]="设计三线";
//	XLNameAry[7]="设计四线";
//}

/*
函数名：GetAllXLNames
函数功能：获取在数据库中xlName之前的所有线路名  //818 应该是二线一组
备注：
编写：李帅 16.6.15 
*/
void CCreatPM::GetAllXLNames(CString mdbName, CString xlName, CStringArray &allXLNameAry)
{
	  
	allXLNameAry.RemoveAll();

	CStringArray XLNameAry;//线路名
	XLNameAry.SetSize(8);
	XLNameAry[0]="既有基线";
	XLNameAry[1]="既有二线";
	XLNameAry[2]="既有三线";
	XLNameAry[3]="既有四线";
	XLNameAry[4]="设计基线";
	XLNameAry[5]="设计二线";
	XLNameAry[6]="设计三线";
	XLNameAry[7]="设计四线";

	//xlName之前的线路为所有可能的参照线路
	int j;
	for(j=0;j<8;j++)
	{
		if (_tcscmp(xlName, XLNameAry[j]) == 0)
			break;
	}


	//是否有数据,有数据则添加进allXLNameAry
	XLDataBase xlmdb;
	CString temp;
	for(int i=0;i<j;i++)
	{
		temp = L"线路名=\"" + XLNameAry[i] + L"\"";
		if( xlmdb.IsXLHasData(mdbName, L"交点数据表", temp) )
			allXLNameAry.Add(XLNameAry[i]);
	}

	allXLNameAry.Add(XLNameAry[j]);
}

/*
函数名：CmpXLPriority
函数功能：比较两个线路名的优先级
备注：
输出：如果线路1优先，返回true，反之，返回false
编写：李帅 16.6.15 
*/
bool CCreatPM::CmpXLPriority(CString xlName1, CString xlName2)//818 用BAS_CALC_FUN::CmpShuXing替换
{
	CStringArray XLNameAry;//线路名
	XLNameAry.SetSize(8);
	XLNameAry[0]="既有基线";
	XLNameAry[1]="既有二线";
	XLNameAry[2]="既有三线";
	XLNameAry[3]="既有四线";
	XLNameAry[4]="设计基线";
	XLNameAry[5]="设计二线";
	XLNameAry[6]="设计三线";
	XLNameAry[7]="设计四线";

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
函数名:ChgXLByXLAttrName   
------------------------------------------------------------
函数功能:根据线路属性和线路名修改线路，eg.将既有基线改为设计基线
------------------------------------------------------------
输入: 
pRHA        线路指针
xlShuXing   目标线路属性
xlName      目标线路名
------------------------------------------------------------
调用: 初始化二线和列表编辑时使用此函数
------------------------------------------------------------
编写: 李帅 2016.8.28 
=====================================================================*/
RHA* CCreatPM::ChgXLByXLAttrName(RHA *pRHA, CString xlShuXing, CString xlName)
{//818 二线和一线的列表编辑框要换
	if (pRHA->m_XLName == xlName && pRHA->XLShuXing == xlShuXing)
		return pRHA;

	int ix=-1;
	int ix2=-1;
	ACHAR mes[128];
	CString oldXLName, oldXLShuXing;
	int iDoc=-1;

	//获取当前窗口
	if(!CurDocIndx(iDoc))
		return NULL;

	
	//如果转为既有线或设计线，如果线路已存在，提醒是否覆盖
	if(xlShuXing != LSX && xlShuXing != SGBX)
	{
		IsIncluded(pRHA->m_mdbName, xlName, iDoc, ix);
		if(ix >= 0)
		{
			_stprintf(mes, L"系统中[%s]的[%s]已存在，是否用%s替换?\n说明：替换后当进行参照里程计算时，将用新的%s参与计算。"
				,pRHA->m_mdbName, xlName, pRHA->m_XLName, xlName);
				//818 可能从数据库中生成，并未绘图。什么时候从数据库中生成？
			if (AfxMessageBox(mes, MB_YESNO) == IDNO)
				return pRHA;

			//线路实体已有,则赋空，在数组g_DocManageAry[iDoc].XLAry中位置不变，为下面的更新准备
			NullRHA(iDoc, ix);
			//g_DocManageAry[iDoc].XLAry[ix]=NULL;
		}
	}

	oldXLShuXing=pRHA->XLShuXing;//原始线路名、线路属性
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

	//818 更新之后，线路名一致和SaveData()
	for (int i=0; i<pRHA->JDarray.size(); i++)
		_tcscpy(pRHA->JDarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->DLarray.size(); i++)
		_tcscpy(pRHA->DLarray[i].XLName, pRHA->m_XLName);

	for (int i=0; i<pRHA->BZLCarray.size(); i++)
		_tcscpy(pRHA->BZLCarray[i].XLName, pRHA->m_XLName);


	if(xlShuXing != LSX && xlShuXing != SGBX)
	{
		//原来的线路是施工便线或临时线
		//，目标是转为既有线或设计线（全局指针）时
		if(oldXLShuXing == LSX || oldXLShuXing == SGBX)
		{
			//
			if (ix<0)//线路没有构建
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
			else			
				g_DocManageAry[iDoc].XLAry[ix]=pRHA;
		}
	}
	else
	{
		//原来的线路是既有线或设计线（全局指针）
		//，目标是转为临时线或施工便线时
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

			//pRHA->erase();//818 注意，pRHA需打开写

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
函数功能: 多段线转线路实体（既有线、设计线、临时线、施工便线)
输入参数:
xlName                  目标线路名
eIdToIn                 拾取的多段线ID
xlShuXing    			目标线路属性
mdbName     			数据库名（包括路径）
函数返回值: 
编写：ls 2016.11.7
***************************************************************/
void CCreatPM::PLinetoRHA(CString xlName , AcDbObjectId eIdToIn , CString xlShuXing, CString mdbName)
{
	double R;
	double L1;
	double L2;

	BAS_CALC_FUN BS;
	BS.ReadGuifanFile();
	if (xlName.Find(L"改建")>-1)
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
				{  //寻找的某顶点
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
				{  //寻找 的某顶点
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
	//如果线路存在，删除(818 erase??)

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
		eId=BAS_DRAW_FUN::AddEntityToDbs(g_DocManageAry[iDoc].XLAry[j]);//绘图
	}
	else
	{
		if (xlShuXing == LSX)
			pRHA = new RHA(JDarray, GH, JDGH, xlName, L"C:\\未命名.mdb", xlShuXing);
		else if (xlShuXing == SGBX)
			pRHA = new RHA(JDarray, GH, JDGH, xlName, mdbName, xlShuXing);
		
		pRHA->setColorLineW();
		eId=BAS_DRAW_FUN::AddEntityToDbs(pRHA);
	}

	//打开列表编辑
	CPMSJ::ListXLInfor(FALSE, eId);

	//删除实体
	AcDbEntity *pEnt;
	if(acdbOpenObject(pEnt, eIdToIn, AcDb::kForWrite)==Acad::eOk)
	{
		pEnt->erase(Adesk::kTrue);
		pEnt->close();
		return;  
	}
	else
		ads_printf(L"删除实体失败!\n");

	return;
}

//获取当前窗口,输出窗口在g_DocManageAry中的下标
bool CCreatPM::CurDocIndx(int &iDoc)//818 有些地方不用获取iDoc了，检查一下
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

//如果线路在全局数组中已经存在,就删除图形，给线路赋空
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


//如果线路在全局数组中已经存在,就删除图形，删除线路
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
函数功能: 创建所有的既有线和设计线的实体
输入参数:
mdbName     			数据库名（包括路径）
iDoc    			    窗口下标
输出参数:
g_DocManageAry          全局线路数组
编写：李帅 2016
***************************************************************/
void CCreatPM::CreatAllJYXandSJX(CString mdbName, int iDoc)
{
	XLDataBase xlmdb;
	RHA *pRHA;
	CString tmpStr ;
	CArray<XLInforTableStruct, XLInforTableStruct> xlInforAry;//线路信息 

	//读取所有线路名
	xlInforAry.RemoveAll();
	if(xlmdb.Read_XLDbs(mdbName,L"线路信息表",L""))	
	{
		if(xlmdb.XLInforNum>0)
		{
			for (int i=0;i<xlmdb.XLInforNum;i++)
				xlInforAry.Add(xlmdb.XLInforTable[i]);
		}
	}

	//形成线路信息数组
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
	
	//生成所有线路
	if (iDoc<0)
		CurDocIndx(iDoc);

	for (int i=0; i<xlInforAry.GetSize(); i++)
	{
		if (IsIncluded(iDoc, mdbName, xlInforAry[i].XLName)<0)
		{
			pRHA=CreatGlobalRHA(mdbName, xlInforAry[i].XLName, false);
			if(pRHA == NULL)
				ads_printf(L"创建线路失败！\n");
			else
				g_DocManageAry[iDoc].XLAry.push_back(pRHA);
		}
	}
}


//功能：从当前窗口中 获取在系统的全局数组中 某数据库 的全部线路信息
//输入：数据库名
//输出：线路信息数组
//编写：17.8.18
void CCreatPM::GetSysXLInfbyMdbName(CString mdbName, CArray<XLInforTableStruct, XLInforTableStruct> &xlInfAry)
{
	xlInfAry.RemoveAll();
	//当前文档指针
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