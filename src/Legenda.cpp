// /******************************************************************************
// *
// * Project:  OpenCPN
// * Purpose:  Printing Plugin
// * Author:   RooieDirk
// *
// ***************************************************************************
// *   Copyright (C) 2017 by RooieDirk                                   *
// *   $EMAIL$                                                               *
// *                                                                         *
// *   This program is free software; you can redistribute it and/or modify  *
// *   it under the terms of the GNU General Public License as published by  *
// *   the Free Software Foundation; either version 2 of the License, or     *
// *   (at your option) any later version.                                   *
// *                                                                         *
// *   This program is distributed in the hope that it will be useful,       *
// *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
// *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
// *   GNU General Public License for more details.                          *
// *                                                                         *
// *   You should have received a copy of the GNU General Public License     *
// *   along with this program; if not, write to the                         *
// *   Free Software Foundation, Inc.,                                       *
// *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
// ***************************************************************************
// */

#include "Printing_pi.h"
#include "Printinggui.h"
#include "Printinggui_impl.h"
#include "ocpn_plugin.h"
#include "Legenda.h"
#include <wx/dc.h>
#include <wx/dcgraph.h>


extern int g_i_PaperSelection;
extern double g_vp_clat;  // center point
extern double g_vp_clon;
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

extern bool g_vp_bValid;  // This VP is valid

extern double g_DisplaySizeMM;
extern wxCoord g_DispalySizePix;
extern double g_DisplayPixMM;
extern wxFont m_BorderFont;
extern wxFont m_LegendaFont;
extern wxFont m_NotesFont;
extern wxSize PaperSizePix;
extern int BorderStyle;

extern wxString LegendaText1;
extern wxString LegendaText2;
extern wxString DepthUnits;
extern int SliderH;
extern int SliderV;
extern wxRect ChRect;
extern wxRect ChR1;
extern wxRect ChR2;
extern wxRect ChR25;
extern wxRect ChR3;
extern wxRect ChR35;

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Legenda::UpdateDCin(wxDC* dc) {
  wxCoord w, h;
  dc->GetSize(&w, &h);
  DcIn = dc;
  w = wxMin(w, SizeMax.GetWidth());
  h = wxMin(h, SizeMax.GetHeight());
  wxBitmap bm(w, h, 32);
  bm.UseAlpha(false);
  if (!DC) DC = new wxMemoryDC();
    DC->SelectObject(wxNullBitmap);
  DC->SelectObject(bm);
  DC->Blit(0,0, w, h, dc, 0,0);
  DrawLegenda();
  int posx= map(SliderH, 0, 100, ChRect.GetLeft(), ChRect.GetRight()-LegendaBm.GetWidth());
  int posy= map(SliderV, 0, 100, ChRect.GetTop(), ChRect.GetBottom()-LegendaBm.GetHeight());
  DC->DrawBitmap(LegendaBm,posx,posy);
  if (Next) Next->UpdateDCin(DC);
}

//https://forums.wxwidgets.org/viewtopic.php?t=19277
void Legenda::DrawLegenda() {
  wxCoord w, h;
  DC->GetSize(&w, &h);

  //make a tempDC to draw LegendaText1
  wxBitmap bm(w, h, 32);
  bm.UseAlpha();
  wxMemoryDC* tdc = new wxMemoryDC();
  tdc->SelectObject(bm);
  tdc->SetPen(*wxBLACK_PEN);
  tdc->SetBackground(*wxTRANSPARENT_BRUSH);
  tdc->SetFont(m_LegendaFont.Larger().Larger().Larger().Larger());
  wxRect lr(0,0,w,h);
  tdc->Clear();
  if (LegendaText1 != _(""))
    tdc->DrawLabel(LegendaText1, lr, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
  lr.SetTop(tdc->MaxY());
  tdc->SetFont(m_LegendaFont.Larger().Larger());
  if (LegendaText2 != _(""))
    tdc->DrawLabel(LegendaText2, lr, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);
  lr.SetTop(tdc->MaxY());
  tdc->SetFont(m_LegendaFont);
  tdc->DrawLabel( wxString::Format("Depth in: %s",DepthUnits), lr, wxALIGN_TOP | wxALIGN_CENTER_HORIZONTAL);


  wxRect l(wxPoint(tdc->MinX(), tdc->MinY()), wxPoint(tdc->MaxX(), tdc->MaxY()));
//   LegendaBm.SetHeight(l.GetHeight());
//   LegendaBm.SetWidth(l.GetWidth());
  LegendaBm = tdc->GetAsBitmap(&l);
}
