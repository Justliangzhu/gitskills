//#include <afxwin.h>
#include  <stdafx.h>
#include  <stdio.h>
#include <string.h>
#include <math.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include  "adslib.h"        

extern HANDLE mapfileHandleP,MEMmapfileHandleP;
extern LPVOID FilePtrP;    
HANDLE mapfileHandleT,MEMmapfileHandleT;
LPVOID FilePtrT;

int PTSUM,TriSum;
double LeftLowPt[2];

struct Triangle{
	int vertex[3];
	//   char Attribute;//属性：代表三角形是否有效,'Y'有效;'N'无效
} *TriList=NULL; 

struct PT{
	float pt[3];
} *ptdata=NULL;

int Read_Dtm();
void ReleaseDtm();//释放Dtm

int Read_Dtm()
{
	FILE *fpr;
	ACHAR fname[80];
	int itmp,memb,i,j,l,k;
	double dtmp,DX,DY,maxz,max1;

	//	_tcscpy(mes,L"正在导入数模数据文件...");
	_stprintf(fname,L"%s\\3d\\3dmap",Cworkdir);
	fpr = _wfopen(fname,L"r");
	fwscanf(fpr,L"%d%d",&PTSUM,&TriSum);
	fwscanf(fpr,L"%lf%lf",&LeftLowPt[0],&LeftLowPt[1]);
	fclose(fpr);

	_stprintf(fname,L"%s\\3d\\3dmodel_pt",Cworkdir);     
	mapfileHandleP = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);

	_tcscpy(fname,L"MemMapPt");
	MEMmapfileHandleP = CreateFileMapping(mapfileHandleP,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrP = MapViewOfFile(MEMmapfileHandleP,FILE_MAP_WRITE,0,0,0);
	ptdata =  (struct PT *)FilePtrP;

	_stprintf(fname,L"%s\\3d\\3dmodel_tri",Cworkdir);       
	mapfileHandleT = CreateFile(fname,          // pointer to name of the file
		GENERIC_READ|GENERIC_WRITE,       // access (read-write) mode
		FILE_SHARE_WRITE,    // share mode
		NULL,  // pointer to security attributes
		OPEN_EXISTING,  // how to create
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN,  // file attributes
		NULL);

	_tcscpy(fname,L"MemMapTri");
	MEMmapfileHandleT = CreateFileMapping(mapfileHandleT,NULL,PAGE_READWRITE,0,0,fname);
	FilePtrT = MapViewOfFile(MEMmapfileHandleT,FILE_MAP_WRITE,0,0,0);
	TriList =  (struct Triangle *)FilePtrT; 
	return true; 
}

void ReleaseDtm()//释放Dtm
{
	if(ptdata)
	{
		UnmapViewOfFile(FilePtrP);
		CloseHandle(MEMmapfileHandleP);
		CloseHandle(mapfileHandleP);

		UnmapViewOfFile(FilePtrT);
		CloseHandle(MEMmapfileHandleT);
		CloseHandle(mapfileHandleT);
	}
	ptdata=NULL;
}
 	