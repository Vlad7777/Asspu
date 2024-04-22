//---------------------------------------------------------------------------
#ifndef SPU_DMUnitH
#define SPU_DMUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TSPUDM : public TDataModule
{
__published:	// IDE-managed Components
    TDatabase *Database1;
    TDataSource *DataSource1;
    TTable *Table1;
    TTable *Table2;
    TTable *Table3;
    TDataSource *DataSource2;
    TDataSource *DataSource3;
private:	// User declarations
public:		// User declarations
    __fastcall TSPUDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSPUDM *SPUDM;
//---------------------------------------------------------------------------
#endif
