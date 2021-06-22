// FastTin.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "FastTin.h"
#include "triDTM_road.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GridWidth 100
#define MaxLen 1000


#define FindOppTri(j,ajtno,trino)  j=0;\
	if(ajtno!=-1)\
{\
	while(j<3 && trino!=TriList[ajtno].Ajt[j])\
	j++;\
}\
	//在ajtno中找到与trino相对的顶点号（0，1，2)
/////////////////////////////////////////////////////////////////////////////
// CFastTin dialog

double PtDis=30,DeltaH=1;

CFastTin::CFastTin(CWnd* pParent /*=NULL*/)
	: CDialog(CFastTin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastTin)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	NRoad = 0;
	trivectmp = NULL;
	ptvec=NULL;
	TriListHead=NULL;
	TriList=NULL; 
	ptdata=NULL;
	GridArray=NULL;
	GridIdx=NULL;
	StackTop=NULL;
	ImpactTriHead=NULL;

	headptr=NULL;
	tailptr=NULL;
	ImpactHead=NULL;
	ImpactTail=NULL;

	EdgeListHead=NULL;
	EdgeListTail=NULL;
	LeftVerList=NULL;
	RightVerList=NULL;

}


void CFastTin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastTin)
	//	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFastTin, CDialog)
	//{{AFX_MSG_MAP(CFastTin)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastTin message handlers
void CFastTin::DealBriBjx()//对桥梁边界线处理 得到高出地面的桥梁边界
{
	//读数模
	int i,j,Pnum,k,Pos,SumEdge=0,CurEdge=0;
	bool delinf;
	EdgeNode *CrsDmxHead,*Tmp;
	double minh,zl;
	//	if(!ptriDTM)
	//		ptriDTM = (triDTM_road *)new triDTM_road;
	if(ptriDTM.DTMHasRead==0)
	{
		if(!ptriDTM.Read_Dtm())//数模不存在
		{
			for(i=0;i<NRoad;i++)
				RoadHole[i]->NBriBj=0;
			return;
		}
		for(i=0;i<NRoad;i++)
			SumEdge+=RoadHole[i]->NBriBj;

		for(i=0;i<NRoad;i++)
		{
			for(j=0;j<RoadHole[i]->NBriBj;j++)
			{
				CurEdge++;
				Pos = (int)(CurEdge*100.0/SumEdge);
				acedSetStatusBarProgressMeterPos(Pos);
				delinf = true;					
				minh = RoadHole[i]->BriBjArr[j].qpt[2]<RoadHole[i]->BriBjArr[j].hpt[2]?RoadHole[i]->BriBjArr[j].qpt[2]:RoadHole[i]->BriBjArr[j].hpt[2];
				Pnum = 0;
				CrsDmxHead = ptriDTM.FindInterPt(RoadHole[i]->BriBjArr[j].qpt,RoadHole[i]->BriBjArr[j].hpt,Pnum);	
				k=0;
				if(Pnum>0)//有点
				{

					while(CrsDmxHead)
					{
						zl =  CrsDmxHead->pt[2];						
						if(zl>minh)
							delinf = false;
						Tmp = CrsDmxHead;
						CrsDmxHead = CrsDmxHead->next; 
						free(Tmp);
						k++;
					}
				}

				if(delinf)
					RoadHole[i]->BriBjArr[j].qpt[0]=-9999999;
			}

			for(j=0;j<RoadHole[i]->BriBjArr.GetSize();j++)
				if(RoadHole[i]->BriBjArr[j].qpt[0]<-99999)
				{
					RoadHole[i]->BriBjArr.RemoveAt(j);
					j--;
					RoadHole[i]->NBriBj--;
				}

		}
		ptriDTM.ReleaseDtm();
	}
	else
	{
		for(i=0;i<NRoad;i++)
			RoadHole[i]->NBriBj=0;
	}	  
}

void CFastTin::FastCreateTin()
{
	FILE *fpr,*fpw;
	int gridId1, gridId2,count,gridNo,ptsum,k,i;
	struct BJNode *lbptr,*rbptr;
	ACHAR fname[256];
	CString strcs;

	CTime t1 = CTime::GetCurrentTime();

	/*
	strcs = _T("正在选择地形数据点，请稍候...");
	GetDlgItem(IDC_STATICstatus)->SetWindowText(strcs);	*/
	ACHAR pszLabel[20];
	_stprintf(pszLabel,L"调整桥梁中的地形");
	acedSetStatusBarProgressMeter(pszLabel,0,100);  

	DealBriBjx();

	_stprintf(pszLabel,L"构建整体三维模型");
	acedSetStatusBarProgressMeter(pszLabel,0,100);  
	SelectPoint();

	ThrowOffPt();

	//step1:点投入格网
	if(!ThrowPtInGrid())
		return; 

	//step2:按照点数从小到大建立格网索引
	if(!CreateGridIndex())
		return;

	//step3:生成初始三角形
	if(!CreateFirstTri())
		return;

	/*
	strcs = _T("正在构建地形三维模型，请稍候...");
	GetDlgItem(IDC_STATICstatus)->SetWindowText(strcs);			*/

	//	 m_progress.SetRange(0,100);
	//    m_progress.SetPos(0);
	//	 m_progress.SetShowText(TRUE);

	//step4:针对排好序的格网链表依次取点插入
	gridId1=gridId2=count=0;
	ptsum=0;
	int Pos;

	vector< GridIndex> watchGrid;
	for (int kk = 0; kk < NCol * NRow; kk++)
		watchGrid.push_back(GridIdx[kk]);

	vector< Triangle> watchTri, PreTri;

	while(gridId1<IdxSum)
	{	    
		while(gridId2<IdxSum)
		{
			gridNo = GridIdx[gridId2].GridNo;

			PreTri = watchTri;

			IncrementInsertPt(GridArray[gridNo].Pncur);//逐点插入过程
			ptsum++;
			//			 m_progress.SetPos((int)(ptsum*100.0/RealPTSum));
			if(fmod(1.0*ptsum,1000)==0)
			{
				Pos = (int)(ptsum*100.0/RealPTSum);
				acedSetStatusBarProgressMeterPos(Pos);
			}
			GridArray[gridNo].Pncur = ptdata[GridArray[gridNo].Pncur].cur;//指定该网格的下一次的待插点

			watchTri.clear();
			for (int kk = 0; kk < TriListTailNo; kk++)
				watchTri.push_back(TriList[kk]);

			gridId2++;
		}
		count++;

		while(count == GridIdx[gridId1].ptsum)
			gridId1++;
		gridId2 = gridId1;		 
	}

	//   for(i=0; i<=TriListTailNo; i++)
	//   {	   
	// //	  if(TriList[i].Attribute=='N')
	// 	    CreateAFace(ptdata[TriList[i].vertex[0]].pt,ptdata[TriList[i].vertex[1]].pt,ptdata[TriList[i].vertex[2]].pt,7);	                                    
	//   }

	AdjustBriBj();

	AdjustBjx();//因边界上的重点在throwoffpt中被删除，此时需修正洞的边界点，使地面点数组中肯定包括边界点
	count=0;
	DigHole();

	FormInitEgdes();

	FILE* fprr;
	_stprintf(fname, L"%s\\3d\\TriSecondForWatch.txt", Cworkdir);
	fprr = _wfopen(fname, L"w");
	fwprintf(fprr, L"%d\n", PreTri.size());
	for (int kk = 0; kk < PreTri.size(); kk++)
	{
		fwprintf(fprr, L"Num:\t%d\t%d\t%d\t%d\n", kk, PreTri[kk].vertex[0], PreTri[kk].vertex[1], PreTri[kk].vertex[2]);
	}
	fclose(fprr);

	struct PtResult
	{
		float pt[3];
	} PtRes;

	struct TriResult
	{
		int vertex[3];
		//		char Attribute;//属性：代表三角形是否有效,'Y'有效;'N'无效
	} TriRes;

	double hmin,hmax;
	hmin = 99999999.0;
	hmax = -99999999.0;

	_stprintf(fname ,L"%s\\3d\\3dmodel_pt",Cworkdir);
	fpw = _wfopen(fname,L"wb");
	for(i=0; i<PTSUM;i++)
	{
		PtRes.pt[0] = ptdata[i].pt[0]-LeftLowPt[0];
		PtRes.pt[1] = ptdata[i].pt[1]-LeftLowPt[1];
		PtRes.pt[2] = ptdata[i].pt[2];
		fwrite(&PtRes,sizeof(struct PtResult),1,fpw);
		hmin = hmin < PtRes.pt[2] ? hmin : PtRes.pt[2];
		hmax = hmax > PtRes.pt[2] ? hmax : PtRes.pt[2];
	}
	fclose(fpw);

	int trisum=0;
	_stprintf(fname ,L"%s\\3d\\3dmodel_tri",Cworkdir);
	fpw = _wfopen(fname,L"wb");	
	for(i=0; i<TriListTailNo;i++)
	{
		if(TriList[i].Attribute=='Y')
		{
			TriRes.vertex[0] = TriList[i].vertex[0];
			TriRes.vertex[1] = TriList[i].vertex[1];
			TriRes.vertex[2] = TriList[i].vertex[2];
			//			TriRes.Attribute = TriList[i].Attribute;
			fwrite(&TriRes,sizeof(struct TriResult),1,fpw);
			trisum++;
		}
	}
	fclose(fpw);	

	acedSetStatusBarProgressMeterPos(100);

	_stprintf(pszLabel,L"正在计算顶点纹理坐标");
	acedSetStatusBarProgressMeter(pszLabel,0,100); 
	Cal_PtNormal();

	/*
	for(i=0; i<=TriListTailNo; i++)
	{	   
	if(TriList[i].Attribute=='N')
	CreateAFace(ptdata[TriList[i].vertex[0]].pt,ptdata[TriList[i].vertex[1]].pt,ptdata[TriList[i].vertex[2]].pt,7);	                                    
	}*/  
	/*
	for(i=0; i<=TriListTailNo; i++)
	{	   
	if(TriList[i].Attribute=='Y')
	CreateAFace(ptdata[TriList[i].vertex[0]].pt,ptdata[TriList[i].vertex[1]].pt,ptdata[TriList[i].vertex[2]].pt,3);	                                    
	}  */


	UnmapViewOfFile(FilePtrPp);
	CloseHandle(MEMmapfileHandlePp);
	CloseHandle(mapfileHandlePp);

	FilePtrTt = (LPVOID *)TriList;
	UnmapViewOfFile(FilePtrTt);
	CloseHandle(MEMmapfileHandleTt);
	CloseHandle(mapfileHandleTt);

	if(GridArray) free(GridArray);
	if(GridIdx) delete []GridIdx;

	acedSetStatusBarProgressMeterPos(100);   
	acedRestoreStatusBar();
	//ReadRoad();

	_stprintf(fname ,L"%s\\3d\\3dmap",Cworkdir);
	fpw = _wfopen(fname,L"w");
	fwprintf(fpw,L"%d %d\n",PTSUM,trisum);
	fwprintf(fpw,L"%lf %lf %lf %lf\n",LeftLowPt[0],LeftLowPt[1],RightUpPt[0],RightUpPt[1]);
	fwprintf(fpw,L"%lf %lf\n",hmin,hmax);
	fclose(fpw); 


	CTime t2 = CTime::GetCurrentTime();
	CTimeSpan ts = t2 - t1;  // Subtract 2 CTimes
	CString s = ts.Format( "地形三维模型建立完毕, 建模耗时：%H小时%M分%S秒" );
	AfxMessageBox(s);

} 

//删除废点,重新写SPT文件,生成3dmodel文件
int CFastTin::ThrowOffPt()
{
	CString  fname;
	int memb,i,j,k,l,jj,inf;
	double dis,ToViewPtDis,MinDis;
	FILE *fpw1,*fpw,*fpr;
	struct Triangle Tri;

	fname = Cworkdir+"\\3d\\spt."+Pro;

	// if(ptdata) delete []ptdata;
	ptdata=NULL;

	if(PTSUM<1)
		return 0;

	ptdata =  new struct PT[PTSUM];
	fpr = _wfopen(fname,L"rb");
	i=fread(ptdata, sizeof(struct PT), PTSUM, fpr);
	fclose(fpr);	   
	/*
	_stprintf(fname,L"%s\\3d\\spt.%s",Cworkdir,Pro);   
	mapfileHandlePp = CreateFile(fname,          // pointer to name of the file
	GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
	FILE_SHARE_WRITE,    // share mode
	NULL,  // pointer to security attributes
	OPEN_EXISTING,  // how to create
	FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
	NULL);
	_tcscpy(fname,L"MemMapPp");
	MEMmapfileHandlePp = CreateFileMapping(mapfileHandlePp,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrPp = MapViewOfFile(MEMmapfileHandlePp,FILE_MAP_WRITE,0,0,0);
	ptdata =  (struct PT *)FilePtrPp;*/


	if(ptdata==NULL)
	{
		AfxMessageBox(L"系统资源不足,无法开辟空间,请重新启动或清理磁盘1!");
		return 0;
	}
	NRow = (int)((RightUpPt[0] - LeftLowPt[0])/GridWidth+0.001)+1;
	NCol = (int)((RightUpPt[1] - LeftLowPt[1])/GridWidth+0.001)+1;

	memb = NRow * NCol;
	if((GridArray = (struct GridNode *)calloc(memb,sizeof(struct GridNode))) == NULL)
	{
		AfxMessageBox(L"网格数组分配内存失败!");
		return  0;
	}      

	GridIdx = new struct GridIndex[NRow * NCol];
	//付初值
	for (i = 0; i < NRow; i++)
	{
		for (j = 0; j < NCol; j++)
		{
			GridArray[i * NCol + j].Pfcur = -1;
			GridArray[i * NCol + j].Ptcur = -1;
			GridArray[i * NCol + j].Pncur = -1;
			GridArray[i * NCol + j].PtSum = 0;
			GridIdx[i * NCol + j].GridNo = i * NCol + j;
			GridIdx[i * NCol + j].ptsum = 0;
		}
	}

	RealPTSum = 4;
	//	  ads_printf(L"%d xmin=%lf ymin=%lf\n",PTSUM,LeftLowPt[0],LeftLowPt[1]);
	for (k = 4; k < PTSUM; k++)
	{
		//		   ads_printf(L"%d x=%0.3lf y=%0.3lf  %c xm=%0.3lf ym=%0.3lf\n",k,ptdata[k].pt[0],ptdata[k].pt[1],ptdata[k].att,LeftLowPt[0],LeftLowPt[1]);
		i = (int)((ptdata[k].pt[0] - LeftLowPt[0]) / GridWidth + 0.001);
		j = (int)((ptdata[k].pt[1] - LeftLowPt[1]) / GridWidth + 0.001);
		l = GridArray[i * NCol + j].Ptcur;

		if (l == -1)
		{
			GridArray[i * NCol + j].Pfcur = k;
			GridArray[i * NCol + j].Pncur = k;
			RealPTSum++;
		}
		else
		{
			jj = GridArray[i * NCol + j].Pfcur;
			inf = 1;
			while (jj != ptdata[l].cur)
			{
				// if(fabs(ptdata[jj].pt[0]-ptdata[k].pt[0])<0.001 &&fabs(ptdata[jj].pt[1]-ptdata[k].pt[1])<0.001)
				dis = sqrt((ptdata[jj].pt[0] - ptdata[k].pt[0]) * (ptdata[jj].pt[0] - ptdata[k].pt[0]) + (ptdata[jj].pt[1] - ptdata[k].pt[1]) * (ptdata[jj].pt[1] - ptdata[k].pt[1]));

				//				 if(k<=NCrs*2+4 && k>4 && dis<0.001)
				// 
				if (k > 4 && ptdata[k].att == 'c' && dis < 0.0001)//特征点
				{
					inf = 0;
					ptdata[k].att = 'd'; //标志该点已删除
					ptdata[jj].att = 'c';
					break;
				}

				//			 if(k>NCrs*2+4 && dis<PtDis)
				if (k > 4 && ptdata[k].att != 'c' && dis < PtDis)
				{
					inf = 0;
					ptdata[k].att = 'd'; //标志该点已删除
					break;
				}
				jj = ptdata[jj].cur;
			}
			if (inf)
			{
				ptdata[l].cur = k;
				RealPTSum++;
			}
			else
				continue;
		}
		GridArray[i * NCol + j].Ptcur = k;
		GridArray[i * NCol + j].PtSum++;

		ptdata[k].trino = -1;
	}

	fname = Cworkdir + "\\3d\\3dmodel." + Pro;
	if ((fpw1 = _wfopen(fname, L"wb")) == NULL)
		return 0;

	for (i = 0; i < 3; i++)
	{
		Tri.Ajt[i] = -1;
		Tri.vertex[i] = -1;
		Tri.EdgeInf[i] = 'a';
	}
	Tri.Attribute = 'N';

	fname = Cworkdir + "\\3d\\Spt." + Pro;
	if ((fpw = _wfopen(fname, L"wb")) == NULL)
		return 0;

	RealPTSum = 0;
	for (k = 0; k < PTSUM; k++)
	{
		if (ptdata[k].att != 'd')
		{
			ptdata[k].cur = -1;
			fwrite(&ptdata[k], sizeof(struct PT), 1, fpw);
			fwrite(&Tri, sizeof(struct Triangle), 1, fpw1);
			fwrite(&Tri, sizeof(struct Triangle), 1, fpw1);
			RealPTSum++;
		}
	}

	PTSUM = RealPTSum;
	fclose(fpw);
	fclose(fpw1);
	delete[]ptdata;
	return 1;

}

//--------------------------------------------------------------------------------
//            step 1 : 将点投入格网, 格网宽度100M
//--------------------------------------------------------------------------------
int CFastTin::ThrowPtInGrid()
{
	ACHAR fname[256],str[80];
	int memb,i,j,k,l,jj,inf;
	double dis,ToViewPtDis,MinDis;

	_stprintf(fname,L"%s\\3d\\spt.%s",Cworkdir,Pro);   
	mapfileHandlePp = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);

	//  _tcscpy(fname,L"MemMapPp");
	_stprintf(fname,L"%d",rand());
	MEMmapfileHandlePp = CreateFileMapping(mapfileHandlePp,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrPp = MapViewOfFile(MEMmapfileHandlePp,FILE_MAP_WRITE,0,0,0);
	/////////此处有问题
	// if(ptdata) delete []ptdata;

	ptdata =  (struct PT *)FilePtrPp;

	if(ptdata==NULL)
	{
		AfxMessageBox(L"系统资源不足,无法开辟空间,请重新启动或清理磁盘2!");
		return 0;
	}
	/*
	NRow = (int)((RightUpPt[0] - LeftLowPt[0])/GridWidth+0.001)+1;
	NCol = (int)((RightUpPt[1] - LeftLowPt[1])/GridWidth+0.001)+1;

	memb = NRow * NCol;
	if((GridArray = (struct GridNode *)calloc(memb,sizeof(struct GridNode))) == NULL)
	{
	AfxMessageBox(L"网格数组分配内存失败!");
	return  0;
	}      

	GridIdx = new struct GridIndex[NRow * NCol];*/

	//付初值
	for(i=0; i<NRow; i++)
		for(j=0;j<NCol;j++)
		{
			GridArray[i*NCol+j].Pfcur = -1;
			GridArray[i*NCol+j].Ptcur = -1;
			GridArray[i*NCol+j].Pncur = -1;
			GridArray[i*NCol+j].PtSum = 0;
			GridIdx[i*NCol+j].GridNo = i*NCol+j;
			GridIdx[i*NCol+j].ptsum =  0;			 
		}

		RealPTSum = PTSUM;

		for(k = 4; k <  PTSUM; k++)
		{
			i = (int)((ptdata[k].pt[0] - LeftLowPt[0]) / GridWidth+0.001);
			j = (int)((ptdata[k].pt[1] - LeftLowPt[1]) / GridWidth+0.001);
			l = GridArray[i*NCol+j].Ptcur;

			if(l == -1)
			{
				GridArray[i*NCol+j].Pfcur = k;
				GridArray[i*NCol+j].Pncur = k;
			}
			else
				ptdata[l].cur = k;			  
			/*
			jj = GridArray[i*NCol+j].Pfcur;
			inf = 1;
			while(jj != ptdata[l].cur)
			{

			dis = sqrt((ptdata[jj].pt[0]-ptdata[k].pt[0])*(ptdata[jj].pt[0]-ptdata[k].pt[0])+(ptdata[jj].pt[1]-ptdata[k].pt[1])*(ptdata[jj].pt[1]-ptdata[k].pt[1]));
			if(k>4 &&ptdata[k].att == 'c'&& dis<0.001)
			{
			inf = 0;
			ptdata[k].att = 'd'; //标志该点已删除
			break;
			}				 
			if(k>4 &&ptdata[k].att != 'c'&&dis<PtDis)
			{
			inf = 0;
			ptdata[k].att = 'd'; //标志该点已删除
			break;
			}

			jj = ptdata[jj].cur;				 
			}
			if(inf)
			{
			ptdata[l].cur = k;
			RealPTSum++;
			}
			else
			continue;  */

			GridArray[i*NCol+j].Ptcur = k;  
			GridArray[i*NCol+j].PtSum++;		  

			ptdata[k].trino=-1;
		}



		return 1;
}

void CFastTin::Cal_TinNormal()
{
	int i;	

	if(trivectmp) delete []trivectmp;
	trivectmp = new struct TriVect[TriListTailNo];
	for(i=0; i < TriListTailNo; i++)
	{
		if(TriList[i].Attribute =='Y')
		{ 
			Vect3d a(ptdata[TriList[i].vertex[0]].pt[0]-ptdata[TriList[i].vertex[1]].pt[0],
				ptdata[TriList[i].vertex[0]].pt[1]-ptdata[TriList[i].vertex[1]].pt[1],
				ptdata[TriList[i].vertex[0]].pt[2]-ptdata[TriList[i].vertex[1]].pt[2]);
			Vect3d b(ptdata[TriList[i].vertex[1]].pt[0]-ptdata[TriList[i].vertex[2]].pt[0],
				ptdata[TriList[i].vertex[1]].pt[1]-ptdata[TriList[i].vertex[2]].pt[1],
				ptdata[TriList[i].vertex[1]].pt[2]-ptdata[TriList[i].vertex[2]].pt[2]);
			trivectmp[i].trivec = a/b; //叉积
			trivectmp[i].trivec.Normalize();//归一化
		}
	}
}

void CFastTin::Cal_PtNormal()
{
	int i,j,BaseTrino,StartVerNo,inf,trino,cur,Pos;
	float trinum;
	ACHAR mes[80];

	Cal_TinNormal();

	ptvec = new struct PtVect[PTSUM];
	for(i=0; i<PTSUM; i++)
	{ 
		Pos = (int)(i*100.0/PTSUM);
		acedSetStatusBarProgressMeterPos(Pos);

		trino = ptdata[i].trino;
		BaseTrino = trino;	      
		StartVerNo = i; //i为当前顶点,找出以ptdata[i]为顶点的所有三角形
		//		cur = ptdata[i].cur;
		if(trino==-1)
		{
			ptvec[i].costhita = 1;
			continue;
		}
		if(TriList[trino].Attribute=='Y')
		{
			ptvec[i].pvec = trivectmp[trino].trivec;
			trinum = 1.0;
		}
		else
		{
			ptvec[i].pvec.Set(0,0,0);
			trinum = 0.0;
		}
		inf = 1;	    
		//找出以ptdata[i]为顶点的所有三角形
		for(;;)
		{
			if(trino!=-1)
			{
				j=0;
				while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
					j++;
				if(j==3)
				{
					_stprintf(mes,L"trino=%d",trino);
					//			  ads_alert(mes);
				}
			}

			if(inf)
				trino = TriList[trino].Ajt[(j+1)%3];
			else if(inf==0)
				trino = TriList[trino].Ajt[(j+2)%3];

			if((inf&&trino==BaseTrino) || (inf==0&&trino==-1))//已经走了一圈
				break;
			if(trino != -1&&TriList[trino].Attribute=='Y')
			{
				ptvec[i].pvec += trivectmp[trino].trivec;		  
				trinum++;
			}

			if(inf&&trino==-1) 
			{
				inf = 0;
				trino = BaseTrino;//从最初位置反转
			}	 	 
		}//三角形循环
		if(trinum>0)
		{
			ptvec[i].pvec /= trinum;
			ptvec[i].pvec.Normalize();
			ptvec[i].costhita = ptvec[i].pvec.z;
		}
		else
			ptvec[i].costhita = 1;
	} //点循环

	FILE *fpw;
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\3d\\3dmodel_pt_texinf",Cworkdir);

	fpw = _wfopen(fname,L"wb");
	for(i=0; i<PTSUM;i++)
		fwrite(&ptvec[i].costhita,sizeof(float),1,fpw); 
	/*
	fpw = _wfopen(fname,L"w");
	for(i=0; i<PTSUM;i++)
	fwprintf(fpw,L"%lf\n",ptvec[i].costhita);
	fclose(fpw);*/

	delete[] trivectmp;//删除三角形法向量
	delete[] ptvec;//删除点法向量
}
//--------------------------------------------------------------------------------
//            step 2 : 按点数建立网格索引
//--------------------------------------------------------------------------------
int CFastTin::CreateGridIndex()
{
	int i,j;
	//	FILE *fpw;

	IdxSum = 0;

	for (i = 0; i < NRow; i++)
	{
		for (j = 0; j < NCol; j++)
		{
			if (GridArray[i * NCol + j].PtSum > 0)
			{
				GridIdx[IdxSum].GridNo = i * NCol + j;
				GridIdx[IdxSum].ptsum = GridArray[i * NCol + j].PtSum;
				IdxSum++;
			}
		}
	}

	vector< GridIndex> watchGrid;
	for (int kk = 0; kk < NCol * NRow; kk++)
		watchGrid.push_back(GridIdx[kk]);

	FastSort(GridIdx,0,IdxSum-1);

	return 1;
}

void CFastTin::FastSort(struct GridIndex R[],int s1,int t1)
{
	int i;
	if(s1<t1)
	{
		i = PARTITION(R,s1,t1);
		FastSort(R,s1,i-1);
		FastSort(R,i+1,t1);
	}
}
int CFastTin::PARTITION(struct GridIndex R[],int l,int h)
{
	int i,j;
	struct GridIndex temp;

	i = l;
	j = h;

	temp = R[i];
	do{
		while((R[j].ptsum>=temp.ptsum) && (i<j))
			j--;
		if(i<j) 
			R[i++] = R[j];
		while((R[i].ptsum<=temp.ptsum) && (i<j))
			i++;
		if(i<j) 
			R[j--] = R[i];
	}while(i!=j);
	R[i] = temp;
	return i;
}
int CFastTin::CreateFirstTri()
{
	int i;
	ACHAR fname[256];

	_stprintf(fname,L"%s\\3d\\3dmodel.%s",Cworkdir,Pro);   
	mapfileHandleTt = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);
	//_tcscpy(fname,L"MemMapTt");
	_stprintf(fname,L"%d",rand());
	MEMmapfileHandleTt = CreateFileMapping(mapfileHandleTt,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrTt = MapViewOfFile(MEMmapfileHandleTt,FILE_MAP_WRITE,0,0,0);
	if(TriList) delete []TriList;
	TriList =  (struct Triangle *)FilePtrTt;

	if(TriList == NULL)
	{
		AfxMessageBox(L"系统资源不足,无法开辟空间2,请重新启动计算机或清理磁盘!");
		return 0;
	}
	for(i=0; i<3; i++)
		TriList[0].vertex[i] = i;

	_tcscpy(TriList[0].EdgeInf,L"aaa");
	TriList[0].Attribute = 'Y';

	TriList[1].vertex[0] = 2;
	TriList[1].vertex[1] = 3;
	TriList[1].vertex[2] = 0;

	_tcscpy(TriList[1].EdgeInf,L"aaa");

	TriList[0].Ajt[0] = -1;
	TriList[0].Ajt[1] = 1;
	TriList[0].Ajt[2] = -1;
	TriList[1].Ajt[0] = -1;
	TriList[1].Ajt[1] = 0;
	TriList[1].Ajt[2] = -1;	
	TriList[1].Attribute = 'Y';
	for(i=0; i<3; i++)
		ptdata[i].trino = 0;
	ptdata[3].trino = 1;
	TriListTailNo = 1;

	return 1; 
}

int CFastTin::IncrementInsertPt(int ptnum)//逐点插入过程
{
	//	struct Triangle *tri;
	int trino;
	double pt[2],Zori,Znc;
	//定位点所位于的三角形
	TRACE(L"ptnum=%d \n",ptnum);
	trino = LocateTri(ptnum);

	pt[0]=ptdata[ptnum].pt[0],pt[1]=ptdata[ptnum].pt[1];
	//比较当前点的实际高程与该三角形内插高程
	if(ptdata[ptnum].att =='a')
	{
		Znc = interpolate(pt,trino);
		Zori = ptdata[ptnum].pt[2];
		if(fabs(Zori-Znc)<DeltaH)//小于限值
		{
			ptdata[ptnum].att = 'd';//标志该点已删除
			return 1;
		}
	}

	if(ptdata[ptnum].att =='d')
	{			
		return 1;
	}

	//找出影响域
	if(trino!=-1)
		FindImpactArea(trino,ptnum);
	else
	{
		ptdata[ptnum].att = 'd';		
		//		ads_printf(L"null tri! %d %lf %lf\n",ptnum,ptdata[ptnum].pt[0],ptdata[ptnum].pt[1]);
	}
	return 1;
}

//以网格中一点与pt的连线方向快速定位PT所在的三角形 2001.6.19
int CFastTin::LocateTri(int ptnum)
{
	int i,j,verno;
	int trino,StartVerNo,inf,StartTriNo=-1,pretno,ajt,vno,ajt1,LoopNum=0,BaseTrino;
	double cw1,cw2,cw,pt[2];
	ACHAR mes[280];
	//找出三角形
	//找出三角形
	//	TRACE("ptnum=%d\n",ptnum);
	i = (int)((ptdata[ptnum].pt[0] - LeftLowPt[0])/GridWidth+0.001);
	j = (int)((ptdata[ptnum].pt[1] - LeftLowPt[1])/GridWidth+0.001);

	trino = ptdata[GridArray[i*NCol+j].Pfcur].trino;
	if(trino==-1)
		trino = TriListTailNo;
	BaseTrino = trino;

	StartVerNo = TriList[trino].vertex[0]; //取出首三角形的点号
	pt[0] = ptdata[ptnum].pt[0],pt[1] = ptdata[ptnum].pt[1];
	inf = 1;
	do
	{
		if(trino!=-1)
		{
			j=0;
			while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
				j++;
			cw1 = CCW2(ptdata[StartVerNo].pt,pt,ptdata[TriList[trino].vertex[(j+1)%3]].pt);
			cw2 = CCW2(ptdata[StartVerNo].pt,pt,ptdata[TriList[trino].vertex[(j+2)%3]].pt);
		}

		if(trino==-1 || trino==StartTriNo) //转到一头或一个三角形在一条线上,换一方向继续找
		{
			if(inf==1)
				inf = 0;
			else
				return -1;
			trino = BaseTrino;//从最初位置反转
			j=0;
			while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
				j++;
		}
		StartTriNo = trino;
		if(inf)
			trino = TriList[trino].Ajt[(j+1)%3];
		else if(inf==0)
			trino = TriList[trino].Ajt[(j+2)%3];
		//	 TRACE("trino=%d %d %d %d %d\n",trino,TriList[trino].vertex[0],TriList[trino].vertex[1],TriList[trino].vertex[2],StartVerNo);
	} while(cw1>0.0 || cw2 <0.0) ;//该过程在以StartVerNo为顶点一圈三角形中，找出StartVerNo-EndVerNo穿越的三角形

	pretno = StartTriNo;
	ajt = pretno;
	vno = j;
	//TRACE("j=%d\n",vno);
	//   CreateAFace(ptdata[TriList[pretno].vertex[0]].pt,ptdata[TriList[pretno].vertex[1]].pt,ptdata[TriList[pretno].vertex[2]].pt,1);	                                    
	if(CheckPtInTri(StartTriNo, pt) == 1)
		return StartTriNo; 


	for(;;)
	{
		ajt = TriList[pretno].Ajt[vno];
		//   TRACE("vno=%d ajt=%d\n",vno,ajt);
		if(ajt<0)
			return -1;
		if(CheckPtInTri(ajt, pt) == 1)
			return ajt;

		FindOppTri(j,ajt,pretno);
		verno = TriList[ajt].vertex[j];
		cw = CCW2(ptdata[StartVerNo].pt,pt,ptdata[verno].pt);

		if(cw >0.0)
		{
			ajt1 = TriList[ajt].Ajt[(j+2)%3]; 
			//	 TRACE("1:ajt1=%d\n",ajt1);
			FindOppTri(vno,ajt1,ajt);
			vno = (j+1)%3;
		}
		else
		{
			ajt1 = TriList[ajt].Ajt[(j+1)%3]; 
			//	 TRACE("2:ajt1=%d\n",ajt1);
			FindOppTri(vno,ajt1,ajt);
			vno = (j+2)%3;
		}
		pretno = ajt;
	}
}

//判断三角形trino内是否包含点PT 是 返回1 否 返回0 2001.6.19
int CFastTin::CheckPtInTri(int trino, double pt[2])
{
	double cg[2],pt0[2],pt1[2],pt2[2];
	double snp01,snp12,snp20;

	pt0[0] = ptdata[TriList[trino].vertex[0]].pt[0] , pt0[1] = ptdata[TriList[trino].vertex[0]].pt[1];
	pt1[0] = ptdata[TriList[trino].vertex[1]].pt[0] , pt1[1] = ptdata[TriList[trino].vertex[1]].pt[1];
	pt2[0] = ptdata[TriList[trino].vertex[2]].pt[0] , pt2[1] = ptdata[TriList[trino].vertex[2]].pt[1];
	//重心
	//      cg[0] = (pt0[0]+pt1[0]+pt2[0])/3.0;
	//	  cg[1] = (pt0[1]+pt1[1]+pt2[1])/3.0;

	/*      snp01 = sign(chkd(pt0,pt1,pt));
	snp12 = sign(chkd(pt1,pt2,pt));
	snp20 = sign(chkd(pt2,pt0,pt)); */

	snp01 = chkd(pt0,pt1,pt);
	snp12 = chkd(pt1,pt2,pt);
	snp20 = chkd(pt2,pt0,pt); 

	//	  sng01 = sign(chkd(pt0,pt1,cg));
	//	  sng12 = sign(chkd(pt1,pt2,cg));
	//	  sng20 = sign(chkd(pt2,pt0,cg));

	if(snp01>=-0.00000001 && snp12>=-0.00000001 && snp20>=-0.00000001)
		return 1;
	else
		return 0;
}

int  CFastTin::LocateTriOnXY(double pt[2])
{
	int i,j,ttrue,verno;
	int trino,StartVerNo,inf,StartTriNo,pretno,ajt,vno,ajt1,gridno;
	double cg[2],pt0[2],pt1[2],pt2[2],cw1,cw2,cw;
	ACHAR mes[256];
	StartTriNo=-1;
	//找出三角形
	i = (int)((pt[0] - LeftLowPt[0])/GridWidth+0.001);
	j = (int)((pt[1] - LeftLowPt[1])/GridWidth+0.001);
	if(i<0 || i>=NRow || j<0 || j>=NCol)
	{
		//	_stprintf(mes,L"%lf %lf",pt[0],pt[1]);
		//	AfxMessageBox(mes);
		return -1;
	}
	if(GridArray[i*NCol+j].Pfcur!=-1)
	{
		trino = ptdata[GridArray[i*NCol+j].Pfcur].trino;
		StartVerNo = GridArray[i*NCol+j].Pfcur; //取出网格中一点
	}
	else
	{
		gridno = FindFirstNoEmptyGridNo(i,j);
		if(gridno==-1)
			return -1;
		trino = ptdata[GridArray[gridno].Pfcur].trino;
		StartVerNo = GridArray[gridno].Pfcur; //取出网格中一点      	
	}

	inf = 1;
	do
	{
		if(trino!=-1)
		{
			j=0;
			while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
				j++;
			cw1 = CCW2(ptdata[StartVerNo].pt,pt,ptdata[TriList[trino].vertex[(j+1)%3]].pt);
			cw2 = CCW2(ptdata[StartVerNo].pt,pt,ptdata[TriList[trino].vertex[(j+2)%3]].pt);
		}
		//////////此处有问题
		if(trino==-1 || trino==StartTriNo) //转到一头或一个三角形在一条线上,换一方向继续找
		{
			if(inf==1)
				inf = 0;
			else
				return -1;
			trino = ptdata[StartVerNo].trino;
			if(trino==-1)
				return -1;
			j=0;
			while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
				j++;
		}
		StartTriNo = trino;
		if(inf)
			trino = TriList[trino].Ajt[(j+1)%3];
		else if(inf==0)
			trino = TriList[trino].Ajt[(j+2)%3];
	} while(cw1>0.0 || cw2 <0.0) ;//该过程在以StartVerNo为顶点一圈三角形中，找出StartVerNo-EndVerNo穿越的三角形

	pretno = StartTriNo;
	ajt = pretno;
	vno = j;
	//   CreateAFace(ptdata[TriList[pretno].vertex[0]].pt,ptdata[TriList[pretno].vertex[1]].pt,ptdata[TriList[pretno].vertex[2]].pt,1);	                                    
	if(CheckPtInTri(StartTriNo, pt) == 1)
		return StartTriNo;
	for(;;)
	{
		ajt = TriList[pretno].Ajt[vno];
		if(ajt<0)
			return -1;
		if(CheckPtInTri(ajt, pt) == 1)
			return ajt;

		FindOppTri(j,ajt,pretno);
		verno = TriList[ajt].vertex[j];
		cw = CCW2(ptdata[StartVerNo].pt,pt,ptdata[verno].pt);

		if(cw >0.0)
		{
			ajt1 = TriList[ajt].Ajt[(j+2)%3]; 
			FindOppTri(vno,ajt1,ajt);
			vno = (j+1)%3;
		}
		else
		{
			ajt1 = TriList[ajt].Ajt[(j+1)%3]; 
			FindOppTri(vno,ajt1,ajt);
			vno = (j+2)%3;
		}
		pretno = ajt;
	}
}

double CFastTin::chkd(double pt11[3],double pt22[3],double pt33[3])
{
	double a,b,c;
	double pt1[2],pt2[2],pt3[2],x,y;

	x = (pt11[0]+pt22[0])/2.0;
	y = (pt11[1]+pt22[1])/2.0;
	pt1[0] = pt11[0] -x;
	pt1[1] = pt11[1]- y;
	pt2[0] = pt22[0]-x;
	pt2[1] = pt22[1] -y;
	pt3[0] = pt33[0]- x;
	pt3[1] = pt33[1] - y;

	c = CCW2(pt1,pt2,pt3);

	return (c);
}

int CFastTin::sign(double result)
{
	if(result > 0.0)
		return 1;
	else if(result < 0.0)
		return -1;
	else
		return 0;

}
double CFastTin::CCW2(double pt1[2],double pt2[2],double pt3[2]) 
{
	double x1,x2,x3,y1,y2,y3,bx,by;

	bx = (pt1[0]+pt2[0]+pt3[0]) /3.0;
	by = (pt1[1]+pt2[1]+pt3[1]) /3.0;
	x1 = pt1[0]-bx;
	x2 = pt2[0]-bx;
	x3 = pt3[0]-bx;
	y1 = pt1[1]-by;
	y2 = pt2[1]-by;
	y3 = pt3[1]-by;
	return (x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2));
}

int CFastTin::FindImpactArea(int FirstTriNo,int ptnum)
{
	struct StackNode  *pop;
	//	struct Triangle *ajt,*tri;
	int  ajtno,trino;
	int vno;
	int i,j,idx,inf;
	struct ImpactArea *temp;

	ImpactTriHead = new struct StackNode;
	ImpactTriHead->trino = FirstTriNo;
	ImpactTriHead->Vno = 0;
	ImpactTriHead->next = NULL;

	ImpactHead = new struct ImpactArea;
	ImpactHead->next = NULL;
	ImpactTail = ImpactHead;

	for(i=0; i<3; i++)
	{
		StackTop = NULL;//置空栈
		ajtno = TriList[FirstTriNo].Ajt[i];

		FindOppTri(j,ajtno,FirstTriNo);

		if(ajtno!=-1 && TriList[FirstTriNo].EdgeInf[i]!='f')
			PUSHSTACK(ajtno, j,-1);
		else
		{
			temp = new struct ImpactArea;
			temp->trino = ajtno;

			temp->Vno = j;
			temp->vf = TriList[FirstTriNo].vertex[(i+1)%3];
			temp->vt = TriList[FirstTriNo].vertex[(i+2)%3];
			//	   TRACE("+++ i=%d vf=%d vt=%d\n",i,temp->vf,temp->vt);	   
			temp->next = ImpactTail->next;
			ImpactTail->next = temp;
			ImpactTail = temp;
		}
		while(pop=POPSTACK())
		{
			trino = pop->trino;
			vno = pop->Vno;
			inf = pop->inf;          

			if(inf==-1 && HCircle(TriList[trino].vertex[0], TriList[trino].vertex[1], TriList[trino].vertex[2], ptnum) <0)//在tri外接圆内
			{
				ExpandProgress(trino,(vno+2)%3);
				ExpandProgress(trino,(vno+1)%3);
				pop->next=ImpactTriHead->next;
				ImpactTriHead->next = pop;
			}
			else
			{
				temp = new struct ImpactArea;
				if(inf==1)//本三角形
				{
					temp->trino = -1;
					temp->vf = TriList[trino].vertex[(vno+1)%3];
					temp->vt = TriList[trino].vertex[(vno+2)%3];			   
				}
				else//外三角形
				{
					temp->trino = trino;
					temp->vf = TriList[trino].vertex[(vno+2)%3];
					temp->vt = TriList[trino].vertex[(vno+1)%3];
				}
				temp->Vno = vno;

				temp->next = ImpactTail->next;
				ImpactTail->next = temp;
				ImpactTail = temp;
				delete pop;
			}
		}
	}
	//开辟两个三角形链表空间
	for(i=0; i<2; i++)
	{
		TriListTailNo++;
		trino = TriListTailNo;
		TriList[trino].Attribute = 'Y';
		// AddList(tri);//加入三角形链表

		pop = new struct StackNode;
		pop->trino = trino;
		pop->Vno = 0;
		pop->next=ImpactTriHead->next;
		ImpactTriHead->next = pop;//加入调整三角形链表
	}
	//根据影响域链表，生成新三角形
	idx=1;
	temp = ImpactHead->next;
	pop = ImpactTriHead;
	while(temp && pop)
	{

		TriList[pop->trino].vertex[0] = temp->vf;
		TriList[pop->trino].vertex[1] = temp->vt;
		TriList[pop->trino].vertex[2] = ptnum;
		TriList[pop->trino].Ajt[2] = temp->trino;
		if(temp->trino!=-1)
			TriList[temp->trino].Ajt[temp->Vno] = pop->trino;
		ptdata[temp->vf].trino=pop->trino;
		temp->trino = pop->trino;

		temp = temp->next;
		pop = pop->next;
	}
	ptdata[ptnum].trino = ImpactTriHead->trino; 
	//新生成三角形链表
	temp = ImpactHead->next;   
	while(temp->next)
	{
		TriList[temp->trino].Ajt[0] = temp->next->trino;
		TriList[temp->next->trino].Ajt[1] = temp->trino;

		temp = temp->next;	        
	}
	TriList[temp->trino].Ajt[0] = ImpactHead->next->trino;
	TriList[ImpactHead->next->trino].Ajt[1] = temp->trino;

	temp = ImpactHead;
	while(temp)
	{
		ImpactHead = temp->next;
		free(temp);
		temp = ImpactHead;
	}

	pop = ImpactTriHead;
	while(pop)
	{
		ImpactTriHead = pop->next;
		free(pop);
		pop = ImpactTriHead;
	}


	return 1;
}
/*v1,v2,v3逆时针返回正，反之为负*/

void CFastTin::PUSHSTACK(int trino,int vno,int inf)
{
	struct StackNode  *p;
	p = new struct StackNode;
	p->trino = trino;
	p->next = StackTop;
	p->Vno = vno;
	p->inf = inf;
	StackTop = p;
}
CFastTin::StackNode* CFastTin::POPSTACK()
{
	struct StackNode  *p;
	if(StackTop==NULL) return NULL;
	else
	{
		p = StackTop;
		StackTop = StackTop->next;
		return p;
	}
}


double CFastTin::CCW(int v1,int v2,int v3) 
{
	double x1,x2,x3,y1,y2,y3,bx,by;

	bx = (ptdata[v1].pt[0]+ptdata[v2].pt[0]+ptdata[v3].pt[0]) /3.0;
	by = (ptdata[v1].pt[1]+ptdata[v2].pt[1]+ptdata[v3].pt[1]) /3.0;
	x1 = ptdata[v1].pt[0]-bx;
	x2 = ptdata[v2].pt[0]-bx;
	x3 = ptdata[v3].pt[0]-bx;
	y1 = ptdata[v1].pt[1]-by;
	y2 = ptdata[v2].pt[1]-by;
	y3 = ptdata[v3].pt[1]-by;
	return (x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2));
}

/* vl 在V1 V2 V3 的外接圆内，返回负，反之为正 */ 
double CFastTin::HCircle(int v1, int v2, int v3, int vl)
{
	double x1,x2,x3,xl,y1,y2,y3,yl,val1,val2,val3,val4,bx,by;
	bx = (ptdata[v1].pt[0]+ptdata[v2].pt[0]+ptdata[v3].pt[0]) /3.0;
	by = (ptdata[v1].pt[1]+ptdata[v2].pt[1]+ptdata[v3].pt[1]) /3.0;
	//	bx=by=0.0;

	x1 = ptdata[v1].pt[0]-bx;
	x2 = ptdata[v2].pt[0]-bx;
	x3 = ptdata[v3].pt[0]-bx;
	xl = ptdata[vl].pt[0]-bx;
	y1 = ptdata[v1].pt[1]-by;
	y2 = ptdata[v2].pt[1]-by;
	y3 = ptdata[v3].pt[1]-by;
	yl = ptdata[vl].pt[1]-by;

	val1 = x2*(y3*(xl*xl+yl*yl) - yl*(x3*x3+y3*y3)) -
		x3*(y2*(xl*xl+yl*yl) - yl*(x2*x2+y2*y2)) +
		xl*(y2*(x3*x3+y3*y3) - y3*(x2*x2+y2*y2));

	val2 = x1*(y3*(xl*xl+yl*yl) - yl*(x3*x3+y3*y3)) -
		x3*(y1*(xl*xl+yl*yl) - yl*(x1*x1+y1*y1)) +
		xl*(y1*(x3*x3+y3*y3) - y3*(x1*x1+y1*y1));

	val3 = x1*(y2*(xl*xl+yl*yl) - yl*(x2*x2+y2*y2)) -
		x2*(y1*(xl*xl+yl*yl) - yl*(x1*x1+y1*y1)) +
		xl*(y1*(x2*x2+y2*y2) - y2*(x1*x1+y1*y1));

	val4 = x1*(y2*(x3*x3+y3*y3) - y3*(x2*x2+y2*y2)) -
		x2*(y1*(x3*x3+y3*y3) - y3*(x1*x1+y1*y1)) +
		x3*(y1*(x2*x2+y2*y2) - y2*(x1*x1+y1*y1));

	return(val1-val2+val3-val4);
}

void  CFastTin::ExpandProgress(int trino,int vno)
{
	int j;
	int ajtno;

	if(TriList[trino].Ajt[vno]!=-1 && TriList[trino].EdgeInf[vno]!='f')
	{
		ajtno = TriList[trino].Ajt[vno];
		FindOppTri(j,ajtno,trino);

		PUSHSTACK(ajtno, j,-1);
	}
	else
		PUSHSTACK(trino,vno,1);
}

void CFastTin::DeleteTris()
{
	int trino,ajtno,headno,opptrino;
	int i,j,ajtvno;
	//对四个角点做删除操作
	for(i=0; i<4; i++)
	{
		//从i点记录的首三角形逆时针转到头
		trino = ptdata[i].trino;
		headno = trino;
		j=0;
		while(j<3 && TriList[ptdata[i].trino].vertex[j]!=i)
			j++;
		ajtno = TriList[ptdata[i].trino].Ajt[(j+1)%3];
		while(ajtno!=-1)
		{
			j=0;
			while(j<3 && TriList[ajtno].vertex[j]!=i)
				j++;
			headno = ajtno;
			ajtno = TriList[ajtno].Ajt[(j+1)%3];
		}

		//从以i点为顶点的三角形顺时针删除三角形
		ajtno = headno;
		while(ajtno!=-1)
		{
			j=0;
			while(j<3 && TriList[ajtno].vertex[j]!=i)
				j++;

			opptrino = TriList[ajtno].Ajt[j];
			FindOppTri(ajtvno,opptrino,ajtno);	
			// ajtvno=0;
			if(opptrino!=-1)
				TriList[opptrino].Ajt[ajtvno] = -1;

			trino = ajtno;
			ajtno = TriList[ajtno].Ajt[(j+2)%3];		  
			TriList[trino].Attribute = 'N';//trino三角形无效
			//		  DeleteATri(tri);
		}	        	
	}
}

void CFastTin::FormInitEgdes()
{
	int trino,ajtno,headno,opptrino;
	int i,j,ajtvno;
	struct TEdgeNode *temp;
	//对四个角点做删除操作

	EdgeListHead = new struct TEdgeNode;
	EdgeListHead->next = NULL;
	EdgeListTail = EdgeListHead;


	for(i=0; i<4; i++)
	{
		//从i点记录的首三角形逆时针转到头
		trino = ptdata[i].trino;
		headno = trino;
		j=0;
		while(j<3 && TriList[ptdata[i].trino].vertex[j]!=i)
			j++;
		ajtno = TriList[ptdata[i].trino].Ajt[(j+1)%3];
		while(ajtno!=-1)
		{
			j=0;
			while(j<3 && TriList[ajtno].vertex[j]!=i)
				j++;
			headno = ajtno;
			ajtno = TriList[ajtno].Ajt[(j+1)%3];
		}

		//从以i点为顶点的三角形顺时针删除三角形
		ajtno = headno;
		while(ajtno!=-1)
		{
			//在本三角形中找到以i点为顶点的点号(0,1,2)j
			j=0;
			while(j<3 && TriList[ajtno].vertex[j]!=i)
				j++;
			//找到与i点相对的邻结三角形号opptrino
			opptrino = TriList[ajtno].Ajt[j];
			//在邻结三角形opptrino中找到与本三角形ajtno 相对的点号ajtvno(0,1,2)
			FindOppTri(ajtvno,opptrino,ajtno);	
			// ajtvno=0;
			//	      if(opptrino!=-1)
			//		     TriList[opptrino].Ajt[ajtvno] = -1;
			//构建初始边界链表
			if(ajtno!=headno && TriList[ajtno].Ajt[(j+2)%3]!=-1)
			{
				temp = new struct TEdgeNode;
				temp->trino = opptrino;
				temp->vf = TriList[opptrino].vertex[(ajtvno+1)%3];
				temp->vt = TriList[opptrino].vertex[(ajtvno+2)%3];			   
				temp->Vno = ajtvno;
				temp->next = EdgeListTail->next;
				EdgeListTail->next = temp;
				EdgeListTail = temp;     
			}

			trino = ajtno;
			ajtno = TriList[ajtno].Ajt[(j+2)%3];		  
			TriList[trino].Attribute = 'N';//trino三角形无效

		}	        	
	}

}

void CFastTin::DealEgdes()
{
	struct TEdgeNode *temp,*temp1;
	double edgeLen;
	int trino,vt,vno,j,ii;
	//   ads_point pt;

	temp = EdgeListHead->next;
	ii = 0;
	while(temp)
	{
		edgeLen = sqrt((ptdata[temp->vf].pt[0]-ptdata[temp->vt].pt[0])*(ptdata[temp->vf].pt[0]-ptdata[temp->vt].pt[0])+
			(ptdata[temp->vf].pt[1]-ptdata[temp->vt].pt[1])*(ptdata[temp->vf].pt[1]-ptdata[temp->vt].pt[1]));
		if(edgeLen>MaxLen && TriList[temp->trino].Attribute != 'N')
		{
			vt = temp->vt;
			vno = temp->Vno;
			temp->vt = TriList[temp->trino].vertex[temp->Vno];
			trino = temp->trino;
			TriList[trino].Attribute = 'N';
			temp->trino = TriList[trino].Ajt[(vno+2)%3];
			FindOppTri(j,temp->trino,trino);
			//			TriList[temp->trino].Ajt[j] = -1;
			temp->Vno = j;

			temp1 = new struct TEdgeNode;
			temp1->vf = temp->vt;
			temp1->vt = vt;
			temp1->trino = TriList[trino].Ajt[(vno+1)%3];
			FindOppTri(j,temp1->trino,trino);
			//			TriList[temp1->trino].Ajt[j] = -1;
			temp1->Vno = j;
			temp1->next = temp->next;
			temp->next = temp1;
		}
		else
			temp = temp->next;
	}
}

void CFastTin::SelectPoint()
{
	FILE *fpr,*fpw;
	CString fname;
	int i,NumAll,j,k;
	double tmp;
	struct PT spt;
	double LP[3],RP[3],xmin,xmax,ymin,ymax,sml;
	xlpoint PZ;
	//	NRoad=2;
	//1.打开范围文件,选择地形点
	xmin=ymin=9999999999;
	xmax=ymax=-9999999999;
	for(i=0;i<NRoad;i++)
	{
		int xynum;
		xynum = RoadHole[i]->pm->XYNum;
		sml = RoadHole[i]->pm->DLArray[0].TYLC;
		while (sml < RoadHole[i]->pm->XLLength)
		{
			PZ.lc = sml;
			RoadHole[i]->pm->xlpoint_pz(&PZ);
			xmin = xmin<PZ.x?xmin:PZ.x;
			xmax = xmax>PZ.x?xmax:PZ.x;
			ymin = ymin<PZ.y?ymin:PZ.y;
			ymax = ymax>PZ.y?ymax:PZ.y;	
			sml +=20;
		}
	}


	LeftLowPt[0] =xmin-500;
	LeftLowPt[1] =ymin-500;
	RightUpPt[0] =xmax+500;
	RightUpPt[1] =ymax+500;

	fname = Cworkdir + "\\3d\\Spt." + Pro;
	if((fpw = _wfopen(fname ,L"wb"))==NULL)
		return ;

	NumAll=4;
	spt.pt[0] = LeftLowPt[0]-100.0;      
	spt.pt[1] = LeftLowPt[1]-100.0;
	spt.pt[2] = 0.0;
	spt.cur = -1;
	spt.att = 'b';  //矩形四个角点标志
	spt.trino = -1;
	fwrite(&spt,sizeof(struct PT),1,fpw);

	spt.pt[0] = RightUpPt[0]+100.0;
	spt.pt[1] = LeftLowPt[1]-100.0;
	spt.pt[2] = 0.0;
	spt.att = 'b';
	spt.trino = -1;
	fwrite(&spt,sizeof(struct PT),1,fpw);

	spt.pt[0] = RightUpPt[0]+100;
	spt.pt[1] = RightUpPt[1]+100;
	spt.pt[2] = 0.0;
	spt.att = 'b';
	spt.trino = -1;
	fwrite(&spt,sizeof(struct PT),1,fpw);

	spt.pt[0] = LeftLowPt[0]-100;
	spt.pt[1] = RightUpPt[1]+100;
	spt.pt[2] = 0.0;
	spt.cur = -1;
	spt.att = 'b';
	spt.trino = -1;
	fwrite(&spt,sizeof(struct PT),1,fpw);

	for(i=0;i<NRoad;i++)
	{
		for(j=0;j<RoadHole[i]->HoleNum;j++)
		{
			for(k=0;k<RoadHole[i]->HoleArray[j].nLpt;k++)
			{
				spt.pt[0] = RoadHole[i]->HoleArray[j].LptArr[k].y;
				spt.pt[1] = RoadHole[i]->HoleArray[j].LptArr[k].x;
				spt.pt[2] = RoadHole[i]->HoleArray[j].LptArr[k].z;
				spt.cur = -1;
				spt.att = 'c';//坡脚点
				spt.trino = -1;
				fwrite(&spt,sizeof(struct PT),1,fpw);
				NumAll++;
			}
			for(k=0;k<RoadHole[i]->HoleArray[j].nRpt;k++)
			{
				spt.pt[0] = RoadHole[i]->HoleArray[j].RptArr[k].y;
				spt.pt[1] = RoadHole[i]->HoleArray[j].RptArr[k].x;
				spt.pt[2] = RoadHole[i]->HoleArray[j].RptArr[k].z;
				spt.cur = -1;
				spt.att = 'c';//坡脚点
				spt.trino = -1;
				fwrite(&spt,sizeof(struct PT),1,fpw);
				NumAll++;
			}			
		}
	}


	for(i=0;i<NRoad;i++)
	{
		for(j=0;j<RoadHole[i]->NBriBj;j++)
		{
			spt.pt[0] = RoadHole[i]->BriBjArr[j].qpt[0];
			spt.pt[1] = RoadHole[i]->BriBjArr[j].qpt[1];
			spt.pt[2] = RoadHole[i]->BriBjArr[j].qpt[2]-20;
			spt.cur = -1;
			spt.att = 'c';//桥边界
			spt.trino = -1;
			fwrite(&spt,sizeof(struct PT),1,fpw);
			NumAll++;
			spt.pt[0] = RoadHole[i]->BriBjArr[j].hpt[0];
			spt.pt[1] = RoadHole[i]->BriBjArr[j].hpt[1];
			spt.pt[2] = RoadHole[i]->BriBjArr[j].hpt[2]-20;
			spt.cur = -1;
			spt.att = 'c';//桥边界
			spt.trino = -1;
			fwrite(&spt,sizeof(struct PT),1,fpw);
			NumAll++;						
		}
	}

	ACHAR RailWorkdir[512],RailPro[512];
	BAS_DRAW_FUN::ReadRailWorkdir(RailWorkdir, RailPro);

	
	ACHAR szPath[MAX_PATH];
	ACHAR TempCurTinName[80]=L"";
	CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", RailWorkdir);
	FILE* pFile = _wfopen(szPath, L"r");
	if (pFile)
	{
		fwscanf(pFile, L"%s", TempCurTinName);
		CurTinName = TempCurTinName;
		fclose(pFile);
	}
	else
		CurTinName.Empty();

	ads_printf(L"正在导入数模数据文件,请稍候...\n");
	if (CurTinName.IsEmpty())
		fname.Format(L"%s\\pmap\\PtIndex.idx", RailWorkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);  
		fname.Format(L"%s\\pmap\\PtIndex.idx.%s", RailWorkdir, CurTinName);
		



	//fname = Cworkdir + "\\pmap\\PtIndex.idx"; 
	//fname.Format(L"%s\\pmap\\PtIndex.idx",RailWorkdir);
	if( (fpr=_wfopen(fname,L"r"))==NULL)
	{
		AfxMessageBox(fname);
		return ;
	}
	fwscanf(fpr ,L"%d%lf%lf%lf%lf" , &PTSUM,&tmp,&tmp,&tmp,&tmp);
	fclose(fpr);

	//ACHAR RailWorkdir[512], RailPro[512];
	BAS_DRAW_FUN::ReadRailWorkdir(RailWorkdir, RailPro);


	//ACHAR szPath[MAX_PATH];
	_tcscpy(TempCurTinName,L"");
	
	//CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", RailWorkdir);
	pFile = _wfopen(szPath, L"r");
	if (pFile)
	{
		fwscanf(pFile, L"%s", TempCurTinName);
		CurTinName = TempCurTinName;
		fclose(pFile);
	}
	else
		CurTinName.Empty();

	ads_printf(L"正在导入数模数据文件,请稍候...\n");
	if (CurTinName.IsEmpty())
		fname.Format(L"%s\\PMAP\\Point.xyz", RailWorkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);  
		fname.Format(L"%s\\PMAP\\Point.xyz.%s", RailWorkdir, CurTinName);



	//fname = Cworkdir + "\\PMAP\\Point.xyz";
	//fname.Format(L"%s\\PMAP\\Point.xyz", RailWorkdir);
	ptdata =  new struct PT[PTSUM];
	if ((fpr=_wfopen(fname, L"rb"))==NULL)
	{
		AfxMessageBox(fname);
		return;
	}
	//fpr = _wfopen(fname,L"rb");
	i=fread(ptdata, sizeof(struct PT), PTSUM, fpr);
	fclose(fpr);	      
	/*
	fname = Cworkdir + "\\PMAP\\Point.xyz";   
	mapfileHandlePp = CreateFile(fname,          // pointer to name of the file
	GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
	FILE_SHARE_WRITE,    // share mode
	NULL,  // pointer to security attributes
	OPEN_EXISTING,  // how to create
	FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
	NULL);

	fname = "MemMapPp1";
	MEMmapfileHandlePp = CreateFileMapping(mapfileHandlePp,NULL,PAGE_READWRITE,0,0,fname);	   
	FilePtrPp = MapViewOfFile(MEMmapfileHandlePp,FILE_MAP_WRITE,0,0,0);
	ptdata =  (struct PT *)FilePtrPp;*/


	for(i=0; i<PTSUM; i++)
	{
		if(ptdata[i].pt[0]>LeftLowPt[0]&&ptdata[i].pt[0]<RightUpPt[0]&&ptdata[i].pt[1]>LeftLowPt[1]&&ptdata[i].pt[1]<RightUpPt[1])
		{
			fwrite(&ptdata[i],sizeof(struct PT),1,fpw);
			NumAll++;
			//		  fwrite(&Tri,sizeof(struct Triangle),1,fpw1);
			//		  fwrite(&Tri,sizeof(struct Triangle),1,fpw1);		 	  
		}
	}

	/*
	UnmapViewOfFile(FilePtrPp);
	CloseHandle(MEMmapfileHandlePp);
	CloseHandle(mapfileHandlePp);*/

	fclose(fpw);
	delete []ptdata;
	// fclose(fpw1);
	PTSUM = NumAll;
}

void CFastTin::AdjustBriBj()
{
	int i,j;
	double ver1[2], ver2[2];
	for(i=0;i<NRoad;i++)
	{		
		for(j=0;j<RoadHole[i]->NBriBj;j++)
		{
			ver1[0]=RoadHole[i]->BriBjArr[j].qpt[0], ver1[1]=RoadHole[i]->BriBjArr[j].qpt[1];
			ver2[0]=RoadHole[i]->BriBjArr[j].hpt[0], ver2[1]=RoadHole[i]->BriBjArr[j].hpt[1];
			AjustTIN(ver1,ver2); 
			SetEdgeInf(ver1,ver2); 
		}
	}
}

void CFastTin::DealHole(int i,int k)
{
	double ver1[2], ver2[2];
	char mes[280];
	int Lnum,Rnum,ii;
	double dis;


	AcGePoint3d spt, ept;
	ver1[0]=RoadHole[i]->HoleArray[k].LptArr[0].y ,ver1[1]=RoadHole[i]->HoleArray[k].LptArr[0].x;
	ver2[0]=RoadHole[i]->HoleArray[k].RptArr[0].y ,ver2[1]=RoadHole[i]->HoleArray[k].RptArr[0].x;

	AjustTIN(ver1,ver2); 

	spt.x=ver1[1];spt.y=ver1[0];
	ept.x=ver2[1];ept.y=ver2[0];
	makeline(spt,ept,6,0,0);
	Lnum = RoadHole[i]->HoleArray[k].nLpt; 
	Rnum = RoadHole[i]->HoleArray[k].nRpt; 

	if(Lnum<1 || Rnum<1)
		return;


	for(ii=0;ii<Lnum-1;ii++)
	{
		ver1[0]=RoadHole[i]->HoleArray[k].LptArr[ii].y;
		ver1[1]=RoadHole[i]->HoleArray[k].LptArr[ii].x;
		ver2[0]=RoadHole[i]->HoleArray[k].LptArr[ii+1].y;
		ver2[1]=RoadHole[i]->HoleArray[k].LptArr[ii+1].x;

		AjustTIN(ver1,ver2);
		spt.x=ver1[1];spt.y=ver1[0];
		ept.x=ver2[1];ept.y=ver2[0];
		makeline(spt,ept,2,0,0);

		//			dis=sqrt((ver1[0]-3577563.72)*(ver1[0]-3577563.72)+(ver1[1]-35590149.28)*(ver1[1]-35590149.28));
		//			if(dis<0.1)


	}

	for(ii=0;ii<Rnum-1;ii++)
	{
		ver1[0]=RoadHole[i]->HoleArray[k].RptArr[ii].y;
		ver1[1]=RoadHole[i]->HoleArray[k].RptArr[ii].x;
		ver2[0]=RoadHole[i]->HoleArray[k].RptArr[ii+1].y;
		ver2[1]=RoadHole[i]->HoleArray[k].RptArr[ii+1].x;


		AjustTIN(ver1,ver2);

		spt.x=ver1[1];spt.y=ver1[0];
		ept.x=ver2[1];ept.y=ver2[0];
		makeline(spt,ept,6,0,0);	

		//		dis=sqrt((ver1[0]-3577563.72)*(ver1[0]-3577563.72)+(ver1[1]-35590149.28)*(ver1[1]-35590149.28));
		//		if(dis<0.1)
		//			ads_printf(L"2:%d %d\n",i,k);
	}

	ver1[0]=RoadHole[i]->HoleArray[k].LptArr[Lnum-1].y;
	ver1[1]=RoadHole[i]->HoleArray[k].LptArr[Lnum-1].x;
	ver2[0]=RoadHole[i]->HoleArray[k].RptArr[Rnum-1].y;
	ver2[1]=RoadHole[i]->HoleArray[k].RptArr[Rnum-1].x;

	AjustTIN(ver1,ver2);
	spt.x=ver1[1];spt.y=ver1[0];
	ept.x=ver2[1];ept.y=ver2[0];
	makeline(spt,ept,7,0,0);	    

	//设置Hole边界三角形信息
	ver1[0]=RoadHole[i]->HoleArray[k].LptArr[0].y;
	ver1[1]=RoadHole[i]->HoleArray[k].LptArr[0].x;
	ver2[0]=RoadHole[i]->HoleArray[k].RptArr[0].y;
	ver2[1]=RoadHole[i]->HoleArray[k].RptArr[0].x;
	SetEdgeInf(ver1,ver2); 

	ver1[0]=RoadHole[i]->HoleArray[k].LptArr[Lnum-1].y;
	ver1[1]=RoadHole[i]->HoleArray[k].LptArr[Lnum-1].x;
	ver2[0]=RoadHole[i]->HoleArray[k].RptArr[Rnum-1].y;
	ver2[1]=RoadHole[i]->HoleArray[k].RptArr[Rnum-1].x;


	SetEdgeInf(ver1,ver2); 

	for(ii=0;ii<Lnum-1;ii++)
	{
		ver1[0]=RoadHole[i]->HoleArray[k].LptArr[ii].y;
		ver1[1]=RoadHole[i]->HoleArray[k].LptArr[ii].x;
		ver2[0]=RoadHole[i]->HoleArray[k].LptArr[ii+1].y;
		ver2[1]=RoadHole[i]->HoleArray[k].LptArr[ii+1].x;

		SetEdgeInf(ver1,ver2);	


	}

	for(ii=0;ii<Rnum-1;ii++)
	{           ver1[0]=RoadHole[i]->HoleArray[k].RptArr[ii].y;
	ver1[1]=RoadHole[i]->HoleArray[k].RptArr[ii].x;
	ver2[0]=RoadHole[i]->HoleArray[k].RptArr[ii+1].y;
	ver2[1]=RoadHole[i]->HoleArray[k].RptArr[ii+1].x;
	SetEdgeInf(ver1,ver2);

	}

}

void  CFastTin::SetTriAtt(int ntri)
{
	int i;
	bool inf=false;

	for(i=0; i<ntri; i++)
	{	
		if(TriList[ImpactTris[i]].Attribute!='Y')
		{
			inf = true;
			break;
		}
	}
	if(inf)
	{
		for(i=0; i<ntri; i++)
			if(TriList[ImpactTris[i]].Attribute=='Y')	
				SetATinAtt(ImpactTris[i]);

	}
}
int  CFastTin::AjustTIN(double ver1[2],double ver2[2])
{
	int StartVerNo,EndVerNo,StartTriNo;
	int trino,j,inf;
	double cw1,cw2;
	int lpcur,rpcur,trcur,ajt,i,RightTriNo,count=0;
	char mes[280];
	double pt1[2],pt2[2];
	//	ads_point pt,ptt[3];



	double dis;
	dis=sqrt((ver1[0]-ver2[0])*(ver1[0]-ver2[0])+(ver1[1]-ver2[1])*(ver1[1]-ver2[1]));
	if(dis<0.001)
	{
		//ads_printf(L"x1=%lf y1=%lf x2=%lf y2=%lf\n",ver1[0],ver1[1],ver2[0],ver2[1]);
		//AfxMessageBox(mes);
		return -1;

	}
	StartTriNo=-1;

	StartVerNo = FindVerNo(ver1);
	EndVerNo = FindVerNo(ver2);
	if(StartVerNo==EndVerNo)
	{
		//	ads_printf(L"x1=%lf y1=%lf x2=%lf y2=%lf",ver1[0],ver1[1],ver2[0],ver2[1]);
		//AfxMessageBox(mes);
		return -1;

	}
	//ads_printf(L"start=%d endno=%d\n",StartVerNo,EndVerNo);
	//	if(dis<0.1)
	//	{
	//       ads_printf(L"############## %lf %lf %lf %lf %d %d\n",ver1[0],ver1[1],ver2[0],ver2[1],StartVerNo,EndVerNo);
	//	   ads_alert(L"a");
	//	}

	if(StartVerNo<0 || EndVerNo<0)
	{
		//	ads_printf(L"1:%d %d\n",StartVerNo,EndVerNo);
		return -1;
	}

	trino = ptdata[StartVerNo].trino;//起始三角形
	if(trino==-1)
	{
		//   	ads_printf(L"2:%d %d\n",StartVerNo,EndVerNo);
		return 0;
	}
	inf = 1;
	do
	{
		if(trino!=-1)
		{
			j=0;
			while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
				j++;

			cw1 = CCW(StartVerNo,EndVerNo,TriList[trino].vertex[(j+1)%3]);
			cw2 = CCW(StartVerNo,EndVerNo,TriList[trino].vertex[(j+2)%3]);
		}

		if(trino==-1 || trino==StartTriNo) //转到一头或一个三角形在一条线上,换一方向继续找
		{
			if(inf==1)
				inf = 0;
			else
			{
				//  ads_printf(L"3:%d %d\n",StartVerNo,EndVerNo);
				return -1;
			}
			trino = ptdata[StartVerNo].trino;
		}
		StartTriNo = trino;
		if(inf)
			trino = TriList[trino].Ajt[(j+1)%3];
		else if(inf==0)
			trino = TriList[trino].Ajt[(j+2)%3];
	} while(cw1>0.0 || cw2 <0.0) ;//该过程在以StartVerNo为顶点一圈三角形中，找出StartVerNo-EndVerNo穿越的三角形

	//	if(dis<0.1)  
	//      ads_printf(L"sno=%d tno=%d trino=%d cw1=%lf cw2=%lf %d %d %d %d\n",
	//	 StartVerNo,EndVerNo,StartTriNo,cw1,cw2,TriList[StartTriNo].vertex[j],TriList[StartTriNo].vertex[(j+1)%3],TriList[StartTriNo].vertex[(j+2)%3],StartTriNo);
	//ads_printf(L"%d %c%c%c\n",StartTriNo,TriList[117885].EdgeInf[0],TriList[117885].EdgeInf[1],TriList[117885].EdgeInf[2]);

	if(TriList[StartTriNo].vertex[(j+1)%3]==EndVerNo)
	{
		TriList[StartTriNo].EdgeInf[(j+2)%3]='f';
		ajt = TriList[StartTriNo].Ajt[(j+2)%3];
		if(ajt !=-1)
		{
			FindOppTri(j,ajt,StartTriNo);
			TriList[ajt].EdgeInf[j]='f';
		}

		return 1;
	}

	if(TriList[StartTriNo].vertex[(j+2)%3]==EndVerNo)
	{
		TriList[StartTriNo].EdgeInf[(j+1)%3]='f';
		ajt = TriList[StartTriNo].Ajt[(j+1)%3];
		if(ajt != -1)
		{
			FindOppTri(j,ajt,StartTriNo);
			TriList[ajt].EdgeInf[j]='f';
		}
		//  if(dis<0.1)  
		//	  ads_printf(L"2:j=%d ajt=%d %c%c%c\n",j,ajt,
		//	  TriList[StartTriNo].EdgeInf[0],TriList[StartTriNo].EdgeInf[1],TriList[StartTriNo].EdgeInf[2]);
		return 1;
	}



	Root = NULL;
	LeftVerList = new struct VerList[5000];
	RightVerList = new struct VerList[5000];
	ImpactTris = new int[5000];


	//生成左右侧点集及影响域内的三角形
	if(FormVerList(StartTriNo,j,StartVerNo,EndVerNo,lpcur,rpcur,trcur)==0)
	{
		delete []LeftVerList;
		delete []RightVerList;
		delete []ImpactTris;
		detree(Root);
		return 0;
	}
	int ntri = trcur;

	//判别地性线是否有共线点
	if(PtZeroNum >0)//将地性线按共线点划分段落
	{
		//	   _stprintf(mes,L"num=%d",PtZeroNum);
		//	   AfxMessageBox(mes);
		delete []LeftVerList;
		delete []RightVerList;
		delete []ImpactTris;
		detree(Root);
		pt1[0] = ver1[0],pt1[1] = ver1[1];
		double distover2;
		for(i=0; i<PtZeroNum; i++)
		{

			pt2[0] = ptdata[PtZero[i]].pt[0],pt2[1] = ptdata[PtZero[i]].pt[1];
			distover2 = sqrt((ver2[0]-pt2[0])*(ver2[0]-pt2[0])+(ver2[1]-pt2[1])*(ver2[1]-pt2[1]));
			if(distover2 < 0.001)
				break;
			AjustTIN(pt1,pt2);
			SetEdgeInf(pt1,pt2);//06.11.25
			pt1[0] = pt2[0],pt1[1]=pt2[1];
		}
		pt2[0] = ver2[0],pt2[1] = ver2[1];
		AjustTIN(pt1,pt2);
		SetEdgeInf(pt1,pt2);//06.11.25
		return 1;
	}

	//TRACE("lpcur=%d rpcur=%d trcur=%d\n",lpcur,rpcur,trcur);
	//  AfxMessageBox(mes);  
	//针对左右点集合调整影响域内的三角形
L100:
	trcur = 0;
	ReConstructTin(LeftVerList , lpcur, trcur);

	trcur++;
	RightTriNo = trcur; 
	for(i=0; i<rpcur; i++)
	{
		LeftVerList[i].ptno = RightVerList[rpcur-i-1].ptno;
		if(i < rpcur-1)
		{
			LeftVerList[i].trino = RightVerList[rpcur-i-2].trino;
			LeftVerList[i].Vno = RightVerList[rpcur-i-2].Vno;
		}
	}

	ReConstructTin(LeftVerList , rpcur, trcur); 

	TriList[ImpactTris[0]].Ajt[2] = ImpactTris[RightTriNo];   	   
	_tcscpy(TriList[ImpactTris[0]].EdgeInf,L"aaf");  
	TriList[ImpactTris[RightTriNo]].Ajt[2] = ImpactTris[0];    
	_tcscpy(TriList[ImpactTris[RightTriNo]].EdgeInf,L"aaf");

	SetTriAtt(ntri);

	delete []LeftVerList;
	delete []RightVerList;
	delete []ImpactTris;
	detree(Root);  
	return 1;
}

int CFastTin::FindVerNo(double ver1[2])
{
	int i,j,jj;
	ACHAR mes[300];
	double Mindis=9999999999999.0,dis;
	int num;

	i = (int)((ver1[0] - LeftLowPt[0]) / GridWidth+0.001);
	j = (int)((ver1[1] - LeftLowPt[1]) / GridWidth+0.001);
	jj = GridArray[i*NCol+j].Pfcur;

	num = jj;
	while(jj != -1)//2002.8.17
	{
		//  _stprintf(mes,L"%d 点:%lf %lf ptdata:%lf %lf",jj,ver1[0],ver1[1],ptdata[jj].pt[0],ptdata[jj].pt[1]);
		//  AfxMessageBox(mes);

		dis = sqrt((ptdata[jj].pt[0]-ver1[0])*(ptdata[jj].pt[0]-ver1[0])+(ptdata[jj].pt[1]-ver1[1])*(ptdata[jj].pt[1]-ver1[1]));

		if(Mindis > dis &&  ptdata[jj].att=='c')
		{
			num = jj;
			Mindis = dis;
		}

		if(fabs(ptdata[jj].pt[0]-ver1[0])<0.001 &&fabs(ptdata[jj].pt[1]-ver1[1])<0.001)
			break;
		jj = ptdata[jj].cur;



		//	 _stprintf(mes,L"%d 点:%lf %lf ptdata:%lf %lf",jj,ver1[0],ver1[1],ptdata[jj].pt[0],ptdata[jj].pt[1]);
		//     AfxMessageBox(mes);

	}

	if(jj<0)
	{
		//	ads_printf(L"!!!!!!!!!!!!!!! mindis=%lf\n",Mindis);
		if(Mindis<1.0)
		{
			_stprintf(mes,L"点:%lf %lf %lf %d 有错误!",ver1[0],ver1[1],Mindis,num);
			//		 AfxMessageBox(mes);
			return num;
		}

		//       AfxMessageBox(mes);
	}
	else
		return jj;
}

//----------------------------------------------------
//生成1: 影响域边界顶点数组LeftVerList[]，RightVerList[],
//    2: 地性线穿越的三角形数组ImpactTris[]
//    3: 影响域边界的邻接三角形LeftVerList[].trino及该三角形中边界对应的顶点号(0,1,2)LeftVerList[].Vno
//----------------------------------------------------
int CFastTin::FormVerList(int StartTriNo,int vno,int StartVerNo,int EndVerNo,int &lpcur,int &rpcur,int &trcur)
{
	int j,verno,ajt,pretno,ajt1,v1,v2;
	double pt[3],cw;
	ACHAR mes[280];

	trcur =lpcur=rpcur=0;
	PtZeroNum = 0;
	ImpactTris[trcur] = StartTriNo; //起始三角形号
	trcur++;
	LeftVerList[lpcur].ptno = StartVerNo;
	LeftVerList[lpcur].trino = TriList[StartTriNo].Ajt[(vno+1)%3];
	ajt1 = LeftVerList[lpcur].trino ;
	//   CreateAFace(ptdata[TriList[ajt1].vertex[0]].pt,ptdata[TriList[ajt1].vertex[1]].pt,ptdata[TriList[ajt1].vertex[2]].pt,1);	                                    
	FindOppTri(j,LeftVerList[lpcur].trino,StartTriNo);
	LeftVerList[lpcur].Vno = j;

	lpcur++;
	LeftVerList[lpcur].ptno = TriList[StartTriNo].vertex[(vno+2)%3];
	v1 = StartVerNo;
	v2 = LeftVerList[lpcur].ptno;
	insert(v1,v2,ajt1,j);
	lpcur++;
	RightVerList[rpcur].ptno = StartVerNo;
	RightVerList[rpcur].trino = TriList[StartTriNo].Ajt[(vno+2)%3];
	ajt1 = RightVerList[rpcur].trino;
	//   CreateAFace(ptdata[TriList[ajt1].vertex[0]].pt,ptdata[TriList[ajt1].vertex[1]].pt,ptdata[TriList[ajt1].vertex[2]].pt,1);	                                       
	FindOppTri(j,RightVerList[rpcur].trino,StartTriNo);
	RightVerList[rpcur].Vno = j;
	rpcur++;
	RightVerList[rpcur].ptno = TriList[StartTriNo].vertex[(vno+1)%3];
	v1 = StartVerNo;
	v2 = RightVerList[rpcur].ptno;
	insert(v1,v2,ajt1,j);
	rpcur++;
	pretno = StartTriNo;
	ajt = pretno;
	//   CreateAFace(ptdata[TriList[pretno].vertex[0]].pt,ptdata[TriList[pretno].vertex[1]].pt,ptdata[TriList[pretno].vertex[2]].pt,1);	                                    
	pt[0] = (ptdata[TriList[ajt].vertex[0]].pt[0]+ptdata[TriList[ajt].vertex[1]].pt[0]+ptdata[TriList[ajt].vertex[2]].pt[0])/3.0;
	pt[1] = (ptdata[TriList[ajt].vertex[0]].pt[1]+ptdata[TriList[ajt].vertex[1]].pt[1]+ptdata[TriList[ajt].vertex[2]].pt[1])/3.0;
	do{
		ajt = TriList[pretno].Ajt[vno];
		if(ajt<0)
			return 0;
		pt[0] = (ptdata[TriList[ajt].vertex[0]].pt[0]+ptdata[TriList[ajt].vertex[1]].pt[0]+ptdata[TriList[ajt].vertex[2]].pt[0])/3.0;
		pt[1] = (ptdata[TriList[ajt].vertex[0]].pt[1]+ptdata[TriList[ajt].vertex[1]].pt[1]+ptdata[TriList[ajt].vertex[2]].pt[1])/3.0;

		ImpactTris[trcur] = ajt;
		trcur++;
		FindOppTri(j,ajt,pretno);
		verno = TriList[ajt].vertex[j];
		if(verno == EndVerNo)
			break;
		cw = CCW(StartVerNo,EndVerNo,verno);

		if(cw > 1e-12)
		{
			LeftVerList[lpcur].ptno = verno;
			ajt1 = TriList[ajt].Ajt[(j+2)%3]; 
			LeftVerList[lpcur-1].trino = ajt1;
			FindOppTri(vno,ajt1,ajt);
			LeftVerList[lpcur-1].Vno = vno;
			v1 = LeftVerList[lpcur-1].ptno;
			v2 = LeftVerList[lpcur].ptno;
			insert(v1,v2,ajt1,vno);
			vno = (j+1)%3;
			lpcur++;
		}
		else
		{
			if(fabs(cw)<1e-12)
			{
				_stprintf(mes,L"%lf %lf  sx=%lf sy=%lf ex=%lf ey=%lf %d %d",
					ptdata[StartVerNo].pt[0],ptdata[StartVerNo].pt[1],ptdata[StartVerNo].pt[2],ptdata[EndVerNo].pt[0],ptdata[EndVerNo].pt[1],ptdata[EndVerNo].pt[2],StartVerNo,EndVerNo);
				//			 AfxMessageBox(mes);
				PtZero[PtZeroNum] = verno;
				PtZeroNum++;
			}
			RightVerList[rpcur].ptno = verno;
			ajt1 = TriList[ajt].Ajt[(j+1)%3]; 
			RightVerList[rpcur-1].trino = ajt1;
			FindOppTri(vno,ajt1,ajt);
			RightVerList[rpcur-1].Vno = vno;
			v1 = RightVerList[rpcur-1].ptno;
			v2 = RightVerList[rpcur].ptno;
			insert(v1,v2,ajt1,vno);
			vno = (j+2)%3;
			rpcur++;
		}
		pretno = ajt;
	}while(verno != EndVerNo);

	LeftVerList[lpcur].ptno = verno;
	LeftVerList[lpcur-1].trino = TriList[ajt].Ajt[(j+2)%3];
	ajt1 = LeftVerList[lpcur-1].trino; 
	FindOppTri(vno,LeftVerList[lpcur-1].trino,ajt);
	LeftVerList[lpcur-1].Vno = vno;
	v1 = LeftVerList[lpcur-1].ptno;
	v2 = LeftVerList[lpcur].ptno;
	insert(v1,v2,ajt1,vno);  
	lpcur++;

	RightVerList[rpcur].ptno = verno;
	RightVerList[rpcur-1].trino = TriList[ajt].Ajt[(j+1)%3];
	ajt1 = RightVerList[rpcur-1].trino; 
	FindOppTri(vno,RightVerList[rpcur-1].trino,ajt);
	RightVerList[rpcur-1].Vno = vno;
	v1 = RightVerList[rpcur-1].ptno;
	v2 = RightVerList[rpcur].ptno;
	insert(v1,v2,ajt1,vno);
	rpcur++;  
	return 1;
}


double CFastTin::distance(double pt1[3],double pt2[3])
{
	double x;

	x = pow((pt1[0] - pt2[0]),2.0) + pow((pt1[1] - pt2[1]),2.0);
	return x;
}

double CFastTin::coc(double pt1[3],double pt2[3],double pt3[3])
{
	double aa,bb,cc;

	aa = distance(pt1,pt3);
	bb = distance(pt2,pt3);
	cc = distance(pt1,pt2);
	return((aa + bb - cc) / (2.0 * sqrt(aa) * sqrt(bb)));
}
//----------------------------------------------------------------------
//    在影响区域内采用栈按角度最大原则重构影响区域内的三角形，因个数不变
// 新生成的三角形序号采用原影响区域内的三角形号ImpacTris[].   
//----------------------------------------------------------------------
void CFastTin::ReConstructTin(struct VerList *verlist, int pcur, int &trcur)
	//verlist：   影响区域一侧边界点数组
	//pcur:  一侧边界点总数
	//trcur:  传入：影响区域内待生成的首三角形在ImpacTris[]中的下标号
	//        传出：影响区域内最后生成的三角形在ImpacTris[]中的下标号
{
	double tt,yxz;
	int i, vt2, vno,addvalue,vf,vt,trino,basenum,count=0;
	int ttrino,vvno;
	struct StackNode  *pop;
	char mes[80];
	struct record *ptree;
	char att;

	StackTop = NULL;//置空栈
	basenum = trcur;  //用于记录每次扩展前,ImpacTris[]已使用的个数
	PUSHSTACK(trcur,0, pcur-1);

	while(pop=POPSTACK())
	{
		trino = pop->trino;//当前生成的三角形在ImpacTris[]中的下标号
		vf = pop->Vno;//当前扩展边，用verlist[]中的下标号表示
		vt = pop->inf;//vf,vt与当前扩展点构成CCW 
		delete pop;
		tt = 1.000001;
		vno =vf;
		for(i=vf+1; i < vt; i++)
		{
			yxz = coc(ptdata[verlist[vf].ptno].pt,ptdata[verlist[vt].ptno].pt,ptdata[verlist[i].ptno].pt);
			if(yxz <= tt) //yxz=1.0 为共线情况
			{
				tt = yxz;
				vt2 = verlist[i].ptno;
				vno = i; 
			}
		}//以vf-vt为扩展边，按角度最大原则，找到扩展点vt2, vno为该点在verlist[]中的下标号

		TriList[ImpactTris[trino]].vertex[0] = verlist[vf].ptno;
		TriList[ImpactTris[trino]].vertex[1] = verlist[vt].ptno;
		TriList[ImpactTris[trino]].vertex[2] = vt2;  

		_tcscpy(TriList[ImpactTris[trino]].EdgeInf,L"aaa");

		ptdata[TriList[ImpactTris[trino]].vertex[0]].trino = ImpactTris[trino];
		ptdata[TriList[ImpactTris[trino]].vertex[1]].trino = ImpactTris[trino];
		ptdata[TriList[ImpactTris[trino]].vertex[2]].trino = ImpactTris[trino];

		//  CreateAFace(ptdata[TriList[ImpactTris[trino]].vertex[0]].pt,ptdata[TriList[ImpactTris[trino]].vertex[1]].pt,ptdata[TriList[ImpactTris[trino]].vertex[2]].pt,2);	                                    

		addvalue = 0;//记录每次扩展ImpacTris[]中的下标增量
		if(vno-vf > 1)//表示vf-vno不是影响区域的边界边,还继续扩展
		{
			TriList[ImpactTris[trino]].Ajt[1] = ImpactTris[basenum+1];
			TriList[ImpactTris[basenum+1]].Ajt[2] = ImpactTris[trino];
			PUSHSTACK(basenum+1,vf,vno); //入栈继续扩展过程
			addvalue++;
		}
		else//表示vno-vf是影响区域的边界边,建立邻接关系
		{
			ptree = insert(verlist[vf].ptno,verlist[vno].ptno,ttrino,vvno);
			if(ptree->mak == 2)
			{
				TriList[ImpactTris[trino]].Ajt[1] = ttrino;
				TriList[ImpactTris[trino]].EdgeInf[1] = TriList[ttrino].EdgeInf[vvno];
				TriList[ttrino].Ajt[vvno] = ImpactTris[trino];
			}
			else
			{
				ptree->mak = 1;
				ptree->trino = ImpactTris[trino];
				ptree->Vno = 1;
			}		
		}

		if(vt-vno > 1)//表示vno-vt不是影响区域的边界边,还继续扩展
		{
			addvalue++;
			TriList[ImpactTris[trino]].Ajt[0] = ImpactTris[basenum+addvalue];
			TriList[ImpactTris[basenum+addvalue]].Ajt[2] = ImpactTris[trino];
			PUSHSTACK(basenum+addvalue,vno,vt);		
		}
		else
		{
			ptree = insert(verlist[vno].ptno,verlist[vt].ptno,ttrino,vvno);
			if(ptree->mak == 2)
			{
				TriList[ImpactTris[trino]].Ajt[0] = ttrino;
				TriList[ImpactTris[trino]].EdgeInf[0] = TriList[ttrino].EdgeInf[vvno];
				TriList[ttrino].Ajt[vvno] = ImpactTris[trino];
			}
			else
			{
				ptree->mak = 1;
				ptree->trino = ImpactTris[trino];
				ptree->Vno = 0;
			}
		}
		trcur = trcur + addvalue;
		basenum = basenum+addvalue;
	}  
}

//itri 设置itri 属性
void CFastTin::SetATinAtt(int itri)
{
	int ii,k,jdnum,v0,v1,v2;
	double tinx,tiny;

	v0 = TriList[itri].vertex[0];
	v1 = TriList[itri].vertex[1];
	v2 = TriList[itri].vertex[2];
	tinx = (ptdata[v0].pt[0]+ptdata[v1].pt[0]+ptdata[v2].pt[0])/3.0;
	tiny = (ptdata[v0].pt[1]+ptdata[v1].pt[1]+ptdata[v2].pt[1])/3.0;

	AcDbLine L2(AcGePoint3d(tinx,tiny,0.0),AcGePoint3d(tinx,0.0,0.0));

	for(ii=0;ii<NRoad;ii++)
	{
		for(k=0; k<RoadHole[ii]->HoleNum; k++)
		{  
			//			ads_printf(L"itri=%d iroad=%d ihole=%d \n",itri,ii,k);
			//			ads_printf(L"ii=%d k=%d\n",ii,k);
			if(RoadHole[ii]->HoleArray[k].nLpt==0||RoadHole[ii]->HoleArray[k].nRpt==0)
				continue;
			AcGePoint3dArray interPt;
			L2.intersectWith(RoadHole[ii]->HoleArray[k].pline,AcDb::kOnBothOperands,interPt,0,0);
			jdnum = interPt.length();
			if(fmod(1.0*jdnum,2)==1)//奇数个交点 在区域内
			{
				TriList[itri].Attribute='M';
				return;
			}
			if (interPt.length()>0)
			{
				interPt.removeSubArray(0,interPt.length()-1);
			}
		}
	}
}
void CFastTin::DigHole()
{ 
	int k,ll,ii,i;
	double pt[2];
	char mes[80];
	struct StackNode *pop;
	ads_point ptt;

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"setvar", RTSTR, L"cmdecho", RTSHORT, 0, RTNONE);
#endif // VERSION_CAD2010
	

	// 	
	for(ii=0;ii<NRoad;ii++)
	{
		//	ads_printf(L"aanum=%d\n",RoadHole[ii]->HoleNum);
		for(k=0; k<RoadHole[ii]->HoleNum; k++)
		{  

			if(RoadHole[ii]->HoleArray[k].nLpt==0||RoadHole[ii]->HoleArray[k].nRpt==0)
				continue;
			//		    ads_printf(L"ii=%d,k=%d\n",ii,k);	
			DealHole(ii,k);
		}
	}

	for(ii=0;ii<NRoad;ii++)
	{
		//	ads_printf(L"aanum=%d\n",RoadHole[ii]->HoleNum);
		for(k=0; k<RoadHole[ii]->HoleNum; k++)
		{  

			if(RoadHole[ii]->HoleArray[k].nLpt==0||RoadHole[ii]->HoleArray[k].nRpt==0)
				continue;

			DealHole(ii,k);

			pt[0] = RoadHole[ii]->HoleArray[k].Inpt[0];
			pt[1] = RoadHole[ii]->HoleArray[k].Inpt[1];

			ll = LocateTriOnXY(pt);	



			//	if(k==37)
			//	{
			//		ads_printf(L"$$$$$$$$$$$$$$$$$$$$$$$$$$$$ %lf %lf\n",pt[0],pt[1]);



			// }

			ads_printf(L"ii=%d,k=%d,%d\n",ii,k,RoadHole[ii]->HoleNum);

			/*
			if(ii==1&&k==0)
			{
			ads_printf(L"%d %lf %lf\n",ll,pt[0],pt[1])	;
			}*/

			if(ll==-1)
				continue;

			init_queue();
			enqueue(ll);	
			TriList[ll].Attribute = 'N'; 
			for(i=0;i<3;i++)
			{
				if(ptdata[TriList[ll].vertex[i]].att!='c')
					ptdata[TriList[ll].vertex[i]].att='d';
			}

			if(RoadHole[ii]->HoleArray[k].qInpt[0]>0&&RoadHole[ii]->HoleArray[k].qInpt[1]>0)
			{
				pt[0] = RoadHole[ii]->HoleArray[k].qInpt[0];
				pt[1] = RoadHole[ii]->HoleArray[k].qInpt[1];

				ll = LocateTriOnXY(pt);	
				if(ll!=-1)
				{
					enqueue(ll);	
					TriList[ll].Attribute = 'N'; 
					for(i=0;i<3;i++)
					{
						if(ptdata[TriList[ll].vertex[i]].att!='c')
							ptdata[TriList[ll].vertex[i]].att='d';
					}
				}
			}


			while( notempty() )
			{
				pop = dequeue();
				ll  = pop->trino;


				// CreateAFace(ptdata[TriList[ll].vertex[0]].pt,ptdata[TriList[ll].vertex[1]].pt,ptdata[TriList[ll].vertex[2]].pt,7);
				//  AcGePoint3d pt,ver1,ver2,ver3;
				//
				//  pt.set(35598786.2134,3570253.9838,0.0);
				//  ver1.set(ptdata[TriList[ll].vertex[0]].pt[0],ptdata[TriList[ll].vertex[0]].pt[1],ptdata[TriList[ll].vertex[0]].pt[2]);
				//  ver2.set(ptdata[TriList[ll].vertex[1]].pt[0],ptdata[TriList[ll].vertex[1]].pt[1],ptdata[TriList[ll].vertex[1]].pt[2]);
				//  ver3.set(ptdata[TriList[ll].vertex[2]].pt[0x],ptdata[TriList[ll].vertex[2]].pt[1],ptdata[TriList[ll].vertex[2]].pt[2]);
				//	if(ii==8)
				//			{
				if(k==83)
				{
					//  ads_printf(L"ll=%d %c%c%c %d %d %d  %d %d %d %lf %lf %lf %lf %lf %lf\n",ll,TriList[ll].EdgeInf[0],TriList[ll].EdgeInf[1],
					// 	 TriList[ll].EdgeInf[2],TriList[ll].Ajt[0],TriList[ll].Ajt[1],TriList[ll].Ajt[2],TriList[ll].vertex[0],TriList[ll].vertex[1],TriList[ll].vertex[2],
					// 	 ptdata[TriList[ll].vertex[0]].pt[1],ptdata[TriList[ll].vertex[0]].pt[0],
					// 	 ptdata[TriList[ll].vertex[1]].pt[1],ptdata[TriList[ll].vertex[1]].pt[0],
					// 	 ptdata[TriList[ll].vertex[2]].pt[1],ptdata[TriList[ll].vertex[2]].pt[0]);
					ads_point pt1,pt2,pt3;
					pt1[0]=ptdata[TriList[ll].vertex[0]].pt[1],pt1[1]=ptdata[TriList[ll].vertex[0]].pt[0],pt1[2]=ptdata[TriList[ll].vertex[0]].pt[2];
					pt2[0]=ptdata[TriList[ll].vertex[1]].pt[1],pt2[1]=ptdata[TriList[ll].vertex[1]].pt[0],pt2[2]=ptdata[TriList[ll].vertex[1]].pt[2];
					pt3[0]=ptdata[TriList[ll].vertex[2]].pt[1],pt3[1]=ptdata[TriList[ll].vertex[2]].pt[0],pt3[2]=ptdata[TriList[ll].vertex[2]].pt[2];
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR,L"Pline",RT3DPOINT,pt1,RT3DPOINT,pt2,RT3DPOINT,pt3,RT3DPOINT,pt1,RTSTR,L"",0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"Pline", RT3DPOINT, pt1, RT3DPOINT, pt2, RT3DPOINT, pt3, RT3DPOINT, pt1, RTSTR, L"", 0);
#endif // VERSION_CAD2010
					ACHAR res[80];
					if(acedGetKword(L"input:\n",res)!=RTNONE)
						return;
				}

				//	ads_printf(L"ii=%d k=%d %d %d\n",ii,k,RoadHole[ii]->HoleArray[k].nLpt,RoadHole[ii]->HoleArray[k].nRpt);
				//	}

				//
				//  if(pt.distan,ceTo(ver1)<0.1 || pt.distanceTo(ver2)<0.1 || pt.distanceTo(ver3)<0.1)
				//  {
				//	  ads_printf(L"@@@@@@@@@@@@@@ ll=%d\n",ll);
				//	  ads_alert(L"1");
				//  }
				//
				// // acedCommandS(RTSTR,L"regen",0);
				// ads_alert(L"1");

				free(pop);

				if(TriList[ll].EdgeInf[0]!='f'&& TriList[ll].Ajt[0]>=0 && TriList[TriList[ll].Ajt[0]].Attribute != 'N')
				{
					TriList[TriList[ll].Ajt[0]].Attribute = 'N';
					for(i=0;i<3;i++)
					{
						if(ptdata[TriList[TriList[ll].Ajt[0]].vertex[i]].att!='c')
							ptdata[TriList[TriList[ll].Ajt[0]].vertex[i]].att='d';
					}
					enqueue(TriList[ll].Ajt[0]);
				}

				if(TriList[ll].EdgeInf[1]!='f' && TriList[ll].Ajt[1]>=0  && TriList[TriList[ll].Ajt[1]].Attribute != 'N')
				{
					TriList[TriList[ll].Ajt[1]].Attribute = 'N';
					for(i=0;i<3;i++)
					{
						if(ptdata[TriList[TriList[ll].Ajt[1]].vertex[i]].att!='c')
							ptdata[TriList[TriList[ll].Ajt[1]].vertex[i]].att='d';
					}
					enqueue(TriList[ll].Ajt[1]);
				}
				//	ads_printf(L"3 %s %d\n",TriList[ll].EdgeInf,TriList[ll].Ajt[2]);			
				if(TriList[ll].EdgeInf[2]!='f' && TriList[ll].Ajt[2]>=0 && TriList[TriList[ll].Ajt[2]].Attribute != 'N')
				{
					TriList[TriList[ll].Ajt[2]].Attribute = 'N';
					for(i=0;i<3;i++)
					{
						if(ptdata[TriList[TriList[ll].Ajt[2]].vertex[i]].att!='c')
							ptdata[TriList[TriList[ll].Ajt[2]].vertex[i]].att='d';
					}
					enqueue(TriList[ll].Ajt[2]);
				}

			}//end while( notempty() )	

		}
	}
}

void CFastTin::enqueue(int trino)
{
	struct StackNode *newptr; 

	newptr = (struct StackNode *)calloc(1,sizeof(struct StackNode));
	if(newptr) {
		newptr->trino = trino;
		newptr->next = NULL;
		tailptr->next = newptr;
		tailptr = newptr;
	}
	else
		AfxMessageBox(L"分配内存失败！");
}

CFastTin::StackNode *CFastTin::dequeue()
{
	struct StackNode *poptr;

	poptr = headptr->next;
	if(poptr == tailptr)
		tailptr = headptr;
	(headptr->next) = (poptr->next);
	return poptr;
}

void CFastTin::init_queue( )
{
	struct StackNode *newptr;

	newptr = (struct StackNode *)calloc(1,sizeof(struct StackNode));
	newptr->next = NULL;
	headptr = newptr;
	tailptr = newptr;
}

CFastTin::StackNode  *CFastTin::notempty()
{
	return (headptr->next);
}


int  CFastTin::SetEdgeInf(double ver1[2],double ver2[2])
{
	int StartVerNo,EndVerNo,StartTriNo;
	int trino,j,inf;
	double cw1,cw2;
	int ajt;

	double dis;
	dis=sqrt((ver1[0]-ver2[0])*(ver1[0]-ver2[0])+(ver1[1]-ver2[1])*(ver1[1]-ver2[1]));
	if(dis<0.0001)
	{
		//ads_printf(L"x1=%lf y1=%lf x2=%lf y2=%lf\n",ver1[0],ver1[1],ver2[0],ver2[1]);

		return -1;

	}
	StartTriNo=-1;

	StartVerNo = FindVerNo(ver1);
	EndVerNo = FindVerNo(ver2);
	if(StartVerNo<0 || EndVerNo<0)
		return -1;


	trino = ptdata[StartVerNo].trino;//起始三角形
	inf = 1;
	do
	{
		j=0;
		while(j<3 && TriList[trino].vertex[j]!=StartVerNo)
			j++;
		cw1 = CCW(StartVerNo,EndVerNo,TriList[trino].vertex[(j+1)%3]);
		cw2 = CCW(StartVerNo,EndVerNo,TriList[trino].vertex[(j+2)%3]);
		if(trino==-1)
		{
			if(inf==1)
				inf = 0;
			else
			{
				//	 ads_printf(L"### %lf %lf %lf %lf\n",ver1[0],ver1[1],ver2[0],ver2[1]);

				return -1;
			}
			trino = ptdata[StartVerNo].trino;
		}
		StartTriNo = trino;
		if(inf)
			trino = TriList[trino].Ajt[(j+1)%3];
		else if(inf==0)
			trino = TriList[trino].Ajt[(j+2)%3];
	} while(cw1>0.0 || cw2 <0.0) ;//该过程在以StartVerNo为顶点一圈三角形中，找出StartVerNo-EndVerNo穿越的三角形


	if(TriList[StartTriNo].vertex[(j+1)%3]==EndVerNo)
	{
		TriList[StartTriNo].EdgeInf[(j+2)%3]='f';
		ajt = TriList[StartTriNo].Ajt[(j+2)%3];
		if(ajt !=-1)
		{
			FindOppTri(j,ajt,StartTriNo);
			TriList[ajt].EdgeInf[j]='f';
		}
		// 	    if(StartTriNo==13810 || StartTriNo==16544)
		//    {
		// 	   ads_printf(L"ajt=%d j=%d %lf %lf , %lf %lf, %lf %lf, %c%c%c\n",ajt,j,ptdata[TriList[ajt].vertex[0]].pt[0],ptdata[TriList[ajt].vertex[0]].pt[1],
		// 		   ptdata[TriList[ajt].vertex[1]].pt[0],ptdata[TriList[ajt].vertex[1]].pt[1],
		// 		   ptdata[TriList[ajt].vertex[2]].pt[0],ptdata[TriList[ajt].vertex[2]].pt[1],
		// 		   TriList[ajt].EdgeInf[0],TriList[ajt].EdgeInf[1],TriList[ajt].EdgeInf[2]);
		// 
		//    }
		return 1;
	}

	if(TriList[StartTriNo].vertex[(j+2)%3]==EndVerNo)
	{
		TriList[StartTriNo].EdgeInf[(j+1)%3]='f';
		ajt = TriList[StartTriNo].Ajt[(j+1)%3];
		if(ajt != -1)
		{
			FindOppTri(j,ajt,StartTriNo);
			TriList[ajt].EdgeInf[j]='f';
		}
		return 1;
	}
	return 1;
}     


CFastTin::record *CFastTin::insert(int v1,int v2,int &trino,int &Vno)
{
	struct record *pivot,*temp,*temp1, *temp2, *temp3, *p, *q;
	int d,eFlag;
	int  balanceflag;
	char tempedge[30], string[7], *blk = "," ;

	if(v1 < v2)
	{
		itoa(v1,string,10);
		strcpy(tempedge,string);
		strcat(tempedge,blk);
		itoa(v2,string,10);
		strcat(tempedge,string);
	}
	else
	{
		itoa(v2,string,10);
		strcpy(tempedge,string);
		strcat(tempedge,blk);
		itoa(v1,string,10);
		strcat(tempedge,string);
	}

	if (Root == NULL)
	{
		//        ads_printf(L"\t\t<* * Insert the first item * *>\n");
		(Root) = (struct record *)calloc(1 , sizeof(struct record));
		strcpy((Root)->edge , tempedge);
		(Root)->bf = 0;
		(Root)->mak = 1;
		(Root)->trino = trino;
		(Root)->Vno = Vno;
		(Root)->llink = NULL;
		(Root)->rlink = NULL;
	}
	else              /*temp1 points to the youngest ancestor which may
					  become unbalanced,pivot points to the father of temp1,
					  and q points to the father of p. */
	{
		pivot = NULL;
		temp1 = Root;
		p = Root;
		q = NULL;
		while(p != NULL && strcmp(p->edge, tempedge) != 0)
		{
			if(p->bf != 0)
			{
				temp1 = p;
				pivot = q;
			}
			if(strcmp(tempedge, p->edge) > 0)
			{
				q = p;
				p = q->rlink;
			}
			else if(strcmp(tempedge, p->edge) < 0)
			{
				q = p;
				p = q->llink;
			}
		} /* end of while() */

		if(p==NULL)
		{
			eFlag=1;
		}
		else
		{
			if(strcmp(p->edge,tempedge)==0)
				eFlag=0;
			else
				eFlag=1;
		}

		if(eFlag==0)
		{
			//                 ads_printf(L"/t/t==This edge already exist ==\n");
			p->mak++;
			trino = p->trino;
			Vno = p->Vno;    
			return p;
		}
		else
		{
			//                 ads_printf(L"\t\t ** Insert begin **\n");
			temp = (struct record *)calloc(1,sizeof(struct record));
			if( !temp)
			{
				AfxMessageBox(L"No enough memory !\n");
				return false;
			}
			strcpy(temp->edge, tempedge);
			temp->bf = 0;
			temp->mak = 1;
			temp->trino = trino;
			temp->Vno = Vno;
			temp->llink = NULL;
			temp->rlink = NULL;
			if(strcmp(tempedge, q->edge) > 0)
			{
				q->rlink = temp;
			}
			else
			{
				q->llink = temp;
			}
			if(strcmp(tempedge, temp1->edge) > 0)
			{
				p = temp1->rlink;
				temp2 = p;
				d = -1;
			}
			else
			{
				p = temp1->llink;
				temp2 = p;
				d = 1;
			}

			while(p != temp && p != NULL)
			{
				if(strcmp(tempedge, p->edge) > 0)
				{
					p->bf = -1;
					p = p->rlink;
				}
				else
				{
					p->bf = 1;
					p = p->llink;
				}
			}
		}
		balanceflag = 1;
		if(temp1->bf == 0)
		{
			//                   ads_printf(L"\t\t ** Tree still balance **\n");
			temp1->bf = d;
			balanceflag = 0;
		}
		if(temp1->bf + d == 0)
		{
			//                   ads_printf(L"\t\t * * Tree still balance * *\n");
			temp1->bf = 0;
			balanceflag = 0;
		}
		if(balanceflag == 1)
		{
			//                   ads_printf(L"\t\t ** Rebalance begin **\n");
			if(d == 1)
			{
				if(temp2->bf == 1)
				{
					//                           ads_printf(L"\t\t<<LL type>> \n");
					temp1->llink = temp2->rlink;
					temp2->rlink = temp1;
					temp1->bf = 0;
					temp2->bf = 0;
				}
				else
				{
					//                           ads_printf(L"\t\t< < LR type > >\n");
					temp3 = temp2->rlink;
					temp2->rlink = temp3->llink;
					temp1->llink = temp3->rlink;
					temp3->llink = temp2;
					temp3->rlink = temp1;
					switch(temp3->bf)
					{
					case 0: temp1->bf = 0;
						temp2->bf = 0;
						break;
					case 1: temp1->bf = -1;
						temp2->bf = 0;
						break;
					case -1: temp1->bf = 0;
						temp2->bf =1;
						break;
					}
					temp3->bf = 0;
					temp2 = temp3;
				}  /* end of if(temp2->bf = 1) */
			}
			else
			{
				if(temp2->bf == -1)
				{
					//                     ads_printf(L"\t\t<< RR type>>/n");
					temp1->rlink = temp2->llink;
					temp2->llink = temp1;
					temp1->bf = 0;
					temp2->bf = 0;
				}
				else
				{
					//                       ads_printf(L"\t\t<< RL type >> \n");
					temp3 = temp2->llink;
					temp1->rlink = temp3->llink;
					temp2->llink = temp3->rlink;
					temp3->llink = temp1;
					temp3->rlink = temp2;
					switch(temp3->bf)
					{
					case 0:  temp1->bf = 0;
						temp2->bf = 0;
						break;
					case 1:  temp1->bf = 0;
						temp2->bf = -1;
						break;
					case -1: temp1->bf = 1;
						temp2->bf = 0;
						break;
					}
					temp3->bf = 0;
					temp2 = temp3;
				}
			} /* end of if(d == 1) */
			if(pivot ==NULL)
			{
				Root = temp2;
			}
			else if(temp1 == pivot->llink)
			{
				pivot->llink = temp2;
			}
			else if(temp1 == pivot->rlink)
			{
				pivot->rlink =temp2;
			} /* end of if(pivot == NULL) */
		} /* end of if(balanceflag == 1) */
	} /* end of if(Root == NULL) */
	return temp;
}

void CFastTin::detree(struct record *rt)
{
	struct record *p,*p1;

	if(rt->llink == rt->rlink)
	{
		free(rt);
		return;
	}
	else if(rt->llink == NULL)
	{
		p = rt->rlink;
		free(rt);
		detree(p);
		return;
	}
	else if(rt->rlink == NULL)
	{
		p = rt->llink;
		free(rt);
		detree(p);
		return;
	}
	else
	{
		p = rt->llink;
		p1 = rt->rlink;
		free(rt);
		detree(p);
		detree(p1);
		return;
	}
}

double CFastTin::interpolate(double pt[2],int l)
{
	double d, a, b,c;
	if( l > 0 )
	{
		d = ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[1]].pt[1] *
			ptdata[TriList[l].vertex[2]].pt[2] + ptdata[TriList[l].vertex[1]].pt[0] *
			ptdata[TriList[l].vertex[2]].pt[1] * ptdata[TriList[l].vertex[0]].pt[2] +
			ptdata[TriList[l].vertex[2]].pt[0] * ptdata[TriList[l].vertex[0]].pt[1] *
			ptdata[TriList[l].vertex[1]].pt[2] - ptdata[TriList[l].vertex[2]].pt[0] *
			ptdata[TriList[l].vertex[1]].pt[1] * ptdata[TriList[l].vertex[0]].pt[2] -
			ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[2]].pt[1] *
			ptdata[TriList[l].vertex[1]].pt[2] - ptdata[TriList[l].vertex[1]].pt[0] *
			ptdata[TriList[l].vertex[0]].pt[1] * ptdata[TriList[l].vertex[2]].pt[2];
		a = ptdata[TriList[l].vertex[0]].pt[1] * ptdata[TriList[l].vertex[1]].pt[2] +
			ptdata[TriList[l].vertex[1]].pt[1] * ptdata[TriList[l].vertex[2]].pt[2] +
			ptdata[TriList[l].vertex[2]].pt[1] * ptdata[TriList[l].vertex[0]].pt[2] -
			ptdata[TriList[l].vertex[2]].pt[1] * ptdata[TriList[l].vertex[1]].pt[2] -
			ptdata[TriList[l].vertex[0]].pt[1] * ptdata[TriList[l].vertex[2]].pt[2] -
			ptdata[TriList[l].vertex[1]].pt[1] * ptdata[TriList[l].vertex[0]].pt[2];
		b = ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[1]].pt[2] +
			ptdata[TriList[l].vertex[1]].pt[0] * ptdata[TriList[l].vertex[2]].pt[2] +
			ptdata[TriList[l].vertex[2]].pt[0] * ptdata[TriList[l].vertex[0]].pt[2] -
			ptdata[TriList[l].vertex[2]].pt[0] * ptdata[TriList[l].vertex[1]].pt[2] -
			ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[2]].pt[2] -
			ptdata[TriList[l].vertex[1]].pt[0] * ptdata[TriList[l].vertex[0]].pt[2];
		c = ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[1]].pt[1] +
			ptdata[TriList[l].vertex[1]].pt[0] * ptdata[TriList[l].vertex[2]].pt[1] +
			ptdata[TriList[l].vertex[2]].pt[0] * ptdata[TriList[l].vertex[0]].pt[1] -
			ptdata[TriList[l].vertex[2]].pt[0] * ptdata[TriList[l].vertex[1]].pt[1] -
			ptdata[TriList[l].vertex[0]].pt[0] * ptdata[TriList[l].vertex[2]].pt[1] -
			ptdata[TriList[l].vertex[1]].pt[0] * ptdata[TriList[l].vertex[0]].pt[1];

		return(d/c - a/c * pt[0] + b/c * pt[1]);
	}
	else
	{
		return 0;
	}
}
//找到第一个非空网格
int CFastTin::FindFirstNoEmptyGridNo(int crow,int ccol)
{
	int i,j;

	i=crow+1;
	while(i<NRow)
	{
		if(GridArray[i*NCol+ccol].Pfcur!=-1)
			return i*NCol+ccol;
		i++;
	}
	i=crow-1;
	while(i>=0)
	{
		if(GridArray[i*NCol+ccol].Pfcur!=-1)
			return i*NCol+ccol;
		i--;
	}

	j=ccol+1;
	while(j<NCol)
	{
		if(GridArray[crow*NCol+j].Pfcur!=-1)
			return crow*NCol+j;
		j++;
	}

	j=ccol-1;
	while(j>=0)
	{
		if(GridArray[crow*NCol+j].Pfcur!=-1)
			return crow*NCol+j;
		j--;
	}

	for(i=0; i<NRow; i++)
		for(j=0; j<NCol; j++)
		{
			if(GridArray[i*NCol+j].Pfcur!=-1)
				return i*NCol+j;
		}		

		return -1;
}



void CFastTin::WritePtIn3DFile(FILE *fpr,FILE *fpw,int &NumAll)
{
	double tmp;
	double LP[3],RP[3];
	struct PT spt;

	fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf%lf%lf%lf",&tmp,&tmp,&tmp,&LP[0],&LP[1],&LP[2],&RP[0],
		&RP[1],&RP[2]);
	if(fabs(LP[0])>0.01)
	{
		spt.pt[0] = LP[0];
		spt.pt[1] = LP[1];
		spt.pt[2] = LP[2];
		spt.cur = -1;
		spt.att = 'c';//坡脚点
		spt.trino = -1;
		fwrite(&spt,sizeof(struct PT),1,fpw);
		NumAll++;
		//		fwrite(&Tri,sizeof(struct Triangle),1,fpw1);
		//		fwrite(&Tri,sizeof(struct Triangle),1,fpw1);
	}
	if(fabs(RP[0])>0.01)
	{
		spt.pt[0] = RP[0];
		spt.pt[1] = RP[1];
		spt.pt[2] = RP[2];
		spt.cur = -1;
		spt.att = 'c';//坡脚点
		spt.trino = -1;
		fwrite(&spt,sizeof(struct PT),1,fpw);
		NumAll++;
		//		fwrite(&Tri,sizeof(struct Triangle),1,fpw1);
		//		fwrite(&Tri,sizeof(struct Triangle),1,fpw1);
	}
}


BOOL CFastTin::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	this->SetWindowText(L"RLDVS V1.0");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFastTin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}


void CFastTin::AdjustBjx()
{

	double ver[2];
	int verno;
	int i;
	for( i=0;i<NRoad;i++)
	{
		for(int j=0;j<RoadHole[i]->HoleNum;j++)
		{
			int k;
			for(k=0;k<RoadHole[i]->HoleArray[j].nLpt;k++)
			{
				ver[0] = RoadHole[i]->HoleArray[j].LptArr[k].y;
				ver[1] = RoadHole[i]->HoleArray[j].LptArr[k].x;
				verno = FindVerNo(ver);
				if(verno>=0)
				{
					RoadHole[i]->HoleArray[j].LptArr[k].y = ptdata[verno].pt[0];
					RoadHole[i]->HoleArray[j].LptArr[k].x = ptdata[verno].pt[1];
				}

			}
			for(k=0;k<RoadHole[i]->HoleArray[j].nRpt;k++)
			{
				ver[0] = RoadHole[i]->HoleArray[j].RptArr[k].y;
				ver[1] = RoadHole[i]->HoleArray[j].RptArr[k].x;
				verno = FindVerNo(ver);
				if(verno>=0)
				{
					RoadHole[i]->HoleArray[j].RptArr[k].y = ptdata[verno].pt[0];
					RoadHole[i]->HoleArray[j].RptArr[k].x = ptdata[verno].pt[1];
				}

			}			
		}
	}

}