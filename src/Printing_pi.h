#pragma once
/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  ShipDriver Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Mike Rossiter                                   *
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

#ifndef _SDR_PI_H_
#define _SDR_PI_H_

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
  #include "wx/wx.h"
  #include <wx/glcanvas.h>
#endif // precompiled headers

#include <wx/fileconf.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

#include "config.h"

#include "json/reader.h"
#include "json/writer.h"

#include "ocpn_plugin.h" //Required for OCPN plugin functions
//#include "Printinggui.h"
#include "Printinggui_impl.h"


class Dlg;

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

class piDC;

class Printer_pi : public opencpn_plugin_118 {
public:
    Printer_pi(void* ppimgr);
    ~Printer_pi(void);

    //    The required PlugIn Methods
    int Init(void);
    bool DeInit(void);

    int GetAPIVersionMajor();
    int GetAPIVersionMinor();
    int GetPlugInVersionMajor();
    int GetPlugInVersionMinor();
    wxBitmap* GetPlugInBitmap();
    wxString GetCommonName();
    wxString GetShortDescription();
    wxString GetLongDescription();

    //    The required override PlugIn Methods
    int GetToolbarToolCount(void);
    void OnToolbarToolCallback(int id);

    //    Optional plugin overrides
    void SetColorScheme(PI_ColorScheme cs);
    bool RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp);
    bool RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp);
    void CopyVp(PlugIn_ViewPort *vpS);
    wxPoint GetLLPix(double lat, double lon);

    void OnShipDriverDialogClose();
    void ShowPreferencesDialog(wxWindow* parent);

    wxString StandardPath();
    wxBitmap m_panelBitmap;

    piDC      *g_pDC;
    wxMemoryDC m_mDC;
    PlugIn_ViewPort m_pVP;
    wxWindow* m_parent_window;


private:
    double m_cursor_lat;
    double m_cursor_lon;

    int m_position_menu_id;
    double m_GUIScaleFactor;
    void OnClose(wxCloseEvent& event);

    Printer_pi* plugin;

    Dlg* m_pDialog;

    wxFileConfig* m_pconfig;

    bool LoadConfig(void);
    bool SaveConfig(void);

    int m_hr_dialog_width, m_hr_dialog_height;
    int m_hr_dialog_sx, m_hr_dialog_sy;
    int m_display_width, m_display_height;
    int m_leftclick_tool_id;
    bool m_bPrintingShowIcon = TRUE;
    bool m_bShowDlg;

    bool m_bCopyUseAis;
    bool m_bCopyUseFile;
    wxString m_tCopyMMSI;
};

#endif
