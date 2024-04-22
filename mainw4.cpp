#include <owl\owlpch.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\statusba.h>
#include <owl\toolbox.h>
#include <owl\opensave.h>
#include <owl\validate.h>
#include <owl\printer.h>
#include <owl\edit.h>
#include <owl\menu.h>
#include <ddeml.h>
#include <stdlib.h>
#include <time.h>
#include <dir.h>
/////////////////////////////////BCB3 !!!//////////////////////////////
/*
#include "pch.h"      // #####################?????
#pragma hdrstop
//#include <owl/framewin.h>
#include <owl/applicat.h>
#include <classlib/pointer.h>

#include "adopt.h"              // for adopting VCL's application window
#include "WorkDialogUnit.h"     // C++Builder form unit header
//#include "main.h"

*/
//////////////////////////////////////////////////////////////////////
#include "mainw4.h"
#include "layout.h"
#include "new_pack.h"
#include "mainw4.rh"

/////////////////////////////////BCB3 !!!//////////////////////////////
// ?????  OR  BEFORE  MAINW4?????
//#include "pch.h"      // #####################?????
#pragma hdrstop
#include <owl/applicat.h>
#include <classlib/pointer.h>

//#include "adopt.h"              // for adopting VCL's application window
// ?????#include "WorkDialogUnit.h"     // C++Builder form unit header


//////////////////////////////////////////////////////////////////////



//
// Generated help file.
//
//const char HelpFileName[] = "spu_ole.hlp";
const char HelpFileName[] = "vp.hlp";

OWL::TPrinter* TMyApp::Printer = NULL;  //##################???
TMDIClient* TMyApp::Client = NULL;

 DEFINE_RESPONSE_TABLE1( TMyApp, OWL::TApplication ) //#############
	EV_COMMAND( CM_FILENEW, CmFileNew ),
	EV_COMMAND( CM_FILEOPEN, CmFileOpen ),
	EV_COMMAND( CM_ABOUT, CmAbout ),
	EV_COMMAND( CM_PRINTERSETUP, CmPrinterSetup ),
	EV_COMMAND( CM_SETWORK, CmSetWork ),
	EV_COMMAND( CM_SETPACK, CmSetPack ),
	EV_COMMAND( CM_SETCONNECT, CmSetConnect ),
	EV_COMMAND( CM_SETCALENDAR, CmSetCalendar ),
	EV_COMMAND( CM_SETBUILD, CmSetBuild ),
	EV_COMMAND( CM_SETSCALE, CmSetScale ),
	EV_COMMAND( CM_WRITESET, CmWriteSet ),

	EV_COMMAND(CM_HELPABOUT, CmHelpAbout),
	EV_COMMAND(CM_HELPCONTENTS, CmHelpContents),
	EV_COMMAND(CM_HELPUSING, CmHelpUsing),

END_RESPONSE_TABLE;

TMyApp::TMyApp() :OWL::TApplication()   //#######################BCB3
//  TMyApp::TMyApp() : TApplication()
	{
	 HelpState = false;
	 ContextHelp = false;
	 HelpCursor = 0;

	 }



TMyApp::~TMyApp()
	{
   // CG !!!
//	free( TWork::StrSetField.FieldName );
//	free( TWork::StrSetField.FieldNumber );
//	free( TWork::StrSetField.FieldStartTime );
//	free( TWork::StrSetField.FieldFinishTime );
//	free( TWork::StrSetField.FieldIndexResource );
//	free( TWork::StrSetField.FieldResource );
//	free( TWork::StrSetField.FieldResourceMin );
//	free( TWork::StrSetField.FieldResourceOpt );
//	free( TWork::StrSetField.FieldResourceMax );
//	free( TWork::StrSetField.FieldVolume );
//	free( TWork::StrSetField.FieldDuration );
//	free( TWork::StrSetField.FieldShiftLimStart );
//	free( TWork::StrSetField.FieldShiftLimFinish );
//	free( TWork::StrSetField.FieldShiftOptStart );
//	free( TWork::StrSetField.FieldShiftOptFinish );
//	free( TWork::StrSetField.FieldShiftStart );
//	free( TWork::StrSetField.FieldShiftFinish );
//	free( TWork::StrSetField.FieldStartKnot );
//	free( TWork::StrSetField.FieldFinishKnot );
//	free( TWork::StrSetField.FieldNumberPack );
//	free( TWork::StrSetField.FieldNamePack );
//	free( TWork::StrSetField.FieldNumberParentPack );
	delete Printer;
	}


void TMyApp::InitApplication()
	{
	EnableBWCC();
	EnableCtl3d(TRUE);
	}

void TMyApp::InitMainWindow()
	{
  	EnableCtl3d(TRUE);

	putenv("TZ=PTS0");
	tzset();

	// Construct the decorated Frame window
	TDecoratedMDIFrame* Frame = new TDecoratedMDIFrame( "ASSPU ++", "COMMANDS", *( Client = new TMDIClient ), TRUE );
	// Construct a status bar
	OWL::TStatusBar* sb = new OWL::TStatusBar(Frame, TGadget::Recessed,
	OWL::TStatusBar::ScrollLock |
	OWL::TStatusBar::CapsLock | OWL::TStatusBar::NumLock | OWL::TStatusBar::Overtype);

	Frame->Insert(*sb, TDecoratedFrame::Bottom);

	// Construct a control bar
	OWL::TControlBar* cb = new OWL::TControlBar( Frame );
	OWL::TControlBar* cb1 = new OWL::TControlBar( Frame ); // New
//	TToolBox*  toolbox = new TToolBox( Frame );  // New

	cb->Insert( *new TButtonGadget( CM_FILENEW, CM_FILENEW, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_FILEOPEN, CM_FILEOPEN, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_FILESAVE, CM_FILESAVE, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_FILESAVEAS, CM_FILESAVEAS, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget );

	cb1->Insert( *new TButtonGadget( CM_PRINTERSETUP, CM_PRINTERSETUP, TButtonGadget::Command ) );
	cb1->Insert( *new TButtonGadget( CM_PRINT, CM_PRINT, TButtonGadget::Command ) );
	cb1->Insert( *new TButtonGadget( CM_SETSCALE, CM_SETSCALE, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget( CM_CLOSEPACK, CM_CLOSEPACK, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget( CM_ACTIVATEMDL, CM_ACTIVATEMDL, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_ACTIVATEGNT, CM_ACTIVATEGNT, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_ACTIVATEGST, CM_ACTIVATEGST, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget( CM_EDITPACK, CM_EDITPACK, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget( CM_SELECTWORK, CM_SELECTWORK, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_SELECTPACK, CM_SELECTPACK, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget( CM_FIND, CM_FIND, TButtonGadget::Command ) );
	cb->Insert( *new TButtonGadget( CM_INDEXRESOURCE, CM_INDEXRESOURCE, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb1->Insert( *new TButtonGadget( CM_CONVERT, CM_CONVERT, TButtonGadget::Command ) );
	cb1->Insert( *new TSeparatorGadget );
	cb1->Insert( *new TButtonGadget( CM_OPTIMIZATION, CM_OPTIMIZATION, TButtonGadget::Command ) );
	cb1->Insert( *new TSeparatorGadget );
	cb1->Insert( *new TButtonGadget( CM_UPDATEMODEL, CM_UPDATEMODEL, TButtonGadget::Command ) );
	cb1->Insert( *new TSeparatorGadget(6));
	cb1->Insert( *new TButtonGadget( CM_SPUFOX, CM_SPUFOX, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb1->Insert( *new TButtonGadget( _OWL_CM_EXIT, _OWL_CM_EXIT, TButtonGadget::Command ) );
	cb->Insert( *new TSeparatorGadget(6));
	cb->Insert( *new TButtonGadget(CM_HELPCONTENTS, CM_HELPCONTENTS));
	cb->Insert( *new TSeparatorGadget );
	cb->Insert( *new TButtonGadget( CM_ABOUT, CM_ABOUT, TButtonGadget::Command ) );

	cb->SetHintMode( TGadgetWindow::EnterHints );
	cb1->SetHintMode( TGadgetWindow::EnterHints );
	// Insert the status bar and control bar into the Frame
	Frame->Insert( *sb, TDecoratedFrame::Bottom );
	Frame->Insert( *cb, TDecoratedFrame::Top );
	Frame->Insert( *cb1, TDecoratedFrame::Top );

	//  Construct a toolbox using same icons as tollbar
	//toolbox = new TToolBox(Frame, 1); // one column toolbox
	//toolbox->Insert( *new TButtonGadget( CM_ABOUT, CM_ABOUT, TButtonGadget::Command ));
	//toolbox->Insert( *new TButtonGadget( _OWL_CM_EXIT, _OWL_CM_EXIT ));
	//Frame->Insert( *toolbox, TDecoratedFrame::Right );

	// Set the main window and its menu
	SetMainWindow( Frame );

	//???Frame->MessageBox("You have pressed the Find button","Message Dispathed",MB_OK);

	Printer = new OWL::TPrinter;

	ReadSet();
	}


//
// Respond to "New" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void TMyApp::CmFileNew()
	{
	TPack* Pack = new TPack();
	unsigned long StartTime = ( time( NULL ) / 86400UL ) * 86400UL;
	Pack->SetStartTime( StartTime );
	Pack->SetFinishTime( StartTime );
	if ( TPackDialog( MainWindow, IDD_NEWPACK, Pack ).Execute() == IDOK )
		{
		TDCutOffTimes* CutOffTimes = Pack->GetCutOffTimes();
		StartTime = ( Pack->GetStartTime() / 604800UL ) * 604800UL + 172800UL;
		while( StartTime < Pack->GetFinishTime() )
			{
			CutOffTimes->AddAtTail( new TCutOffTime( StartTime, StartTime + 172800UL ) );
			StartTime += 604800UL;
			}

		TMDIChild* child = new TMDIChild( *Client, "", new TMyLayout( Pack ) );
		TMDIChild* curChild = Client->GetActiveMDIChild();
		if ( curChild && ( curChild->GetWindowLong( GWL_STYLE ) & WS_MAXIMIZE ) )
			child->Attr.Style |= WS_MAXIMIZE;
		child->Create();
		}
	else
		delete Pack;
	}

//
// Respond to "Open" command by constructing, creating, and setting up a
// new TFileWindow MDI child
//
void TMyApp::CmFileOpen()
	{
	TOpenSaveDialog::TData FileData;
	FileData.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;

    ///////////////// BCP 31.03.98   was string !!!!!
    FileData.SetFilter( TResString( *this, IDS_FILEFILTER ).c_str() );

	*FileData.FileName = 0;
	if ( TFileOpenDialog( MainWindow, FileData ).Execute() == IDOK )
		{
		ifpstream is( FileData.FileName );
		if ( !is )
			{
         /////////////////////// BCP  31.03.98
			TResString msgTmp1( *this, IDS_UNABLEREAD );
			TResString msgTmp2( *this, IDS_ERROR );

			char* msg = new char[ MAXPATH + msgTmp1.length() ];
			wsprintf( msg, msgTmp1.c_str(), FileData.FileName );
			::MessageBox( 0, msg, msgTmp2.c_str(), MB_OK | MB_ICONQUESTION );
			delete msg;
			}
		else
			{
			TPack* Pack = new TPack();
			Pack->read( is );
			Pack->CorrectTime();
			TMDIChild* child = new TMDIChild( *Client, "", new TMyLayout( Pack, FileData.FileName ) );
			TMDIChild* curChild = Client->GetActiveMDIChild();
			if ( curChild && ( curChild->GetWindowLong( GWL_STYLE ) & WS_MAXIMIZE ) )
				child->Attr.Style |= WS_MAXIMIZE;
			child->Create();
			}
		}
	}

void TMyApp::CmPrinterSetup()
	{
	if ( Printer )
		Printer->Setup( Client );
	}

void TMyApp::CmAbout()
	{
	TDialog( MainWindow, IDD_ABOUT ).Execute();
	}



bool TMyApp::CanClose ()
{
	 bool result = OWL::TApplication::CanClose();

	 //
	 // Close the help engine if we used it.
	 //
	 if (result && HelpState)
		  GetMainWindow()->WinHelp(HelpFileName, HELP_QUIT, 0L);

	 return result;
}


//////////////////////////////////////////////////////////
// spu_oleApp
// =====
// Menu Help Contents command
void TMyApp::CmHelpContents ()
{
	 //
	 // Show the help table of contents.
	 //
	 HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_CONTENTS, 0L);
}


//////////////////////////////////////////////////////////
// spu_oleApp
// =====
// Menu Help Using Help command
void TMyApp::CmHelpUsing ()
{
	 //
	 // Display the contents of the Windows help file.
	 //
	 HelpState = GetMainWindow()->WinHelp(HelpFileName, HELP_HELPONHELP, 0L);
}


//////////////////////////////////////////////////////////
// spu_oleApp
// ===========
// Menu Help About spu_ole.exe command
void TMyApp::CmHelpAbout ()
{
	 //
	 // Show the modal dialog.
	 //
			//!!!!!!!!!!!!!!!!!!!!!!!!!
	 // spu_oleAboutDlg(&TWindow(::GetFocus(), this)).Execute();
	 /////////////////////////////////////
}


bool TMyApp::ProcessAppMsg (MSG& msg)
{
	 if (msg.message == WM_COMMAND) {
		  if (ContextHelp || (::GetKeyState(VK_F1) < 0)) {
				ContextHelp = false;
				GetMainWindow()->WinHelp(HelpFileName, HELP_CONTEXT, msg.wParam);
				return true;
		  }
	 } else
		  switch (msg.message) {
		  case WM_KEYDOWN:
				if (msg.wParam == VK_F1) {
					 // If the Shift/F1 then set the help cursor and turn on the modal help state.
					 if (::GetKeyState(VK_SHIFT) < 0) {
						  ContextHelp = true;
						  HelpCursor = ::LoadCursor(GetMainWindow()->GetModule()->GetInstance(), MAKEINTRESOURCE(IDC_HELPCURSOR));
						  ::SetCursor(HelpCursor);
						  return true;        // Gobble up the message.
					 } else {
						  // If F1 w/o the Shift key then bring up help's main index.
						  GetMainWindow()->WinHelp(HelpFileName, HELP_INDEX, 0L);
						  return true;        // Gobble up the message.
					 }
				} else {
					 if (ContextHelp && (msg.wParam == VK_ESCAPE)) {
						  if (HelpCursor)
								::DestroyCursor(HelpCursor);
						  ContextHelp = false;
						  HelpCursor = 0;
						  GetMainWindow()->SetCursor(0, IDC_ARROW);
						  return true;    // Gobble up the message.
					 }
				}
				break;

		  case WM_MOUSEMOVE:
		  case WM_NCMOUSEMOVE:
				if (ContextHelp) {
					 ::SetCursor(HelpCursor);
					 return true;        // Gobble up the message.
				}
				break;

		  case WM_INITMENU:
				if (ContextHelp) {
					 ::SetCursor(HelpCursor);
					 return true;        // Gobble up the message.
				}
				break;

		  case WM_ENTERIDLE:
				if (msg.wParam == MSGF_MENU)
					 if (GetKeyState(VK_F1) < 0) {
						  ContextHelp = true;
						  GetMainWindow()->PostMessage(WM_KEYDOWN, VK_RETURN, 0L);
						  return true;       // Gobble up the message.
					 }
				break;

		  default:
				;
		  };  // End of switch

	 // Continue normal processing.

	 return OWL::TApplication::ProcessAppMsg(msg);
}


int OwlMain( int /*argc*/, char* /*argv*/ [] )
	{
	TMyApp  App;
	int     result;

	result = App.Run();
   return result;
	//return TMyApp().Run();
	}
