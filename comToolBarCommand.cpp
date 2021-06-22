
#include "stdafx.h"
//#include "stdarx.h"
//#include "rxdlinkr.h"
//#include "acedads.h"
//#include "adslib.h"
#include "acadi.h"
#include "rxmfcapi.h"
#include "CheckToolBar.h"
#include "acadi_i.c"


CCheckToolBar * dlg=NULL;
///////// ///////////////////////通过在ARX环境下使用COM接口访问AutoCAD////////
bool findtoolbar(CString tbar);
IAcadToolbarItem* CreateTbButton(BSTR tooltip,BSTR Command,CString TbName,IAcadToolbar *tlbr,VARIANT index,VARIANT vtFalse);//
void Tbpath(CString Tb, CString *path);
bool getApplication(LPDISPATCH * pVal)
{
	LPDISPATCH pDispatch = acedGetAcadWinApp()->GetIDispatch(TRUE);
	if (pDispatch == NULL)
		return false;
	*pVal = pDispatch;
	return true;
}

bool getAcadMenuGroup(IAcadMenuGroup  **pVal)
{
	IAcadApplication *acadApp = NULL;
	LPDISPATCH  pDisp = NULL;

	if (!getApplication(&pDisp))
	{
		return false;
	}

	HRESULT hr = S_OK;
	hr = pDisp->QueryInterface(IID_IAcadApplication, (LPVOID*)&acadApp);
	///访问自动控件接口的COM捷径 ，，返回值是S_OK //获取AutoCad应用程序对象
	if (FAILED(hr))
		return false;

	LPDISPATCH  pTempDisp = NULL;
	IAcadMenuGroups *mnuGrps = NULL; //菜单组
	long cnt = 0;

	//get the menu groups
	hr = acadApp->get_MenuGroups(&mnuGrps);//通过AcadCAD应用程序，获得菜单集合
	if (FAILED(hr))
	{
		acadApp->Release();
		return false;
	}
	mnuGrps->get_Count(&cnt);


	//get AutoCAD menu group. say it is index 0.
	IAcadMenuGroup *mnuGrp = NULL;//菜单

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  grpName;
	bool found = false ;
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = mnuGrps->Item(vtName, &mnuGrp);
		if (FAILED(hr))
			return false;

		hr  = mnuGrp->get_Name(&grpName);
		CString cgrpName(grpName);
		if (cgrpName.CompareNoCase(L"Acad")==0) 
		{
			found = true;
			*pVal = mnuGrp;
			break;
		}
	}

	acadApp->Release();

	return found;
}
bool findtoolbar(CString tbar)
{
	IAcadMenuGroup *mnuGrp = NULL;
	if (!getAcadMenuGroup(&mnuGrp))
		return false;


	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;

	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();

	long cnt = 0;
	hr = tlbrs->get_Count(&cnt);

	IAcadToolbar *polyTlbr = NULL;
	BSTR  tempName;
	//int  pos;
	VARIANT vtName;
	for ( long i=0; i < cnt; i++)
	{
		vtName.vt = VT_I4;
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &polyTlbr);
		hr = polyTlbr->get_Name(&tempName);
		CString tlbrName(tempName);
		SysFreeString(tempName);
		if(tlbrName.CompareNoCase(tbar)==0)
		{

			short int v;
			if(polyTlbr->get_Visible(&v)==S_OK) //发现工具条
			{
				if(v==-1) //工具条出现
				{
					polyTlbr->Release();
					return true;
				}
				if(v==0)  //工具条不出现
				{
					polyTlbr->Release();
					return false;
				}
			}
		}
		else
			polyTlbr->Release();

	}

	tlbrs->Release();
	return false;
}
void LoadToolbar(CString t_Name)
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;


	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(t_Name)==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{

				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//tlbr->Float(250,50,50);
		}
	}
}
void CleanUpToolbars(CString name)
{
	IAcadMenuGroup *mnuGrp = NULL;
	if (!getAcadMenuGroup(&mnuGrp))
		return ;


	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;

	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();

	long cnt = 0;
	hr = tlbrs->get_Count(&cnt);

	IAcadToolbar *polyTlbr = NULL;
	BSTR  tempName;

	VARIANT vtName;
	for ( long i=0; i < cnt; i++)
	{
		vtName.vt = VT_I4;
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &polyTlbr);
		hr = polyTlbr->get_Name(&tempName);
		CString tlbrName(tempName);
		SysFreeString(tempName);
		if(tlbrName.CompareNoCase(name)==0)
		{


			polyTlbr->put_Visible(0);//可见为-1，不可见为0

		}
		else
			polyTlbr->Release();

	}

	tlbrs->Release();
	return ;
}
void  darcydarcyxmglout()
{
	CleanUpToolbars("项目管理");

}
void  darcydarcyjlsmout()
{
	CleanUpToolbars("建立数模");

}
void DarcySLDESIGNDXSJOUT()
{
	CleanUpToolbars("基线设计");
}
void DarcyFAGLOUT()
{
	CleanUpToolbars("方案管理");
}
void DarcyDLDESIGNDLSJOUT()
{
	CleanUpToolbars("二线设计");
}
void DarcyYTHDLSZOUT()
{   
	CleanUpToolbars("断链设置");
}
void DarcyPDDESIGNPDSJOUT()
{
	CleanUpToolbars("坡度设计");
}
void DarcyBRIDGEDESIGNQLSJOUT()
{
	CleanUpToolbars("桥梁设置");
}
void DarcySDDESIGNSDDESIGNOUT()
{
	CleanUpToolbars("隧道设置");
}
void DarcyPLANPMTOUT()
{
	CleanUpToolbars("平面图");
}
void DarcyZDMDESZDMTOUT()
{
	CleanUpToolbars("纵断面图");
}

void DarcyDLDESIGNPMYTHOUT()
{
	CleanUpToolbars("平面一体化");
}
void DarcyDLDESIGNZMYTHSJOUT()
{
	CleanUpToolbars("纵断面一体化");
}
void DarcyHDMDESOUT()
{
	CleanUpToolbars("横断面设计");
}
void DarcyYDOUT()
{
	CleanUpToolbars("用地设计");
}
void  darcydarcyxmgl()
{
	//	CCheckToolBar Checktbar;
	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int  rows,left,top;
	tlbrs->get_Count(&cnt);
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"项目管理")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}
			tlbr->Release();
			tlbrs->Release();
			return;
		}
	}


	hr = tlbrs->Add(L"项目管理", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();


	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;
	// tlbr->a();
	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(50,300,50);
	CreateTbButton(L"新建项目",L"_CreatePrj ","新建项目",tlbr,index,vtFalse);
	CreateTbButton(L"选择项目",L"_OpenPrj ","选择项目",tlbr,index,vtFalse);
	CreateTbButton(L"数据录入与查询",L"_InMdb ","数据录入与查询",tlbr,index,vtFalse);

	tlbr->Release();
	return;

}
void  darcydarcyjlsm()
{
	//	CCheckToolBar Checktbar;
	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int  rows,left,top;
	tlbrs->get_Count(&cnt);
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"建立数模")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}
			tlbr->Release();
			tlbrs->Release();
			return;
		}
	}


	hr = tlbrs->Add(L"建立数模", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();


	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;
	// tlbr->a();
	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(100,300,50);
	CreateTbButton(L"提取数据",L"_pickdata ",L"提取数据",tlbr,index,vtFalse);
	CreateTbButton(L"处理数据",L"_dealdata ",L"处理数据",tlbr,index,vtFalse);
	CreateTbButton(L"建立数模",L"_createtin ",L"建立数模",tlbr,index,vtFalse);
	CreateTbButton(L"显示数模",L"_displaydtm ",L"显示数模",tlbr,index,vtFalse);

	tlbr->Release();
	return;

}


void darcysldesignsldesign()    //基线设计
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);   
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"基线设计")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//	tlbr->Float(250,300,50);
		}
	}
	hr = tlbrs->Add(L"基线设计", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(150,300,50);
	CreateTbButton(L"基本参数",L"_SET_PM_JBSJ ","基本参数",tlbr,index,vtFalse);
	CreateTbButton(L"线形设计",L"_XXSJ ","平面定线",tlbr,index,vtFalse);
	//	CreateTbButton(L"文件方式导入",L"_ipm ","文件方式导入",tlbr,index,vtFalse);
	CreateTbButton(L"数据库方式导入",L"_ipmmdb ","数据库方式导入",tlbr,index,vtFalse);
	CreateTbButton(L"转动态设计线",L"_SCLOC ","转动态设计线",tlbr,index,vtFalse);
	CreateTbButton(L"列表编辑数据",L"_JDB ","列表编辑数据",tlbr,index,vtFalse);
	CreateTbButton(L"逐个交点编辑",L"_editJD ","逐个交点编辑",tlbr,index,vtFalse);
	CreateTbButton(L"修改曲线",L"_EJD ","修改曲线",tlbr,index,vtFalse);
	CreateTbButton(L"插入交点",L"_Raddjd ","插入交点",tlbr,index,vtFalse);
	CreateTbButton(L"删除交点",L"_Rdeljd ","删除交点",tlbr,index,vtFalse);
	CreateTbButton(L"连接设计线",L"_linkpm ","连接设计线",tlbr,index,vtFalse);
	CreateTbButton(L"违规检测",L"_checkpm ","违规检测",tlbr,index,vtFalse);
	CreateTbButton(L"保存方案",L"_InputPmb ","保存方案",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcysldesignfagl()    //方案管理
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);   
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"方案管理")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//	tlbr->Float(250,300,50);
		}
	}
	hr = tlbrs->Add(L"方案管理", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(200,300,50);
	CreateTbButton(L"创建关联",L"_SETREL ","创建关联",tlbr,index,vtFalse);
	CreateTbButton(L"方案组合",L"_ZHFA ","方案组合",tlbr,index,vtFalse);
	CreateTbButton(L"组合线转设计线",L"_ZSJXST ","组合线转设计线",tlbr,index,vtFalse);
	CreateTbButton(L"导入组合方案",L"_SYZST ","导入组合方案",tlbr,index,vtFalse);
	CreateTbButton(L"经济指标",L"_JJZB ","经济指标",tlbr,index,vtFalse);
	CreateTbButton(L"方案比选",L"_FABX ","方案比选",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcydldesigndldesign()  //二线设计
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"二线设计")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
		}
	}

	hr = tlbrs->Add(L"二线设计", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(250,300,50);

	//CreateTbButton(L"初始化",L"_EXSJ ","初始化",tlbr,index,vtFalse);
	CreateTbButton(L"自动配置二线",L"_EXSJ ","自动配置二线",tlbr,index,vtFalse);
	CreateTbButton(L"导入二线方案",L"_IN2XPM ","导入二线方案",tlbr,index,vtFalse);
	CreateTbButton(L"修改线间距",L"_MDYXJJ ","修改线间距",tlbr,index,vtFalse);
	CreateTbButton(L"编辑二线数据",L"_EXSJ ","编辑二线数据",tlbr,index,vtFalse);
	CreateTbButton(L"增加S弯",L"_ADDSMODEL ","增加S弯",tlbr,index,vtFalse);
	CreateTbButton(L"删除S弯",L"_DELSMODEL ","删除S弯",tlbr,index,vtFalse);
	CreateTbButton(L"删除绕行段",L"_DELRXD ","删除绕行段",tlbr,index,vtFalse);
	CreateTbButton(L"保存方案",L"_InputPmb ","保存方案",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcydldes() //断链设置
{
	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int rows,left,top;
	tlbrs->get_Count(&cnt);
	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"断链设置")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
		}
	}

	hr = tlbrs->Add(L"断链设置", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;
	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(200,300,50);

	CreateTbButton(L"编辑断链表",L"_editdlb ","编辑断链表",tlbr,index,vtFalse);
	CreateTbButton(L"插入断链",L"_adddl ","插入断链",tlbr,index,vtFalse);
	CreateTbButton(L"删除断链",L"_deldl ","删除断链",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyythyth()//平面一体化
{
	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int rows,left,top;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"平面一体化")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{	
					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 
					tlbr->Dock(dockStatus);
				}
			}
			tlbr->Release();
			tlbrs->Release();
			return;
		}
	}

	hr = tlbrs->Add(L"平面一体化", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(150,300,50);

	CreateTbButton(L"平面一体化",L"_",L"平面一体化工具条",tlbr,index,vtFalse);
	CreateTbButton(L"控制断面",L"_viewtypehdm ",L"控制断面",tlbr,index,vtFalse);
	CreateTbButton(L"断面提取",L"_cxhdm ",L"断面提取",tlbr,index,vtFalse);
	CreateTbButton(L"土方查询",L"_cxtsf ",L"土方查询",tlbr,index,vtFalse);
	CreateTbButton(L"三维浏览",L"_3d_browser ",L"三维浏览",tlbr,index,vtFalse);
	CreateTbButton(L"保存方案",L"_InputPmb ",L"保存二线方案",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcypddesignpddesign()//拉坡设计
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;

	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"坡度设计")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;

		}
	}

	hr = tlbrs->Add(L"坡度设计", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(100,300,50);

	CreateTbButton(L"基本参数",L"_InitZdm ",L"基本参数",tlbr,index,vtFalse);
	CreateTbButton(L"选择平面方案",L"_ZM_ZDPMFA ",L"选择平面方案",tlbr,index,vtFalse);
	CreateTbButton(L"导入基线方案",L"_InputZMFA ",L"导入基线方案",tlbr,index,vtFalse);
	CreateTbButton(L"导入左线绕行线方案",L"_INZXRXDZDM ",L"导入左线绕行线方案",tlbr,index,vtFalse);
	CreateTbButton(L"导入右线绕行线方案",L"_INYXRXDZDM ",L"导入右线绕行线方案",tlbr,index,vtFalse);

	CreateTbButton(L"采集地面数据",L"_PickZdmDmx ",L"采集地面数据",tlbr,index,vtFalse);
	CreateTbButton(L"点绘地面线",L"_dmx ",L"点绘地面线",tlbr,index,vtFalse);
	CreateTbButton(L"地面数据编辑",L"_EDITDMD ",L"地面数据编辑",tlbr,index,vtFalse);

	//CreateTbButton(L"选择纵坡方案",L"_PIZDM ","选择纵坡方案",tlbr,index,vtFalse);
	CreateTbButton(L"直线转坡度线",L"_IDZDM ",L"直线转坡度线",tlbr,index,vtFalse);
	CreateTbButton(L"拉坡设计",L"_CZMLPSJ ",L"拉坡设计",tlbr,index,vtFalse);
	CreateTbButton(L"调整坡度",L"_MDYPD ",L"调整坡度",tlbr,index,vtFalse);
	CreateTbButton(L"编辑坡度",L"_ZPB ",L"数据编辑",tlbr,index,vtFalse);
	CreateTbButton(L"增加变坡点",L"_addbpd ",L"增加变坡点",tlbr,index,vtFalse);
	CreateTbButton(L"删除变坡点",L"_delbpd ",L"删除变坡点",tlbr,index,vtFalse);
	CreateTbButton(L"修改坡段",L"_MDFPoDuan ",L"修改坡段",tlbr,index,vtFalse);
	CreateTbButton(L"插入坡段",L"_ADDPoDuan ",L"插入坡段",tlbr,index,vtFalse);
	//CreateTbButton(L"自动校整",L"_adjbpd ","自动校整",tlbr,index,vtFalse);
	CreateTbButton(L"坡段连接",L"_linkpdx ",L"坡段连接",tlbr,index,vtFalse);
	CreateTbButton(L"违规检测",L"_checkzm ",L"违规检测",tlbr,index,vtFalse);
	//	CreateTbButton(L"文件方式保存",L"_SaveBPD ","文件方式保存",tlbr,index,vtFalse);
	CreateTbButton(L"保存方案",L"_SaveZdm ",L"存入方案数据库",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcybridgedesignbridgedes()//桥梁设置
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	//hr = mnuGrp->get_Toolbars(&tlbrs);
	//mnuGrp->Release();

	//hr = mnuGrp->get_Toolbars(&tlbrs);
	//mnuGrp->Release();
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;
	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"桥梁设置")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//	tlbr->Float(150,50,50);
		}
	}

	hr = tlbrs->Add(L"桥梁设置", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons

	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(100,50,50);


	CreateTbButton(L"自动设桥",L"_calbri ",L"自动设桥",tlbr,index,vtFalse);
	CreateTbButton(L"导入桥梁",L"_inputbri ",L"导入桥梁",tlbr,index,vtFalse);
	CreateTbButton(L"删除桥梁",L"_delbri ",L"删除桥梁",tlbr,index,vtFalse);
	CreateTbButton(L"增加桥梁",L"_addbri ",L"增加桥梁",tlbr,index,vtFalse);
	CreateTbButton(L"编辑数据",L"_editbri ",L"桥数据编辑",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}



void darcysddesignsddesign()//隧道设置
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;



	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"隧道设置")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//	tlbr->Float(200,50,50);
		}
	}

	hr = tlbrs->Add(L"隧道设置", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons

	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(200,50,50);

	CreateTbButton(L"自动设隧",L"_caltun ",L"自动设隧",tlbr,index,vtFalse);
	CreateTbButton(L"导入隧道",L"_mdbintun ",L"导入隧道",tlbr,index,vtFalse);
	CreateTbButton(L"删除隧道",L"_deltun ",L"删除隧道",tlbr,index,vtFalse);
	CreateTbButton(L"增加隧道",L"_addtun ",L"增加隧道",tlbr,index,vtFalse);
	CreateTbButton(L"数据编辑",L"_edittun ",L"隧数据编辑",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcyzdmdeszdmdes()//纵断面一体化
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);

		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"纵断面一体化")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//tlbr->Float(250,50,50);
		}
	}

	hr = tlbrs->Add(L"纵断面一体化", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;  
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(250,50,50);

	CreateTbButton(L"纵断面一体化",L"_ ",L"纵断面一体化工具条",tlbr,index,vtFalse);
	CreateTbButton(L"填挖高查询",L"_cxtwg ",L"填挖高查询",tlbr,index,vtFalse);
	CreateTbButton(L"控制断面",L"_viewtypehdm ",L"纵控制断面",tlbr,index,vtFalse);
	CreateTbButton(L"断面提取",L"_CXHDM ",L"纵断面提取",tlbr,index,vtFalse);
	CreateTbButton(L"土方查询",L"_CXTSF ",L"纵土方查询",tlbr,index,vtFalse);
	CreateTbButton(L"三维浏览",L"_3D_BROWSER ",L"纵三维浏览",tlbr,index,vtFalse);

	//CreateTbButton(L"三维浏览",L"_browse_sw ","纵三维浏览",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}

void darcyhdmdes()//横断面设计
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);

		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"横断面设计")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//tlbr->Float(250,50,50);
		}
	}

	hr = tlbrs->Add(L"横断面设计", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(300,50,50);

	CreateTbButton(L"基本参数",L"_hdmjbcs ",L"横断面基本参数",tlbr,index,vtFalse);
	CreateTbButton(L"计算填挖表",L"_jstwb ",L"计算填挖表",tlbr,index,vtFalse);
	CreateTbButton(L"自动设计",L"_hdmsj ",L"自动设计",tlbr,index,vtFalse);
	CreateTbButton(L"导入横断面",L"_inhdm ",L"导入横断面",tlbr,index,vtFalse);
	CreateTbButton(L"编辑边坡",L"_edithdmbp ",L"编辑边坡",tlbr,index,vtFalse);

	CreateTbButton(L"路基工程数量",L"_ljgcsl ",L"路基工程数量",tlbr,index,vtFalse);

	//CreateTbButton(L"三维浏览",L"_browse_sw ","纵三维浏览",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyyddes()//横断面设计
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;
	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	IAcadToolbar  *tlbr = NULL;

	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,top,left;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);

		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"用地设计")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//tlbr->Float(250,50,50);
		}
	}

	hr = tlbrs->Add(L"用地设计", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(350,50,50);

	CreateTbButton(L"导入用地数据",L"_INZDK ",L"导入用地数据",tlbr,index,vtFalse);
	CreateTbButton(L"删除用地桩",L"_DELYDZ ",L"删除用地桩",tlbr,index,vtFalse);
	CreateTbButton(L"增加用地桩",L"_ADDYDZ ",L"增加用地桩",tlbr,index,vtFalse);
	CreateTbButton(L"编辑用地数据",L"_EDITYDZ ",L"编辑用地数据",tlbr,index,vtFalse);
	CreateTbButton(L"修改用地桩",L"_CHANGEYDZ ",L"修改用地桩",tlbr,index,vtFalse);

	CreateTbButton(L"保存用地数据",L"_SAVEYDZ ",L"保存用地数据",tlbr,index,vtFalse);
	CreateTbButton(L"用地面积计算",L"_CALYDAREA ",L"用地面积计算",tlbr,index,vtFalse);

	//CreateTbButton(L"三维浏览",L"_browse_sw ","纵三维浏览",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyplanplan()//平面图
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;

	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,left,top;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"平面图")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//	tlbr->Float(300,50,50);
		}
	}

	hr = tlbrs->Add(L"平面图", &tlbr); //加入名为DARCY的工具条
	if FAILED(hr)
		return;
	tlbrs->Release();
	//now add toolbar buttons
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;
	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(100,50,50);

	CreateTbButton(L"平面图",L"_",L"平面一体化工具条",tlbr,index,vtFalse);
	CreateTbButton(L"绘线路平面(单个)",L"_drawpm ",L"绘线路平面(单个)",tlbr,index,vtFalse);
	CreateTbButton(L"绘线路平面(批量)",L"_batch_drawpm ",L"绘线路平面(批量)",tlbr,index,vtFalse);
	CreateTbButton(L"标注桥梁",L"_PMBRI ",L"标注桥梁",tlbr,index,vtFalse);
	CreateTbButton(L"标注隧道",L"_PMTUN ",L"标注隧道",tlbr,index,vtFalse);
	CreateTbButton(L"标注车站",L"_PMSTA ",L"标注车站",tlbr,index,vtFalse);/////
	CreateTbButton(L"标注导线",L"_PMDX ",L"标注导线",tlbr,index,vtFalse);
	CreateTbButton(L"标注水准点",L"_PMBM ",L"标注水准点",tlbr,index,vtFalse);
	CreateTbButton(L"新建图框",L"_pmaddtk ",L"新建图框",tlbr,index,vtFalse);
	CreateTbButton(L"导入图框",L"_pmintk ",L"导入图框",tlbr,index,vtFalse);
	CreateTbButton(L"保存图框",L"_pmsavetk ",L"保存图框",tlbr,index,vtFalse);
	CreateTbButton(L"自动成图",L"_ptr ",L"自动成图",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcyverticalsectionver_section()//纵断面图
{

	IAcadMenuGroup *mnuGrp = NULL;//指针先付空值得值,否则会随意指向
	if (!getAcadMenuGroup(&mnuGrp))
	{
		acutPrintf(L"\nSearch menu error!");	
		return ;
	}
	//now get all the popup menus 
	IAcadToolbars  *tlbrs = NULL;

	HRESULT hr = S_OK;
	hr = mnuGrp->get_Toolbars(&tlbrs);
	mnuGrp->Release();
	//let us create toolbars for polysamp
	IAcadToolbar  *tlbr = NULL;
	VARIANT  vtName;
	vtName.vt = VT_I4;
	BSTR  tlbrName;

	bool found = false ;
	long cnt;
	int rows,left,top;
	tlbrs->get_Count(&cnt);

	for (long i=0; i < cnt; i++)
	{
		vtName.lVal = i;
		hr = tlbrs->Item(vtName, &tlbr);
		//tlbr->a();
		if (FAILED(hr))
			return ;
		hr  =tlbr->get_Name(&tlbrName);
		CString ctlbrName(tlbrName);
		if (ctlbrName.CompareNoCase(L"纵断面图")==0)  //纵断面图已存在就直接让它出现
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //得到上一次的停靠位置
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //得到前一次的停靠位置
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //在这个位置上浮现
				}
				else
				{ 

					tlbr->Dock(dockStatus);
				}
			}

			tlbr->Release();
			tlbrs->Release();
			return;
			//tlbr->Float(350,50,50);
		}
	}

	hr = tlbrs->Add(L"纵断面图", &tlbr); //加入名为DARCY的工具条

	if FAILED(hr)
		return;
	tlbrs->Release();
	//IAcadToolbarItem * button;
	IAcadToolbarItem* button;
	VARIANT index;
	index.vt = VT_I4;
	index.lVal = 100l;

	VARIANT vtFalse;
	vtFalse.vt = VT_BOOL;
	vtFalse.boolVal = VARIANT_FALSE;
	tlbr->Float(50,50,50);
	CreateTbButton(L"纵断面图",L"_",L"纵断面一体化工具条",tlbr,index,vtFalse);
	CreateTbButton(L"绘纵断面图",L"_drawzm ",L"纵初始化",tlbr,index,vtFalse);
	CreateTbButton(L"绘大中桥",L"_drawdzq ",L"绘大中桥",tlbr,index,vtFalse);
	CreateTbButton(L"绘隧道",L"_drawtun ",L"绘隧道",tlbr,index,vtFalse);
	CreateTbButton(L"绘车站",L"_zmdrawsta ",L"绘车站",tlbr,index,vtFalse);
	CreateTbButton(L"绘水准点",L"_zmdrawbm ",L"绘水准点",tlbr,index,vtFalse);
	CreateTbButton(L"绘小桥涵",L"_zmdrawxqh ",L"绘小桥涵",tlbr,index,vtFalse);
	CreateTbButton(L"绘竖曲线",L"_zmdrawsqx ",L"绘竖曲线",tlbr,index,vtFalse);
	CreateTbButton(L"单个桥",L"_zmsingleQ ",L"单个桥",tlbr,index,vtFalse);
	CreateTbButton(L"单个隧",L"_zmsingletun ",L"单个隧",tlbr,index,vtFalse);
	CreateTbButton(L"单个站",L"_zmsinglesta ",L"单个站",tlbr,index,vtFalse);
	CreateTbButton(L"单个涵",L"_zmsingleHD ",L"单个涵",tlbr,index,vtFalse);
	CreateTbButton(L"单个平交道",L"_zmsinglepjd ",L"单个平交道",tlbr,index,vtFalse);
	CreateTbButton(L"增加标尺",L"_zmaddbc ",L"增加标尺",tlbr,index,vtFalse);
	button=CreateTbButton(L"删除标尺",L"_zmdelbc ",L"删除标尺",tlbr,index,vtFalse);
	tlbr->AddSeparator(index,&button);
	button=CreateTbButton(L"保存标尺",L"_zmsavebc ",L"保存标尺",tlbr,index,vtFalse);
	tlbr->AddSeparator(index,&button);
	tlbr->Release();
	return;
}

void darcychecka()
{

	if(dlg!=NULL)
	{
		dlg->SetActiveWindow();
		return ;
	}
	dlg=new CCheckToolBar();
	dlg->Create(IDD_CheckToolBar,acedGetAcadFrame());
	dlg->ShowWindow(SW_SHOW);

}

// This is command 'CHOOSETOOLBAR'
void Tbpath(CString Tb, CString *path)//传入图标名，返回图标所在路径
{
	//GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
	path->Format(L"%s\\lib\\%s.bmp",SysPath,Tb);//当前目录下的res目录
	return;
}
//建立工具条的按扭，传入tooltip的显示文本,命令名，图标名，
IAcadToolbarItem* CreateTbButton(BSTR tooltip,BSTR Command,CString TbName,IAcadToolbar *tlbr,VARIANT index,VARIANT vtFalse)//
{

	IAcadToolbarItem* button=NULL;
	HRESULT hr=S_OK;

	hr = tlbr->AddToolbarButton(index,tooltip, L"Click the  button!",Command, vtFalse,&button); //提取数据,_getdata为单击后调用函数
	if(button)
	{
		CString  m_path;
		Tbpath(TbName,&m_path);
		button->SetBitmaps(m_path.AllocSysString(),m_path.AllocSysString());
		button->Release();
	}
	return button;
}




















