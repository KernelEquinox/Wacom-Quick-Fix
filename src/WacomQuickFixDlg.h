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

#include <afxext.h>
#include "MyCButton.h"
#include "MyCStatic.h"
#pragma once


// CWacomQuickFixDlg dialog
class CWacomQuickFixDlg : public CDialogEx
{
// Construction
public:
	CWacomQuickFixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WacomQuickFix_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMinimize();
	afx_msg void OnBnClickedHome();
	afx_msg void OnBnClickedCycle();
	afx_msg void OnBnClickedToggle();
	afx_msg void OnBnClickedPatch();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedAbout1();
	afx_msg void OnBnClickedAbout2();
	afx_msg void OnBnClickedAbout3();
	afx_msg void OnBnClickedAbout4();
	afx_msg void OnBnClickedAbout5();
	afx_msg void OnBnClickedAbout6();
	afx_msg void OnBnClickedBack();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnGetDefID(WPARAM wp, LPARAM lp);


///////////////////////////////////////////////////////////////////////////////
// Permanent dialog elements:

	/* Main UI Buttons */
	MyCButton* m_HomeButton = new MyCButton;
	MyCButton* m_AboutButton = new MyCButton;
	MyCButton* m_CloseButton = new MyCButton;
	MyCButton* m_MinimizeButton = new MyCButton;

	/* Wacom Elements */
	CStatic* m_WacomBorder = new CStatic;
	CStatic* m_WacomScreen = new CStatic;
	CStatic* m_WacomLogo = new CStatic;
	CStatic* m_PenLight = new CStatic;
	CStatic* m_PowerLight = new CStatic;
	
	/* Non-Functioning Buttons */
	CStatic* m_UpperInactiveButtonsLeft = new CStatic;
	CStatic* m_UpperInactiveButtonsRight = new CStatic;
	CStatic* m_LowerInactiveButtonsLeft = new CStatic;
	CStatic* m_LowerInactiveButtonsRight = new CStatic;

	/* Additional Style */
	CStatic* m_UpperTopBevelLeft = new CStatic;
	CStatic* m_UpperTopBevelRight = new CStatic;
	CStatic* m_UpperBottomBevelLeft = new CStatic;
	CStatic* m_UpperBottomBevelRight = new CStatic;
	CStatic* m_LowerTopBevelLeft = new CStatic;
	CStatic* m_LowerTopBevelRight = new CStatic;
	CStatic* m_LowerBottomBevel = new CStatic;


///////////////////////////////////////////////////////////////////////////////
// Pre-load some stuff:

	/* Font Setup */
	CFont* m_TitleFont = new CFont;
	CFont* m_DefaultFont = new CFont;
	CFont* m_SubheaderFont = new CFont;
	CFont* m_BackFont = new CFont;

	/* Screen Header Text */
	CStatic* m_HomeTitleText = new CStatic;
	CStatic* m_AboutTitleText = new CStatic;
	CStatic* m_QuestionTitleText = new CStatic;


///////////////////////////////////////////////////////////////////////////////
// Home screen elements:

	/* Home Screen Text */
	CStatic* m_DefaultText = new CStatic;
	CStatic* m_CycleText = new CStatic;
	CStatic* m_ToggleText = new CStatic;
	CStatic* m_PatchText = new CStatic;
	CStatic* m_ToggleStatusEnabled = new CStatic;
	CStatic* m_ToggleStatusDisabled = new CStatic;
	CStatic* m_PatchStatusUnpatched = new CStatic;
	CStatic* m_PatchStatusPatched = new CStatic;
	CStatic* m_PatchStatusNotPatched = new CStatic;
	CStatic* m_PatchStatusNotExist = new CStatic;

	/* Home Screen Buttons */
	MyCButton* m_CycleButton = new MyCButton;
	MyCButton* m_ToggleButton = new MyCButton;
	MyCButton* m_PatchButton = new MyCButton;


///////////////////////////////////////////////////////////////////////////////
// About screen elements:

	/* About Screen Text */
	MyCStatic* m_AboutText1 = new MyCStatic;
	MyCStatic* m_AboutText2 = new MyCStatic;
	MyCStatic* m_AboutText3 = new MyCStatic;
	MyCStatic* m_AboutText4 = new MyCStatic;
	MyCStatic* m_AboutText5 = new MyCStatic;
	MyCStatic* m_AboutText6 = new MyCStatic;
	MyCStatic* m_AboutArrow = new MyCStatic;


///////////////////////////////////////////////////////////////////////////////
// Question entry elements:

	/* Question Entry Text*/
	CStatic* m_QuestionText1 = new CStatic;
	CStatic* m_AnswerText1 = new CStatic;
	CStatic* m_QuestionText2 = new CStatic;
	CStatic* m_AnswerText2 = new CStatic;
	CStatic* m_QuestionText3 = new CStatic;
	CStatic* m_AnswerText3 = new CStatic;
	CStatic* m_QuestionText4 = new CStatic;
	CStatic* m_AnswerText4 = new CStatic;
	CStatic* m_QuestionText5 = new CStatic;
	CStatic* m_AnswerText5 = new CStatic;
	CStatic* m_QuestionText6 = new CStatic;
	CStatic* m_AnswerText6 = new CStatic;
	MyCStatic* m_BackText = new MyCStatic;
	CStatic* m_CopyrightText = new CStatic;
};