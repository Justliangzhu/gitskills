#if !defined(AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_)
#define AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FastTin.h : header file
//
#include "vect3d.h"
#include "DrawPM.h"
#include "BAS_DRAW_FUN.h"
#include "XLdataBase.h"
#include "resource.h"
//#include "TextProgressCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CFastTin dialog

class CFastTin : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CFastTin(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	CDrawPM *RoadHole[500];
	int NRoad;
// Dialog Data
	//{{AFX_DATA(CFastTin)
	enum { IDD = IDD_addJD1 };
//	CTextProgressCtrl	m_progress;
	//}}AFX_DATA
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastTin)
public:		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	XLDataBase xlmdb;
	
	struct PtVect
	{
		Vect3d pvec; //顶点法向量,单位化
		float costhita; //顶点法向量与平面法向量角度的余弦
	} *ptvec;
	struct TriVect{
		Vect3d trivec; //三角形法向量,用于计算顶点法向量,用完即删除
	} *trivectmp;
	
    struct Triangle{
	   int vertex[3];
	   int Ajt[3];//*prior,*next;
	   ACHAR EdgeInf[4];//'f' fixed ,'a' active
       ACHAR Attribute;//属性：代表三角形是否有效,'Y'有效;'N'无效
	} *TriListHead, *TriList; 
    int TriListTailNo;
	
    struct PT{
	   double pt[3];
	   int cur;
	   ACHAR att;//属性
	   int trino;
	} *ptdata;

    struct GridNode{
	      int Pfcur;//第一个落在该网格的点
		  int Ptcur;//最后落在该网格的点
		  int Pncur;//当前网格的待插点号
          int PtSum;//当前网格的总点数
	} *GridArray;

   struct GridIndex{
	         int GridNo;
			 int ptsum;			 
	      } *GridIdx;

     struct StackNode{  //在处理地性线中，Vno代表VerList前下标，inf代表VerList后下标
         int trino;	        
	    	int Vno;  
		  int inf;//是否当前三角形为空: 1:是, -1:不是
		  struct StackNode *next;
	 }  *StackTop,*ImpactTriHead;

     struct StackNode *headptr,*tailptr;
     struct ImpactArea{
		int vf,vt;//影响域边界边顶点号
		int Vno;
		int trino;
		struct ImpactArea *next;
	 } *ImpactHead,*ImpactTail;

    struct TEdgeNode{
		int vf,vt;//影响域边界边顶点号
		int Vno;
		int trino;
		struct TEdgeNode *next;
	} *EdgeListHead,*EdgeListTail;

struct VerList//处理地性线影响边界结构
{
	int ptno; //点号
	int	Vno;//边界边邻接三角形对应点号（0，1，2）
	int trino;//边界边邻接三角形号
} *LeftVerList, *RightVerList;

int *ImpactTris; //地性线所影响三角形号
int PtZero[500], PtZeroNum; //地性线通过点个数

struct record {
                struct record *llink;
                char edge[30];
                int trino; //边界边邻接三角形号
                int Vno;//边界边邻接三角形对应点号（0，1，2）
                int bf;//平衡因子
                int mak;//
                struct record *rlink;
              };
struct record *Root;
double LeftLowPt[2],RightUpPt[2];
int PTSUM,RealPTSum;
int NRow,NCol,IdxSum,NCrs;
void FastCreateTin();
HANDLE mapfileHandlePp,mapfileHandleTt,MEMmapfileHandlePp,MEMmapfileHandleTt;
LPVOID FilePtrPp,FilePtrTt;    

void Cal_PtNormal();
void Cal_TinNormal();
void AdjustBriBj();
void FastSort(struct GridIndex R[],int s1,int t1);
int PARTITION(struct GridIndex R[],int l,int h);
int CreateGridIndex();
void AddList(struct Triangle *p);
int CreateFirstTri();
int IncrementInsertPt(int ptnum);//逐点插入过程
int LocateTri(int ptnum);
double chkd(double pt11[3],double pt22[3],double pt33[3]);
int sign(double result);
double HCircle(int v1, int v2, int v3, int vl);
double CCW(int v1,int v2,int v3) ;
void  ExpandProgress(int trino,int vno);
int FindImpactArea(int trino,int ptnum);
void PUSHSTACK(int trino,int vno,int inf);
struct StackNode  *POPSTACK();
int ThrowPtInGrid();
//void DeleteTris();
void DeleteATri(struct Triangle *tri);
//void CreateAFace(double p0[3],double p1[3],double p2[3],Adesk::UInt16 color);
int FormHole();
int FindVerNo(double ver1[2]);
int AjustTIN(double ver1[2],double ver2[2]);
void DealHole(int i,int k);
int FormVerList(int StartTriNo,int vno,int StartVerNo,int EndVerNo,int &lpcur,int &rpcur,int &trcur);
double distance(double *,double *);
double coc(double *,double *,double *);
void ReConstructTin(struct VerList *verlist, int pcur, int &trcur);
int LocateTriOnXY(double pt[2]);
void  DigHole();
void enqueue(int);
struct StackNode *dequeue(void);
void init_queue(void);
struct StackNode *notempty(void);
int  SetEdgeInf(double ver1[2],double ver2[2]);
void detree(struct record *);
struct record *insert(int v1,int v2,int &trino ,int &Vno);
double CCW2(double pt1[2],double pt2[2],double pt3[2]) ;
double interpolate(double pt[2],int l);
int CheckPtInTri(int trino, double pt[2]);
int FindFirstNoEmptyGridNo(int crow,int ccol);
int CheckPtinViewCone(double pt[2]);
int read_FDfile();
int  AllocateFD(int nfend);
void WritePtIn3DFile(FILE *fpr,FILE *fpw,int &NumAll);
int FormZXHole();//主线
void FormInitEgdes();
void DealEgdes();
void DeleteTris();
void SelectPoint();
void DealBriBjx();//对桥梁边界线处理 得到高出地面的桥梁边界
void SetATinAtt(int itri);
void SetTriAtt(int ntri);
//删除废点,重新写SPT文件,生成3dmodel文件
int ThrowOffPt();
void AdjustBjx();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastTin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_)
