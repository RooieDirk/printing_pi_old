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
//  *   along with this program; if not, write to the                         *
//  *   Free Software Foundation, Inc.,                                       *
//  *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
//  ***************************************************************************
//  */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif // precompiled headers

#include <wx/graphics.h>
//#include "pidc.h"
#include "Printing_pi.h"
#include "Printinggui.h"
#include "Printinggui_impl.h"
#include "ocpn_plugin.h"
#include <wx/display.h>
#include <wx/msgdlg.h>


double g_vp_clat;  // center point
double g_vp_clon;
double g_vp_view_scale_ppm;
double g_vp_skew;
double g_vp_rotation;

float g_vp_chart_scale;  // conventional chart displayed scale

int g_vp_pix_width;
int g_vp_pix_height;
wxRect g_vp_rv_rect;
bool g_vp_b_quilt;
int g_vp_m_projection_type;

double g_vp_lat_min;
double g_vp_lat_max;
double g_vp_lon_min;
double g_vp_lon_max;

bool g_vp_bValid;  // This VP is valid
int g_i_PaperSelection;
double g_DisplaySizeMM;
wxCoord g_DispalySizePix;
double g_DisplayPixMM;
wxFont m_BorderFont;
wxFont m_LegendaFont;
wxFont m_NotesFont;
wxSize PaperSizePix;
int BorderStyle;
wxString g_DepthUnits;

PlugIn_ViewPort *vpl;

//class Printer_pi;
class Dlg;


using namespace std;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr)
{
    return new Printer_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }
extern DECL_EXP double PlugInGetDisplaySizeMM();
extern "C" DECL_EXP void GetCanvasLLPix(PlugIn_ViewPort *vp, wxPoint p,
                                        double *plat, double *plon);

//---------------------------------------------------------------------------------------------------------
//
//    Printing PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

/**
 * Load a icon, possibly using SVG
 * Parameters
 *  - api_name: Argument to GetPluginDataDir()
 *  - icon_name: Base name of icon living in data/ directory. When using
 *    SVG icon_name.svg is used, otherwise icon_name.png
 */

static wxBitmap load_plugin(const char* icon_name, const char* api_name) {
    wxBitmap bitmap; 
    wxFileName fn;
    auto path = GetPluginDataDir(api_name);
    fn.SetPath(path);
    fn.AppendDir("data");
    fn.SetName(icon_name);
#ifdef ocpnUSE_SVG
    wxLogDebug("Loading SVG icon");
    fn.SetExt("svg");
    const static int ICON_SIZE = 48;  // FIXME: Needs size from GUI
    bitmap = GetBitmapFromSVGFile(fn.GetFullPath(), ICON_SIZE, ICON_SIZE);
#else
    wxLogDebug("Loading png icon");
    fn.SetExt("png");
    path = fn.GetFullPath();
    if (!wxImage::CanRead(path)) {
        wxLogDebug("Initiating image handlers.");
        wxInitAllImageHandlers();
    }
    wxImage panelIcon(path);
    bitmap = wxBitmap(panelIcon);
#endif
    wxLogDebug("Icon loaded, result: %s", bitmap.IsOk() ? "ok" : "fail");
    return bitmap;
}

 

Printer_pi::Printer_pi(void* ppimgr)
    : opencpn_plugin_118(ppimgr)
{
    // Create the PlugIn icons
    initialize_images();
    m_panelBitmap = load_plugin("printing_panel_icon", "Printer_pi");
    m_bShowDlg = false;
}

Printer_pi::~Printer_pi(void)
{

    delete _img_ShipDriverIcon;

    if (m_pDialog) {

        wxFileConfig* pConf = GetOCPNConfigObject();
        ;

        if (pConf) {
            pConf->SetPath("/PlugIns/Printer_pi");
//             pConf->Write("shipdriverUseAis", m_bCopyUseAis);
//             pConf->Write("shipdriverUseFile", m_bCopyUseFile);
//             pConf->Write("shipdriverMMSI", m_tCopyMMSI);
        }
        delete m_pDialog;
    }
}

int Printer_pi::Init(void)
{
    AddLocaleCatalog("opencpn-Printer_pi");

    // Set some default private member parameters
//     m_hr_dialog_x = 40;
//     m_hr_dialog_y = 80;
//     m_hr_dialog_sx = 400;
//     m_hr_dialog_sy = 300;
    ::wxDisplaySize(&m_display_width, &m_display_height);

    //    Get a pointer to the opencpn display canvas, to use as a parent for
    //    the POI Manager dialog
    m_parent_window = GetOCPNCanvasWindow();
    //    Get a pointer to the opencpn configuration object
    m_pconfig = GetOCPNConfigObject();
    //    And load the configuration items
    LoadConfig();

    //    This PlugIn needs a toolbar icon, so request its insertion
    if (m_bPrintingShowIcon) {
#ifdef ocpnUSE_SVG
        m_leftclick_tool_id = InsertPlugInToolSVG("Printing",
            _svg_shipdriver, _svg_shipdriver, _svg_shipdriver_toggled,
            wxITEM_CHECK, "Printing", "", NULL,
            -1, 0, this);
#else
        m_leftclick_tool_id = InsertPlugInTool("", _img_ShipDriverIcon,
            _img_ShipDriverIcon, wxITEM_CHECK, _("ShipDriver"), "", NULL,
            -1, 0, this);
#endif
    }
    //g_pDC = new piDC( );
    g_DisplaySizeMM = PlugInGetDisplaySizeMM();
    wxDisplay *d = new wxDisplay;
    g_DispalySizePix = d->GetClientArea().GetWidth();
    delete d;
    g_DisplayPixMM = (double)g_DispalySizePix/g_DisplaySizeMM;

    m_pDialog = new Dlg(m_parent_window);
    m_pDialog->plugin = this;
    m_bShowDlg = false;

    return (WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK | WANTS_DYNAMIC_OPENGL_OVERLAY_CALLBACK
        | WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | WANTS_PREFERENCES
        | WANTS_ONPAINT_VIEWPORT|  WANTS_CONFIG
        | WANTS_VECTOR_CHART_OBJECT_INFO);
}

bool Printer_pi::DeInit(void)
{
    //    Record the dialog position
    if (NULL != m_pDialog) {

        m_pDialog->Close();
        delete m_pDialog;
        m_pDialog = NULL;
        //delete g_pDC;
        //g_pDC = NULL;
        m_bShowDlg = false;
        SetToolbarItemState(m_leftclick_tool_id, m_bShowDlg);
    }

    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

    return true;
}

int Printer_pi::GetAPIVersionMajor()
{
    return atoi(API_VERSION);
}

int Printer_pi::GetAPIVersionMinor()
{
    std::string v(API_VERSION);
    size_t dotpos = v.find('.');
    return atoi(v.substr(dotpos + 1).c_str());
}

int Printer_pi::GetPlugInVersionMajor()
{
    return PLUGIN_VERSION_MAJOR;
}

int Printer_pi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

int GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }

int GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; };

const char *GetPlugInVersionPre() { return PKG_PRERELEASE; }

const char *GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap* Printer_pi::GetPlugInBitmap() { return &m_panelBitmap; }

wxString Printer_pi::GetCommonName() { return _("Printing 2.0"); }

wxString Printer_pi::GetShortDescription() { return _("Extended chart printing"); }

wxString Printer_pi::GetLongDescription() { return _("Make your own paper charts"); }

int Printer_pi::GetToolbarToolCount(void) { return 1; }

void Printer_pi::SetColorScheme(PI_ColorScheme cs)
{
    if (NULL == m_pDialog)
        return;

    DimeWindow(m_pDialog);
}
bool Printer_pi::RenderOverlay(wxDC &dc, PlugIn_ViewPort *vp)
{
  if (NULL != m_pDialog )
  {
    wxBitmap bm(dc.GetSize().x, dc.GetSize().y);
    m_mDC.SelectObject(bm);
    m_mDC.Blit(0,0, dc.GetSize().x, dc.GetSize().y, &dc, 0,0);
    CopyVp(vp);
    vpl = vp;
    m_pDialog->render(&m_mDC);
  }
  return true;
}
bool Printer_pi::RenderGLOverlay(wxGLContext *pcontext, PlugIn_ViewPort *vp)
{
  if (NULL != m_pDialog )
  {
//     wxString m("For this plugin you need to switch of OpenGL");
//     wxMessageDialog* MDlg = new wxMessageDialog (m_parent_window, m);
//     MDlg->ShowModal();
    wxClientDC Odc(m_parent_window);
    //keep a copy of the dc for use without update from O
    wxBitmap bm(Odc.GetSize().x, Odc.GetSize().y);
    m_mDC.SelectObject(bm);
    m_mDC.Blit(0,0, Odc.GetSize().x, Odc.GetSize().y, &Odc, 0,0);
    CopyVp(vp);
    // with Opengl the viewprot is not always exact the window so we do some improvement
//     g_vp_pix_width = Odc.GetSize().GetWidth();
//     g_vp_pix_height = Odc.GetSize().GetHeight();
//     GetCanvasLLPix(vp, wxPoint(0,0), &g_vp_lat_max, &g_vp_lon_min);
//     GetCanvasLLPix(vp, wxPoint(g_vp_pix_width, g_vp_pix_height), &g_vp_lat_min, &g_vp_lon_max);
//                                             double *plat, double *plon);
//     chartpix_to_latlong(0, 0, &g_vp_lat_max,&g_vp_lon_min);
//     chartpix_to_latlong(g_vp_pix_width, g_vp_pix_height, &g_vp_lat_min, &g_vp_lon_max);

    m_pDialog->render(&m_mDC);
   }
  return true;
}
void Printer_pi::CopyVp(PlugIn_ViewPort *vpS)
{
  g_vp_clat= vpS->clat;  // center point
  g_vp_clon = vpS->clon;
  g_vp_view_scale_ppm = vpS->view_scale_ppm;
  g_vp_skew = vpS->skew;
  g_vp_rotation = vpS->rotation;
  g_vp_chart_scale = vpS->chart_scale;
  g_vp_pix_width = vpS->pix_width;
  g_vp_pix_height = vpS->pix_height;
  g_vp_rv_rect = vpS->rv_rect;
  g_vp_b_quilt = vpS->b_quilt;
  g_vp_m_projection_type = vpS->m_projection_type;
  g_vp_lat_min= vpS->lat_min;
  g_vp_lat_max= vpS->lat_max;
  g_vp_lon_min= vpS->lon_min;
  g_vp_lon_max= vpS->lon_max;
  g_vp_bValid= vpS->bValid;
}


void Printer_pi::ShowPreferencesDialog(wxWindow* parent)
{
    printingPreferences* Pref = new printingPreferences(parent);

    Pref->m_cbTransmitAis->SetValue(m_bCopyUseAis);
    Pref->m_cbAisToFile->SetValue(m_bCopyUseFile);
    Pref->m_textCtrlMMSI->SetValue(m_tCopyMMSI);

    if (Pref->ShowModal() == wxID_OK) {

        bool copyAis = Pref->m_cbTransmitAis->GetValue();
        bool copyFile = Pref->m_cbAisToFile->GetValue();
        wxString copyMMSI = Pref->m_textCtrlMMSI->GetValue();

        if (m_bCopyUseAis != copyAis || m_bCopyUseFile != copyFile
            || m_tCopyMMSI != copyMMSI) {
            m_bCopyUseAis = copyAis;
            m_bCopyUseFile = copyFile;
            m_tCopyMMSI = copyMMSI;
        }

        SaveConfig();

        RequestRefresh(m_parent_window); // refresh main window
    }

        delete Pref;
        Pref = NULL;
}

void Printer_pi::OnToolbarToolCallback(int id)
{

    if (NULL == m_pDialog) {
      m_pDialog = new Dlg(m_parent_window);
        m_pDialog->plugin = this;
        m_bShowDlg = false;
    }

    m_bShowDlg = !m_bShowDlg;

    //    Toggle dialog?
    if (m_bShowDlg) {
        m_pDialog->Show();

    } else {
        m_pDialog->Hide();
    }
}

bool Printer_pi::LoadConfig(void)
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (pConf) {
        pConf->SetPath("/PlugIns/Printer_pi");
        wxString temp;
        pConf->Read("BorderFont", &temp);
        m_BorderFont.SetNativeFontInfo(temp);
        pConf->Read("LegendaFont", &temp);
        m_LegendaFont.SetNativeFontInfo(temp);
        pConf->Read("NotesFont", &temp);
        m_NotesFont.SetNativeFontInfo(temp);
        g_i_PaperSelection = pConf->Read("PaperSizeSelection", 2L);


//         pConf->Read("ShowShipDriverIcon", &m_bPrintingShowIcon, 1);
//         pConf->Read("shipdriverUseAis", &m_bCopyUseAis, 0);
//         pConf->Read("shipdriverUseFile", &m_bCopyUseFile, 0);
//         m_tCopyMMSI = pConf->Read("shipdriverMMSI", "123456789");
//
//         m_hr_dialog_x = pConf->Read("DialogPosX", 40L);
//         m_hr_dialog_y = pConf->Read("DialogPosY", 140L);
//         m_hr_dialog_sx = pConf->Read("DialogSizeX", 330L);
// #ifdef __WXOSX__
//         m_hr_dialog_sy = pConf->Read("DialogSizeY", 250L);
// #else
//         m_hr_dialog_sy = pConf->Read("DialogSizeY", 300L);
// #endif
//         if ((m_hr_dialog_x < 0) || (m_hr_dialog_x > m_display_width))
//             m_hr_dialog_x = 40;
//         if ((m_hr_dialog_y < 0) || (m_hr_dialog_y > m_display_height))
//             m_hr_dialog_y = 140;
//
        return true;
    } else
        return false;
}

bool Printer_pi::SaveConfig(void)
{
    wxFileConfig* pConf = (wxFileConfig*)m_pconfig;

    if (pConf) {
        pConf->SetPath("/PlugIns/Printer_pi");
        pConf->Write("BorderFont", m_BorderFont.GetNativeFontInfoDesc());
        pConf->Write("LegendaFont", m_LegendaFont.GetNativeFontInfoDesc());
        pConf->Write("NotesFont", m_NotesFont.GetNativeFontInfoDesc());
        pConf->Write("PaperSizeSelection",g_i_PaperSelection);
//         m_LegendaFont;
//         wxFont m_NotesFont
//         pConf->Write("SBorderFontSize", m_BorderFontSize);
//         pConf->Write("shipdriverUseFile", m_bCopyUseFile);
//         pConf->Write("shipdriverMMSI", m_tCopyMMSI);
//
//         pConf->Write("DialogPosX", m_hr_dialog_x);
//         pConf->Write("DialogPosY" , m_hr_dialog_y);
//         pConf->Write("DialogSizeX", m_hr_dialog_sx);
//         pConf->Write("DialogSizeY", m_hr_dialog_sy);

        return true;
    } else
        return false;
}

void Printer_pi::OnShipDriverDialogClose()
{
    m_bShowDlg = false;
    SetToolbarItemState(m_leftclick_tool_id, m_bShowDlg);
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window
}

wxPoint Printer_pi::GetLLPix(double lat, double lon)
{
  wxPoint LL;
  GetCanvasPixLL( vpl, &LL, lat, lon);
  return LL;
}
