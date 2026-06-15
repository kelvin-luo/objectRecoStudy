// ClassicalStudy.cpp

#include "stdafx.h"
#include "objectRecoStudy.h"
#include "ClassicalStudy.h"
#include "classicalBayessianDlg.h"
#include "ClassicalLinearDlg.h"
#include "afxdialogex.h"
#include "UiStrings.h"

IMPLEMENT_DYNAMIC(ClassicalStudy, CDialogEx)

ClassicalStudy::ClassicalStudy(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2ClassicalStudy, pParent)
{
}

ClassicalStudy::~ClassicalStudy()
{
}

void ClassicalStudy::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClassicalStudy, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ClassicalStudy::OnBnClickedButton1_classical_Bayessian)
	ON_BN_CLICKED(IDC_BUTTON2, &ClassicalStudy::OnBnClickedButton2_classical_Linear)
	ON_BN_CLICKED(IDC_BUTTON3, &ClassicalStudy::OnBnClickedButton3_classical_Non_linear)
END_MESSAGE_MAP()

void ClassicalStudy::OnBnClickedButton1_classical_Bayessian()
{
	classicalBayessianDlg dlg(this);
	dlg.DoModal();
}

void ClassicalStudy::OnBnClickedButton2_classical_Linear()
{
	ClassicalLinearDlg dlg(this);
	dlg.DoModal();
}

void ClassicalStudy::OnBnClickedButton3_classical_Non_linear()
{
	AfxMessageBox(LoadUiStr(IDS_STR_NONLINEAR));
}
