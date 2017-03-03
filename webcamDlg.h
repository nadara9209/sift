// webcamDlg.h : header file
//
#include "vfw.h"
#include "afxwin.h"


#if !defined(AFX_WEBCAMDLG_H__FFE12A97_8554_42E0_A38A_5DEE11277A96__INCLUDED_)
#define AFX_WEBCAMDLG_H__FFE12A97_8554_42E0_A38A_5DEE11277A96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWebcamDlg dialog
class CWebcamDlg : public CDialog
{
// Construction
public:
	HWND m_hCamWnd;
	CString m_strErrMsg;
	CAPSTATUS m_psCapStatus;
	CAPDRIVERCAPS m_psCapsInfo;

	CWebcamDlg(CWnd* pParent = NULL);	// standard constructor
	static LRESULT CALLBACK VideoCallbackProc(HWND, LPVIDEOHDR);

// Dialog Data
	//{{AFX_DATA(CWebcamDlg)
	enum { IDD = IDD_WEBCAM_DIALOG };
	CStatic	m_Cam;
	CStatic m_Cam2;
	//CStatic m_Cam3;
	CString m_PrintObject;
	//}}AFX_DATA



	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebcamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWebcamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetting();
	afx_msg void OnCapture();
	afx_msg void OnEnChangePrintObject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnStnClickedCam3();
	afx_msg void OnStnClickedCam2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCAMDLG_H__FFE12A97_8554_42E0_A38A_5DEE11277A96__INCLUDED_)
