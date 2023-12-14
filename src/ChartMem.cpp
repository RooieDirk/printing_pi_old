// /******************************************************************************
//  *
//  * Project:  OpenCPN
//  * Purpose:  Printing Plugin
//  * Author:   RooieDirk
//  *
//  ***************************************************************************
//  *   Copyright (C) 2017 by RooieDirk                                *
//  *   $EMAIL$                                                               *
//  *                                                                         *
//  *   This program is free software; you can redistribute it and/or modify  *
//  *   it under the terms of the GNU General Public License as published by  *
//  *   the Free Software Foundation; either version 2 of the License, or     *
//  *   (at your option) any later version.                                   *
//  *                                                                         *
//  *   This program is distributed in the hope that it will be useful,       *
//  *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
//  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
//  *   GNU General Public License for more details.                          *
//  *                                                                         *
//  *   You should have received a copy of the GNU General Public License     *
//  *   al_minor_ming with this program; if not, write to the                         *
//  *   Free Software Foundation, Inc.,                                       *
//  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
//  ***************************************************************************
//  */

#include "wx/wxprec.h"
#include "ChartMem.h"
#include "Printing_pi.h"
#include <vector>
#include <list>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

extern int g_i_PaperSelection;
extern double g_DisplaySizeMM;
extern wxCoord g_DispalySizePix;
extern double g_DisplayPixMM;
extern wxFont m_BorderFont;
extern wxFont m_LegendaFont;
extern wxFont m_NotesFont;
extern wxSize PaperSizePix;
extern int BorderStyle;
extern double g_vp_clat;  // center point
extern double g_vp_cl_minor_min;
extern double g_vp_view_scale_ppm;
extern double g_vp_skew;
extern double g_vp_rotation;

extern float g_vp_chart_scale;  // conventional chart displayed scale

extern int g_vp_pix_width;
extern int g_vp_pix_height;
extern wxRect g_vp_rv_rect;
extern bool g_vp_b_quilt;
extern int g_vp_m_projection_type;

extern double g_vp_lat_min;
extern double g_vp_lat_max;
extern double g_vp_lon_min;
extern double g_vp_lon_max;

extern PlugIn_ViewPort *vpl;
wxRect ChRect, ChR1, ChR2, ChR25, ChR3, ChR35;

// wxRealPoint gLLtopLeft, gLLlowRight;
// wxPoint gPixTopLeft, gPixLowRight;



ChartMem::ChartMem(wxDC* dc)
{
  DcIn = dc;
  Next = NULL;
  Previous = NULL;
  DC = new wxMemoryDC();
  UpdateDCin(DcIn);
}
ChartMem::ChartMem(ChartMem* pr)
{
  ChartMem(pr->GetDC());
  Previous = pr;
}
ChartMem::ChartMem()
{
  DcIn = NULL;
  Next = NULL;
  Previous = NULL;
  DC = new wxMemoryDC();
}
ChartMem::~ChartMem()
{
  if(Next) delete Next;
  if (DC) delete DC;
}

void ChartMem::UpdateDCin(wxDC* dc)
{
  wxCoord w, h;
  dc->GetSize(&w, &h);
  if(Previous) SizeMax=Previous->GetMaxSize();
  DcIn = dc;
  w = wxMin(w, SizeMax.GetWidth());
  h = wxMin(h, SizeMax.GetHeight());
  wxBitmap bm(w, h);
  if(!DC) DC = new wxMemoryDC();
  DC->SelectObject(wxNullBitmap);
  DC->SelectObject(bm);
  DC->Blit(0,0, w, h, dc, 0,0);
  if(Next) Next->UpdateDCin(DC);
}
void ChartMem::UpdateDC()
{
  if(DcIn) UpdateDCin(DcIn);
}
void ChartMem::AddChild(ChartMem* newCM)
{
  Next = newCM;
  Next->SetSizeMax(SizeMax);
  Next->SetPrevious(this);
  Next->UpdateDCin(GetDC());
  //return Next;
}
float ChartMem::PixToLon(wxCoord pix)
{
  return (pix - 0) * (g_vp_lon_max - g_vp_lon_min) / g_vp_pix_width + g_vp_lon_min;
}
wxCoord ChartMem::LonToPix(double l_minor_min)
{
  return (int)round(((double)(l_minor_min - g_vp_lon_min) * (double)g_vp_pix_width) / (double)(g_vp_lon_max - g_vp_lon_min));
}

wxCoord ChartMem::LatToPix(double lat)
{
  double lat_exAgg = LatToExAgg(lat);
  double g_vp_lat_min_exAgg= LatToExAgg(g_vp_lat_min);
  double g_vp_lat_max_exAgg= LatToExAgg(g_vp_lat_max);
  return (int)(((g_vp_lat_max_exAgg - lat_exAgg) * g_vp_pix_height) / (g_vp_lat_max_exAgg - g_vp_lat_min_exAgg) + 0.5);
}
float ChartMem::PixToLat(wxCoord pix)
{
  double g_vp_lat_min_exAgg= LatToExAgg(g_vp_lat_min);
  double g_vp_lat_max_exAgg= LatToExAgg(g_vp_lat_max);
  return ExAggToLat((pix - 0) * (g_vp_lat_min_exAgg - g_vp_lat_max_exAgg) / g_vp_pix_height + g_vp_lat_max_exAgg);
}

