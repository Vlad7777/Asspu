#include <owl\owlpch.h>
#include <ddeml.h>
			 
#include "layout.h"

unsigned long FindBestTime( TWork* TheWork, unsigned long TheStartTimeFindStart, unsigned long TheFinishTimeFindFinish, TSBitTimes* TheBitTimes );

void TMyLayout::BuildFreeTime()
	{
	unsigned long StartTime = PackMain->GetStartTime();
	unsigned long FinishTime = PackMain->GetFinishTime();
//	unsigned long FinishTime = StartTime;
	TConnects* ConnectsTmp = new TConnects();

	int Tmp;

	TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->ResetStartTime( StartTime, ConnectsTmp );
	while ( !ConnectsTmp->IsEmpty() )
		{
		TConnect* ConnectTmp = ConnectsTmp->PeekHead();
		ConnectsTmp->DetachAtHead();
		ConnectTmp->GetWorkFinish()->MoveStartTime( ConnectTmp->GetWorkStart()->GetFinishTime(), ConnectsTmp );
		}
	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* work = DWorksIterator++;
		work->SetTmpL( work->GetStartTime() );
//		if ( FinishTime < work->GetFinishTime() )
//			FinishTime = work->GetFinishTime();
		}

	DWorksIterator.Restart();
	while ( DWorksIterator )
		DWorksIterator++->ResetFinishTime( FinishTime, ConnectsTmp );
	while ( !ConnectsTmp->IsEmpty() )
		{
		TConnect* ConnectTmp = ConnectsTmp->PeekHead();
		ConnectsTmp->DetachAtHead();
		ConnectTmp->GetWorkStart()->MoveFinishTime( ConnectTmp->GetWorkFinish()->GetStartTime(), ConnectsTmp );
		}
	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* work = DWorksIterator++;
		work->SetTmpR( work->GetFinishTime() );
		}

	delete ConnectsTmp;
	}
		
void TMyLayout::BuildPlan()
	{
	if ( AllWindow[2].BASScreen )
		PackMain->ResetBitTimes();

	TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->SetCritical( CRITICAL_NOT );

	BuildFreeTime();

	TSBitTimes* BitTimesMain = new TSBitTimes();
	TSBitTimes* BitTimesMatWait = new TSBitTimes();
	TSBitTimes* BitTimesTmp = new TSBitTimes();
	TDWorks* WorksMatWait = new TDWorks();	

	DWorksIterator.Restart();
	while ( DWorksIterator )
		{
		TWork* work = DWorksIterator++;
		if ( work->GetStartTime() <= work->GetTmpL() + TEvent::StrSetScale.CriticalTime ) 
			{
			work->SetStartTime( work->GetTmpL() );
			work->ArrangeStartTime();
			work->SetCritical( CRITICAL_MAIN );
			work->GetBitTimes( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
			BitTimesMain->Sum( BitTimesTmp );
			BitTimesTmp->Flush( TRUE );
			}
		else
			{
			WorksMatWait->Add( work );
			work->GetBitTimesMatWait( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
			BitTimesMatWait->Sum( BitTimesTmp );
			BitTimesTmp->Flush( TRUE );
			}
		}
	BitTimesMain->Sum( BitTimesMatWait );

	unsigned long MaxResourceSumOld;
	unsigned long MaxResourceSum;
	unsigned long StartTimeMaxResourceSum;
	unsigned long FinishTimeMaxResourceSum;
	TWork* WorkCritical;
	TWork* WorkTmp;
	TDWorks* WorksTmp = new TDWorks();
	TSBitTimesIterator* BitTimesMainIterator;
	TDWorksIterator* WorksMatWaitIterator;

//	int in = 1;  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		((TSBitTimes*)*( PackMain->BitTimes + in++ ))->Sum( BitTimesMain );  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


	while ( !WorksMatWait->IsEmpty() )
		{		
		BitTimesMainIterator = new TSBitTimesIterator( *BitTimesMain );
		WorksMatWaitIterator = new TDWorksIterator( *WorksMatWait );
		MaxResourceSumOld = 4294967295UL;
		while ( TRUE )
			{
			MaxResourceSum = 0UL;
			BitTimesMainIterator->Restart();
			while ( *BitTimesMainIterator )
				{
				TBitTime* BitTimeTmp = (*BitTimesMainIterator)++;
				if ( MaxResourceSum < BitTimeTmp->GetResource() && MaxResourceSumOld > BitTimeTmp->GetResource() )
					MaxResourceSum = BitTimeTmp->GetResource();
				}	

			if ( !MaxResourceSum )
				break;
			
			BitTimesMainIterator->Restart();
			while ( *BitTimesMainIterator )
				{
				TBitTime* BitTimeTmp = (*BitTimesMainIterator)++;
				if ( MaxResourceSum == BitTimeTmp->GetResource() )
					{
					StartTimeMaxResourceSum = BitTimeTmp->GetTime();
					BitTimeTmp = (*BitTimesMainIterator)++;
					while ( MaxResourceSum == BitTimeTmp->GetResource() )
						BitTimeTmp = (*BitTimesMainIterator)++;
					FinishTimeMaxResourceSum = BitTimeTmp->GetTime();

					WorksMatWaitIterator->Restart();
					while ( *WorksMatWaitIterator )
						{
						TWork* workIt = (*WorksMatWaitIterator)++;
						if ( StartTimeMaxResourceSum == workIt->GetTmpL() )
							WorksTmp->Add( workIt );
						}
					if ( WorksTmp->IsEmpty() )
						{
						WorksMatWaitIterator->Restart();
						while ( *WorksMatWaitIterator )
							{
							TWork* workIt = (*WorksMatWaitIterator)++;
							if ( FinishTimeMaxResourceSum == workIt->GetTmpR() )
								WorksTmp->Add( workIt );
							}
						}
					if ( WorksTmp->IsEmpty() )
						{
						WorksMatWaitIterator->Restart();
						while ( *WorksMatWaitIterator )
							{
							TWork* workIt = (*WorksMatWaitIterator)++;
							if ( ( StartTimeMaxResourceSum > workIt->GetTmpL() && StartTimeMaxResourceSum < workIt->GetTmpR() && FinishTimeMaxResourceSum > workIt->GetTmpL() && FinishTimeMaxResourceSum < workIt->GetTmpR() ) ||
								  ( StartTimeMaxResourceSum < workIt->GetTmpL() && StartTimeMaxResourceSum < workIt->GetTmpR() && FinishTimeMaxResourceSum > workIt->GetTmpL() && FinishTimeMaxResourceSum > workIt->GetTmpR() ) ||
								  ( StartTimeMaxResourceSum < workIt->GetTmpL() && StartTimeMaxResourceSum < workIt->GetTmpR() && FinishTimeMaxResourceSum > workIt->GetTmpL() && FinishTimeMaxResourceSum < workIt->GetTmpR() ) ||
								  ( StartTimeMaxResourceSum > workIt->GetTmpL() && StartTimeMaxResourceSum < workIt->GetTmpR() && FinishTimeMaxResourceSum > workIt->GetTmpL() && FinishTimeMaxResourceSum > workIt->GetTmpR() ) )
								WorksTmp->Add( workIt );
							}
						}						
					if ( !WorksTmp->IsEmpty() )
						break;
					}
				}
			if ( !WorksTmp->IsEmpty() )
				break;
			MaxResourceSumOld = MaxResourceSum;
			}

		delete BitTimesMainIterator;
		delete WorksMatWaitIterator;


		if ( !WorksTmp->IsEmpty() )
			{
			WorkCritical = WorksTmp->PeekHead();
			WorksTmp->DetachAtHead();
			if ( !WorksTmp->IsEmpty() )
				{
				WorkTmp = WorksTmp->PeekHead();
				WorksTmp->DetachAtHead();
				if ( WorkCritical->GetResources()->GetResource( TEvent::StrSetScale.QuantityResource ) < WorkTmp->GetResources()->GetResource( TEvent::StrSetScale.QuantityResource ) )
					WorkCritical = WorkTmp;
				}
			}
		else
			WorkCritical = WorksMatWait->PeekHead();

		WorkCritical->GetBitTimesMatWait( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
		BitTimesMain->Sub( BitTimesTmp );
		BitTimesMatWait->Sub( BitTimesTmp );
		BitTimesTmp->Flush( TRUE );

		unsigned long BestStartTime = FindBestTime( WorkCritical, WorkCritical->GetTmpL(), WorkCritical->GetTmpR(), BitTimesMain );

		if ( BestStartTime )
			{
			WorkCritical->SetStartTime( BestStartTime );
			WorkCritical->SetCritical( CRITICAL_BEST_TIME );
			}
		else	
			{
			WorkCritical->SetStartTime( WorkCritical->GetTmpL() );
			WorkCritical->SetCritical( CRITICAL_FIX_TIME );
			}
		WorkCritical->ArrangeStartTime();			

		BitTimesMain->Sub( BitTimesMatWait );
		BitTimesMatWait->Flush( TRUE );
		WorkCritical->GetBitTimes( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
		BitTimesMain->Sum( BitTimesTmp );
		BitTimesTmp->Flush( TRUE );
		WorksMatWait->Detach( WorkCritical );

		BuildFreeTime();
		WorksMatWaitIterator = new TDWorksIterator( *WorksMatWait );
		while ( *WorksMatWaitIterator )
			{
			TWork* work = (*WorksMatWaitIterator)++;
			if ( work->GetStartTime() == work->GetTmpL() )
				{
				work->SetCritical( CRITICAL_FREEZE_TIME );
				work->GetBitTimes( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
				BitTimesMain->Sum( BitTimesTmp );
				BitTimesTmp->Flush( TRUE );
				WorksTmp->Add( work );
				}
			else
				{
				work->GetBitTimesMatWait( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
				BitTimesMatWait->Sum( BitTimesTmp );
				BitTimesTmp->Flush( TRUE );
				}
			}
		delete WorksMatWaitIterator;
		while ( !WorksTmp->IsEmpty() )
			{		
			WorksMatWait->Detach( WorksTmp->PeekHead() );
			WorksTmp->DetachAtHead();
			}
		BitTimesMain->Sum( BitTimesMatWait );	
		}
	delete BitTimesTmp;
	delete BitTimesMain;
	delete BitTimesMatWait;
	delete WorksMatWait;
	delete WorksTmp;

	PackMain->CorrectTime();
	Invalidate();	
	IsDirty = TRUE;
	if ( AllWindow[2].BASScreen )
		PackMain->BuildBitTimes();
	}



void TMyLayout::BuildResource()
	{
/*
	TDWorksIterator DWorksIterator( *PackMain->GetWorks() );
	while ( DWorksIterator )
		{
		TWork* work = DWorksIterator++;
		work->SetResourceWork( work->GetOptResourceWork() );
		work->BuildDuration();
		}
	bool Move = TRUE;
	while ( Move )
		{
		Move = FALSE;
		BuildFreeTime();
		BuildCritical();
		DWorksIterator.Restart();
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->GetCritical() == CRITICAL_MAIN && work->GetResourceWork() != work->GetMaxResourceWork() )
				{
				Move = TRUE;
				work->SetResourceWork( work->GetMaxResourceWork() );
				work->BuildDuration();
				}
			}
		}
	PackMain->CorrectTime();
	Invalidate();	
	IsDirty = TRUE;
  */
	}




class TCutTime
	{
	protected:
		unsigned long StartTime;
		unsigned long FinishTime;
		unsigned long MaxResource;
	public:
		TCutTime( unsigned long TheStartTime, unsigned long TheFinishTime )
			{ StartTime = TheStartTime, FinishTime = TheFinishTime, MaxResource = 0; }
		unsigned long GetStartTime()
			{ return StartTime; }
		unsigned long GetFinishTime()
			{ return FinishTime; }
		void SetMaxResource( unsigned long TheMaxResource )
			{ MaxResource = TheMaxResource; }
		unsigned long GetMaxResource()
			{ return MaxResource; }
			
		bool operator == ( const TCutTime& other ) const
			{ return &other == this; }
		bool operator < ( const TCutTime& other ) const
			{ return StartTime < other.StartTime; }
	};

typedef TISListImp<TCutTime> TSCutTimes;
typedef TIDoubleListImp<TCutTime> TDCutTimes;
typedef TIDoubleListIteratorImp<TCutTime> TDCutTimesIterator;
		 

unsigned long FindBestTime( TWork* TheWork, unsigned long TheStartTimeFindStart, unsigned long TheFinishTimeFindFinish, TSBitTimes* TheBitTimes )
	{
	unsigned long BestStartTime = 0;
	unsigned long StartTimeFindStart = TheStartTimeFindStart;
	unsigned long FinishTimeFindFinish = TheFinishTimeFindFinish;
	TSCutTimes* SCutTimes = new TSCutTimes(); 

	TheWork->SetFinishTime( FinishTimeFindFinish );
	TheWork->ArrangeFinishTime();
	SCutTimes->Add( new TCutTime( TheWork->GetStartTime(), TheWork->GetFinishTime() ) );
	unsigned long FinishTimeFindStart = TheWork->GetStartTime();

	TheWork->SetStartTime( StartTimeFindStart );
	TheWork->ArrangeStartTime();
	SCutTimes->Add( new TCutTime( TheWork->GetStartTime(), TheWork->GetFinishTime() ) );
	unsigned long StartTimeFindFinish = TheWork->GetFinishTime();

	TSBitTimesIterator* BitTimesIterator = new TSBitTimesIterator( *TheBitTimes );
	while ( *BitTimesIterator )
		{
		TBitTime* BitTimeTmp = (*BitTimesIterator)++;
		unsigned long TimeTmp = BitTimeTmp->GetTime();
		if ( TimeTmp > StartTimeFindStart && TimeTmp < FinishTimeFindStart )
			{
			TheWork->SetStartTime( TimeTmp );
			TheWork->ArrangeStartTime();
			SCutTimes->Add( new TCutTime( TheWork->GetStartTime(), TheWork->GetFinishTime() ) );
			}						
		if ( TimeTmp > StartTimeFindFinish && TimeTmp < FinishTimeFindFinish )
			{
			TheWork->SetFinishTime( TimeTmp );
			TheWork->ArrangeFinishTime();
			SCutTimes->Add( new TCutTime( TheWork->GetStartTime(), TheWork->GetFinishTime() ) );
			}
		}

	unsigned long ShiftStart = TheWork->GetShiftStart();
	unsigned long ShiftFinish = TheWork->GetShiftFinish();
	unsigned long TimeTmp = TEvent::StrSetScale.Day * ( StartTimeFindStart / TEvent::StrSetScale.Day + 1 ) + ShiftStart;
	while ( TimeTmp < FinishTimeFindStart )
		{
		TheWork->SetStartTime( TimeTmp );
		TheWork->ArrangeStartTime();
		SCutTimes->Add( new TCutTime( TheWork->GetStartTime(), TheWork->GetFinishTime() ) );
		TimeTmp += TEvent::StrSetScale.Day;
		}
					
	unsigned long OldTimeTmp = 0;
	TDCutTimes* CutTimes = new TDCutTimes(); 
	while ( !SCutTimes->IsEmpty() )
		{		
		TCutTime* cutTime = SCutTimes->PeekHead();
		if ( OldTimeTmp != cutTime->GetStartTime() )
			{
			SCutTimes->DetachAtHead();
			OldTimeTmp = cutTime->GetStartTime();
			CutTimes->AddAtTail( cutTime );
			}
		else
			SCutTimes->DetachAtHead( TRUE );
		}
	delete SCutTimes;

	unsigned long OldResourceTmp = 0;
	OldTimeTmp = 0;
	TDCutTimesIterator* CutTimesIterator = new TDCutTimesIterator( *CutTimes );
	BitTimesIterator->Restart();
	while ( *BitTimesIterator )
		{
		TBitTime* BitTimeTmp = (*BitTimesIterator)++;
		unsigned long TimeTmp = BitTimeTmp->GetTime();
		if ( ( ( StartTimeFindStart >= OldTimeTmp && StartTimeFindStart < TimeTmp ) ||
				 ( StartTimeFindStart < OldTimeTmp && FinishTimeFindFinish > OldTimeTmp ) ) &&
			  ( ( ShiftStart >= OldTimeTmp % TEvent::StrSetScale.Day && ShiftStart < TimeTmp % TEvent::StrSetScale.Day ) ||
				 ( ShiftStart < OldTimeTmp % TEvent::StrSetScale.Day && ShiftFinish > OldTimeTmp % TEvent::StrSetScale.Day ) ) )
			{
			CutTimesIterator->Restart();
			while ( *CutTimesIterator )
				{
				TCutTime* cutTime = (*CutTimesIterator)++;
				if ( cutTime->GetMaxResource() < OldResourceTmp &&
					  ( ( cutTime->GetStartTime() >= OldTimeTmp && cutTime->GetStartTime() < TimeTmp ) ||
						 ( cutTime->GetStartTime() < OldTimeTmp && cutTime->GetFinishTime() > OldTimeTmp ) ) )
					cutTime->SetMaxResource( OldResourceTmp );
				}
			}	
		OldResourceTmp = BitTimeTmp->GetResource();
		OldTimeTmp = BitTimeTmp->GetTime();
		}

	OldResourceTmp = 0UL;
	unsigned long MinResource = 4294967295UL;
	CutTimesIterator->Restart();
	while ( *CutTimesIterator )
		{
		TCutTime* cutTime = (*CutTimesIterator)++;
		if ( OldResourceTmp < cutTime->GetMaxResource() )
			OldResourceTmp = cutTime->GetMaxResource();
		if ( MinResource > cutTime->GetMaxResource() )
			{
			MinResource = cutTime->GetMaxResource();
			BestStartTime = cutTime->GetStartTime();
			}
		}

	if ( MinResource == OldResourceTmp )
		BestStartTime = 0;

	delete CutTimesIterator;
	delete CutTimes;
	return BestStartTime;
	}
	
