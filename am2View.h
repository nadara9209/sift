#pragma once



// Cam2View �� ���Դϴ�.

class Cam2View : public CFormView
{
	DECLARE_DYNCREATE(Cam2View)

protected:
	Cam2View();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


