//---------------------------------------------------------------------------
#ifndef WorkDialogUnitH
#define WorkDialogUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
/*
#include "MyCheckBox.h"
#include "MyGroupBox.h"
#include "MyListBox.h"
*/
/*
#include "MyCheckBox.h"
#include "MyGroupBox.h"
#include "MyListBox.h"
*/
///////////////////////////////////////////////
#include "SPUDMUnit.h"
#include "MyCheckBox.h"
#include "MyGroupBox.h"
#include "MyListBox.h"

#define  STRICT

using namespace Forms;

//---------------------------------------------------------------------------
struct STR_WORK_DIALOG
	{
	char Name[ 255 ];
	char IndexBrigade[ 11 ];
	char IndexPack[ 11 ];
	char IndexShift[ 11 ];
	char Percent[ 11 ];
	char Duration_hh[ 11 ];
	char Duration_mm[ 11 ];
	char Duration_ss[ 11 ];
	char Volume[ 11 ];
	char ResourceMin[ 11 ];
	char ResourceOpt[ 11 ];
	char ResourceMax[ 11 ];
	char Resource[ 11 ];
	char ShiftStart_hh[ 11 ];
	char ShiftStart_mm[ 11 ];
	char ShiftStart_ss[ 11 ];
	char ShiftFinish_hh[ 11 ];
	char ShiftFinish_mm[ 11 ];
	char ShiftFinish_ss[ 11 ];
	char Start_DD[ 11 ];
	char Start_MM[ 11 ];
	char Start_YY[ 11 ];
	char Start_hh[ 11 ];
	char Start_mm[ 11 ];
	char Start_ss[ 11 ];
	char Finish_DD[ 11 ];
	char Finish_MM[ 11 ];
	char Finish_YY[ 11 ];
	char Finish_hh[ 11 ];
	char Finish_mm[ 11 ];
	char Finish_ss[ 11 ];
	};

class TWorkDialogForm : public TForm
{
__published:	// IDE-managed Components
    TBitBtn *BitBtnOK;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;


    TMaskEdit *MaskEdit1;
    TMaskEdit *MaskEdit4;
    TBitBtn *BitBtnCancel;
    TMyListBox *ListBoxPack;
    TMyListBox *ListBoxBrigade;
    TMyListBox *ListBoxResourceAll;
    TMyListBox *ListBoxResourceWork;
    TMyCheckBox *WeekEndCheckBox;

    TBitBtn *AddResourceBitBtn;
    TBitBtn *RemoveResourceBitBtn;
    TStaticText *StaticText1;
    TStaticText *StaticText2;
    TStaticText *StaticText3;
    TStaticText *StaticText4;
    TStaticText *StaticText5;
    TStaticText *StaticText6;
    TStaticText *StaticText7;
    TStaticText *StaticText12;
    TStaticText *StaticText13;
    TStaticText *StaticText18;
    TStaticText *StaticText19;
    TStaticText *StaticText20;
    TStaticText *StaticText21;
    TStaticText *StaticText22;
    TMaskEdit *MaskEdit5;
    TMaskEdit *MaskEdit6;
    TMaskEdit *MaskEdit7;
    TMaskEdit *MaskEdit8;
    TMaskEdit *MaskEdit9;
    TBitBtn *BitBtn1;
    TStaticText *StaticText23;
    TLabel *Label7;
    TLabel *Label8;
    TMyGroupBox *MyGroupBox1;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label13;
    TLabel *Label14;
    TMyGroupBox *MyGroupBox2;
    TLabel *Label15;
    TLabel *Label16;
    TStaticText *StaticText8;
    TStaticText *StaticText9;
    TStaticText *StaticText10;
    TStaticText *StaticText11;
    TStaticText *StaticText14;
    TStaticText *StaticText15;
    TStaticText *StaticText16;
    TStaticText *StaticText17;
    TStaticText *StaticText24;
    TStaticText *StaticText25;
    TStaticText *StaticText26;
    TStaticText *StaticText27;
    TMaskEdit *MaskEdit10;
    TMaskEdit *MaskEdit11;
    TMaskEdit *MaskEdit12;

    TDataSource *DataSource1;
    TDataSource *DataSource2;
    TDataSource *DataSource3;
    TDataSource *DataSource4;
    TDataSource *DataSource5;
    TTable *SmuziTable;
    TTable *SmuzlTable;
    TTable *PackTable;
    TTable *BrigadeTable;
    TTable *SpeciTable;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall AddResourceBitBtnClick(TObject *Sender);
    void __fastcall RemoveResourceBitBtnClick(TObject *Sender);


    void __fastcall ListBoxResourceWorkClick(TObject *Sender);
    void __fastcall ListBoxResourceWorkExit(TObject *Sender);
    void __fastcall ListBoxResourceWorkEnter(TObject *Sender);
    void __fastcall BitBtnOKClick(TObject *Sender);
private:	// User declarations

    //////////////////////////////////////////////
    TWork* Work;
   	TMyResource* Resource;
	TSResources* Resources;
    STR_WORK_DIALOG StrWorkDialog;


public:		// User declarations
    __fastcall TWorkDialogForm(TComponent* Owner);

    void TransferWork(TWork *Work);
    TWork* ReturnWork(TWork *Work);
	void IdAddResource();
	void IdRemoveResource();
	void ListBoxResourceWorkChange();
};
//---------------------------------------------------------------------------
extern PACKAGE TWorkDialogForm *WorkDialogForm;
//---------------------------------------------------------------------------
#endif
