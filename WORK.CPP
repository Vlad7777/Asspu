#include <time.h>
#include <owl\owlpch.h>
#include <ddeml.h>


#include "layout.h"
#include "work.h"

TMyResource& TMyResource::operator = ( TMyResource& other )
	{
	IndexResource = other.IndexResource;
	ResourceMin = other.ResourceMin;	
	ResourceOpt = other.ResourceOpt;	
	ResourceMax = other.ResourceMax;	
	Resource = other.Resource;	
	Volume = other.Volume;			
	return *this;
	}

void TSResources::SetResourceMin()
	{
	TSResourcesIterator ResourcesIterator( *this );
	while ( ResourcesIterator )
		ResourcesIterator++->SetResourceMin();
	}
	
void TSResources::SetResourceOpt()
	{
	TSResourcesIterator ResourcesIterator( *this );
	while ( ResourcesIterator )
		ResourcesIterator++->SetResourceOpt();
	}
	
void TSResources::SetResourceMax()
	{
	TSResourcesIterator ResourcesIterator( *this );
	while ( ResourcesIterator )
		ResourcesIterator++->SetResourceMax();
	}

unsigned long TSResources::GetDuration()
	{
	unsigned long Duration = 0;
	TSResourcesIterator ResourcesIterator( *this );
	while ( ResourcesIterator )
		{
		TMyResource* ResourceTmp = ResourcesIterator++;
		if ( Duration < ( unsigned long )ResourceTmp->GetVolume() / ResourceTmp->GetResource() )
			Duration = ( unsigned long )ResourceTmp->GetVolume() / ResourceTmp->GetResource();
		}
	return Duration;
	}

unsigned long TSResources::GetResource( int TheIndexResource )
	{
	unsigned long Resource = 0;
	TSResourcesIterator ResourcesIterator( *this );
	while ( ResourcesIterator )
		{
		TMyResource* ResourceTmp = ResourcesIterator++;
		if ( TheIndexResource == TEvent::StrSetScale.QuantityResource )
			Resource += ResourceTmp->GetResource();
		if ( TheIndexResource == ResourceTmp->GetIndexResource() )
			Resource = ResourceTmp->GetResource();
		}
	return Resource;
	}

TSResources& TSResources::operator = ( TSResources& other )
	{
	Flush( TRUE );
	TSResourcesIterator ResourcesIterator( other );
	while ( ResourcesIterator )
		{
		TMyResource* resource = new TMyResource();
		*resource = *(ResourcesIterator++);
		Add( resource );
		}
	return *this;
	}

opstream& operator << ( opstream& os, const TSResources& resources )
	{
	os << resources.GetItemsInContainer();
	TSResourcesIterator ResourcesIterator( resources );
	while ( ResourcesIterator )
		os << *(ResourcesIterator++);
	return os;
	}

ipstream& operator >> ( ipstream& is, TSResources& resources )
	{
	unsigned QuantityResources;
	is >> QuantityResources;
	while ( QuantityResources-- )
		{
		TMyResource* resource = new TMyResource();
		is >> *resource;
		resources.Add( resource );
		}
	return is;
	}

TCutOffTime& TCutOffTime::operator = ( TCutOffTime& other )
	{
	StartTime = other.StartTime;
	FinishTime = other.FinishTime;
	return *this;
	}

void TDCutOffTimes::CorrectBitTimes( TSBitTimes* TheBitTimes )
	{
	TDBitTimes* DBitTimes = new TDBitTimes();
	TSBitTimesIterator BitTimesIterator( *TheBitTimes );
	while ( BitTimesIterator )
		DBitTimes->AddAtTail( BitTimesIterator++ );

	unsigned long ResourceOld = 0UL;
	unsigned long TimeOld = 0UL;
	TBitTime* BitTime = DBitTimes->PeekHead();
	TDCutOffTimesIterator CutOffTimesIterator( *this );
	while ( CutOffTimesIterator )
		{
		TCutOffTime* CutOffTime = CutOffTimesIterator++;
		if ( CutOffTime->GetStartTime() == BitTime->GetTime() )
			{
			ResourceOld = BitTime->GetResource();
			DBitTimes->DetachAtHead();
			BitTime->DelResource( BitTime->GetResource() );
			}
		if ( CutOffTime->GetStartTime() > BitTime->GetTime() )
			while ( !DBitTimes->IsEmpty() )
				{
				BitTime = DBitTimes->PeekHead();
				if ( CutOffTime->GetFinishTime() > BitTime->GetTime() )
					{
					ResourceOld = BitTime->GetResource();
					DBitTimes->DetachAtHead();
					if ( CutOffTime->GetStartTime() == BitTime->GetTime() )
						BitTime->DelResource( BitTime->GetResource() );
					if ( CutOffTime->GetStartTime() > TimeOld && CutOffTime->GetStartTime() < BitTime->GetTime() )
						TheBitTimes->Add( new TBitTime( CutOffTime->GetStartTime(), 0 ) );
					TimeOld = BitTime->GetTime();
					if ( CutOffTime->GetStartTime() < BitTime->GetTime() && CutOffTime->GetFinishTime() > BitTime->GetTime() )
						TheBitTimes->Detach( BitTime, TRUE );
					}
				else
					{
					if ( CutOffTime->GetFinishTime() < BitTime->GetTime() )
						TheBitTimes->Add( new TBitTime( CutOffTime->GetFinishTime(), ResourceOld ) );
					break;
					}
				}
		if ( DBitTimes->IsEmpty() )
			break;
		}
	delete DBitTimes;
	}

TDCutOffTimes& TDCutOffTimes::operator = ( TDCutOffTimes& other )
	{
	Flush( TRUE );
	TDCutOffTimesIterator CutOffTimesIterator( other );
	while ( CutOffTimesIterator )
		{
		TCutOffTime* CutOffTime = CutOffTimesIterator++;
		AddAtTail( new TCutOffTime( CutOffTime->GetStartTime(), CutOffTime->GetFinishTime() ) );
		}
	return *this;
	}

opstream& operator << ( opstream& os, const TDCutOffTimes& cutOffTimes )
	{
	os << cutOffTimes.GetItemsInContainer();
	TDCutOffTimesIterator CutOffTimesIterator( cutOffTimes );
	while ( CutOffTimesIterator )
		os << *(CutOffTimesIterator++);
	return os;
	}

ipstream& operator >> ( ipstream& is, TDCutOffTimes& cutOffTimes )
	{
	unsigned long Quantity;
	is >> Quantity;
	while ( Quantity-- )
		{
		TCutOffTime* cutOffTime = new TCutOffTime();
		is >> *cutOffTime;
		cutOffTimes.AddAtTail( cutOffTime );
		}
	return is; 
	}

	
STR_SET_EVENT TWork::StrSetEvent = { 75, 45, 30, 35, 5, { 0, 1, 0, 0L }, { 0, 1, 0, 255L }, 12632256L, 65280L, 8388608L, 0 };
STR_SET_FIELD TWork::StrSetField = { "NAME", "NUMBER", "START_T", "FINISH_T", "DURATION",	"PERCENT", "LIM_S_S", "LIM_F_S",	"OPT_S_S", "OPT_F_S", "START_S", "FINISH_S",	"CRITICAL",	"START_K", "FINISH_K", "NAME_P", "NUMBER_P", "NUMBER_P_P", "INDEX_R", "MIN_R", "OPT_R", "MAX_R", "RESOURCE", "VOLUME" };

TWork::TWork()
	{
	Name = strnewdup( "" );
	lPoint = TPoint( 0L, 0L );
	Number = 0L;
	ParentPack = NULL;
	Entrances = new TConnects();
	Exits = new TConnects();

	StartTime = 0UL;
	FinishTime = 0UL;

	IndexShift = 0;
	IndexBrigade = 0;
	IndexPack = 0;
	ShiftStart = 0UL;
	ShiftFinish = TEvent::StrSetScale.Day;

	Critical = CRITICAL_NOT;
	Duration = 0UL;
	Percent = 0;
	Resources = new TSResources();
	WeekEnd = FALSE;
	}

TWork::~TWork()
	{
	Entrances->Flush();    // true ???????????????????
	Exits->Flush();        //  true ???????????????????????
	delete Entrances;
	delete Exits;
//	free(	Name ); // CG !!!
   delete[] Name;
   Resources->Flush(true);
	delete Resources;
	}

void TWork::BuildReserveTime()
	{
	unsigned long MaxFinishTime = 0UL;
	unsigned long MinStartTime = 4294967295UL;
	TConnectsIterator EntrancesIterator( *Entrances );
	while ( EntrancesIterator )
		{
		TConnect* connect = EntrancesIterator++;
		if ( MaxFinishTime < connect->GetWorkStart()->GetFinishTime() )
			MaxFinishTime = connect->GetWorkStart()->GetFinishTime();
		}
	TConnectsIterator ExitsIterator( *Exits );
	while ( ExitsIterator )
		{
		TConnect* connect = ExitsIterator++;
		if ( MinStartTime > connect->GetWorkFinish()->GetStartTime() )
			MinStartTime = connect->GetWorkFinish()->GetStartTime();
		}

	unsigned long DeleteTmp;
	if ( ShiftStart < ShiftFinish )
		DeleteTmp = StrSetScale.Day - ShiftFinish + ShiftStart;
	else
		DeleteTmp = ShiftStart - ShiftFinish;

	if ( !Exits->IsEmpty() &&
		  ( FinishTime - StrSetScale.Day * ( FinishTime  / StrSetScale.Day ) != ShiftFinish ||
			 FinishTime + DeleteTmp < MinStartTime ) )
		TmpR = MinStartTime - FinishTime;
	else
		TmpR = 0;

	if ( !Entrances->IsEmpty() &&
		  ( StartTime - StrSetScale.Day * ( StartTime / StrSetScale.Day ) != ShiftStart || 
			 StartTime - DeleteTmp > MaxFinishTime ) )
		TmpL = StartTime - MaxFinishTime;
	else
		TmpL = 0;
	}

void TWork::BuildWork()
	{
	if ( !Resources->IsEmpty() )
		Duration = Resources->GetDuration();
	if ( Duration % StrSetScale.StepTime > 0 )
		Duration = ( Duration / StrSetScale.StepTime + 1 ) * StrSetScale.StepTime;
	ArrangeStartTime();
	}

void TWork::SetChec( TConnects* Check )
	{
	if ( Tmp == 0 )
		GetExitsConnect( Check );
	Tmp++;	
	}

void TWork::ResetConvert()
	{
	TmpL = 2 * Number - 1;
	TmpR = 2 * Number;
	SetL( FALSE );
	SetR( FALSE );
	}

void TWork::SetConvert( TConnects* Check )
	{
	unsigned long LocalTmpL = TmpL;
	unsigned long LocalTmpR = TmpR;
	if (	Entrances->GetItemsInContainer() == 1 && !GetL() )
		{
		TConnect* connect = Entrances->PeekHead();
		TWork* work = connect->GetWorkStart();
		bool Happy = TRUE;
		LocalTmpL = work->GetTmpR();
		if ( work->GetR() )
			{
			TConnects* Connects = new TConnects();
			work->GetExitsConnect( Connects );
			Connects->Detach( connect );			
			while ( !Connects->IsEmpty() )
				{
				TWork* workD = Connects->PeekHead()->GetWorkFinish();
				Connects->DetachAtHead();
				if ( Happy && LocalTmpL == workD->GetTmpL() && LocalTmpR == workD->GetTmpR() )
					Happy = FALSE;
				}
			delete Connects;
			}
		if ( Happy )
			{
			TmpL = LocalTmpL;
			Check->Detach( connect );
			work->SetR();
			SetL();
			}
		}
		
	if ( Exits->GetItemsInContainer() == 1 && !GetR() )
		{
		TConnect* connect = Exits->PeekHead();
		TWork* work = connect->GetWorkFinish();
		bool Happy = TRUE;
		LocalTmpR = work->GetTmpL();
		if ( work->GetL() )
			{
			TConnects* Connects = new TConnects();
			work->GetEntrancesConnect( Connects );
			Connects->Detach( connect );			
			while ( !Connects->IsEmpty() )
				{
				TWork* workD = Connects->PeekHead()->GetWorkStart();
				Connects->DetachAtHead();
				if ( Happy && LocalTmpL == workD->GetTmpL() && LocalTmpR == workD->GetTmpR() )
					Happy = FALSE;
				}
			delete Connects;
			}
		if ( Happy )
			{
			TmpR = LocalTmpR;
			Check->Detach( connect );
			work->SetL();
			SetR();
			}
		}
	}


void TWork::BuildTimes( TSBitTimes* TheBitTimes, unsigned long TheStartTime, unsigned long TheFinishTime, unsigned long TheResource )
	{
	unsigned long StartTimeTmp = TheStartTime;
	unsigned long FinishTimeTmp;
	unsigned long ShiftTmp;
	unsigned long DeleteTmp;

	if ( ShiftStart < ShiftFinish )
		{
		ShiftTmp = ShiftFinish - ShiftStart;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( StartTimeTmp % StrSetScale.Day < ShiftStart )
			StartTimeTmp += ShiftStart - StartTimeTmp % StrSetScale.Day;
		if ( StartTimeTmp % StrSetScale.Day >= ShiftFinish )
			StartTimeTmp += StrSetScale.Day - StartTimeTmp % StrSetScale.Day + ShiftStart;
		}
	else
		{
		ShiftTmp = StrSetScale.Day - ShiftStart + ShiftFinish;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( StartTimeTmp % StrSetScale.Day < ShiftStart && StartTimeTmp % StrSetScale.Day >= ShiftFinish )
			StartTimeTmp += ShiftStart - StartTimeTmp % StrSetScale.Day;
		}
	FinishTimeTmp = StartTimeTmp;
	if ( StartTimeTmp % StrSetScale.Day != ShiftStart )
		{
		if ( StartTimeTmp % StrSetScale.Day < ShiftFinish )
			FinishTimeTmp += ShiftFinish - StartTimeTmp % StrSetScale.Day;
		else
			FinishTimeTmp += StrSetScale.Day - StartTimeTmp % StrSetScale.Day + ShiftFinish;
		}
	else
		FinishTimeTmp += ShiftTmp;

	if ( FinishTimeTmp > TheFinishTime )
		FinishTimeTmp = TheFinishTime;

	TBitTime* BitTimeFinish = NULL;
	while ( TRUE )
		{
		if ( BitTimeFinish == NULL || StartTimeTmp != BitTimeFinish->GetTime() )
			TheBitTimes->Add( new TBitTime( StartTimeTmp, TheResource ) );
		else
			{
			BitTimeFinish->AddResource( TheResource );
			BitTimeFinish->AddQuantity();
			}
		BitTimeFinish = new TBitTime( FinishTimeTmp, 0L ); 
		TheBitTimes->Add( BitTimeFinish );
		StartTimeTmp = FinishTimeTmp + DeleteTmp;
		if ( FinishTimeTmp == TheFinishTime || StartTimeTmp == TheFinishTime )
			break;
		FinishTimeTmp = StartTimeTmp + ShiftTmp;
		if ( FinishTimeTmp > TheFinishTime )
			FinishTimeTmp = TheFinishTime;
		}
	}

void TWork::GetBitTimesMatWait( TSBitTimes* TheBitTimes, int TheIndexResource )
	{
	unsigned long Resource = Resources->GetResource( TheIndexResource );
	if ( Resource )
		{
		unsigned long ShiftTmp;
		unsigned long DeleteTmp;
		unsigned long DurationMatWait;
		unsigned long ResourceMatWait;

		if ( ShiftStart < ShiftFinish )
			ShiftTmp = ShiftFinish - ShiftStart;
		else
			ShiftTmp = StrSetScale.Day - ShiftStart + ShiftFinish;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		DurationMatWait = ( ( TmpR - TmpL ) / StrSetScale.Day ) * ShiftTmp + ( TmpR - TmpL ) % StrSetScale.Day;
		if ( ( TmpR - TmpL ) % StrSetScale.Day > ShiftTmp )
			DurationMatWait -= DeleteTmp;
		ResourceMatWait = ( Resource * Duration ) / DurationMatWait;
		if ( ResourceMatWait )
			BuildTimes( TheBitTimes, TmpL, TmpR, ResourceMatWait );  

		if ( WeekEnd && ParentPack && !TheBitTimes->IsEmpty() )
			ParentPack->GetCutOffTimes()->CorrectBitTimes( TheBitTimes );
		}
	}

bool TWork::CorrectTime( unsigned long TheTime, TConnects* Check )
	{
	Tmp--;
	if ( StartTime < TheTime )
		{
		if ( !(bool)TmpL && ParentPack )
			ParentPack->DetachBitTimes( this );
		TmpL = (unsigned long)TRUE;	
		StartTime = TheTime;
		}
	if ( Tmp == 0 )
		{
		GetExitsConnect( Check );		
		if ( (bool)TmpL )
			{
			ArrangeStartTime();	
			if ( ParentPack )
				ParentPack->AddBitTimes( this );
			return TRUE;
			}	
		}
	return FALSE;
	}

void TWork::MoveStartTime( unsigned long TheStartTime, TConnects* Check )
	{
	Tmp--;
	if ( Percent == 0 && Critical == CRITICAL_NOT && StartTime < TheStartTime )
		StartTime = TheStartTime ;
	if ( Tmp == 0 )
		{
		if ( Percent == 0 && Critical == CRITICAL_NOT )
			ArrangeStartTime();
		GetExitsConnect( Check );
		}
	}

void TWork::MoveFinishTime( unsigned long TheFinishTime, TConnects* Check )
	{
	Tmp--;
	if ( Percent == 0 && Critical == CRITICAL_NOT && FinishTime > TheFinishTime )
		FinishTime = TheFinishTime;
	if ( Tmp == 0 )
		{
		if ( Percent == 0 && Critical == CRITICAL_NOT )
			ArrangeFinishTime();
		GetEntrancesConnect( Check );
		}
	}

void TWork::GetBitTimes( TSBitTimes* TheBitTimes, int TheIndexResource )
	{ 
	unsigned long Resource = Resources->GetResource( TheIndexResource );
	if ( Resource )
		{
		BuildTimes( TheBitTimes, StartTime, FinishTime, Resource );

		if ( WeekEnd && ParentPack && !TheBitTimes->IsEmpty() )
			ParentPack->GetCutOffTimes()->CorrectBitTimes( TheBitTimes );
		}
	}

unsigned long TWork::GetCarentTime()
	{ 
	if ( Percent == 0 )
		return StartTime;
	if ( Percent == 100 )
		return FinishTime;
	unsigned long FinishTimeTmp = FinishTime;
	unsigned long DurationTmp = Duration;
	Duration *= ( ((float)Percent) / 100 );
	ArrangeStartTime();
	unsigned long CarentTime = FinishTime;
	FinishTime = FinishTimeTmp;
	Duration = DurationTmp;
	return CarentTime;
	}

void TWork::ResetStartTime( unsigned long TheStartTime, TConnects* Check )
	{
	Tmp = GetEntrancesConnect();
	if ( Percent == 0 && Critical == CRITICAL_NOT )
		StartTime = TheStartTime;
	if ( Tmp == 0 )
		{
		if ( Percent == 0 && Critical == CRITICAL_NOT )
			ArrangeStartTime();
		GetExitsConnect( Check );
		}
	}

void TWork::ResetFinishTime( unsigned long TheFinishTime, TConnects* Check )
	{
	Tmp = GetExitsConnect();
	if ( Percent == 0 && Critical == CRITICAL_NOT )
		FinishTime = TheFinishTime;
	if ( Tmp == 0 )
		{
		if ( Percent == 0 && Critical == CRITICAL_NOT )
			ArrangeFinishTime();
		GetEntrancesConnect( Check );
		}
	}

void TWork::ArrangeStartTime()
	{
	TDCutOffTimesIterator* CutOffTimesIterator;
	if ( WeekEnd && ParentPack )
		{
		CutOffTimesIterator = new TDCutOffTimesIterator( *ParentPack->GetCutOffTimes() );
		while ( *CutOffTimesIterator )
			{
			TCutOffTime* CutOffTime = (*CutOffTimesIterator)++;
			if ( CutOffTime->GetStartTime() <= StartTime && CutOffTime->GetFinishTime() > StartTime )
				StartTime = CutOffTime->GetFinishTime();
			}
		}
  
	unsigned long ShiftTmp;
	unsigned long DeleteTmp;
	unsigned long DurationTmp = Duration;
	if ( ShiftStart < ShiftFinish )
		{
		ShiftTmp = ShiftFinish - ShiftStart;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( StartTime % StrSetScale.Day < ShiftStart )
			StartTime += ShiftStart - StartTime % StrSetScale.Day;
		if ( StartTime % StrSetScale.Day >= ShiftFinish )
			StartTime += StrSetScale.Day - StartTime % StrSetScale.Day + ShiftStart;
		FinishTime = StartTime;
		if ( StartTime % StrSetScale.Day != ShiftStart )
			{
			DurationTmp += StartTime % StrSetScale.Day - ShiftStart;
			FinishTime -= StartTime % StrSetScale.Day - ShiftStart;
			}
		}
	else
		{
		ShiftTmp = StrSetScale.Day - ShiftStart + ShiftFinish;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( StartTime % StrSetScale.Day < ShiftStart && StartTime % StrSetScale.Day >= ShiftFinish )
			StartTime += ShiftStart - StartTime % StrSetScale.Day;
		FinishTime = StartTime;
		if ( StartTime % StrSetScale.Day > ShiftFinish )
			{
			if ( StartTime % StrSetScale.Day != ShiftStart )
				{
				DurationTmp += StartTime % StrSetScale.Day - ShiftStart;
				FinishTime -= StartTime % StrSetScale.Day - ShiftStart;
				}
			}
		else
			{
			if ( StartTime % StrSetScale.Day != ShiftStart )
				{
				DurationTmp += StrSetScale.Day - ShiftStart + StartTime % StrSetScale.Day;
				FinishTime -= StrSetScale.Day - ShiftStart + StartTime % StrSetScale.Day;
				}
			}
		}
	if ( DurationTmp % ShiftTmp == 0L )
		FinishTime += DeleteTmp * ( ( DurationTmp / ShiftTmp ) - 1L ) + DurationTmp;
	else
		FinishTime += DeleteTmp * ( DurationTmp / ShiftTmp ) + DurationTmp;

	if ( WeekEnd && ParentPack )
		{
		CutOffTimesIterator->Restart();
		while ( *CutOffTimesIterator )
			{
			TCutOffTime* CutOffTime = (*CutOffTimesIterator)++;
			if ( CutOffTime->GetStartTime() < StartTime && CutOffTime->GetFinishTime() > StartTime )
				{
				FinishTime += CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
				StartTime += CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
				}
			if ( CutOffTime->GetStartTime() > StartTime && CutOffTime->GetStartTime() < FinishTime )
				FinishTime += CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
			}
		delete CutOffTimesIterator;
		}	
	}

void TWork::ArrangeFinishTime()
	{
	TDCutOffTimes* CutOffTimesTmp;
	if ( WeekEnd && ParentPack )
		{
		CutOffTimesTmp = new TDCutOffTimes();
		TDCutOffTimesIterator CutOffTimesIterator( *ParentPack->GetCutOffTimes() );
		while ( CutOffTimesIterator )
			{
			TCutOffTime* CutOffTime = CutOffTimesIterator++;
			if ( CutOffTime->GetStartTime() < FinishTime && CutOffTime->GetFinishTime() >= FinishTime )
				FinishTime = CutOffTime->GetStartTime();
			CutOffTimesTmp->Add( CutOffTime );
			}
		}

	unsigned long ShiftTmp;
	unsigned long DeleteTmp;
	unsigned long DurationTmp = Duration;
	if ( ShiftStart < ShiftFinish )
		{
		ShiftTmp = ShiftFinish - ShiftStart;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( FinishTime % StrSetScale.Day <= ShiftStart )
			FinishTime -= StrSetScale.Day - ShiftFinish + FinishTime % StrSetScale.Day;
		if ( FinishTime % StrSetScale.Day > ShiftFinish )
			FinishTime -= FinishTime % StrSetScale.Day - ShiftFinish;
		StartTime = FinishTime;
		if ( ( FinishTime % StrSetScale.Day != 0 || StrSetScale.Day != ShiftFinish ) && FinishTime % StrSetScale.Day != ShiftFinish )
			{
			DurationTmp += ShiftFinish - FinishTime % StrSetScale.Day;
			StartTime += ShiftFinish - FinishTime % StrSetScale.Day;
			}
		}
	else
		{
		ShiftTmp = StrSetScale.Day - ShiftStart + ShiftFinish;
		DeleteTmp = StrSetScale.Day - ShiftTmp;
		if ( FinishTime % StrSetScale.Day <= ShiftStart && FinishTime % StrSetScale.Day > ShiftFinish )
			FinishTime -= FinishTime % StrSetScale.Day - ShiftFinish;
		StartTime = FinishTime;
		if ( FinishTime % StrSetScale.Day > ShiftFinish )
			{
			if ( FinishTime % StrSetScale.Day != ShiftFinish )
				{
				DurationTmp += StrSetScale.Day - FinishTime % StrSetScale.Day + ShiftFinish;
				StartTime += StrSetScale.Day - FinishTime % StrSetScale.Day + ShiftFinish;
				}
			}
		else
			{
			if ( FinishTime % StrSetScale.Day != ShiftFinish )
				{
				DurationTmp += ShiftFinish - FinishTime % StrSetScale.Day;
				StartTime += ShiftFinish - FinishTime % StrSetScale.Day;
				}
			}
		}
	if ( DurationTmp % ShiftTmp == 0L )
		StartTime -= DeleteTmp * ( ( DurationTmp / ShiftTmp ) - 1L ) + DurationTmp;
	else
		StartTime -= DeleteTmp * ( DurationTmp / ShiftTmp ) + DurationTmp;

	if ( WeekEnd && ParentPack )
		{
		while ( !CutOffTimesTmp->IsEmpty() )
			{
			TCutOffTime* CutOffTime = CutOffTimesTmp->PeekHead();
			CutOffTimesTmp->DetachAtHead();
			if ( CutOffTime->GetStartTime() < FinishTime && CutOffTime->GetFinishTime() > FinishTime )
				{
				StartTime -= CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
				FinishTime -= CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
				}
			if ( CutOffTime->GetFinishTime() < FinishTime && CutOffTime->GetFinishTime() > StartTime )
				StartTime -= CutOffTime->GetFinishTime() - CutOffTime->GetStartTime();
			}
		delete CutOffTimesTmp;
		}
	}

TWork& TWork::operator = ( TWork& other )
	{
	if ( ParentPack )
		ParentPack->DetachBitTimes( this );
 //	free( Name ); // CG !!!
   delete[] Name;
	Name = strnewdup( other.Name );
	lPoint = other.lPoint;
	Number = other.Number;
	StartTime = other.StartTime;
	FinishTime = other.FinishTime;
	Entrances->Flush();
	other.GetEntrancesConnect( Entrances );
	Exits->Flush();
	other.GetExitsConnect( Exits );

	IndexShift = other.IndexShift;
	IndexBrigade = other.IndexBrigade;
	IndexPack = other.IndexPack;
	Critical = other.Critical;
	Duration = other.Duration;
	Percent = other.Percent;
	ShiftStart = other.ShiftStart;
	ShiftFinish = other.ShiftFinish;
	*Resources = *other.Resources;
	WeekEnd = other.WeekEnd;

	if ( ParentPack )
		ParentPack->AddBitTimes( this );
	return *this;
	}

void TWork::write( opstream& os ) const
	{
	os.fwriteString( Name );
	os << lPoint << Number << StartTime << FinishTime;
	os << IndexShift << IndexBrigade << IndexPack << Duration << Percent << ShiftStart << ShiftFinish << Critical;
	os << *Resources;
	os << WeekEnd;
	}

void* TWork::read( ipstream& is, TPack* ThePack )
	{
	ParentPack = ThePack;
//	free( Name ); // CG !!!
   delete[] Name;
	Name = is.freadString();
	is >> lPoint >> Number >> StartTime >> FinishTime;
	is >> IndexShift >> IndexBrigade >> IndexPack >> Duration >> Percent >> ShiftStart >> ShiftFinish >> Critical;
	is >> *Resources;
	is >> WeekEnd;
	return this;
	}



