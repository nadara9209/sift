; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWebcamDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "webcam.h"

ClassCount=4
Class1=CWebcamApp
Class2=CWebcamDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_WEBCAM_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CCamSetting
Resource4=IDD_DIALOG_CAM_SETTING

[CLS:CWebcamApp]
Type=0
HeaderFile=webcam.h
ImplementationFile=webcam.cpp
Filter=N

[CLS:CWebcamDlg]
Type=0
HeaderFile=webcamDlg.h
ImplementationFile=webcamDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CWebcamDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=webcamDlg.h
ImplementationFile=webcamDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WEBCAM_DIALOG]
Type=1
Class=CWebcamDlg
ControlCount=5
Control1=IDC_SETTING,button,1342242816
Control2=IDC_CAPTURE,button,1342242816
Control3=IDC_CAM,static,1342177287
Control4=IDC_CAM2,static,1342177287
Control5=PrintObject,edit,1350565889

[DLG:IDD_DIALOG_CAM_SETTING]
Type=1
Class=CCamSetting
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_DRIVER_INFO,SysListView32,1350631425
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_LIST_STATUS,SysListView32,1350631425
Control7=IDC_BUTTON_REFRESH_STATUS,button,1342242816

[CLS:CCamSetting]
Type=0
HeaderFile=CamSetting.h
ImplementationFile=CamSetting.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCamSetting

