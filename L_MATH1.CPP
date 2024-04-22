#include <owl\owlpch.h>
#include <ddeml.h>

#include "layout.h"

void TMyLayout::Rang()
	{
	MaxRang = 0UL;
	unsigned long Rang;
	TConnects* ConnectsTmp = new TConnects();
	
	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		packIt->SetTmp( 0 );
		packIt->SetTmpR( 0 );
		}
	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		workIt->SetTmp( 0 );
		workIt->SetTmpR( 0 );
		}

	TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
	while ( ConnectsIterator )
		{
		TEvent* eventIt = ConnectsIterator++->GetEventFinish();
		eventIt->SetTmp( eventIt->GetTmp() + 1 );
		}

	DPacksIterator.Restart();
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		if ( packIt->GetTmp() == 0 )
			packIt->GetExitsConnect( ConnectsTmp );
		}
	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		if ( workIt->GetTmp() == 0 )
			workIt->GetExitsConnect( ConnectsTmp );
		}

	while ( !ConnectsTmp->IsEmpty() )
		{
		TConnect* connect = ConnectsTmp->PeekHead();
		ConnectsTmp->DetachAtHead();
		if ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView ) 
			{
			TEvent* event = connect->GetEventFinish();
			Rang = connect->GetEventStart()->GetTmpR() + 1;
			if ( event->GetTmpR() < Rang )
				{
				event->SetTmpR( Rang );
				if ( MaxRang < Rang  )
					MaxRang = Rang;
				}
			event->SetTmp( event->GetTmp() - 1 );	
			if ( event->GetTmp() == 0 )
				event->GetExitsConnect( ConnectsTmp );
			}
		}
	delete ConnectsTmp;
	}

void TMyLayout::Level()
	{
   unsigned long i;
	TEvents** RangEvents = (TEvents**)malloc( sizeof( TEvents* ) * ( MaxRang + 1 ) );
	TConnects** ArrayConnects = (TConnects**)malloc( sizeof( TConnects* ) * ( MaxRang + 1 ) );
	unsigned long* ArrayLevel = (unsigned long*)malloc( sizeof( unsigned long ) * ( MaxRang + 1 ) );
	for (  i = 0UL; i <= MaxRang; i++ )
		{
		((TEvents*)*( RangEvents + i )) = new TEvents();
		((TConnects*)*( ArrayConnects + i )) = new TConnects();
		*( ArrayLevel + i ) = 0;
		}
		
	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		((TEvents*)*( RangEvents + packIt->GetTmpR() ))->Add( packIt );
		if ( packIt->GetTmpR() == MaxRang )
			packIt->SetTmpL( (unsigned long)TRUE );
		else
			packIt->SetTmpL( (unsigned long)FALSE );
		}
	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		((TEvents*)*( RangEvents + workIt->GetTmpR() ))->Add( workIt );
		if ( workIt->GetTmpR() == MaxRang )
			workIt->SetTmpL( (unsigned long)TRUE );
		else
			workIt->SetTmpL( (unsigned long)FALSE );
		}

	TConnects* ConnectsTmp = new TConnects();	
	for ( i = 1; i <= MaxRang; i++ )
		{
		TEventsIterator* EventsIterator = new TEventsIterator( (TEvents)**( RangEvents + MaxRang - i ) );
		while ( *EventsIterator )
			{
			TEvent* eventIt = (*EventsIterator)++;
			eventIt->GetExitsConnect( ConnectsTmp );
			while ( !ConnectsTmp->IsEmpty() )
				{
				TConnect* connect = ConnectsTmp->PeekHead();
				ConnectsTmp->DetachAtHead();
				if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView ) && 
					  (bool)connect->GetEventFinish()->GetTmpL() && eventIt->GetTmpR() + 1 == connect->GetEventFinish()->GetTmpR() )
					{
					eventIt->SetTmpL( (unsigned long)TRUE );
					ConnectsTmp->Flush();
					}
				}
			}
		delete EventsIterator;	
		}
		
	for ( i = 0; i <= MaxRang; i++ )
		{
		TEventsIterator* EventsIterator = new TEventsIterator( (TEvents)**( RangEvents + MaxRang - i ) );
		while ( *EventsIterator )
			{
			TEvent* eventIt = (*EventsIterator)++;
			if ( (bool)eventIt->GetTmpL() )
				{
				unsigned long MinConnect = ( i == 0 ) ? 0 : 4294967295UL;
				eventIt->GetExitsConnect( ConnectsTmp );
				while ( !ConnectsTmp->IsEmpty() )
					{
					TConnect* connect = ConnectsTmp->PeekHead();
					ConnectsTmp->DetachAtHead();
					if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView ) && MinConnect > connect->GetEventFinish()->GetTmp() )
						MinConnect = connect->GetEventFinish()->GetTmp();
					}	
				eventIt->SetTmp( eventIt->GetEntrancesConnect() + eventIt->GetExitsConnect() + MinConnect );
				}
			else
				eventIt->SetTmp( 4294967295UL );
			}
		delete EventsIterator;	
		}


	DPacksIterator.Restart();
	while ( DPacksIterator )
		DPacksIterator++->SetTmpL( 4294967295UL );
	DWorksIterator.Restart();
	while ( DWorksIterator )
		DWorksIterator++->SetTmpL( 4294967295UL );

	TEvent* EventLevel;	
	for ( i = 0; i <= MaxRang; i++ )
		{
		TEventsIterator* EventsIterator = new TEventsIterator( (TEvents)**( RangEvents + i ) );
		unsigned long MinConnect = 4294967295UL;
		while ( *EventsIterator )
			{
			TEvent* eventIt = (*EventsIterator)++;
			if ( i == 0 && MinConnect > eventIt->GetTmp() )
				{
				MinConnect = eventIt->GetTmp();
				EventLevel = eventIt;
				}
			if ( MinConnect > eventIt->GetTmp() )
				{
				eventIt->GetEntrancesConnect( ConnectsTmp );
				while ( !ConnectsTmp->IsEmpty() )
					{
					TConnect* connect = ConnectsTmp->PeekHead();
					ConnectsTmp->DetachAtHead();
					if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventStart()->GetParent() == StrPackEdit->PackView ) && connect->GetEventStart()->GetTmpL() == 0 )
						{
						MinConnect = eventIt->GetTmp();
						EventLevel = eventIt;
						ConnectsTmp->Flush();
						}
					}	
				}
			}	
		EventLevel->SetTmpL( 0UL );		
		EventLevel->GetExitsConnect( ((TConnects*)*( ArrayConnects + 0UL )) );
		delete EventsIterator;	
		}

	while ( !((TConnects*)*( ArrayConnects + 0UL ))->IsEmpty() )
		{
		TConnectsIterator* ConnectsIterator = new TConnectsIterator( *((TConnects*)*( ArrayConnects + 0UL )) );
		while ( *ConnectsIterator )
			{
			TConnect* connectIt = (*ConnectsIterator)++;
			if ( ( connectIt->GetPackFinish() != NULL && connectIt->GetEventFinish()->GetParent() != StrPackEdit->PackView ) || ( connectIt->GetEventStart()->GetTmpL() != 4294967295UL && connectIt->GetEventFinish()->GetTmpL() != 4294967295UL ) )
				ConnectsTmp->Add( connectIt );
			}
		delete ConnectsIterator;
		while ( !ConnectsTmp->IsEmpty() )
			{
			((TConnects*)*( ArrayConnects + 0UL ))->Detach( ConnectsTmp->PeekHead() );
			ConnectsTmp->DetachAtHead();
			}

		DPacksIterator.Restart();
		while ( DPacksIterator )
			DPacksIterator++->SetTmp( 4294967295UL );
		DWorksIterator.Restart();
		while ( DWorksIterator )
			DWorksIterator++->SetTmp( 4294967295UL );

		EventLevel = NULL;
		unsigned long EventStep = 4294967295UL;
		for ( i = 1; i <= MaxRang; i++ )
			{
			((TConnects*)*( ArrayConnects + i ))->Flush();	
			ConnectsIterator = new TConnectsIterator( *((TConnects*)*( ArrayConnects + i - 1 )) );
			while ( *ConnectsIterator )
				{
				TConnect* connectIt = (*ConnectsIterator)++;
				if ( ( connectIt->GetPackStart() == connectIt->GetPackFinish() || connectIt->GetEventFinish()->GetParent() == StrPackEdit->PackView ) && connectIt->GetEventFinish()->GetTmp() == 4294967295UL )
					{
					connectIt->GetEventFinish()->GetExitsConnect( ConnectsTmp );
					if ( connectIt->GetEventStart()->GetTmp() == 4294967295UL )
						connectIt->GetEventFinish()->SetTmp( connectIt->GetEventFinish()->GetTmpR() - connectIt->GetEventStart()->GetTmpR() );
					else	
						connectIt->GetEventFinish()->SetTmp( connectIt->GetEventStart()->GetTmp() + connectIt->GetEventFinish()->GetTmpR() - connectIt->GetEventStart()->GetTmpR() );
					}
				while ( !ConnectsTmp->IsEmpty() )
					{		
					TConnect* connect = ConnectsTmp->PeekHead();
					ConnectsTmp->DetachAtHead();
					if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView ) && 
						  connect->GetEventFinish()->GetTmpL() != 4294967295UL && EventStep > connect->GetEventStart()->GetTmp() + connect->GetEventFinish()->GetTmpR() - connect->GetEventStart()->GetTmpR() ) 
						{
						EventStep = connect->GetEventStart()->GetTmp() + connect->GetEventFinish()->GetTmpR() - connect->GetEventStart()->GetTmpR();
						EventLevel = connect->GetEventStart();
						}
					else
						((TConnects*)*( ArrayConnects + i ))->Add( connect );	
					}
				}
			delete ConnectsIterator;	

			if ( EventLevel )
				{
				EventLevel->SetTmpL( ++(*( ArrayLevel + EventLevel->GetTmpR() )) );
				EventLevel->GetExitsConnect( ((TConnects*)*( ArrayConnects + 0UL )) );
				while ( i > 1 )
					{
					EventLevel->GetEntrancesConnect( ConnectsTmp );
					ConnectsIterator = new TConnectsIterator( *((TConnects*)*( ArrayConnects + i-- - 1 )) );
					while ( !ConnectsTmp->IsEmpty() )
						{		
						TConnect* connect = ConnectsTmp->PeekHead();
						ConnectsTmp->DetachAtHead();
						ConnectsIterator->Restart();
						while ( *ConnectsIterator )
							if ( connect == (*ConnectsIterator)++ )
								{
								EventLevel = connect->GetEventStart();
								EventLevel->SetTmpL( ++(*( ArrayLevel + EventLevel->GetTmpR() )) );
								EventLevel->GetExitsConnect( ((TConnects*)*( ArrayConnects + 0UL )) );
								ConnectsTmp->Flush();
								break;
								}
						}
					delete ConnectsIterator;	
					}
				break;
				}	
			}
		}	

	delete ConnectsTmp;
	for ( i = 0UL; i <= MaxRang; i++ )
		{
		((TEvents*)*( RangEvents + i ))->Flush();
		delete ((TEvents*)*( RangEvents + i ));
		((TConnects*)*( ArrayConnects + i ))->Flush();
		delete ((TConnects*)*( ArrayConnects + i ));
		}
	free( RangEvents );
	free( ArrayConnects );
	free( ArrayLevel );
	}

void TMyLayout::Arrange()
	{
   unsigned long i;
	TWork* WorkStart = new TWork();
	TWork* WorkFinish = new TWork();

	TConnects* ConnectsTmp = new TConnects();

	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		packIt->GetEntrancesConnect( ConnectsTmp );
		while ( !ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = ConnectsTmp->PeekHead();
			if ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventStart()->GetParent() == StrPackEdit->PackView )
				break;
			ConnectsTmp->DetachAtHead();
			}
		if ( ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = new TConnect( WorkStart, WorkFinish, NULL, packIt );
			StrPackEdit->PackView->Add( connect );
			}
		else
			ConnectsTmp->Flush();

		packIt->GetExitsConnect( ConnectsTmp );
		while ( !ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = ConnectsTmp->PeekHead();
			if ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView )
				break;
			ConnectsTmp->DetachAtHead();
			}
		if ( ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = new TConnect( WorkStart, WorkFinish, packIt, NULL );
			StrPackEdit->PackView->Add( connect );
			}
		else
			ConnectsTmp->Flush();
		}

	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		workIt->GetEntrancesConnect( ConnectsTmp );
		while ( !ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = ConnectsTmp->PeekHead();
			if ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventStart()->GetParent() == StrPackEdit->PackView )
				break;
			ConnectsTmp->DetachAtHead();
			}
		if ( ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = new TConnect( WorkStart, workIt, NULL, NULL );
			StrPackEdit->PackView->Add( connect );
			}
		else
			ConnectsTmp->Flush();

		workIt->GetExitsConnect( ConnectsTmp );
		while ( !ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = ConnectsTmp->PeekHead();
			if ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView )
				break;
			ConnectsTmp->DetachAtHead();
			}
		if ( ConnectsTmp->IsEmpty() )
			{
			TConnect* connect = new TConnect( workIt, WorkFinish, NULL, NULL );
			StrPackEdit->PackView->Add( connect );
			}
		else
			ConnectsTmp->Flush();
		}
	StrPackEdit->PackView->Add( WorkStart );
	StrPackEdit->PackView->Add( WorkFinish );

	Rang();
	Level();

	StrPackEdit->PackView->Detach( WorkStart, TRUE );
	StrPackEdit->PackView->Detach( WorkFinish, TRUE );

	Rang();

	int SizeRang = Max( TWork::StrSetEvent.HConstSizeMDL, TPack::StrSetEvent.HConstSizeMDL ) * 2.2;
	int SizeLevel = Max( Max( Max( TWork::StrSetEvent.VConstSizeTopMDL, TWork::StrSetEvent.VConstSizeBottomMDL ),
									  Max( TPack::StrSetEvent.VConstSizeTopMDL, TPack::StrSetEvent.VConstSizeBottomMDL ) ),
								Max( Max( TWork::StrSetEvent.VConstSizeTopGNT, TWork::StrSetEvent.VConstSizeBottomGNT ),
									  Max( TPack::StrSetEvent.VConstSizeTopGNT, TPack::StrSetEvent.VConstSizeBottomGNT ) ) ) * 2.2;

	TSEvents** SRangEvents = (TSEvents**)malloc( sizeof( TSEvents* ) * ( MaxRang + 1 ) );
	for ( i = 0UL; i <= MaxRang; i++ )
		((TSEvents*)*( SRangEvents + i )) = new TSEvents();

	TLPoint lPoint;
	DPacksIterator.Restart();
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		lPoint.y = SizeLevel / 2 + SizeLevel * packIt->GetTmpL();		
		packIt->Move( lPoint );
		((TSEvents*)*( SRangEvents + packIt->GetTmpR() ))->Add( packIt );
		}
	TDWorksIterator DWorksIteratorD( *StrPackEdit->PackView->GetWorks() );	
	while ( DWorksIteratorD )
		{
		TWork* workIt = DWorksIteratorD++;
		lPoint.y = SizeLevel / 2 + SizeLevel * workIt->GetTmpL();		
		workIt->Move( lPoint );
		((TSEvents*)*( SRangEvents + workIt->GetTmpR() ))->Add( workIt );
		}

	unsigned long Level;
	unsigned long LevelTmp;
	bool Move = TRUE;
	while ( Move )
		{
		Move = FALSE;
		for ( i = 1L; i < MaxRang; i++ )
			{
			Level = 0UL;
			TSEventsIterator* EventsIterator = new TSEventsIterator( (TSEvents)**( SRangEvents + i ) );
			while ( *EventsIterator )
				{
				TEvent* eventIt = (*EventsIterator)++;
				LevelTmp = eventIt->GetTmpL();
				if ( LevelTmp <= Level && Level != 0UL )
					{
					LevelTmp = Level + 1;
					Move = TRUE;
					}

				Level = 4294967295UL;
				eventIt->GetExitsConnect( ConnectsTmp );
				while ( !ConnectsTmp->IsEmpty() )
					{
					TConnect* connect = ConnectsTmp->PeekHead();
					ConnectsTmp->DetachAtHead();
					if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventFinish()->GetParent() == StrPackEdit->PackView ) && Level > connect->GetEventFinish()->GetTmpL() )
						Level = connect->GetEventFinish()->GetTmpL();
					}
				if ( Level == 4294967295UL )
					Level = 0UL;
				
				if ( LevelTmp < Level )
					{
					LevelTmp = Level;
					Move = TRUE;
					}
				eventIt->SetTmpL( LevelTmp );	
				Level = LevelTmp;
				}
			delete EventsIterator;
			}
		 
		for ( i = 1L; i < MaxRang; i++ )
			{
			Level = 0UL;
			TSEventsIterator* EventsIterator = new TSEventsIterator( (TSEvents)**( SRangEvents + MaxRang - i ) );
			while ( *EventsIterator )
				{
				TEvent* eventIt = (*EventsIterator)++;
				LevelTmp = eventIt->GetTmpL();
				if ( LevelTmp <= Level && Level != 0UL )
					{
					LevelTmp = Level + 1;
					Move = TRUE;
					}

				Level = 4294967295UL;
				eventIt->GetEntrancesConnect( ConnectsTmp );
				while ( !ConnectsTmp->IsEmpty() )
					{
					TConnect* connect = ConnectsTmp->PeekHead();
					ConnectsTmp->DetachAtHead();
					if ( ( connect->GetPackStart() == connect->GetPackFinish() || connect->GetEventStart()->GetParent() == StrPackEdit->PackView ) && Level > connect->GetEventStart()->GetTmpL() )
						Level = connect->GetEventStart()->GetTmpL();
					}
				if ( Level == 4294967295UL )
					Level = 0UL;

				if ( LevelTmp < Level )
					{
					LevelTmp = Level;
					Move = TRUE;
					}
				eventIt->SetTmpL( LevelTmp );	
				Level = LevelTmp;
				}
			delete EventsIterator;
			}
		}
		 
	DPacksIterator.Restart();
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		lPoint.x = SizeRang / 2 + SizeRang * packIt->GetTmpR();
		lPoint.y = SizeLevel / 2 + SizeLevel * packIt->GetTmpL();		
		packIt->Move( lPoint );
		}                                     
	DWorksIteratorD.Restart();
	while ( DWorksIteratorD )
		{
		TWork* workIt = DWorksIteratorD++;
		lPoint.x = SizeRang / 2 + SizeRang * workIt->GetTmpR();
		lPoint.y = SizeLevel / 2 + SizeLevel * workIt->GetTmpL();		
		workIt->Move( lPoint );
		}

	unsigned long RangStart;
	unsigned long RangFinish;
	TLPoint lPointStart;
	TLPoint lPointFinish;
	TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
	Move = TRUE;
	while ( Move )
		{
		Move = FALSE;
		ConnectsIterator.Restart();
		while ( ConnectsIterator )
			{
			TConnect* connectIt = ConnectsIterator++;
			TEvent* EventStart = connectIt->GetEventStart();
			TEvent* EventFinish = connectIt->GetEventFinish();
			RangStart = EventStart->GetTmpR();
			RangFinish = EventFinish->GetTmpR();
			if ( RangFinish - RangStart > 1 )
				{
				lPointStart = EventStart->GetPoint();
				lPointFinish = EventFinish->GetPoint();
				if ( lPointStart.y == lPointFinish.y )
					{
					for ( i = RangStart + 1; i < RangFinish; i++ )
						{
						TSEventsIterator* EventsIterator = new TSEventsIterator( (TSEvents)**( SRangEvents + i ) );
						while ( *EventsIterator )
							{
							lPoint = (*EventsIterator)++->GetPoint();
							if ( lPointStart.y == lPoint.y )
								{
								lPointStart.y += SizeLevel / 10;
								lPointFinish.y += SizeLevel / 10;
								EventStart->Move( lPointStart );
								EventFinish->Move( lPointFinish );
								Move = TRUE;
								break;
								}
							}
						delete EventsIterator;
						}
					}
				}
			}
		}	

   delete ConnectsTmp;
	for ( i = 0UL; i <= MaxRang; i++ )
		{
		((TSEvents*)*( SRangEvents + i ))->Flush();
		delete ((TSEvents*)*( SRangEvents + i ));
		}
	free( SRangEvents );
		
	Invalidate();	
	IsDirty = TRUE;
	}	

void TMyLayout::Renumber()
	{
	Rang();
   unsigned long i;
	TSEvents** SRangEvents = (TSEvents**)malloc( sizeof( TSEvents* ) * ( MaxRang + 1 ) );
	for (  i = 0UL; i <= MaxRang; i++ )
		((TSEvents*)*( SRangEvents + i )) = new TSEvents();

	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		{
		TPack* packIt = DPacksIterator++;
		((TSEvents*)*( SRangEvents + packIt->GetTmpR() ))->Add( packIt );
		}
	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		{
		TWork* workIt = DWorksIterator++;
		((TSEvents*)*( SRangEvents + workIt->GetTmpR() ))->Add( workIt );
		}
	unsigned long Number = 0UL; 	
	for ( i = 0UL; i <= MaxRang; i++ )
		{
		TSEventsIterator* EventsIterator = new TSEventsIterator( (TSEvents)**( SRangEvents + i ) );
		while ( *EventsIterator )
			(*EventsIterator)++->SetNumber( ++Number );
		delete EventsIterator;
		}
	StrPackEdit->PackView->SetMaxNumber( Number );
	for ( i = 0UL; i <= MaxRang; i++ )
		{
		((TSEvents*)*( SRangEvents + i ))->Flush();
		delete ((TSEvents*)*( SRangEvents + i ));
		}
	free( SRangEvents );
	Invalidate();
	IsDirty = TRUE;
	}

void TMyLayout::Convert()
	{
	HCURSOR hcurSave=::SetCursor(LoadCursor(NULL,IDC_WAIT));
	TConnects* ConnectsTmp = new TConnects();
	TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
	while ( ConnectsIterator )
		ConnectsTmp->Add( ConnectsIterator++ );

	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->ResetConvert();

	DWorksIterator.Restart();
	while ( DWorksIterator )
		DWorksIterator++->SetConvert( ConnectsTmp );

	WriteDB( ConnectsTmp );

	ConnectsTmp->Flush();
	delete ConnectsTmp;
	::SetCursor(hcurSave);
	}


