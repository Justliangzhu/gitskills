
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
///////// ///////////////////////ͨ����ARX������ʹ��COM�ӿڷ���AutoCAD////////
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
	///�����Զ��ؼ��ӿڵ�COM�ݾ� ��������ֵ��S_OK //��ȡAutoCadӦ�ó������
	if (FAILED(hr))
		return false;

	LPDISPATCH  pTempDisp = NULL;
	IAcadMenuGroups *mnuGrps = NULL; //�˵���
	long cnt = 0;

	//get the menu groups
	hr = acadApp->get_MenuGroups(&mnuGrps);//ͨ��AcadCADӦ�ó��򣬻�ò˵�����
	if (FAILED(hr))
	{
		acadApp->Release();
		return false;
	}
	mnuGrps->get_Count(&cnt);


	//get AutoCAD menu group. say it is index 0.
	IAcadMenuGroup *mnuGrp = NULL;//�˵�

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
			if(polyTlbr->get_Visible(&v)==S_OK) //���ֹ�����
			{
				if(v==-1) //����������
				{
					polyTlbr->Release();
					return true;
				}
				if(v==0)  //������������
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

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(t_Name)==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{

				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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


			polyTlbr->put_Visible(0);//�ɼ�Ϊ-1�����ɼ�Ϊ0

		}
		else
			polyTlbr->Release();

	}

	tlbrs->Release();
	return ;
}
void  darcydarcyxmglout()
{
	CleanUpToolbars("��Ŀ����");

}
void  darcydarcyjlsmout()
{
	CleanUpToolbars("������ģ");

}
void DarcySLDESIGNDXSJOUT()
{
	CleanUpToolbars("�������");
}
void DarcyFAGLOUT()
{
	CleanUpToolbars("��������");
}
void DarcyDLDESIGNDLSJOUT()
{
	CleanUpToolbars("�������");
}
void DarcyYTHDLSZOUT()
{   
	CleanUpToolbars("��������");
}
void DarcyPDDESIGNPDSJOUT()
{
	CleanUpToolbars("�¶����");
}
void DarcyBRIDGEDESIGNQLSJOUT()
{
	CleanUpToolbars("��������");
}
void DarcySDDESIGNSDDESIGNOUT()
{
	CleanUpToolbars("�������");
}
void DarcyPLANPMTOUT()
{
	CleanUpToolbars("ƽ��ͼ");
}
void DarcyZDMDESZDMTOUT()
{
	CleanUpToolbars("�ݶ���ͼ");
}

void DarcyDLDESIGNPMYTHOUT()
{
	CleanUpToolbars("ƽ��һ�廯");
}
void DarcyDLDESIGNZMYTHSJOUT()
{
	CleanUpToolbars("�ݶ���һ�廯");
}
void DarcyHDMDESOUT()
{
	CleanUpToolbars("��������");
}
void DarcyYDOUT()
{
	CleanUpToolbars("�õ����");
}
void  darcydarcyxmgl()
{
	//	CCheckToolBar Checktbar;
	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"��Ŀ����")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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


	hr = tlbrs->Add(L"��Ŀ����", &tlbr); //������ΪDARCY�Ĺ�����
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
	CreateTbButton(L"�½���Ŀ",L"_CreatePrj ","�½���Ŀ",tlbr,index,vtFalse);
	CreateTbButton(L"ѡ����Ŀ",L"_OpenPrj ","ѡ����Ŀ",tlbr,index,vtFalse);
	CreateTbButton(L"����¼�����ѯ",L"_InMdb ","����¼�����ѯ",tlbr,index,vtFalse);

	tlbr->Release();
	return;

}
void  darcydarcyjlsm()
{
	//	CCheckToolBar Checktbar;
	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"������ģ")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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


	hr = tlbrs->Add(L"������ģ", &tlbr); //������ΪDARCY�Ĺ�����
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
	CreateTbButton(L"��ȡ����",L"_pickdata ",L"��ȡ����",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_dealdata ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"������ģ",L"_createtin ",L"������ģ",tlbr,index,vtFalse);
	CreateTbButton(L"��ʾ��ģ",L"_displaydtm ",L"��ʾ��ģ",tlbr,index,vtFalse);

	tlbr->Release();
	return;

}


void darcysldesignsldesign()    //�������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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
	hr = tlbrs->Add(L"�������", &tlbr); //������ΪDARCY�Ĺ�����
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
	CreateTbButton(L"��������",L"_SET_PM_JBSJ ","��������",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_XXSJ ","ƽ�涨��",tlbr,index,vtFalse);
	//	CreateTbButton(L"�ļ���ʽ����",L"_ipm ","�ļ���ʽ����",tlbr,index,vtFalse);
	CreateTbButton(L"���ݿⷽʽ����",L"_ipmmdb ","���ݿⷽʽ����",tlbr,index,vtFalse);
	CreateTbButton(L"ת��̬�����",L"_SCLOC ","ת��̬�����",tlbr,index,vtFalse);
	CreateTbButton(L"�б�༭����",L"_JDB ","�б�༭����",tlbr,index,vtFalse);
	CreateTbButton(L"�������༭",L"_editJD ","�������༭",tlbr,index,vtFalse);
	CreateTbButton(L"�޸�����",L"_EJD ","�޸�����",tlbr,index,vtFalse);
	CreateTbButton(L"���뽻��",L"_Raddjd ","���뽻��",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ������",L"_Rdeljd ","ɾ������",tlbr,index,vtFalse);
	CreateTbButton(L"���������",L"_linkpm ","���������",tlbr,index,vtFalse);
	CreateTbButton(L"Υ����",L"_checkpm ","Υ����",tlbr,index,vtFalse);
	CreateTbButton(L"���淽��",L"_InputPmb ","���淽��",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcysldesignfagl()    //��������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"��������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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
	hr = tlbrs->Add(L"��������", &tlbr); //������ΪDARCY�Ĺ�����
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
	CreateTbButton(L"��������",L"_SETREL ","��������",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_ZHFA ","�������",tlbr,index,vtFalse);
	CreateTbButton(L"�����ת�����",L"_ZSJXST ","�����ת�����",tlbr,index,vtFalse);
	CreateTbButton(L"������Ϸ���",L"_SYZST ","������Ϸ���",tlbr,index,vtFalse);
	CreateTbButton(L"����ָ��",L"_JJZB ","����ָ��",tlbr,index,vtFalse);
	CreateTbButton(L"������ѡ",L"_FABX ","������ѡ",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcydldesigndldesign()  //�������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{
					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�������", &tlbr); //������ΪDARCY�Ĺ�����
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

	//CreateTbButton(L"��ʼ��",L"_EXSJ ","��ʼ��",tlbr,index,vtFalse);
	CreateTbButton(L"�Զ����ö���",L"_EXSJ ","�Զ����ö���",tlbr,index,vtFalse);
	CreateTbButton(L"������߷���",L"_IN2XPM ","������߷���",tlbr,index,vtFalse);
	CreateTbButton(L"�޸��߼��",L"_MDYXJJ ","�޸��߼��",tlbr,index,vtFalse);
	CreateTbButton(L"�༭��������",L"_EXSJ ","�༭��������",tlbr,index,vtFalse);
	CreateTbButton(L"����S��",L"_ADDSMODEL ","����S��",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ��S��",L"_DELSMODEL ","ɾ��S��",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ�����ж�",L"_DELRXD ","ɾ�����ж�",tlbr,index,vtFalse);
	CreateTbButton(L"���淽��",L"_InputPmb ","���淽��",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcydldes() //��������
{
	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"��������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"��������", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"�༭������",L"_editdlb ","�༭������",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_adddl ","�������",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ������",L"_deldl ","ɾ������",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyythyth()//ƽ��һ�廯
{
	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"ƽ��һ�廯")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{	
					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"ƽ��һ�廯", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"ƽ��һ�廯",L"_",L"ƽ��һ�廯������",tlbr,index,vtFalse);
	CreateTbButton(L"���ƶ���",L"_viewtypehdm ",L"���ƶ���",tlbr,index,vtFalse);
	CreateTbButton(L"������ȡ",L"_cxhdm ",L"������ȡ",tlbr,index,vtFalse);
	CreateTbButton(L"������ѯ",L"_cxtsf ",L"������ѯ",tlbr,index,vtFalse);
	CreateTbButton(L"��ά���",L"_3d_browser ",L"��ά���",tlbr,index,vtFalse);
	CreateTbButton(L"���淽��",L"_InputPmb ",L"������߷���",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcypddesignpddesign()//�������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�¶����")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�¶����", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"��������",L"_InitZdm ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"ѡ��ƽ�淽��",L"_ZM_ZDPMFA ",L"ѡ��ƽ�淽��",tlbr,index,vtFalse);
	CreateTbButton(L"������߷���",L"_InputZMFA ",L"������߷���",tlbr,index,vtFalse);
	CreateTbButton(L"�������������߷���",L"_INZXRXDZDM ",L"�������������߷���",tlbr,index,vtFalse);
	CreateTbButton(L"�������������߷���",L"_INYXRXDZDM ",L"�������������߷���",tlbr,index,vtFalse);

	CreateTbButton(L"�ɼ���������",L"_PickZdmDmx ",L"�ɼ���������",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_dmx ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"�������ݱ༭",L"_EDITDMD ",L"�������ݱ༭",tlbr,index,vtFalse);

	//CreateTbButton(L"ѡ�����·���",L"_PIZDM ","ѡ�����·���",tlbr,index,vtFalse);
	CreateTbButton(L"ֱ��ת�¶���",L"_IDZDM ",L"ֱ��ת�¶���",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_CZMLPSJ ",L"�������",tlbr,index,vtFalse);
	CreateTbButton(L"�����¶�",L"_MDYPD ",L"�����¶�",tlbr,index,vtFalse);
	CreateTbButton(L"�༭�¶�",L"_ZPB ",L"���ݱ༭",tlbr,index,vtFalse);
	CreateTbButton(L"���ӱ��µ�",L"_addbpd ",L"���ӱ��µ�",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ�����µ�",L"_delbpd ",L"ɾ�����µ�",tlbr,index,vtFalse);
	CreateTbButton(L"�޸��¶�",L"_MDFPoDuan ",L"�޸��¶�",tlbr,index,vtFalse);
	CreateTbButton(L"�����¶�",L"_ADDPoDuan ",L"�����¶�",tlbr,index,vtFalse);
	//CreateTbButton(L"�Զ�У��",L"_adjbpd ","�Զ�У��",tlbr,index,vtFalse);
	CreateTbButton(L"�¶�����",L"_linkpdx ",L"�¶�����",tlbr,index,vtFalse);
	CreateTbButton(L"Υ����",L"_checkzm ",L"Υ����",tlbr,index,vtFalse);
	//	CreateTbButton(L"�ļ���ʽ����",L"_SaveBPD ","�ļ���ʽ����",tlbr,index,vtFalse);
	CreateTbButton(L"���淽��",L"_SaveZdm ",L"���뷽�����ݿ�",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcybridgedesignbridgedes()//��������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"��������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"��������", &tlbr); //������ΪDARCY�Ĺ�����
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


	CreateTbButton(L"�Զ�����",L"_calbri ",L"�Զ�����",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_inputbri ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ������",L"_delbri ",L"ɾ������",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_addbri ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"�༭����",L"_editbri ",L"�����ݱ༭",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}



void darcysddesignsddesign()//�������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�������", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"�Զ�����",L"_caltun ",L"�Զ�����",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_mdbintun ",L"�������",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ�����",L"_deltun ",L"ɾ�����",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_addtun ",L"�������",tlbr,index,vtFalse);
	CreateTbButton(L"���ݱ༭",L"_edittun ",L"�����ݱ༭",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}

void darcyzdmdeszdmdes()//�ݶ���һ�廯
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�ݶ���һ�廯")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�ݶ���һ�廯", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"�ݶ���һ�廯",L"_ ",L"�ݶ���һ�廯������",tlbr,index,vtFalse);
	CreateTbButton(L"���ڸ߲�ѯ",L"_cxtwg ",L"���ڸ߲�ѯ",tlbr,index,vtFalse);
	CreateTbButton(L"���ƶ���",L"_viewtypehdm ",L"�ݿ��ƶ���",tlbr,index,vtFalse);
	CreateTbButton(L"������ȡ",L"_CXHDM ",L"�ݶ�����ȡ",tlbr,index,vtFalse);
	CreateTbButton(L"������ѯ",L"_CXTSF ",L"��������ѯ",tlbr,index,vtFalse);
	CreateTbButton(L"��ά���",L"_3D_BROWSER ",L"����ά���",tlbr,index,vtFalse);

	//CreateTbButton(L"��ά���",L"_browse_sw ","����ά���",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}

void darcyhdmdes()//��������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"��������")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"��������", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"��������",L"_hdmjbcs ",L"������������",tlbr,index,vtFalse);
	CreateTbButton(L"�������ڱ�",L"_jstwb ",L"�������ڱ�",tlbr,index,vtFalse);
	CreateTbButton(L"�Զ����",L"_hdmsj ",L"�Զ����",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_inhdm ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"�༭����",L"_edithdmbp ",L"�༭����",tlbr,index,vtFalse);

	CreateTbButton(L"·����������",L"_ljgcsl ",L"·����������",tlbr,index,vtFalse);

	//CreateTbButton(L"��ά���",L"_browse_sw ","����ά���",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyyddes()//��������
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�õ����")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�õ����", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"�����õ�����",L"_INZDK ",L"�����õ�����",tlbr,index,vtFalse);
	CreateTbButton(L"ɾ���õ�׮",L"_DELYDZ ",L"ɾ���õ�׮",tlbr,index,vtFalse);
	CreateTbButton(L"�����õ�׮",L"_ADDYDZ ",L"�����õ�׮",tlbr,index,vtFalse);
	CreateTbButton(L"�༭�õ�����",L"_EDITYDZ ",L"�༭�õ�����",tlbr,index,vtFalse);
	CreateTbButton(L"�޸��õ�׮",L"_CHANGEYDZ ",L"�޸��õ�׮",tlbr,index,vtFalse);

	CreateTbButton(L"�����õ�����",L"_SAVEYDZ ",L"�����õ�����",tlbr,index,vtFalse);
	CreateTbButton(L"�õ��������",L"_CALYDAREA ",L"�õ��������",tlbr,index,vtFalse);

	//CreateTbButton(L"��ά���",L"_browse_sw ","����ά���",tlbr,index,vtFalse);
	tlbr->Release();
	return;
}
void darcyplanplan()//ƽ��ͼ
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"ƽ��ͼ")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"ƽ��ͼ", &tlbr); //������ΪDARCY�Ĺ�����
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

	CreateTbButton(L"ƽ��ͼ",L"_",L"ƽ��һ�廯������",tlbr,index,vtFalse);
	CreateTbButton(L"����·ƽ��(����)",L"_drawpm ",L"����·ƽ��(����)",tlbr,index,vtFalse);
	CreateTbButton(L"����·ƽ��(����)",L"_batch_drawpm ",L"����·ƽ��(����)",tlbr,index,vtFalse);
	CreateTbButton(L"��ע����",L"_PMBRI ",L"��ע����",tlbr,index,vtFalse);
	CreateTbButton(L"��ע���",L"_PMTUN ",L"��ע���",tlbr,index,vtFalse);
	CreateTbButton(L"��ע��վ",L"_PMSTA ",L"��ע��վ",tlbr,index,vtFalse);/////
	CreateTbButton(L"��ע����",L"_PMDX ",L"��ע����",tlbr,index,vtFalse);
	CreateTbButton(L"��עˮ׼��",L"_PMBM ",L"��עˮ׼��",tlbr,index,vtFalse);
	CreateTbButton(L"�½�ͼ��",L"_pmaddtk ",L"�½�ͼ��",tlbr,index,vtFalse);
	CreateTbButton(L"����ͼ��",L"_pmintk ",L"����ͼ��",tlbr,index,vtFalse);
	CreateTbButton(L"����ͼ��",L"_pmsavetk ",L"����ͼ��",tlbr,index,vtFalse);
	CreateTbButton(L"�Զ���ͼ",L"_ptr ",L"�Զ���ͼ",tlbr,index,vtFalse);

	tlbr->Release();
	return;
}


void darcyverticalsectionver_section()//�ݶ���ͼ
{

	IAcadMenuGroup *mnuGrp = NULL;//ָ���ȸ���ֵ��ֵ,���������ָ��
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
		if (ctlbrName.CompareNoCase(L"�ݶ���ͼ")==0)  //�ݶ���ͼ�Ѵ��ھ�ֱ����������
		{
			found = true;
			// hr=tlbr->get_Visible(&boolVal);
			AcToolbarDockStatus dockStatus;

			if (tlbr->get_DockStatus(&dockStatus)==S_OK)  //�õ���һ�ε�ͣ��λ��
			{
				if(dockStatus==acToolbarFloating)       
				{

					tlbr->get_FloatingRows(&rows);     //�õ�ǰһ�ε�ͣ��λ��
					tlbr->get_Left(&left);
					tlbr->get_Top(&top);
					tlbr->Float(top,left,rows);     //�����λ���ϸ���
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

	hr = tlbrs->Add(L"�ݶ���ͼ", &tlbr); //������ΪDARCY�Ĺ�����

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
	CreateTbButton(L"�ݶ���ͼ",L"_",L"�ݶ���һ�廯������",tlbr,index,vtFalse);
	CreateTbButton(L"���ݶ���ͼ",L"_drawzm ",L"�ݳ�ʼ��",tlbr,index,vtFalse);
	CreateTbButton(L"�������",L"_drawdzq ",L"�������",tlbr,index,vtFalse);
	CreateTbButton(L"�����",L"_drawtun ",L"�����",tlbr,index,vtFalse);
	CreateTbButton(L"�泵վ",L"_zmdrawsta ",L"�泵վ",tlbr,index,vtFalse);
	CreateTbButton(L"��ˮ׼��",L"_zmdrawbm ",L"��ˮ׼��",tlbr,index,vtFalse);
	CreateTbButton(L"��С�ź�",L"_zmdrawxqh ",L"��С�ź�",tlbr,index,vtFalse);
	CreateTbButton(L"��������",L"_zmdrawsqx ",L"��������",tlbr,index,vtFalse);
	CreateTbButton(L"������",L"_zmsingleQ ",L"������",tlbr,index,vtFalse);
	CreateTbButton(L"������",L"_zmsingletun ",L"������",tlbr,index,vtFalse);
	CreateTbButton(L"����վ",L"_zmsinglesta ",L"����վ",tlbr,index,vtFalse);
	CreateTbButton(L"������",L"_zmsingleHD ",L"������",tlbr,index,vtFalse);
	CreateTbButton(L"����ƽ����",L"_zmsinglepjd ",L"����ƽ����",tlbr,index,vtFalse);
	CreateTbButton(L"���ӱ��",L"_zmaddbc ",L"���ӱ��",tlbr,index,vtFalse);
	button=CreateTbButton(L"ɾ�����",L"_zmdelbc ",L"ɾ�����",tlbr,index,vtFalse);
	tlbr->AddSeparator(index,&button);
	button=CreateTbButton(L"������",L"_zmsavebc ",L"������",tlbr,index,vtFalse);
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
void Tbpath(CString Tb, CString *path)//����ͼ����������ͼ������·��
{
	//GetPrivateProfileString("RLDVS","RLDVS",NULL,SysPath,256,"win.ini");
	path->Format(L"%s\\lib\\%s.bmp",SysPath,Tb);//��ǰĿ¼�µ�resĿ¼
	return;
}
//�����������İ�Ť������tooltip����ʾ�ı�,��������ͼ������
IAcadToolbarItem* CreateTbButton(BSTR tooltip,BSTR Command,CString TbName,IAcadToolbar *tlbr,VARIANT index,VARIANT vtFalse)//
{

	IAcadToolbarItem* button=NULL;
	HRESULT hr=S_OK;

	hr = tlbr->AddToolbarButton(index,tooltip, L"Click the  button!",Command, vtFalse,&button); //��ȡ����,_getdataΪ��������ú���
	if(button)
	{
		CString  m_path;
		Tbpath(TbName,&m_path);
		button->SetBitmaps(m_path.AllocSysString(),m_path.AllocSysString());
		button->Release();
	}
	return button;
}




















