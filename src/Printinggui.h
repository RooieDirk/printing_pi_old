///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

//#include "Printing_pi.h"
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/gauge.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/tglbtn.h>
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/timer.h>
#include <wx/dialog.h>
#include <wx/checkbox.h>
#include <wx/gbsizer.h>
#include <wx/combobox.h>
#include <wx/gbsizer.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include "pidc.h"



///////////////////////////////////////////////////////////////////////////
// (wxWindow *parent,
//  wxWindowID winid = wxID_ANY,
//  const wxPoint& pos = wxDefaultPosition,
//  const wxSize& size = wxDefaultSize,
//  long style = wxTAB_TRAVERSAL | wxNO_BORDER,
//  const wxString& name = wxASCII_STR(wxPanelNameStr))
// //class Printer_pi;
///////////////////////////////////////////////////////////////////////////////

/// Class PrintingBase
///////////////////////////////////////////////////////////////////////////////
//(*Headers(PrintingBase)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/dialog.h>
#include <wx/filedlg.h>
#include <wx/fontpicker.h>
#include <wx/gbsizer.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/scrolwin.h>
//*)

class PreviewWin;
class PrintingBase: public wxDialog
{
public:

  PrintingBase(wxWindow* parent1, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long int style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
  virtual ~PrintingBase();

  //(*Declarations(PrintingBase)
  wxButton* SaveBtn;
  wxButton* CancelBtn;
  wxCheckBox* BottemCB;
  wxCheckBox* LeftCB;
  wxCheckBox* RightCB;
  wxCheckBox* TopCB;
  wxComboBox* PaperSizeCombo;
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
  virtual wxSize SetSize(wxSize si){return SetSize(si.x, si.y);}
  virtual wxSize SetSize(const int x, const int y);



  wxWindow *  	parentW;


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

