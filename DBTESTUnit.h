//---------------------------------------------------------------------------
#ifndef DBTESTUnitH
#define DBTESTUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>


//---------------------------------------------------------------------------
class TDBTESTForm : public TForm
{
__published:	// IDE-managed Components
    TDBGrid *DBGrid1;
    TDBGrid *DBGrid2;
    TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TDBGrid *DBGrid3;
    TDBGrid *DBGrid4;
    TDBNavigator *DBNavigator1;
    TDBGrid *DBGrid5;
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
    TDataSource *DataSource6;
    TQuery *SmuzlDeleteQuery;
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
    __fastcall TDBTESTForm(TComponent* Owner);

      int TransferSmuziStruct(long number, struct smuzi *SmuziStruct);
      int ReturnSmuziStruct(long number, struct smuzi *SmuziStruct);
      int ReadDBF(long number, struct smuzl *SmuzlStruct);
      int TransferSmuzlStruct(long number, struct smuzl *SmuzlStruct);
      int AppendWork(long number, struct smuzl *SmuzlStruct);
      int DelRecord(long number);
      int DelAllNullNumberRecord(void);
      int ReadDBFSmuzl(long number, struct smuzl *SmuzlStruct);
      int ReadDBFSpeci(long number, struct speci *SpeciStruct);
      int ReadDBFPack(long number, struct pack *PackStruct);
      int ReadDBFBrigade(long number, struct brigade *BrigadeStruct);

};
//---------------------------------------------------------------------------
extern PACKAGE TDBTESTForm *DBTESTForm;
//---------------------------------------------------------------------------
#endif
