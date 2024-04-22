//---------------------------------------------------------------------------
#define   STRICT
#include <vcl.h>
#pragma hdrstop

//#include "pch.h"   //############################!!!!!!!!!
// After  close //#include "pch.h"  !!!!!  I include  2  #define
//#define  _USE_VCL_CM_EXIT
//#define   STRICT
/////////////////////////////////////////////////////////////////
#include "work.h"
//#include "new_work.h"
//#include "file_dbf.h"
#include "speci.str"
#include "pack.str"
#include "brigade.str"
#include "mainw4.rh"
#include "WorkDialogUnit.h"
//#include "SPUDMUnit.h" // Add !!!!!!!!!!!!!!! do not work ?????

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyButton"
#pragma link "MyListBox"
#pragma link "MyEdit"
#pragma link "MyCheckBox"
#pragma link "MyGroupBox"
#pragma link "MyCheckBox"
#pragma link "MyGroupBox"
#pragma link "MyListBox"
#pragma link "MyCheckBox"
#pragma link "MyGroupBox"
#pragma link "MyListBox"
#pragma resource "*.dfm"
TWorkDialogForm *WorkDialogForm;
//---------------------------------------------------------------------------
__fastcall TWorkDialogForm::TWorkDialogForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TWorkDialogForm::FormCreate(TObject *Sender)
{
//  Fill  All ListBoxes !!!
// Pack
  PackTable->Open();
  while (!PackTable->Eof){
//  ListBoxPack->Items->Add(PackTable->Fields[1]->AsString); //WAS !!!
  ListBoxPack->Items->Add(PackTable->Fields->Fields[3]->AsString);
  PackTable->Next();
  }
  PackTable->Close();

// Default the drop-down list to the first value in the list
 //  Do not select first !!!
//  ListBoxPack->ItemIndex = 0;    //  Select a needed ItemIndex  Pack!!!
//////////////////////////////////////////////////////////////
// Brigade
  BrigadeTable->Open();
  while (!BrigadeTable->Eof){
 // ListBoxBrigade->Items->Add(BrigadeTable->Fields[1]->AsString); //WAS !!!
  ListBoxBrigade->Items->Add(BrigadeTable->Fields->Fields[3]->AsString);
  BrigadeTable->Next();
  }
  BrigadeTable->Close();

// Default the drop-down list to the first value in the list
// Don't  select first !!!
//  ListBoxBrigade->ItemIndex = 0;  //  Select  a needed ItemIndex  Brigade!!!

}
//---------------------------------------------------------------------------

void TWorkDialogForm::TransferWork(TWork *Work)
{
  strcpy( StrWorkDialog.Name, Work->GetName() );
  MaskEdit1->Text = StrWorkDialog.Name;
//  Edit1->Text = StrWorkDialog.Name; //OK !!!
//  Edit4->???SetValidator( new TRangeValidator( 0, 3 ) );
////////////////////////////////////////////////////////////////////
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

	MaskEdit1->SetTextBuf( StrWorkDialog.Name );
  //	edit2->SetTextBuf( StrWorkDialog.IndexBrigade );
  //	edit3->SetTextBuf( StrWorkDialog.IndexPack );
	MaskEdit4->SetTextBuf( StrWorkDialog.IndexShift );
	MaskEdit5->SetTextBuf( StrWorkDialog.Percent );
             /////////////////////New ////////////
   // MaskEdit5->EditMask = "!999;1";

	StaticText2->SetTextBuf( StrWorkDialog.ShiftStart_hh );
	StaticText3->SetTextBuf( StrWorkDialog.ShiftStart_mm );
	StaticText4->SetTextBuf( StrWorkDialog.ShiftStart_ss );
	StaticText5->SetTextBuf( StrWorkDialog.ShiftFinish_hh );
	StaticText6->SetTextBuf( StrWorkDialog.ShiftFinish_mm );
	StaticText7->SetTextBuf( StrWorkDialog.ShiftFinish_ss );

	StaticText8->SetTextBuf( StrWorkDialog.Start_DD );
	StaticText9->SetTextBuf( StrWorkDialog.Start_MM );
	StaticText10->SetTextBuf( StrWorkDialog.Start_YY );
	StaticText11->SetTextBuf( StrWorkDialog.Start_hh );
	StaticText12->SetTextBuf( StrWorkDialog.Start_mm );
	StaticText13->SetTextBuf( StrWorkDialog.Start_ss );
	StaticText14->SetTextBuf( StrWorkDialog.Finish_DD );
	StaticText15->SetTextBuf( StrWorkDialog.Finish_MM );
	StaticText16->SetTextBuf( StrWorkDialog.Finish_YY );
	StaticText17->SetTextBuf( StrWorkDialog.Finish_hh );
	StaticText18->SetTextBuf( StrWorkDialog.Finish_mm );
	StaticText19->SetTextBuf( StrWorkDialog.Finish_ss );
    // My ADD
    StaticText20->SetTextBuf( StrWorkDialog.Duration_hh );
    StaticText21->SetTextBuf( StrWorkDialog.Duration_mm );
    StaticText22->SetTextBuf( StrWorkDialog.Duration_ss );
 // StaticText23->SetTextBuf( StrWorkDialog.Number );  // In the future !!
//////////////////////////RESORCES !!! /////////////////////////
  Resource = NULL;  //
  Resources = Work->GetResources();
    int i;   //////////////////////////////////new
	bool Free;
	char StringTmp[40];
//	FileDBF *file = new FileDBF( "speci.dbf" );
    SpeciTable->Open(); //
	struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
	TSResourcesIterator ResourcesIterator( *Resources );
 //	for ( i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
    for ( i = 0; i < SpeciTable->RecordCount  && i < TEvent::StrSetScale.QuantityResource; i++ )
		{
     //	SpeciStruct.Name[ 24 ] = '\0';
		Free = TRUE;
		ResourcesIterator.Restart();
		while ( ResourcesIterator )
			{
			TMyResource* resourceIt = ResourcesIterator++;
			if ( i == resourceIt->GetIndexResource() )
				{
		//		ListBoxResourceWork->AddString( SpeciStruct.Name );
                ListBoxResourceWork->Items->Add(SpeciTable->Fields->Fields[1]->AsString);
				Free = FALSE;
				break;
				}
			}
		if ( Free )
	   //		ListBoxResourceAll->AddString( SpeciStruct.Name );
                ListBoxResourceAll->Items->Add(SpeciTable->Fields->Fields[1]->AsString);

       // Next STR DB !!!!!!!!!
          SpeciTable->Next();
		}
//	delete file;
    SpeciTable->Close();

	if ( Work->GetWeekEnd() )
	  //WeekEndCheckBox->SetCheck( BF_CHECKED );
        WeekEndCheckBox->Checked;

	else
	  //WeekEndCheckBox->SetCheck( BF_UNCHECKED );
        WeekEndCheckBox->State = cbUnchecked;

 	ListBoxResourceWorkChange();  //  TEMPORARY !!!
}
//---------------------------------------------------------------------------
void TWorkDialogForm::IdAddResource()
	{
    int i;
  //int Index = ListBoxResourceAll->GetSelIndex();
    int Index = ListBoxResourceAll->ItemIndex;

	if ( Index >= 0 )
		{
		char String[ 40 ]; // New size !!!
//      ListBoxResourceAll->GetString( String, Index );
        strcpy(String, ListBoxResourceAll->Items->Strings[Index].c_str());

  //	FileDBF *file = new FileDBF( "speci.dbf" );
        SpeciTable->Open();  // !!!
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
	   //	for (  i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
        for ( i = 0; i < SpeciTable->RecordCount  && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
			//SpeciStruct.Name[ 24 ] = '\0';
			//if ( strcmp( SpeciStruct.Name, String ) == 0 )
            // STR DB !!!!!!!!!!!!!!
         //   if  (SpeciTable->FieldByName("NAIMSPEC")->AsString == ListBoxResourceWork->Items->Strings[Index])
         //     if  (SpeciTable->FieldByName("NAIMSPEC")->AsString.c_str() == String)
              if ( strcmp (SpeciTable->FieldByName("NAIMSPEC")->AsString.c_str(), String ) == 0 )
		      break;

            // Next !!!!!!!!!!!!!!!!
            SpeciTable->Next();
			}
//		delete file;
        SpeciTable->Close();

		MaskEdit9->GetTextBuf( StrWorkDialog.Volume, sizeof( StrWorkDialog.Volume ) );
		MaskEdit10->GetTextBuf( StrWorkDialog.ResourceMin, sizeof( StrWorkDialog.ResourceMin ) );
		MaskEdit11->GetTextBuf( StrWorkDialog.ResourceOpt, sizeof( StrWorkDialog.ResourceOpt ) );
		MaskEdit12->GetTextBuf( StrWorkDialog.ResourceMax, sizeof( StrWorkDialog.ResourceMax ) );
		Resources->Add( new TMyResource( i,
												 atol( StrWorkDialog.ResourceMin ),
												 atol( StrWorkDialog.ResourceOpt ),
												 atol( StrWorkDialog.ResourceMax ),
												 atol( StrWorkDialog.Volume ) * 3600 ) );
   //	ListBoxResourceWork->AddString( String );
        ListBoxResourceWork->Items->Add(String );

   //	ListBoxResourceAll->DeleteString( Index );
        ListBoxResourceAll->Items->Delete( Index );

		char StringTmp[ 40 ];
		for ( Index = 0; Index < ListBoxResourceWork->Items->Count; Index++ )
			{
	 //		ListBoxResourceWork->GetString( StringTmp, Index );
            strcpy(StringTmp, ListBoxResourceWork->Items->Strings[Index].c_str());

			if ( strcmp( String, StringTmp ) == 0 )
              	{
		   //	ListBoxResourceWork->SetSelIndex( Index );
                ListBoxResourceWork->Selected[Index]; // Check it !!!
				break;
				}
              }
		ListBoxResourceWorkChange();
		}
     }
//---------------------------------------------------------------------------
void TWorkDialogForm::IdRemoveResource()
	{

    int i = 0;
 //	int Index = ListBoxResourceWork->GetSelIndex();
    int Index = ListBoxResourceWork->ItemIndex;

	if ( Index >= 0 )
		{
		char String[ 40 ];
   //	ListBoxResourceWork->GetString( String, Index );
        strcpy(String, ListBoxResourceWork->Items->Strings[Index].c_str());

//		ListBoxResourceAll->AddString( String );
//      ListBoxResourceAll->Items->Add( String );
        ListBoxResourceAll->Items->Add(ListBoxResourceWork->Items->Strings[Index]);  // Check !!!

	//	ListBoxResourceWork->DeleteString( Index );
        ListBoxResourceWork->Items->Delete( Index ); // Check!!!

   //	FileDBF *file = new FileDBF( "speci.dbf" );
        SpeciTable->Open();   //
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
//		for ( i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
        for ( i = 0; i < SpeciTable->RecordCount  && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
		 //	SpeciStruct.Name[ 24 ] = '\0';
         //	if ( strcmp( SpeciStruct.Name, String ) == 0 )
         // if  (SpeciTable->FieldByName("NAIMSPEC")->AsString == ListBoxResourceWork->Items->Strings[Index])
            if ( strcmp (SpeciTable->FieldByName("NAIMSPEC")->AsString.c_str(), ListBoxResourceWork->Items->Strings[Index].c_str()) == 0 )
		   	break;
            SpeciTable->Next();
			}
  //		delete file;
            SpeciTable->Close();

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
//---------------------------------------------------------------------------
void TWorkDialogForm::ListBoxResourceWorkChange()
	{
	if ( Resources->IsEmpty() )
		{
		MaskEdit6->SetTextBuf( StrWorkDialog.Duration_hh );
             /////////////////////New ////////////
     //   MaskEdit6->EditMask = "!9999;1";

		MaskEdit7->SetTextBuf( StrWorkDialog.Duration_mm );
		MaskEdit8->SetTextBuf( StrWorkDialog.Duration_ss );
        MaskEdit6->Enabled = true ;
		MaskEdit7->Enabled = true ;
		MaskEdit8->Enabled = true ;
		}
	else
		{
		MaskEdit6->SetTextBuf( "" );
		MaskEdit7->SetTextBuf( "" );
		MaskEdit8->SetTextBuf( "" );
        MaskEdit6->Enabled = false ;
		MaskEdit7->Enabled = false ;
		MaskEdit8->Enabled = false ;
		}
	if ( Resource )  // !!!
		{

		MaskEdit9->GetTextBuf( StrWorkDialog.Volume, sizeof( StrWorkDialog.Volume ) );
		MaskEdit10->GetTextBuf( StrWorkDialog.ResourceMin, sizeof( StrWorkDialog.ResourceMin ) );
		MaskEdit11->GetTextBuf( StrWorkDialog.ResourceOpt, sizeof( StrWorkDialog.ResourceOpt ) );
		MaskEdit12->GetTextBuf( StrWorkDialog.ResourceMax, sizeof( StrWorkDialog.ResourceMax ) );
		Resource->SetVolume( atol( StrWorkDialog.Volume ) * 3600 );
		Resource->SetResourceMin( atol( StrWorkDialog.ResourceMin ) );
		Resource->SetResourceOpt( atol( StrWorkDialog.ResourceOpt ) );
		Resource->SetResourceMax( atol( StrWorkDialog.ResourceMax ) );
		Resource->SetResourceMax();   //    max persons !!!
		}
/////////////////////////////////////////////////////////////////////////
  //  int Index = ListBoxResourceWork->GetSelIndex();
      int Index = ListBoxResourceWork->ItemIndex;

  if ( Index >= 0 )
	 {
	  char String[ 40 ];
	  // ListBoxResourceWork->GetString( String, Index );
      strcpy(String, ListBoxResourceWork->Items->Strings[Index].c_str());

     //FileDBF *file = new FileDBF( "speci.dbf" );
      SpeciTable->Open();
      int i;
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
   //  for (  i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
        for ( i = 0; i < SpeciTable->RecordCount  && i < TEvent::StrSetScale.QuantityResource; i++ ) //
			{
		 //	SpeciStruct.Name[ 24 ] = '\0'; //  Take away ???
      	//if ( strcmp( SpeciStruct.Name, String ) == 0 )
        //  Compare strings !!!
        //   (SpeciTable->FieldByName("NOMERSPEC")->AsString == String)
        if ( strcmp (SpeciTable->FieldByName("NAIMSPEC")->AsString.c_str(), String)  == 0 )
			break;
            SpeciTable->Next();
			}
	//	delete file;
        SpeciTable->Close();

      TSResourcesIterator ResourcesIterator( *Resources );
		while ( ResourcesIterator )
			{
			TMyResource* resourceIt = ResourcesIterator++;
			if ( i == resourceIt->GetIndexResource() )
				{
				Resource = resourceIt;
				ltoa( Resource->GetVolume() / 3600, String, 10 );
				MaskEdit9->SetTextBuf( String );
             /////////////////////New ////////////
       //         MaskEdit9->EditMask = "!99999;1";
             //	MaskEdit1->Text = "";
	         // MaskEdit9->AutoSelect = false;
             //////////////////////////////////////////////

				ltoa( Resource->GetResourceMin(), String, 10 );
				MaskEdit10->SetTextBuf( String );
             /////////////////////New ////////////
         //       MaskEdit10->EditMask = "!99;1";
				ltoa( Resource->GetResourceOpt(), String, 10 );
				MaskEdit11->SetTextBuf( String );
             /////////////////////New ////////////
           //     MaskEdit11->EditMask = "!99;1";

				ltoa( Resource->GetResourceMax(), String, 10 );
				MaskEdit12->SetTextBuf( String );
             /////////////////////New ////////////
            //    MaskEdit12->EditMask = "!99;1";

				ltoa( Resource->GetResource(), String, 10 );
				StaticText1->SetTextBuf( String );
				break;
				}
			}
		MaskEdit9->Enabled = true ;
		MaskEdit10->Enabled = true ;
		MaskEdit11->Enabled = true ;
		MaskEdit12->Enabled = true ;
		StaticText1->Enabled = true ;
		}
	else
		{
		MaskEdit9->SetTextBuf( "" );
		MaskEdit10->SetTextBuf( "" );
		MaskEdit11->SetTextBuf( "" );
		MaskEdit12->SetTextBuf( "" );
		StaticText1->SetTextBuf( "" );
		MaskEdit9->Enabled = false;
		MaskEdit10->Enabled = false;
		MaskEdit11->Enabled = false;
		MaskEdit12->Enabled = false;
		StaticText1->Enabled = false;
		}

	}
//---------------------------------------------------------------------------

  TWork* TWorkDialogForm::ReturnWork(TWork *Work)
     {
   // Analog  Close Window !!!!!!!! OR  in FORMCLOSE ???? !!!
   // Assign All parameters of Work  from Form (StrWorkDialog) !!!

	if ( Resources->IsEmpty() )
		{
  		MaskEdit6->GetTextBuf( StrWorkDialog.Duration_hh, sizeof( StrWorkDialog.Duration_hh ) );
		MaskEdit7->GetTextBuf( StrWorkDialog.Duration_mm, sizeof( StrWorkDialog.Duration_mm ) );
		MaskEdit8->GetTextBuf( StrWorkDialog.Duration_ss, sizeof( StrWorkDialog.Duration_ss ) );

		Work->SetDuration( atoi( StrWorkDialog.Duration_hh ) * 3600UL + atoi( StrWorkDialog.Duration_mm ) * 60UL + atoi( StrWorkDialog.Duration_ss ) );
		}
	else
		ListBoxResourceWorkChange();

    MaskEdit1->GetTextBuf(StrWorkDialog.Name, sizeof(StrWorkDialog.Name));
//	edit2->GetText( StrWorkDialog.IndexBrigade, sizeof( StrWorkDialog.IndexBrigade ) );
//	edit3->GetText( StrWorkDialog.IndexPack, sizeof( StrWorkDialog.IndexPack ) );
	MaskEdit4->GetTextBuf( StrWorkDialog.IndexShift, sizeof( StrWorkDialog.IndexShift ) );
	MaskEdit5->GetTextBuf( StrWorkDialog.Percent, sizeof( StrWorkDialog.Percent ) );

	Work->SetName( StrWorkDialog.Name ); // OK !!!
//	Work->SetIndexBrigade( atoi( StrWorkDialog.IndexBrigade ) );
//	Work->SetIndexPack( atoi( StrWorkDialog.IndexPack ) );
	Work->SetIndexShift( atoi( StrWorkDialog.IndexShift ) );
	Work->SetPercent( atoi( StrWorkDialog.Percent ) );

//	int Index = ListBoxPack->GetSelIndex();
    int Index = ListBoxPack->ItemIndex;

	if ( Index >= 0 )
		{
		char String[ 40 ];  //  dimension 40  !!!!!!!!!!!!!!!!!!!
     // ListBoxPack->GetString( String, Index );
        strcpy(String, ListBoxPack->Items->Strings[Index].c_str());

  //    FileDBF *file = new FileDBF( "pack.dbf" );
        PackTable->Open();  //
		struct pack PackStruct, *indexPackStruct = &PackStruct; //!!!!!!!!!!!!!!!
		int i = 0; //  !!! Check i == 0 or 1  for search ???
	//	while ( file->readDBF( i, (void *)indexPackStruct ) )
        while (!PackTable->Eof)  // OK !!!
			{
		//	PackStruct.Name[ 24 ] = '\0'; // SIZE !!!!
		//	if ( strcmp( PackStruct.Name, String ) == 0 )
                                    // OLD Structure !!!
        if ( strcmp (PackTable->FieldByName("NAIMEN")->AsString.c_str(), String) == 0 )

                {
				Work->SetIndexPack( i );
                break;
                }
            PackTable->Next();
			i++;
			}
	 //	delete file;
        PackTable->Close();
		}

//	int IndexB = ListBoxBrigade->GetSelIndex();
    int IndexB= ListBoxBrigade->ItemIndex;

	if ( IndexB >= 0 )
		{
		char String[ 40 ]; // SIZE !!!!!!!!!!!!
//		ListBoxBrigade->GetString( String, IndexB );
        strcpy(String, ListBoxBrigade->Items->Strings[IndexB].c_str());
	//	FileDBF *file = new FileDBF( "brigade.dbf" );
        BrigadeTable->Open();
		struct brigade BrigadeStruct, *indexBrigadeStruct = &BrigadeStruct; //!!!!!!!!!!!!!!!
		int i = 0;
	  //	while ( file->readDBF( i, (void *)indexBrigadeStruct ) )
        while (!BrigadeTable->Eof) // !!!
			{
	   // 	 BrigadeStruct.Name[ 24 ] = '\0';
	   // 	 if ( strcmp( BrigadeStruct.Name, String ) == 0 )
             /// str DB !!!!!!!!!!!!!!
                                        // OLD Structure !!!
          //   if  (BrigadeTable->FieldByName("NAIMP")->AsString.c_str() == ListBoxBrigade->Items->Strings[Index])
               if ( strcmp(BrigadeTable->FieldByName("NAIMP")->AsString.c_str(),  String ) == 0 )

               {
				Work->SetIndexBrigade( i );
                break;
               }
			 i++;
             BrigadeTable->Next();
			}
   //	delete file;
        BrigadeTable->Close();
		}

//	if ( CheckBox->GetCheck() == BF_CHECKED )
  	if ( WeekEndCheckBox->State == cbChecked )

		Work->SetWeekEnd();
	else
		Work->SetWeekEnd( FALSE );

//	TDialog::CloseWindow( ret );    //  ret ???

  return Work;  //
}

//---------------------------------------------------------------------------
void __fastcall TWorkDialogForm::AddResourceBitBtnClick(TObject *Sender)
{
   IdAddResource();
}
//---------------------------------------------------------------------------

void __fastcall TWorkDialogForm::RemoveResourceBitBtnClick(TObject *Sender)
{
   IdRemoveResource();
}
//---------------------------------------------------------------------------

void __fastcall TWorkDialogForm::ListBoxResourceWorkClick(TObject *Sender)
{
 // New event !!!
  ListBoxResourceWorkChange();

        ///////////////////New /////////////////////////////////
/*
        char String[10];
        int n;
		MaskEdit9->GetTextBuf( String, sizeof(String));
        n = atoi(String);
        if (n == 0)
        ShowMessage (" Error in  Volume!!!");
 */
      ////////////////////////////////////////////////////////////////



}
//---------------------------------------------------------------------------

void __fastcall TWorkDialogForm::ListBoxResourceWorkExit(TObject *Sender)
{
  // New event !!!
//  ListBoxResourceWorkChange();
}
//---------------------------------------------------------------------------

void __fastcall TWorkDialogForm::ListBoxResourceWorkEnter(TObject *Sender)
{
// ListBoxResourceWorkChange();
}
//---------------------------------------------------------------------------


void __fastcall TWorkDialogForm::BitBtnOKClick(TObject *Sender)
{
    /*    char String[10];
        int n;
		MaskEdit9->GetTextBuf( String, sizeof(String));
        n = atoi(String);

        if (n == 0)
           {
            ShowMessage (" Error in  Volume!!!");
        //    WorkDialogForm->ModalResult = mrRetry;  //  Not enouph indices !!!
            ModalResult = mrNone;   // mrRetry;   or  mrNone
           }
           else
           {
        //    WorkDialogForm->ModalResult = mrOk;
           ModalResult = mrOk;
           }
      */
}
//---------------------------------------------------------------------------




