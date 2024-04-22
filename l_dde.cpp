#define  STRICT
//#define  _USE_VCL_CM_EXIT  //???
#include <vcl.h> // ???
#include <owl\owlpch.h>
#include <stdlib.h>
#include <stdio.h>
#include <dir.h>
#include <mem.h>
#include <string.h>

#include "mainw4.rh"
#include "layout.h"
#include "file_dbf.h"
#include "smuzi.str"
#include "smuzl.str"
#include "speci.str"
#include "pack.str"
#include "brigade.str"

/////////////////// BCB !!! ////////
#include "SPUDMUnit.h"


///////////////
//#include "pch.h"      // #############
#pragma hdrstop
#include <owl/applicat.h>
#include <classlib/pointer.h>

//#include "adopt.h"              // for adopting VCL's application window
#define  STRICT
#include "DBTESTUnit.h"     // C++Builder form unit header




bool TMyLayout::WriteDB( TConnects* Check ) //  Write MDL->DB  !!!

	{
	HCURSOR hcurSave=::SetCursor(LoadCursor(NULL,IDC_WAIT));
////////////////////////////////////// BCB3 #################
	struct smuzi SmuziStruct, *indexSmuziStruct = &SmuziStruct;
	strcpy( SmuziStruct.Name, PackMain->GetName() );
	ultoa( PackMain->GetStartTime(), SmuziStruct.StartTime, 10 );
	ultoa( PackMain->GetFinishTime(), SmuziStruct.FinishTime, 10 );

  // Create the form object
  //
    TPointer<TDBTESTForm> dlgDBSPU = new TDBTESTForm(::Application);
    // Try to change the  <TDBTESTForm>  on  SPUDB (TDataModule) !!!
    dlgDBSPU->TransferSmuziStruct(0, &SmuziStruct);
  // Invoke the form, running it as a modal dialog
  /*
    TModalResult result = dlgDBSPU->ShowModal(); // Possible  without Show !!!
  // If user closed with the OK button, change Work setting
  //
    if (result == mrOk) {
 //  Work =  dlgDBSPU->ReturnWork(????Work);    //  Need Result ?????
    }
   */
 //delete  dlgDBSPU; // It is Error.  Are there  any problems with leaking ?????
//////////////////////////////////////////////////////////////////////////////////
    struct smuzl SmuzlStruct, *indexSmuzlStruct = &SmuzlStruct;

  //	FileDBF  *myfile = new FileDBF( "smuzl.dbf" );
  //	struct smuzl SmuzlStruct, *indexSmuzlStruct = &SmuzlStruct;
    int Del = 0; // Lipa !!!
	long i = 1;              //  MY CHANGE  i=1 !!!! WAS 0 !!!
	unsigned long Number;
	char StringTmp[ 12 ];

   //	FileDBF *fileS = new FileDBF( "speci.dbf" );
	struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
    // My ADD  !!!
    setmem( indexSpeciStruct, sizeof( SpeciStruct ), '\0' );//my NEW !!!

/////////////////////////!!!!!!!!!!!!!!!!!!///////////////////////////////////////////////
// Delete ALL !!!!
// New addition  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//???? Doubling of works!!!!
 dlgDBSPU->DelAllNullNumberRecord();
////////////////////////////////////////////////////////////


	TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->SetTmp( 1 );

//	while ( i )  /// myfile->readDBF( i, (void *)indexSmuzlStruct
    while ( dlgDBSPU->ReadDBF(i, &SmuzlStruct))
    		{
		if ( i >= 1 )   //  !myfile->IsDelRecord( i )   LIPA
			{
			Number = atol( SmuzlStruct.Number );
			if ( Number != 0 )
				{

				TWork* workTmp = NULL;
				DWorksIterator.Restart();
				while ( DWorksIterator )
					{
					TWork* work = DWorksIterator++;
					if ( Number == work->GetNumber() )
						{                              //
                        setmem( indexSmuzlStruct, sizeof( SmuzlStruct ), '\0' );//my NEW !!!
						workTmp = work;
						work->SetTmp( 0 );
						strcpy( SmuzlStruct.Name, work->GetName() );
						ultoa( work->GetNumber(), SmuzlStruct.Number, 10 );
               // Later I think b1 == 2 bytes !!!    ultoa( 0, SmuzlStruct.b1, 10 ); //  PRIZ !!!  My Add
						ultoa( work->GetStartTime(), SmuzlStruct.StartTime, 10 );
						ultoa( work->GetFinishTime(), SmuzlStruct.FinishTime, 10 );

						itoa( work->GetIndexShift(), SmuzlStruct.IndexShift, 10 );
						itoa( work->GetIndexBrigade(), SmuzlStruct.IndexBrigade, 10 );


			   //		FileDBF *fileP = new FileDBF( "pack.dbf" );
						struct pack PackStruct, *indexPackStruct = &PackStruct;
                        // My ADD  !!!
                        setmem( indexPackStruct, sizeof( PackStruct ), ' ' );//my NEW !!!
			   //		fileP->readDBF( work->GetIndexPack(), (void *)indexPackStruct );
                        dlgDBSPU->ReadDBFPack(work->GetIndexPack() + 1, &PackStruct);
						strncpy( SmuzlStruct.c5, PackStruct.b1, 11 );
						itoa( work->GetIndexPack(), SmuzlStruct.IndexPack, 10 );
				//		delete fileP;

				//		FileDBF *fileB = new FileDBF( "brigade.dbf" );
						struct brigade BrigadeStruct, *indexBrigadeStruct = &BrigadeStruct;
                                                // My ADD  !!!
                        setmem( indexBrigadeStruct, sizeof( BrigadeStruct ), ' ' );//my NEW !!!

				//		fileB->readDBF( work->GetIndexBrigade(), (void *)indexBrigadeStruct );
                        dlgDBSPU->ReadDBFBrigade(work->GetIndexBrigade() + 1, &BrigadeStruct);
						strncpy( SmuzlStruct.c0  , BrigadeStruct.b1, 11 );
						itoa( work->GetIndexBrigade(), SmuzlStruct.IndexBrigade, 10 );
				//		delete fileB;


						ultoa( work->GetDuration(), SmuzlStruct.Duration, 10 );
						itoa( work->GetPercent(), SmuzlStruct.Percent, 10 );
						ultoa( work->GetShiftStart(), SmuzlStruct.ShiftStart, 10 );
						ultoa( work->GetShiftFinish(), SmuzlStruct.ShiftFinish, 10 );
						itoa( work->GetCritical(), SmuzlStruct.Critical, 10 );
						itoa( (int)work->GetWeekEnd(), SmuzlStruct.WeekEnd, 10 );
						ultoa( work->GetTmpL(), SmuzlStruct.StartKnot, 10 );
						ultoa( work->GetTmpR(), SmuzlStruct.FinishKnot, 10 );

						int j = 1;
						TSResourcesIterator ResourcesIterator( *work->GetResources() );
						while ( ResourcesIterator )
							{
							TMyResource* ResourceTmp = ResourcesIterator++;
							if ( j == 1 )
								{
					 //			fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                                dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource() + 1, &SpeciStruct);
								strncpy( SmuzlStruct.c1, SpeciStruct.b1, 11 );
								itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource1, 10 );
								ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin1, 10 );
								ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt1, 10 );
								ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax1, 10 );
								ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource1, 10 );
								ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume1, 10 );
								}
							if ( j == 2 )
								{
					 //			fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                                dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource() + 1, &SpeciStruct);
								strncpy( SmuzlStruct.c2, SpeciStruct.b1, 11 );
								itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource2, 10 );
								ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin2, 10 );
								ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt2, 10 );
								ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax2, 10 );
								ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource2, 10 );
								ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume2, 10 );
								}
							if ( j == 3 )
								{
					 //			fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                                dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource() + 1, &SpeciStruct);
								strncpy( SmuzlStruct.c3, SpeciStruct.b1, 11 );
								itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource3, 10 );
								ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin3, 10 );
								ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt3, 10 );
								ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax3, 10 );
								ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource3, 10 );
								ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume3, 10 );
								}
							if ( j == 4 )
								{
					  //  		fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                                dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource()+ 1, &SpeciStruct);
								strncpy( SmuzlStruct.c4, SpeciStruct.b1, 11 );
								itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource4, 10 );
								ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin4, 10 );
								ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt4, 10 );
								ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax4, 10 );
								ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource4, 10 );
								ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume4, 10 );
								}
							j++;
							}
					 //	myfile->writeDBF( i, (void *)indexSmuzlStruct );
                     // My Write !!!
                        dlgDBSPU->TransferSmuzlStruct(i, &SmuzlStruct);

						}
					}
				if ( !workTmp )
		     //    Del = 1; /// Lipa !!!   	//		myfile->DelRecord( i );
                   dlgDBSPU->DelRecord(i);
				}
			else
		 //	 Del = 1; // Lipa !!!     //	myfile->DelRecord( i );
             dlgDBSPU->DelRecord(i);
			}
		i++;
		}
	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* work = DWorksIterator++;
		if ( work->GetTmp() )                // Additional works !!!
			{
                                                    // My change !!!
			setmem( indexSmuzlStruct, sizeof( SmuzlStruct ), '\0' );
			strcpy( SmuzlStruct.Name, work->GetName() );
			ultoa( work->GetNumber(), SmuzlStruct.Number, 10 );
            //ultoa( 0, SmuzlStruct.b1, 10 ); // PRIZNAK My Add

			ultoa( work->GetStartTime(), SmuzlStruct.StartTime, 10 );
			ultoa( work->GetFinishTime(), SmuzlStruct.FinishTime, 10 );

			itoa( work->GetIndexShift(), SmuzlStruct.IndexShift, 10 );
			itoa( work->GetIndexBrigade(), SmuzlStruct.IndexBrigade, 10 );

   //		FileDBF *fileP = new FileDBF( "pack.dbf" );
			struct pack PackStruct, *indexPackStruct = &PackStruct;
            // My ADD  !!!
            setmem( indexPackStruct, sizeof( PackStruct ), ' ' );//my NEW !!!
  //		fileP->readDBF( work->GetIndexPack(), (void *)indexPackStruct );
            dlgDBSPU->ReadDBFPack(work->GetIndexPack() + 1, &PackStruct);
 			strncpy( SmuzlStruct.c5, PackStruct.b1, 11 );
        	itoa( work->GetIndexPack(), SmuzlStruct.IndexPack, 10 );
  //		delete fileP;

  //		FileDBF *fileB = new FileDBF( "brigade.dbf" );
  			struct brigade BrigadeStruct, *indexBrigadeStruct = &BrigadeStruct;
            // My ADD  !!!
            setmem( indexBrigadeStruct, sizeof( BrigadeStruct ), ' ' );//my NEW !!!

  //		fileB->readDBF( work->GetIndexBrigade(), (void *)indexBrigadeStruct );
            dlgDBSPU->ReadDBFBrigade(work->GetIndexBrigade()+ 1, &BrigadeStruct);
  			strncpy( SmuzlStruct.c0  , BrigadeStruct.b1, 11 );
  			itoa( work->GetIndexBrigade(), SmuzlStruct.IndexBrigade, 10 );
  	//		delete fileB;

			ultoa( work->GetDuration(), SmuzlStruct.Duration, 10 );
			itoa( work->GetPercent(), SmuzlStruct.Percent, 10 );
			ultoa( work->GetShiftStart(), SmuzlStruct.ShiftStart, 10 );
			ultoa( work->GetShiftFinish(), SmuzlStruct.ShiftFinish, 10 );
			itoa( work->GetCritical(), SmuzlStruct.Critical, 10 );
			itoa( (int)work->GetWeekEnd(), SmuzlStruct.WeekEnd, 10 );
			ultoa( work->GetTmpL(), SmuzlStruct.StartKnot, 10 );
			ultoa( work->GetTmpR(), SmuzlStruct.FinishKnot, 10 );

			int j = 1;
			TSResourcesIterator ResourcesIterator( *work->GetResources() );
			while ( ResourcesIterator )
				{
				TMyResource* ResourceTmp = ResourcesIterator++;
				if ( j == 1 )
					{
	 //				fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                    dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource()+ 1, &SpeciStruct);
					strncpy( SmuzlStruct.c1, SpeciStruct.b1, 11 );
					itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource1, 10 );
					ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin1, 10 );
					ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt1, 10 );
					ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax1, 10 );
					ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource1, 10 );
					ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume1, 10 );
					}
				if ( j == 2 )
					{
	 //				fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                    dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource()+ 1, &SpeciStruct);
					strncpy( SmuzlStruct.c2, SpeciStruct.b1, 11 );
					itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource2, 10 );
					ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin2, 10 );
					ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt2, 10 );
					ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax2, 10 );
					ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource2, 10 );
					ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume2, 10 );
					}
				if ( j == 3 )
					{
	  //   			fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                    dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource()+ 1, &SpeciStruct);
					strncpy( SmuzlStruct.c3, SpeciStruct.b1, 11 );
					itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource3, 10 );
					ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin3, 10 );
					ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt3, 10 );
					ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax3, 10 );
					ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource3, 10 );
					ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume3, 10 );
					}
				if ( j == 4 )
					{
	   //  			fileS->readDBF( ResourceTmp->GetIndexResource(), (void *)indexSpeciStruct );
                    dlgDBSPU->ReadDBFSpeci( ResourceTmp->GetIndexResource()+ 1, &SpeciStruct);
					strncpy( SmuzlStruct.c4, SpeciStruct.b1, 11 );
					itoa( ResourceTmp->GetIndexResource(), SmuzlStruct.IndexResource4, 10 );
					ultoa( ResourceTmp->GetResourceMin(), SmuzlStruct.ResourceMin4, 10 );
					ultoa( ResourceTmp->GetResourceOpt(), SmuzlStruct.ResourceOpt4, 10 );
					ultoa( ResourceTmp->GetResourceMax(), SmuzlStruct.ResourceMax4, 10 );
					ultoa( ResourceTmp->GetResource(), SmuzlStruct.Resource4, 10 );
					ultoa( ResourceTmp->GetVolume(), SmuzlStruct.Volume4, 10 );
					}
				j++;
				}

		 //	myfile->AppendRecord();
		 //	myfile->writeDBF( i++, (void *)indexSmuzlStruct );

             dlgDBSPU->AppendWork(0, &SmuzlStruct);
         //  Remember about i++ !!!

			}
		}

	while ( !Check->IsEmpty() )    //  Fiction  works (links) !!!
		{
		TConnect* ConnectTmp = Check->PeekHead();
		Check->DetachAtHead();
		setmem( (void*)indexSmuzlStruct, sizeof( SmuzlStruct ), '\0' );
		ultoa( 0UL, SmuzlStruct.Number, 10 );
		ultoa( ConnectTmp->GetWorkStart()->GetTmpR(), SmuzlStruct.StartKnot, 10 );
		ultoa( ConnectTmp->GetWorkFinish()->GetTmpL(), SmuzlStruct.FinishKnot, 10 );
        // My Addition !!!!
		ultoa( 0, SmuzlStruct.StartTime, 10 );
		ultoa( 0, SmuzlStruct.FinishTime, 10 );
		ultoa( 0, SmuzlStruct.ShiftStart, 10 );
		ultoa( 0, SmuzlStruct.ShiftFinish, 10 );
        ultoa( 0, SmuzlStruct.WeekEnd, 10 );

	 //	myfile->AppendRecord();
	 //	myfile->writeDBF( i++, (void *)indexSmuzlStruct );
        //
         dlgDBSPU->AppendWork(0, &SmuzlStruct);
        //  Remember about i++ !!!
		}
   //	while ( myfile->DelRecord( i++ ) );      I++ !!!!!!!!!
		;
 //	delete myfile;
 //	delete fileS;
   	::SetCursor(hcurSave);
	return TRUE;
	}

bool TMyLayout::ReadDB()  //  Read  DB -> MDL !!!
	{
	HCURSOR hcurSave=::SetCursor(LoadCursor(NULL,IDC_WAIT));
	char StringTmp[ 12 ];
	StringTmp[ 11 ] = '\0';

    // May be conflict ??? Put in  nuclear   of class  TMyLayout ????  !!!
    TPointer<TDBTESTForm> dlgDBSPU2 = new TDBTESTForm(::Application);
    ///////////////////////////////////////////////////////////////////

//	FileDBF *file = new FileDBF( "smuzi.dbf" );
	struct smuzi SmuziStruct, *indexSmuziStruct = &SmuziStruct;
//	file->readDBF( 0, (void *)indexSmuziStruct );
    dlgDBSPU2->ReturnSmuziStruct(0, &SmuziStruct);
	PackMain->SetName( SmuziStruct.Name );
	strncpy( StringTmp, SmuziStruct.StartTime, 11 );
	PackMain->SetStartTime( atol( StringTmp ) );
	strncpy( StringTmp, SmuziStruct.FinishTime, 11 );
	PackMain->SetFinishTime( atol( StringTmp ) );
 //	delete file;

//	file = new FileDBF( "smuzl.dbf" );
	struct smuzl SmuzlStruct, *indexSmuzlStruct = &SmuzlStruct;
	long i = 1;  // WAS i= 0;
	unsigned long Number;


	TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
//	while ( file->readDBF( i, (void *)indexSmuzlStruct ) )
    while ( dlgDBSPU2->ReadDBFSmuzl(i, &SmuzlStruct))
//		if ( !file->IsDelRecord( i++ ) )
//      if  ( i++ >= 0 ) // My  skip !!!
        if  ( i++ >= 1 ) // My  skip !!!   LIPA !!!
			{
			Number = atol( SmuzlStruct.Number );
			DWorksIterator.Restart();
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;
				if ( Number == work->GetNumber() )
					{
         //Error ???    setmem( indexSmuzlStruct, sizeof( SmuzlStruct ), ' ' );//my NEW !!!
					strncpy( StringTmp, SmuzlStruct.StartTime, 11 );
					work->SetStartTime( atol( StringTmp ) );
					strncpy( StringTmp, SmuzlStruct.FinishTime, 11 );
					work->SetFinishTime( atol( StringTmp ) );

             // WILL BE Support in ReadDBFSmuzl !!!!!
					strncpy( StringTmp, SmuzlStruct.IndexShift, 11 );
    				work->SetIndexShift( atoi( StringTmp ) );
//					strncpy( StringTmp, SmuzlStruct.IndexBrigade, 11 );
//					work->SetIndexBrigade( atoi( StringTmp ) );
//					strncpy( StringTmp, SmuzlStruct.IndexPack, 11 );
//					work->SetIndexPack( atoi( StringTmp ) );

					strncpy( StringTmp, SmuzlStruct.Duration, 11 );
					work->SetDuration( atol( StringTmp ) );
					strncpy( StringTmp, SmuzlStruct.Percent, 11 );
					work->SetPercent( atoi( StringTmp ) );
			  /////////////////////////////////////////////
					strncpy( StringTmp, SmuzlStruct.ShiftStart, 11 );
					work->SetShiftStart( atol( StringTmp ) );
				////////////////////????///////////////////
					strncpy( StringTmp, SmuzlStruct.ShiftFinish, 11 );
					work->SetShiftFinish( atol( StringTmp ) );
			  //////////////////////////////////////////////
					strncpy( StringTmp, SmuzlStruct.Critical, 11 );
					work->SetCritical( atoi( StringTmp ) );
					strncpy( StringTmp, SmuzlStruct.WeekEnd, 11 );
					work->SetWeekEnd( atoi( StringTmp ) );

					int j = 1;
					TSResourcesIterator ResourcesIterator( *work->GetResources() );
					while ( ResourcesIterator )
						{
						TMyResource* ResourceTmp = ResourcesIterator++;
						if ( j == 1 )
							{
							strncpy( StringTmp, SmuzlStruct.Resource1, 11 );
							ResourceTmp->SetResource( atol( StringTmp ) );
							}
						if ( j == 2 )
							{
							strncpy( StringTmp, SmuzlStruct.Resource2, 11 );
							ResourceTmp->SetResource( atol( StringTmp ) );
							}
						if ( j == 3 )
							{
							strncpy( StringTmp, SmuzlStruct.Resource3, 11 );
							ResourceTmp->SetResource( atol( StringTmp ) );
							}
						if ( j == 4 )
							{
							strncpy( StringTmp, SmuzlStruct.Resource4, 11 );
							ResourceTmp->SetResource( atol( StringTmp ) );
							}
						j++;

						}
					work->ArrangeStartTime(); // Коррекция Finish
					}
				}
			}
//	delete file;
/////////////////// Recalculate  and show GST  /////////////////////
	if ( AllWindow[2].BASScreen )
		{
		PackMain->ResetBitTimes();
		PackMain->BuildBitTimes();
		}

/////////////////////////////////////////////////////////////////////
	Invalidate();
	::SetCursor(hcurSave);
	return TRUE;
	}

void TMyLayout::CmUpdateDBF()
	{
	}

bool TMyLayout::UpdateDBF( TConnects* Check )
	{
	}


