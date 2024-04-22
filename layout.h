#ifndef __LAYOUT_H
#define __LAYOUT_H

#include <owl\button.h>

#include "pack.h"
#include "bas_wind.h"

struct CHARACTER_WINDOW
	{
	TBASWindow* BASWindow;
	bool BASScreen;
	bool MaxScreen;
	};

class TMyLayout: public TLayoutWindow
	{
	protected:
		STR_PACK_EDIT* StrPackEdit;
		TPack* PackMain;
		bool IsDirty;
		bool DrawNetButton;
		char far* FileName;
		CHARACTER_WINDOW AllWindow[ 4 ];
		OWL::TButton* Button;
		int Type;
		int IndexResource;
		unsigned long MaxRang;
		unsigned long MaxLevel;

	public:
		TMyLayout( TPack* ThePack, const char far* fileName = NULL );
		~TMyLayout();
		void SetupWindow();

		bool CanClose();    ///////////////////// 13.09.96

		bool Save();
		bool SaveAs();
		bool Write( const char far* fileName = NULL );
		bool WriteDB( TConnects* Check = NULL );
		bool ReadDB();
		bool UpdateDBF( TConnects* Check = NULL );
		bool Print();
		void SetFileName( const char far* fileName );
		void SetWindowText();
		void ReLayout();
		void ClosePack();

		void BuildFreeTime();
		void BuildResource();
		void BuildPlan();
		void Rang();
		void Level();
		void Arrange();
		void Renumber();
		void Convert();
		void Optimization();
		void SpuFox();
		void Find();
		void SelectType( int TheType )
			{ Type = TheType; }
		void SetNet( UINT );
		void EditPack();

		void CmFileSave() { Save(); }      				// CM_FILESAVE
		void CmFileSaveAs() { SaveAs(); }  				// CM_FILESAVEAS
		void CmPrint() { Print(); }  						// CM_PRINT
		void CmClosePack() { ClosePack(); }          // CM_CLOSEPACK
		void CmActivateMDL(); 								// CM_ACTIVATEMDL
		void CmActivateGNT(); 								// CM_ACTIVATEGNT
		void CmActivateGST(); 								// CM_ACTIVATEGST
		void CmArrange() { Arrange(); }      			// CM_ARRANGE
		void CmConvert() { Convert(); }      			// CM_CONVERT
		void CmOptimization() { Optimization(); }    // CM_OPTIMIZATION
		void CmRenumber() { Renumber(); }      		// CM_ARRANGENUMBER
		void CmBuildResource() { BuildResource(); }  // CM_BUILDRESOURCE
		void CmBuildPlan() { BuildPlan(); }      		// CM_BUILDPLAN
		void CmUpdateModel() { ReadDB(); }  	//
		void CmSpuFox() { SpuFox(); }                // CM_SPUFOX
		void CmFind() { Find(); }
		void CmUpdateDBF();                          // CM_FIND
		void CmUnPack();										// CM_UNPACK
		void CmInPack();										// CM_INPACK
		void CmSelectWork() { SelectType( 0 ); }     // CM_SELECTWORK
		void CmSelectPack() { SelectType( 1 ); }     // CM_SELECTPACK
		void CmIndexResource();                  		//	CM_INDEXRESOURCE
		void CmEditPack() { EditPack(); }      		   //	CM_EDITPACK
		void CeFileSave( TCommandEnabler& );
		void CeFileSaveAs( TCommandEnabler& );
		void CeClosePack( TCommandEnabler& );
		void CeActivateMDL( TCommandEnabler& );
		void CeActivateGNT( TCommandEnabler& );
		void CeActivateGST( TCommandEnabler& );
		void CeConvert( TCommandEnabler& );
		void CeOptimization( TCommandEnabler& );
		void CeUpdateModel( TCommandEnabler& );
		void CeSpuFox ( TCommandEnabler& );
		void CeFind ( TCommandEnabler& );
		void CeBuildResource( TCommandEnabler& );
		void CeBuildPlan( TCommandEnabler& );
		void CeUnPack( TCommandEnabler& );
		void CeInPack( TCommandEnabler& );
		void CeSelectWork( TCommandEnabler& );
		void CeSelectPack( TCommandEnabler& );
		void CeIndexResource( TCommandEnabler& );

		LRESULT EvReLayout( WPARAM wParam, LPARAM lParam ); 			// EV_RELAYOUT
		LRESULT EvSetDirty( WPARAM wParam, LPARAM lParam ); 			// EV_DIRTY
		LRESULT EvGetDrawNet( WPARAM wParam, LPARAM lParam ); 		// EV_GETDRAWNET
		LRESULT EvGetIndexResource( WPARAM wParam, LPARAM lParam ); // EV_GETINDEXRESOURCE

		LRESULT EvInvalidateMDL( WPARAM wParam, LPARAM lParam ); 	// EV_INVALIDATEMDL
		LRESULT EvInvalidateGNT( WPARAM wParam, LPARAM lParam ); 	// EV_INVALIDATEGNT
		LRESULT EvInvalidateGST( WPARAM wParam, LPARAM lParam );		// EV_INVALIDATEGST
		LRESULT EvScrollMDL( WPARAM wParam, LPARAM lParam ); 			// EV_SCROLLMDL
		LRESULT EvScrollGNT( WPARAM wParam, LPARAM lParam ); 			// EV_SCROLLGNT
		LRESULT EvScrollGST( WPARAM wParam, LPARAM lParam ); 			// EV_SCROLLGST

		LRESULT EvNewElement( WPARAM wParam, LPARAM lParam ); 		// EV_NEWELEMENT
		LRESULT EvDeleteElement( WPARAM wParam, LPARAM lParam ); 	// EV_DELETEELEMENT
		LRESULT EvEntranceInWork( WPARAM wParam, LPARAM lParam ); 	// EV_ENTRANCEINWORK
		LRESULT EvEntranceInPack( WPARAM wParam, LPARAM lParam ); 	// EV_ENTRANCEINPACK
		LRESULT EvMoveInPack( WPARAM wParam, LPARAM lParam ); 		// EV_MOVEINPACK

	DECLARE_RESPONSE_TABLE( TMyLayout );
	};

typedef TIDoubleListImp<TMyLayout> TDMyLayouts;
typedef TIDoubleListIteratorImp<TMyLayout> TDMyLayoutsIterator;

#endif // __LAYOUT_H
