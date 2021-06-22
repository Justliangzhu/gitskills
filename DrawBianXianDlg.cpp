// DrawBianXianDlg.cpp : implementation file
// 绘制线路偏置边线对话框 2005.07.12 ldf
//
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "resource.h"
#include "DrawBianXianDlg.h"
#include "XLDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// 绘制线路偏置边线对话框

CDrawBianXianDlg::CDrawBianXianDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawBianXianDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawBianXianDlg)
	m_strStyle = _T("全线");
	m_strMethod = _T("等宽");
	m_dScml = 0.0;
	m_dEcml = 0.0;
	m_strSWidth = _T("11 -11 10.5 -10.5");
	m_strEWidth = _T("11 -11 10.5 -10.5");
	m_dStep = 2.0;
	m_pOffsetRoad=NULL;
	//}}AFX_DATA_INIT
}

void CDrawBianXianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawBianXianDlg)
	DDX_CBString(pDX, IDC_COMBO_STYLE, m_strStyle);
	DDX_CBString(pDX, IDC_COMBO_METHOD, m_strMethod);
	DDX_Text(pDX, IDC_EDIT_Sml, m_dScml);
	DDX_Text(pDX, IDC_EDIT_Eml, m_dEcml);
	DDX_CBString(pDX, IDC_COMBO_SW, m_strSWidth);
	DDX_CBString(pDX, IDC_COMBO_EW, m_strEWidth);
	DDX_Text(pDX, IDC_EDIT_STEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 0., 10000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrawBianXianDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawBianXianDlg)
	ON_BN_CLICKED(IDC_BUTTON_GET_ROAD, OnButtonGetRoad)
	ON_BN_CLICKED(IDC_BUTTON_GET_SML, OnButtonGetSml)
	ON_BN_CLICKED(IDC_BUTTON_GET_EML, OnButtonGetEml)
	ON_CBN_SELENDOK(IDC_COMBO_METHOD, OnSelendokComboMethod)
	ON_CBN_SELENDOK(IDC_COMBO_SW, OnSelendokComboSw)
	ON_CBN_SELENDOK(IDC_COMBO_STYLE, OnSelendokComboStyle)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CDrawBianXianDlg message handlers

BOOL CDrawBianXianDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	// 初始为等宽度偏置
	m_strEWidth = m_strSWidth; 
	GetDlgItem(IDC_COMBO_EW)->EnableWindow(FALSE);

	m_pOffsetRoad  = NULL;
	m_OffsetRoadID = NULL;
	m_dBaseProMile = 0.0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// 选择偏置线路或线元
void CDrawBianXianDlg::OnButtonGetRoad() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}

	ads_name   EntityName; // 所选实体名称
	ads_point  BasePoint;  // 所选实体基点

	// 提示用户选择主线
	if(ads_entsel(L"\n请选取偏线实体:\n",EntityName,BasePoint) != RTNORM) 
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"偏线选取失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		DestroyWindow();
		return ;
	}
	if(acdbGetObjectId(m_OffsetRoadID,EntityName) != Acad::eOk)
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"获取偏线实体ID失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}

	if(!GetOffsetRoad()) // 获得偏线实体指针
	{
		return;
	}

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	// 按偏线类型确定初始桩号范围
	m_dBaseProMile = m_pOffsetRoad->PROJ_ML(BasePoint[Y], BasePoint[X]); // 获取基点对应的统一里程

	if(m_strStyle == "全线")
	{
		int iXYNum = m_pOffsetRoad->XYNum;
		m_dScml = m_pOffsetRoad->XYArray[0][4];
		m_dEcml = m_pOffsetRoad->XYArray[iXYNum][6];
	}
	else // 单个线元
	{
		int ItemIndex = m_pOffsetRoad->GetXYNoFromTYLC(m_dBaseProMile);      // 基点对应线路的线元号
		m_dScml = m_pOffsetRoad->XYArray[ItemIndex][6];
		m_dEcml = m_pOffsetRoad->XYArray[ItemIndex+1][6];
	}

	UpdateData(FALSE);
}

// 实时获得偏线实体指针
bool CDrawBianXianDlg::GetOffsetRoad()
{
	// 打开所获取的实体, 根据所得基点获取桩号值
	double cml;
	CString mdbname,roadname;
	XLDataBase DBS;

	AcDbEntity * pEntity = NULL;
	if(acdbOpenObject(pEntity,m_OffsetRoadID,AcDb::kForRead) != Acad::eOk)
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"实体打开失败,释放实体ID!\n");	
		return false;
	}
	pEntity->close();
	// 判断所选实体是否为线路实体
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		DBS.GetXLXdata(m_OffsetRoadID,mdbname,roadname);
		if(mdbname!=""&&roadname!="")
		{
			//			DBS.Read_XLDbs(mdbname,"控制点表",roadname);
			DBS.Read_XLDbs(mdbname,L"线元表",roadname);				   
			DBS.Read_XLDbs(mdbname,L"断链表",roadname);
			if(DBS.XYNum>0)
			{
				m_pOffsetRoad=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//				m_pOffsetRoad->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					m_pOffsetRoad->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(m_pOffsetRoad->mdbname,mdbname);
				_tcscpy(m_pOffsetRoad->RoadName,roadname); 
				return true;
			}
		}
		else
		{
			m_OffsetRoadID = NULL;
			acutPrintf(L"所选实体不是线路实体,释放实体ID!\n");
			return false;
		}			
	}	
	m_pOffsetRoad  = JD_CENTER::cast(pEntity); // 获取主线实体指针

	return true;
}

// 获得偏置起始桩号
void CDrawBianXianDlg::OnButtonGetSml() 
{
	// TODO: Add your control notification handler code here
	GetCml(m_dScml); // 动态拾取桩号
	UpdateData(FALSE);
}

// 获得偏置终止桩号
void CDrawBianXianDlg::OnButtonGetEml() 
{
	// TODO: Add your control notification handler code here
	GetCml(m_dEcml); // 动态拾取桩号
	UpdateData(FALSE);
}

// 绘制线路偏置边线
void CDrawBianXianDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	// 获得宽度数据
	if(!GetOffSetWidth(m_strSWidth, m_SWidth))
	{
		return;
	}
	if(m_strMethod == "等宽")
	{
		m_strEWidth = m_strSWidth;	
	}
	if(!GetOffSetWidth(m_strEWidth, m_EWidth))
	{
		return;
	}

	if(m_SWidth.GetSize() != m_EWidth.GetSize()) 
	{
		AfxMessageBox(L"起始与终止宽度序列必须一一对应!");
		return;
	}

	if(!GetOffsetRoad()) // 实时获得偏线
	{
		return;
	}

	// 获得变宽方式
	int bkmode = 0; // 变宽模式,初始为0: 0--等宽、线性变宽 1--抛物线变宽
	if(m_strMethod == "抛物线变宽")
	{
		bkmode = 1;
	}

	// 绘制线路偏置边线
	for(int i=0; i<m_SWidth.GetSize(); i++)
	{
		m_pOffsetRoad->DrawKD(m_dScml,m_SWidth[i],m_dEcml,m_EWidth[i],bkmode,7,m_dStep);
	}

	CDialog::OnOK();
}

// 获得偏置宽度
bool CDrawBianXianDlg::GetOffSetWidth(CString strWidth, CArray<double,double>& arrays)
{
	arrays.RemoveAll();

	if(strWidth.GetLength() < 1)
	{
		AfxMessageBox(L"请正确输入边线偏置宽度!");
		return false;
	}

	int nStart = 0, nEnd, nCount;
	CString strTmp;
	double  dTmp;
	nEnd   = strWidth.Find(L" ", nStart);
	while(nEnd != -1)
	{
		nCount = nEnd - nStart;
		if(nCount)
		{
			strTmp = strWidth.Mid(nStart, nCount);
			dTmp = _wtof(strTmp);
			arrays.Add(dTmp);
		}
		nStart = nEnd + 1;
		nEnd   = strWidth.Find(L" ", nStart);
	}

	nCount = strWidth.GetLength() - nStart; // 最后一个
	if(nCount)
	{
		strTmp = strWidth.Mid(nStart, nCount);
		dTmp = _wtof(strTmp);
		arrays.Add(dTmp);
	}

	return true;
}

// 动态拾取桩号
void CDrawBianXianDlg::GetCml(double& dCml)
{
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}

	GetOffsetRoad(); // 实时获得主线单元指针

	int MouseType;             // 鼠标控制类型
	struct resbuf ClickResult; // 结果缓冲区变量
	short KeyCode;             // 定义关键码变量

	AcGePoint3d PickDot;
	for( ; ; ) // 动态拾取桩号循环
	{	
		// 设置鼠标拖动控制并进行跟踪;
		if(ads_grread(1,&MouseType,&ClickResult) != RTNORM) 			
		{
			break;
		}
		KeyCode = ClickResult.resval.rint;
		if(KeyCode==13 || KeyCode==32 || MouseType==25) 
		{
			break;
		}
		if(fabs(MouseType - 3.0) < DataPrecision) 
		{
			break; // 点击左键退出拾取循环
		}
		else if(MouseType == 5) // 跟踪鼠标移动
		{
			PickDot.x = ClickResult.resval.rpoint[X];
			PickDot.y = ClickResult.resval.rpoint[Y];
			PickDot.z = ClickResult.resval.rpoint[Z];	

			dCml = m_pOffsetRoad->PROJ_ML(PickDot.y,PickDot.x); // 基点对应线路的统一里程	
			UpdateData(FALSE);
		}
		else
		{
			break; // 如果别的鼠标键被点击,退出动态设计循环
		}
	}	

	acDocManager->unlockDocument(acDocManager->curDocument());
}

// 选择偏置方法
void CDrawBianXianDlg::OnSelendokComboMethod() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_strMethod == "等宽")
	{
		m_strEWidth = m_strSWidth; // 起终点宽度一致
		GetDlgItem(IDC_COMBO_EW)->EnableWindow(FALSE);	
	}
	else
	{
		GetDlgItem(IDC_COMBO_EW)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// 选择边线起始宽度序列
void CDrawBianXianDlg::OnSelendokComboSw() 
{
	// TODO: Add your control notification handler code here

	//	GetDlgItemText(IDC_COMBO_SW, m_strSWidth);

	//	GetDlgItem(IDC_COMBO_SW)->UpdateData(TRUE);
	//
	//	if(m_strMethod == "等宽")
	//	{
	//		m_strEWidth = m_strSWidth; // 起终点宽度一致
	//		GetDlgItem(IDC_COMBO_EW)->SetWindowText(m_strEWidth);
	//	}
	//	
	//	UpdateData(FALSE);
}

// 输入边线起始宽度序列
//void CDrawBianXianDlg::OnEditchangeComboSw() 
//{
// TODO: Add your control notification handler code here
//	UpdateData(TRUE);

//	if(m_strMethod == "等宽")
//	{
//		GetDlgItemText(IDC_COMBO_SW,m_strSWidth);
//		m_strEWidth = m_strSWidth; // 起终点宽度一致
//		GetDlgItem(IDC_COMBO_EW)->SetWindowText(m_strEWidth);
//	}


//	UpdateData(FALSE);
//}

// 选择偏线类型
void CDrawBianXianDlg::OnSelendokComboStyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// 如果选择了偏线,则按偏线类型初始桩号范围
	if(m_pOffsetRoad)
	{
		if(m_strStyle == "全线")
		{
			int iXYNum = m_pOffsetRoad->XYNum;
			m_dScml = m_pOffsetRoad->XYArray[0][4];
			m_dEcml = m_pOffsetRoad->XYArray[iXYNum][6];
		}
		else // 单个线元
		{
			int ItemIndex = m_pOffsetRoad->GetXYNoFromTYLC(m_dBaseProMile); // 基点对应线路的线元号
			m_dScml = m_pOffsetRoad->XYArray[ItemIndex][6];
			m_dEcml = m_pOffsetRoad->XYArray[ItemIndex+1][6];
		}
	}

	UpdateData(FALSE);
}

// 关闭对话框时释放空间
void CDrawBianXianDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pOffsetRoad)
	{
		delete m_pOffsetRoad;
		m_pOffsetRoad = NULL;
	}
	if(m_OffsetRoadID)
	{
		m_OffsetRoadID = NULL;
	}

	CDialog::OnClose();
}
