#pragma once



// Cam2View 폼 뷰입니다.

class Cam2View : public CFormView
{
	DECLARE_DYNCREATE(Cam2View)

protected:
	Cam2View();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~Cam2View();

public:
	enum { IDD = IDD_WEBCAM_DIALOG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


