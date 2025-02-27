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

#pragma once

//#include "Printing_pi.h"
//#include "pidc.h"
//#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/colour.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/fontpicker.h>
#include <wx/gauge.h>
#include <wx/gbsizer.h>
#include <wx/gdicmn.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/scrolwin.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
//#include <wx/timer.h>
//#include <wx/xrc/xmlres.h>

//*)

class PreviewWin;
class PrintingBase: public wxDialog
{
public:

  PrintingBase(wxWindow* parent1, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long int style = wxDEFAULT_DIALOG_STYLE);
  virtual ~PrintingBase();

  //(*Declarations(PrintingBase)
  wxButton* SaveBtn;
  wxButton* CancelBtn;
  wxCheckBox* BottemCB;
  wxCheckBox* LeftCB;
  wxCheckBox* RightCB;
  wxCheckBox* TopCB;
  wxComboBox* PaperSizeCombo;
  wxChoice* Choice1;
  wxFileDialog* FileDialog1;
  wxFontPickerCtrl* FontPickerCtrl1;
  wxFontPickerCtrl* FontPickerCtrl2;
  wxNotebook* Notebook1;
  wxPanel* BorderPanel;
  wxPanel* LegendaPanel;
  wxPanel* NotesPanel;
  wxPanel* Panel1;
  wxRadioBox* RadioBox1;
  wxSlider* Slider1;
  wxSlider* Slider2;
  wxStaticText* StaticText1;
  wxTextCtrl* TextCtrl1;
  wxStaticText* StaticText2;
  wxStaticText* StaticText3;
  wxTextCtrl* TextCtrl2;
  PreviewWin* PreviewWin1;
  //*)

protected:

  //(*Identifiers(PrintingBase)
  static const long ID_SCROLLEDWINDOW1;
  static const long ID_PANEL1;
  static const long ID_CHECKBOX1;
  static const long ID_CHECKBOX2;
  static const long ID_CHECKBOX3;
  static const long ID_CHECKBOX4;
  static const long ID_STATICTEXT3;
  static const long ID_CHOICE1;
  static const long ID_COMBOBOX1;
  static const long ID_RADIOBOX1;
  static const long ID_FONTPICKERCTRL1;
  static const long ID_PANEL4;
  static const long ID_SLIDER1;
  static const long ID_SLIDER2;
  static const long ID_STATICTEXT1;
  static const long ID_TEXTCTRL1;
  static const long ID_STATICTEXT2;
  static const long ID_TEXTCTRL2;
  static const long ID_FONTPICKERCTRL2;
  static const long ID_PANEL5;
  static const long ID_PANEL2;
  static const long ID_NOTEBOOK1;
  static const long ID_SAVEBTN;
  static const long ID_CANCELBTN;
  //*)
    wxWindow* parent;
    wxSize PaperSizeMM= wxSize(2970,2100);
private:

  //(*Handlers(PrintingBase)
  virtual void OnTopCheckBoxClick(wxCommandEvent& event){event.Skip();}
  virtual void CheckBoxClick(wxCommandEvent& event){event.Skip();}
  virtual void OnPreviewPaint(wxPaintEvent& event){event.Skip();}
  virtual void OnComboBox1Selected(wxCommandEvent& event){event.Skip();}
  virtual void OnRadioBox1Selected(wxCommandEvent& event){event.Skip();}
  virtual void OnBorderFontPickerChanged(wxCommandEvent& event){event.Skip();}
  virtual void OnInitDlg(wxInitDialogEvent& event){event.Skip();}
  virtual void OnSaveButtonClick(wxCommandEvent& event){event.Skip();}
  virtual void OnTextCtrlText(wxCommandEvent& event){event.Skip();}
  virtual void OnSliderCmdScroll(wxScrollEvent& event){event.Skip();}
  //*)



  DECLARE_EVENT_TABLE()
};
class PreviewWin: public wxScrolledWindow
{
public:
  PreviewWin( wxWindow *  	parent,
              wxWindowID  	id = -1,
              const wxPoint &  	pos = wxDefaultPosition,
              const wxSize &  	size = wxDefaultSize,
              long  	style = wxHSCROLL|wxVSCROLL,
              const wxString &  	name = "scrolledWindow");
  ~PreviewWin();
//   virtual void OnPaint( wxPaintEvent &event ){event.Skip();}
//   virtual void OnEraseBackground(wxEraseEvent& event){event.Skip();}
//   virtual void OnMouseEvent(wxMouseEvent& event){event.Skip();}
  virtual wxSize SetSize(wxSize si){return SetSize(si.x, si.y);}
  virtual wxSize SetSize(const int x, const int y);

  wxWindow *  	parentW;
private:
//   wxDECLARE_EVENT_TABLE();
};

///////////////////////////////////////////////////////////////////////////////
/// Class printingPreferences
///////////////////////////////////////////////////////////////////////////////
class printingPreferences : public wxDialog
{
	private:

	protected:
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;

	public:
		wxCheckBox* m_cbTransmitAis;
		wxCheckBox* m_cbAisToFile;
		wxTextCtrl* m_textCtrlMMSI;

    printingPreferences( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~printingPreferences();

};

