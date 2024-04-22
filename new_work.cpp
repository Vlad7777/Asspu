#include <owl\owlpch.h>
#include <owl\edit.h>
#include <owl\validate.h>
#include <owl\listbox.h>
#include <owl\checkbox.h>
#include <dir.h>

#include "work.h"
#include "new_work.h"
#include "file_dbf.h"
#include "speci.str"
#include "pack.str"
#include "brigade.str"
#include "mainw4.rh"

DEFINE_RESPONSE_TABLE1( TWorkDialog, TDialog )
	EV_LBN_SELCHANGE( ID_LISTBOXRESOURCEWORK, ListBoxResourceWorkChange ),
	EV_COMMAND( ID_ADDRESOURCE, IdAddResource ),
	EV_COMMAND( ID_REMOVERESOURCE, IdRemoveResource ),
END_RESPONSE_TABLE;

TWorkDialog::TWorkDialog( TWindow* parent, int resId, TWork* TheWork )
	: TDialog( parent, resId )
	{
	Work = TheWork;
	Resource = NULL;
	Resources = Work->GetResources();
	edit1 = new TEdit( this, ID_NAME, sizeof( StrWorkDialog.Name ) );

//	edit2 = new TEdit( this, ID_INDEXBRIGADE, sizeof( StrWorkDialog.IndexBrigade ) );
//	edit2->SetValidator( new TRangeValidator( 0, 2147483647L ) );
//	edit3 = new TEdit( this, ID_INDEXPACK, sizeof( StrWorkDialog.IndexPack ) );
//	edit3->SetValidator( new TRangeValidator( 0, 2147483647L ) );
	edit4 = new TEdit( this, ID_INDEXSHIFT, sizeof( StrWorkDialog.IndexShift ) );
	edit4->SetValidator( new TRangeValidator( 0, 3 ) );
	edit5 = new TEdit( this, ID_PERCENT, sizeof( StrWorkDialog.Percent ) );
	edit5->SetValidator( new TRangeValidator( 0, 100 ) );

	edit6 = new TEdit( this, ID_DURATION_hh, sizeof( StrWorkDialog.Duration_hh ) );
	edit6->SetValidator( new TRangeValidator( 0, 2147483647L ) );
	edit7 = new TEdit( this, ID_DURATION_mm, sizeof( StrWorkDialog.Duration_mm ) );
	edit7->SetValidator( new TRangeValidator( 0, 59 ) );
	edit8 = new TEdit( this, ID_DURATION_ss, sizeof( StrWorkDialog.Duration_ss ) );
	edit8->SetValidator( new TRangeValidator( 0, 59 ) );

	edit9 = new TEdit( this, ID_VOLUME, sizeof( StrWorkDialog.Volume ) );
	edit9->SetValidator( new TRangeValidator( 1L, 2147483647L ) );
	edit10 = new TEdit( this, ID_RESOURCEMIN, sizeof( StrWorkDialog.ResourceMin ) );
	edit10->SetValidator( new TRangeValidator( 1L, 2147483647L ) );
	edit11 = new TEdit( this, ID_RESOURCEOPT, sizeof( StrWorkDialog.ResourceOpt ) );
	edit11->SetValidator( new TRangeValidator( 1L, 2147483647L ) );
	edit12 = new TEdit( this, ID_RESOURCEMAX, sizeof( StrWorkDialog.ResourceMax ) );
	edit12->SetValidator( new TRangeValidator( 1L, 2147483647L ) );


	static1 = new TStatic( this, ID_RESOURCE, sizeof( StrWorkDialog.Resource ) );
	static2 = new TStatic( this, ID_SHIFTSTART_hh, sizeof( StrWorkDialog.ShiftStart_hh ) );
	static3 = new TStatic( this, ID_SHIFTSTART_mm, sizeof( StrWorkDialog.ShiftStart_mm ) );
	static4 = new TStatic( this, ID_SHIFTSTART_ss, sizeof( StrWorkDialog.ShiftStart_ss ) );
	static5 = new TStatic( this, ID_SHIFTFINISH_hh, sizeof( StrWorkDialog.ShiftFinish_hh ) );
	static6 = new TStatic( this, ID_SHIFTFINISH_mm, sizeof( StrWorkDialog.ShiftFinish_mm ) );
	static7 = new TStatic( this, ID_SHIFTFINISH_ss, sizeof( StrWorkDialog.ShiftFinish_ss ) );

	static8 = new TStatic( this, ID_START_DD, sizeof( StrWorkDialog.Start_DD ) );
	static9 = new TStatic( this, ID_START_MM, sizeof( StrWorkDialog.Start_MM ) );
	static10 = new TStatic( this, ID_START_YY, sizeof( StrWorkDialog.Start_YY ) );
	static11 = new TStatic( this, ID_START_hh, sizeof( StrWorkDialog.Start_hh ) );
	static12 = new TStatic( this, ID_START_mm, sizeof( StrWorkDialog.Start_mm ) );
	static13 = new TStatic( this, ID_START_ss, sizeof( StrWorkDialog.Start_ss ) );
	static14 = new TStatic( this, ID_FINISH_DD, sizeof( StrWorkDialog.Finish_DD ) );
	static15 = new TStatic( this, ID_FINISH_MM, sizeof( StrWorkDialog.Finish_MM ) );
	static16 = new TStatic( this, ID_FINISH_YY, sizeof( StrWorkDialog.Finish_YY ) );
	static17 = new TStatic( this, ID_FINISH_hh, sizeof( StrWorkDialog.Finish_hh ) );
	static18 = new TStatic( this, ID_FINISH_mm, sizeof( StrWorkDialog.Finish_mm ) );
	static19 = new TStatic( this, ID_FINISH_ss, sizeof( StrWorkDialog.Finish_ss ) );

	ListBoxPack = new TListBox( this, ID_INDEXPACK );
   ListBoxBrigade = new TListBox( this, ID_INDEXBRIGADE );
	ListBoxResourceAll = new TListBox( this, ID_LISTBOXRESOURCEALL );
	ListBoxResourceWork = new TListBox( this, ID_LISTBOXRESOURCEWORK );

	CheckBox = new TCheckBox( this, ID_WEEKEND );
	}

void TWorkDialog::SetupWindow()
	{
	TDialog::SetupWindow();

	strcpy( StrWorkDialog.Name, Work->GetName() );

//	itoa( Work->GetIndexBrigade(), StrWorkDialog.IndexBrigade, 10 );
//	itoa( Work->GetIndexPack(), StrWorkDialog.IndexPack, 10 );
	itoa( Work->GetIndexShift(), StrWorkDialog.IndexShift, 10 );
	itoa( Work->GetPercent(), StrWorkDialog.Percent, 10 );

	ltoa( (long)( Work->GetDuration() / 3600 ), StrWorkDialog.Duration_hh, 10 );
	itoa( (int)( Work->GetDuration() % 3600 / 60 ), StrWorkDialog.Duration_mm, 10 );
	itoa( (int)( Work->GetDuration() % 60 ), StrWorkDialog.Duration_ss, 10 );

	itoa( (int)( Work->GetShiftStart() / 3600 ), StrWorkDialog.ShiftStart_hh, 10 );
	itoa( (int)( Work->GetShiftStart() % 3600 / 60 ), StrWorkDialog.ShiftStart_mm, 10 );
	itoa( (int)( Work->GetShiftStart() % 60 ), StrWorkDialog.ShiftStart_ss, 10 );
	itoa( (int)( Work->GetShiftFinish() / 3600 ), StrWorkDialog.ShiftFinish_hh, 10 );
	itoa( (int)( Work->GetShiftFinish() % 3600 / 60 ), StrWorkDialog.ShiftFinish_mm, 10 );
	itoa( (int)( Work->GetShiftFinish() % 60 ), StrWorkDialog.ShiftFinish_ss, 10 );

	struct date StrDate;
	struct time StrTime;
	unixtodos( Work->GetStartTime(), &StrDate, &StrTime );
	itoa( (int)StrDate.da_day, StrWorkDialog.Start_DD, 10 );
	itoa( (int)StrDate.da_mon, StrWorkDialog.Start_MM, 10 );
	itoa( StrDate.da_year, StrWorkDialog.Start_YY, 10 );
	itoa( (int)StrTime.ti_hour, StrWorkDialog.Start_hh, 10 );
	itoa( (int)StrTime.ti_min, StrWorkDialog.Start_mm, 10 );
	itoa( (int)StrTime.ti_sec, StrWorkDialog.Start_ss, 10 );
	unixtodos( Work->GetFinishTime(), &StrDate, &StrTime );
	itoa( (int)StrDate.da_day, StrWorkDialog.Finish_DD, 10 );
	itoa( (int)StrDate.da_mon, StrWorkDialog.Finish_MM, 10 );
	itoa( StrDate.da_year, StrWorkDialog.Finish_YY, 10 );
	itoa( (int)StrTime.ti_hour, StrWorkDialog.Finish_hh, 10 );
	itoa( (int)StrTime.ti_min, StrWorkDialog.Finish_mm, 10 );
	itoa( (int)StrTime.ti_sec, StrWorkDialog.Finish_ss, 10 );

	edit1->SetText( StrWorkDialog.Name );
  //	edit2->SetText( StrWorkDialog.IndexBrigade );
  //	edit3->SetText( StrWorkDialog.IndexPack );
	edit4->SetText( StrWorkDialog.IndexShift );
	edit5->SetText( StrWorkDialog.Percent );

	static2->SetText( StrWorkDialog.ShiftStart_hh );
	static3->SetText( StrWorkDialog.ShiftStart_mm );
	static4->SetText( StrWorkDialog.ShiftStart_ss );
	static5->SetText( StrWorkDialog.ShiftFinish_hh );
	static6->SetText( StrWorkDialog.ShiftFinish_mm );
	static7->SetText( StrWorkDialog.ShiftFinish_ss );

	static8->SetText( StrWorkDialog.Start_DD );
	static9->SetText( StrWorkDialog.Start_MM );
	static10->SetText( StrWorkDialog.Start_YY );
	static11->SetText( StrWorkDialog.Start_hh );
	static12->SetText( StrWorkDialog.Start_mm );
	static13->SetText( StrWorkDialog.Start_ss );
	static14->SetText( StrWorkDialog.Finish_DD );
	static15->SetText( StrWorkDialog.Finish_MM );
	static16->SetText( StrWorkDialog.Finish_YY );
	static17->SetText( StrWorkDialog.Finish_hh );
	static18->SetText( StrWorkDialog.Finish_mm );
	static19->SetText( StrWorkDialog.Finish_ss );

   int i;   //////////////////////////////////new
	bool Free;
	char StringTmp[25];
	FileDBF *file = new FileDBF( "speci.dbf" );
	struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
	TSResourcesIterator ResourcesIterator( *Resources );
	for ( i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
		{
		SpeciStruct.Name[ 24 ] = '\0';
		Free = TRUE;
		ResourcesIterator.Restart();
		while ( ResourcesIterator )
			{
			TMyResource* resourceIt = ResourcesIterator++;
			if ( i == resourceIt->GetIndexResource() )
				{
				ListBoxResourceWork->AddString( SpeciStruct.Name );
				Free = FALSE;
				break;
				}
			}
		if ( Free )
			ListBoxResourceAll->AddString( SpeciStruct.Name );
		}
	delete file;

	file = new FileDBF( "pack.dbf" );
	struct pack PackStruct, *indexPackStruct = &PackStruct; //!!!!!!!!!!!!!!!
	i = 0;
	while ( file->readDBF( i++, (void *)indexPackStruct ) )
		{
		PackStruct.Name[ 24 ] = '\0';
		ListBoxPack->AddString( PackStruct.Name );
		}
	delete file;

	file = new FileDBF( "brigade.dbf" );
	struct brigade BrigadeStruct, *indexBrigadeStruct = &BrigadeStruct; //!!!!!!!!!!!!!!!
	i = 0;
	while ( file->readDBF( i++, (void *)indexBrigadeStruct ) )
		{
		BrigadeStruct.Name[ 24 ] = '\0';
		ListBoxBrigade->AddString( BrigadeStruct.Name );
		}
	delete file;


	if ( Work->GetWeekEnd() )
		CheckBox->SetCheck( BF_CHECKED );
	else
		CheckBox->SetCheck( BF_UNCHECKED );
	ListBoxResourceWorkChange();
	}

void TWorkDialog::IdAddResource()
	{
   int i;
	int Index = ListBoxResourceAll->GetSelIndex();
	if ( Index >= 0 )
		{
		char String[ 25 ];
		ListBoxResourceAll->GetString( String, Index );
		FileDBF *file = new FileDBF( "speci.dbf" );
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
		for ( /*int */ i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
			SpeciStruct.Name[ 24 ] = '\0';
			if ( strcmp( SpeciStruct.Name, String ) == 0 )
				break;
			}
		delete file;

		edit9->GetText( StrWorkDialog.Volume, sizeof( StrWorkDialog.Volume ) );
		edit10->GetText( StrWorkDialog.ResourceMin, sizeof( StrWorkDialog.ResourceMin ) );
		edit11->GetText( StrWorkDialog.ResourceOpt, sizeof( StrWorkDialog.ResourceOpt ) );
		edit12->GetText( StrWorkDialog.ResourceMax, sizeof( StrWorkDialog.ResourceMax ) );
		Resources->Add( new TMyResource( i,
												 atol( StrWorkDialog.ResourceMin ),
												 atol( StrWorkDialog.ResourceOpt ),
												 atol( StrWorkDialog.ResourceMax ),
												 atol( StrWorkDialog.Volume ) * 3600 ) );
		ListBoxResourceWork->AddString( String );
		ListBoxResourceAll->DeleteString( Index );
		char StringTmp[ 25 ];
		for ( Index = 0; Index < ListBoxResourceWork->GetCount(); Index++ )
			{
			ListBoxResourceWork->GetString( StringTmp, Index );
			if ( strcmp( String, StringTmp ) == 0 )
				{
				ListBoxResourceWork->SetSelIndex( Index );
				break;
				}
			}
		ListBoxResourceWorkChange();
		}
	}

void TWorkDialog::IdRemoveResource()
	{
   int i = 0;
	int Index = ListBoxResourceWork->GetSelIndex();
	if ( Index >= 0 )
		{
		char String[ 25 ];
		ListBoxResourceWork->GetString( String, Index );
		ListBoxResourceAll->AddString( String );
		ListBoxResourceWork->DeleteString( Index );
		FileDBF *file = new FileDBF( "speci.dbf" );
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
		for ( i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
			SpeciStruct.Name[ 24 ] = '\0';
			if ( strcmp( SpeciStruct.Name, String ) == 0 )
				break;
			}
		delete file;
		TSResourcesIterator ResourcesIterator( *Resources );
		while ( ResourcesIterator )
			{
			TMyResource* resourceIt = ResourcesIterator++;
			if (  i == resourceIt->GetIndexResource() )
				{
				Resources->Detach( resourceIt, TRUE );
				break;
				}
			}
		Resource = NULL;
		ListBoxResourceWorkChange();
		}

	}

void TWorkDialog::ListBoxResourceWorkChange()
	{
	if ( Resources->IsEmpty() )
		{
		edit6->SetText( StrWorkDialog.Duration_hh );
		edit7->SetText( StrWorkDialog.Duration_mm );
		edit8->SetText( StrWorkDialog.Duration_ss );
		edit6->EnableWindow( TRUE );
		edit7->EnableWindow( TRUE );
		edit8->EnableWindow( TRUE );
		}
	else
		{
		edit6->SetText( "" );
		edit7->SetText( "" );
		edit8->SetText( "" );
		edit6->EnableWindow( FALSE );
		edit7->EnableWindow( FALSE );
		edit8->EnableWindow( FALSE );
		}
	if ( Resource )
		{
		edit9->GetText( StrWorkDialog.Volume, sizeof( StrWorkDialog.Volume ) );
		edit10->GetText( StrWorkDialog.ResourceMin, sizeof( StrWorkDialog.ResourceMin ) );
		edit11->GetText( StrWorkDialog.ResourceOpt, sizeof( StrWorkDialog.ResourceOpt ) );
		edit12->GetText( StrWorkDialog.ResourceMax, sizeof( StrWorkDialog.ResourceMax ) );
		Resource->SetVolume( atol( StrWorkDialog.Volume ) * 3600 );
		Resource->SetResourceMin( atol( StrWorkDialog.ResourceMin ) );
		Resource->SetResourceOpt( atol( StrWorkDialog.ResourceOpt ) );
		Resource->SetResourceMax( atol( StrWorkDialog.ResourceMax ) );
		Resource->SetResourceMax();
		}
	int Index = ListBoxResourceWork->GetSelIndex();
	if ( Index >= 0 )
		{
		char String[ 25 ];
		ListBoxResourceWork->GetString( String, Index );
		FileDBF *file = new FileDBF( "speci.dbf" );
      int i;
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
		for (  i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
			SpeciStruct.Name[ 24 ] = '\0';
			if ( strcmp( SpeciStruct.Name, String ) == 0 )
				break;
			}
		delete file;

      TSResourcesIterator ResourcesIterator( *Resources );
		while ( ResourcesIterator )
			{
			TMyResource* resourceIt = ResourcesIterator++;
			if ( i == resourceIt->GetIndexResource() )
				{
				Resource = resourceIt;
				ltoa( Resource->GetVolume() / 3600, String, 10 );
				edit9->SetText( String );
				ltoa( Resource->GetResourceMin(), String, 10 );
				edit10->SetText( String );
				ltoa( Resource->GetResourceOpt(), String, 10 );
				edit11->SetText( String );
				ltoa( Resource->GetResourceMax(), String, 10 );
				edit12->SetText( String );
				ltoa( Resource->GetResource(), String, 10 );
				static1->SetText( String );
				break;
				}
			}
		edit9->EnableWindow( TRUE );
		edit10->EnableWindow( TRUE );
		edit11->EnableWindow( TRUE );
		edit12->EnableWindow( TRUE );
		static1->EnableWindow( TRUE );
		}
	else
		{
		edit9->SetText( "" );
		edit10->SetText( "" );
		edit11->SetText( "" );
		edit12->SetText( "" );
		static1->SetText( "" );
		edit9->EnableWindow( FALSE );
		edit10->EnableWindow( FALSE );
		edit11->EnableWindow( FALSE );
		edit12->EnableWindow( FALSE );
		static1->EnableWindow( FALSE );
		}
	}

void TWorkDialog::CloseWindow( int ret )
	{
	if ( Resources->IsEmpty() )
		{
		edit6->GetText( StrWorkDialog.Duration_hh, sizeof( StrWorkDialog.Duration_hh ) );
		edit7->GetText( StrWorkDialog.Duration_mm, sizeof( StrWorkDialog.Duration_mm ) );
		edit8->GetText( StrWorkDialog.Duration_ss, sizeof( StrWorkDialog.Duration_ss ) );
		Work->SetDuration( atoi( StrWorkDialog.Duration_hh ) * 3600UL + atoi( StrWorkDialog.Duration_mm ) * 60UL + atoi( StrWorkDialog.Duration_ss ) );
		}
	else
		ListBoxResourceWorkChange();

	edit1->GetText( StrWorkDialog.Name, sizeof( StrWorkDialog.Name ) );
//	edit2->GetText( StrWorkDialog.IndexBrigade, sizeof( StrWorkDialog.IndexBrigade ) );
//	edit3->GetText( StrWorkDialog.IndexPack, sizeof( StrWorkDialog.IndexPack ) );
	edit4->GetText( StrWorkDialog.IndexShift, sizeof( StrWorkDialog.IndexShift ) );
	edit5->GetText( StrWorkDialog.Percent, sizeof( StrWorkDialog.Percent ) );

	Work->SetName( StrWorkDialog.Name );
//	Work->SetIndexBrigade( atoi( StrWorkDialog.IndexBrigade ) );
//	Work->SetIndexPack( atoi( StrWorkDialog.IndexPack ) );
	Work->SetIndexShift( atoi( StrWorkDialog.IndexShift ) );
	Work->SetPercent( atoi( StrWorkDialog.Percent ) );

	int Index = ListBoxPack->GetSelIndex();
	if ( Index >= 0 )
		{
		char String[ 25 ];
		ListBoxPack->GetString( String, Index );
		FileDBF *file = new FileDBF( "pack.dbf" );
		struct pack PackStruct, *indexPackStruct = &PackStruct; //!!!!!!!!!!!!!!!
		int i = 0;
		while ( file->readDBF( i, (void *)indexPackStruct ) )
			{
			PackStruct.Name[ 24 ] = '\0';
			if ( strcmp( PackStruct.Name, String ) == 0 )
				Work->SetIndexPack( i );
			i++;
			}
		delete file;
		}

	int IndexB = ListBoxBrigade->GetSelIndex();
	if ( IndexB >= 0 )
		{
		char String[ 25 ];
		ListBoxBrigade->GetString( String, IndexB );
		FileDBF *file = new FileDBF( "brigade.dbf" );
		struct brigade BrigadeStruct, *indexBrigadeStruct = &BrigadeStruct; //!!!!!!!!!!!!!!!
		int i = 0;
		while ( file->readDBF( i, (void *)indexBrigadeStruct ) )
			{
			BrigadeStruct.Name[ 24 ] = '\0';
			if ( strcmp( BrigadeStruct.Name, String ) == 0 )
				Work->SetIndexBrigade( i );
			i++;
			}
		delete file;
		}


	if ( CheckBox->GetCheck() == BF_CHECKED )
		Work->SetWeekEnd();
	else
		Work->SetWeekEnd( FALSE );

	TDialog::CloseWindow( ret );    //  ret ?????????
	}

