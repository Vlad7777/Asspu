#include <owl\owlpch.h>
#include <owl\edit.h>                                          
#include <owl\validate.h>
#include <owl\listbox.h>
#include <dir.h>

#include "pack.h"
#include "new_pack.h"
#include "mainw4.rh"


TPackDialog::TPackDialog( TWindow* parent, int resId, TPack* ThePack )
	: TDialog( parent, resId )
	{
	Pack = ThePack;
	edit1 = new TEdit( this, ID_NAME, sizeof( StrPackDialog.Name ) );
	edit2 = new TEdit( this, ID_START_DD, sizeof( StrPackDialog.Start_DD ) );
	edit2->SetValidator( new TRangeValidator( 1, 31 ) );
	edit3 = new TEdit( this, ID_START_MM, sizeof( StrPackDialog.Start_MM ) );
	edit3->SetValidator( new TRangeValidator( 1, 12 ) );
	edit4 = new TEdit( this, ID_START_YY, sizeof( StrPackDialog.Start_YY ) );
	edit4->SetValidator( new TRangeValidator( 1971, 2037 ) );
	edit5 = new TEdit( this, ID_START_hh, sizeof( StrPackDialog.Start_hh ) );
	edit5->SetValidator( new TRangeValidator( 0, 23 ) );
	edit6 = new TEdit( this, ID_START_mm, sizeof( StrPackDialog.Start_mm ) );
	edit6->SetValidator( new TRangeValidator( 0, 59 ) );
	edit7 = new TEdit( this, ID_START_ss, sizeof( StrPackDialog.Start_ss ) );
	edit7->SetValidator( new TRangeValidator( 0, 59 ) );
	edit8 = new TEdit( this, ID_FINISH_DD, sizeof( StrPackDialog.Finish_DD ) );
	edit8->SetValidator( new TRangeValidator( 1, 31 ) );
	edit9 = new TEdit( this, ID_FINISH_MM, sizeof( StrPackDialog.Finish_MM ) );
	edit9->SetValidator( new TRangeValidator( 1, 12 ) );
	edit10 = new TEdit( this, ID_FINISH_YY, sizeof( StrPackDialog.Finish_YY ) );
	edit10->SetValidator( new TRangeValidator( 1971, 2037 ) );
	edit11 = new TEdit( this, ID_FINISH_hh, sizeof( StrPackDialog.Finish_hh ) );
	edit11->SetValidator( new TRangeValidator( 0, 23 ) );
	edit12 = new TEdit( this, ID_FINISH_mm, sizeof( StrPackDialog.Finish_mm ) );
	edit12->SetValidator( new TRangeValidator( 0, 59 ) );
	edit13 = new TEdit( this, ID_FINISH_ss, sizeof( StrPackDialog.Finish_ss ) );
	edit13->SetValidator( new TRangeValidator( 0, 59 ) );
	}

void TPackDialog::SetupWindow()
	{
	TDialog::SetupWindow();

	strcpy( StrPackDialog.Name, Pack->GetName() );
	struct date StrDate;
	struct time StrTime;
	unixtodos( Pack->GetStartTime(), &StrDate, &StrTime );
	itoa( (int)StrDate.da_day, StrPackDialog.Start_DD, 10 );
	itoa( (int)StrDate.da_mon, StrPackDialog.Start_MM, 10 );
	itoa( StrDate.da_year, StrPackDialog.Start_YY, 10 );
	itoa( (int)StrTime.ti_hour, StrPackDialog.Start_hh, 10 );
	itoa( (int)StrTime.ti_min, StrPackDialog.Start_mm, 10 );
	itoa( (int)StrTime.ti_sec, StrPackDialog.Start_ss, 10 );
	unixtodos( Pack->GetFinishTime(), &StrDate, &StrTime );
	itoa( (int)StrDate.da_day, StrPackDialog.Finish_DD, 10 );
	itoa( (int)StrDate.da_mon, StrPackDialog.Finish_MM, 10 );
	itoa( StrDate.da_year, StrPackDialog.Finish_YY, 10 );
	itoa( (int)StrTime.ti_hour, StrPackDialog.Finish_hh, 10 );
	itoa( (int)StrTime.ti_min, StrPackDialog.Finish_mm, 10 );
	itoa( (int)StrTime.ti_sec, StrPackDialog.Finish_ss, 10 );

	edit1->SetText( StrPackDialog.Name );
	edit2->SetText( StrPackDialog.Start_DD ); 
	edit3->SetText( StrPackDialog.Start_MM );
	edit4->SetText( StrPackDialog.Start_YY );
	edit5->SetText( StrPackDialog.Start_hh );
	edit6->SetText( StrPackDialog.Start_mm ); 
	edit7->SetText( StrPackDialog.Start_ss ); 
	edit8->SetText( StrPackDialog.Finish_DD ); 
	edit9->SetText( StrPackDialog.Finish_MM );
	edit10->SetText( StrPackDialog.Finish_YY );
	edit11->SetText( StrPackDialog.Finish_hh );
	edit12->SetText( StrPackDialog.Finish_mm );
	edit13->SetText( StrPackDialog.Finish_ss );
	}
	
void TPackDialog::CloseWindow( int ret )
	{
	edit1->GetText( StrPackDialog.Name, sizeof( StrPackDialog.Name ) );
	edit2->GetText( StrPackDialog.Start_DD, sizeof(StrPackDialog.Start_DD ) ); 
	edit3->GetText( StrPackDialog.Start_MM, sizeof( StrPackDialog.Start_MM ) );
	edit4->GetText( StrPackDialog.Start_YY, sizeof( StrPackDialog.Start_YY ) );
	edit5->GetText( StrPackDialog.Start_hh, sizeof( StrPackDialog.Start_hh ) );
	edit6->GetText( StrPackDialog.Start_mm, sizeof( StrPackDialog.Start_mm ) ); 
	edit7->GetText( StrPackDialog.Start_ss, sizeof( StrPackDialog.Start_ss ) ); 
	edit8->GetText( StrPackDialog.Finish_DD, sizeof( StrPackDialog.Finish_DD ) ); 
	edit9->GetText( StrPackDialog.Finish_MM, sizeof( StrPackDialog.Finish_MM ) );
	edit10->GetText( StrPackDialog.Finish_YY, sizeof( StrPackDialog.Finish_YY ) );
	edit11->GetText( StrPackDialog.Finish_hh, sizeof( StrPackDialog.Finish_hh ) );
	edit12->GetText( StrPackDialog.Finish_mm, sizeof( StrPackDialog.Finish_mm ) );
	edit13->GetText( StrPackDialog.Finish_ss, sizeof( StrPackDialog.Finish_ss ) );

	Pack->SetName( StrPackDialog.Name );
	struct date StrDate;
	struct time StrTime;
	StrDate.da_year = atoi( StrPackDialog.Start_YY );
	StrDate.da_mon = (char)atoi( StrPackDialog.Start_MM );
	StrDate.da_day = (char)atoi( StrPackDialog.Start_DD );
	StrTime.ti_hour = (char)atoi( StrPackDialog.Start_hh );
	StrTime.ti_min = (char)atoi( StrPackDialog.Start_mm );
	StrTime.ti_sec = (char)atoi( StrPackDialog.Start_ss );
	StrTime.ti_hund = (char)0;		
	Pack->SetStartTime( dostounix( &StrDate, &StrTime ) );
	StrDate.da_year = atoi( StrPackDialog.Finish_YY );
	StrDate.da_mon = (char)atoi( StrPackDialog.Finish_MM );
	StrDate.da_day = (char)atoi( StrPackDialog.Finish_DD );
	StrTime.ti_hour = (char)atoi( StrPackDialog.Finish_hh );
	StrTime.ti_min = (char)atoi( StrPackDialog.Finish_mm );
	StrTime.ti_sec = (char)atoi( StrPackDialog.Finish_ss );
	StrTime.ti_hund = (char)0;		
	Pack->SetFinishTime( dostounix( &StrDate, &StrTime ) );

	TDialog::CloseWindow( ret );
	}