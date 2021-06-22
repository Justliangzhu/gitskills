// DTM.cpp: implementation of the DTM class.
//
//////////////////////////////////////////////////////////////////////


#include  "stdafx.h"
#include <io.h>
#include <process.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "BAS_DRAW_FUN.h"
#include  "triDTM_road.h"

#define X 0
#define Y 1 
#define Z 2
#define FindOppTri(j,ajtno,trino)  j=0;\
	if(ajtno!=-1)\
		 {\
		 while(j<3 && trino!=tarray[ajtno].Ajt[j])\
		 j++;\
		 }\

extern CString Cworkdir,Pro;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
triDTM_road::triDTM_road()
{
	GridWidth = 100;
	DTMHasRead = 0;
	parray = NULL;
	tarray = NULL;
	GridArray = NULL;
	IsDtm = FALSE;
	DefaultH = 0.0;//缺省高程
	mapfileHandleP=NULL;
	MEMmapfileHandleP=NULL;
	mapfileHandleT= NULL;
	MEMmapfileHandleT=NULL;
	mapfileHandleG=NULL;
	MEMmapfileHandleG=NULL;
	FilePtrP=NULL;
	FilePtrT=NULL;
	FilePtrG=NULL;    
	LeftLowPt[0] = LeftLowPt[1] = RightUpPt[0] = RightUpPt[1] =0.0;

}

triDTM_road::~triDTM_road()
{
}

////////////////////////////////////////////函数声明///////////////////////////////////////////////////////////////
int triDTM_road::Read_Dtm()//导入数模
{
	FILE *fpr;
	ACHAR fname[256];
	/*

	GridWidth = 100;
	DTMHasRead = 0;*/

	/*
	mapfileHandleP=NULL;
	MEMmapfileHandleP=NULL;
	mapfileHandleT= NULL;
	MEMmapfileHandleT=NULL;
	mapfileHandleG=NULL;
	MEMmapfileHandleG=NULL;
	FilePtrP=NULL;
	FilePtrT=NULL;
	FilePtrG=NULL;   */ 




	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	//ACHAR RailWorkDir[512], RailPro[512];
	//ob.ReadRailWorkdir(RailWorkDir, RailPro);

	ACHAR szPath[MAX_PATH];
	ACHAR TempCurTinName[80]=L"";
	CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
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
		_stprintf(fname, L"%s\\PMAP\\PtIndex.idx", Cworkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);   
		_stprintf(fname, L"%s\\PMAP\\PtIndex.idx.%s", Cworkdir, CurTinName); //zy-测试


	/*ads_printf(L"正在导入数模数据文件,请稍候...\n");
	_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);*/
	//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",RailWorkDir);

	if( (fpr=_wfopen(fname,L"r"))==NULL)
	{
	//	AfxMessageBox(L"未构建数字地面模型，无法内插地面线，地面高程均所给高程平均值处理!");
		ads_printf(L"未构建数字地面模型，无法内插地面线，地面高程均所给高程平均值处理!\n");
		IsDtm = FALSE;
		return 0;
	}
	fwscanf(fpr ,L"%d%lf%lf%lf%lf" , &PTSUM,&LeftLowPt[0],&LeftLowPt[1],&RightUpPt[0],&RightUpPt[1]);

	fclose(fpr);

	if(parray)//DTM已经读入
		ReleaseDtm();//释放


	//BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	//ACHAR szPath[MAX_PATH];
	//ACHAR TempCurTinName[80];
	_tcscpy(TempCurTinName, L"");
	//CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
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
		_stprintf(fname, L"%s\\PMAP\\Point.xyz", Cworkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);   
		_stprintf(fname, L"%s\\PMAP\\Point.xyz.%s", Cworkdir, CurTinName); //zy-测试


	// _stprintf(fname,L"%s\\PMAP\\Point.xyz",Cworkdir);   
//	_stprintf(fname,L"%s\\PMAP\\Point.xyz",RailWorkDir);
	mapfileHandleP = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);

	_tcscpy(fname,L"MemMapping");
	MEMmapfileHandleP = CreateFileMapping(mapfileHandleP,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrP = MapViewOfFile(MEMmapfileHandleP,FILE_MAP_WRITE,0,0,0);
	parray =  (struct PT *)FilePtrP;


	//BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	//ACHAR szPath[MAX_PATH];
	//ACHAR TempCurTinName[80];
	_tcscpy(TempCurTinName, L"");
	//CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
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
		_stprintf(fname, L"%s\\PMAP\\TIN.tri", Cworkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);   
		_stprintf(fname, L"%s\\PMAP\\TIN.tri.%s", Cworkdir, CurTinName); //zy-测试


	//_stprintf(fname,L"%s\\PMAP\\TIN.tri",Cworkdir);   
	//_stprintf(fname,L"%s\\PMAP\\TIN.tri",RailWorkDir);   
	mapfileHandleT = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);

	_tcscpy(fname,L"MemMapT");
	MEMmapfileHandleT = CreateFileMapping(mapfileHandleT,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrT = MapViewOfFile(MEMmapfileHandleT,FILE_MAP_WRITE,0,0,0);
	tarray =  (struct Triangle *)FilePtrT;

	if(GridArray) delete []GridArray;
	NRow = (int)((RightUpPt[0] - LeftLowPt[0])/GridWidth+0.001)+1;
	NCol = (int)((RightUpPt[1] - LeftLowPt[1])/GridWidth+0.001)+1;
	GridArray =  new struct GridNode[NRow*NCol];

	//BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	//ACHAR szPath[MAX_PATH];
	//ACHAR TempCurTinName[80];
	_tcscpy(TempCurTinName, L"");
	//CString CurTinName;
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
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
		_stprintf(fname, L"%s\\PMAP\\Grid.idx", Cworkdir);
	else
		//_stprintf(fname,L"%s\\PMAP\\PtIndex.idx",Cworkdir);   
		_stprintf(fname, L"%s\\PMAP\\Grid.idx.%s", Cworkdir, CurTinName); //zy-测试


	//_stprintf(fname,L"%s\\PMAP\\Grid.idx",Cworkdir);   
	//_stprintf(fname,L"%s\\PMAP\\Grid.idx",RailWorkDir);
	mapfileHandleG = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);
	DWORD dwBytesWritten=0; 
	ReadFile(mapfileHandleG,                // handle of file to read
		GridArray,             // pointer to buffer that receives data
		sizeof(struct GridNode)*NCol*NRow,  // number of bytes to read
		&dwBytesWritten, // pointer to number of bytes read
		NULL    // pointer to structure for data
		);
	IsDtm = true;
	DTMHasRead = 1;
	return true;
}


double triDTM_road::interpolate(double pt[2])//纵断面内插,输入x,y返回地面标高z
{
	double d, a, b,c;
	int l;

	if(!IsDtm)
		return DefaultH;

	std::vector<GridNode> watchvec;
	for (int i = 0; i < NCol * NRow; i++)
	{
		watchvec.push_back(GridArray[i]);
	}

	l = LocatetrlNew(pt);

	if( l > 0 )
	{
		if(tarray[l].Attribute == 'N')//找到的三角形为废三角形
			return -100.0;
		d = parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[1]].pt[Y] *
			parray[tarray[l].vertex[2]].pt[Z] + parray[tarray[l].vertex[1]].pt[X] *
			parray[tarray[l].vertex[2]].pt[Y] * parray[tarray[l].vertex[0]].pt[Z] +
			parray[tarray[l].vertex[2]].pt[X] * parray[tarray[l].vertex[0]].pt[Y] *
			parray[tarray[l].vertex[1]].pt[Z] - parray[tarray[l].vertex[2]].pt[X] *
			parray[tarray[l].vertex[1]].pt[Y] * parray[tarray[l].vertex[0]].pt[Z] -
			parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[2]].pt[Y] *
			parray[tarray[l].vertex[1]].pt[Z] - parray[tarray[l].vertex[1]].pt[X] *
			parray[tarray[l].vertex[0]].pt[Y] * parray[tarray[l].vertex[2]].pt[Z];
		a = parray[tarray[l].vertex[0]].pt[Y] * parray[tarray[l].vertex[1]].pt[Z] +
			parray[tarray[l].vertex[1]].pt[Y] * parray[tarray[l].vertex[2]].pt[Z] +
			parray[tarray[l].vertex[2]].pt[Y] * parray[tarray[l].vertex[0]].pt[Z] -
			parray[tarray[l].vertex[2]].pt[Y] * parray[tarray[l].vertex[1]].pt[Z] -
			parray[tarray[l].vertex[0]].pt[Y] * parray[tarray[l].vertex[2]].pt[Z] -
			parray[tarray[l].vertex[1]].pt[Y] * parray[tarray[l].vertex[0]].pt[Z];
		b = parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[1]].pt[Z] +
			parray[tarray[l].vertex[1]].pt[X] * parray[tarray[l].vertex[2]].pt[Z] +
			parray[tarray[l].vertex[2]].pt[X] * parray[tarray[l].vertex[0]].pt[Z] -
			parray[tarray[l].vertex[2]].pt[X] * parray[tarray[l].vertex[1]].pt[Z] -
			parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[2]].pt[Z] -
			parray[tarray[l].vertex[1]].pt[X] * parray[tarray[l].vertex[0]].pt[Z];
		c = parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[1]].pt[Y] +
			parray[tarray[l].vertex[1]].pt[X] * parray[tarray[l].vertex[2]].pt[Y] +
			parray[tarray[l].vertex[2]].pt[X] * parray[tarray[l].vertex[0]].pt[Y] -
			parray[tarray[l].vertex[2]].pt[X] * parray[tarray[l].vertex[1]].pt[Y] -
			parray[tarray[l].vertex[0]].pt[X] * parray[tarray[l].vertex[2]].pt[Y] -
			parray[tarray[l].vertex[1]].pt[X] * parray[tarray[l].vertex[0]].pt[Y];

		return(d/c - a/c * pt[X] + b/c * pt[Y]);
	}
	else
	{
		return -100;
	}
}

//横断面内插,输入一条直线(起、终两点pt1,pt2),返回一条线段（由顶点链表表示struct EdgeNode *）及顶点数count
struct EdgeNode * triDTM_road::FindInterPt(double pt1[3], double pt2[3],int &count)
{
	double x,y,z,Vpt1[3],Vpt2[3],Vpt3[3];
	int ajt,ts,te,ajtpre;    
	struct EdgeNode *CrossHead,*eptr, *CrossTail;   

	count = 0;
	ts = LocatetrlNew(pt1);
	te = LocatetrlNew(pt2);

	if(ts >= 0 && tarray[ts].Attribute != 'N')
	{
		CrossHead = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
		CrossHead->pt[0] = pt1[0];
		CrossHead->pt[1] = pt1[1];
		CrossHead->pt[2] = interpt(pt1,ts); 

		CrossHead->next=NULL;
		CrossTail = CrossHead;
		count++;
		ajt = ts;
		ajtpre = ts;
		while(ajt != te && ajt != -1 && tarray[ajt].Attribute != 'N')
		{
			Vpt1[0] = parray[tarray[ajt].vertex[0]].pt[0]; 
			Vpt1[1] = parray[tarray[ajt].vertex[0]].pt[1]; 
			Vpt1[2] = parray[tarray[ajt].vertex[0]].pt[2]; 
			Vpt2[0] = parray[tarray[ajt].vertex[1]].pt[0]; 
			Vpt2[1] = parray[tarray[ajt].vertex[1]].pt[1]; 
			Vpt2[2] = parray[tarray[ajt].vertex[1]].pt[2]; 
			Vpt3[0] = parray[tarray[ajt].vertex[2]].pt[0]; 
			Vpt3[1] = parray[tarray[ajt].vertex[2]].pt[1]; 
			Vpt3[2] = parray[tarray[ajt].vertex[2]].pt[2]; 			  

			if(ajtpre!=tarray[ajt].Ajt[2] && INTERS(&x,&y,Vpt1[0],Vpt1[1],Vpt2[0],Vpt2[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt2[0]-Vpt1[0]) > 0.01)
					z= Vpt1[2]+(x-Vpt1[0])*(Vpt2[2]-Vpt1[2])/(Vpt2[0]-Vpt1[0]);
				else
					z= Vpt1[2]+(y-Vpt1[1])*(Vpt2[2]-Vpt1[2])/(Vpt2[1]-Vpt1[1]);
				if(fabs(x-CrossTail->pt[0])>0.1 || fabs(y-CrossTail->pt[1])>0.1 || fabs(z-CrossTail->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = NULL;
					CrossTail->next = eptr;
					CrossTail = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[2];
			}
			else if(ajtpre!=tarray[ajt].Ajt[0] && INTERS(&x,&y,Vpt2[0],Vpt2[1],Vpt3[0],Vpt3[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt3[0]-Vpt2[0])>0.01)
					z = Vpt2[2]+(x-Vpt2[0])*(Vpt3[2]-Vpt2[2])/(Vpt3[0]-Vpt2[0]);
				else
					z = Vpt2[2]+(y-Vpt2[1])*(Vpt3[2]-Vpt2[2])/(Vpt3[1]-Vpt2[1]);
				if(fabs(x-CrossTail->pt[0])>0.1 || fabs(y-CrossTail->pt[1])>0.1 || fabs(z-CrossTail->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = NULL;
					CrossTail->next = eptr;
					CrossTail = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[0];
			}
			else if(ajtpre!=tarray[ajt].Ajt[1] && INTERS(&x,&y,Vpt3[0],Vpt3[1],Vpt1[0],Vpt1[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt1[0]-Vpt3[0])>0.01)
					z = Vpt3[2]+(x-Vpt3[0])*(Vpt1[2]-Vpt3[2])/(Vpt1[0]-Vpt3[0]); 	 
				else
					z = Vpt3[2]+(y-Vpt3[1])*(Vpt1[2]-Vpt3[2])/(Vpt1[1]-Vpt3[1]); 	 
				if(fabs(x-CrossTail->pt[0])>0.1 || fabs(y-CrossTail->pt[1])>0.1 || fabs(z-CrossTail->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = NULL;
					CrossTail->next = eptr;
					CrossTail = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[1];
			}
			else
			{
				/*		n=INTERS(&x,&y,Vpt3[0],Vpt3[1],Vpt1[0],Vpt1[1],pt1[0],pt1[1],pt2[0],pt2[1]);
				sprintf(mes,"ts=%d %d ajt=%d x=%lf ,pt1:%lf %lf pt2:%lf %lf  %d",ts,te,ajt,x,pt1[0],pt1[1],pt2[0],pt2[1],n);		
				AfxMessageBox(mes);
				AfxMessageBox("内插点有误1");*/
				break;
			}
		}

		x= pt2[0], y = pt2[1];
		if(te >= 0)
			z = interpt(pt2,te);
		else
			z = -100000.0;
		if(fabs(x-CrossTail->pt[0])>0.1 || fabs(y-CrossTail->pt[1])>0.1 || fabs(z-CrossTail->pt[2])>0.1)
		{
			eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
			eptr->pt[0]=x;
			eptr->pt[1]=y;
			eptr->pt[2]=z;
			eptr->next = NULL;
			CrossTail->next = eptr;
			CrossTail = eptr;
			count++;
		}

	}
	else if(te >= 0)
	{
		CrossHead = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
		CrossHead->pt[0] = pt1[0];
		CrossHead->pt[1] = pt1[1];
		CrossHead->pt[2] = -100000.0;     
		CrossHead->next=NULL;
		count++;
		eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
		eptr->pt[0]=pt2[0];
		eptr->pt[1]=pt2[1];
		eptr->pt[2]=interpt(pt2,te);
		eptr->next = CrossHead->next;      
		CrossHead->next = eptr;
		count++;
		ajt = te;
		ajtpre = te;

		while(ajt != -1 && tarray[ajt].Attribute != 'N')
		{
			Vpt1[0] = parray[tarray[ajt].vertex[0]].pt[0]; 
			Vpt1[1] = parray[tarray[ajt].vertex[0]].pt[1]; 
			Vpt1[2] = parray[tarray[ajt].vertex[0]].pt[2]; 
			Vpt2[0] = parray[tarray[ajt].vertex[1]].pt[0]; 
			Vpt2[1] = parray[tarray[ajt].vertex[1]].pt[1]; 
			Vpt2[2] = parray[tarray[ajt].vertex[1]].pt[2]; 
			Vpt3[0] = parray[tarray[ajt].vertex[2]].pt[0]; 
			Vpt3[1] = parray[tarray[ajt].vertex[2]].pt[1]; 
			Vpt3[2] = parray[tarray[ajt].vertex[2]].pt[2]; 

			if(ajtpre!=tarray[ajt].Ajt[2] && INTERS(&x,&y,Vpt1[0],Vpt1[1],Vpt2[0],Vpt2[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt2[0]-Vpt1[0]) > 0.01)
					z= Vpt1[2]+(x-Vpt1[0])*(Vpt2[2]-Vpt1[2])/(Vpt2[0]-Vpt1[0]);
				else
					z= Vpt1[2]+(y-Vpt1[1])*(Vpt2[2]-Vpt1[2])/(Vpt2[1]-Vpt1[1]);
				if(fabs(x-CrossHead->next->pt[0])>0.1 || fabs(y-CrossHead->next->pt[1])>0.1 || fabs(z-CrossHead->next->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = CrossHead->next;
					CrossHead->next = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[2];
			}
			else if(ajtpre!=tarray[ajt].Ajt[0] && INTERS(&x,&y,Vpt2[0],Vpt2[1],Vpt3[0],Vpt3[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt3[0]-Vpt2[0])>0.01)
					z = Vpt2[2]+(x-Vpt2[0])*(Vpt3[2]-Vpt2[2])/(Vpt3[0]-Vpt2[0]);
				else
					z = Vpt2[2]+(y-Vpt2[1])*(Vpt3[2]-Vpt2[2])/(Vpt3[1]-Vpt2[1]);
				if(fabs(x-CrossHead->next->pt[0])>0.1 || fabs(y-CrossHead->next->pt[1])>0.1 || fabs(z-CrossHead->next->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = CrossHead->next;
					CrossHead->next = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[0];
			}
			else if(ajtpre!=tarray[ajt].Ajt[1] && INTERS(&x,&y,Vpt3[0],Vpt3[1],Vpt1[0],Vpt1[1],pt1[0],pt1[1],pt2[0],pt2[1])==0)
			{
				if(fabs(Vpt1[0]-Vpt3[0])>0.01)
					z = Vpt3[2]+(x-Vpt3[0])*(Vpt1[2]-Vpt3[2])/(Vpt1[0]-Vpt3[0]); 	 
				else
					z = Vpt3[2]+(y-Vpt3[1])*(Vpt1[2]-Vpt3[2])/(Vpt1[1]-Vpt3[1]); 	 
				if(fabs(x-CrossHead->next->pt[0])>0.1 || fabs(y-CrossHead->next->pt[1])>0.1 || fabs(z-CrossHead->next->pt[2])>0.1)
				{
					eptr = (struct EdgeNode *)calloc(1,sizeof(struct EdgeNode));
					eptr->pt[0]=x;
					eptr->pt[1]=y;
					eptr->pt[2]=z;
					eptr->next = CrossHead->next;
					CrossHead->next = eptr;
					count++;
				}
				ajtpre = ajt;
				ajt = tarray[ajt].Ajt[1];
			}
			else
				break;
			//		  AfxMessageBox(L"内插点有误2");
		}
	}
	else 
		return NULL;

	return CrossHead;
}  

void triDTM_road::ReleaseDtm()//释放Dtm
{

	if(FilePtrP)
	{

		UnmapViewOfFile(FilePtrP);
		CloseHandle(MEMmapfileHandleP);
		CloseHandle(mapfileHandleP);
		FilePtrP = NULL;
	}
	if(FilePtrT)
	{
		UnmapViewOfFile(FilePtrT);
		CloseHandle(MEMmapfileHandleT);
		CloseHandle(mapfileHandleT);


		CloseHandle(MEMmapfileHandleG);
		CloseHandle(mapfileHandleG);
		FilePtrT = NULL;

	}
	parray=NULL;
	DTMHasRead = 0;
}


//以网格中一点与pt的连线方向快速定位PT所在的三角形 
int triDTM_road::LocatetrlNew(double pt[3])
{
	int i,j,verno;
	int trino,StartVerNo=0,inf,StartTriNo=0,pretno,ajt,vno,ajt1,gridno;
	double cw1,cw2,cw;

	//找出三角形
	i = (int)((pt[0] - LeftLowPt[0])/GridWidth+0.001);
	j = (int)((pt[1] - LeftLowPt[1])/GridWidth+0.001);
	if(i<0 || i>=NRow || j<0 || j>=NCol)
	{
		//		_stprintf(mes,L"%lf %lf",pt[0],pt[1]);
		//		AfxMessageBox(mes);
		return -1;
	}
	if(GridArray[i*NCol+j].Pfcur!=-1)
	{
		trino = parray[GridArray[i*NCol+j].Pfcur].trino;
		StartVerNo = GridArray[i*NCol+j].Pfcur; //取出网格中一点
	}
	else
	{
		gridno = FindFirstNoEmptyGridNo(i,j);
		if(gridno==-1)
			return -1;
		trino = parray[GridArray[gridno].Pfcur].trino;
		StartVerNo = GridArray[gridno].Pfcur; //取出网格中一点      	
	}

	inf = 1;
	do
	{
		if(trino!=-1)
		{
			j=0;
			while(j<3 && tarray[trino].vertex[j]!=StartVerNo)
				j++;
			cw1 = CCW2(parray[StartVerNo].pt,pt,parray[tarray[trino].vertex[(j+1)%3]].pt);
			cw2 = CCW2(parray[StartVerNo].pt,pt,parray[tarray[trino].vertex[(j+2)%3]].pt);
		}

		if(trino==-1 || trino==StartTriNo) //转到一头或一个三角形在一条线上,换一方向继续找
		{
			if(inf==1)
				inf = 0;
			else
				return -1;
			trino = parray[StartVerNo].trino;
			j=0;
			while(j<3 && tarray[trino].vertex[j]!=StartVerNo)
				j++;
		}
		StartTriNo = trino;
		if(inf)
			trino = tarray[trino].Ajt[(j+1)%3];
		else if(inf==0)
			trino = tarray[trino].Ajt[(j+2)%3];
	} while(cw1>0.0 || cw2 <0.0) ;//该过程在以StartVerNo为顶点一圈三角形中，找出StartVerNo-EndVerNo穿越的三角形

	pretno = StartTriNo;
	ajt = pretno;
	vno = j;
	//   CreateAFace(ptdata[tarray[pretno].vertex[0]].pt,ptdata[tarray[pretno].vertex[1]].pt,ptdata[tarray[pretno].vertex[2]].pt,1);	                                    
	if(CheckPtInTri(StartTriNo, pt) == 1)
		return StartTriNo;
	for(;;)
	{
		ajt = tarray[pretno].Ajt[vno];
		if(ajt<0)
			return -1;
		if(CheckPtInTri(ajt, pt) == 1)
			return ajt;

		FindOppTri(j,ajt,pretno);
		verno = tarray[ajt].vertex[j];
		cw = CCW2(parray[StartVerNo].pt,pt,parray[verno].pt);

		if(cw >0.0)
		{
			ajt1 = tarray[ajt].Ajt[(j+2)%3]; 
			FindOppTri(vno,ajt1,ajt);
			vno = (j+1)%3;
		}
		else
		{
			ajt1 = tarray[ajt].Ajt[(j+1)%3]; 
			FindOppTri(vno,ajt1,ajt);
			vno = (j+2)%3;
		}
		pretno = ajt;
	}
}

//判断三角形trino内是否包含点PT 是 返回1 否 返回0 
int triDTM_road::CheckPtInTri(int trino, double pt[2])
{
	double pt0[2],pt1[2],pt2[2];
	double snp01,snp12,snp20;

	pt0[0] = parray[tarray[trino].vertex[0]].pt[0] , pt0[1] = parray[tarray[trino].vertex[0]].pt[1];
	pt1[0] = parray[tarray[trino].vertex[1]].pt[0] , pt1[1] = parray[tarray[trino].vertex[1]].pt[1];
	pt2[0] = parray[tarray[trino].vertex[2]].pt[0] , pt2[1] = parray[tarray[trino].vertex[2]].pt[1];
	//重心
	snp01 = chkd(pt0,pt1,pt);
	snp12 = chkd(pt1,pt2,pt);
	snp20 = chkd(pt2,pt0,pt); 


	if(snp01>=-0.00000001 && snp12>=-0.00000001 && snp20>=-0.00000001)
		return 1;
	else
		return 0;
}

double triDTM_road::chkd(double pt11[3],double pt22[3],double pt33[3])
{
	double c;
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

int triDTM_road::sign(double result)
{
	if(result > 0.0)
		return 1;
	else if(result < 0.0)
		return -1;
	else
		return 0;

}
double triDTM_road::CCW2(double pt1[2],double pt2[2],double pt3[2]) 
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


int triDTM_road::INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4)
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;


	A11 = Y2-Y1;
	A12 = X1-X2;
	A21 = Y4-Y3;
	A22 = X3-X4;
	B1 = X1*Y2 - X2*Y1;
	B2 = X3*Y4 - X4*Y3;
	D = A11*A22-A21*A12;
	if(fabs(D)<1.0E-5)
		INF = -1;
	else
	{
		D1 = B1*A22-B2*A12;
		D2 = B2*A11-B1*A21;

		*x  = D1/D;
		*y  = D2/D;
		X1min=min(X1,X2)-0.00001;
		X1max=max(X1,X2)+0.00001;
		Y1min=min(Y1,Y2)-0.00001;
		Y1max=max(Y1,Y2)+0.00001;
		X2min=min(X3,X4)-0.00001;
		X2max=max(X3,X4)+0.00001;
		Y2min=min(Y3,Y4)-0.00001;
		Y2max=max(Y3,Y4)+0.00001;

		if(X1min<=*x && *x<=X1max && Y1min<=*y && *y<=Y1max
			&& X2min<=*x && *x<=X2max
			&& Y2min<=*y && *y<=Y2max)
			INF = 0;
		else
			INF = 1;
	}

	return INF; 
}    

double triDTM_road::interpt(double pt[3],int l)
{
	double d, a, b,c;

	d = parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[1]].pt[1] *
		parray[tarray[l].vertex[2]].pt[2] + parray[tarray[l].vertex[1]].pt[0] *
		parray[tarray[l].vertex[2]].pt[1] * parray[tarray[l].vertex[0]].pt[2] +
		parray[tarray[l].vertex[2]].pt[0] * parray[tarray[l].vertex[0]].pt[1] *
		parray[tarray[l].vertex[1]].pt[2] - parray[tarray[l].vertex[2]].pt[0] *
		parray[tarray[l].vertex[1]].pt[1] * parray[tarray[l].vertex[0]].pt[2] -
		parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[2]].pt[1] *
		parray[tarray[l].vertex[1]].pt[2] - parray[tarray[l].vertex[1]].pt[0] *
		parray[tarray[l].vertex[0]].pt[1] * parray[tarray[l].vertex[2]].pt[2];
	a = parray[tarray[l].vertex[0]].pt[1] * parray[tarray[l].vertex[1]].pt[2] +
		parray[tarray[l].vertex[1]].pt[1] * parray[tarray[l].vertex[2]].pt[2] +
		parray[tarray[l].vertex[2]].pt[1] * parray[tarray[l].vertex[0]].pt[2] -
		parray[tarray[l].vertex[2]].pt[1] * parray[tarray[l].vertex[1]].pt[2] -
		parray[tarray[l].vertex[0]].pt[1] * parray[tarray[l].vertex[2]].pt[2] -
		parray[tarray[l].vertex[1]].pt[1] * parray[tarray[l].vertex[0]].pt[2];
	b = parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[1]].pt[2] +
		parray[tarray[l].vertex[1]].pt[0] * parray[tarray[l].vertex[2]].pt[2] +
		parray[tarray[l].vertex[2]].pt[0] * parray[tarray[l].vertex[0]].pt[2] -
		parray[tarray[l].vertex[2]].pt[0] * parray[tarray[l].vertex[1]].pt[2] -
		parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[2]].pt[2] -
		parray[tarray[l].vertex[1]].pt[0] * parray[tarray[l].vertex[0]].pt[2];
	c = parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[1]].pt[1] +
		parray[tarray[l].vertex[1]].pt[0] * parray[tarray[l].vertex[2]].pt[1] +
		parray[tarray[l].vertex[2]].pt[0] * parray[tarray[l].vertex[0]].pt[1] -
		parray[tarray[l].vertex[2]].pt[0] * parray[tarray[l].vertex[1]].pt[1] -
		parray[tarray[l].vertex[0]].pt[0] * parray[tarray[l].vertex[2]].pt[1] -
		parray[tarray[l].vertex[1]].pt[0] * parray[tarray[l].vertex[0]].pt[1];

	return(d/c - a/c * pt[0] + b/c * pt[1]);
}
//找到第一个非空网格
int triDTM_road::FindFirstNoEmptyGridNo(int crow,int ccol)
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
