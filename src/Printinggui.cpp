

#include "Printinggui.h"
#include <wx/paper.h>
#include "ocpn_plugin.h"
#include "pidc.h"
#include <wx/dcclient.h>
//#include "Printing_pi.h"


///////////////////////////////////////////////////////////////////////////
//(*InternalHeaders(PrintingBase)
#include <wx/intl.h>
#include <wx/string.h>
//*)
extern int g_i_PaperSelection;
extern double g_DisplaySizeMM;
extern wxCoord g_DispalySizePix;
extern double g_DisplayPixMM;
extern wxFont m_BorderFont;
extern wxFont m_LegendaFont;
extern wxFont m_NotesFont;

//(*IdInit(PrintingBase)
const long  PrintingBase::ID_SCROLLEDWINDOW1 = wxNewId();
const long PrintingBase::ID_PANEL1 = wxNewId();
const long PrintingBase::ID_CHECKBOX1 = wxNewId();
const long PrintingBase::ID_CHECKBOX2 = wxNewId();
const long PrintingBase::ID_CHECKBOX3 = wxNewId();
const long PrintingBase::ID_CHECKBOX4 = wxNewId();
const long PrintingBase::ID_COMBOBOX1 = wxNewId();
const long PrintingBase::ID_RADIOBOX1 = wxNewId();
const long PrintingBase::ID_FONTPICKERCTRL1 = wxNewId();
const long PrintingBase::ID_PANEL4 = wxNewId();
const long PrintingBase::ID_SLIDER1 = wxNewId();
const long PrintingBase::ID_SLIDER2 = wxNewId();
const long PrintingBase::ID_STATICTEXT1 = wxNewId();
const long PrintingBase::ID_TEXTCTRL1 = wxNewId();
const long PrintingBase::ID_STATICTEXT2 = wxNewId();
const long PrintingBase::ID_TEXTCTRL2 = wxNewId();
const long PrintingBase::ID_FONTPICKERCTRL2 = wxNewId();
const long PrintingBase::ID_PANEL5 = wxNewId();
const long PrintingBase::ID_PANEL2 = wxNewId();
const long PrintingBase::ID_NOTEBOOK1 = wxNewId();
const long PrintingBase::ID_SAVEBTN = wxNewId();
const long PrintingBase::ID_CANCELBTN = wxNewId();
//*)

BEGIN_EVENT_TABLE(PrintingBase,wxDialog)
//(*EventTable(PrintingBase)
//*)
END_EVENT_TABLE()

PrintingBase::PrintingBase(wxWindow* parent1, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent1, id, title, pos, size, style )
{
  parent = parent1;
  //(*Initialize(PrintingBase)
  wxBoxSizer* ButtonSizer;
  wxFlexGridSizer* BorderP_FGSizer;
  wxGridBagSizer* GridBagSizer1;
  wxGridBagSizer* GridBagSizer2;
  wxStaticBoxSizer* StaticBoxSizer1;
  wxStaticBoxSizer* StaticBoxSizer2;
  wxStaticBoxSizer* StaticBoxSizer3;

  SetClientSize(wxSize(703,631));
  Move(wxDefaultPosition);
  GridBagSizer1 = new wxGridBagSizer(0, 0);
  Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
  Panel1->SetMinSize(wxSize(200,150));
  PreviewWin1 = new PreviewWin(Panel1, ID_SCROLLEDWINDOW1, wxPoint(50,50), wxDefaultSize, wxBORDER_DOUBLE|wxBORDER_SUNKEN|wxVSCROLL|wxHSCROLL, _T("ID_SCROLLEDWINDOW1"));
  GridBagSizer1->Add(Panel1, wxGBPosition(0, 0), wxGBSpan(2, 1), wxALL|wxEXPAND, 5);
  Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_NOTEBOOK1"));
  BorderPanel = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
  BorderP_FGSizer = new wxFlexGridSizer(0, 1, 0, 0);
  StaticBoxSizer1 = new wxStaticBoxSizer(wxVERTICAL, BorderPanel, _("Chart Borders"));
  TopCB = new wxCheckBox(BorderPanel, ID_CHECKBOX1, _("Top"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
  TopCB->SetValue(false);
  StaticBoxSizer1->Add(TopCB, 1, wxALL|wxALIGN_LEFT, 0);
  LeftCB = new wxCheckBox(BorderPanel, ID_CHECKBOX2, _("Left"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
  LeftCB->SetValue(false);
  StaticBoxSizer1->Add(LeftCB, 1, wxALL|wxALIGN_LEFT, 0);
  RightCB = new wxCheckBox(BorderPanel, ID_CHECKBOX3, _("Right"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
  RightCB->SetValue(false);
  StaticBoxSizer1->Add(RightCB, 1, wxALL|wxALIGN_LEFT, 0);
  BottemCB = new wxCheckBox(BorderPanel, ID_CHECKBOX4, _("Bottem"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
  BottemCB->SetValue(false);
  StaticBoxSizer1->Add(BottemCB, 1, wxALL|wxALIGN_LEFT, 0);
  BorderP_FGSizer->Add(StaticBoxSizer1, 1, wxALL|wxEXPAND, 5);
  StaticBoxSizer2 = new wxStaticBoxSizer(wxVERTICAL, BorderPanel, _("Paper "));
  size_t      n = wxThePrintPaperDatabase->GetCount();
      wxString   *choices = new wxString [n];
      for (size_t i = 0; i < n; i++)
      {
        wxPrintPaperType *paper = wxThePrintPaperDatabase->Item(i);
        choices[i] = paper->GetName().Left(paper->GetName().Find(','));
      }

  PaperSizeCombo = new wxComboBox(BorderPanel, ID_COMBOBOX1, _("Paper Size"), wxDefaultPosition, wxDefaultSize, n, choices, wxCB_READONLY);
    PaperSizeCombo->SetValue(choices[g_i_PaperSelection]);
  StaticBoxSizer2->Add(PaperSizeCombo, 1, wxALL|wxEXPAND, 5);
  wxString __wxRadioBoxChoices_1[2] =
  {
    _("Portrait"),
    _("Landscape")
  };
  RadioBox1 = new wxRadioBox(BorderPanel, ID_RADIOBOX1, _("Orientation"), wxDefaultPosition, wxDefaultSize, 2, __wxRadioBoxChoices_1, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX1"));
  StaticBoxSizer2->Add(RadioBox1, 1, wxALL|wxEXPAND, 5);
  BorderP_FGSizer->Add(StaticBoxSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  StaticBoxSizer3 = new wxStaticBoxSizer(wxHORIZONTAL, BorderPanel, _("Border Font"));
  FontPickerCtrl1 = new wxFontPickerCtrl(BorderPanel, ID_FONTPICKERCTRL1, m_BorderFont, wxDefaultPosition, wxDefaultSize, wxFNTP_FONTDESC_AS_LABEL|wxFNTP_USEFONT_FOR_LABEL, wxDefaultValidator, _T("ID_FONTPICKERCTRL1"));
  StaticBoxSizer3->Add(FontPickerCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  BorderP_FGSizer->Add(StaticBoxSizer3, 1, wxALL|wxEXPAND, 5);
  BorderPanel->SetSizer(BorderP_FGSizer);
  BorderP_FGSizer->Fit(BorderPanel);
  BorderP_FGSizer->SetSizeHints(BorderPanel);


  LegendaPanel = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
  GridBagSizer2 = new wxGridBagSizer(0, 0);
  Slider1 = new wxSlider(LegendaPanel, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxSize(176,21), wxSL_HORIZONTAL|wxHSCROLL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_SLIDER1"));
  Slider1->SetSelection(0, 100);
  GridBagSizer2->Add(Slider1, wxGBPosition(0, 1), wxGBSpan(1, 5), wxEXPAND, 0);
  Slider2 = new wxSlider(LegendaPanel, ID_SLIDER2, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL|wxVSCROLL|wxALWAYS_SHOW_SB, wxDefaultValidator, _T("ID_SLIDER2"));
  GridBagSizer2->Add(Slider2, wxGBPosition(1, 0), wxGBSpan(5, 1), wxALL|wxEXPAND, 0);
  StaticText1 = new wxStaticText(LegendaPanel, ID_STATICTEXT1, _("Title"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
  GridBagSizer2->Add(StaticText1, wxGBPosition(1, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  TextCtrl1 = new wxTextCtrl(LegendaPanel, ID_TEXTCTRL1, _("Text"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
  GridBagSizer2->Add(TextCtrl1, wxGBPosition(1, 2), wxGBSpan(1, 3), wxALL|wxEXPAND, 5);
  StaticText2 = new wxStaticText(LegendaPanel, ID_STATICTEXT2, _("Sub.\nTitle"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
  GridBagSizer2->Add(StaticText2, wxGBPosition(2, 1), wxDefaultSpan, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  TextCtrl2 = new wxTextCtrl(LegendaPanel, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL2"));
  GridBagSizer2->Add(TextCtrl2, wxGBPosition(2, 2), wxGBSpan(1, 3), wxALL|wxEXPAND, 5);
  FontPickerCtrl2 = new wxFontPickerCtrl(LegendaPanel, ID_FONTPICKERCTRL2, m_LegendaFont, wxDefaultPosition, wxDefaultSize, wxFNTP_FONTDESC_AS_LABEL|wxFNTP_USEFONT_FOR_LABEL, wxDefaultValidator, _T("ID_FONTPICKERCTRL2"));
  GridBagSizer2->Add(FontPickerCtrl2, wxGBPosition(5, 1), wxGBSpan(1, 4), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  LegendaPanel->SetSizer(GridBagSizer2);
  GridBagSizer2->Fit(LegendaPanel);
  GridBagSizer2->SetSizeHints(LegendaPanel);



  NotesPanel = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
  Notebook1->AddPage(BorderPanel, _("Borders"), true);
  Notebook1->AddPage(LegendaPanel, _("Legenda"), false);
  Notebook1->AddPage(NotesPanel, _("Notes"), false);
  GridBagSizer1->Add(Notebook1, wxGBPosition(0, 1), wxDefaultSpan, wxALL|wxEXPAND, 5);
  ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  SaveBtn = new wxButton(this, ID_SAVEBTN, _("Save"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_SAVEBTN"));
  ButtonSizer->Add(SaveBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  CancelBtn = new wxButton(this, ID_CANCELBTN, _("Label"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_CANCELBTN"));
  ButtonSizer->Add(CancelBtn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
  GridBagSizer1->Add(ButtonSizer, wxGBPosition(1, 1), wxDefaultSpan, wxALL|wxEXPAND, 5);
  GridBagSizer1->AddGrowableCol(0);
  GridBagSizer1->AddGrowableRow(0);
  SetSizer(GridBagSizer1);
  FileDialog1 = new wxFileDialog(this, _("Select file"), wxEmptyString, wxEmptyString, _("*.png"), wxFD_DEFAULT_STYLE|wxFD_SAVE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
  SetSizer(GridBagSizer1);
  Layout();

  //Panel1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PrintingBase::OnPreviewPaint,0,this);
  PreviewWin1->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PrintingBase::OnPreviewPaint,0,this);
  Connect(ID_CHECKBOX1,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PrintingBase::CheckBoxClick);
  Connect(ID_CHECKBOX2,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PrintingBase::CheckBoxClick);
  Connect(ID_CHECKBOX3,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PrintingBase::CheckBoxClick);
  Connect(ID_CHECKBOX4,wxEVT_COMMAND_CHECKBOX_CLICKED,(wxObjectEventFunction)&PrintingBase::CheckBoxClick);
  //Connect(ID_COMBOBOX1,wxEVT_COMMAND_COMBOBOX_SELECTED,(wxObjectEventFunction)&PrintingBase::OnComboBox1Selected);
  Connect(ID_COMBOBOX1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PrintingBase::OnComboBox1Selected);
  Connect(ID_RADIOBOX1,wxEVT_COMMAND_RADIOBOX_SELECTED,(wxObjectEventFunction)&PrintingBase::OnRadioBox1Selected);
  Connect(ID_FONTPICKERCTRL1,wxEVT_COMMAND_FONTPICKER_CHANGED,(wxObjectEventFunction)&PrintingBase::OnBorderFontPickerChanged);
  Connect(ID_FONTPICKERCTRL2,wxEVT_COMMAND_FONTPICKER_CHANGED,(wxObjectEventFunction)&PrintingBase::OnBorderFontPickerChanged);
  Connect(ID_SAVEBTN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PrintingBase::OnSaveButtonClick);
  Connect(wxID_ANY,wxEVT_INIT_DIALOG,(wxObjectEventFunction)&PrintingBase::OnInitDlg);


  delete[] choices;
}

PrintingBase::~PrintingBase()
{
  //(*Destroy(PrintingBase)
  //*)
}


printingPreferences::printingPreferences( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("AIS") ), wxVERTICAL );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_cbTransmitAis = new wxCheckBox( sbSizer1->GetStaticBox(), wxID_ANY, _("Transmit"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_cbTransmitAis, 0, wxALL, 5 );

	m_cbAisToFile = new wxCheckBox( sbSizer1->GetStaticBox(), wxID_ANY, _("Save to file"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_cbAisToFile, 0, wxALL, 5 );


	sbSizer1->Add( fgSizer2, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( sbSizer1->GetStaticBox(), wxID_ANY, _("MMSI") ), wxVERTICAL );

	m_textCtrlMMSI = new wxTextCtrl( sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_textCtrlMMSI, 0, wxALL, 5 );


	sbSizer1->Add( sbSizer2, 1, wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( sbSizer1->GetStaticBox(), wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( sbSizer1->GetStaticBox(), wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	sbSizer1->Add( m_sdbSizer1, 1, wxEXPAND, 5 );


	this->SetSizer( sbSizer1 );
	this->Layout();
	sbSizer1->Fit( this );

	this->Centre( wxBOTH );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
wxBEGIN_EVENT_TABLE(PreviewWin, wxScrolledWindow)
EVT_PAINT(PreviewWin::OnPaint)
EVT_ERASE_BACKGROUND(PreviewWin::OnEraseBackground)
EVT_MOUSE_EVENTS(PreviewWin::OnMouseEvent)
wxEND_EVENT_TABLE()

PreviewWin::PreviewWin( wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long	style, const wxString &name )
                      : wxScrolledWindow( parent, id,  pos, size, style, name )
{
  parentW = parent;

}
PreviewWin::~PreviewWin(){}

wxSize PreviewWin::SetSize(const int x, const int y)
{
  double maxSize = 0.9; //Max size compared to parent size
  wxCoord newWidth =  x;
  wxCoord newHeight = y;
  if (newWidth > (wxCoord)parentW->GetClientSize().GetWidth() * maxSize){
    double scalefact = (double)((parentW->GetClientSize().GetWidth() * maxSize))/newWidth ;
    newWidth =(wxCoord) newWidth * scalefact ;
    newHeight =(wxCoord) newHeight * scalefact ;
  }
  if (newHeight > (wxCoord)parentW->GetClientSize().GetHeight() * maxSize){
    double scalefact2 = (double)(parentW->GetClientSize().GetHeight() * maxSize)/newHeight;
    newWidth =(wxCoord) newWidth * scalefact2 ;
    newHeight =(wxCoord) newHeight * scalefact2 ;
  }
  wxCoord PosX = (wxCoord)(parentW->GetClientSize().GetWidth() - newWidth)/2;
  wxCoord PosY = (wxCoord)(parentW->GetClientSize().GetHeight() - newHeight)/2;
  wxScrolledWindow::SetSize(PosX, PosY, newWidth, newHeight);
  return wxSize(newWidth, newHeight);
}

printingPreferences::~printingPreferences()
{
}



