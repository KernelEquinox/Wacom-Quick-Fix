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

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWacomQuickFixApp:
// See WacomQuickFix.cpp for the implementation of this class
//

class CWacomQuickFixApp : public CWinApp
{
public:
	CWacomQuickFixApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWacomQuickFixApp theApp;
