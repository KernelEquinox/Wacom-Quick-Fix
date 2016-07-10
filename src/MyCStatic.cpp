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
#include "MyCStatic.h"
#include "resource.h"

#define ARROWS pParent->GetDlgItem(6008)
#define SHOW ShowWindow(SW_SHOW)
#define HIDE ShowWindow(SW_HIDE)
#define WACOM_SCREEN pParent->GetDlgItem(2002)


MyCStatic::MyCStatic()
{
}


MyCStatic::~MyCStatic()
{
}
BEGIN_MESSAGE_MAP(MyCStatic, CStatic)
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
BOOL MyCStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	/* Set the main window variable. */
	CWnd* pParent = AfxGetApp()->m_pMainWnd;

	/* Check which static-button hybrid-thing we're over. */
	switch (this->GetDlgCtrlID()) {

		/* First About Question */
	case 6002:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 122, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

		/* Second About Question */
	case 6003:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 152, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

		/* Third About Question */
	case 6004:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 182, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

		/* Fourth About Question */
	case 6005:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 212, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

		/* Fifth About Question */
	case 6006:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 242, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

		/* Sixth About Question */
	case 6007:
		if (!ARROWS->IsWindowVisible()) {
			ARROWS->SetWindowPos(WACOM_SCREEN, 93, 272, 15, 22, NULL);
			ARROWS->SHOW;
		}
		break;

	default:
		break;
	}

	/* Set the cursor to a hand when hovering over a button. */
	HCURSOR hCursor = LoadCursor(NULL, IDC_HAND);
	::SetCursor(hCursor);
	return TRUE;

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
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
void MyCStatic::OnMouseLeave()
{
	/* Set the main window variable. */
	CWnd* pParent = AfxGetApp()->m_pMainWnd;

	/* Get that arrow out of here. */
	ARROWS->HIDE;

	CStatic::OnMouseLeave();
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
void MyCStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE;
	tme.dwHoverTime = (DWORD)10000;
	TrackMouseEvent(&tme);

	CStatic::OnMouseMove(nFlags, point);
}
