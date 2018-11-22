; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVQCopressView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VQCopress.h"
LastPage=0

ClassCount=6
Class1=CVQCopressApp
Class2=CVQCopressDoc
Class3=CVQCopressView
Class4=CMainFrame

ResourceCount=6
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CDxWindow
Resource3=IDD_VQCOPRESS_FORM
Resource4=IDR_MAINFRAME (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_VQCOPRESS_FORM (English (U.S.))

[CLS:CVQCopressApp]
Type=0
HeaderFile=VQCopress.h
ImplementationFile=VQCopress.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CVQCopressDoc]
Type=0
HeaderFile=VQCopressDoc.h
ImplementationFile=VQCopressDoc.cpp
Filter=N

[CLS:CVQCopressView]
Type=0
HeaderFile=VQCopressView.h
ImplementationFile=VQCopressView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=IDC_BROWSE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T




[CLS:CAboutDlg]
Type=0
HeaderFile=VQCopress.cpp
ImplementationFile=VQCopress.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_VQCOPRESS_FORM]
Type=1
Class=CVQCopressView
ControlCount=26
Control1=IDC_MESSAGES,edit,1352728708
Control2=IDC_FILENAME_BMP,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BROWSE,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CBSIZE,edit,1350631552
Control7=IDC_GO,button,1342242816
Control8=IDC_DX_PALETTE,static,1342177287
Control9=IDC_DX_SOURCE,static,1342177287
Control10=IDC_DX_DEST,static,1342177287
Control11=IDC_DX_PALETTE2,static,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_DX_SOURCE2,static,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_DX_DEST2,static,1342177287
Control16=IDC_STATIC,static,1342308352
Control17=IDC_CBSIZE2,edit,1350631552
Control18=IDC_GO2,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_BLOCKW,edit,1350631552
Control22=IDC_BLOCKH,edit,1350631552
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_CBSIZE3,edit,1350631552
Control26=IDC_SAVE,button,1342242816

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[CLS:CDxWindow]
Type=0
HeaderFile=DxWindow.h
ImplementationFile=DxWindow.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_VQCOPRESS_FORM (English (U.S.))]
Type=1
Class=CVQCopressView
ControlCount=26
Control1=IDC_MESSAGES,edit,1352728708
Control2=IDC_FILENAME_BMP,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_BROWSE,button,1342242816
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CBSIZE,edit,1350631552
Control7=IDC_GO,button,1342242816
Control8=IDC_DX_PALETTE,static,1342177287
Control9=IDC_DX_SOURCE,static,1342177287
Control10=IDC_DX_DEST,static,1342177287
Control11=IDC_DX_PALETTE2,static,1342177287
Control12=IDC_STATIC,static,1342308352
Control13=IDC_DX_SOURCE2,static,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_DX_DEST2,static,1342177287
Control16=IDC_STATIC,static,1342308352
Control17=IDC_CBSIZE2,edit,1350631552
Control18=IDC_GO2,button,1342242816
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,static,1342308352
Control21=IDC_BLOCKW,edit,1350631552
Control22=IDC_BLOCKH,edit,1350631552
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_CBSIZE3,edit,1350631552
Control26=IDC_SAVE,button,1342242816

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

