#include <owl\owlpch.h>
#include <owl\opensave.h>
#include <owl\edit.h>
#include <owl\validate.h>
#include <owl\listbox.h>
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "layout.h"
#include "mdl_wind.h"
#include "cal_wind.h"
#include "gnt_wind.h"
#include "gst_wind.h"
#include "new_pack.h"
#include "mainw4.rh"
#include "file_dbf.h"
#include "speci.str"

DEFINE_RESPONSE_TABLE1( TMyLayout, TLayoutWindow )
	EV_COMMAND( CM_FILESAVE, CmFileSave ),
	EV_COMMAND( CM_FILESAVEAS, CmFileSaveAs ),
	EV_COMMAND( CM_CLOSEPACK, CmClosePack ),
	EV_COMMAND( CM_EDITPACK, CmEditPack ),
	EV_COMMAND( CM_PRINT, CmPrint ),
	EV_COMMAND( CM_ACTIVATEMDL, CmActivateMDL ),
	EV_COMMAND( CM_ACTIVATEGNT, CmActivateGNT ),
	EV_COMMAND( CM_ACTIVATEGST, CmActivateGST ),
	EV_COMMAND( CM_ARRANGE, CmArrange ),
	EV_COMMAND( CM_RENUMBER, CmRenumber ),
	EV_COMMAND( CM_CONVERT, CmConvert ),
	EV_COMMAND( CM_OPTIMIZATION, CmOptimization ),
	EV_COMMAND( CM_UPDATEMODEL, CmUpdateModel ),
	EV_COMMAND( CM_UPDATEDBF, CmUpdateDBF ),
	EV_COMMAND( CM_BUILDRESOURCE, CmBuildResource ),
	EV_COMMAND( CM_BUILDPLAN, CmBuildPlan ),
	EV_COMMAND( CM_UNPACK, CmUnPack ),
	EV_COMMAND( CM_INPACK, CmInPack ),
	EV_COMMAND( CM_SELECTWORK, CmSelectWork ),
	EV_COMMAND( CM_SELECTPACK, CmSelectPack ),
	EV_COMMAND( CM_INDEXRESOURCE, CmIndexResource ),
	EV_COMMAND( CM_SPUFOX, CmSpuFox ),
	EV_COMMAND( CM_FIND, CmFind ),

	EV_COMMAND_ENABLE( CM_FILESAVE, CeFileSave ),
	EV_COMMAND_ENABLE( CM_FILESAVEAS, CeFileSaveAs ),
	EV_COMMAND_ENABLE( CM_CLOSEPACK, CeClosePack ),
	EV_COMMAND_ENABLE( CM_ACTIVATEMDL, CeActivateMDL ),
	EV_COMMAND_ENABLE( CM_ACTIVATEGNT, CeActivateGNT ),
	EV_COMMAND_ENABLE( CM_ACTIVATEGST, CeActivateGST ),
	EV_COMMAND_ENABLE( CM_CONVERT, CeConvert ),
	EV_COMMAND_ENABLE( CM_OPTIMIZATION, CeOptimization ),
	EV_COMMAND_ENABLE( CM_UPDATEMODEL, CeUpdateModel ),
	EV_COMMAND_ENABLE( CM_BUILDRESOURCE, CeBuildResource ),
	EV_COMMAND_ENABLE( CM_BUILDPLAN, CeBuildPlan ),
	EV_COMMAND_ENABLE( CM_UNPACK, CeUnPack ),
	EV_COMMAND_ENABLE( CM_INPACK, CeInPack ),
	EV_COMMAND_ENABLE( CM_SELECTWORK, CeSelectWork ),
	EV_COMMAND_ENABLE( CM_SELECTPACK, CeSelectPack ),
	EV_COMMAND_ENABLE( CM_INDEXRESOURCE, CeIndexResource ),
	EV_COMMAND_ENABLE( CM_SPUFOX, CeSpuFox ),
	EV_COMMAND_ENABLE( CM_FIND, CeFind ),
	EV_CHILD_NOTIFY_ALL_CODES( ID_NETBUTTON, SetNet ),

	EV_REGISTERED( "EV_RELAYOUT", EvReLayout ),
	EV_REGISTERED( "EV_DIRTY", EvSetDirty ),
	EV_REGISTERED( "EV_GETDRAWNET", EvGetDrawNet ),
	EV_REGISTERED( "EV_GETINDEXRESOURCE", EvGetIndexResource ),

	EV_REGISTERED( "EV_INVALIDATEMDL", EvInvalidateMDL ),
	EV_REGISTERED( "EV_INVALIDATEGNT", EvInvalidateGNT ),
	EV_REGISTERED( "EV_INVALIDATEGST", EvInvalidateGST ),
	EV_REGISTERED( "EV_SCROLLMDL", EvScrollMDL ),
	EV_REGISTERED( "EV_SCROLLGNT", EvScrollGNT),
	EV_REGISTERED( "EV_SCROLLGST", EvScrollGST ),

	EV_REGISTERED( "EV_NEWELEMENT", EvNewElement ),
	EV_REGISTERED( "EV_DELETEELEMENT", EvDeleteElement ),
	EV_REGISTERED( "EV_ENTRANCEINWORK", EvEntranceInWork ),
	EV_REGISTERED( "EV_ENTRANCEINPACK", EvEntranceInPack),
	EV_REGISTERED( "EV_MOVEINPACK", EvMoveInPack ),
END_RESPONSE_TABLE;

TMyLayout::TMyLayout( TPack* ThePack, const char far* fileName )
	: TLayoutWindow( 0 ), TWindow( 0, 0, 0 )
	{
	FileName = fileName ? strnewdup( fileName ) : NULL;
	Attr.Style |= WS_BORDER;

	StrPackEdit = new STR_PACK_EDIT();
	StrPackEdit->PackStart = NULL;
	StrPackEdit->PackFinish = NULL;
	StrPackEdit->WorkStart = NULL;
	StrPackEdit->WorkFinish = NULL;
	StrPackEdit->Connect = NULL;
	PackMain = StrPackEdit->PackView = ThePack;

	IsDirty = FALSE;
	DrawNetButton = TRUE;
	Type = 0;
	MaxRang = 0;
	MaxLevel = 0;
	IndexResource = TEvent::StrSetScale.QuantityResource;

	AllWindow[0].BASWindow = new TMDLWindow( this, StrPackEdit );
	AllWindow[0].BASScreen = TRUE;
	AllWindow[0].MaxScreen = FALSE;
	AllWindow[1].BASWindow = new TGNTWindow( this, StrPackEdit );
	AllWindow[1].BASScreen = TRUE;
	AllWindow[1].MaxScreen = FALSE;
	AllWindow[2].BASWindow = new TGSTWindow( this, StrPackEdit );
	AllWindow[2].BASScreen = FALSE;
	AllWindow[2].MaxScreen = FALSE;
	AllWindow[3].BASWindow = new TCALWindow( this, StrPackEdit );
	AllWindow[3].BASScreen = TRUE;
	AllWindow[3].MaxScreen = FALSE;

	Button = new TButton( this, ID_NETBUTTON, "", 0, 0, 0, 0 );
	}

TMyLayout::~TMyLayout()
	{
	if ( FileName )
	//	free( FileName );
   delete[] FileName; // CG 
	delete StrPackEdit;
	delete PackMain;

	delete AllWindow[0].BASWindow;
	delete AllWindow[1].BASWindow;
	delete AllWindow[2].BASWindow;
	delete AllWindow[3].BASWindow;
	delete Button;
	}


void TMyLayout::SetupWindow()
	{
	TLayoutWindow::SetupWindow();
	SetFileName( FileName );
	ReLayout();
	}

void TMyLayout::ClosePack()
	{
	if ( PackMain != StrPackEdit->PackView  )
		{
		StrPackEdit->PackView = StrPackEdit->PackView->GetParent();
		SetWindowText();
		Invalidate();
		}
	}


bool TMyLayout::CanClose()
	{
	if ( IsDirty )
		{
        ////////////////////// BCP !!!!!!!!!!!!!!!
		TResString msgTmp( *GetModule(), IDS_FILECHANGED );
		TResString untitled( *GetModule(), IDS_UNTITLEDFILE );
		char* msg = new char[ MAXPATH + msgTmp.length() ];
		wsprintf( msg, msgTmp.c_str(), FileName ? (const char far*)FileName : untitled.c_str() );
		int result = MessageBox( msg, GetApplication()->GetName(), MB_YESNOCANCEL|MB_ICONQUESTION );
		//delete msg;
      delete[] msg; // CG
		return result == IDYES ? Save() : result != IDCANCEL;
		}
	return TRUE;
	}


//
// saves the contents of the TEdit child control into the file currently
// being editted
//
bool TMyLayout::Save()
	{
	if ( IsDirty )
		{
		if ( !FileName )
			return SaveAs();

		if ( !Write() )
			{
            //////////////BCP !!!!!!!!!!!!!!!!!!!
			TResString msgTmp( *GetModule(), IDS_UNABLEWRITE );
			char* msg = new char[MAXPATH + msgTmp.length()];
			wsprintf( msg, msgTmp.c_str(), FileName );
			MessageBox( msg, GetApplication()->GetName(), MB_ICONEXCLAMATION | MB_OK );
			delete msg;
			return FALSE;
			}
		IsDirty = FALSE;
		}		
	return TRUE;  // editor's contents haven't been changed
	}

//
// saves the contents of the TEdit child control into a file whose name
// is retrieved from the user, through execution of a "Save" file dialog
//
// returns true if the file was saved
//
bool TMyLayout::SaveAs()
	{
	TOpenSaveDialog::TData FileData;
	FileData.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
    /////////////////// BCP !!!!!!!!!!!!!!!!!!!!!!
	FileData.SetFilter( TResString( *GetModule(), IDS_FILEFILTER ).c_str() );
	if ( FileName )
		strcpy( FileData.FileName, FileName );
	else
		*FileData.FileName = 0;

	if ( (TFileSaveDialog( this, FileData )).Execute() == IDOK )
		{
		if ( Write( FileData.FileName ) )
			{
			SetFileName( FileData.FileName );
			IsDirty = FALSE;
			return TRUE;
			}
        /////////////////////// BCP !!!!!!!!!!!!!!!!!!!!!!!!!
		TResString msgTmp( *GetModule(), IDS_UNABLEWRITE );
		char* msg = new char[ MAXPATH + msgTmp.length() ];
		wsprintf( msg, msgTmp.c_str(), FileName );
		MessageBox( msg, GetApplication()->GetName(), MB_ICONEXCLAMATION | MB_OK );
		delete msg;
		}
	return FALSE;
	}


//
// writes the contents of the TEdit child control to a specified file, or
// the previously-specified file if none passed.
// The caller is responsible for any error UI
//
bool TMyLayout::Write( const char far* fileName )
	{

	if ( !fileName )
		if ( FileName )
			fileName = FileName;
		else
			return FALSE;

	ofpstream os( fileName );
	if (!os)
		{
        //////////////// BCP !!!!!!!!!!!!!!!!!!!!!!!!!!
		TResString msgTmp1( *GetModule(), IDS_UNABLEWRITE );
		TResString msgTmp2( *GetModule(), IDS_ERROR );
		char* msg = new char[ MAXPATH + msgTmp1.length() ];
		wsprintf( msg, msgTmp1.c_str(), fileName );
		MessageBox( msg, msgTmp2.c_str(), MB_OK | MB_ICONQUESTION );
		delete msg;
		return FALSE;
		}
	PackMain->write( os );
	PackMain->FlushShade();
	return TRUE;
	}

void TMyLayout::SetFileName( const char far* fileName )
	{
	if ( fileName != FileName )
		{
		if ( FileName )
			free( FileName );
		FileName = fileName ? strnewdup( fileName ) : 0;
		}
	SetWindowText();
	}

void TMyLayout::SetWindowText()
	{
    //////////////////////// BCP !!!!!!!!!!!!!!!!!!!
	TResString untitled( *GetModule(), IDS_UNTITLEDFILE );
	const char far* p = FileName ? ( const char far* )FileName : untitled.c_str();
	char* newName = new char[ strlen( p ) + 1 + strlen( StrPackEdit->PackView->GetName() ) + 2 ];
	const char frmt1[] = "%s(%s)";
	wsprintf( newName, frmt1, p, StrPackEdit->PackView->GetName() );
	Parent->SetWindowText( newName );

	if ( !Parent->Title || !*Parent->Title )
		Parent->SetWindowText( newName );
	else
		{
		const char frmt2[] = "%s - %s";
		char* newCaption = new char[ strlen( Parent->Title ) + 3 + strlen( newName ) + 1 ];
		wsprintf( newCaption, frmt2, newName, Parent->Title );
		Parent->SetWindowText( newCaption );
		delete newCaption;
		}
	delete[] newName;  // CG !!!
	}

class TNewIndexResource : public TDialog
	{
	private:
		int* IndexResource;
		TListBox* ListBoxResourceAll;
	public:
		TNewIndexResource( TWindow* parent, int resId, int* TheIndexResource );
		void SetupWindow();
		void CloseWindow( int ret );
	};

TNewIndexResource::TNewIndexResource( TWindow* parent, int resId, int* TheIndexResource )
	: TDialog( parent, resId )
	{
	IndexResource = TheIndexResource;
	ListBoxResourceAll = new TListBox( this, ID_LISTBOXRESOURCEALL );
	}

void TNewIndexResource::SetupWindow()
	{
	TDialog::SetupWindow();

	char StringTmp[25];
	FileDBF *file = new FileDBF( "speci.dbf" );
	struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
	for ( int i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
		{
		SpeciStruct.Name[ 24 ] = '\0';
		ListBoxResourceAll->AddString( SpeciStruct.Name );
		}
	delete file;
	ListBoxResourceAll->AddString( "All" );
	}

void TNewIndexResource::CloseWindow( int ret )
	{
   int i;
	int Index = ListBoxResourceAll->GetSelIndex();
	bool Tmp = FALSE;
	if ( Index >= 0 )
		{
		char String[ 25 ];
		ListBoxResourceAll->GetString( String, Index );
		FileDBF *file = new FileDBF( "speci.dbf" );
		struct speci SpeciStruct, *indexSpeciStruct = &SpeciStruct;
		for (  i = 0; file->readDBF( i, (void *)indexSpeciStruct ) && i < TEvent::StrSetScale.QuantityResource; i++ )
			{
			SpeciStruct.Name[ 24 ] = '\0';
			if ( strcmp( SpeciStruct.Name, String ) == 0 )
				{
				Tmp = TRUE;
				break;
				}
			}
		delete file;
		if ( Tmp )
			*IndexResource = i;
		else
			*IndexResource = TEvent::StrSetScale.QuantityResource;
		}
	else
		*IndexResource = TEvent::StrSetScale.QuantityResource;

	TDialog::CloseWindow( ret );
	}



void TMyLayout::CmIndexResource()
	{
	int IndexResourceTmp = IndexResource;
	if ( TNewIndexResource( this, IDD_INDEXRESOURCE, &IndexResourceTmp ).Execute() == IDOK )
		{
		IndexResource = IndexResourceTmp;
		if	( !AllWindow[0].MaxScreen && !AllWindow[1].MaxScreen )
			AllWindow[2].BASWindow->Invalidate();
		}
	}


void TMyLayout::SetNet( UINT )
	{
	if ( DrawNetButton )
		DrawNetButton = FALSE;
	else
		DrawNetButton = TRUE;
	if	( AllWindow[1].BASScreen && !AllWindow[0].MaxScreen && !AllWindow[2].MaxScreen )
		AllWindow[1].BASWindow->Invalidate();
	if	( AllWindow[2].BASScreen && !AllWindow[0].MaxScreen && !AllWindow[1].MaxScreen )
		AllWindow[2].BASWindow->Invalidate();
	}

void TMyLayout::EditPack()
	{
	if ( TPackDialog( this, IDD_NEWPACK, PackMain ).Execute() == IDOK )
		;
	}


void TMyLayout::Optimization()
	{
////////////////////////////// My insert //////////////////////////////
WORD wReturn;
char szMsg[80];
HCURSOR hcurSave=::SetCursor(LoadCursor(NULL,IDC_WAIT));
wReturn = WinExec("PartProject", SW_SHOW);
::SetCursor(hcurSave);
if (wReturn < 32) {
//	 sprintf(szMsg, "WinExec failed; error code = %d", wReturn);
//	 MessageBox(HWindow, szMsg, "Error", MB_ICONSTOP);
}
else {
//sprintf(szMsg, "WinExec returned %d", wReturn);
//	 MessageBox(HWindow, szMsg, "", MB_OK);
}
//////////////////////////////////////////////////////////////////////////
}

void TMyLayout::SpuFox()
	{
////////////////////////////// My insert //////////////////////////////
WORD wReturn;
char szMsg[80];
HCURSOR hcurSave=::SetCursor(LoadCursor(NULL,IDC_WAIT));
wReturn = WinExec("PMDBProject", SW_SHOW);
::SetCursor(hcurSave);
if (wReturn < 32) {
//	 sprintf(szMsg, "WinExec failed; error code = %d", wReturn);
//	 MessageBox(this, szMsg, "Error", MB_ICONSTOP);
}
else {
//  sprintf(szMsg, "WinExec returned %d", wReturn);
//	 MessageBox(this, szMsg, "", MB_OK);
}

//////////////////////////////////////////////////////////////////////////
}

void TMyLayout::Find()
	{
	// ????????? Parent->PostMessage();
	AllWindow[0].BASWindow->PostMessage(WM_LBUTTONDBLCLK);
	}





