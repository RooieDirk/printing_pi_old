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
#include "ChartBorders.h"

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

std::vector<double> d{0.0001666666666666666666666666666666666666666666666666667,
                      0.0003333333333333333333333333333333333333333333333333333,
                      0.0016666666666666666666666666666666666666666666666666667,
                      0.0033333333333333333333333333333333333333333333333333333,
                      0.0166666666666666666666666666666666666666666666666666667,
                      0.0333333333333333333333333333333333333333333333333333333,
                      0.1666666666666666666666666666666666666666666666666666667,
                      0.3333333333333333333333333333333333333333333333333333333,
                      0.5000000000000000000000000000000000000000000000000000000,
                      1.0000000000000000000000000000000000000000000000000000000,
                      2.0,
                      5.0,
                      10.0,
                      20.0,
                      50.0,
                      100.0};

void ChartBorderCalc::AddChartBorderScales(wxDC* dcIn) {
  DCin = dcIn;
  CalcSizes(DCin);
  wxCoord w = Rect.GetRight();
  wxCoord h = Rect.GetBottom();
  wxBitmap bm(w, h);
  DC->SelectObject(bm);
  DC->Blit(Rect.GetLeft(), Rect.GetTop(), Rect.GetWidth(), Rect.GetHeight(),
           dcIn, 0, 0);
}

// void ChartBorderCalc::CalcSizes(wxDC* dcIn)
// Calc and set the wxRect's needed to draw the borders
// returns the size needed for the output wxDC
void ChartBorderCalc::CalcSizes(wxDC* dcIn) {
  dcIn->SetFont(m_BorderFont);
  wxSize Textext = dcIn->GetTextExtent("55°N");
  BorderWidth = Textext.GetWidth() + 14;
  BorderHeight = Textext.GetHeight() + 14;
  Rect.SetLeftTop(wxPoint(0, 0));
  Rect.SetSize(SizeMax);

  while (1) {
    ChRect = Rect;
    ChRect.Deflate(1, 1);  // make space for a single black line border
    if (BorderStyle & TOPBORDER) {
      ChRect.SetTop(BorderHeight);
      ChRect.SetBottom(ChRect.GetBottom() - BorderHeight);
    }
    if (BorderStyle & LEFTBORDER) {
      ChRect.SetLeft(BorderWidth);
      ChRect.SetRight(ChRect.GetRight() - BorderWidth);
    }
    if (BorderStyle & RIGHTBORDER) {
      ChRect.SetRight(ChRect.GetRight() - BorderWidth);
    }
    if (BorderStyle & BOTTOMBORDER) {
      ChRect.SetBottom(ChRect.GetBottom() - BorderHeight);
    }
    if (dcIn->GetSize().GetWidth() < ChRect.GetWidth()) {
      Rect.SetWidth(dcIn->GetSize().GetWidth());
      continue;
    }
    if (dcIn->GetSize().GetHeight() < ChRect.GetHeight()) {
      Rect.SetHeight(dcIn->GetSize().GetHeight());
      continue;
    }
    break;
  }

  ChR1 = ChRect;
  ChR1.Inflate(1);
  SetBorderRect(&ChR1, Rect);
  ChR2 = ChR1;
  ChR2.Inflate(4);
  SetBorderRect(&ChR2, Rect);
  ChR25 = ChR2;
  ChR25.Inflate(2);
  SetBorderRect(&ChR25, Rect);
  ChR3 = ChR25;
  ChR3.Inflate(2);
  SetBorderRect(&ChR3, Rect);
  ChR35 = ChR3;
  ChR35.Inflate(2);
  SetBorderRect(&ChR35, Rect);
}

void ChartBorderCalc::SetBorderRect(wxRect* R, wxRect maxR) {
  if (R->GetLeft() < 0) {
    R->SetRight(R->GetRight() + R->GetLeft());
    R->SetLeft(0);
  }
  if (R->GetTop() < 0) {
    R->SetBottom(R->GetBottom() + R->GetTop());
    R->SetTop(0);
  }
  if (R->GetRight() > maxR.GetRight()) R->SetRight(maxR.GetRight());
  if (R->GetBottom() > maxR.GetBottom()) R->SetBottom(maxR.GetBottom());
}

void ChartBorderCalc::UpdateDCin(wxDC* dc) {
  wxCoord w, h;
  dc->GetSize(&w, &h);
  DcIn = dc;
  w = wxMin(w, SizeMax.GetWidth());
  h = wxMin(h, SizeMax.GetHeight());
  wxBitmap bm(w, h);
  if (!DC) DC = new wxMemoryDC();
  DC->SelectObject(wxNullBitmap);
  DC->SelectObject(bm);
  // DC->Blit(0,0, w, h, dc, 0,0);
  CalcSizes(dc);
  DrawBorders();
  if (Next) Next->UpdateDCin(DC);
}

void ChartBorderCalc::DrawBorders() {
  DC->Clear();

  DC->SetPen(*wxBLACK_PEN);
  DC->SetBrush(*wxWHITE_BRUSH);
  DC->SetFont(m_BorderFont);

  DC->DrawRectangle(ChR3);
  DC->DrawRectangle(ChR2);
  DC->DrawRectangle(ChR1);
  DC->Blit(ChRect.GetLeft(), ChRect.GetTop(), ChRect.GetWidth(),
           ChRect.GetHeight(), DcIn, 0, 0);

  DrawLongitudeScale();
  DrawLatitudeScale();
}

void ChartBorderCalc::DrawLongitudeScale() {
  double LonDegPPix =
      (double)(((g_vp_lon_max) - (g_vp_lon_min)) / (double)g_vp_pix_width);
  double DeltaLonChart = ChRect.GetWidth() * LonDegPPix;
  double LonChartMax = g_vp_lon_min + DeltaLonChart;
  double minor_max_even;
  int i_minor_min, i_minor_max, i_major_min, i_major_max;
  double w_min = (ChR1.GetTop() - ChR2.GetTop()) * LonDegPPix;

  // find the minimum divide factor that fits on the paper
  uint i = 0;
  for (i = 0; i < d.size() - 1; i++) {
    if (d[i] >= w_min) {
      i_minor_min = i;
      break;
    }
  }
  for (uint i = 1; i < d.size(); i++) {
    if (d[i] > DeltaLonChart) {
      i_major_max = i - 1;
      break;
    }
  }
  int delta_i = i_major_max - i_minor_min;
  i_minor_max = round(i_minor_min + delta_i / 3);
  i_major_min = round(i_minor_min + delta_i * 2 / 3);
  while (round(d[i_minor_max] / d[i_minor_min]) > 10.1) i_minor_max--;
  while (round(d[i_major_max] / d[i_major_min]) > 10.1) i_major_min++;

  i_minor_max = i_minor_min - (int)round((i_minor_min - i_major_max) / 3.0);
  i_major_min = i_minor_min - (int)round((i_minor_min - i_major_max) / 1.5);

  double frac = fmod(abs(g_vp_lon_min), d[i_minor_min]);
  double l_minor_min = g_vp_lon_min - (d[i_minor_min] - abs(frac));

  minor_max_even = l_minor_min;
  double lon = l_minor_min;
  wxString s;
  while (lon <= LonChartMax) {
    if (IsMultiOf(lon, d[i_major_max])) {
      int m = round(lon / d[i_major_max]);
      lon = m * d[i_major_max];
      DC->SetFont(m_BorderFont);
      float PrVal = (lon < -180) ? lon + 360 : (lon > 180) ? lon - 360 : lon;
      s = wxString::Format(
          _("%i%c"), (int)round(abs(PrVal) - fmod(fabs(PrVal), 1.0)), 0x00B0);
      // s = wxString::Format(_("%i%c"), (int)round(abs(PrVal)), 0x00B0);
      int minutes = round(fmod(fabs(lon), 1.0) * 60);
      wxString Sign = ((lon < -0.01)  ? ((lon > -179.99) ? "W" : "")
                       : (lon > 0.01) ? ((lon < 179.99) ? "E" : "")
                                      : "");
      wxCoord w;
      if (!minutes) {
        PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop()),
                   s.Append(Sign), wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);
        PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom()),
                   s, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM);
      } else {
        PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop()), s,
                   wxALIGN_LEFT | wxALIGN_TOP);
        PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom()),
                   s, wxALIGN_LEFT | wxALIGN_BOTTOM);
        s = wxString::Format(_("%g%c"),
                             fmod(fabs(lon), 1.0) * 60 /*round(abs(minutes))*/,
                             0x0027);
        DC->SetFont(m_BorderFont.Smaller().Smaller());
        w = PrintLabel(
            wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop()), s,
            wxALIGN_RIGHT | wxALIGN_TOP);
        PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom()),
                   s, wxALIGN_RIGHT | wxALIGN_BOTTOM);
        DC->SetFont(m_BorderFont);
        PrintLabel(
            wxPoint(LonToPix(lon) + ChRect.GetLeft() + w, ChR35.GetTop()), Sign,
            wxALIGN_RIGHT | wxALIGN_TOP);
        PrintLabel(
            wxPoint(LonToPix(lon) + ChRect.GetLeft() + w, ChR35.GetBottom()),
            Sign, wxALIGN_RIGHT | wxALIGN_BOTTOM);
      }
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());

    } else if (IsMultiOf(lon, d[i_major_min])) {
      int m = round(lon / d[i_major_min]);
      lon = m * d[i_major_min];
      DC->SetFont(m_BorderFont.Smaller().Smaller());
      if (((int)(round(fabs(lon * 60)))) % 60 == 0) {
        float PrVal = (lon < -180) ? lon + 360 : (lon > 180) ? lon - 360 : lon;
        s = wxString::Format(_("%i%c"), (int)round(abs(PrVal)), 0x00B0);
      } else
        s = wxString::Format(_("%g%c"), fabs(fmod(lon, 1) * 60), 0x0027);
      PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop()), s,
                 wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);
      PrintLabel(wxPoint(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom()),
                 s, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM);
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());

    } else if (IsMultiOf(lon, d[i_major_min] / 2) &&
               (d[i_major_min] / d[i_minor_max] > 9.9)) {
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR35.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());

    } else if (IsMultiOf(lon, d[i_minor_max])) {
      int m = round(lon / d[i_minor_max]);
      lon = m * d[i_minor_max];
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR3.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR3.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());

    } else if (IsMultiOf(lon, d[i_minor_max] / 2) &&
               (d[i_minor_max] / d[i_minor_min] > 9.9)) {
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR25.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR25.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());

    } else {
      int m = round(lon / d[i_minor_min]);
      lon = m * d[i_minor_min];
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR2.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetTop());
      DC->DrawLine(LonToPix(lon) + ChRect.GetLeft(), ChR2.GetBottom(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR1.GetBottom());
    }

    if (IsMultiOf(lon, d[i_minor_max] * 2)) {  // is odd
      DC->DrawLine(LonToPix(minor_max_even) + ChRect.GetLeft(), ChR25.GetTop(),
                   LonToPix(lon) + ChRect.GetLeft(), ChR25.GetTop());
      DC->DrawLine(LonToPix(minor_max_even) + ChRect.GetLeft(),
                   ChR25.GetBottom(), LonToPix(lon) + ChRect.GetLeft(),
                   ChR25.GetBottom());
    } else if (IsMultiOf(lon, d[i_minor_max]))
      minor_max_even = lon;

    lon += d[i_minor_min];
  }
}

void ChartBorderCalc::DrawLatitudeScale() {
  double LatDegPPix =
      (double)(((g_vp_lat_max) - (g_vp_lat_min)) / (double)g_vp_pix_height);

  double DeltaLatChart = g_vp_lat_max - PixToLat(ChRect.GetHeight());
  double LatChartMin = g_vp_lat_max - DeltaLatChart;
  double minor_max_even;
  int i_minor_min, i_minor_max, i_major_min, i_major_max;
  double w_min = (ChR1.GetLeft() - ChR2.GetLeft()) * LatDegPPix;

  // find the minimum divide factor that fits on the paper
  uint i = 0;
  for (i = 0; i < d.size() - 1; i++) {
    if (d[i] >= w_min) {
      i_minor_min = i;
      break;
    }
  }
  for (uint i = 1; i < d.size(); i++) {
    if (d[i] > DeltaLatChart) {
      i_major_max = i - 1;
      break;
    }
  }
  int delta_i = i_major_max - i_minor_min;
  i_minor_max = round(i_minor_min + delta_i / 3);
  i_major_min = round(i_minor_min + delta_i * 2 / 3);
  while (round(d[i_minor_max] / d[i_minor_min]) > 10.1) i_minor_max--;
  while (round(d[i_major_max] / d[i_major_min]) > 10.1) i_major_min++;

  i_minor_max = i_minor_min - (int)round((i_minor_min - i_major_max) / 3.0);
  i_major_min = i_minor_min - (int)round((i_minor_min - i_major_max) / 1.5);

  double frac = fmod(abs(g_vp_lat_max), d[i_minor_min]);
  double l_minor_min = g_vp_lat_max + (d[i_minor_min] - abs(frac));

  minor_max_even = l_minor_min;
  double lat = l_minor_min;
  int m = (int)round(lat / d[i_minor_min]);
  lat = m * d[i_minor_min];

  wxString s;
  while (lat >= LatChartMin) {
    if (IsMultiOf(lat, d[i_major_max])) {
      int m = (int)round(lat / d[i_major_max]);
      lat = m * d[i_major_max];
      DC->SetFont(m_BorderFont);
      s = wxString::Format(_("%2.0f%c"), round(abs(lat) - fmod(fabs(lat), 1.0)),
                           0x00B0);
      int minutes = round(fmod(fabs(lat), 1.0) * 60);
      wxString Sign = ((lat < -0.01) ? "S" : (lat > 0.01) ? "N" : "");
      wxCoord w;
      if (!minutes) {
        PrintLabel(wxPoint(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop()),
                   s.Append(Sign), wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
        PrintLabel(wxPoint(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop()),
                   s /*.Append(Sign)*/,
                   wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
      } else {
        PrintLabel(wxPoint(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop()), s,
                   wxALIGN_LEFT | wxALIGN_TOP);
        PrintLabel(wxPoint(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop()),
                   s, wxALIGN_RIGHT | wxALIGN_TOP);
        DC->SetFont(m_BorderFont.Smaller().Smaller());
        s = wxString::Format(_("%g%c"), fabs(fmod(lat, 1) * 60), 0x0027);
        w = PrintLabel(
            wxPoint(ChR35.GetLeft(), ChRect.GetTop() + LatToPix(lat)), Sign,
            wxALIGN_LEFT | wxALIGN_BOTTOM);
        PrintLabel(
            wxPoint(ChR35.GetLeft() - w, LatToPix(lat) + ChRect.GetTop()), s,
            wxALIGN_LEFT | wxALIGN_BOTTOM);

        w = PrintLabel(
            wxPoint(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop()), s,
            wxALIGN_RIGHT | wxALIGN_BOTTOM);
        PrintLabel(
            wxPoint(ChR35.GetRight() + w, ChRect.GetTop() + LatToPix(lat)),
            Sign, wxALIGN_RIGHT | wxALIGN_BOTTOM);
      }
      DC->DrawLine(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChRect.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChRect.GetRight(), LatToPix(lat) + ChRect.GetTop());

    } else if (IsMultiOf(lat, d[i_major_min])) {
      //       double m = round(lat/d[i_major_min]);
      //       lat = m * d[i_major_min];
      DC->SetFont(m_BorderFont.Smaller().Smaller());
      if (((int)(round(fabs(lat * 60)))) % 60 == 0) {
        float PrVal = (lat < -180) ? lat + 360 : (lat > 180) ? lat - 360 : lat;
        s = wxString::Format(_("%i%c"), (int)round(abs(PrVal)), 0x00B0);
      } else
        s = wxString::Format(_("%g'"), fabs(fmod(lat, 1) * 60));

      PrintLabel(wxPoint(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop()), s,
                 wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
      PrintLabel(wxPoint(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop()), s,
                 wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);

      DC->DrawLine(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetRight(), LatToPix(lat) + ChRect.GetTop());

    } else if (IsMultiOf(lat, d[i_major_min] / 2) &&
               (d[i_major_min] / d[i_minor_max] > 9.9)) {
      DC->DrawLine(ChR35.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR35.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetRight(), LatToPix(lat) + ChRect.GetTop());

    } else if (IsMultiOf(lat, d[i_minor_max])) {
      DC->DrawLine(ChR3.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR3.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetRight(), LatToPix(lat) + ChRect.GetTop());

    } else if (IsMultiOf(lat, d[i_minor_max] / 2) &&
               (d[i_minor_max] / d[i_minor_min] > 9.9)) {
      DC->DrawLine(ChR25.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR25.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetRight(), LatToPix(lat) + ChRect.GetTop());

    } else {
      DC->DrawLine(ChR2.GetLeft(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetLeft(), LatToPix(lat) + ChRect.GetTop());
      DC->DrawLine(ChR2.GetRight(), LatToPix(lat) + ChRect.GetTop(),
                   ChR1.GetRight(), LatToPix(lat) + ChRect.GetTop());
    }

    if (IsMultiOf(lat, d[i_minor_max] * 2)) {  // is odd
      DC->DrawLine(ChR25.GetLeft(), ChRect.GetTop() + LatToPix(minor_max_even),
                   ChR25.GetLeft(), ChRect.GetTop() + LatToPix(lat));
      DC->DrawLine(ChR25.GetRight(), ChRect.GetTop() + LatToPix(minor_max_even),
                   ChR25.GetRight(), ChRect.GetTop() + LatToPix(lat));
    } else if (IsMultiOf(lat, d[i_minor_max]))
      minor_max_even = lat;

    lat -= d[i_minor_min];
  }
}

wxCoord ChartBorderCalc::PrintLabel(wxPoint p, wxString s, long style) {
  if (p.x == ChR1.x) return 0;
  if (p.y == ChR1.y) return 0;
  wxCoord w, h;
  DC->GetTextExtent(s, &w, &h);
  wxRect lr;
  lr.SetLeft(std::max(p.x - w, 0));
  lr.SetTop(std::max(p.y - (int)(0.8 * h), 0));
  lr.SetRight(p.x + w);
  lr.SetBottom(p.y + (int)(0.8 * h));
  DC->DrawLabel(s, lr, style);
  return w;
}

double ChartBorderCalc::fmod_away(double _x, double _y) {
  double temp = fmod(_x, _y);
  return std::min(_y - temp, temp - _y);
}
bool ChartBorderCalc::IsMultiOf(double lon, double div) {
  double temp = fabs(fmod(lon, div));
  double temp2 = (temp < abs(div - temp)) ? temp : abs(div - temp);
  return (temp2 < div / 100000);
}
