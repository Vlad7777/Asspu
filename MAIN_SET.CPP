#include <owl\owlpch.h>
#include <owl\printer.h>
#include <owl\choosefo.h>
#include <owl\chooseco.h>
#include <owl\control.h>
#include <owl\checkbox.h>
#include <owl\scrollba.h>
#include <owl\radiobut.h>
#include <owl\groupbox.h>
#include <owl/inputdia.h>
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "layout.h"
#include "mainw4.h"
#include "mainw4.rh"
#include "pack.h"
#include "cal_wind.h"

bool ChangeFont( TWindow* parent, LOGFONT& TheLogFont, COLORREF& TheColorRefFont )
	{
	TPrintDC* PrnDC;
	PrnDC = new TPrintDC( TMyApp::Printer->GetSetup().GetDriverName(),
								 TMyApp::Printer->GetSetup().GetDeviceName(),
								 TMyApp::Printer->GetSetup().GetOutputName(),
								 TMyApp::Printer->GetSetup().GetDevMode() );
	TChooseFontDialog::TData FontData;
	FontData.LogFont = TheLogFont;
	FontData.DC = *PrnDC;
	FontData.Flags = CF_ENABLETEMPLATE | CF_EFFECTS | CF_FORCEFONTEXIST | CF_SCREENFONTS | CF_BOTH | CF_WYSIWYG | CF_INITTOLOGFONTSTRUCT;		
	FontData.Color = TColor( TheColorRefFont );
	FontData.Style = 0;
	FontData.FontType = SCREEN_FONTTYPE;
	FontData.SizeMin = 0;
	FontData.SizeMax = 0;
	if ( TChooseFontDialog( parent, FontData ).Execute() == IDOK )
		{
		TheColorRefFont = FontData.Color;
		TheLogFont = FontData.LogFont;
		delete PrnDC;
		return TRUE;
		}
	delete PrnDC;
	return FALSE;	
	}

bool ChangeColor( TWindow* parent, COLORREF& TheColorRef )
	{
	TChooseColorDialog::TData choose;
	static TColor custColors[16] = { 0x010101L, 0x101010L, 0x202020L, 0x303030L,
												0x404040L, 0x505050L, 0x606060L, 0x707070L,
												0x808080L, 0x909090L, 0xA0A0A0L, 0xB0B0B0L,
												0xC0C0C0L, 0xD0D0D0L, 0xE0E0E0L, 0xF0F0F0L };
	choose.Flags = CC_RGBINIT;
	choose.Color = TheColorRef;
	choose.CustColors = custColors;
	if ( TChooseColorDialog( parent, choose ).Execute() == IDOK )
		{
		TheColorRef = choose.Color;
		return TRUE;		
		}
	return FALSE;	
	}

class TControlEvent : public TControl 
	{
	private:
		TEvent* Event;
	public:
		TControlEvent( TWindow* parent, int resId, TEvent* TheEvent );
	protected:
		char far* GetClassName() { return "ControlEvent"; }
		void EvPaint();
	DECLARE_RESPONSE_TABLE( TControlEvent );
	};

DEFINE_RESPONSE_TABLE1( TControlEvent, TControl )
	EV_WM_PAINT,
END_RESPONSE_TABLE;

TControlEvent::TControlEvent( TWindow* parent, int resId, TEvent* TheEvent )
	: TControl( parent, resId, ::Module )
	{ Event = TheEvent; }

void TControlEvent::EvPaint()
	{
	TPaintDC dc(*this);
	Event->Draw( dc, TLSize( 0, 0 ) );
	}

class TSetEventDialog : public TDialog
	{
	private:
		STR_SET_EVENT* StrSetEventTmp;
		TScrollBar* Scroll_Bar1;
		TScrollBar* Scroll_Bar2;
		TScrollBar* Scroll_Bar3;
		TControlEvent* ControlEvent;
	public:
		TSetEventDialog( TWindow* parent, int resId, STR_SET_EVENT* TheStrSetEvent, TEvent* TheEvent );
		void SetupWindow();
	protected:	
		void IdChangeFont();
		void IdChangeColorMain();
		void IdChangeColorCrit();
		void IdChangeColorResource();
		void IdChangeColorPercent();
		void SetHSize( UINT code );
		void SetVSizeBottom( UINT code );
		void SetVSizeTop( UINT code );

	DECLARE_RESPONSE_TABLE( TSetEventDialog );
	};

DEFINE_RESPONSE_TABLE1( TSetEventDialog, TDialog )
	EV_COMMAND( ID_SETFONT, IdChangeFont ),
	EV_COMMAND( ID_SETCOLORMAIN, IdChangeColorMain ),
	EV_COMMAND( ID_SETCOLORCRIT, IdChangeColorCrit ),
	EV_COMMAND( ID_SETCOLORRESOURCE, IdChangeColorResource ),
	EV_COMMAND( ID_SETCOLORPERCENT, IdChangeColorPercent ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_BAR1, SetHSize ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_BAR2, SetVSizeBottom ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_BAR3, SetVSizeTop ),
END_RESPONSE_TABLE;

TSetEventDialog::TSetEventDialog( TWindow* parent, int resId, STR_SET_EVENT* TheStrSetEvent, TEvent* TheEvent )
	: TDialog( parent, resId )
	{ 
	StrSetEventTmp = TheStrSetEvent; 
	Scroll_Bar1 = new TScrollBar( this, ID_BAR1 );
	Scroll_Bar2 = new TScrollBar( this, ID_BAR2 );
	Scroll_Bar3 = new TScrollBar( this, ID_BAR3 );
	ControlEvent = new TControlEvent( this, ID_EVENT, TheEvent );
	}

void TSetEventDialog::SetupWindow()
	{
	TWindow::SetupWindow();
	Scroll_Bar1->SetRange( 1, 100 );
	Scroll_Bar1->SetPosition( StrSetEventTmp->HConstSizeMDL );
	Scroll_Bar2->SetRange( 1, 100);
	Scroll_Bar2->SetPosition( StrSetEventTmp->VConstSizeTopMDL );
	Scroll_Bar3->SetRange( 1, 100);
	Scroll_Bar3->SetPosition( StrSetEventTmp->VConstSizeBottomMDL );
	}


void TSetEventDialog::SetHSize( UINT )
	{
	StrSetEventTmp->HConstSizeMDL = Scroll_Bar1->GetPosition(); 
	ControlEvent->Invalidate();
	}

void TSetEventDialog::SetVSizeBottom( UINT )
	{ 
	StrSetEventTmp->VConstSizeTopMDL = Scroll_Bar2->GetPosition(); 
	ControlEvent->Invalidate();
	}

void TSetEventDialog::SetVSizeTop( UINT )
	{ 
	StrSetEventTmp->VConstSizeBottomMDL = Scroll_Bar3->GetPosition(); 
	ControlEvent->Invalidate();
	}


void TSetEventDialog::IdChangeFont() 
	{
	if ( ChangeFont( this, StrSetEventTmp->LogFont, StrSetEventTmp->ColorRefFont ) )
		ControlEvent->Invalidate();
	}

void TSetEventDialog::IdChangeColorMain()
	{                                              
	if ( ChangeColor( this, StrSetEventTmp->LogPenMain.lopnColor ) )
		ControlEvent->Invalidate();
	}

void TSetEventDialog::IdChangeColorCrit()
	{
	if ( ChangeColor( this, StrSetEventTmp->LogPenCrit.lopnColor ) )
		ControlEvent->Invalidate();
	}

void TSetEventDialog::IdChangeColorResource()
	{                                            	
	if ( ChangeColor( this, StrSetEventTmp->ColorRefResource ) )
		ControlEvent->Invalidate();
	}

void TSetEventDialog::IdChangeColorPercent()
	{                                            	
	if ( ChangeColor( this, StrSetEventTmp->ColorRefPercent ) )
		ControlEvent->Invalidate();
	}


void TMyApp::CmSetWork()
	{
	STR_SET_SCALE StrSetScaleTmp = TEvent::StrSetScale;
	TEvent::StrSetScale.Day = 86400L;
	TEvent::StrSetScale.ScaleMDL = 1;
	TEvent::StrSetScale.VScaleGNT = 1;
	TEvent::StrSetScale.VScaleGST = 1;
	TEvent::StrSetScale.HScaleGNTGST = 3600;

	TEvent* Event = new TWork();
	Event->SetName( "Example work" );
	Event->Move( TLPoint( 100L, 100L ) );
	Event->SetNumber( 1UL );
	unsigned long StartTime = ( time( NULL ) / 86400L ) * 86400L ;
	Event->SetStartTime( StartTime );
	Event->SetFinishTime( StartTime );
	STR_SET_EVENT StrSetEventTmp = TWork::StrSetEvent;
	if ( TSetEventDialog( MainWindow, IDD_SETEVENT, &TWork::StrSetEvent, Event ).Execute() == IDOK )
		;
	else
		{
		TWork::StrSetEvent.HConstSizeMDL = StrSetEventTmp.HConstSizeMDL;
		TWork::StrSetEvent.VConstSizeTopMDL = StrSetEventTmp.VConstSizeTopMDL;
		TWork::StrSetEvent.VConstSizeBottomMDL = StrSetEventTmp.VConstSizeBottomMDL;
		TWork::StrSetEvent.VConstSizeTopGNT = StrSetEventTmp.VConstSizeTopGNT;
		TWork::StrSetEvent.VConstSizeBottomGNT = StrSetEventTmp.VConstSizeBottomGNT;
		}
	TEvent::StrSetScale = StrSetScaleTmp;
	delete ((TWork*)Event);
	}
	
void TMyApp::CmSetPack()
	{
	STR_SET_SCALE StrSetScaleTmp = TEvent::StrSetScale;
	TEvent::StrSetScale.Day = 86400UL;
	TEvent::StrSetScale.ScaleMDL = 1;
	TEvent::StrSetScale.VScaleGNT = 1;
	TEvent::StrSetScale.VScaleGST = 1;
	TEvent::StrSetScale.HScaleGNTGST = 3600;
	TEvent* Event = new TPack();
	Event->SetName( "Example packet" );
	Event->Move( TLPoint( 100L, 100L ) );
	Event->SetNumber( 1UL );
	STR_SET_EVENT StrSetEventTmp = TPack::StrSetEvent;
	if ( TSetEventDialog( MainWindow, IDD_SETEVENT, &TPack::StrSetEvent, Event ).Execute() == IDOK )
		;
	else
		{
		TPack::StrSetEvent.HConstSizeMDL = StrSetEventTmp.HConstSizeMDL;
		TPack::StrSetEvent.VConstSizeTopMDL = StrSetEventTmp.VConstSizeTopMDL;
		TPack::StrSetEvent.VConstSizeBottomMDL = StrSetEventTmp.VConstSizeBottomMDL;
		TPack::StrSetEvent.VConstSizeTopGNT = StrSetEventTmp.VConstSizeTopGNT;
		TPack::StrSetEvent.VConstSizeBottomGNT = StrSetEventTmp.VConstSizeBottomGNT;
		}
	TEvent::StrSetScale = StrSetScaleTmp;
	delete ((TPack*)Event);
	}


class TControlConnect : public TControl 
	{
	private:
		TConnect* Connect;
	public:
		TControlConnect( TWindow* parent, int resId, TConnect* TheConnect );
	protected:
		char far* GetClassName() { return "ControlConnect"; }
		void EvPaint();
	DECLARE_RESPONSE_TABLE( TControlConnect );
	};

DEFINE_RESPONSE_TABLE1( TControlConnect, TControl )
	EV_WM_PAINT,
END_RESPONSE_TABLE;

TControlConnect::TControlConnect( TWindow* parent, int resId, TConnect* TheConnect )
	: TControl( parent, resId, ::Module )
	{ Connect = TheConnect; }

void TControlConnect::EvPaint()
	{
	TLRect lRect;
	lRect = GetClientRect();
	TPaintDC dc(*this);
	Connect->Draw( dc, TLSize( 0, 0 ), &lRect );
	}

class TSetConnectDialog : public TDialog
	{
	private:
		STR_SET_CONNECT* StrSetConnectTmp;
		TScrollBar* Scroll_Bar1;
		TScrollBar* Scroll_Bar2;
		TControlConnect* ControlConnect;
	public:
		TSetConnectDialog( TWindow* parent, int resId, STR_SET_CONNECT* TheStrSetConnect, TConnect* TheConnect );
		void SetupWindow();
	protected:
		void IdChangeColorMain();
		void IdChangeColorCrit();
		void IdChangePenMain();
		void IdChangePenCrit();
		void SetSizeArrow( UINT code );
		void SetAngjeArrow( UINT code );

	DECLARE_RESPONSE_TABLE( TSetConnectDialog );
	};

DEFINE_RESPONSE_TABLE1( TSetConnectDialog, TDialog )
	EV_COMMAND( ID_SETCOLORMAIN, IdChangeColorMain ),
	EV_COMMAND( ID_SETCOLORCRIT, IdChangeColorCrit ),
	EV_COMMAND( ID_SETPENMAIN, IdChangePenMain ),
	EV_COMMAND( ID_SETPENCRIT, IdChangePenCrit ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_BAR1, SetSizeArrow ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_BAR2, SetAngjeArrow ),
END_RESPONSE_TABLE;

TSetConnectDialog::TSetConnectDialog( TWindow* parent, int resId, STR_SET_CONNECT* TheStrSetConnect, TConnect* TheConnect )
	: TDialog( parent, resId )
	{ 
	StrSetConnectTmp = TheStrSetConnect; 
	Scroll_Bar1 = new TScrollBar( this, ID_BAR1 );
	Scroll_Bar2 = new TScrollBar( this, ID_BAR2 );
	ControlConnect = new TControlConnect( this, ID_CONNECT, TheConnect );
	}
	
void TSetConnectDialog::SetupWindow()
	{
	TWindow::SetupWindow();
	Scroll_Bar1->SetRange( 1, 100 );
	Scroll_Bar1->SetPosition( StrSetConnectTmp->ConstSizeArrow );
	Scroll_Bar2->SetRange( 1, 100);
	Scroll_Bar2->SetPosition( (int)( StrSetConnectTmp->ConstAngjeArrow * 100 ) );
	}

void TSetConnectDialog::SetSizeArrow( UINT )
	{              
	StrSetConnectTmp->ConstSizeArrow = Scroll_Bar1->GetPosition(); 
	ControlConnect->Invalidate();
	}

void TSetConnectDialog::SetAngjeArrow( UINT )
	{ 
	StrSetConnectTmp->ConstAngjeArrow = ( (float)Scroll_Bar2->GetPosition() ) / 100 ; 
	ControlConnect->Invalidate();
	}

void TSetConnectDialog::IdChangeColorMain()
	{
	if ( ChangeColor( this, StrSetConnectTmp->LogPenMain.lopnColor ) )
		ControlConnect->Invalidate();
	}

void TSetConnectDialog::IdChangeColorCrit()
	{
	if ( ChangeColor( this, StrSetConnectTmp->LogPenCrit.lopnColor ) )
		ControlConnect->Invalidate();
	}

void TSetConnectDialog::IdChangePenMain()
	{}
void TSetConnectDialog::IdChangePenCrit()
	{}
	
void TMyApp::CmSetConnect()
	{
	STR_SET_SCALE StrSetScaleTmp = TEvent::StrSetScale;
	TEvent::StrSetScale.Day = 86400L;
	TEvent::StrSetScale.ScaleMDL = 1;
	TEvent::StrSetScale.VScaleGNT = 1;
	TEvent::StrSetScale.VScaleGST = 1;
	TEvent::StrSetScale.HScaleGNTGST = 3600;
	
	TWork* WorkStart = new TWork();
	WorkStart->Move( TLPoint( 0L, 50L ) );
	TWork* WorkFinish = new TWork();
	WorkFinish->Move( TLPoint( 200L, 150L ) );
	TConnect* Connect = new TConnect( WorkStart, WorkFinish );
	STR_SET_CONNECT StrSetConnectTmp = TConnect::StrSetConnect;
	if ( TSetConnectDialog( MainWindow, IDD_SETCONNECT, &TConnect::StrSetConnect, Connect ).Execute() == IDOK )
		;
	else
		{
		TConnect::StrSetConnect.ConstSizeArrow = StrSetConnectTmp.ConstSizeArrow;
		TConnect::StrSetConnect.ConstAngjeArrow = StrSetConnectTmp.ConstAngjeArrow;
		}
	TEvent::StrSetScale = StrSetScaleTmp;
	delete Connect;
	delete WorkStart;
	delete WorkFinish;
	}


class TControlCalendar : public TControl 
	{
	public:
		TControlCalendar( TWindow* parent, int resId );
	protected:
		char far* GetClassName() { return "ControlCalendar"; }
		void EvPaint();
	DECLARE_RESPONSE_TABLE( TControlCalendar );
	};

DEFINE_RESPONSE_TABLE1( TControlCalendar, TControl )
	EV_WM_PAINT,
END_RESPONSE_TABLE;

TControlCalendar::TControlCalendar( TWindow* parent, int resId )
	: TControl( parent, resId, ::Module )
	{}

void TControlCalendar::EvPaint()
	{
	TPaintDC dc(*this);
	TLRect lRect;
	lRect = GetClientRect();
	lRect.left *= TEvent::StrSetScale.HScaleGNTGST;
	lRect.right *= TEvent::StrSetScale.HScaleGNTGST;
   //DrawCalendar( dc, TLSize( 0, 0 ), &lRect, 0 );
   DrawCalendar( dc, TLSize( 0, 0 ), &lRect, 800000000 );
	lRect.top += 40;
	DrawNet( dc, TLSize( 0, 0 ), &lRect, 0 );
	}

class TSetCalendarDialog : public TDialog
	{
	private:
		STR_SET_CALEDAR* StrSetCalendarTmp;
		TControlCalendar* ControlCalendar;
	public:
		TSetCalendarDialog( TWindow* parent, int resId, STR_SET_CALEDAR* TheStrSetCalendarTmp );
		void SetupWindow();
	protected:
		void IdChangeFont();
		void IdChangeColorMain();
		void IdChangeColorCrit();

	DECLARE_RESPONSE_TABLE( TSetCalendarDialog );
	};

DEFINE_RESPONSE_TABLE1( TSetCalendarDialog, TDialog )
	EV_COMMAND( ID_SETFONT, IdChangeFont ),
	EV_COMMAND( ID_SETCOLORMAIN, IdChangeColorMain ),
	EV_COMMAND( ID_SETCOLORCRIT, IdChangeColorCrit ),
END_RESPONSE_TABLE;

TSetCalendarDialog::TSetCalendarDialog( TWindow* parent, int resId, STR_SET_CALEDAR* TheStrSetCalendar )
	: TDialog( parent, resId )
	{
	StrSetCalendarTmp = TheStrSetCalendar;
	SetTransferBuffer( TheStrSetCalendar);
	ControlCalendar = new TControlCalendar( this, ID_CALENDAR );
	}

void TSetCalendarDialog::SetupWindow()
	{ TWindow::SetupWindow(); }

void TSetCalendarDialog::IdChangeFont()
	{
	if ( ChangeFont( this, StrSetCalendarTmp->LogFont, StrSetCalendarTmp->ColorRefFont ) )
		ControlCalendar->Invalidate();
	}
	
void TSetCalendarDialog::IdChangeColorMain()
	{
	if ( ChangeColor( this, StrSetCalendarTmp->LogPenMain.lopnColor) )
		ControlCalendar->Invalidate();
	}

void TSetCalendarDialog::IdChangeColorCrit()
	{
	if ( ChangeColor( this, StrSetCalendarTmp->LogPenCrit.lopnColor) )
		ControlCalendar->Invalidate();
	}

void TMyApp::CmSetCalendar()
	{
	if ( TSetCalendarDialog( MainWindow, IDD_SETCALENDAR, &TCALWindow::StrSetCalendar ).Execute() == IDOK )
		;
	}


/*
struct STR_SET_CALEDAR
	{
	char CriticalTime[ 11 ];
	};

class TSetBuildDialog : public TDialog
	{
	private:
		STR_SET_BUILD* StrSetBuildTmp;
		TEdit* edit1;
	public:
		TSetBuildDialog( TWindow* parent, int resId, STR_SET_BUILD* TheStrSetBuildTmp );
//		void SetupWindow();
//		void CloseWindow( int ret );
//	protected:
//	DECLARE_RESPONSE_TABLE( TSetBuildDialog );
	};

TSetBuildDialog::TSetBuildDialog( TWindow* parent, int resId, STR_SET_BUILD* TheStrSetBuildTmp )
	: TDialog( parent, resId )
	{
	edit1 = new TEdit( this, ID_NAME, sizeof( StrWorkDialog.Name ) );
	}

void TSetBuildDialog::SetupWindow()
	{
	TDialog::SetupWindow();
	ltoa( (long)( Work->GetDuration() / 3600 ), StrWorkDialog.Duration_hh, 10 );

	}


void TSetBuildDialog::CloseWindow( int ret )
	{
	TDialog::CloseWindow( ret );
	}
*/

void TMyApp::CmSetBuild()
	{
	char CriticalTimeTmp[ 11 ];
	ltoa( TEvent::StrSetScale.CriticalTime, CriticalTimeTmp, 10 );
	if ( ( TInputDialog( MainWindow, " Bild ",	"Critical Time :", CriticalTimeTmp,	sizeof( CriticalTimeTmp ) ) ).Execute() == IDOK )
		TEvent::StrSetScale.CriticalTime = atoi( CriticalTimeTmp );

	}


class TScaleDialog : public TDialog
	{
	private:
		STR_SET_SCALE* StrSetScale;
		TGroupBox* ScaleTime;
		TRadioButton* ScaleYear;
		TRadioButton* ScaleMonth;
		TRadioButton* ScaleDay;
		TRadioButton* ScaleHour;
		TRadioButton* ScaleMinute;
		TScrollBar* Scroll_ScaleMDL;
		TScrollBar* Scroll_VScaleGNT;
		TScrollBar* Scroll_VScaleGST;
		TScrollBar* Scroll_HScaleGNTGST;
	public:
		TScaleDialog( TWindow* parent, int resId, STR_SET_SCALE* TheStrSetScale );
		void SetupWindow();
	private:
		void SetScaleMDL( UINT code );
		void SetVScaleGNT( UINT code );
		void SetVScaleGST( UINT code );
		void SetHScaleGNTGST( UINT code );
		void SetTime( UINT code );
	DECLARE_RESPONSE_TABLE( TScaleDialog );
	};

DEFINE_RESPONSE_TABLE1( TScaleDialog, TDialog )
	EV_CHILD_NOTIFY_ALL_CODES( ID_SCALEMDL, SetScaleMDL ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_VSCALEGNT, SetVScaleGNT ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_VSCALEGST, SetVScaleGST ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_HSCALEGNTGST, SetHScaleGNTGST ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_TIME, SetTime ),
END_RESPONSE_TABLE;

TScaleDialog::TScaleDialog( TWindow* parent, int resId, STR_SET_SCALE* TheStrSetScale )
	: TDialog( parent, resId )
	{
	StrSetScale = TheStrSetScale;
	ScaleTime = new TGroupBox( this, ID_TIME );
	ScaleYear = new TRadioButton( this, ID_YEAR, ScaleTime );
	ScaleMonth = new TRadioButton( this, ID_MONTH, ScaleTime );
	ScaleDay = new TRadioButton( this, ID_DAY, ScaleTime );
	ScaleHour = new TRadioButton( this, ID_HOUR, ScaleTime );
	ScaleMinute = new TRadioButton( this, ID_MINUTE, ScaleTime );
	Scroll_ScaleMDL = new TScrollBar( this, ID_SCALEMDL );
	Scroll_VScaleGNT = new TScrollBar( this, ID_VSCALEGNT );
	Scroll_VScaleGST = new TScrollBar( this, ID_VSCALEGST );
	Scroll_HScaleGNTGST = new TScrollBar( this, ID_HSCALEGNTGST );

	}

void TScaleDialog::SetupWindow()
	{
	TWindow::SetupWindow();
	if ( StrSetScale->ScaleYear )
		ScaleYear->Check();
	if ( StrSetScale->ScaleMonth )
		ScaleMonth->Check();
	if ( StrSetScale->ScaleDay )
		ScaleDay->Check();
	if ( StrSetScale->ScaleHour )
		ScaleHour->Check();
	if ( StrSetScale->ScaleMinute )
		ScaleMinute->Check();
	Scroll_ScaleMDL->SetRange( 5, 100 );
	Scroll_ScaleMDL->SetPosition( (int)( StrSetScale->ScaleMDL * 10 ) );
	Scroll_VScaleGNT->SetRange( 5, 100);
	Scroll_VScaleGNT->SetPosition( (int)( StrSetScale->VScaleGNT * 10 ) );
	Scroll_VScaleGST->SetRange( 5, 100);
	Scroll_VScaleGST->SetPosition( (int)( StrSetScale->VScaleGST * 10 ) );
	Scroll_HScaleGNTGST->SetRange( 1, 32767 );
	Scroll_HScaleGNTGST->SetPosition( (int)( StrSetScale->HScaleGNTGST ) );
	}

void TScaleDialog::SetScaleMDL( UINT )
	{ StrSetScale->ScaleMDL = ( (float)Scroll_ScaleMDL->GetPosition() ) / 10; }

void TScaleDialog::SetVScaleGNT( UINT )
	{ StrSetScale->VScaleGNT = ( (float)Scroll_VScaleGNT->GetPosition() ) / 10; }

void TScaleDialog::SetVScaleGST( UINT )
	{ StrSetScale->VScaleGST = ( (float)Scroll_VScaleGST->GetPosition() ) / 10; }

void TScaleDialog::SetHScaleGNTGST( UINT )
	{ StrSetScale->HScaleGNTGST = ( (float)Scroll_HScaleGNTGST->GetPosition() ); }


void TScaleDialog::SetTime( UINT )
	{
	STR_SET_SCALE StrSetScaleTmp = *StrSetScale;
	StrSetScale->ScaleYear = ( ScaleYear->GetCheck() == BF_CHECKED ) ? TRUE : FALSE;
	StrSetScale->ScaleMonth = ( ScaleMonth->GetCheck() == BF_CHECKED ) ? TRUE : FALSE;
	StrSetScale->ScaleDay = ( ScaleDay->GetCheck() == BF_CHECKED ) ? TRUE : FALSE;
	StrSetScale->ScaleHour = ( ScaleHour->GetCheck() == BF_CHECKED ) ? TRUE : FALSE;
	StrSetScale->ScaleMinute = ( ScaleMinute->GetCheck() == BF_CHECKED ) ? TRUE : FALSE;
	if ( StrSetScale->ScaleYear && !StrSetScaleTmp.ScaleYear )
		StrSetScale->HScaleGNTGST = 32767;
	if ( StrSetScale->ScaleMonth && !StrSetScaleTmp.ScaleMonth )
		StrSetScale->HScaleGNTGST = 32767;
	if ( StrSetScale->ScaleDay && !StrSetScaleTmp.ScaleDay )
		StrSetScale->HScaleGNTGST = 3600;
	if ( StrSetScale->ScaleHour && !StrSetScaleTmp.ScaleHour )
		StrSetScale->HScaleGNTGST = 180;
	if ( StrSetScale->ScaleMinute && !StrSetScaleTmp.ScaleMinute )
		StrSetScale->HScaleGNTGST = 3;
	Scroll_HScaleGNTGST->SetPosition( (int)( StrSetScale->HScaleGNTGST ) );
	}


void TMyApp::CmSetScale()
	{
	STR_SET_SCALE StrSetScaleTmp = TEvent::StrSetScale;
	if ( TScaleDialog( MainWindow, IDD_SCALE, &StrSetScaleTmp ).Execute() == IDOK )
		{
		TEvent::StrSetScale = StrSetScaleTmp;
		RedrawAll();
		}
	}

static void Redraw( TWindow* win, void* )
	{
	win->ShowWindow( SW_HIDE );
	win->ShowWindow( SW_SHOW );
	}

void TMyApp::RedrawAll()
	{ Client->ForEach( Redraw ); }

