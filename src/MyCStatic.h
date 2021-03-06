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
#include "afxext.h"
class MyCStatic :
	public CStatic
{
public:
	MyCStatic();
	~MyCStatic();
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

