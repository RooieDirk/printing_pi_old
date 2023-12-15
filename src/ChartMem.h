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


#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include <wx/bitmap.h>
#include <wx/image.h>


#define TOPBORDER 1
#define LEFTBORDER 2
#define RIGHTBORDER 4
#define BOTTOMBORDER 8

class ChartMem;
class ChartMem
{
public:
  ChartMem(wxDC* dc);
  ChartMem(ChartMem* pr);
  ChartMem();
  virtual ~ChartMem();
  virtual void UpdateDCin(wxDC* dc);
  void UpdateDC();
  void UpdateDC(int nextno){if(nextno)if(Next) Next->UpdateDC(--nextno);}
  wxDC* GetDC(){return DC;}
  void AddChild(ChartMem*);
  void SetSizeMax(wxSize size){SizeMax = size; if (Next) Next->SetSizeMax(size);}
  void SetPrevious(ChartMem* cm){Previous = cm;}
  wxSize GetMaxSize(){return SizeMax;}
  void SetDC(wxDC* dc){DcIn = dc;}
  wxDC*  GetLastDC(){return (Next)? Next->GetLastDC(): GetDC();}
  wxMemoryDC* DC;
protected:

  wxDC* DcIn;
  wxSize SizeMax;
  ChartMem* Next;
  ChartMem* Previous;
  double LatToExAgg(double Lat){return (10800/M_PI) * log(tan((M_PI/4) + (Lat * M_PI/180/2)));}
  double ExAggToLat(double exLat) {return (atan(exp(exLat/(10800/M_PI)))-(M_PI/4))/ (M_PI/180/2);}
  float PixToLon(wxCoord pix);
  wxCoord LonToPix(double lon);
  wxCoord LatToPix(double lat);
  float PixToLat(wxCoord pix);

};


