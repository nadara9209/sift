// CamSetting.cpp : implementation file
//

#include "stdafx.h"
#include "webcam.h"
#include "CamSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamSetting dialog


CCamSetting::CCamSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CCamSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCamSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamSetting)
	DDX_Control(pDX, IDC_LIST_STATUS, m_listStatus);
	DDX_Control(pDX, IDC_LIST_DRIVER_INFO, m_listDriverInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamSetting, CDialog)
	//{{AFX_MSG_MAP(CCamSetting)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH_STATUS, OnButtonRefreshStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamSetting message handlers

BOOL CCamSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	char *szListTitle[10]={"설정 값","내용",""};
	int nListColumnWidth[10]={200, 150, 50, 50, 50, 50, 50, 50, 50, 50};
	int i=0;
	int nItem;
	CString tmpMsg;
	LV_COLUMN lvcolumn;				//COLUMN 선언

	// 드라이버 정보 리스트
	m_listDriverInfo.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT );
	while(strlen(szListTitle[i]) != 0 )
	{
		lvcolumn.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt=LVCFMT_LEFT;
		lvcolumn.pszText=szListTitle[i];
		lvcolumn.iSubItem=i;
		lvcolumn.cx=nListColumnWidth[i];
		m_listDriverInfo.InsertColumn(i,&lvcolumn);
		i++;
	}

	capDriverGetCaps(m_hCamWnd, &m_psCapsInfo, sizeof(m_psCapsInfo));

	tmpMsg.Format("%d", m_psCapsInfo.wDeviceIndex);
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "연결된 디바이스 번호(0~9)");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fCaptureInitialized?"양호":"불량";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "디바이스 연결상태");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fDriverSuppliesPalettes?"가능":"불가";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "팔레트 생성");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoDisplay?"가능":"불가";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "Redisplay 지원");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoFormat?"가능":"불가";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "비디오 포맷 변환 다이얼로그");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoSource?"가능":"불가";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "비디오 소스 제어 다이얼로그");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasOverlay?"가능":"불가";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "오버레이 지원");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	
	// 드라이버 상태 리스트
	i=0;
	m_listStatus.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT );
	while(strlen(szListTitle[i]) != 0 )
	{
		lvcolumn.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt=LVCFMT_LEFT;
		lvcolumn.pszText=szListTitle[i];
		lvcolumn.iSubItem=i;
		lvcolumn.cx=nListColumnWidth[i];
		m_listStatus.InsertColumn(i,&lvcolumn);
		i++;
	}

	OnButtonRefreshStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCamSetting::OnButtonRefreshStatus() 
{
	// TODO: Add your control notification handler code here
	CString tmpMsg;
	int nItem;
	m_listStatus.DeleteAllItems();
	capGetStatus(m_hCamWnd, &m_psCapStatus, sizeof(m_psCapStatus));

	tmpMsg.Format("%d pixels", m_psCapStatus.uiImageHeight);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "높이");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d pixels", m_psCapStatus.uiImageWidth);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "넓이");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fLiveWindow?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Preview 사용");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fOverlayWindow?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Hardware Overlay 사용");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fScale?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Scale 변환");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d ms", m_psCapStatus.dwCurrentTimeElapsedMS);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "캡쳐 시간(ms)");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d frame", m_psCapStatus.dwCurrentVideoFrame);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "캡쳐된 총 프레임 수");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d frame", m_psCapStatus.dwCurrentVideoFramesDropped);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "드랍된 프레임 수");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d sample", m_psCapStatus.dwCurrentWaveSamples);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "오디오 셈플 수");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.dwReturn);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "보고된 에러");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fAudioHardware?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Waveform-audio 하드웨어 설치");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fCapFileExists?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "캡쳐된 사진 있음");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fCapturingNow?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "캡쳐 진행 여부");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fUsingDefaultPalette?"예":"아니오";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "기본 팔레트 사용");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.hPalCurrent );
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "현재 사용되는 팔레트 헨들");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.ptScroll );
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "좌상단 오프셋");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.wNumAudioAllocated);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "allocated된 오디오 버퍼 수");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.wNumVideoAllocated);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "allocated된 비디오 버퍼 수");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);	
}
