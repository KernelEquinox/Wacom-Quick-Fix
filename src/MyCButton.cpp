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
#include "MyCButton.h"
#include "resource.h"
#include <winsvc.h>

#define DEFAULTTEXT pParent->GetDlgItem(5002)
#define CYCLETEXT pParent->GetDlgItem(5003)
#define TOGGLETEXT pParent->GetDlgItem(5004)
#define TISENABLED pParent->GetDlgItem(5005)
#define TISDISABLED pParent->GetDlgItem(5006)
#define PATCHTEXT pParent->GetDlgItem(5007)
#define UNPATCHED pParent->GetDlgItem(5008)
#define PATCHED pParent->GetDlgItem(5009)
#define NOTPATCHED pParent->GetDlgItem(5010)
#define NOTEXIST pParent->GetDlgItem(5011)
#define SHOW ShowWindow(SW_SHOW)
#define HIDE ShowWindow(SW_HIDE)

void getTabletInputServiceStatus(CWnd* pParent);
void getPatchStatus(CWnd* pParent);


MyCButton::MyCButton()
{
}


MyCButton::~MyCButton()
{
}
BEGIN_MESSAGE_MAP(MyCButton, CBitmapButton)
	ON_WM_SETCURSOR()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnSetCursor(CWnd*, UINT, UINT)
//
//  PURPOSE: Detects when the mouse is moved within the control's coordinates.
//
//  COMMENTS:
//
//    I really wish I could implement BCN_HOTITEMCHANGE efficiently.
//
BOOL MyCButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	/* Set the main window variable. */
	CWnd* pParent = AfxGetApp()->m_pMainWnd;

	Invalidate(FALSE);

	/* Check which custom button we're over. */
	switch (GetDlgCtrlID()) {

		/* Home Button */
	case 3001:
		LoadBitmaps(IDB_HOMEBUTTONHOVER, IDB_HOMEBUTTONPRESSED);
		break;

		/* About Button */
	case 3002:
		LoadBitmaps(IDB_ABOUTBUTTONHOVER, IDB_ABOUTBUTTONPRESSED);
		break;

		/* Close Button */
	case 3003:
		LoadBitmaps(IDB_CLOSEBUTTONHOVER, IDB_CLOSEBUTTONPRESSED);
		break;
		
		/* Minimize Button */
	case 3004:
		LoadBitmaps(IDB_MINIMIZEBUTTONHOVER, IDB_MINIMIZEBUTTONPRESSED);
		break;

		/* Cycle Services Button */
	case 4001:
		LoadBitmaps(IDB_CYCLEBUTTONHOVER, IDB_CYCLEBUTTONPRESSED);
		DEFAULTTEXT->HIDE;
		TOGGLETEXT->HIDE;
		PATCHTEXT->HIDE;
		CYCLETEXT->SHOW;
		break;

		/* Toggle TIS Button */
	case 4002:
		LoadBitmaps(IDB_TOGGLEBUTTONHOVER, IDB_TOGGLEBUTTONPRESSED);
		DEFAULTTEXT->HIDE;
		CYCLETEXT->HIDE;
		PATCHTEXT->HIDE;
		TOGGLETEXT->SHOW;
		getTabletInputServiceStatus(pParent);
		break;

		/* Patch Wintab32 Button */
	case 4003:
		LoadBitmaps(IDB_PATCHBUTTONHOVER, IDB_PATCHBUTTONPRESSED);
		DEFAULTTEXT->HIDE;
		TOGGLETEXT->HIDE;
		CYCLETEXT->HIDE;
		PATCHTEXT->SHOW;
		getPatchStatus(pParent);
		break;

	default:
		break;
	}

	/* Set the cursor to a hand when hovering over a button. */
	HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
	::SetCursor(hCursor);
	return TRUE;

	return CBitmapButton::OnSetCursor(pWnd, nHitTest, message);
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnMouseLeave()
//
//  PURPOSE: Detects when the mouse leaves the control's working area.
//
//  COMMENTS:
//
//    I still really wish I could implement BCN_HOTITEMCHANGE efficiently.
//
void MyCButton::OnMouseLeave()
{
	/* Set the main window variable. */
	CWnd* pParent = AfxGetApp()->m_pMainWnd;
	
	Invalidate(FALSE);

	/* Check which custom button we're leaving. */
	switch (GetDlgCtrlID()) {
		
		/* Home Button */
	case 3001:
		LoadBitmaps(IDB_HOMEBUTTON);
		break;

		/* About Button */
	case 3002:
		LoadBitmaps(IDB_ABOUTBUTTON);
		break;

		/* Close Button */
	case 3003:
		LoadBitmaps(IDB_CLOSEBUTTON);
		break;

		/* Minimize Button */
	case 3004:
		LoadBitmaps(IDB_MINIMIZEBUTTON);
		break;

		/* Cycle Services Button */
	case 4001:
		LoadBitmaps(IDB_CYCLEBUTTON);
		CYCLETEXT->HIDE;
		DEFAULTTEXT->SHOW;
		break;

		/* Toggle TIS Button */
	case 4002:
		LoadBitmaps(IDB_TOGGLEBUTTON);
		TOGGLETEXT->HIDE;
		if (TISENABLED->IsWindowVisible())
			TISENABLED->HIDE;
		else
			TISDISABLED->HIDE;
		DEFAULTTEXT->SHOW;
		break;

		/* Patch Wintab32 Button */
	case 4003:
		LoadBitmaps(IDB_PATCHBUTTON);
		PATCHTEXT->HIDE;
		DEFAULTTEXT->SHOW;
		if (UNPATCHED->IsWindowVisible())
			UNPATCHED->HIDE;
		if (PATCHED->IsWindowVisible())
			PATCHED->HIDE;
		if (NOTPATCHED->IsWindowVisible())
			NOTPATCHED->HIDE;
		if (NOTEXIST->IsWindowVisible())
			NOTEXIST->HIDE;
		break;

	default:
		break;
	}

	CBitmapButton::OnMouseLeave();
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: OnMouseMove(UINT, CPoint)
//
//  PURPOSE: Track the cursor's coordinates.
//
//  COMMENTS:
//
//    I was worried this would take up a lot of memory, but it really doesn't.
//
void MyCButton::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = (DWORD)10000;
	TrackMouseEvent(&tme);

	CBitmapButton::OnMouseMove(nFlags, point);
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: getTabletInputServiceStatus(CWnd*)
//
//  PURPOSE: Same functionality as toggleStatus in the main dialog.
//
//  COMMENTS:
//
//    The main difference between this function and the one in the main dialog
//    (the only difference, really) is that this one simply shows a message
//    depending on whether or not TabletInputService is running or stopped.
//
void getTabletInputServiceStatus(CWnd* pParent)
{

	/* Create our necessary service handling variables. */
	SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	SC_HANDLE schService;
	SERVICE_STATUS ssStatus;
	LPCWSTR lpszServiceName = _T("TabletInputService");

	/* Open the service if it exists. */
	schService = OpenService(schSCManager, lpszServiceName, SERVICE_ALL_ACCESS);

	if (schService != NULL) {

		/* Get the service's status and store it in ssStatus. */
		QueryServiceStatus(schService, &ssStatus);

		/* Display "Enabled" if running; "Disabled" if stopped. */
		if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
			TISENABLED->SHOW;
		}
		else {
			TISDISABLED->SHOW;
		}
	}
}


///////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: getPatchStatus(CWnd*)
//
//  PURPOSE: Same functionality as patchWinTab in the main dialog.
//
//  COMMENTS:
//
//    As with the last function, the only real difference is that this does
//    nothing but shows a message based on Wintab32's status.
//
void getPatchStatus(CWnd* pParent)
{

	/* Create our necessary file handling variables. */
	CFileStatus fileStatus;
	LPCWSTR winTabPath = _T("C:\\Windows\\system32\\Wintab32.dll");
	LPCWSTR origWinTabPath = _T("C:\\Windows\\system32\\Wintab32.dll.orig");
	LPCWSTR patchWinTabPath = _T("C:\\Windows\\system32\\Wintab32.dll.patch");

	/* Wintab32 was previously patched, but has been unpatched. Re-patch it. */
	if (CFile::GetStatus(patchWinTabPath, fileStatus)) {
		UNPATCHED->SHOW;
	}

	/* Wintab32 was previously patched. Unpatch it. */
	else if (CFile::GetStatus(origWinTabPath, fileStatus)) {
		PATCHED->SHOW;
	}

	/* Wintab32 exists, rename it to "Wintab32.dll.orig". */
	else if (CFile::GetStatus(winTabPath, fileStatus)) {
		NOTPATCHED->SHOW;
	}

	/* Wacom driver isn't installed OR it's a super old Wacom driver. */
	else {
		NOTEXIST->SHOW;
	}
}