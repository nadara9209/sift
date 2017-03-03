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
	
	char *szListTitle[10]={"���� ��","����",""};
	int nListColumnWidth[10]={200, 150, 50, 50, 50, 50, 50, 50, 50, 50};
	int i=0;
	int nItem;
	CString tmpMsg;
	LV_COLUMN lvcolumn;				//COLUMN ����

	// ����̹� ���� ����Ʈ
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
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "����� ����̽� ��ȣ(0~9)");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fCaptureInitialized?"��ȣ":"�ҷ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "����̽� �������");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fDriverSuppliesPalettes?"����":"�Ұ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "�ȷ�Ʈ ����");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoDisplay?"����":"�Ұ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "Redisplay ����");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoFormat?"����":"�Ұ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "���� ���� ��ȯ ���̾�α�");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasDlgVideoSource?"����":"�Ұ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "���� �ҽ� ���� ���̾�α�");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapsInfo.fHasOverlay?"����":"�Ұ�";
	nItem = m_listDriverInfo.InsertItem(m_listDriverInfo.GetItemCount(), "�������� ����");
	m_listDriverInfo.SetItemText(nItem, 1, tmpMsg);

	
	// ����̹� ���� ����Ʈ
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
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "����");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d pixels", m_psCapStatus.uiImageWidth);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "����");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fLiveWindow?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Preview ���");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fOverlayWindow?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Hardware Overlay ���");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fScale?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Scale ��ȯ");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d ms", m_psCapStatus.dwCurrentTimeElapsedMS);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "ĸ�� �ð�(ms)");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d frame", m_psCapStatus.dwCurrentVideoFrame);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "ĸ�ĵ� �� ������ ��");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d frame", m_psCapStatus.dwCurrentVideoFramesDropped);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "����� ������ ��");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d sample", m_psCapStatus.dwCurrentWaveSamples);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "����� ���� ��");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.dwReturn);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "����� ����");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fAudioHardware?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "Waveform-audio �ϵ���� ��ġ");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fCapFileExists?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "ĸ�ĵ� ���� ����");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fCapturingNow?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "ĸ�� ���� ����");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg = m_psCapStatus.fUsingDefaultPalette?"��":"�ƴϿ�";
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "�⺻ �ȷ�Ʈ ���");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.hPalCurrent );
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "���� ���Ǵ� �ȷ�Ʈ ���");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.ptScroll );
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "�»�� ������");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.wNumAudioAllocated);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "allocated�� ����� ���� ��");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);

	tmpMsg.Format("%d", m_psCapStatus.wNumVideoAllocated);
	nItem = m_listStatus.InsertItem(m_listStatus.GetItemCount(), "allocated�� ���� ���� ��");
	m_listStatus.SetItemText(nItem, 1, tmpMsg);	
}
