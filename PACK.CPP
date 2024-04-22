#include <time.h>
#include <owl\owlpch.h>
#include <owl\dialog.h>
#include <owl\edit.h>                                          
#include <owl\listbox.h>
#include <owl\inputdia.h>
#include <dir.h>

#include "mainw4.rh"
#include "pack.h"

STR_SET_EVENT TPack::StrSetEvent = { 70, 45, 35, 40, 5, { 0, 1, 0, 0L }, { 0, 1, 0, 255L }, 12632256L, 65280L, 8388608L, 0 };

TPack::TPack()
	{
	Name = strnewdup( "" );
	lPoint = TPoint( 0L, 0L );
	Number = 0L;
	ParentPack = NULL;
	Entrances = new TConnects();
	Exits = new TConnects();
	StartTime = FinishTime = 0UL;
   ShiftStart = ShiftFinish = 0UL;   // New

	MaxNumber = 0L; 
	DPacks = new TDPacks();
	DWorks = new TDWorks();
	Connects = new TConnects();
	BitTimes = NULL;
	ShadePacks = NULL;	
	CutOffTimes = new TDCutOffTimes();
	}

TPack::~TPack()
	{
	Entrances->Flush(true);
	Exits->Flush(true);
	delete Entrances;    // for one pack !!!!!
	delete Exits;        // for one pack !!!!!
	//free(	Name );
	delete[] Name; // CG
   Connects->Flush(true);
	delete Connects;
   if ( BitTimes )
		ResetBitTimes();
   DWorks->Flush(true);
	delete DWorks;

   DPacks->Flush(true);
	delete DPacks;

	if ( ShadePacks )
   ShadePacks->Flush(true);
		delete ShadePacks;
	if ( CutOffTimes )
   {CutOffTimes->Flush(true);
		delete CutOffTimes;}
	}

unsigned long TPack::GetQuantityEvent()
	{ 
	unsigned long QuantityEvent = DWorks->GetItemsInContainer() + DPacks->GetItemsInContainer();
	TDPacksIterator PacksIterator( *DPacks );
	while ( PacksIterator )
		QuantityEvent += PacksIterator++->GetQuantityEvent();
	return QuantityEvent;
	}

class TSelectWorkDialog : public TDialog 
	{
	private:
		TListBox*       ListBox;         // cache for transfer access.
	public:
		TSelectWorkDialog(TWindow* parent, int resId, TListBoxData& ListBoxData );
	};

TSelectWorkDialog::TSelectWorkDialog(TWindow* parent, int resId, TListBoxData& ListBoxData )
	: TDialog(parent, resId)
	{
	ListBox = new TListBox( this, ID_LISTBOXWORK );
	SetTransferBuffer( &ListBoxData );
	}

TWork* TPack::SelectWork()
	{
	char string[25];
	TListBoxData ListBoxData;
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		ultoa( DWorksIterator++->GetNumber(), string, 10 );
		ListBoxData.AddString( string );
		}
	if ( TSelectWorkDialog( NULL, IDD_SELECTWORK, ListBoxData ).Execute() == IDOK )
		{
		ListBoxData.GetSelString( string, 25 );
		return GetWork( atol( string ) );
		}
	return NULL;	
	}
	
TWork* TPack::GetWork( unsigned long TheNumber )
	{
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		TWork* Work = DWorksIterator++;
		if ( TheNumber == Work->GetNumber() )
			return Work;
		}
	return NULL;
	}

void TPack::Add( TWork* TheWork, bool Move )
	{
	if ( MaxNumber < TheWork->GetNumber() )
		MaxNumber = TheWork->GetNumber();
	DWorks->AddAtTail( TheWork );
	TheWork->SetParent( this );
	AddBitTimes( TheWork );
	if ( Move )
		{
		TConnects* Connects = new TConnects();
		TheWork->GetEntrancesConnect( Connects );
		TheWork->GetExitsConnect( Connects );
		ParentPack->SortConnects( Connects );
		delete Connects;
		}
	}

void TPack::Detach( TWork* TheWork, bool Del )
	{
	if ( Del )
		{
		TConnects* ConnectsTmp = new TConnects();
		TheWork->GetEntrancesConnect( ConnectsTmp );
		TheWork->GetExitsConnect( ConnectsTmp );
		DeleteConnects( ConnectsTmp );
		delete ConnectsTmp;
		}
	DetachBitTimes( TheWork );
	DWorks->Detach( TheWork, Del );
	}

void TPack::MoveAtTail( TWork* TheWork )
	{ 
	DWorks->Detach( TheWork );
	DWorks->AddAtTail( TheWork );
	}

void TPack::Add( TPack* ThePack, bool Move )
	{
	if ( MaxNumber < ThePack->GetNumber() )
		MaxNumber = ThePack->GetNumber();
	DPacks->AddAtTail( ThePack );
	ThePack->SetParent( this );
	AddBitTimes( ThePack );
	if ( Move )
		{
//		Not ready ! ! !
		}
	}
	
void TPack::Detach( TPack* ThePack, bool Del )
	{
	if ( Del )
		{
		TConnects* ConnectsTmp = new TConnects();
		ThePack->GetEntrancesConnect( ConnectsTmp );
		ThePack ->GetExitsConnect( ConnectsTmp );
		DeleteConnects( ConnectsTmp );
		delete ConnectsTmp;
		}
	DetachBitTimes( ThePack );	
	DPacks->Detach( ThePack, Del );
	}
	
void TPack::MoveAtTail( TPack* ThePack )
	{
	DPacks->Detach( ThePack );
	DPacks->AddAtTail( ThePack );
	}

void TPack::Add( TConnect* TheConnect )
	{ Connects->Add( TheConnect ); }

void TPack::Detach( TConnect* TheConnect, bool Del )
	{ Connects->Detach( TheConnect, Del ); }

void TPack::AddBitTimes( TWork* TheWork )
	{
	if ( BitTimes )
		{
		TSBitTimes* BitTimesTmp = new TSBitTimes();
		for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
			{
			TheWork->GetBitTimes( BitTimesTmp, i );
			if ( !BitTimesTmp->IsEmpty() )
				{
				((TSBitTimes*)*( BitTimes + i ))->Sum( BitTimesTmp );
				BitTimesTmp->Flush( TRUE );
				}
			}
		delete BitTimesTmp;
		}
	if ( ParentPack )
		ParentPack->AddBitTimes( TheWork );
	}

void TPack::DetachBitTimes( TWork* TheWork )
	{
	if ( BitTimes )
		{
		TSBitTimes* BitTimesTmp = new TSBitTimes();
		for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
			{
			TheWork->GetBitTimes( BitTimesTmp, i );
			if ( !BitTimesTmp->IsEmpty() )
				{
				((TSBitTimes*)*( BitTimes + i ))->Sub( BitTimesTmp );
				BitTimesTmp->Flush( TRUE );
				}
			}
		delete BitTimesTmp;
		}
	if ( ParentPack )
		ParentPack->DetachBitTimes( TheWork );
	}

void TPack::AddBitTimes( TPack* ThePack )
	{
	if ( BitTimes )
		for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
			((TSBitTimes*)*( BitTimes + i ))->Sum( ThePack->GetBitTimes( i ) );
	if ( ParentPack )
		ParentPack->AddBitTimes( ThePack );
	}
	
void TPack::DetachBitTimes( TPack* ThePack )
	{
	if ( BitTimes )
		for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
			((TSBitTimes*)*( BitTimes + i ))->Sub( ThePack->GetBitTimes( i ) );
	if ( ParentPack )
		ParentPack->DetachBitTimes( ThePack );
	}
	
void TPack::SortConnects( TConnects* TheConnects )
	{
	while ( !TheConnects->IsEmpty() )
		{
		TConnect* connect = TheConnects->PeekHead();
		TheConnects->DetachAtHead();
		TPack* PackStart = connect->GetWorkStart()->GetParent();
		TPack* PackFinish = connect->GetWorkFinish()->GetParent();
		if ( PackStart != this || PackFinish != this )
			{
			if ( PackStart == PackFinish )
				{
				Detach( connect );
				connect->ResetPack();
				PackStart->Add( connect );
				}
			else
				if ( PackStart == PackFinish->GetParent() )
					{
					connect->SetFinish( PackFinish );
					if ( PackStart != this )
						{
						Detach( connect );
						PackStart->Add( connect );
						}
					}
				else	
					if ( PackFinish == PackStart->GetParent() )
						{
						connect->SetStart( PackStart );
						if ( PackFinish != this )
							{
							Detach( connect );
							PackFinish->Add( connect );
							}
						}
					else	
						{
						connect->SetStart( PackStart );
						connect->SetFinish( PackFinish );
						if ( PackStart->GetParent() != this )
							{
							Detach( connect );
							PackStart->GetParent()->Add( connect );
							}
					  }
			}
		}
	}

void TPack::DeleteConnects( TConnects* TheConnects )
	{
	while ( !TheConnects->IsEmpty() )
		{
		TConnect* connect = TheConnects->PeekHead();
		TheConnects->DetachAtHead();
		Detach( connect, TRUE );
		}
	}


void TPack::UnPack( TDPacks* MainDPacks, TDWorks* MainDWorks, TConnects* MainConnects, TDPacks* MainShadePacks )
	{
	if ( !MainDPacks && !MainDWorks && !MainConnects )
		{
		if ( !ShadePacks )
			ShadePacks = new TDPacks();
		TConnectsIterator ConnectsIterator( *Connects );
		while ( ConnectsIterator )
			ConnectsIterator++->ResetPack();
		while ( !DPacks->IsEmpty() )
			{
			TPack* pack = DPacks->PeekHead();
			DPacks->DetachAtHead();
			pack->UnPack( DPacks, DWorks, Connects, ShadePacks );
			ShadePacks->AddAtTail( pack );
			}
		}
	else
		{
		while ( !DWorks->IsEmpty() )
			{
			TWork* work = DWorks->PeekHead();
			DWorks->DetachAtHead();
			MainDWorks->AddAtTail( work );
			}
		while ( !Connects->IsEmpty() )
			{
			TConnect* connect = Connects->PeekHead();
			Connects->DetachAtHead();
			connect->ResetPack();
			MainConnects->Add( connect );
			}
			
		while ( !DPacks->IsEmpty() )
			{
			TPack* pack = DPacks->PeekHead();
			DPacks->DetachAtHead();
			pack->UnPack( MainDPacks, MainDWorks, MainConnects, MainShadePacks );
			MainShadePacks->AddAtTail( pack );
			}

		if ( BitTimes )
			{
			delete BitTimes;
			BitTimes = NULL; 
			}
		}	
	}	

void TPack::OutShade()
	{
	while ( !ShadePacks->IsEmpty() ) 
		{
		TPack* pack = ShadePacks->PeekHead();
		ShadePacks->DetachAtHead();
		if ( BitTimes )
			pack->BuildBitTimes();
		pack->GetParent()->Add( pack );
		}
	delete ShadePacks;
	ShadePacks = NULL;

	TDWorks* WorksTmp = new TDWorks();
	TDWorksIterator WorksIterator( *DWorks );
	while ( WorksIterator )
		{
		TWork* workIt = WorksIterator++;
		if ( workIt->GetParent() != this )
			WorksTmp->AddAtTail( workIt );
		}
	while ( !WorksTmp->IsEmpty() ) 
		{
		TWork* work = WorksTmp->PeekHead();
		WorksTmp->DetachAtHead();
		Detach( work );
		work->GetParent()->Add( work );
		}
	delete WorksTmp;

	TConnects* ConnectsTmp = new TConnects();
	TConnectsIterator ConnectsIterator( *Connects );
	while ( ConnectsIterator )
		ConnectsTmp->Add( ConnectsIterator++ ); 
	SortConnects( ConnectsTmp );
	delete ConnectsTmp;
	}

void TPack::FlushShade()
	{
	delete ShadePacks;
	ShadePacks = NULL;
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		DWorksIterator++->SetParent( this );
	}

void TPack::CorrectTime()
	{
	if ( Number != 0L )
		{
		if ( DWorks->IsEmpty() && DPacks->IsEmpty() )
			FinishTime = StartTime;
		else
			{
			StartTime = 4294967295UL;
			FinishTime = 0L;
			}
		}
		
	TDPacksIterator DPacksIterator( *DPacks );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		packIt->CorrectTime();
		if ( StartTime > packIt->GetStartTime() )
			StartTime = packIt->GetStartTime();
		if ( FinishTime < packIt->GetFinishTime() )
			FinishTime = packIt->GetFinishTime();
		}
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		if ( StartTime > workIt->GetStartTime() )
			StartTime = workIt->GetStartTime();
		if ( FinishTime < workIt->GetFinishTime() )
			FinishTime = workIt->GetFinishTime();
		}
	}


void TPack::BuildBitTimes()
	{
	BitTimes = (TSBitTimes**)malloc( sizeof( TSBitTimes* ) * ( TEvent::StrSetScale.QuantityResource + 1 ) );
	for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
		*( BitTimes + i ) = new TSBitTimes();
		
	TDWorksIterator WorksIterator( *DWorks );
	while ( WorksIterator )
		AddBitTimes( WorksIterator++ );

	TDPacksIterator PacksIterator( *DPacks );
	while ( PacksIterator )
		PacksIterator++->BuildBitTimes();
	}


void TPack::ResetBitTimes()
	{
	for ( int i = 0; i <= TEvent::StrSetScale.QuantityResource; i++ )
		{
      ((TSBitTimes*)*( BitTimes + i ))->Flush(true);
		delete ((TSBitTimes*)*( BitTimes + i ));
      }
	free( BitTimes );
	BitTimes = NULL;

	TDPacksIterator PacksIterator( *DPacks );
	while ( PacksIterator )
		PacksIterator++->ResetBitTimes();
	}

void TPack::ResetCutOffTimes()

	{
   CutOffTimes->Flush(true); // ????????  my 16.11.96
	delete CutOffTimes;
	CutOffTimes = NULL;
	}
	
void TPack::GetBitTimes( TSBitTimes* TheBitTimes, int TheIndexResource )
	{ 
	if ( GetBitTimes( TheIndexResource ) )
		TheBitTimes->Sum( GetBitTimes( TheIndexResource ) ); 
	}

unsigned long TPack::GetCarentTime()
	{ 
	unsigned long CarentTime = FinishTime;
	unsigned long CarentTimeEvent;
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		if ( workIt->GetPercent() != 100 )
			{
			CarentTimeEvent = workIt->GetCarentTime();
			if ( CarentTime > CarentTimeEvent )
				CarentTime = CarentTimeEvent;
			}	
		}	
	TDPacksIterator DPacksIterator( *DPacks );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		CarentTimeEvent = packIt->GetCarentTime();
		if ( packIt->GetFinishTime() != CarentTimeEvent && CarentTime > CarentTimeEvent )
			CarentTime = CarentTimeEvent;
		}
	return CarentTime; 
	}

void TPack::ResetChec()
	{
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		DWorksIterator++->ResetChec();
	TDPacksIterator DPacksIterator( *DPacks );
	while ( DPacksIterator )
		DPacksIterator++->ResetChec();
	}


void TPack::ArrangeStartTime()
	{
	ResetChec();
	TConnects* ConnectsCheck = new TConnects();
	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		if ( workIt->GetEntrancesConnect() == 0 )
			workIt->SetChec( ConnectsCheck );
		}
	while ( !ConnectsCheck->IsEmpty() )
		{
		TConnect* connect = ConnectsCheck->PeekHead();
		ConnectsCheck->DetachAtHead();
		connect->GetWorkFinish()->SetChec( ConnectsCheck );
		}

	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		if ( workIt->GetEntrancesConnect() == 0 )
			workIt->CorrectTime( StartTime, ConnectsCheck );
		}
	while ( !ConnectsCheck->IsEmpty() )
		{
		TConnect* connect = ConnectsCheck->PeekHead();
		ConnectsCheck->DetachAtHead();
		connect->GetWorkFinish()->CorrectTime( connect->GetWorkStart()->GetFinishTime(), ConnectsCheck );
		}
	delete ConnectsCheck;

	TDPacksIterator DPacksIterator( *DPacks );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		packIt->SetStartTime( StartTime );
		packIt->ArrangeStartTime();
		}
	}
	
void TPack::ArrangeFinishTime()
	{}

TPack& TPack::operator = ( TPack& other )
	{
	//free( Name );
   delete[] Name; // CG !!!
	Name = strnewdup( other.Name );
	lPoint = other.lPoint;
	Number = other.Number;
	StartTime = other.StartTime;
	FinishTime = other.FinishTime;
	Entrances->Flush();
	other.GetEntrancesConnect( Entrances );
	Exits->Flush();
	other.GetExitsConnect( Exits );
	return *this;
	}

void TPack::write( opstream& os, unsigned long* TheIndex ) 
	{
	os.fwriteString( Name );
	os << lPoint << Number << StartTime << FinishTime;
	os << GetQuantityEvent();
	if ( !ParentPack )
		{
		os << *CutOffTimes;
		TheIndex = new unsigned long;
		*TheIndex = 1L;
		}

	os << DWorks->GetItemsInContainer();
	os << DPacks->GetItemsInContainer();
	os << Connects->GetItemsInContainer();

	TDWorksIterator DWorksIterator( *DWorks );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		workIt->SetTmp( (*TheIndex)++ );
		workIt->write( os );
		}

	TDPacksIterator DPacksIterator( *DPacks );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		packIt->SetTmp( (*TheIndex)++ );
		packIt->write( os, TheIndex );
		}

	TConnectsIterator ConnectsIterator( *Connects );
	while ( ConnectsIterator )
		ConnectsIterator++->write( os );

	if ( !ParentPack )
		delete TheIndex;
	}

void* TPack::read( ipstream& is, TPack* ThePack, unsigned long* TheIndex, TEvent** TheArrayEvents )
	{
	ParentPack = ThePack;
//  	free( Name );   CG !!!
   delete[] Name;
	Name = is.freadString();
	is >> lPoint >> Number >> StartTime >> FinishTime;
	unsigned long QuantityEvent;
	is >> QuantityEvent;
	if ( !ParentPack )
		{
		is >> *CutOffTimes;
		TheIndex = new unsigned long;
		*TheIndex = 1L;
		TheArrayEvents = (TEvent**)malloc( sizeof( TEvent* ) * QuantityEvent );
		}

	unsigned QuantityWorks;
	unsigned QuantityPacks;
	unsigned QuantityConnects;
	is >> QuantityWorks;
	is >> QuantityPacks;
	is >> QuantityConnects;

	while ( QuantityWorks-- )
		{
     	TWork* work = new TWork();
		*( TheArrayEvents + (*TheIndex)++ - 1L ) = work;
		work->read( is, this );
		Add( work );
		}

	while ( QuantityPacks-- )
		{
		TPack* pack = new TPack();
		*( TheArrayEvents + (*TheIndex)++ - 1L ) = pack;
		pack->read( is, this, TheIndex, TheArrayEvents );
		Add( pack );
		}

	while ( QuantityConnects-- )
		{
		TConnect* connect = new TConnect();
		connect->read( is, TheArrayEvents );
		Add( connect );
		}

	if ( !ParentPack )
		{
		delete TheIndex;
		free( TheArrayEvents );	
		}
	return this;
	}


