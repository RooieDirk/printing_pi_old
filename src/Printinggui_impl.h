// /******************************************************************************
//  *
//  * Project:  OpenCPN
//  * Purpose:  Printing Plugin
//  * Author:   RooieDirk
//  *
//  ***************************************************************************
//  *   Copyright (C) 2017 by RooieDirk                                   *
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
//
#pragma once

#ifndef _HRGUI_IMPL_H_
#define _HRGUI_IMPL_H_

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "Printing_pi.h"
#include "Printinggui.h"
#include "ocpn_plugin.h"
#include "ChartBorders.h"
#include "tinyxml.h"
#include "wx/process.h"
#include "json/reader.h"
#include "json/writer.h"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <wx/filedlg.h>
#include <wx/gdicmn.h>
#include <wx/listctrl.h>
#include <wx/stdpaths.h>
#include <wx/thread.h>
#include <wx/utils.h>
#include <wx/msgdlg.h>
#include <wx/arrstr.h>
#include <wx/vector.h>
#include <wx/dataobj.h>
#include <wx/list.h>
#include "ChartMem.h"

#ifdef __WXOSX__
#define SHIPDRIVER_DLG_STYLE                                                   \
    wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxSTAY_ON_TOP
#else
#define SHIPDRIVER_DLG_STYLE                                                   \
    wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER
#endif

class Printer_pi;
class Dlg : public PrintingBase
{
public:
    Dlg(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& title = _("Printing"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = SHIPDRIVER_DLG_STYLE);
    ~Dlg();
    Printer_pi* plugin;
    ChartMem* First;
    ChartMem* Second;
    ChartMem* Third;
    //wxWindow* parent;

#ifdef __ANDROID__
    void OnMouseEvent( wxMouseEvent& event );
#endif

     void OnTopCheckBoxClick(wxCommandEvent& event){ event.Skip(); }
     void CheckBoxClick(wxCommandEvent& event);
     void OnComboBox1Selected(wxCommandEvent& event){OnComboBox1Selected();}
     void OnComboBox1Selected();
     void OnRadioBox1Selected(wxCommandEvent& event);
     void OnPreviewPaint(wxPaintEvent& event);
     wxSize GetPaperSize(int Selected);
     void render();
     void render(wxDC* dc);
     void renderPreview();
     void OnBorderFontPickerChanged(wxCommandEvent& event);
     void OnSaveButtonClick(wxCommandEvent& event);
     void OnTextCtrlText(wxCommandEvent& event);
     void OnSliderCmdScroll(wxScrollEvent& event);
     //void OnInitDlg(wxInitDialogEvent& event);

    wxSize PaperSizeMM;
protected:


private:

    void OnClose(wxCloseEvent& event);
    //void SetupPreviewWin(piDC*  pidc);
};


#endif
