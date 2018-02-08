/*
*	Wacom Quick Fix - Fix Wacom tablet issues without sacrificing productivity.
*	Copyright(C) 2015  CryoGanix (cryoganix.tumblr.com)
*
*	This program is free software : you can redistribute it and / or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 2 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "WacomQuickFix.h"
#include "WacomQuickFixDlg.h"
#include "afxdialogex.h"
#include "Device.h"
#include <winsvc.h>

#define STATIC_STYLE WS_CHILD | WS_VISIBLE | SS_LEFT
#define STATIC_HIDDEN WS_CHILD | SS_LEFT
#define MYSTATIC_STYLE WS_CHILD | SS_LEFT | SS_NOTIFY
#define BUTTON_STYLE WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | BS_OWNERDRAW
#define WACOM_BORDER GetDlgItem(2001)
#define WACOM_SCREEN GetDlgItem(2002)
#define SHOW ShowWindow(SW_SHOW)
#define HIDE ShowWindow(SW_HIDE)

#define FILENOTFOUND 0
#define NOTPATCHED 1
#define PATCHED 2
#define UNPATCHED 3

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void cycleServices(void);
int toggleService(LPCWSTR lpszServiceName);
int patchWinTab(void);


// CWacomQuickFixDlg dialog


CWacomQuickFixDlg::CWacomQuickFixDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWacomQuickFixDlg::IDD, pParent)
{
	/* We need to work with System32 later, so prevent 64-bit machines from using SysWOW64. */
	SYSTEM_INFO lpSystemInfo;
	GetNativeSystemInfo(&lpSystemInfo);
	if (lpSystemInfo.dwProcessorType == PROCESSOR_AMD_X8664) {
		PVOID OldValue = NULL;
		Wow64DisableWow64FsRedirection(&OldValue);
	}
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWacomQuickFixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WACOM, *m_WacomLogo);
	DDX_Control(pDX, IDC_PENLIGHT, *m_PenLight);
	DDX_Control(pDX, IDC_POWERLIGHT, *m_PowerLight);
	DDX_Control(pDX, IDC_INACTIVEBUTTONS_TL, *m_UpperInactiveButtonsLeft);
	DDX_Control(pDX, IDC_INACTIVEBUTTONS_TR, *m_UpperInactiveButtonsRight);
	DDX_Control(pDX, IDC_INACTIVEBUTTONS_BL, *m_LowerInactiveButtonsLeft);
	DDX_Control(pDX, IDC_INACTIVEBUTTONS_BR, *m_LowerInactiveButtonsRight);
}

BEGIN_MESSAGE_MAP(CWacomQuickFixDlg, CDialogEx)


///////////////////////////////////////////////////////////////////////////////
// Set up event handlers:
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	/* Draggable Window */
	ON_WM_NCHITTEST()

	/* Home Button */
	ON_BN_CLICKED(3001, &CWacomQuickFixDlg::OnBnClickedHome)

	/* F.A.Q. Button */
	ON_BN_CLICKED(3002, &CWacomQuickFixDlg::OnBnClickedAbout)

	/* Close Button */
	ON_BN_CLICKED(3003, &CWacomQuickFixDlg::OnBnClickedClose)

	/* Minimize Button */
	ON_BN_CLICKED(3004, &CWacomQuickFixDlg::OnBnClickedMinimize)

	/* About Button 1 */
	ON_BN_CLICKED(6002, &CWacomQuickFixDlg::OnBnClickedAbout1)

	/* About Button 2 */
	ON_BN_CLICKED(6003, &CWacomQuickFixDlg::OnBnClickedAbout2)

	/* About Button 3 */
	ON_BN_CLICKED(6004, &CWacomQuickFixDlg::OnBnClickedAbout3)

	/* About Button 4 */
	ON_BN_CLICKED(6005, &CWacomQuickFixDlg::OnBnClickedAbout4)

	/* About Button 5 */
	ON_BN_CLICKED(6006, &CWacomQuickFixDlg::OnBnClickedAbout5)

	/* About Button 6 */
	ON_BN_CLICKED(6007, &CWacomQuickFixDlg::OnBnClickedAbout6)

	/* Back Button */
	ON_BN_CLICKED(7014, &CWacomQuickFixDlg::OnBnClickedBack)

	/* Cycle Button */
	ON_BN_CLICKED(4001, &CWacomQuickFixDlg::OnBnClickedCycle)

	/* Toggle Button */
	ON_BN_CLICKED(4002, &CWacomQuickFixDlg::OnBnClickedToggle)

	/* Patch Button */
	ON_BN_CLICKED(4003, &CWacomQuickFixDlg::OnBnClickedPatch)

	/* */
	ON_MESSAGE(DM_GETDEFID, OnGetDefID)

	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWacomQuickFixDlg message handlers

BOOL CWacomQuickFixDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ModifyStyle(0, WS_CLIPCHILDREN);


///////////////////////////////////////////////////////////////////////////////
// Make the window have rounded corners:

	/* Set the pixel size of the dialog box. */
	SetWindowPos(NULL, 0, 0, 600, 368, SWP_NOMOVE);

	CRgn rgn;
	CRect rect;
	GetWindowRect(&rect);
	rgn.CreateRoundRectRgn(0,
		0,
		rect.Width() + 1,
		rect.Height() + 1,
		23,
		23);

	/* Set the windows region */
	SetWindowRgn(static_cast<HRGN>(rgn.GetSafeHandle()), TRUE);
	SetBackgroundColor(RGB(81, 82, 86));

	/* Detach the 'CRgn' object from the region. */
	rgn.Detach();


///////////////////////////////////////////////////////////////////////////////
// Set the positions of the preloaded dialog items:

	/* Position the outer border. */
	m_WacomBorder->Create(NULL, STATIC_STYLE, CRect(43, 27, 557, 341), this, 2001);

	/* Position the inner interface area. */
	m_WacomScreen->Create(NULL, STATIC_STYLE, CRect(79, 60, 521, 308), this, 2002);

	/* Position the bevels. */
	m_UpperTopBevelLeft->Create(NULL, STATIC_STYLE, CRect(0, 27, 43, 28), this, 2003);
	m_UpperTopBevelRight->Create(NULL, STATIC_STYLE, CRect(557, 27, 600, 28), this, 2004);
	m_UpperBottomBevelLeft->Create(NULL, STATIC_STYLE, CRect(0, 28, 43, 29), this, 2005);
	m_UpperBottomBevelRight->Create(NULL, STATIC_STYLE, CRect(557, 28, 600, 29), this, 2006);
	m_LowerTopBevelLeft->Create(NULL, STATIC_STYLE, CRect(0, 340, 43, 341), this, 2007);
	m_LowerTopBevelRight->Create(NULL, STATIC_STYLE, CRect(557, 340, 600, 341), this, 2008);
	m_LowerBottomBevel->Create(NULL, STATIC_STYLE, CRect(0, 341, 600, 342), this, 2009);

	/* Position the Wacom logo. */
	m_WacomLogo->SetWindowPos(WACOM_BORDER, 274, 320, 53, 10, NULL);

	/* Position the Pen and Power lights. */
	m_PenLight->SetWindowPos(NULL, 40, 8, 9, 16, NULL);
	m_PowerLight->SetWindowPos(NULL, 53, 8, 9, 16, NULL);

	/* Position the inactive buttons. */
	m_UpperInactiveButtonsLeft->SetWindowPos(NULL, 10, 114, 24, 57, NULL);
	m_UpperInactiveButtonsRight->SetWindowPos(NULL, 566, 114, 24, 57, NULL);
	m_LowerInactiveButtonsLeft->SetWindowPos(NULL, 10, 197, 24, 57, NULL);
	m_LowerInactiveButtonsRight->SetWindowPos(NULL, 566, 197, 24, 57, NULL);


///////////////////////////////////////////////////////////////////////////////
// Set up the outer interface buttons:

	/* Home Button */
	m_HomeButton->Create(NULL, BUTTON_STYLE, CRect(10, 177, 34, 191), this, 3001);
	m_HomeButton->LoadBitmaps(IDB_HOMEBUTTON);
	m_HomeButton->SizeToContent();

	/* About Button */
	m_AboutButton->Create(NULL, BUTTON_STYLE, CRect(566, 177, 590, 191), this, 3002);
	m_AboutButton->LoadBitmaps(IDB_ABOUTBUTTON);
	m_AboutButton->SizeToContent();

	/* Close Button */
	m_CloseButton->Create(NULL, BUTTON_STYLE, CRect(536, 10, 556, 24), this, 3003);
	m_CloseButton->LoadBitmaps(IDB_CLOSEBUTTON);
	m_CloseButton->SizeToContent();

	/* Minimize Button */
	m_MinimizeButton->Create(NULL, BUTTON_STYLE, CRect(510, 10, 530, 24), this, 3004);
	m_MinimizeButton->LoadBitmaps(IDB_MINIMIZEBUTTON);
	m_MinimizeButton->SizeToContent();


///////////////////////////////////////////////////////////////////////////////
// Set up outer button tooltips:

	/* Home Button Tooltip */
	CToolTipCtrl *homeToolTip = new CToolTipCtrl;
	TOOLINFO homeToolInfo;
	homeToolInfo.cbSize = sizeof(TOOLINFO);
	homeToolInfo.lpszText = _T("Home");
	homeToolInfo.hinst = AfxGetInstanceHandle();
	homeToolInfo.hwnd = m_HomeButton->m_hWnd;
	homeToolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	homeToolInfo.uId = (UINT)m_HomeButton->m_hWnd;
	homeToolTip->Create(this);
	homeToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&homeToolInfo);

	/* About Button Tooltip */
	CToolTipCtrl *aboutToolTip = new CToolTipCtrl;
	TOOLINFO aboutToolInfo;
	aboutToolInfo.cbSize = sizeof(TOOLINFO);
	aboutToolInfo.lpszText = _T("About");
	aboutToolInfo.hinst = AfxGetInstanceHandle();
	aboutToolInfo.hwnd = m_AboutButton->m_hWnd;
	aboutToolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	aboutToolInfo.uId = (UINT)m_AboutButton->m_hWnd;
	aboutToolTip->Create(this);
	aboutToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&aboutToolInfo);

	/* Close Button Tooltip */
	CToolTipCtrl *closeToolTip = new CToolTipCtrl;
	TOOLINFO closeToolInfo;
	closeToolInfo.cbSize = sizeof(TOOLINFO);
	closeToolInfo.lpszText = _T("Close");
	closeToolInfo.hinst = AfxGetInstanceHandle();
	closeToolInfo.hwnd = m_CloseButton->m_hWnd;
	closeToolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	closeToolInfo.uId = (UINT)m_CloseButton->m_hWnd;
	closeToolTip->Create(this);
	closeToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&closeToolInfo);

	/* Minimize Button Tooltip */
	CToolTipCtrl *minimizeToolTip = new CToolTipCtrl;
	TOOLINFO minimizeToolInfo;
	minimizeToolInfo.cbSize = sizeof(TOOLINFO);
	minimizeToolInfo.lpszText = _T("Minimize");
	minimizeToolInfo.hinst = AfxGetInstanceHandle();
	minimizeToolInfo.hwnd = m_MinimizeButton->m_hWnd;
	minimizeToolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
	minimizeToolInfo.uId = (UINT)m_MinimizeButton->m_hWnd;
	minimizeToolTip->Create(this);
	minimizeToolTip->SendMessage(TTM_ADDTOOL, 0, (LPARAM)&minimizeToolInfo);


///////////////////////////////////////////////////////////////////////////////
// Set up Home screen buttons:

	/* Cycle Services Button */
	m_CycleButton->Create(NULL, BUTTON_STYLE, CRect(79, 257, 225, 308), this, 4001);
	m_CycleButton->LoadBitmaps(IDB_CYCLEBUTTON);
	m_CycleButton->SizeToContent();

	/* Toggle TIS Button */
	m_ToggleButton->Create(NULL, BUTTON_STYLE, CRect(225, 257, 375, 308), this, 4002);
	m_ToggleButton->LoadBitmaps(IDB_TOGGLEBUTTON);
	m_ToggleButton->SizeToContent();

	/* Patch Wintab32 Button */
	m_PatchButton->Create(NULL, BUTTON_STYLE, CRect(375, 257, 521, 308), this, 4003);
	m_PatchButton->LoadBitmaps(IDB_PATCHBUTTON);
	m_PatchButton->SizeToContent();


///////////////////////////////////////////////////////////////////////////////
// Set up dialog fonts:

	m_TitleFont->DeleteObject();

	/* Title Font */
	m_TitleFont->CreateFontW(
		44,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Lucida Sans Unicode")
	);

	/* Main Body Font */
	m_DefaultFont->CreateFontW(
		22,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Lucida Sans Unicode")
	);

	/* Subheader Font */
	m_SubheaderFont->CreateFontW(
		24,
		0,
		0,
		0,
		FW_BOLD,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Lucida Sans Unicode")
		);

	/* Back Font */
	m_BackFont->CreateFontW(
		17,
		0,
		0,
		0,
		FW_NORMAL,
		FALSE,
		FALSE,
		0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		_T("Lucida Sans Unicode")
		);


///////////////////////////////////////////////////////////////////////////////
// Create Home screen textual content:

	/* Home Screen Title */
	m_HomeTitleText->Create(NULL, STATIC_STYLE, CRect(93, 66, 507, 110), this, 5001);
	m_HomeTitleText->SetFont(m_TitleFont);
	m_HomeTitleText->SetWindowTextW(_T("Wacom/MP Quick Fix"));

	/* This is the text shown before hovering over anything. */
	m_DefaultText->Create(NULL, STATIC_STYLE, CRect(93, 139, 507, 227), this, 5002);
	m_DefaultText->SetFont(m_DefaultFont);
	m_DefaultText->SetWindowTextW(_T("Please choose a fix below or click on the\r\nAbout button at the right for more info."));

	/* Cycle Services Explanation */
	m_CycleText->Create(NULL, STATIC_HIDDEN, CRect(93, 139, 507, 227), this, 5003);
	m_CycleText->SetFont(m_DefaultFont);
	m_CycleText->SetWindowTextW(_T("Stop TabletInputService and all of the Wacom and Monoprice services, then immediately restart them in order."));

	/* Toggle TIS Explanation */
	m_ToggleText->Create(NULL, STATIC_HIDDEN, CRect(93, 139, 507, 227), this, 5004);
	m_ToggleText->SetFont(m_DefaultFont);
	m_ToggleText->SetWindowTextW(_T("Disables TabletInputService if currently enabled, or enables it if currently disabled."));

	/* TabletInputSerivce Status (Enabled) */
	m_ToggleStatusEnabled->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5005);
	m_ToggleStatusEnabled->SetFont(m_DefaultFont);
	m_ToggleStatusEnabled->SetWindowTextW(_T("TabletInputService Status: Enabled"));

	/* TabletInputSerivce Status (Disabled) */
	m_ToggleStatusDisabled->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5006);
	m_ToggleStatusDisabled->SetFont(m_DefaultFont);
	m_ToggleStatusDisabled->SetWindowTextW(_T("TabletInputService Status: Disabled"));

	/* Patch Wintab32 Explanation */
	m_PatchText->Create(NULL, STATIC_HIDDEN, CRect(93, 139, 507, 227), this, 5007);
	m_PatchText->SetFont(m_DefaultFont);
	m_PatchText->SetWindowTextW(_T("Patches Wintab32.dll with the most up-to-date version and backs up the old version. You can also use this fix to unpatch Wintab32."));

	/* Patch Status (Unpatched) */
	m_PatchStatusUnpatched->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5008);
	m_PatchStatusUnpatched->SetFont(m_DefaultFont);
	m_PatchStatusUnpatched->SetWindowTextW(_T("Wintab32.dll Status: Unpatched"));

	/* Patch Status (Patched) */
	m_PatchStatusPatched->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5009);
	m_PatchStatusPatched->SetFont(m_DefaultFont);
	m_PatchStatusPatched->SetWindowTextW(_T("Wintab32.dll Status: Patched"));

	/* Patch Status (Not Patched) */
	m_PatchStatusNotPatched->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5010);
	m_PatchStatusNotPatched->SetFont(m_DefaultFont);
	m_PatchStatusNotPatched->SetWindowTextW(_T("Wintab32.dll Status: Not Patched"));

	/* Patch Status (Not Exist) */
	m_PatchStatusNotExist->Create(NULL, STATIC_HIDDEN, CRect(93, 227, 507, 249), this, 5011);
	m_PatchStatusNotExist->SetFont(m_DefaultFont);
	m_PatchStatusNotExist->SetWindowTextW(_T("Wintab32.dll Status: File Not Found"));


///////////////////////////////////////////////////////////////////////////////
// Create About screen textual content:

	/* About Screen Title */
	m_AboutTitleText->Create(NULL, STATIC_HIDDEN, CRect(93, 66, 507, 110), this, 6001);
	m_AboutTitleText->SetFont(m_TitleFont);
	m_AboutTitleText->SetWindowTextW(_T("About This Program"));

	/* About Entry 1 */
	m_AboutText1->Create(NULL, MYSTATIC_STYLE, CRect(110, 124, 315, 154), this, 6002);
	m_AboutText1->SetFont(m_DefaultFont);
	m_AboutText1->SetWindowTextW(_T("What does this tool do?"));

	/* About Entry 2 */
	m_AboutText2->Create(NULL, MYSTATIC_STYLE, CRect(110, 154, 393, 184), this, 6003);
	m_AboutText2->SetFont(m_DefaultFont);
	m_AboutText2->SetWindowTextW(_T("Why do Wacom drivers mess up?"));

	/* About Entry 3 */
	m_AboutText3->Create(NULL, MYSTATIC_STYLE, CRect(110, 184, 433, 214), this, 6004);
	m_AboutText3->SetFont(m_DefaultFont);
	m_AboutText3->SetWindowTextW(_T("How many devices does this tool fix?"));

	/* About Entry 4 */
	m_AboutText4->Create(NULL, MYSTATIC_STYLE, CRect(110, 214, 355, 244), this, 6005);
	m_AboutText4->SetFont(m_DefaultFont);
	m_AboutText4->SetWindowTextW(_T("Why not make a new driver?"));

	/* About Entry 5 */
	m_AboutText5->Create(NULL, MYSTATIC_STYLE, CRect(110, 244, 437, 274), this, 6006);
	m_AboutText5->SetFont(m_DefaultFont);
	m_AboutText5->SetWindowTextW(_T("What is TabletInputService/Wintab32?"));

	/* About Entry 6 */
	m_AboutText6->Create(NULL, MYSTATIC_STYLE, CRect(110, 274, 449, 296), this, 6007);
	m_AboutText6->SetFont(m_DefaultFont);
	m_AboutText6->SetWindowTextW(_T("Where's the licensing and contact info?"));

	/* About Arrow */
	m_AboutArrow->Create(NULL, MYSTATIC_STYLE, CRect(93, 122, 108, 144), this, 6008);
	m_AboutArrow->SetFont(m_DefaultFont);
	m_AboutArrow->SetWindowTextW(_T("»"));


///////////////////////////////////////////////////////////////////////////////
// Set up all questions and answers:

	/* Question Screen Title */
	m_QuestionTitleText->Create(NULL, STATIC_HIDDEN, CRect(93, 66, 507, 110), this, 7001);
	m_QuestionTitleText->SetFont(m_TitleFont);
	m_QuestionTitleText->SetWindowTextW(_T("Question:"));

	/* Question 1 */
	m_QuestionText1->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7002);
	m_QuestionText1->SetFont(m_SubheaderFont);
	m_QuestionText1->SetWindowTextW(_T("What does this tool do?"));

	/* Answer 1 */
	m_AnswerText1->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7003);
	m_AnswerText1->SetFont(m_DefaultFont);
	m_AnswerText1->SetWindowTextW(_T("This tool aims to remedy any and all issues that might occur when using a Wacom-brand tablet or Monoprice-brand tablet. This isn't meant to be a permanent fix; it's only meant to be used to get your tablet back in working condition in as little time as possible."));

	/* Question 2 */
	m_QuestionText2->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7004);
	m_QuestionText2->SetFont(m_SubheaderFont);
	m_QuestionText2->SetWindowTextW(_T("Why do Wacom drivers mess up?"));

	/* Answer 2 */
	m_AnswerText2->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7005);
	m_AnswerText2->SetFont(m_DefaultFont);
	m_AnswerText2->SetWindowTextW(_T("There's no definitive answer to that for all cases, but Windows' TabletInputService seems to be the culprit most of the time."));

	/* Question 3 */
	m_QuestionText3->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7006);
	m_QuestionText3->SetFont(m_SubheaderFont);
	m_QuestionText3->SetWindowTextW(_T("How many devices does this tool fix?"));

	/* Answer 3 */
	m_AnswerText3->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7007);
	m_AnswerText3->SetFont(m_DefaultFont);
	m_AnswerText3->SetWindowTextW(_T("This tool works on most Wacom tablets and was modified to work on certain Monoprice tablets."));

	/* Question 4 */
	m_QuestionText4->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7008);
	m_QuestionText4->SetFont(m_SubheaderFont);
	m_QuestionText4->SetWindowTextW(_T("Why not make a new driver?"));

	/* Answer 4 */
	m_AnswerText4->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7009);
	m_AnswerText4->SetFont(m_DefaultFont);
	m_AnswerText4->SetWindowTextW(_T("Unfortunately, it’s not that easy. Wacom is very protective of the information required to interface directly with the tablet, and the ones who have that information have signed NDA's. Overall, fixing issues as quickly as possible is preferable to creating a custom tablet driver."));

	/* Question 5 */
	m_QuestionText5->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7010);
	m_QuestionText5->SetFont(m_SubheaderFont);
	m_QuestionText5->SetWindowTextW(_T("What is TabletInputService/Wintab32?"));

	/* Answer 5 */
	m_AnswerText5->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7011);
	m_AnswerText5->SetFont(m_DefaultFont);
	m_AnswerText5->SetWindowTextW(_T("TabletInputService is a Windows service used mainly for Flicks, Gestures, and Handwriting. Wintab32 is a handler for all sorts of pointing devices and is what all modern Wacom devices interface with. These two handlers (very) often conflict, hence the creation of this tool."));

	/* Question 6 */
	m_QuestionText6->Create(NULL, STATIC_HIDDEN, CRect(93, 114, 507, 137), this, 7012);
	m_QuestionText6->SetFont(m_SubheaderFont);
	m_QuestionText6->SetWindowTextW(_T("Where's the licensing and contact info?"));

	/* Answer 6 */
	m_AnswerText6->Create(NULL, STATIC_HIDDEN, CRect(93, 163, 507, 295), this, 7013);
	m_AnswerText6->SetFont(m_DefaultFont);
	m_AnswerText6->SetWindowTextW(_T("This program is now open source and licensed under the GNU General Public License (GPLv2).\r\n\r\nIf you have any additional questions, feel free to contact me at cryoganix@gmail.com! =)"));

	/* Back */
	m_BackText->Create(NULL, MYSTATIC_STYLE, CRect(475, 71, 508, 88), this, 7014);
	m_BackText->SetFont(m_BackFont);
	m_BackText->SetWindowTextW(_T("Back"));

	/* Copyright */
	m_CopyrightText->Create(NULL, STATIC_HIDDEN, CRect(205, 280, 395, 297), this, 7015);
	m_CopyrightText->SetFont(m_BackFont);
	m_CopyrightText->SetWindowTextW(_T("© CryoGanix 2015-2018"));


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWacomQuickFixDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWacomQuickFixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnNcHitTest(CPoint point)
//
//  PURPOSE: Makes the window universally draggable.
//
//  COMMENTS:
//
//    Since the frame is removed from the dialog box, we need some way to drag
//    the window around. This function allows you to drag the window around by
//    clicking and holding any part of the window. I only meant to make the
//    outer portion the draggable handle, but this works just fine.
//
LRESULT CWacomQuickFixDlg::OnNcHitTest(CPoint point)
{
	UINT ret = CDialog::OnNcHitTest(point);
	if (ret == HTCLIENT) {
		return HTCAPTION;
	}
	return CDialogEx::OnNcHitTest(point);
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedHome()
//
//  PURPOSE: Displays the Home part of the dialog.
//
//  COMMENTS:
//
//    Remove everything from the dialog and place the Home elements.
//
void CWacomQuickFixDlg::OnBnClickedHome()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Hide the Question entry elements. */
	for (i = 7001; i <= 7015; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the Home screen buttons. */
	for (i = 4001; i <= 4003; ++i)
		if (!GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->SHOW;

	/* Hide the Home screen text. */
	for (i = 5001; i <= 5002; ++i)
		if (!GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout()
//
//  PURPOSE: Displays the About part of the dialog.
//
//  COMMENTS:
//
//    Remove everything from the dialog and place the About elements.
//
void CWacomQuickFixDlg::OnBnClickedAbout()
{
	// TODO: Add your control notification handler code here

	int i;

	/* Hide the Home screen buttons. */
	for (i = 4001; i <= 4003; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Hide the Home screen text. */
	for (i = 5001; i <= 5005; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Hide the Question entry elements. */
	for (i = 7001; i <= 7015; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the About screen elements. */
	for (i = 6001; i <= 6007; ++i)
		if (!GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->SHOW;

}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedClose()
//
//  PURPOSE: Allows the window to be closed.
//
//  COMMENTS:
//
//    By removing the frame from the dialog box, it also removed the title bar,
//    meaning we need to manually draw the close button and make a function
//    that closes the window.
//
void CWacomQuickFixDlg::OnBnClickedClose()
{
	CWnd::DestroyWindow();
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedMinimize()
//
//  PURPOSE: Allows the window to be minimized.
//
//  COMMENTS:
//
//    Same as above, except with the minimize button.
//
void CWacomQuickFixDlg::OnBnClickedMinimize()
{
	CWnd::ShowWindow(SW_MINIMIZE);
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnCtlColor(CDC*, CWnd*, UINT)
//
//  PURPOSE: Colors certain controls as they're drawn.
//
//  COMMENTS:
//
//    Mostly used to set up the main interface and speech bubbles. Also makes
//    the About text backgrounds transparent.
//
HBRUSH CWacomQuickFixDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	hbr = (HBRUSH)::GetStockObject(DC_BRUSH);

	switch (pWnd->GetDlgCtrlID()) {

	/* Wacom Border */
	case 2001:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(16, 17, 21));
		break;

	/* Wacom Screen */
	case 2002:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(33, 33, 41));
		break;

	/* Upper Top Bevel */
	case 2003: case 2004:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(57, 58, 62));
		break;

	/* Upper Bottom Bevel */
	case 2005: case 2006:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(93, 94, 98));
		break;

	/* Lower Top Bevel */
	case 2007: case 2008:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(57, 58, 62));
		break;

	/* Lower Bottom Bevel */
	case 2009:
		SetDCBrushColor(pDC->GetSafeHdc(), RGB(105, 106, 110));
		break;

	/* Default is a transparent background. */
	default:
		hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
		break;
	}

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 255, 255));

	return hbr;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout1()
//
//  PURPOSE: Displays the Question 1 dialog.
//
//  COMMENTS:
//
//    Question text: "What does this tool do?"
//
void CWacomQuickFixDlg::OnBnClickedAbout1()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText1->SHOW;
	m_AnswerText1->SHOW;
	m_BackText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout2()
//
//  PURPOSE: Displays the Question 2 dialog.
//
//  COMMENTS:
//
//    Question text: "Why do Wacom drivers mess up?"
//
void CWacomQuickFixDlg::OnBnClickedAbout2()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText2->SHOW;
	m_AnswerText2->SHOW;
	m_BackText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout3()
//
//  PURPOSE: Displays the Question 3 dialog.
//
//  COMMENTS:
//
//    Question text: "How many devices does this tool fix?"
//
void CWacomQuickFixDlg::OnBnClickedAbout3()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText3->SHOW;
	m_AnswerText3->SHOW;
	m_BackText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout4()
//
//  PURPOSE: Displays the Question 4 dialog.
//
//  COMMENTS:
//
//    Question text: "Why not make a new driver?"
//
void CWacomQuickFixDlg::OnBnClickedAbout4()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText4->SHOW;
	m_AnswerText4->SHOW;
	m_BackText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout5()
//
//  PURPOSE: Displays the Question 5 dialog.
//
//  COMMENTS:
//
//    Question text: "What is TabletInputService/Wintab32?"
//
void CWacomQuickFixDlg::OnBnClickedAbout5()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText5->SHOW;
	m_AnswerText5->SHOW;
	m_BackText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedAbout6()
//
//  PURPOSE: Displays the Question 6 dialog.
//
//  COMMENTS:
//
//    Question text: "Where's the licensing and contact info?"
//
//    And yes, this was just an excuse to place contact info and to make known
//    its open source status. People don't trust programs running with admin
//    rights without knowing what they do, y'know?
//
void CWacomQuickFixDlg::OnBnClickedAbout6()
{
	int i;

	/* Hide the About screen elements. */
	for (i = 6001; i <= 6008; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the specific Question entry elements. */
	m_QuestionTitleText->SHOW;
	m_QuestionText6->SHOW;
	m_AnswerText6->SHOW;
	m_BackText->SHOW;
	m_CopyrightText->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedBack()
//
//  PURPOSE: Goes back to the About screen.
//
//  COMMENTS:
//
//    No other cases are needed here; it only shows up in the Question dialog.
//
void CWacomQuickFixDlg::OnBnClickedBack()
{
	int i;

	/* Hide all Question entry elements. */
	for (i = 7001; i <= 7015; ++i)
		if (GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->HIDE;

	/* Show the About screen elements. */
	for (i = 6001; i <= 6007; ++i)
		if (!GetDlgItem(i)->IsWindowVisible())
			GetDlgItem(i)->SHOW;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedCycle()
//
//  PURPOSE: Kickstarts the service cycling routines.
//
//  COMMENTS:
//
//    Makes sure nothing else is clickable while the services cycle, then
//    display a status message box.
//
void CWacomQuickFixDlg::OnBnClickedCycle()
{
	/* Prevent the user from doing crazy things during execution. */
	CWaitCursor waitCursor;

	/* Cycle those services. */
	cycleServices();

	/* The services will never not cycle successfully. */
	MessageBoxW(_T("Services cycled successfully!"), _T("Success"), MB_ICONINFORMATION);

	/* Cycle hardware this time around. */
	PWSTR args[1];
	/* Check for Wacom tablets. */
	args[0] = (PWSTR)_T("USB\\VID_056A*");
	int wacomdev = cmdFind((LPCTSTR)_T("none"), NULL, 0, 1, args);
	if (wacomdev) {
		cmdRestart((LPCTSTR)_T("none"), NULL, 0, 1, args);
	}
	/* Check for Monoprice tablets. */
	args[0] = (PWSTR)_T("USB\\VID_0B57*");
	int mpdev = cmdFind((LPCTSTR)_T("none"), NULL, 0, 1, args);
	if (mpdev) {
		cmdRestart((LPCTSTR)_T("none"), NULL, 0, 1, args);
	}
	if (!wacomdev && !mpdev) {
		MessageBoxW((LPCTSTR)_T("Did not detect a connected tablet.\r\n\r\nSkipping hardware reconnection."), _T("Success"), MB_ICONINFORMATION);
	}
	else {
		MessageBoxW((LPCTSTR)_T("All tablet systems reconnected!"), _T("Success"), MB_ICONINFORMATION);
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedToggle()
//
//  PURPOSE: Kickstarts the TabletInputService toggling routines.
//
//  COMMENTS:
//
//    Makes sure nothing else is clickable while TabletInputService is cycled,
//    then display a status message box.
//
void CWacomQuickFixDlg::OnBnClickedToggle()
{
	/* Prevent the user from doing crazy things during execution. */
	CWaitCursor waitCursor;

	/* Get the service's new status and stick it in a variable. */
	int tabletInputServiceStatus = toggleService(_T("TabletInputService"));

	/* Was stopped, so we started it. */
	if (tabletInputServiceStatus == SERVICE_RUNNING) {
		MessageBoxW(_T("Started TabletInputService"), _T("Success"), MB_ICONINFORMATION);
	}

	/* Was running, so we stopped it. */
	else if (tabletInputServiceStatus == SERVICE_STOPPED) {
		MessageBoxW(_T("Stopped TabletInputService"), _T("Success"), MB_ICONINFORMATION);
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnBnClickedPatch()
//
//  PURPOSE: Kickstarts the Wintab32 patching routines.
//
//  COMMENTS:
//
//    Makes sure nothing else is clickable while the patching process starts,
//    then display a status message box.
//
void CWacomQuickFixDlg::OnBnClickedPatch()
{
	/* Prevent the user from doing crazy things during execution. */
	CWaitCursor waitCursor;

	/* Get the patching status and stick it in a variable. */
	int patchStatus = patchWinTab();

	switch (patchStatus)
	{

		/* Previously unpatched file has been re-patched. */
	case UNPATCHED:
		MessageBoxW(_T("Wintab32.dll has been re-patched!"), _T("Success"), MB_ICONINFORMATION);
		break;

		/* Patched file has been unpatched. */
	case PATCHED:
		MessageBoxW(_T("Wintab32.dll has been unpatched!"), _T("Success"), MB_ICONINFORMATION);
		break;

		/* Unpatched file has been patched. */
	case NOTPATCHED:
		MessageBoxW(_T("Successfully patched Wintab32.dll!"), _T("Success"), MB_ICONINFORMATION);
		break;

		/* File doesn't exist. */
	case FILENOTFOUND:
		MessageBoxW(_T("Wintab32.dll was not found on this system!"), _T("Error"), MB_ICONEXCLAMATION);
		break;
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: cycleServices()
//
//  PURPOSE: Go through each tablet service and toggle it twice.
//
//  COMMENTS:
//
//    The list of services are from Wacom's own driver installation files, plus
//    Windows' own TabletInputService. This function goes through all of the
//    services that affect Wacom tablets and toggles their activated state.
//
//    Experimental support for the Monoprice 22" (Parblo Coast22) tablet was
//    added on 02/05/2018.
//
void cycleServices()
{
	/* Create array of service names. */
	LPCWSTR wacomServiceList[11];
	wacomServiceList[0] = _T("TabletInputService");     // Lowest priority
	wacomServiceList[1] = _T("TabletService");
	wacomServiceList[2] = _T("TabletServicePen");
	wacomServiceList[3] = _T("TabletServiceWacom");
	wacomServiceList[4] = _T("TabletServiceISD");
	wacomServiceList[5] = _T("TouchServicePen");
	wacomServiceList[6] = _T("TouchServiceWacom");
	wacomServiceList[7] = _T("TouchServiceISD");
	wacomServiceList[8] = _T("WTabletServicePro");
	wacomServiceList[9] = _T("WTabletServiceCon");
	wacomServiceList[10] = _T("WTabletServiceISD");
	wacomServiceList[10] = _T("HWSuperPowerTablet");    // Highest priority

	/* Cycle each service's running state. */
	for each (LPCWSTR wacomService in wacomServiceList) {
		toggleService(wacomService);
		toggleService(wacomService);
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: toggleService(LPCWSTR)
//
//  PURPOSE: Stop the service if it's running, start it if it's stopped.
//
//  COMMENTS:
//
//    Added a label for quick re-evaluation. I know it may be frowned upon,
//    but if it works in ASM, why not try in C++?
//
int toggleService(LPCWSTR lpszServiceName)
{

// Aw yeah, dat label.
reEvaluateMyLife:

	/* Create our necessary service handling variables. */
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE schService;
	SERVICE_STATUS ssStatus;

	/* Open the service if it exists. */
	schService = OpenService(schSCManager, lpszServiceName, SERVICE_ALL_ACCESS);

	if (schService != NULL) {

		/* Get the service's status and store it in ssStatus. */
		QueryServiceStatus(schService, &ssStatus);

		/* Test what the service's running state is. */
		switch (ssStatus.dwCurrentState) {

			/* Start if stopped. */
		case SERVICE_STOPPED:
			StartService(schService, 0, NULL);
			//OutputDebugString(lpszServiceName);
			//OutputDebugString(_T(" started.\n"));
			return SERVICE_RUNNING;
			break;

			/* Stop if running. */
		case SERVICE_RUNNING:
			ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus);
			//OutputDebugString(lpszServiceName);
			//OutputDebugString(_T(" stopped.\n"));
			return SERVICE_STOPPED;
			break;

			/* Wait for 0.1 seconds if the service is still stopping or starting, then re-evaluate. */
		case SERVICE_START_PENDING: case SERVICE_STOP_PENDING: case SERVICE_CONTINUE_PENDING: case SERVICE_PAUSE_PENDING:
			//OutputDebugString(_T("Waiting..."));
			Sleep(100);
			goto reEvaluateMyLife;
			break;

			/* The service is paused. Stop it, then re-evaluate. */
		default:
			ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus);
			Sleep(100);
			goto reEvaluateMyLife;
			break;
		}
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: patchWinTab()
//
//  PURPOSE: Updates the user's WinTab32.dll.
//
//  COMMENTS:
//
//    EXPERIMENTAL!
//
//    If a Wacom driver is already installed, it renames the original WinTab32
//    to "Wintab32.dll.orig" and installs the new Wintab32. It also checks
//    whether or not Wintab32 has already been updated, and simply swaps the
//    old and new files as needed to patch/unpatch. Does nothing if no Wacom
//    driver is installed.
//
//    This currently only utilizes the Wacom version of Wintab32.dll; it does
//    not use the Monoprice version of Wintab32.dll.
//    
int patchWinTab()
{
	/* Grab our handy dandy Wintab32 file. */
	HRSRC winTabResource = FindResource(NULL, MAKEINTRESOURCE(IDR_WINTAB32), _T("File"));
	unsigned int winTabSize = SizeofResource(NULL, winTabResource);
	HGLOBAL winTabLoad = LoadResource(NULL, winTabResource);
	void* winTabData = LockResource(winTabLoad);

	/* Create our necessary file handling variables. */
	CFile winTab32File;
	CFileStatus fileStatus;
	LPCWSTR winTabPath = _T("C:\\Windows\\system32\\Wintab32.dll");
	LPCWSTR origWinTabPath = _T("C:\\Windows\\system32\\Wintab32.dll.orig");
	LPCWSTR patchWinTabPath = _T("C:\\Windows\\system32\\Wintab32.dll.patch");

	/* Wintab32 was previously patched, but has been unpatched. Re-patch it. */
	if (CFile::GetStatus(patchWinTabPath, fileStatus)) {
		CFile::Rename(winTabPath, origWinTabPath);
		CFile::Rename(patchWinTabPath, winTabPath);
		return UNPATCHED;
	}

	/* Wintab32 was previously patched. Unpatch it. */
	else if (CFile::GetStatus(origWinTabPath, fileStatus)) {
		CFile::Rename(winTabPath, patchWinTabPath);
		CFile::Rename(origWinTabPath, winTabPath);
		return PATCHED;
	}

	/* Wintab32 exists, rename it to "Wintab32.dll.orig". */
	else if (CFile::GetStatus(winTabPath, fileStatus)) {
			CFile::Rename(winTabPath, origWinTabPath);

		/* Patch Wintab32. */
		winTab32File.Open(winTabPath, CFile::modeReadWrite | CFile::shareExclusive | CFile::modeCreate);
		winTab32File.Write(winTabData, winTabSize);
		winTab32File.Flush();
		return NOTPATCHED;
	}

	/* Wacom driver isn't installed OR it's a super old Wacom driver. */
	else {
		return FILENOTFOUND;
	}
	return 0;
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnGetDefID(WPARAM, LPARAM)
//
//  PURPOSE: Disable that Enter key.
//
//  COMMENTS:
//
//    N/A
//    
LRESULT CWacomQuickFixDlg::OnGetDefID(WPARAM wp, LPARAM lp)
{
	return MAKELONG(0, DC_HASDEFID);
}