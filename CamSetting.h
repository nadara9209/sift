#if !defined(AFX_CAMSETTING_H__D843FC54_1BF1_48CF_B7C3_547412335757__INCLUDED_)
#define AFX_CAMSETTING_H__D843FC54_1BF1_48CF_B7C3_547412335757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vfw.h"

// CamSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamSetting dialog

class CCamSetting : public CDialog
{
// Construction
public:
	CCamSetting(CWnd* pParent = NULL);   // standard constructor
	HWND m_hCamWnd;
	CAPDRIVERCAPS m_psCapsInfo;
	CAPSTATUS m_psCapStatus;

// Dialog Data
	//{{AFX_DATA(CCamSetting)
	enum { IDD = IDD_DIALOG_CAM_SETTING };
	CListCtrl	m_listStatus;
	CListCtrl	m_listDriverInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRefreshStatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMSETTING_H__D843FC54_1BF1_48CF_B7C3_547412335757__INCLUDED_)
