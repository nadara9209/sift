// am2View.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "webcam.h"
#include "am2View.h"


// Cam2View

IMPLEMENT_DYNCREATE(Cam2View, CFormView)

Cam2View::Cam2View()
	: CFormView(Cam2View::IDD)
{

}

Cam2View::~Cam2View()
{
}

void Cam2View::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cam2View, CFormView)
END_MESSAGE_MAP()


// Cam2View �����Դϴ�.

#ifdef _DEBUG
void Cam2View::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void Cam2View::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// Cam2View �޽��� ó�����Դϴ�.
