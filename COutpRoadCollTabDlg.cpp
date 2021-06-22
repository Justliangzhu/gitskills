// COutpRoadCollTabDlg.cpp: 实现文件
//

#include "StdAfx.h"
#include "COutpRoadCollTabDlg.h"
#include "afxdialogex.h"
#include "XLDataBase.h"
#include "MROADExcel.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016
// COutpRoadCollTabDlg 对话框

extern CString Cworkdir, Pro;
extern ACHAR SysPath[1024];

IMPLEMENT_DYNAMIC(COutpRoadCollTabDlg, CDialog)

COutpRoadCollTabDlg::COutpRoadCollTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_OUTPROADCOLLTAB, pParent)
	, m_Coor_X(0)
	, m_Coor_Y(0)
{

}

COutpRoadCollTabDlg::~COutpRoadCollTabDlg()
{
}

void COutpRoadCollTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OUTPROADCOLLTAB_X, m_Coor_X);
	DDX_Text(pDX, IDC_EDIT_OUTPROADCOLLTAB_Y, m_Coor_Y);
}


BEGIN_MESSAGE_MAP(COutpRoadCollTabDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_OUTPROADCOLLTAB_PICK, &COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabPick)
	ON_BN_CLICKED(IDC_BUTTON_OUTPROADCOLLTAB_DRAW, &COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabDraw)
	ON_BN_CLICKED(IDC_BUTTON_OUTPROADCOLLTAB_EXCEL, &COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabExcel)
END_MESSAGE_MAP()


// COutpRoadCollTabDlg 消息处理程序


BOOL COutpRoadCollTabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabPick()
{
	// TODO: 在此添加控件通知处理程序代码
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	BAS_DRAW_FUN BasFun;
	AcGePoint3d ptPick;
	ACHAR Prom[256];
	_tcscpy(Prom, L"\n请在窗口中拾取绘图起点：");
	if (BasFun.getPoint(Prom, ptPick) == RTNORM)
	{
		m_Coor_X = (double)((int)(ptPick.x * 1000)) / 1000.0;
		m_Coor_Y = (double)((int)(ptPick.y * 1000)) / 1000.0;
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/
	UpdateData(false);
}


void COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabDraw()
{
	// TODO: 在此添加控件通知处理程序代码


	ACHAR DWGPath[256];
	double TextH = 3.5;
	AcGePoint3d InsertPt;
	ads_point InsPt;
	AcGePoint3d NotePt;
	int PageSum;
	int PageIndex;
	InsertPt.x = m_Coor_X;
	InsertPt.y = m_Coor_Y;
	InsertPt.z = 0.0;
	_stprintf(DWGPath, L"%s\\dwg\\改移道路汇总表.dwg", SysPath);

	double RowHeigh = 29.0;
	double ColWide[19];
	ColWide[0] = 14.0;
	ColWide[1] = 25.5;
	ColWide[2] = 14.0;
	ColWide[3] = 13.0;
	ColWide[4] = 14.5;
	ColWide[5] = 11.0;
	ColWide[6] = 47.0;
	ColWide[7] = 12.5;
	ColWide[8] = 12.0;
	ColWide[9] = 12.0;
	ColWide[10] = 20.0;
	ColWide[11] = 20.0;
	ColWide[12] = 20.0;
	ColWide[13] = 15.0;
	ColWide[14] = 15.0;
	ColWide[15] = 15.0;
	ColWide[16] = 15.0;
	ColWide[17] = 57.0;
	ColWide[18] = 32.5;

	XLDataBase DBS;
	DBS.ReadWorkdir();
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	if (DBS.Read_XLDbs(MdfName, L"改移道路表"))
	{
		acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
		if (DBS.ModifyRoadSum > 0)
		{
			PageSum = (int)(DBS.ModifyRoadSum / 8.0) + 1;
			int iRoad = 0;
			for (int PageIndex = 0; PageIndex < PageSum; PageIndex++)
			{

				InsPt[X] = InsertPt.x;
				InsPt[Y] = InsertPt.y;
				InsPt[Z] = 0.0;

#ifdef VERSION_CAD2016
				acedCommandS(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
				ads_command(RTSTR, L"-insert", RTSTR, DWGPath, RTSTR, L"s", RTREAL, 1.0, RTPOINT, InsPt, RTREAL, 0.0, RTNONE);
#endif // VERSION_CAD2010

				NotePt.x = InsertPt.x + 402.0;
				NotePt.y = InsertPt.y + 282.0;
				ACHAR PageInf[20];
				_stprintf(PageInf, L"第 %d 页  共 %d 页", PageIndex + 1, PageSum);
				DBS.maketext(NotePt, PageInf, 0.0, 4.0);

				for (int iRow = 0; iRow < 8; iRow++)
				{
					TextH = 3.5;

					NotePt.x = InsertPt.x + 57.0;
					NotePt.y = InsertPt.y + 234.5 - iRow * RowHeigh;

					ACHAR TextArray[19][40];
					ACHAR Ckml[20], GH[8];
					double dml;

					for (int iCol = 0; iCol < 19; iCol++)
						_tcscpy(TextArray[iCol], L"\0");

					if (!DBS.pModifyRoad[iRoad].SCkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].SCkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[0], DBS.LCchr(GH, dml, 1, 1));
					}

					if (!DBS.pModifyRoad[iRoad].RailCkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].RailCkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[1], DBS.LCchr(GH, dml, 1, 1));
					}

					if (!DBS.pModifyRoad[iRoad].ECkml.IsEmpty())
					{
						_tcscpy(Ckml, DBS.pModifyRoad[iRoad].ECkml);
						DBS.split_ckml(Ckml, &dml, GH);
						_tcscpy(TextArray[2], DBS.LCchr(GH, dml, 1, 1));
					}

					if (DBS.pModifyRoad[iRoad].RoadLen > 1.0)
						_stprintf(TextArray[3], L"%.0lf", DBS.pModifyRoad[iRoad].RoadLen);

					if (DBS.pModifyRoad[iRoad].InterAng > 1.0)
						_stprintf(TextArray[4], L"%.4lf", DBS.pModifyRoad[iRoad].InterAng);

					if (!DBS.pModifyRoad[iRoad].UpDown.IsEmpty())
						_tcscpy(TextArray[5], DBS.pModifyRoad[iRoad].UpDown);

					if (!DBS.pModifyRoad[iRoad].KKSY.IsEmpty())
						_tcscpy(TextArray[6], DBS.pModifyRoad[iRoad].KKSY);

					if (DBS.pModifyRoad[iRoad].LjWide > 1.0)
						ads_rtos(DBS.pModifyRoad[iRoad].LjWide, 2, 1, TextArray[8]);

					if (DBS.pModifyRoad[iRoad].LmWide > 1.0)
						ads_rtos(DBS.pModifyRoad[iRoad].LmWide, 2, 1, TextArray[9]);

					if (DBS.pModifyRoad[iRoad].LmThick > 1.0)
						_stprintf(TextArray[10], L"%.0lf", DBS.pModifyRoad[iRoad].LmThick);

					if (!DBS.pModifyRoad[iRoad].LmType.IsEmpty())
						_tcscpy(TextArray[11], DBS.pModifyRoad[iRoad].LmType);

					if (DBS.pModifyRoad[iRoad].LmArea > 1.0)
						_stprintf(TextArray[12], L"%.0lf", DBS.pModifyRoad[iRoad].LmArea);

					if (DBS.pModifyRoad[iRoad].FillVol > 1.0)
						_stprintf(TextArray[13], L"%.0lf", DBS.pModifyRoad[iRoad].FillVol);

					if (DBS.pModifyRoad[iRoad].CutVol > 1.0)
						_stprintf(TextArray[14], L"%.0lf", DBS.pModifyRoad[iRoad].CutVol);

					if (DBS.pModifyRoad[iRoad].MasonryVol > 1.0)
						_stprintf(TextArray[15], L"%.0lf", DBS.pModifyRoad[iRoad].MasonryVol);

					if (!DBS.pModifyRoad[iRoad].Note.IsEmpty())
						_tcscpy(TextArray[18], DBS.pModifyRoad[iRoad].Note);


					if (_tcslen(TextArray[0]) > 0)
						DBS.maketext(NotePt, TextArray[0], 0.0, TextH);

					for (int iCol = 1; iCol < 19; iCol++)
					{
						NotePt.x += 0.5 * (ColWide[iCol - 1] + ColWide[iCol]);
						if (_tcslen(TextArray[iCol]) > 0)
							DBS.maketext(NotePt, TextArray[iCol], 0.0, TextH);
					}

					//多行文字写公路等级
					if (!DBS.pModifyRoad[iRoad].RandType.IsEmpty())
					{
						_tcscpy(TextArray[7], DBS.pModifyRoad[iRoad].RandType);
						NotePt.x = InsertPt.x + 195.25;
						DBS.MakeMText(NotePt, TextArray[7], 0.0, TextH, TextH);
					}

					//多行文字写用地
					if (!DBS.pModifyRoad[iRoad].LandAreaStr.IsEmpty())
					{
						int SLandTypeIndex = -1;
						int ELandTypeIndex = -1;
						int LandNumIndex = 0;
						CString TempStr = DBS.pModifyRoad[iRoad].LandAreaStr;
						CStringArray NoteTextArray;
						SLandTypeIndex = TempStr.FindOneOf(L"(");
						ELandTypeIndex = TempStr.FindOneOf(L")");
						while (SLandTypeIndex > -1 && ELandTypeIndex > SLandTypeIndex)
						{
							NoteTextArray.Add(TempStr.Mid(LandNumIndex, SLandTypeIndex - LandNumIndex));
							NoteTextArray.Add(TempStr.Mid(SLandTypeIndex + 1, ELandTypeIndex - SLandTypeIndex - 1));
							TempStr = TempStr.Mid(ELandTypeIndex + 1, TempStr.GetLength() - ELandTypeIndex);
							SLandTypeIndex = TempStr.FindOneOf(L"(");
							ELandTypeIndex = TempStr.FindOneOf(L")");
						}
						if (NoteTextArray.GetSize() == 0)
						{
							ads_printf(L"第%张表第%d行中的用地数据格式有误！\n", PageIndex + 1, iRow + 1);
							NoteTextArray.Add(DBS.pModifyRoad[iRoad].LandAreaStr);
						}
						NotePt.x = InsertPt.x + 338.0;
						int TypeSum = (int)((NoteTextArray.GetSize() + 0.5) / 2.0);
						NotePt.y += (1.2 * (TypeSum - 1) * TextH + 0.5 * 1.2 * TextH);
						for (int i = 0; i < TypeSum; i++)
						{
							_tcscpy(TextArray[16], NoteTextArray[2 * i]);
							DBS.maketext(NotePt, TextArray[16], 0.0, TextH);
							NotePt.y -= 1.2 * TextH;
							_tcscpy(TextArray[16], NoteTextArray[2 * i + 1]);
							DBS.maketext(NotePt, TextArray[16], 0.0, TextH);
							NotePt.y -= 1.2 * TextH;
						}
					}

					//绘制纵断面示意图
					//////////////////////////////////////////////////////////////////////////
					CString RoadName = DBS.pModifyRoad[iRoad].RoadName;
					CString MdbName = Cworkdir + "\\DATA\\" + RoadName + ".mdb";
					double PD_array[MAXBPDNUM][4];
					int res = DBS.Read_XLDbs(MdbName, L"坡度表", L"主线", L"全幅");
					int res1 = DBS.Read_XLDbs(MdbName, L"纵断链表", L"主线");
					if (res && res1)//读坡度
					{
						int NBPD = DBS.NPD;
						double MaxLever = -1e9;
						double MinLever = 1e9;
						for (int i = 0; i < NBPD; i++)
						{
							PD_array[i][0] = DBS.TYLC1(DBS.PDB[i].ml, DBS.NZDL, DBS.ZDLArray);
							PD_array[i][1] = DBS.PDB[i].Level;
							PD_array[i][2] = DBS.PDB[i].Rshu;
							PD_array[i][3] = 0.0;
							if (MaxLever < PD_array[i][1])
								MaxLever = PD_array[i][1];
							if (MinLever > PD_array[i][1])
								MinLever = PD_array[i][1];
						}

						if (DBS.PDB)
						{
							if (DBS.PDB) delete[]DBS.PDB;
							DBS.PDB = NULL;
						}

						TextH = 3.0;
						double XScale, YScale;
						double MlNoteLen = 15.0;
						AcGePoint3d ZDMCenPt, SPt, EPt;
						ZDMCenPt.x = InsertPt.x + 345.5;
						ZDMCenPt.y = InsertPt.y + 220.0 - iRow * RowHeigh;

						XScale = (ColWide[17] - 8.0) / (PD_array[NBPD - 1][0] - PD_array[0][0]);
						YScale = (RowHeigh - 8.0 - MlNoteLen - TextH) / (MaxLever - MinLever);

						SPt.x = ZDMCenPt.x + 4.0;
						SPt.y = ZDMCenPt.y + 4.0 + (PD_array[0][1] - MinLever) * YScale;
						NotePt.x = SPt.x;
						NotePt.y = SPt.y + MlNoteLen;
						DBS.makeline(SPt, NotePt, 0, 0.0);//绘起点里程标注线
						dml = DBS.XLC1(PD_array[0][0], GH, DBS.NZDL, DBS.ZDLArray);
						_tcscpy(Ckml, DBS.LCchr(GH, dml, 3, 1));
						NotePt.y = SPt.y + TextH;
						NotePt.x += 1.2 * TextH;
						DBS.maketext(NotePt, Ckml, 0.5 * pi, TextH, 0, 1);//标注起点里程

						double RoadCml = DBS.TYLC1(DBS.pModifyRoad[iRoad].RoadDml, DBS.NZDL, DBS.ZDLArray);

						for (int i = 1; i < NBPD; i++)
						{
							EPt.x = ZDMCenPt.x + 4.0 + (PD_array[i][0] - PD_array[0][0]) * XScale;
							EPt.y = ZDMCenPt.y + 4.0 + (PD_array[i][1] - MinLever) * YScale;
							DBS.makeline(SPt, EPt, 0, 0.35);//绘坡度线
							NotePt.x = EPt.x;
							NotePt.y = EPt.y + MlNoteLen;
							DBS.makeline(EPt, NotePt, 0, 0.0);//绘里程标注线
							dml = DBS.XLC1(PD_array[i][0], GH, DBS.NZDL, DBS.ZDLArray);
							_tcscpy(Ckml, DBS.LCchr(GH, dml, 3, 1));
							NotePt.y = EPt.y + TextH;
							NotePt.x -= 0.2 * TextH;
							DBS.maketext(NotePt, Ckml, 0.5 * pi, TextH, 0, 1);//标注里程

							NotePt.x = 0.5 * (SPt.x + EPt.x);
							NotePt.y = 0.5 * (SPt.y + EPt.y);
							ads_point Pt1, Pt2;
							Pt1[X] = SPt.x;
							Pt1[Y] = SPt.y;
							Pt1[Z] = 0.0;
							Pt2[X] = EPt.x;
							Pt2[Y] = EPt.y;
							Pt2[Z] = 0.0;
							double Ang = ads_angle(Pt1, Pt2);
							double PDLen = PD_array[i][0] - PD_array[i - 1][0];
							double Degree = (PD_array[i][1] - PD_array[i - 1][1]) / (PD_array[i][0] - PD_array[i - 1][0]) * 100.0;
							ads_rtos(PDLen, 2, 3, Ckml);
							NotePt.y += 0.6 * TextH;
							DBS.maketext(NotePt, Ckml, Ang, TextH);//标注坡长
							ads_rtos(Degree, 2, 1, Ckml);
							_tcscat(Ckml, L"%");
							NotePt.y -= 1.6 * TextH;
							DBS.maketext(NotePt, Ckml, Ang, TextH);//标注坡度

							if (RoadCml > PD_array[i - 1][0] && RoadCml < PD_array[i][0])
							{
								AcGePoint3d RailSPt, RailEPt;
								RailSPt.x = SPt.x + (RoadCml - PD_array[i - 1][0]) * XScale;
								RailSPt.y = SPt.y + (EPt.y - SPt.y) / (EPt.x - SPt.x) * (RoadCml - PD_array[i - 1][0]) * XScale;
								RailEPt.x = RailSPt.x;
								RailEPt.y = RailSPt.y + MlNoteLen;
								NotePt.x = RailSPt.x - 0.6 * TextH;
								NotePt.y = RailEPt.y;
								DBS.makeline(RailSPt, RailEPt, 0, 0.0);
								DBS.maketext(NotePt, L"铁路中心", 0.5 * pi, TextH);//标注铁路中心
							}

							SPt = EPt;
						}


					}
					//////////////////////////////////////////////////////////////////////////


					iRoad++;
					if (iRoad == DBS.ModifyRoadSum)
					{
						AfxMessageBox(L"绘制成功!");
						return;
					}

				}
				InsertPt.x += 485.0;
			}
			AfxMessageBox(L"绘制成功!");
		}
		acDocManager->unlockDocument(acDocManager->curDocument());
	}
	else
	{
		AfxMessageBox(L"改移道路汇总表中无数据!");
		return;
	}


}


void COutpRoadCollTabDlg::OnBnClickedButtonOutproadcolltabExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取道路信息数据
	XLDataBase DBS;
	DBS.ReadWorkdir();
	CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
	if (!DBS.Read_XLDbs(MdfName, L"改移道路表"))
	{
		AfxMessageBox(L"读取改移道路表信息失败！请检查是否有道路方案");
		return;
	}
	if (DBS.ModifyRoadSum <= 0)
	{
		AfxMessageBox(L"读取改移道路表信息失败！请检查是否有道路方案");
		return;
	}

	//获取模板文件路径
	ACHAR OriFilePath[280];
	_stprintf(OriFilePath, L"%s\\lib\\用地模板.xlt", SysPath);

	BAS_DRAW_FUN BasFun;
	BasFun.ReadWorkdir();
	CString DesFilePath = Cworkdir + L"\\RESULT\\道路汇总表.xls";

	//初始化Excel
	CMROADExcel iExcel;
	iExcel.ConnectExcelWookSheet(DesFilePath, L"Sheet1");


	//写表头
	iExcel.WriteExcel(1, 1, L"改 移 道 路 及 立 交 道 设 计 表");

	iExcel.WriteExcel(2, 1, L"道路名");

	iExcel.WriteExcel(2, 2, L"里程");
	iExcel.WriteExcel(3, 2, L"起");
	iExcel.WriteExcel(3, 3, L"道口（立交）中心");
	iExcel.WriteExcel(3, 4, L"迄");

	iExcel.WriteExcel(2, 5, L"改移道路长度(m)");
	iExcel.WriteExcel(2, 6, L"交叉角(°′″）");
	iExcel.WriteExcel(2, 7, L"铁路在上或在下");
	iExcel.WriteExcel(2, 8, L"孔跨结构");

	iExcel.WriteExcel(2, 9, L"道路");
	iExcel.WriteExcel(3, 9, L"等级及种类");
	iExcel.WriteExcel(3, 10, L"路基宽(m)");

	iExcel.WriteExcel(2, 11, L"路面工程");
	iExcel.WriteExcel(3, 11, L"宽度(m)");
	iExcel.WriteExcel(3, 12, L"厚度(cm)");
	iExcel.WriteExcel(3, 13, L"材料");
	iExcel.WriteExcel(3, 14, L"数量(m²)");

	iExcel.WriteExcel(2, 15, L"道路土石方");
	iExcel.WriteExcel(3, 15, L"填方(m³)");
	iExcel.WriteExcel(3, 16, L"挖方(m³)");
	iExcel.WriteExcel(3, 17, L"圬工(m³)");
	iExcel.WriteExcel(3, 18, L"用地(亩)");

	iExcel.WriteExcel(2, 19, L"附注");

	//写入数据
	int CurLineNum = 4;
	CString tempCStr;
	for (int i = 0; i < DBS.ModifyRoadSum; i++)
	{
		//道路名
		iExcel.WriteExcel(CurLineNum, 1, _variant_t(DBS.pModifyRoad[i].RoadName));

		//里程	始	交叉	迄
		iExcel.WriteExcel(CurLineNum, 2, _variant_t(DBS.pModifyRoad[i].SCkml));
		iExcel.WriteExcel(CurLineNum, 3, _variant_t(DBS.pModifyRoad[i].RailCkml));
		iExcel.WriteExcel(CurLineNum, 4, _variant_t(DBS.pModifyRoad[i].ECkml));

		//改移道路长度
		if (DBS.pModifyRoad[i].RoadLen > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].RoadLen);
			iExcel.WriteExcel(CurLineNum, 5, _variant_t(tempCStr));
		}

		//交叉角
		if (DBS.pModifyRoad[i].InterAng > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].InterAng);
			iExcel.WriteExcel(CurLineNum, 6, _variant_t(tempCStr));
		}

		//铁路在上或在下
		iExcel.WriteExcel(CurLineNum, 7, _variant_t(DBS.pModifyRoad[i].UpDown));

		//孔跨结构
		iExcel.WriteExcel(CurLineNum, 8, _variant_t(DBS.pModifyRoad[i].KKSY));

		//道路	等级与种类（暂时无）	路基宽
		iExcel.WriteExcel(CurLineNum, 9, _variant_t(DBS.pModifyRoad[i].RandType));
		iExcel.WriteExcel(CurLineNum, 10, _variant_t(DBS.pModifyRoad[i].LjWide));

		//路面工程	宽度	厚度	材料	数量
		if (DBS.pModifyRoad[i].LmWide > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].LmWide);
			iExcel.WriteExcel(CurLineNum, 11, _variant_t(tempCStr));
		}
		if (DBS.pModifyRoad[i].LmThick > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].LmThick);
			iExcel.WriteExcel(CurLineNum, 12, _variant_t(tempCStr));
		}
		iExcel.WriteExcel(CurLineNum, 13, _variant_t(DBS.pModifyRoad[i].LmType));
		if (DBS.pModifyRoad[i].LmArea > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].LmArea);
			iExcel.WriteExcel(CurLineNum, 14, _variant_t(tempCStr));
		}

		//道路土石方	填方	挖方	圬工	用地
		if (DBS.pModifyRoad[i].FillVol > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].FillVol);
			iExcel.WriteExcel(CurLineNum, 15, _variant_t(tempCStr));
		}
		if (DBS.pModifyRoad[i].CutVol > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].CutVol);
			iExcel.WriteExcel(CurLineNum, 16, _variant_t(tempCStr));
		}
		if (DBS.pModifyRoad[i].MasonryVol > 0.0)
		{
			tempCStr.Format(L"%.3lf", DBS.pModifyRoad[i].MasonryVol);
			iExcel.WriteExcel(CurLineNum, 17, _variant_t(tempCStr));
		}
		iExcel.WriteExcel(CurLineNum, 18, _variant_t(DBS.pModifyRoad[i].LandAreaStr));

		//附注
		iExcel.WriteExcel(CurLineNum, 18, _variant_t(DBS.pModifyRoad[i].Note));

		CurLineNum++;
	}


	//设置表头	增加道路名一列
	iExcel.MergeCellInExcel(1, 1, L"A", L"T");
	iExcel.MergeCellInExcel(2, 3, L"A", L"A");
	iExcel.MergeCellInExcel(2, 2, L"B", L"D");
	iExcel.MergeCellInExcel(2, 3, L"E", L"E");
	iExcel.MergeCellInExcel(2, 3, L"F", L"F");
	iExcel.MergeCellInExcel(2, 3, L"G", L"G");
	iExcel.MergeCellInExcel(2, 3, L"H", L"H");
	iExcel.MergeCellInExcel(2, 2, L"I", L"J");
	iExcel.MergeCellInExcel(2, 2, L"K", L"N");
	iExcel.MergeCellInExcel(2, 2, L"O", L"R");
	iExcel.MergeCellInExcel(2, 3, L"T", L"T");

	//自动调整
	iExcel.AutoSize();

	iExcel.QuitSheetConnect();
	iExcel.QuitExcel(true);
}
