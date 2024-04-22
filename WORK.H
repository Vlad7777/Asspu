#if !defined(__WORK_H)
#define __WORK_H

#include "bas_obj.h"

#define CRITICAL_NOT 0
#define CRITICAL_FREEZE_TIME 1
#define CRITICAL_FIX_TIME 2
#define CRITICAL_BEST_TIME 3
#define CRITICAL_MAIN 4

struct STR_SET_FIELD
	{
	char FieldName[ 11 ];
	char FieldNumber[ 11 ];
	char FieldStartTime[ 11 ];
	char FieldFinishTime[ 11 ];
	char FieldDuration[ 11 ];
	char FieldPercent[ 11 ];
	char FieldShiftLimStart[ 11 ];
	char FieldShiftLimFinish[ 11 ];		
	char FieldShiftOptStart[ 11 ];
	char FieldShiftOptFinish[ 11 ];		
	char FieldShiftStart[ 11 ];
	char FieldShiftFinish[ 11 ];
	char FieldCritical[ 11 ];
	char FieldNamePack[ 11 ];		
	char FieldNumberPack[ 11 ];		
	char FieldNumberParentPack[ 11 ];
	char FieldStartKnot[ 11 ];
	char FieldFinishKnot[ 11 ];
	char FieldIndexResource[ 11 ];
	char FieldResourceMin[ 11 ];	
	char FieldResourceOpt[ 11 ];	
	char FieldResourceMax[ 11 ];	
	char FieldResource[ 11 ];	
	char FieldVolume[ 11 ];			
	};
  
class TMyResource
	{
	protected:
		int IndexResource;
		unsigned long ResourceMin;	
		unsigned long ResourceOpt;
		unsigned long ResourceMax;	
		unsigned long Resource;	
		unsigned long Volume;			
	public:
		TMyResource( int TheIndexResource,	unsigned long TheResourceMin,	unsigned long TheResourceOpt,	unsigned long TheResourceMax,	unsigned long TheVolume )
			{ IndexResource = TheIndexResource,	ResourceMin = TheResourceMin,	ResourceOpt = TheResourceOpt,	ResourceMax = Resource = TheResourceMax,	Volume = TheVolume; }			
		TMyResource()
			{}

		void SetResourceMin()
			{ Resource = ResourceMin; }
		void SetResourceOpt()
			{ Resource = ResourceOpt; }
		void SetResourceMax()
			{ Resource = ResourceMax; }
		void SetResource( unsigned long TheResource )
			{ Resource = TheResource; }

		void SetResourceMin( unsigned long TheResourceMin )
			{ ResourceMin = TheResourceMin; }
		void SetResourceOpt( unsigned long TheResourceOpt )
			{ ResourceOpt = TheResourceOpt; }
		void SetResourceMax( unsigned long TheResourceMax )
			{ ResourceMax = TheResourceMax; }
		void SetVolume( unsigned long TheVolume )
			{ Volume = TheVolume; }

		int GetIndexResource()
			{ return IndexResource; }
		unsigned long GetResourceMin()
			{ return ResourceMin; }
		unsigned long GetResourceOpt()
			{ return ResourceOpt; }
		unsigned long GetResourceMax()
			{ return ResourceMax; }
		unsigned long GetResource()
			{ return Resource; }
		unsigned long GetVolume()
			{ return Volume; }


		bool operator == ( const TMyResource& other ) const
			{ return &other == this; }
		bool operator < ( const TMyResource& other ) const
			{ return IndexResource < other.IndexResource; }

		TMyResource& operator = ( TMyResource& other );
		friend opstream& operator << ( opstream& os, const TMyResource& resource )
			{ return os << resource.IndexResource << resource.ResourceMin	<< resource.ResourceOpt << resource.ResourceMax << resource.Resource	<< resource.Volume; }
		friend ipstream& operator >> ( ipstream& is, TMyResource& resource )
			{ return is >> resource.IndexResource >> resource.ResourceMin	>> resource.ResourceOpt >> resource.ResourceMax >> resource.Resource	>> resource.Volume; }
	};

typedef TISListImp<TMyResource> TSResourcesTmp;
typedef TISListIteratorImp<TMyResource> TSResourcesIterator;

class TSResources : public TSResourcesTmp
	{
	public:
		TSResources() : TSResourcesTmp() {}

		void SetResourceMin();
		void SetResourceOpt();
		void SetResourceMax();

		unsigned long GetDuration();
		unsigned long GetResource( int TheIndexResource );

		TSResources& operator = ( TSResources& other );
		friend opstream& operator << ( opstream& os, const TSResources& resources );
		friend ipstream& operator >> ( ipstream& is, TSResources& resources );
	};

class TCutOffTime
	{
	protected:
		unsigned long StartTime;
		unsigned long FinishTime;
	public:
		TCutOffTime( unsigned long TheStartTime, unsigned long TheFinishTime )
			{ StartTime = TheStartTime, FinishTime = TheFinishTime; }
		TCutOffTime()
			{}

		unsigned long GetStartTime()
			{ return StartTime; }
		unsigned long GetFinishTime()
			{ return FinishTime; }

		bool operator == ( const TCutOffTime& other ) const
			{ return &other == this; }
		bool operator < ( const TCutOffTime& other ) const
			{ return StartTime < other.StartTime; }

		TCutOffTime& operator = ( TCutOffTime& other );
		friend opstream& operator << ( opstream& os, const TCutOffTime& cutOffTime )
			{ return os << cutOffTime.StartTime << cutOffTime.FinishTime; }
		friend ipstream& operator >> ( ipstream& is, TCutOffTime& cutOffTime )
			{ return is >> cutOffTime.StartTime >> cutOffTime.FinishTime; }
	};

typedef TISListImp<TCutOffTime> TSCutOffTimes;
typedef TISListIteratorImp<TCutOffTime> TSCutOffTimesIterator;
typedef TIDoubleListImp<TCutOffTime> TDCutOffTimesTmp;
typedef TIDoubleListIteratorImp<TCutOffTime> TDCutOffTimesIterator;

class TDCutOffTimes : public TDCutOffTimesTmp
	{
	public:
		TDCutOffTimes() : TDCutOffTimesTmp() {}

		void CorrectBitTimes( TSBitTimes* TheBitTimes );

		TDCutOffTimes& operator = ( TDCutOffTimes& other );
		friend opstream& operator << ( opstream& os, const TDCutOffTimes& cutOffTimes );
		friend ipstream& operator >> ( ipstream& is, TDCutOffTimes& cutOffTimes );
	};				


class TWork : public TEvent
	{
	protected:
		int IndexShift;
		int IndexBrigade;
		int IndexPack;
		unsigned long Duration;
		int Percent;
//		unsigned long ShiftStart;
//		unsigned long ShiftFinish;
		int Critical;
		TSResources* Resources;
		bool WeekEnd;
	public:
		static STR_SET_EVENT StrSetEvent;
		static STR_SET_FIELD StrSetField;
	public:
		TWork();
		~TWork();
/*                  was !!!!!!
		void SetL( bool bool = TRUE )
			{ Tmp = MAKELONG( LOWORD( (DWORD)Tmp ), (WORD)bool ); }
		void SetR( bool bool = TRUE )
			{ Tmp = MAKELONG( (WORD)bool, HIWORD( (DWORD)Tmp ) ); }
		bool GetL() const
			{ return (bool)HIWORD( (DWORD)Tmp ); }
		bool GetR() const
			{ return (bool)LOWORD( (DWORD)Tmp ); }
*/
///////////////////////////bool////////////////////////////////////////
		void SetL( bool mybool = TRUE )
			{ Tmp = MAKELONG( LOWORD( (DWORD)Tmp ), (WORD)mybool ); }
		void SetR( bool mybool = TRUE )
			{ Tmp = MAKELONG( (WORD)mybool, HIWORD( (DWORD)Tmp ) ); }
		bool GetL() const
			{ return (bool)HIWORD( (DWORD)Tmp ); }
		bool GetR() const
			{ return (bool)LOWORD( (DWORD)Tmp ); }

////////////////////////////////////////////////////////////////////

		void SetIndexBrigade( int TheIndexBrigade )
			{ IndexBrigade = TheIndexBrigade; }
		void SetIndexPack( int TheIndexPack )
			{ IndexPack = TheIndexPack; }
		void SetIndexShift( int TheIndexShift )
			{ IndexShift = TheIndexShift; }
		void SetDuration( unsigned long TheDuration )
			{ Duration = TheDuration; }
		void SetPercent( int ThePercent )
			{ Percent = ThePercent; }
		void SetWeekEnd( bool TheWeekEnd = TRUE )
			{ WeekEnd = TheWeekEnd; }
 /*
		void SetShiftStart( unsigned long TheShiftStart )
			{ ShiftStart = TheShiftStart; }
		void SetShiftFinish( unsigned long TheShiftFinish )
			{ ShiftFinish = TheShiftFinish; }
 */
		int GetIndexBrigade()
			{ return IndexBrigade; }
		int GetIndexPack()
			{ return IndexPack; }
		int GetIndexShift()
			{ return IndexShift; }
		unsigned long GetDuration()
			{ return Duration; }
		int GetPercent()
			{ return Percent; }
  /*
		unsigned long GetShiftStart()
			{ return ShiftStart; }
		unsigned long GetShiftFinish()
			{ return ShiftFinish; }
  */
		int GetCritical()
			{ return Critical; }
		TSResources* GetResources()
			{ return Resources; }
		bool GetWeekEnd()
			{ return WeekEnd; }

		void BuildReserveTime();
		void BuildWork();
		void SetChec( TConnects* Check );
		void ResetConvert();
		void SetConvert( TConnects* Check );
		void BuildTimes( TSBitTimes* TheBitTimes, unsigned long TheStartTime, unsigned long TheFinishTime, unsigned long TheResource );
		void GetBitTimesMatWait( TSBitTimes* TheBitTimes, int TheIndexResource );
		bool CorrectTime( unsigned long TheStartTime, TConnects* Check );
		void MoveStartTime( unsigned long TheStartTime, TConnects* Check );
		void MoveFinishTime( unsigned long TheFinishTime, TConnects* Check );

		int GetHConstSizeMDL()
			{ return StrSetEvent.HConstSizeMDL; }
		int GetVConstSizeTopMDL()
			{ return StrSetEvent.VConstSizeTopMDL; }
		int GetVConstSizeBottomMDL()
			{ return StrSetEvent.VConstSizeBottomMDL; }
		int GetVConstSizeTopGNT() 
			{ return StrSetEvent.VConstSizeTopGNT; }
		int GetVConstSizeBottomGNT()
			{ return StrSetEvent.VConstSizeBottomGNT; }
		LOGPEN GetLogPen()
			{ return ( GetCritical() == CRITICAL_MAIN ) ? StrSetEvent.LogPenCrit : StrSetEvent.LogPenMain; }
		COLORREF GetColorRefResource()
			{ return StrSetEvent.ColorRefResource; }
		COLORREF GetColorRefPercent()
			{ return StrSetEvent.ColorRefPercent; }
		COLORREF GetColorRefFont()
			{ return StrSetEvent.ColorRefFont; }
		LOGFONT GetLogFont()
			{ return StrSetEvent.LogFont; }

		void GetBitTimes( TSBitTimes* TheBitTimes, int TheIndexResource );
		unsigned long GetCarentTime();
		void SetCritical( int TheCritical )
			{ Critical = TheCritical; }
		void ResetChec()
			{ Tmp = 0, TmpL = (unsigned long)FALSE; }
		void ResetStartTime( unsigned long TheStartTime, TConnects* Check );
		void ResetFinishTime( unsigned long TheFinishTime, TConnects* Check );
		void ArrangeStartTime();
		void ArrangeFinishTime();

		TWork& operator = ( TWork& other );

		void write( opstream& os ) const;
		void* read( ipstream& is, TPack* ThePack );
	};

typedef TIDoubleListImp<TWork> TDWorks;
typedef TIDoubleListIteratorImp<TWork> TDWorksIterator;

#endif // __WORK_H

