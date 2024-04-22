
//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991,1993 by Borland International
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\opensave.h>
#include <owl\edit.h>                                          
#include <owl\validate.h>                                          
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "layout.h"
#include "mdl_wind.h"
#include "cal_wind.h"
#include "gnt_wind.h"
#include "gst_wind.h"
#include "mainw4.rh"

//
// Enables save command only if model is modified
//
void TMyLayout::CeFileSave( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( IsDirty ); }

//
// Enables save as command only if model is modified
//
void TMyLayout::CeFileSaveAs( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( IsDirty ); }

void TMyLayout::CeClosePack( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( PackMain != StrPackEdit->PackView ); }

void TMyLayout::CeIndexResource( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( AllWindow[2].BASScreen == TRUE ); }

void TMyLayout::CeSelectWork( TCommandEnabler& commandHandler )
	{ commandHandler.SetCheck( Type == 0 ); }

void TMyLayout::CeSelectPack( TCommandEnabler& commandHandler )
	{ commandHandler.SetCheck( Type == 1 ); }

void TMyLayout::CeBuildResource( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( PackMain->GetPacks()->IsEmpty() ); }

void TMyLayout::CeBuildPlan( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( PackMain->GetPacks()->IsEmpty() ); }

void TMyLayout::CmUnPack()
	{
	StrPackEdit->PackView->UnPack();
	Arrange();
	}

void TMyLayout::CeUnPack( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( !StrPackEdit->PackView->GetPacks()->IsEmpty() ); }

void TMyLayout::CmInPack()
	{
	StrPackEdit->PackView->OutShade();
	StrPackEdit->PackView->CorrectTime();
	Arrange();
	}

void TMyLayout::CeInPack( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( StrPackEdit->PackView->GetShade() != NULL ); }


void TMyLayout::CeConvert( TCommandEnabler& commandHandler )
	{
	if ( PackMain->GetPacks()->IsEmpty() && FileName )
		{
		unsigned long i = 0UL;
		TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->GetEntrancesConnect() == 0UL )
				i++;
			if ( work->GetExitsConnect() == 0UL )
				i++;
			}
		commandHandler.Enable( i == 2 );
		}
	else
		commandHandler.Enable( FALSE );
	}

void TMyLayout::CeOptimization( TCommandEnabler& commandHandler )
//	{ commandHandler.Enable( PackMain->GetPacks()->IsEmpty() ); 	}
	{
	if ( PackMain->GetPacks()->IsEmpty() && FileName )
		{
		unsigned long i = 0UL;
		TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->GetEntrancesConnect() == 0UL )
				i++;
			if ( work->GetExitsConnect() == 0UL )
				i++;
			}
		commandHandler.Enable( i == 2 );
		}
	else
		commandHandler.Enable( FALSE );
	}

void TMyLayout::CeUpdateModel( TCommandEnabler& commandHandler )
//	{ commandHandler.Enable( PackMain->GetPacks()->IsEmpty() ); 	}
	{
	if ( PackMain->GetPacks()->IsEmpty() && FileName )
		{
		unsigned long i = 0UL;
		TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->GetEntrancesConnect() == 0UL )
				i++;
			if ( work->GetExitsConnect() == 0UL )
				i++;
			}
		commandHandler.Enable( i == 2 );
		}
	else
		commandHandler.Enable( FALSE );
	}


void TMyLayout::CeSpuFox( TCommandEnabler& commandHandler )
	{
	if ( PackMain->GetPacks()->IsEmpty() && FileName )
		{
		unsigned long i = 0UL;
		TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->GetEntrancesConnect() == 0UL )
				i++;
			if ( work->GetExitsConnect() == 0UL )
				i++;
			}
		commandHandler.Enable( i == 2 );
		}
	else
		commandHandler.Enable( FALSE );
	}

void TMyLayout::CeFind( TCommandEnabler& commandHandler )
	{ commandHandler.Enable( AllWindow[0].BASScreen == TRUE ); }

void TMyLayout::CeActivateMDL( TCommandEnabler& commandHandler )
	{ commandHandler.SetCheck( AllWindow[0].BASScreen ); }

void TMyLayout::CeActivateGNT( TCommandEnabler& commandHandler )
	{ commandHandler.SetCheck( AllWindow[1].BASScreen ); }

void TMyLayout::CeActivateGST( TCommandEnabler& commandHandler )
	{ commandHandler.SetCheck( AllWindow[2].BASScreen ); }


