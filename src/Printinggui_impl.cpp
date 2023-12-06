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
#include <stdio.h>

#include <wx/progdlg.h>
#include <wx/textfile.h>
#include <wx/timer.h>
#include <wx/wx.h>
#include <wx/wfstream.h>
#include "wx/tglbtn.h"

#include "qtstylesheet.h"
#include "Printinggui_impl.h"
#include "Printing_pi.h"
#include <pidc.h>
#include <wx/paper.h>
#include "ChartBorders.h"
#include "Legenda.h"



class chartborderCalc;
// #ifdef __ANDROID__
//   wxWindow *g_Window;
// #endif

extern int g_i_PaperSelection;
extern double g_DisplaySizeMM;
extern wxCoord g_DispalySizePix;
extern double g_DisplayPixMM;
extern wxFont m_BorderFont;
extern wxFont m_LegendaFont;
extern wxFont m_NotesFont;
extern wxSize PaperSizePix;
extern int BorderStyle;

Dlg::Dlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : PrintingBase(parent, id, title,  pos, size, style )
{
  First = new ChartMem();
  First->SetSizeMax(GetPaperSize(g_i_PaperSelection));
  Second = new ChartBorderCalc();
  First->AddChild(Second);
  Third = new Legenda();
  Second->AddChild(Third);
}
Dlg::~Dlg()
{
  if (First) delete First;
}

void Dlg::CheckBoxClick(wxCommandEvent& event)
{
  BorderStyle = TopCB->GetValue() * TOPBORDER +
  LeftCB->GetValue() * LEFTBORDER +
  RightCB->GetValue() * RIGHTBORDER +
  BottemCB->GetValue() * BOTTOMBORDER;
  render();
  event.Skip();
}
void Dlg::OnComboBox1Selected()
{
  g_i_PaperSelection = (PaperSizeCombo->GetCurrentSelection());
  First->SetSizeMax(GetPaperSize(g_i_PaperSelection));
  if(First->GetLastDC()->IsOk()){
    render();
    //renderPreview();
  }
  else RequestRefresh(plugin->m_parent_window);

}
void Dlg::OnRadioBox1Selected(wxCommandEvent& event)
{
  OnComboBox1Selected(event);
  event.Skip();
}
void Dlg::OnBorderFontPickerChanged(wxCommandEvent& event)
{
  m_BorderFont.SetNativeFontInfo(FontPickerCtrl1->GetSelectedFont().GetNativeFontInfoDesc());
  m_LegendaFont.SetNativeFontInfo(FontPickerCtrl2->GetSelectedFont().GetNativeFontInfoDesc());
  render();
  event.Skip();
}

void Dlg::OnPreviewPaint(wxPaintEvent& event)
{
  render();
  event.Skip();
}
void Dlg::OnSaveButtonClick(wxCommandEvent& event)
{
  wxCoord w, h;
  First->GetLastDC()->GetSize(&w, &h);
  wxBitmap bm(w, h);
  wxMemoryDC* mdc = new wxMemoryDC();
  mdc->SelectObject(bm);
  mdc->Blit(0,0, First->GetLastDC()->GetSize().GetWidth(), First->GetLastDC()->GetSize().GetHeight(), First->GetLastDC(), 0, 0);

  wxFileDialog
  saveFileDialog(this, _("Save chart file"), "", "",
                 "PNG files (*.png)|*.png", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...

    // save the current contents in the file;
    // this can be done with e.g. wxWidgets output streams:
    //wxFileOutputStream output_stream(saveFileDialog.GetPath());
  wxString fn(saveFileDialog.GetPath());
  bm.SaveFile(fn, wxBITMAP_TYPE_PNG ) ;

}

wxSize Dlg::GetPaperSize(int Selected)
{
  g_i_PaperSelection = Selected;
  PaperSizeMM = wxThePrintPaperDatabase->Item(Selected)->GetSize();
  if(RadioBox1->GetSelection()!=0){ //swap if portrait orientation
    PaperSizeMM= wxSize( PaperSizeMM.GetHeight(), PaperSizeMM.GetWidth());
  }
  return wxSize((int)(0.1*g_DisplayPixMM*PaperSizeMM.x), (int)(0.1*g_DisplayPixMM*PaperSizeMM.y)) ;
}

void Dlg::render()
{
  if(IsShown() ){
    if(First->GetLastDC()->IsOk()){
      First->SetSizeMax(GetPaperSize(g_i_PaperSelection));
      First->UpdateDC();
      renderPreview();
    }else{
      RequestRefresh(plugin->m_parent_window);
    }
  }
}
void Dlg::render(wxDC* dc)
{
  if(IsShown() ){
    First->SetSizeMax(GetPaperSize(g_i_PaperSelection));
    First->UpdateDCin(dc);
    renderPreview();
  }
}

void Dlg::renderPreview()
{
  PreviewWin1->SetSize(First->GetLastDC()->GetSize());
  wxClientDC PvWinDC( PreviewWin1);
  PvWinDC.Clear();
  PvWinDC.StretchBlit(0,0,
                      PvWinDC.GetSize().x,
                      PvWinDC.GetSize().y,
                      First->GetLastDC(),0,0,
                      First->GetLastDC()->GetSize().x,
                      First->GetLastDC()->GetSize().y);
}
