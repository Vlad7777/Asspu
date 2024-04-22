//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DBTESTUnit.h"

#include "smuzi.str"
#include "smuzl.str"
#include "speci.str"
#include "pack.str"
#include "brigade.str"

//#include "SPUDMUnit.h"  // SPUDM  does not  work ??????
                          //  MB  from OWL ???
////////////////////////////
#include <stdio.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDBTESTForm *DBTESTForm;
//---------------------------------------------------------------------------
__fastcall TDBTESTForm::TDBTESTForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDBTESTForm::BitBtn2Click(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TDBTESTForm::BitBtn1Click(TObject *Sender)
{
//  OK !!!

}
//---------------------------------------------------------------------------
int TDBTESTForm::TransferSmuziStruct(long number, struct smuzi *SmuziStruct)
{
  //  DatabaseASSPU->Open();  // !!! then  CLOSE !!!
  //  To CreateForm !!! SmuziTable->Open();  SmuziTable->Edit(); //
    SmuziTable->FieldByName("NamePack")->AsString = SmuziStruct->Name;
    SmuziTable->FieldByName("StartTime")->AsString  =  SmuziStruct->StartTime;
    SmuziTable->FieldByName("FinishTime")->AsString  = SmuziStruct->FinishTime;
    SmuziTable->Post();
  //SmuziTable->Close(); // !!!
  //Database1->Close();
    return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReturnSmuziStruct(long number, struct smuzi *SmuziStruct)
{

    sprintf (SmuziStruct->Name, "%s", SmuziTable->FieldByName("NamePack")->AsString );
    sprintf (SmuziStruct->StartTime, "%s", SmuziTable->FieldByName("StartTime")->AsString );
    sprintf (SmuziStruct->FinishTime, "%s", SmuziTable->FieldByName("FinishTime")->AsString );
    return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReadDBF(long number, struct smuzl *SmuzlStruct)
{
// WAS if (number >= SmuzlTable->RecordCount)
  if (number > SmuzlTable->RecordCount)
  return(0);

  SmuzlTable->Edit();

  //////////////////////////// Locate  number !!!!
      // i=1
       // SMUZLSTRUCT --->  Litter !!!!
      bool LocateSuccess;
      TLocateOptions SearchOptions;
      SearchOptions << loPartialKey;
      LocateSuccess = SmuzlTable->Locate("NUMBR", number, SearchOptions);      if   (LocateSuccess) {      sprintf (SmuzlStruct->Number, "%s", SmuzlTable->FieldByName("NUMBR")->AsString );      // SMUZLSTRUCT --->  Litter !!!!      }      else      {      // ERROR !!!      ShowMessage("Number  in LocateSuccess !!!");      return(0);      }  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReadDBFSmuzl(long number, struct smuzl *SmuzlStruct)
{
  if (number > SmuzlTable->RecordCount)
  return(0);

      bool LocateSuccess;
      TLocateOptions SearchOptions;
      SearchOptions << loPartialKey;
      LocateSuccess = SmuzlTable->Locate("NUMBR", number, SearchOptions);      if   (LocateSuccess) {      sprintf (SmuzlStruct->Number, "%s", SmuzlTable->FieldByName("NUMBR")->AsString );      sprintf (SmuzlStruct->Duration, "%s", SmuzlTable->FieldByName("PRODOJ")->AsString );      sprintf (SmuzlStruct->Critical, "%s", SmuzlTable->FieldByName("CRITICAL")->AsString );      sprintf (SmuzlStruct->Percent, "%s", SmuzlTable->FieldByName("PERCENT")->AsString );      sprintf (SmuzlStruct->StartTime, "%s", SmuzlTable->FieldByName("STARTTIME")->AsString );      sprintf (SmuzlStruct->FinishTime, "%s", SmuzlTable->FieldByName("FINISHTIME")->AsString );      sprintf (SmuzlStruct->ShiftStart, "%s", SmuzlTable->FieldByName("SHIFTSTART")->AsString );      sprintf (SmuzlStruct->ShiftFinish, "%s", SmuzlTable->FieldByName("SHIFTFIN")->AsString );      sprintf (SmuzlStruct->WeekEnd, "%s", SmuzlTable->FieldByName("WEEK")->AsString );      sprintf (SmuzlStruct->Resource1, "%s", SmuzlTable->FieldByName("SELECT1")->AsString );      sprintf (SmuzlStruct->Resource2, "%s", SmuzlTable->FieldByName("SELECT2")->AsString );      sprintf (SmuzlStruct->Resource3, "%s", SmuzlTable->FieldByName("SELECT3")->AsString );      sprintf (SmuzlStruct->Resource4, "%s", SmuzlTable->FieldByName("SELECT4")->AsString );      //return(1); //   If not  fictional works  in NetWork m.b. Error !!!      }      else
      {      ShowMessage("NUMBR (ReadDBFSmuzl) in LocateSuccess !!!");      return(0);      }  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::TransferSmuzlStruct(long number, struct smuzl *SmuzlStruct)
{
  if (number > SmuzlTable->RecordCount)
  return(0);

        bool LocateSuccess;
      TLocateOptions SearchOptions;
      SearchOptions << loPartialKey;
      LocateSuccess = SmuzlTable->Locate("NUMBR", number, SearchOptions);      if   (LocateSuccess) {///////////////////////////// EDIT or UPDATE !!!!///////////////////////////////////////
  SmuzlTable->Edit();
  SmuzlTable->FieldByName("NAIMRAB")->AsString =   SmuzlStruct->Name;
  SmuzlTable->FieldByName("NUMBR")->AsString =   SmuzlStruct->Number;
// Later  SmuzlTable->FieldByName("PRIZ")->AsString =   SmuzlStruct->b1;
  SmuzlTable->FieldByName("KODRAB_PI")->AsString =   SmuzlStruct->StartKnot;
  SmuzlTable->FieldByName("KODRAB_PJ")->AsString =   SmuzlStruct->FinishKnot;
 // SmuzlTable->FieldByName("IZMENOB")->AsString =   SmuzlStruct->b2;
  SmuzlTable->FieldByName("PRODOJ")->AsString =   SmuzlStruct->Duration;
  SmuzlTable->FieldByName("KODORG")->AsString =   SmuzlStruct->c0;
  SmuzlTable->FieldByName("INDEXBRIG")->AsString =   SmuzlStruct->IndexBrigade;
  SmuzlTable->FieldByName("TREBSM")->AsString =   SmuzlStruct->IndexShift;

  SmuzlTable->FieldByName("KODSPEC1")->AsString =   SmuzlStruct->c1;
  SmuzlTable->FieldByName("INDEX1")->AsString =   SmuzlStruct->IndexResource1;
  SmuzlTable->FieldByName("MIN1")->AsString =   SmuzlStruct->ResourceMin1;
  SmuzlTable->FieldByName("OPT1")->AsString =   SmuzlStruct->ResourceOpt1;
  SmuzlTable->FieldByName("MAX1")->AsString =   SmuzlStruct->ResourceMax1;
  SmuzlTable->FieldByName("SELECT1")->AsString =   SmuzlStruct->Resource1;
  SmuzlTable->FieldByName("VOLUME1")->AsString =   SmuzlStruct->Volume1;

  SmuzlTable->FieldByName("KODSPEC2")->AsString =   SmuzlStruct->c2;
  SmuzlTable->FieldByName("INDEX2")->AsString =   SmuzlStruct->IndexResource2;
  SmuzlTable->FieldByName("MIN2")->AsString =   SmuzlStruct->ResourceMin2;
  SmuzlTable->FieldByName("OPT2")->AsString =   SmuzlStruct->ResourceOpt2;
  SmuzlTable->FieldByName("MAX2")->AsString =   SmuzlStruct->ResourceMax2;
  SmuzlTable->FieldByName("SELECT2")->AsString =   SmuzlStruct->Resource2;
  SmuzlTable->FieldByName("VOLUME2")->AsString =   SmuzlStruct->Volume2;

  SmuzlTable->FieldByName("KODSPEC3")->AsString =   SmuzlStruct->c3;
  SmuzlTable->FieldByName("INDEX3")->AsString =   SmuzlStruct->IndexResource3;
  SmuzlTable->FieldByName("MIN3")->AsString =   SmuzlStruct->ResourceMin3;
  SmuzlTable->FieldByName("OPT3")->AsString =   SmuzlStruct->ResourceOpt3;
  SmuzlTable->FieldByName("MAX3")->AsString =   SmuzlStruct->ResourceMax3;
  SmuzlTable->FieldByName("SELECT3")->AsString =   SmuzlStruct->Resource3;
  SmuzlTable->FieldByName("VOLUME3")->AsString =   SmuzlStruct->Volume3;

  SmuzlTable->FieldByName("KODSPEC4")->AsString =   SmuzlStruct->c4;
  SmuzlTable->FieldByName("INDEX4")->AsString =   SmuzlStruct->IndexResource4;
  SmuzlTable->FieldByName("MIN4")->AsString =   SmuzlStruct->ResourceMin4;
  SmuzlTable->FieldByName("OPT4")->AsString =   SmuzlStruct->ResourceOpt4;
  SmuzlTable->FieldByName("MAX4")->AsString =   SmuzlStruct->ResourceMax4;
  SmuzlTable->FieldByName("SELECT4")->AsString =   SmuzlStruct->Resource4;
  SmuzlTable->FieldByName("VOLUME4")->AsString =   SmuzlStruct->Volume4;

//  SmuzlTable->FieldByName("KODOB")->AsString =   SmuzlStruct->b3;
  SmuzlTable->FieldByName("KODPACK")->AsString =   SmuzlStruct->c5;
  SmuzlTable->FieldByName("INDEXPACK")->AsString =   SmuzlStruct->IndexPack;
/*SmuzlTable->FieldByName("KODRAB")->AsString =   SmuzlStruct->b4;
  SmuzlTable->FieldByName("TZM1")->AsString =   SmuzlStruct->b5;
  SmuzlTable->FieldByName("GPM")->AsString =   SmuzlStruct->b6;
  SmuzlTable->FieldByName("NOMPOM")->AsString =   SmuzlStruct->b7;
  SmuzlTable->FieldByName("DOZAPOM")->AsString =   SmuzlStruct->b8;
  SmuzlTable->FieldByName("DOZAR")->AsString =   SmuzlStruct->b9;
  SmuzlTable->FieldByName("USLTR")->AsString =   SmuzlStruct->b10;
  SmuzlTable->FieldByName("KODO")->AsString =   SmuzlStruct->b11;
  SmuzlTable->FieldByName("GRUPO")->AsString =   SmuzlStruct->b12;
  SmuzlTable->FieldByName("NAMEDI")->AsString =   SmuzlStruct->b13;
  SmuzlTable->FieldByName("KREM")->AsString =   SmuzlStruct->b14;
 */
  SmuzlTable->FieldByName("CRITICAL")->AsString =   SmuzlStruct->Critical;
  SmuzlTable->FieldByName("PERCENT")->AsString =   SmuzlStruct->Percent;
  SmuzlTable->FieldByName("STARTTIME")->AsString  = SmuzlStruct->StartTime;
  SmuzlTable->FieldByName("FINISHTIME")->AsString = SmuzlStruct->FinishTime;
  SmuzlTable->FieldByName("SHIFTSTART")->AsString =   SmuzlStruct->ShiftStart;
  SmuzlTable->FieldByName("SHIFTFIN")->AsString =   SmuzlStruct->ShiftFinish;
  SmuzlTable->FieldByName("WEEK")->AsString =   SmuzlStruct->WeekEnd;

  SmuzlTable->Post();

  } //  By Locate !!!

      else
      {      // ERROR !!!      ShowMessage(" Number (TransferSmuzlStruct)  in LocateSuccess !!!");      return(0);      }   return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::AppendWork(long number, struct smuzl *SmuzlStruct)
{
   /////////////////////////// APPEND !!!!/////////////////////////////
  SmuzlTable->Append();
  SmuzlTable->FieldByName("NAIMRAB")->AsString =   SmuzlStruct->Name;
  SmuzlTable->FieldByName("NUMBR")->AsString =   SmuzlStruct->Number;
//Later !!! SmuzlTable->FieldByName("PRIZ")->AsString =   SmuzlStruct->b1;
  SmuzlTable->FieldByName("KODRAB_PI")->AsString =   SmuzlStruct->StartKnot;
  SmuzlTable->FieldByName("KODRAB_PJ")->AsString =   SmuzlStruct->FinishKnot;
 // SmuzlTable->FieldByName("IZMENOB")->AsString =   SmuzlStruct->b2;
  SmuzlTable->FieldByName("PRODOJ")->AsString =   SmuzlStruct->Duration;
  SmuzlTable->FieldByName("KODORG")->AsString =   SmuzlStruct->c0;
  SmuzlTable->FieldByName("INDEXBRIG")->AsString =   SmuzlStruct->IndexBrigade;
  SmuzlTable->FieldByName("TREBSM")->AsString =   SmuzlStruct->IndexShift;

  SmuzlTable->FieldByName("KODSPEC1")->AsString =   SmuzlStruct->c1;
  SmuzlTable->FieldByName("INDEX1")->AsString =   SmuzlStruct->IndexResource1;
  SmuzlTable->FieldByName("MIN1")->AsString =   SmuzlStruct->ResourceMin1;
  SmuzlTable->FieldByName("OPT1")->AsString =   SmuzlStruct->ResourceOpt1;
  SmuzlTable->FieldByName("MAX1")->AsString =   SmuzlStruct->ResourceMax1;
  SmuzlTable->FieldByName("SELECT1")->AsString =   SmuzlStruct->Resource1;
  SmuzlTable->FieldByName("VOLUME1")->AsString =   SmuzlStruct->Volume1;

  SmuzlTable->FieldByName("KODSPEC2")->AsString =   SmuzlStruct->c2;
  SmuzlTable->FieldByName("INDEX2")->AsString =   SmuzlStruct->IndexResource2;
  SmuzlTable->FieldByName("MIN2")->AsString =   SmuzlStruct->ResourceMin2;
  SmuzlTable->FieldByName("OPT2")->AsString =   SmuzlStruct->ResourceOpt2;
  SmuzlTable->FieldByName("MAX2")->AsString =   SmuzlStruct->ResourceMax2;
  SmuzlTable->FieldByName("SELECT2")->AsString =   SmuzlStruct->Resource2;
  SmuzlTable->FieldByName("VOLUME2")->AsString =   SmuzlStruct->Volume2;

  SmuzlTable->FieldByName("KODSPEC3")->AsString =   SmuzlStruct->c3;
  SmuzlTable->FieldByName("INDEX3")->AsString =   SmuzlStruct->IndexResource3;
  SmuzlTable->FieldByName("MIN3")->AsString =   SmuzlStruct->ResourceMin3;
  SmuzlTable->FieldByName("OPT3")->AsString =   SmuzlStruct->ResourceOpt3;
  SmuzlTable->FieldByName("MAX3")->AsString =   SmuzlStruct->ResourceMax3;
  SmuzlTable->FieldByName("SELECT3")->AsString =   SmuzlStruct->Resource3;
  SmuzlTable->FieldByName("VOLUME3")->AsString =   SmuzlStruct->Volume3;

  SmuzlTable->FieldByName("KODSPEC4")->AsString =   SmuzlStruct->c4;
  SmuzlTable->FieldByName("INDEX4")->AsString =   SmuzlStruct->IndexResource4;
  SmuzlTable->FieldByName("MIN4")->AsString =   SmuzlStruct->ResourceMin4;
  SmuzlTable->FieldByName("OPT4")->AsString =   SmuzlStruct->ResourceOpt4;
  SmuzlTable->FieldByName("MAX4")->AsString =   SmuzlStruct->ResourceMax4;
  SmuzlTable->FieldByName("SELECT4")->AsString =   SmuzlStruct->Resource4;
  SmuzlTable->FieldByName("VOLUME4")->AsString =   SmuzlStruct->Volume4;

//  SmuzlTable->FieldByName("KODOB")->AsString =   SmuzlStruct->b3;
  SmuzlTable->FieldByName("KODPACK")->AsString =   SmuzlStruct->c5;
  SmuzlTable->FieldByName("INDEXPACK")->AsString =   SmuzlStruct->IndexPack;
/*SmuzlTable->FieldByName("KODRAB")->AsString =   SmuzlStruct->b4;
  SmuzlTable->FieldByName("TZM1")->AsString =   SmuzlStruct->b5;
  SmuzlTable->FieldByName("GPM")->AsString =   SmuzlStruct->b6;
  SmuzlTable->FieldByName("NOMPOM")->AsString =   SmuzlStruct->b7;
  SmuzlTable->FieldByName("DOZAPOM")->AsString =   SmuzlStruct->b8;
  SmuzlTable->FieldByName("DOZAR")->AsString =   SmuzlStruct->b9;
  SmuzlTable->FieldByName("USLTR")->AsString =   SmuzlStruct->b10;
  SmuzlTable->FieldByName("KODO")->AsString =   SmuzlStruct->b11;
  SmuzlTable->FieldByName("GRUPO")->AsString =   SmuzlStruct->b12;
  SmuzlTable->FieldByName("NAMEDI")->AsString =   SmuzlStruct->b13;
  SmuzlTable->FieldByName("KREM")->AsString =   SmuzlStruct->b14;
 */
  SmuzlTable->FieldByName("CRITICAL")->AsString =   SmuzlStruct->Critical;
  SmuzlTable->FieldByName("PERCENT")->AsString =   SmuzlStruct->Percent;
  SmuzlTable->FieldByName("STARTTIME")->AsString  = SmuzlStruct->StartTime;
  SmuzlTable->FieldByName("FINISHTIME")->AsString = SmuzlStruct->FinishTime;
  SmuzlTable->FieldByName("SHIFTSTART")->AsString =   SmuzlStruct->ShiftStart;
  SmuzlTable->FieldByName("SHIFTFIN")->AsString =   SmuzlStruct->ShiftFinish;
  SmuzlTable->FieldByName("WEEK")->AsString =   SmuzlStruct->WeekEnd;
  SmuzlTable->Post();
  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::DelRecord(long number)
{
  if (number > SmuzlTable->RecordCount)
  return(0);

  SmuzlTable->Edit();
  SmuzlTable->MoveBy(number - 1);
  SmuzlTable->Delete();
  return(1);
}
//---------------------------------------------------------------------------

int TDBTESTForm::DelAllNullNumberRecord(void) // Delete ALL !!!!
{
      SmuzlDeleteQuery->Close();
      SmuzlDeleteQuery->SQL->Clear();
      // open the Query.
       SmuzlDeleteQuery->SQL->Add("delete from  smuzl");
      // execute the new SQL statement.
      if (!SmuzlDeleteQuery->Prepared)
      SmuzlDeleteQuery->Prepare();
      try
      {
         SmuzlDeleteQuery->ExecSQL();
      }
      catch(EDBEngineError* dbError)
      {
         for (int i = 0; i < dbError->ErrorCount; i++)
         {
            MessageBox(0, dbError[i].Message.c_str(), "SQL Error", MB_OK);
         }
      }

      SmuzlDeleteQuery->Close();

/*
  SmuzlTable->Edit();
  while (!SmuzlTable->Eof)// Not All Null-records removed !!!???
//  for (int k = 0; k< SmuzlTable->RecordCount; k++)
  {
  //if (SmuzlTable->FieldByName("NUMBR")->AsString == '0')
  SmuzlTable->Delete();
  SmuzlTable->Next();
  }
*/
  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReadDBFSpeci(long number, struct speci *SpeciStruct)
{
  SpeciTable->Open();
  if (number >= SpeciTable->RecordCount)
  {
  SpeciTable->Close();
  return(0);
  }
 // SpeciTable->Edit();
  SpeciTable->MoveBy(number - 1);
  sprintf (SpeciStruct->b1, "%s", SpeciTable->FieldByName("NOMERSPEC")->AsString );
  SpeciTable->Close();
  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReadDBFPack(long number, struct pack *PackStruct)
{
  PackTable->Open();
  if (number >= PackTable->RecordCount)
  {
  PackTable->Close();
  return(0);
  }
//  PackTable->Edit();
  PackTable->MoveBy(number - 1);
  sprintf (PackStruct->b1, "%s", PackTable->FieldByName("KODUZ")->AsString );
  PackTable->Close();
  return(1);
}
//---------------------------------------------------------------------------
int TDBTESTForm::ReadDBFBrigade(long number, struct brigade *BrigadeStruct)
{
  BrigadeTable->Open();
  if (number >= BrigadeTable->RecordCount)
  {
  BrigadeTable->Close();
  return(0);
  }
//  BrigadeTable->Edit();
  BrigadeTable->MoveBy(number - 1);
  sprintf (BrigadeStruct->b1, "%s", BrigadeTable->FieldByName("KODISP")->AsString );
  BrigadeTable->Close();
  return(1);
}
//---------------------------------------------------------------------------
void __fastcall TDBTESTForm::FormCreate(TObject *Sender)
{
   // DatabaseASSPU->Open();
    SmuziTable->Open();
    SmuziTable->Edit(); //
    SmuzlTable->Open();
    SmuzlTable->Edit(); //
}
//---------------------------------------------------------------------------

void __fastcall TDBTESTForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
//
  SmuziTable->Close();
  SmuzlTable->Close();
}
//---------------------------------------------------------------------------

