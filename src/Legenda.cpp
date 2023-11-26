// /******************************************************************************
// *
// * Project:  OpenCPN
// * Purpose:  ShipDriver Plugin
// * Author:   Mike Rossiter
// *
// ***************************************************************************
// *   Copyright (C) 2017 by Mike Rossiter                                   *
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
