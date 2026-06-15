
// objectRecoStudyDlg.h : header
//

#pragma once

#include "CaffeModelDialog.h"
#include "TensorflowModelStudy.h"
#include "TorchModelStudy.h"
#include "ClassicalStudy.h"
#include "ssdModelStudy.h"

class CobjectRecoStudyDlg : public CDialogEx
{
public:
	CobjectRecoStudyDlg(CWnd* pParent = NULL);

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTRECOSTUDY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedButton1_maskRcnn_image_locate();
	afx_msg void OnBnClickedButton2_locPts4();
	afx_msg void OnBnClickedButton3_objectLocateCamera();
	afx_msg void OnBnClickedButton4_localObjMobileCamera();
	afx_msg void OnBnClickedButton5_objectMaskRcnnVideo();
	afx_msg void OnBnClickedButton6_rectangleFineMobile();
	afx_msg void OnBnClickedButton7_grabcutStudy();
	afx_msg void OnBnClickedButton8_setIdCamera();
	afx_msg void OnBnClickedButton9_caffeModel();
	afx_msg void OnBnClickedButton10_tensorflowModel();
	afx_msg void OnBnClickedButton11TorchStudy();
	afx_msg void OnBnClickedButton12ClassicalStudy();
	afx_msg void OnBnClickedButton13SSD_Study();
	afx_msg void OnBnClickedButton14_oneKeyRunAll();

	void fetchFileName(std::string& nameOut, bool video);
	void logLine(const CString& line);
	void showImageFile(const std::wstring& path);

	HICON m_hIcon;
	CToolTipCtrl m_toolTip;
	int id_camera;
};

int main_studyGrabcut(std::string nameOfImage);
