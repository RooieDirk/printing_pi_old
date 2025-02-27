#pragma once

/******************************************************************************
*
* Project:  OpenCPN
* Purpose:  Printing Plugin
* Author:   RooieDirk
*
***************************************************************************
*   Copyright (C) 2017 by RooieDirk                                   *
*   $EMAIL$                                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************
*/

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Printing_pi.h"
#include "Printinggui.h"
#include "Printinggui_impl.h"
#include "ocpn_plugin.h"
#include "ChartMem.h"


class ChartBorderCalc : public ChartMem
{
public:
  void AddChartBorderScales(wxDC* dcIn);
  void CalcSizes(wxDC* dcIn);
  void UpdateDCin(wxDC* dc);
  void DrawBorders();
  wxRect Rect;
  int BorderHeight, BorderWidth;

  void DrawLongitudeScale();
  void DrawLatitudeScale();
  wxCoord PrintLabel(wxPoint p, wxString s, long style);
  double fmod_away(double _x, double _y);
  bool IsMultiOf(double lon, double div);
private:
  void SetBorderRect(wxRect* R, wxRect maxR);
  //wxDC* DCin;
};
