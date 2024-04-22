//---------------------------------------------------------------------------
#ifndef SPUDMUnitH
#define SPUDMUnitH
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
  //  TDatabase *DatabaseASSPU;
    TTable *SmuziTable;
    TDataSource *DataSourceSmuzi;
    TDataSource *DataSourceSmuzl;
    TTable *SmuzlTable;
    TTable *PackTable;
    TDataSource *DataSourcePack;
    TDataSource *DataSourceBrigade;
    TDataSource *DataSourceSpeci;
    TTable *BrigadeTable;
    TTable *SpeciTable;
private:	// User declarations
public:		// User declarations
    __fastcall TSPUDM(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSPUDM *SPUDM;
//---------------------------------------------------------------------------
#endif
