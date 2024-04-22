#define  STRICT
#define  _USE_VCL_CM_EXIT  //???
/*
#include "pch.h"      // #####################
#pragma hdrstop
#include <owl/applicat.h>
#include <classlib/pointer.h>
*/

/////////////////////////////////////
#include <owl\owlpch.h>
#include <owl\opensave.h>
#include <owl\printer.h>
#include <owl\opensave.h>
#include <owl\validate.h>
#include <owl\listbox.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "layout.h"
#include "mdl_wind.h"
#include "cal_wind.h"
#include "gnt_wind.h"
#include "gst_wind.h"
#include "mainw4.h"
#include "mainw4.rh"
//#include "new_work.h"   // BCB333#####################

///////////////////////////////// BCB333#####################
//#include "WorkDialogUnit.h"

//#include "pch.h"      // #####################?????
#pragma hdrstop
#include <owl/applicat.h>
#include <classlib/pointer.h>

//#include "adopt.h"              // for adopting VCL's application window
//#define  STRICT
#include "WorkDialogUnit.h"     // C++Builder form unit header

///////////////////////////////////////////////////////////////



LRESULT TMyLayout::EvReLayout( WPARAM wParam, LPARAM )
	{
	for ( int i = 0; i < 3; i++ )
		 if ( AllWindow[i].BASWindow->HWindow == (HWND)wParam )
			 if ( AllWindow[i].MaxScreen )
				 AllWindow[i].MaxScreen = FALSE;
			 else
				 AllWindow[i].MaxScreen = TRUE;
	ReLayout();
	return TRUE;
	}

LRESULT TMyLayout::EvSetDirty( WPARAM , LPARAM )
	{ return IsDirty = TRUE; }

LRESULT TMyLayout::EvGetDrawNet( WPARAM, LPARAM )
	{ return DrawNetButton;	}

LRESULT TMyLayout::EvGetIndexResource( WPARAM, LPARAM )
	{ return IndexResource;	}

LRESULT TMyLayout::EvInvalidateMDL( WPARAM, LPARAM )
	{
	if	( AllWindow[0].BASScreen && !AllWindow[1].MaxScreen && !AllWindow[2].MaxScreen )
		{
		AllWindow[0].BASWindow->Invalidate();
		return TRUE;
		}
	return FALSE;
	}

LRESULT TMyLayout::EvInvalidateGNT( WPARAM, LPARAM )
	{
	if	( AllWindow[1].BASScreen && !AllWindow[0].MaxScreen && !AllWindow[2].MaxScreen )
		{
		AllWindow[1].BASWindow->Invalidate();
		return TRUE;
		}
	return FALSE;
	}

LRESULT TMyLayout::EvInvalidateGST( WPARAM, LPARAM )
	{
	if	( AllWindow[2].BASScreen && !AllWindow[0].MaxScreen && !AllWindow[1].MaxScreen )
		{
		AllWindow[2].BASWindow->Invalidate();
		return TRUE;
		}
	return FALSE;
	}

LRESULT TMyLayout::EvScrollMDL( WPARAM wParam, LPARAM lParam )
	{
	AllWindow[0].BASWindow->ScrollTo( (long)wParam, (long)lParam );
	return TRUE;
	}

LRESULT TMyLayout::EvScrollGNT( WPARAM wParam, LPARAM lParam )
	{
	AllWindow[1].BASWindow->ScrollTo( (long)wParam, (long)lParam );
	AllWindow[3].BASWindow->ScrollTo( (long)wParam, (long)lParam );
	return TRUE;
	}

LRESULT TMyLayout::EvScrollGST( WPARAM wParam, LPARAM lParam )
	{
	AllWindow[2].BASWindow->ScrollTo( (long)wParam, (long)lParam );
	AllWindow[3].BASWindow->ScrollTo( (long)wParam, (long)lParam );
	return TRUE;
	}

LRESULT TMyLayout::EvNewElement( WPARAM wParam, LPARAM lParam )
	{
	bool GoodIdea = TRUE;
	if ( ( StrPackEdit->PackStart || StrPackEdit->WorkStart ) && ( StrPackEdit->PackFinish || StrPackEdit->WorkFinish ) )
		{
		if ( StrPackEdit->PackStart )
			StrPackEdit->WorkStart = StrPackEdit->PackStart->SelectWork();
		if ( StrPackEdit->PackFinish && StrPackEdit->WorkStart )
			StrPackEdit->WorkFinish = StrPackEdit->PackFinish->SelectWork();

		if ( StrPackEdit->WorkStart && StrPackEdit->WorkFinish && StrPackEdit->WorkFinish->GetPercent() == 0 )
			{
			TConnects* ConnectsCheck = new TConnects();
			StrPackEdit->WorkFinish->GetEntrancesConnect( ConnectsCheck );
			while ( !ConnectsCheck->IsEmpty() )
				{
				if ( ConnectsCheck->PeekHead()->GetWorkStart() == StrPackEdit->WorkStart )
					GoodIdea = FALSE;
				ConnectsCheck->DetachAtHead();
				}
			if ( GoodIdea  )
				{
				PackMain->ResetChec();
				StrPackEdit->WorkFinish->SetChec( ConnectsCheck );
				while ( !ConnectsCheck->IsEmpty() )
					{
					TConnect* connect = ConnectsCheck->PeekHead();
					ConnectsCheck->DetachAtHead();
					if ( GoodIdea  )
						{
						TWork* WorkChec = connect->GetWorkFinish();
						if ( WorkChec == StrPackEdit->WorkStart )
							{
                            ///////////////////////// BCP !!!!!!!!!!
							TResString msgTmp1( *GetModule(), IDS_CICLE );
							TResString msgTmp2( *GetModule(), IDS_ERROR );
							MessageBox( msgTmp1.c_str(), msgTmp2.c_str(), MB_ICONEXCLAMATION | MB_OK );
							GoodIdea = FALSE;
							}
						else
							WorkChec->SetChec( ConnectsCheck );
						}
					}
				}	
			delete ConnectsCheck;
				
			if ( GoodIdea  )
				{
				TConnect* Connect = new TConnect( StrPackEdit->WorkStart, StrPackEdit->WorkFinish, StrPackEdit->PackStart, StrPackEdit->PackFinish );
				StrPackEdit->PackView->Add( Connect );

				TConnects* ConnectsCheck = new TConnects();
				StrPackEdit->WorkFinish->CorrectTime( StrPackEdit->WorkStart->GetFinishTime(), ConnectsCheck );
				while ( !ConnectsCheck->IsEmpty() )
					{
					TConnect* connect = ConnectsCheck->PeekHead();
					ConnectsCheck->DetachAtHead();
					connect->GetWorkFinish()->CorrectTime( connect->GetWorkStart()->GetFinishTime(), ConnectsCheck );
					}
				delete ConnectsCheck;
				PackMain->CorrectTime();
				Invalidate();	
				}
			}
		StrPackEdit->PackStart = NULL;
		StrPackEdit->WorkStart = NULL;
		StrPackEdit->PackFinish = NULL; 
		StrPackEdit->WorkFinish = NULL;
		}
	else
		{
		if ( Type == 0 )   // My  add ---->  New Work !!!
			{
			TWork* Work = new TWork();
			Work->SetNumber( StrPackEdit->PackView->GetFreeNumber() );
			Work->SetStartTime( StrPackEdit->PackView->GetStartTime() );
			Work->SetFinishTime( StrPackEdit->PackView->GetStartTime() );
			Work->SetWeekEnd();

 ////////////////////////////////////// BCB3 #################

  // Create the form object
  //
  TPointer<TWorkDialogForm> dlg = new TWorkDialogForm(::Application);
//   auto_ptr<TWorkDialogForm> dlg(new TWorkDialogForm(this));

 dlg->TransferWork(Work);
//   dlg->GetVCLForm()->TransferWork(Work);

  // Invoke the form, running it as a modal dialog
  //
 TModalResult result = dlg->ShowModal();
//   TModalResult result = dlg->GetVCLForm()->ShowModal();


  // If user closed with the OK button, change Work setting
  //
  if (result == mrOk) {

   Work =  dlg->ReturnWork(Work);
 //  Work =  dlg->GetVCLForm()->ReturnWork(Work);

 ////////////////////////////////////////////////////////////////////

   // Temporary !!!!
  	//		if ( TWorkDialog( NULL, IDD_NEWWORK, Work ).Execute() == IDOK )
	//			{
				Work->Move( TLPoint( (long)wParam, (long)lParam ) );
				Work->SetParent( StrPackEdit->PackView );
				Work->BuildWork();
				StrPackEdit->PackView->Add( Work );
				PackMain->CorrectTime();
				Invalidate();
				}
			else
				{
				delete Work;
				GoodIdea = FALSE;
				}

/////////////////////////////////////////////////////////////////////
			}
		if ( Type == 1 )
			{
			TOpenSaveDialog::TData FileData;
			FileData.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
            /////////////////////////// BCP !!!!!!!!!!!!!!!!!!!!!
			FileData.SetFilter( TResString( *GetModule(), IDS_FILEFILTER ).c_str() );
			*FileData.FileName = 0;
			if ( TFileOpenDialog( this, FileData ).Execute() == IDOK )
				{
				ifpstream is( FileData.FileName );
				if ( !is )
					{
                    //////////////// BCP !!!!!!!!!!!!!!!!!!!!!!!
					TResString msgTmp1( *GetModule(), IDS_UNABLEREAD );
					TResString msgTmp2( *GetModule(), IDS_ERROR );
					char* msg = new char[ MAXPATH + msgTmp1.length() ];
					wsprintf( msg, msgTmp1.c_str(), FileData.FileName );
					MessageBox( msg, msgTmp2.c_str(), MB_OK | MB_ICONQUESTION );
					delete msg;
					GoodIdea = FALSE;
					}
				else
					{
					TPack* Pack = new TPack();
					Pack->read( is );
					Pack->ResetCutOffTimes();
					Pack->SetNumber( StrPackEdit->PackView->GetFreeNumber() );
					Pack->Move( TLPoint( (long)wParam, (long)lParam ) );
					Pack->SetParent( StrPackEdit->PackView );
					if ( Pack->GetStartTime() < StrPackEdit->PackView->GetStartTime() )
						{
						Pack->SetStartTime( StrPackEdit->PackView->GetStartTime() );
						Pack->ArrangeStartTime();
						}
					Pack->CorrectTime();
					if ( AllWindow[2].BASScreen )
						Pack->BuildBitTimes();
					StrPackEdit->PackView->Add( Pack );
					PackMain->CorrectTime();
					Invalidate();
					}
				}
			else
				GoodIdea = FALSE;
			}
		}
	return GoodIdea;
	}

LRESULT TMyLayout::EvDeleteElement( WPARAM, LPARAM )
	{
	if ( StrPackEdit->WorkStart && StrPackEdit->WorkStart->GetPercent() == 0 )
		{
        //////////////////// BCP !!!!!!!!!!!!!!!!!!!
		TResString msgTmp1( *GetModule(), IDS_DELETEWORK );
		TResString msgTmp2( *GetModule(), IDS_DELETE );
		char* msg = new char[ 26 + msgTmp1.length() ];
		wsprintf( msg, msgTmp1.c_str(), StrPackEdit->WorkStart->GetNumber() );
		if ( MessageBox( msg, msgTmp2.c_str(), MB_ICONEXCLAMATION | MB_YESNOCANCEL) == IDYES )
			{
			TConnects* ConnectsCheck = new TConnects();
			StrPackEdit->WorkStart->GetEntrancesConnect( ConnectsCheck );
			while ( !ConnectsCheck->IsEmpty() )
				{
				if ( ConnectsCheck->PeekHead()->GetPackFinish() == StrPackEdit->PackView )
					{
					StrPackEdit->PackView->GetParent()->Detach( ConnectsCheck->PeekHead() );
					ConnectsCheck->DetachAtHead( TRUE );
					}
				else
					ConnectsCheck->DetachAtHead();
				}
			StrPackEdit->WorkStart->GetExitsConnect( ConnectsCheck );
			while ( !ConnectsCheck->IsEmpty() )
				{
				if ( ConnectsCheck->PeekHead()->GetPackStart() == StrPackEdit->PackView )
					{
					StrPackEdit->PackView->GetParent()->Detach( ConnectsCheck->PeekHead() );
					ConnectsCheck->DetachAtHead( TRUE );
					}
				else
					ConnectsCheck->DetachAtHead();
				}
			delete ConnectsCheck;
			StrPackEdit->PackView->Detach( StrPackEdit->WorkStart, TRUE );
			PackMain->CorrectTime();
			Invalidate();
			}
		delete[] msg;   // CG !!!
		StrPackEdit->WorkStart = NULL;
		}

	if ( StrPackEdit->PackStart && StrPackEdit->PackStart->GetCarentTime() == StrPackEdit->PackStart->GetStartTime() )
		{
        ////////////////////////////// BCP !!!!!!!!!!!!!!!!!!!!!
		TResString msgTmp1( *GetModule(), IDS_DELETEPACK );
		TResString msgTmp2( *GetModule(), IDS_DELETE );
		char* msg = new char[ 26 + msgTmp1.length() ];
		wsprintf( msg, msgTmp1.c_str(), StrPackEdit->PackStart->GetNumber() );
		if ( MessageBox( msg, msgTmp2.c_str(), MB_ICONEXCLAMATION | MB_YESNOCANCEL) == IDYES )
			{
			StrPackEdit->PackView->Detach( StrPackEdit->PackStart, TRUE );
			PackMain->CorrectTime();
			Invalidate();
			}
		delete msg;
		StrPackEdit->PackStart= NULL;
		}

	if ( StrPackEdit->Connect && StrPackEdit->Connect->GetWorkFinish()->GetPercent() == 0 )
		{
		char* msg;
		if ( !StrPackEdit->Connect->GetPackStart() )
			if ( !StrPackEdit->Connect->GetPackFinish() )
				{
                /////////////////////// BCP !!!!!!!!!!!!!
				TResString msgTmp1( *GetModule(), IDS_DELETECONNECTWORKWORK );
				msg = new char[ 25 + 25 + msgTmp1.length() ];
				wsprintf( msg, msgTmp1.c_str(), StrPackEdit->Connect->GetWorkStart()->GetNumber(), StrPackEdit->Connect->GetWorkFinish()->GetNumber() );
				}
			else
				{
                //////////////////////////// BCP
				TResString msgTmp1( *GetModule(), IDS_DELETECONNECTWORKPACKWORK );
				msg = new char[ 25 + 25 + 25 + msgTmp1.length() ];
				wsprintf( msg, msgTmp1.c_str(), StrPackEdit->Connect->GetWorkStart()->GetNumber(), StrPackEdit->Connect->GetPackFinish()->GetNumber(), StrPackEdit->Connect->GetWorkFinish()->GetNumber() );
				}
		else
			if ( !StrPackEdit->Connect->GetPackFinish() )
				{
                /////////////////////////// BCP
				TResString msgTmp1( *GetModule(), IDS_DELETECONNECTPACKWORKWORK );
				msg = new char[ 25 + 25 + 25 + msgTmp1.length() ];
				wsprintf( msg, msgTmp1.c_str(), StrPackEdit->Connect->GetPackStart()->GetNumber(), StrPackEdit->Connect->GetWorkStart()->GetNumber(), StrPackEdit->Connect->GetWorkFinish()->GetNumber() );
				}
			else
				{
                /////////////////////////////// BCP
				TResString msgTmp1( *GetModule(), IDS_DELETECONNECTPACKWORKPACKWORK );
				msg = new char[ 25 + 25 + 25 + 25 + msgTmp1.length() ];
				wsprintf( msg, msgTmp1.c_str(), StrPackEdit->Connect->GetPackStart()->GetNumber(), StrPackEdit->Connect->GetWorkStart()->GetNumber(), StrPackEdit->Connect->GetPackFinish()->GetNumber(), StrPackEdit->Connect->GetWorkFinish()->GetNumber() );
				}
        ////////////////////////////////////// BCP
		TResString msgTmp2( *GetModule(), IDS_DELETE );
		if ( MessageBox( msg, msgTmp2.c_str(), MB_ICONEXCLAMATION | MB_YESNOCANCEL) == IDYES )
			{
			StrPackEdit->PackView->Detach( StrPackEdit->Connect, TRUE );
			Invalidate();
			}
		delete msg;
		StrPackEdit->Connect = NULL;
		}
	return TRUE;
	}

LRESULT TMyLayout::EvEntranceInWork( WPARAM, LPARAM )
	{
	TWork* Work = new TWork();
	*Work = *StrPackEdit->WorkStart;

///////////////###################################BCB3 !!!
//	if ( TWorkDialog( NULL, IDD_NEWWORK, Work ).Execute() == IDOK )
//		{

 ////////////////////////////////////// BCB3 #################

  // Create the form object
  //
  TPointer<TWorkDialogForm> dlg = new TWorkDialogForm(::Application);
 //   auto_ptr<TWorkDialogForm> dlg(new TWorkDialogForm(this));

  dlg->TransferWork(Work);
 //   dlg->GetVCLForm()->TransferWork(Work);

  // Invoke the form, running it as a modal dialog
  //
   TModalResult result = dlg->ShowModal();
 // TModalResult result = dlg->GetVCLForm()->ShowModal();

  // If user closed with the OK button, change color setting
  //

  /*  if (result == mrNone) {
     result = dlg->ShowModal();
  } */

  if (result == mrOk) {

     Work =  dlg->ReturnWork(Work); //!!!!!  OK !!!
//    Work =  dlg->GetVCLForm()->ReturnWork(Work);
 ////////////////////////////////////////////////////////////////////

		Work->BuildWork();
		*StrPackEdit->WorkStart = *Work;

		TConnects* ConnectsCheck = new TConnects();
		if ( StrPackEdit->WorkStart->GetPercent() != 100 )
			{
			StrPackEdit->WorkStart->GetExitsConnect( ConnectsCheck );
			while ( !ConnectsCheck->IsEmpty() )
				{
				TConnect* connect = ConnectsCheck->PeekHead();
				ConnectsCheck->DetachAtHead();
				if ( connect->GetWorkFinish()->GetPercent() != 0 )
					{
					connect->GetWorkFinish()->SetPercent( 0 );
					connect->GetWorkFinish()->GetExitsConnect( ConnectsCheck );
					}
				}
			}
		if ( StrPackEdit->WorkStart->GetPercent() != 0 )
			{
			StrPackEdit->WorkStart->GetEntrancesConnect( ConnectsCheck );
			while ( !ConnectsCheck->IsEmpty() )
				{
				TConnect* connect = ConnectsCheck->PeekHead();
				ConnectsCheck->DetachAtHead();
				if ( connect->GetWorkStart()->GetPercent() != 100 )
					{
					connect->GetWorkStart()->SetPercent( 100 );
					connect->GetWorkStart()->GetEntrancesConnect( ConnectsCheck );
					}
				}
			}
		PackMain->ResetChec();
		StrPackEdit->WorkStart->GetExitsConnect( ConnectsCheck );
		while ( !ConnectsCheck->IsEmpty() )
			{
			TConnect* connect = ConnectsCheck->PeekHead();
			ConnectsCheck->DetachAtHead();
			connect->GetWorkFinish()->SetChec( ConnectsCheck );
			}

		StrPackEdit->WorkStart->GetExitsConnect( ConnectsCheck );
		while ( !ConnectsCheck->IsEmpty() )
			{
			TConnect* connect = ConnectsCheck->PeekHead();
			ConnectsCheck->DetachAtHead();
			connect->GetWorkFinish()->CorrectTime( connect->GetWorkStart()->GetFinishTime(), ConnectsCheck );
			}
		delete ConnectsCheck;

		PackMain->CorrectTime();
		Invalidate();
		}
	delete Work;

	StrPackEdit->WorkStart = NULL;
	return TRUE;
	}

LRESULT TMyLayout::EvEntranceInPack( WPARAM, LPARAM )
	{
	StrPackEdit->PackView = StrPackEdit->PackStart;
	StrPackEdit->PackStart = NULL;
	SetWindowText();
	Arrange();
	Invalidate();
	return TRUE;
	}

LRESULT TMyLayout::EvMoveInPack( WPARAM, LPARAM )
	{
	if ( StrPackEdit->PackView != PackMain )
		return FALSE;
	if ( StrPackEdit->WorkStart )
		{
		StrPackEdit->PackView->GetWorks()->Detach( StrPackEdit->WorkStart );
		StrPackEdit->PackFinish->Add( StrPackEdit->WorkStart, TRUE );
		StrPackEdit->WorkStart = NULL;
		}
	if ( StrPackEdit->PackStart )
		{
//		Not ready ! ! !
		StrPackEdit->PackStart = NULL;
		}
	StrPackEdit->PackFinish	= NULL;
	Invalidate(); // ???????????????????????
	return TRUE;
	}







