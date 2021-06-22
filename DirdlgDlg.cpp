// DirdlgDlg.cpp : implementation file
//

#include "stdafx.h"
#include <windows.h>
//#include <iostream.h>
#include "Shlwapi.h"
#include "DirdlgDlg.h"
#include "XLdatabase.h"
#include "PmPointer.h"
#include "XLDataBase_JYX.h"

#include "Shlwapi.h "
#include <direct.h>

#include "SelectFolderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString Cworkdir,Pro;
/////////////////////////////////////////////////////////////////////////////
// CDirdlgDlg dialog


CDirdlgDlg::CDirdlgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDirdlgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDirdlgDlg)
	m_dirstr = _T("C:\\改移道路工程");
	m_ProNo = _T("方案1");
	m_Grade = _T("四级");
	m_TerrainType = _T("一般地段");
	m_DV = _T("20Km/h");	//20190724	根据公路规范，默认四级	20km/h
	m_DXLB = _T("平原微丘");
	//}}AFX_DATA_INIT
}


void CDirdlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDirdlgDlg)
	//DDX_Control(pDX, IDC_TREE_EXPLORER, m_treeExplorer);
	DDX_Text(pDX, IDC_DIR_EDIT, m_dirstr);
	DDX_Text(pDX, IDC_ProNo, m_ProNo);
	DDX_Text(pDX, IDC_INSERTRAIL, InsertRail);
	DDX_CBString(pDX, IDC_COMBO_DJ, m_Grade);
	DDX_CBString(pDX, IDC_COMBO_DL, m_TerrainType);
	DDX_CBString(pDX, IDC_COMBO_DV, m_DV);
	DDX_CBString(pDX, IDC_COMBO_DXLB, m_DXLB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDirdlgDlg, CDialog)
	//{{AFX_MSG_MAP(CDirdlgDlg)
	//ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_EXPLORER, OnItemexpandingTreeExplorer)
	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_EXPLORER, OnSelchangedTreeExplorer)
	//ON_NOTIFY(NM_RCLICK, IDC_TREE_EXPLORER, OnRclickTreeExplorer)
	ON_BN_CLICKED(IDC_BUTTONBrowser, OnBUTTONBrowser)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONBrowser2, OnBnClickedButtonbrowser2)
	//ON_BN_CLICKED(IDC_BUTTONPICKUP, OnBnClickedButtonpickup)
	ON_BN_CLICKED(IDC_BUTTON_LLGCML, &CDirdlgDlg::OnBnClickedButtonLlgcml)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirdlgDlg message handlers
void CDirdlgDlg::NullInStrto$(CString& text)
{
	int len,i;
	
	len = text.GetLength();
	i = text.Find(L" ");
	
    while(i < len && i >=0)
	{
		text.SetAt(i,'$');
		i = text.Find(L" ");
	}
	if(len==0)
		text="$";	
}

void CDirdlgDlg::InStr$toNull(CString& text)
{
	int len,i;
	
	len = text.GetLength();
	i = text.Find('$');
    while(i < len && i >= 0)
	{
		text.SetAt(i,' ');
		i = text.Find('$');
	}	
}

BOOL CDirdlgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	FILE *fp;
	CString  fname;
	ACHAR str[256];

	_stprintf(str,L"%s\\workdir",SysPath);

	BAS_DRAW_FUN ob;
	if (DirMode==0)
	{
		GetDlgItem(IDC_BUTTON_LLGCML)->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_DIR_EDIT))->SetReadOnly(FALSE);
		GetDlgItem(IDC_BUTTONBrowser)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_LLGCML)->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_DIR_EDIT))->SetReadOnly(TRUE);
		GetDlgItem(IDC_BUTTONBrowser)->EnableWindow(TRUE);
	}

	if(DirMode==1)//选择项目
	{
	   if((fp=_wfopen(str,L"r"))!=NULL)
	   {
		  fwscanf(fp,L"%s",str);
		  m_dirstr = str;

		  fclose(fp);
		  InStr$toNull(m_dirstr);
		  fname = m_dirstr + L"\\PROJECT";
		  if ((fp = _wfopen(fname, L"r")) != NULL)
		  {
			  fwscanf(fp, L"%s", str);
			  m_ProNo = str;
			  InStr$toNull(m_ProNo);
			  fclose(fp);
		  }
		  //else	20191106	这里允许用户直接打开对话框打开方案，现在在ok按钮上添加了检测
		  //{
			 // AfxMessageBox(L"尚未新建改移道路或改移道路.mdf缺失，\n请先新建改移道路");
			 // OnCancel();
			 // return TRUE;
		  //}
	   }

	   Cworkdir = m_dirstr;
	   Pro = m_ProNo;

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
		 // m_dirstr += "\\改移道路";
	     CString MdfRoadNmae = m_ProNo;
		 CString MdfName = Cworkdir + L"\\DATA\\改移道路.mdf";
		 XLDataBase DBS;
	    if(DBS.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae)&&DBS.ModifyRoadSum>0)
	    {
	       //InsertRail=DBS.pModifyRoad[0].RailName; 
			InsertRail = GetRailNameFromPath(DBS.pModifyRoad[0].RailName);
			m_RailMdbPath = DBS.pModifyRoad[0].RailName;
	    }
	    else
	    {
			InsertRail="";
			AfxMessageBox(L"尚未新建改移道路或改移道路.mdf缺失，\n请先新建改移道路");
			OnCancel();
	    }
#endif
	   CString mdbname;
	   mdbname = m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
	   ReadRoadProperty(mdbname);
	   UpdateData(false);
	}
	else
	{
		//20190712	添加默认的文件地址
		if ((fp = _wfopen(str, L"r")) != NULL)
		{
			fwscanf(fp, L"%s", str);
			m_dirstr = str;

			fclose(fp);
			InStr$toNull(m_dirstr);
		}
		UpdateData(false);
		#ifdef GY_Road_SYS
			//GetDlgItem(IDC_BUTTONBrowser)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_DIR_EDIT)->EnableWindow(FALSE);
			//GetDlgItem(IDC_TREE_EXPLORER)->EnableWindow(FALSE);
			if((fp=_wfopen(str,L"r"))!=NULL)
			{
				fwscanf(fp,L"%s",str);
				m_dirstr = str;
				if (m_dirstr.Find(L"改移道路")<0)
				{
					m_dirstr += "\\改移道路";
				}
				fclose(fp);
				InStr$toNull(m_dirstr);
			}
			m_ProNo = "改移道路1";
			ACHAR path[256];
			ACHAR pro[256];
			if (ob.ReadRailWorkdir(path, pro))
				InsertRail.Format(L"%s基线", pro);
			else
				AfxMessageBox(L"尚未设置当前铁路方案!");

			Cworkdir = m_dirstr;
			Pro = m_ProNo;

			UpdateData(false);
		#endif
	}
	m_treeExplorer.PopulateTree();
	m_treeExplorer.EnableImages();	

#ifdef DIGIROAD
	GetDlgItem(IDC_BUTTONBrowser2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INSERTRAIL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_xiangjiaotielu)->ShowWindow(SW_HIDE);
#endif




	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDirdlgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	if(DirMode==1)//选择项目
	{
		if (!IsRightDir())
			return;

		CFileFind Find;
		CString mdbname;
		mdbname =  m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
		if (!Find.FindFile(mdbname))
		{
			CString Temp;
			Temp.Format(L"%s文件不存在，无法打开项目，请核查！",mdbname);
			AfxMessageBox(Temp);
			return;
		}
		SelectDir();
	}
    else
		CreateDir();
	
	CDialog::OnOK();
	//标注工程项目及方案号
	CString str;
	str = L"当前工程项目:" + m_dirstr + L"  [方案号: " + m_ProNo + L"]";
	//_stprintf(mes,L" %s 方案号: %s",,);
    acedGetAcadFrame()->SetWindowText(str);
	//acedGrText(-1,mes,0);
    //写数据库
	CString mdbname;
    mdbname =  m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
	WriteRoadProperty(mdbname);

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))


	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	CString MdfRoadNmae = m_ProNo;
	CString MdfName = Cworkdir + L"\\DATA\\改移道路.mdf";
	XLDataBase DBS;
	//if(DBS.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae)&&DBS.ModifyRoadSum>0)
	//{
	//	InsertRail=DBS.pModifyRoad[0].RailName; 
	//} 
	//else
	//	InsertRail="";
	//CString mdbname;
	//mdbname = m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
	//UpdateData(false);


	//CString MdfRoadNmae = m_ProNo;
	//CString MdfName = Cworkdir + L"\\DATA\\改移道路.mdf";
	//XLDataBase DBS;
	DBS.Read_XLDbs(MdfName,L"改移道路表", MdfRoadNmae);
	if (DBS.ModifyRoadSum<1)
	{
		DBS.ModifyRoadSum = 1;
		if (DBS.pModifyRoad)
		{
			delete DBS.pModifyRoad;
			DBS.pModifyRoad=NULL;
		}
		DBS.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[DBS.ModifyRoadSum];
		DBS.IniMdfRoadData(MdfRoadNmae, DBS.pModifyRoad[0]);
	}
	else if (DBS.ModifyRoadSum>1)
	{
		AfxMessageBox(Pro + L"在改移道路表中有多条记录!");
		return;
	}

	//DBS.pModifyRoad[0].RailName = InsertRail;
	DBS.pModifyRoad[0].RailName = m_RailMdbPath;	//20200522修改	现在改为原路径存储	
    DBS.pModifyRoad[0].RoadName = m_ProNo;
	DBS.pModifyRoad[0].RandType.Format(L"%s/%s", m_Grade, m_TerrainType);
	/*DBS.pModifyRoad[0].CutVol = DBS.QXB[0].;
	DBS.pModifyRoad[0].LandArea = YdAll;
	DBS.pModifyRoad[0].LandAreaStr = LandAreaStr;*/
	DBS.Write_XLDbs(MdfName,L"改移道路表",MdfRoadNmae);
#endif
	
}

//void CDirdlgDlg::OnItemexpandingTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
//	m_treeExplorer.FolderExpanding(pNMHDR, pResult);
//	
//	*pResult = 0;
//}

//void CDirdlgDlg::OnSelchangedTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	// TODO: Add your control notification handler code here
//    m_treeExplorer.GetSelectedFolderPath(m_dirstr);	
//	CWnd * pWnd;
//	pWnd =  GetDlgItem( IDC_DIR_EDIT );
//	pWnd->SetWindowText(m_dirstr);	
//	*pResult = 0;
//}

//void CDirdlgDlg::OnRclickTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	// TODO: Add your control notification handler code here
//	m_treeExplorer.FolderPopup(pNMHDR, pResult);
//	
//	*pResult = 0;
//}

void CDirdlgDlg::SelectDir()
{

	CString pathstr1,pathstr2,pathstr3,pathstr4,pathstr5,pathstr6,pathstr7,pathstr8;
	FILE *fp;
	CString  fname;
	ACHAR str[256];

	if(m_dirstr=="")
		MessageBox(L"请输入工程目录");
	else
	{		
		if(PathIsDirectory( pathstr1 = m_dirstr+"\\DATA"   )
		 &&PathIsDirectory( pathstr2 = m_dirstr+"\\PMAP"   )
		 &&PathIsDirectory( pathstr3 = m_dirstr+"\\3D"    )
		 &&PathIsDirectory( pathstr3 = m_dirstr+"\\RESULT"    ))
		 //&&::PathIsDirectory( pathstr8 = m_dirstr+"\\TMAP"   ) ) 
		{
			//_stprintf(str,L"%s\\workdir",SysPath);
   //         if((fp=_wfopen(str,L"w"))!=NULL)
			//{
			//	fname = m_dirstr;
			//	NullInStrto$(fname);
			//	fwprintf(fp,fname);
			//	fclose(fp);
			//    fname=m_dirstr + L"\\PROJECT";
			//    if((fp = _wfopen(fname ,L"w"))!=NULL)
			//	{
   //               fwprintf(fp , m_ProNo);
		 //         fclose(fp);
			//	}
			////	BAS_DRAW_FUN ob;
			////	ob.ReadGuifanFile();//读取规范标准
			//}
			//else
   //             MessageBox(L"can't open workdir");
			fname=m_dirstr + L"\\PROJECT";
			NullInStrto$(m_ProNo);
			if((fp = _wfopen(fname ,L"w"))!=NULL)
			{
				fwprintf(fp , m_ProNo);
				fclose(fp);
			}
			else
				MessageBox(L"can't open workdir");
		}           
		else
           MessageBox(L"该目录不是工程目录");
	}

	ACHAR wfname[1024];
	_stprintf(wfname,L"%s\\workdir",SysPath);

	if((fp=_wfopen(wfname,L"w"))!=NULL)
	{
		fname = m_dirstr;
		NullInStrto$(fname);
		fwprintf(fp,L"%s",fname);
		fclose(fp);
	}
}

bool CDirdlgDlg::IsRightDir()
{
	CString pathstr1, pathstr2, pathstr3, pathstr4;

	//检查该目录是不是工程目录
	if (m_dirstr == L"")
		//MessageBox(L"请输入工程目录");
		;
	else
	{
		if (PathIsDirectory(pathstr1 = m_dirstr + L"\\DATA")
			&& PathIsDirectory(pathstr2 = m_dirstr + L"\\PMAP")
			&& PathIsDirectory(pathstr3 = m_dirstr + L"\\3D")
			&& PathIsDirectory(pathstr3 = m_dirstr + L"\\RESULT"))
			return TRUE;
	}

	return FALSE;
}

void CDirdlgDlg::CreateDir()
{

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	CString pathstr1,pathstr2,pathstr3,pathstr4,pathstr5,pathstr6,pathstr7,pathstr8;
	FILE *fp;
	CString  fname;
	ACHAR str[256];

	if(m_dirstr==L"")
		MessageBox(L"please input workdir");
	else
	{		         
		pathstr1 = m_dirstr+L"\\DATA";
		pathstr2 = m_dirstr+L"\\PMAP" ;
		pathstr3 = m_dirstr+L"\\3D" ;
		pathstr4 = m_dirstr+L"\\RESULT" ;

		if (!::PathIsDirectory(m_dirstr))
		{
			if (::_wmkdir(  m_dirstr ))
			{
				MessageBox(L"can't create workdir");
				return;
			}
		}
		if (!::PathIsDirectory(pathstr1))
		{
			if (::_wmkdir(  pathstr1 ))
			{
				MessageBox(L"can't create workdir\\DATA");;
				return;
			}
		}
		if (!::PathIsDirectory(pathstr2))
		{
			if (::_wmkdir(  pathstr2 ))
			{
				MessageBox(L"can't create PMAP\\PMAP");
				return;
			}
		}
		if (!::PathIsDirectory(pathstr3))
		{
			if (::_wmkdir(  pathstr3 ))
			{
				MessageBox(L"can't create PMAP\\3D");
				return;
			}
		}
		if (!::PathIsDirectory(pathstr4))
		{
			if (::_wmkdir(  pathstr4 ))
			{
				MessageBox(L"can't create PMAP\\RESULT");
				return;
			}
		}

		CString MdbName = pathstr1 + L"\\" + L"改移道路.mdf";
		CString LibFName;
		LibFName.Format(L"%s\\lib\\改移道路.mdf", SysPath);
		CFileFind Findlg;
		if (Findlg.FindFile(MdbName)==0)
			CopyFile(LibFName,MdbName,false);


		ACHAR wfname[1024];
		_stprintf(wfname,L"%s\\workdir",SysPath);

		if((fp=_wfopen(wfname,L"w"))!=NULL)
		{
			fname = m_dirstr;
			NullInStrto$(fname);
			fwprintf(fp,L"%s",fname);
			fclose(fp);
		}

		fname =m_dirstr+ L"\\PROJECT";
		if((fp = _wfopen(fname ,L"w"))!=NULL)
		{
			fwprintf(fp , m_ProNo);
			fclose(fp);
		}
		else
			MessageBox(L"can't open PROJECT");
	}
#endif

#ifdef DIGIROAD
	CString pathstr1,pathstr2,pathstr3,pathstr4,pathstr5,pathstr6,pathstr7,pathstr8;
	FILE *fp;
	CString  fname;
	ACHAR str[256];

	if(m_dirstr=="")
		MessageBox(L"please input workdir");
	else
	{		         
		pathstr1 = m_dirstr+"\\DATA";
		pathstr2 = m_dirstr+"\\PMAP" ;
		pathstr3 = m_dirstr+"\\3D" ;
		pathstr4 = m_dirstr+"\\RESULT" ;

		if(::_wmkdir(  m_dirstr ) )
		{
			if(errno==17)MessageBox(L"不能创建工程目录,路径已经存在,清另选路径");
			//MessageBox("can't create workdir");
			return;
		}
		else if( ::_wmkdir(  pathstr1 ) ) 
		{
			MessageBox(L"can't create workdir\\data");
			return;
		}
		else if( ::_wmkdir(  pathstr2 ) ) 
		{
			MessageBox(L"can't create workdir\\pmap");
			return;
		}
		else if( ::_wmkdir(  pathstr3 ) ) 
		{
			MessageBox(L"can't create workdir\\3d");
			return;
		}
		else if( ::_wmkdir(  pathstr4 ) ) 
		{
			MessageBox(L"can't create workdir\\result");
			return;
		}	
		else
		{
			_stprintf(str,L"%s\\workdir",SysPath);
			if((fp=_wfopen(str,L"w"))!=NULL)
			{
				fname = m_dirstr;
				NullInStrto$(fname);
				fwprintf(fp,fname);
				fclose(fp);
				fname =m_dirstr+ "\\PROJECT";
				if((fp = _wfopen(fname , L"w"))!=NULL)
				{
					fwprintf(fp , m_ProNo);
					fclose(fp);
				}
			}
			else
				MessageBox(L"can't open workdir");
		}

	}

	//创建一个空的数据库 m_ProNo.mdb
	ACHAR StdMdbName[1024],CurMdbName[1024];//标准mdb和当前mdb文件名

	_stprintf(StdMdbName,L"%s\\lib\\标准.mdb",SysPath);
	_stprintf(CurMdbName,L"%s\\data\\%s.mdb",m_dirstr,m_ProNo);

	BOOL CopySucess = TRUE;
	CopySucess = CopyFile(StdMdbName,CurMdbName,false);//将当前mdb文件名拷贝到标准mdb文件如果标准mdb存在，则覆盖。
	if (!CopySucess)
	{
		CString Mes;
		Mes.Format(L"复制\"%s\"文件到工程目录失败，请确认该文件是否存在！",StdMdbName);
		AfxMessageBox(Mes);
		return;
	}
#endif
    
	
}

void CDirdlgDlg::OnBUTTONBrowser() 
{
	// TODO: Add your control notification handler code here
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	UpdateData(true);
	CString curpm = m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
	CFileDialog fname(true,NULL,curpm);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir = m_dirstr + "\\data\\";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	if (DirMode==0)
	{
		fname.m_ofn.lpstrFilter=L"文件夹\0|*.*";//lbw
	} 
	else
	{
		fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0";
	}

	if(fname.DoModal()==IDOK)
	{
		m_ProNo = fname.GetFileName();
		m_ProNo.MakeUpper();
		m_ProNo.Replace(L".MDB",NULL);

		CString str = fname.GetPathName();
		str.MakeUpper();
		if (DirMode==0)
		{
			CString str2=L".MDB";
			str.Replace(str2,NULL);
		}

		CString str1 = L"\\data\\" + m_ProNo + L".mdb";
		str1.MakeUpper();
		str.Replace(str1,NULL);
		m_dirstr = str;

		CString mdbname;
		mdbname = m_dirstr + L"\\data\\" + m_ProNo + L".mdb";
		ReadRoadProperty(mdbname);

		//向workdir写项目路径
		ACHAR wfname[1024];	
		FILE *fp=NULL;
		CString  tmpname;

		_stprintf(wfname,L"%s\\workdir",SysPath);

		if((fp=_wfopen(wfname,L"w"))!=NULL)
		{
			tmpname = m_dirstr;
			NullInStrto$(tmpname);
			fwprintf(fp,L"%s",tmpname);
			fclose(fp);
		}

		//20191107新增	浏览数据库后需要更新与铁路相交的信息
		CString MdfRoadNmae = m_ProNo;
		CString MdfName = m_dirstr + L"\\DATA\\改移道路.mdf";
		XLDataBase DBS;
		if (DBS.Read_XLDbs(MdfName, L"改移道路表", MdfRoadNmae) && DBS.ModifyRoadSum > 0)
		{
			//InsertRail = DBS.pModifyRoad[0].RailName;
			InsertRail = GetRailNameFromPath(DBS.pModifyRoad[0].RailName);
			m_RailMdbPath = DBS.pModifyRoad[0].RailName;
		}

		UpdateData(false);
	}

#endif

#ifdef DIGIROAD
	UpdateData(true);
	CString curpm ;
	if(DirMode==1)//选择项目
	{
		curpm = m_dirstr + "\\data\\" + m_ProNo + ".mdb";
	}
	else
	{
		curpm =  m_ProNo + ".mdb";
	}


	CFileDialog fname(true,NULL,curpm);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=m_dirstr+"\\data";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES|OFN_EXPLORER;
	if (DirMode==0)
	{
		fname.m_ofn.lpstrFilter=L"文件夹\0|*.*";//lbw
	} 
	else
	{
		fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0";
	}

	if(fname.DoModal()==IDOK)
	{
		m_ProNo = fname.GetFileName();
		m_ProNo.MakeUpper();
		m_ProNo.Replace(L".MDB",NULL);

		CString str = fname.GetPathName();
		str.MakeUpper();

		if (DirMode==0)
		{
			CString str2=L".MDB";
			str.Replace(str2,NULL);
		}

		CString str1 = L"\\data\\" + m_ProNo + ".mdb";
		str1.MakeUpper();
		str.Replace(str1,NULL);
		m_dirstr = str;

		CString mdbname;
		mdbname = m_dirstr + "\\data\\" + m_ProNo + ".mdb";
		if(ReadRoadProperty(mdbname))
		{
			UpdateData(false);
		};
	}
#endif
	
}
//读取道路属性
BOOL CDirdlgDlg::ReadRoadProperty(CString mdbname)
{
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(mdbname,L"线路属性");//读取线路数据库中的数据

	if(xlmdb.RoadXZ.Grade!=L"")
		m_Grade = xlmdb.RoadXZ.Grade;
	if(xlmdb.RoadXZ.TerrainType!=L"")
		m_TerrainType = xlmdb.RoadXZ.TerrainType;
	if(xlmdb.RoadXZ.DV>0)
		m_DV.Format(L"%dKm/h",xlmdb.RoadXZ.DV);
	if(xlmdb.RoadXZ.DXLB!=L"")
		m_DXLB = xlmdb.RoadXZ.DXLB;
	UpdateData(false);
	
	return true;
}
//写出道路属性
BOOL CDirdlgDlg::WriteRoadProperty(CString mdbname)
{
	XLDataBase xlmdb;

    xlmdb.RoadXZ.Grade = m_Grade;
    xlmdb.RoadXZ.TerrainType = m_TerrainType;
    xlmdb.RoadXZ.DV = _wtoi(m_DV);
	xlmdb.RoadXZ.DXLB = m_DXLB;
	xlmdb.Write_XLDbs(mdbname,L"线路属性",L"");//读取线路数据库中的数据
	
	return true;
}

CString CDirdlgDlg::GetRailNameFromPath(CString MdbName)
{
	int myINT = MdbName.ReverseFind('\\');
	if (myINT == -1)
		return MdbName;

	int myINT2 = MdbName.GetLength();
	myINT++;
	myINT2 = myINT2 - myINT - 4;
	CString ProName = MdbName.Mid(myINT, myINT2);
	return ProName;
}

void CDirdlgDlg::OnBnClickedButtonbrowser2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString  mdbPath;
	BAS_DRAW_FUN ob;
	ACHAR path[256];
	ACHAR pro[256];
	//ob.ReadRailWorkdir(path,pro);
	CFileDialog fname(true );
	//_tcscat(path,L"\\data");
	//fname.m_ofn.lpstrInitialDir=path;
	//int col=GetFocusCell().col;
	//int row=GetFocusCell().row;
	fname.m_ofn.lpstrFilter = L"(*.MDB)\0*.mdb\0\0";
	if (fname.DoModal() == IDOK)
	{
		mdbPath = fname.GetPathName();
 		XLDataBase myXLDataBase;
 		XLDataBase_JYX myJYXXLDataBase;
		int ZorY = 0;
		//AfxMessageBox(mdbPath);
// 		if (PmPointer::SJXorJYX(mdbPath,6,false))
// 		{
// 			
			myXLDataBase.IsMdb2(mdbPath,ZorY);
// 		}
// 		else
// 		{
// 			if(PmPointer::SJXorJYX(mdbPath,8,false))
// 			{
// 				//if (PmPointer::SJXorJYX(mdbPath,4,false))
// 				//{
// 				//	ZorY=2;
// 				//}
				myJYXXLDataBase.IsJYXMdb2(mdbPath,ZorY);
// 				
// 			}
// 			else
// 				return;
// 		}

		ZorY=2;
		int myINT=mdbPath.ReverseFind('\\');
		int myINT2=mdbPath.GetLength();
		myINT++;
		myINT2=myINT2-myINT-4;
		InsertRail=mdbPath.Mid(myINT,myINT2);
		if (ZorY==0)
		{
			InsertRail+=L"基线";
		}
		if (ZorY==1)
		{
			if (AfxMessageBox(L"选择基线还是左线?，是选择基线，否选择左线",MB_YESNO) == IDYES)
			{
				InsertRail+=L"基线";
			}
			else
				InsertRail+=L"左线";
		}
		if (ZorY==2)
		{
			if (AfxMessageBox(L"选择基线还是右线?，是选择基线，否选择右线",MB_YESNO) == IDYES)
			{
				InsertRail+=L"基线";
			}
			else
				InsertRail+=L"右线";
			//JCXLM+="右线";
		}

		//20200521新增	处理mdb
		CString RailProName = InsertRail.Mid(0, InsertRail.GetLength() - 2);
		mdbPath.Replace(RailProName, InsertRail);
		m_RailMdbPath = mdbPath;


		//20191107新增	将数据库复制到DATA文件夹下	20200521	不再复制
		//CString RailProName = InsertRail.Mid(0, InsertRail.GetLength() - 2);
		//CString DesPath = m_dirstr + L"\\DATA\\" + RailProName + L".mdb";
		//CopyFile(mdbPath, DesPath, false);

		//SetItemText(row,col,JCXLM);
	}
	UpdateData(false);
}

void CDirdlgDlg::OnBnClickedButtonpickup()
{
	// TODO: 在此添加控件通知处理程序代码
	//acedGetAcadDwgView()->SetFocus();
	//acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//CString DQXLM;
	//ads_name en;
	//AcGePoint3d PT;
	//int rc;
	//AcDbObjectId eId;
	//rc = ads_entsel(L"\n请选择实体: ", en, asDblArray(PT));

	//if(rc == RTCAN)
	//{
	//	acDocManager->unlockDocument(acDocManager->curDocument());
	//	GetParent()->ShowWindow(TRUE); // Enable our dialog*/	
	//	SetFocus();
	//	UpdateData(FALSE);
	//	return;
	//}				
	//if (rc == RTNORM)
	//{
	//	acdbGetObjectId(eId, en);    
	//	AcDbObject *pObj = NULL;
	//	if(acdbOpenObject(pObj, eId, AcDb::kForRead)==Acad::eOk)
	//	{	
	//		pObj->close();
	//		if(pObj->isKindOf(GTZX::desc()))
	//		{
	//			CString myStr;
	//			GTZX *myGTZX = GTZX::cast(pObj);
	//			myStr=myGTZX->mdbname;
	//			if (myStr.IsEmpty())
	//			{
	//				AfxMessageBox(L"方案名为空，请保存方案！");
	//				return;
	//			}
	//			int myINT=myStr.ReverseFind('\\');
	//			int myINT2=myStr.GetLength()-4;
	//			myINT++;
	//			myINT2=myINT2-myINT;
	//			DQXLM=myStr.Mid(myINT,myINT2);
	//			DQXLM+="基线";
	//		}
	//		else if (pObj->isKindOf(GTZX2::desc()))
	//		{
	//			CString myStr;
	//			GTZX2 *myGTZX = GTZX2::cast(pObj);
	//			myStr=myGTZX->mdbname;
	//			if (myStr.IsEmpty())
	//			{
	//				AfxMessageBox(L"方案名为空，请保存方案！");
	//				return;
	//			}
	//			int myINT=myStr.ReverseFind('\\');
	//			int myINT2=myStr.GetLength()-4;
	//			myINT++;
	//			myINT2=myINT2-myINT;
	//			DQXLM=myStr.Mid(myINT,myINT2);
	//			if (myGTZX->LEFT_OR_RIGHT==-1)
	//			{
	//				if (AfxMessageBox(L"选择基线或左线，是为基线，否为左线",MB_YESNO) == IDYES)
	//				{
	//					DQXLM+="基线";
	//				}
	//				else
	//				{
	//					DQXLM+="左线";
	//				}

	//			}
	//			else
	//			{
	//				if (AfxMessageBox(L"选择基线或左线，是为基线，否为右线",MB_YESNO) == IDYES)
	//				{
	//					DQXLM+="基线";
	//				}
	//				else
	//				{    
	//					DQXLM+="右线";
	//				}
	//			}
	//		}
	//		else
	//		{
	//			acDocManager->unlockDocument(acDocManager->curDocument());
	//			GetParent()->ShowWindow(TRUE); // Enable our dialog*/	
	//			SetFocus();
	//			UpdateData(FALSE);
	//			return;
	//		}

	//		acDocManager->unlockDocument(acDocManager->curDocument());
	//		GetParent()->ShowWindow(TRUE); // Enable our dialog*/	
	//		SetFocus();
	//		UpdateData(FALSE);

	//		int col=GetFocusCell().col;
	//		int row=GetFocusCell().row;
	//		SetItemText(row,col,DQXLM);
	//		Invalidate();
	//		return;

	//	}
	//}

	//acDocManager->unlockDocument(acDocManager->curDocument());
	////GetParent()->ShowWindow(TRUE); // Enable our dialog*/	
	//SetFocus();
	//UpdateData(FALSE);
	//return;
}




void CDirdlgDlg::OnBnClickedButtonLlgcml()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	m_dirstr = CSelectFolderDlg::Show()/* + "\\自定义项目名"*/;//得到所选文件夹目录
	UpdateData(FALSE);
}
