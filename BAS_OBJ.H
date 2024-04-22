#if !defined(__BAS_OBJ_H)
#define __BAS_OBJ_H

#include <classlib\objstrm.h>
#include <classlib\listimp.h>
#include <classlib\dlistimp.h>
#include <owl\owldefs.h>
#include <owl\lpoint.h>      //#3##################????
#include <owl\dc.h>
#include <windows.h>

class TEvent;
class TWork;
class TPack;

struct STR_SET_CONNECT
	{
	int ConstSizeArrow;
	float ConstAngjeArrow;
	LOGPEN LogPenMain;
	LOGPEN LogPenCrit;
	};

class TConnect
	{
	protected:
		TPack* PackStart;
		TPack* PackFinish;
		TWork* WorkStart;
		TWork* WorkFinish;
	public:
		static STR_SET_CONNECT StrSetConnect;
	public:
		TConnect( TWork* TheWorkStart, TWork* TheWorkFinish, TPack* ThePackStart = NULL, TPack* ThePackFinish = NULL );
		TConnect();
		~TConnect();
		void ResetPack();
		void SetStart( TPack* ThePack );
		void SetFinish( TPack* ThePack );
		void SetStart( TWork* TheWork );
		void SetFinish( TWork* TheWork );
		TEvent* GetEventStart() const
			{ 
			if ( PackStart != NULL )
				return (TEvent*)PackStart; 
			return (TEvent*)WorkStart;	
			}
		TEvent* GetEventFinish() const
			{ 
			if ( PackFinish != NULL )
				return (TEvent*)PackFinish; 
			return (TEvent*)WorkFinish;
			}
		TPack* GetPackStart() const
			{ return PackStart; }
		TPack* GetPackFinish() const
			{ return PackFinish; }
		TWork* GetWorkStart() const
			{ return WorkStart; }
		TWork* GetWorkFinish() const
			{ return WorkFinish; }

		bool operator == ( const TConnect& other ) const
			{ return &other == this; }

		void DrawArrow( TDC& dc, TIPoint& iThePointStart, TIPoint& iThePointFinish );
		void Draw( TDC& dc, TLSize& lTheSize, TLRect* lTheRect = NULL, unsigned long TheMinTime = 0 );
		bool FindForDelete( TLPoint& lPointDown, TLPoint& lPointUp, unsigned long TheMinTime = 0 );		

		void write( opstream& os ) const;
		void* read( ipstream& is, TEvent** ge );
	};

typedef TIListImp<TConnect> TConnects;
typedef TIListIteratorImp<TConnect> TConnectsIterator;

class TBitTime
	{
	protected:
		unsigned long Quantity;
		unsigned long Time;
		unsigned long Resource;
	public:
		TBitTime( unsigned long TheTime, unsigned long TheResource )
			{ Time = TheTime, Resource =  TheResource, Quantity = 1; }
		TBitTime( TBitTime* TheBitTime )
			{ Time = TheBitTime->Time, Resource = TheBitTime->Resource, Quantity = TheBitTime->Quantity; }
		void AddResource( unsigned long TheResource )
			{ Resource += TheResource; }
		void AddResource( TBitTime* TheBitTime )
			{ Resource += TheBitTime->Resource, Quantity += TheBitTime->Quantity; }
		void DelResource( unsigned long TheResource )
			{ Resource -= TheResource; }
		void DelResource( TBitTime* TheBitTime )
			{ Resource -= TheBitTime->Resource, Quantity -= TheBitTime->Quantity; }
		void AddQuantity() 
			{ Quantity++; }
		unsigned long GetQuantity() const
			{ return Quantity; }
		long GetTime() const
			{ return Time; }
		unsigned long GetResource() const
			{ return Resource; }

		bool operator == ( const TBitTime& other ) const
			{ return &other == this; }

		bool operator < ( const TBitTime& other ) const
			{ return Time < other.Time; }
	};

typedef TISDoubleListImp<TBitTime> TSBitTimesTmp;
typedef TISDoubleListIteratorImp<TBitTime> TSBitTimesIterator;
typedef TIDoubleListImp<TBitTime> TDBitTimes;

class TSBitTimes : public TSBitTimesTmp
	{
	public: 
		TSBitTimes() : TSBitTimesTmp() {}
		void Sum( TSBitTimes* TheBitTimes );
		void Sub( TSBitTimes* TheBitTimes );
	};				

struct STR_SET_SCALE
	{
	bool ScaleYear;
	bool ScaleMonth;
	bool ScaleDay;
	bool ScaleHour;
	bool ScaleMinute;
	float ScaleMDL;
	float VScaleGNT;
	float VScaleGST;
	float HScaleGNTGST;
	unsigned long Day;
	int QuantityResource;
	unsigned long StepTime;
	unsigned long CriticalTime;
	LOGPEN LogPenResource;
	COLORREF ColorRefResource;
	};

struct STR_SET_EVENT
	{
	int HConstSizeMDL;
	int VConstSizeTopMDL;
	int VConstSizeBottomMDL;
	int VConstSizeTopGNT;
	int VConstSizeBottomGNT;
	LOGPEN LogPenMain;
	LOGPEN LogPenCrit;
	COLORREF ColorRefResource;
	COLORREF ColorRefPercent;
	COLORREF ColorRefFont;
	LOGFONT LogFont;
	};

class TEvent
	{
	protected:
		char* Name;
		unsigned long Number;
		unsigned long StartTime;
		unsigned long FinishTime;
		unsigned long ShiftStart;  // New
		unsigned long ShiftFinish; // New

		TLPoint lPoint;
		TConnects* Entrances;
		TConnects* Exits;
		TPack* ParentPack;
		unsigned long Tmp;
		unsigned long TmpR;
		unsigned long TmpL;
	public:
		static STR_SET_SCALE StrSetScale;
	public:
		void SetName( char* TheName )
			{ 
			if ( Name )
			//	free( Name );  CG !!!
         delete[] Name;
			Name = strnewdup( TheName ); 
			}
		void SetNumber( unsigned long TheNumber )
			{ Number = TheNumber; }
		void SetStartTime( unsigned long TheStartTime )
			{ StartTime = TheStartTime; }
		void SetFinishTime( unsigned long TheFinishTime )
			{ FinishTime = TheFinishTime; }
		void SetParent( TPack* ThePack = NULL )
			{ ParentPack = ThePack; }
		void SetTmp( unsigned long TheTmp )
			{ Tmp = TheTmp; }
		void SetTmpR( unsigned long TheTmp )
			{ TmpR = TheTmp; }
		void SetTmpL( unsigned long TheTmp )
			{ TmpL = TheTmp; }

		char* GetName() const
			{ return Name; }
		unsigned long GetNumber() const
			{ return Number; }
		unsigned long GetStartTime() const
			{ return StartTime; }
		unsigned long GetFinishTime() const
			{ return FinishTime; }

/////////////////////////   New /////////////////////////////////
		void SetShiftStart( unsigned long TheShiftStart )
			{ ShiftStart = TheShiftStart; }
		void SetShiftFinish( unsigned long TheShiftFinish )
			{ ShiftFinish = TheShiftFinish; }

		unsigned long GetShiftStart()
			{ return ShiftStart; }
		unsigned long GetShiftFinish()
			{ return ShiftFinish; }

//////////////////////////////////////////////////////////////////

		TLPoint GetPoint() const
			{ return lPoint; }
		TPack* GetParent() const
			{ return ParentPack; }
		unsigned long GetTmp() const
			{ return Tmp; }
		unsigned long GetTmpR() const
			{ return TmpR; }
		unsigned long GetTmpL() const
			{ return TmpL; }

		unsigned long GetEntrancesConnect()
			{ return Entrances->GetItemsInContainer(); }
		unsigned long GetExitsConnect()
			{ return Exits->GetItemsInContainer(); }
		void GetEntrancesConnect( TConnects* Check );
		void GetExitsConnect( TConnects* Check );
		void AddEntrance( TConnect* TheConnect )
			{ Entrances->Add( TheConnect ); }
		void AddExit( TConnect* TheConnect )
			{ Exits->Add( TheConnect ); }
		void DetachEntrance( TConnect* TheConnect )
			{ Entrances->Detach( TheConnect ); }
		void DetachExit( TConnect* TheConnect )
			{ Exits->Detach( TheConnect ); }

		bool operator == ( const TEvent& other ) const
			{ return &other == this; }

		bool operator < ( const TEvent& other ) const
			{ return lPoint.y < other.lPoint.y; }

		virtual int GetHConstSizeMDL() = 0;
		virtual int GetVConstSizeTopMDL() = 0;
		virtual int GetVConstSizeBottomMDL() = 0;
		virtual int GetVConstSizeTopGNT() = 0;
		virtual int GetVConstSizeBottomGNT() = 0;
		virtual LOGPEN GetLogPen() = 0;
		virtual COLORREF GetColorRefResource() = 0;
		virtual COLORREF GetColorRefPercent() = 0;
		virtual COLORREF GetColorRefFont() = 0;
		virtual LOGFONT GetLogFont() = 0;

		virtual void GetBitTimes( TSBitTimes* TheBitTimes, int TheIndexResource ) = 0;
		virtual unsigned long GetCarentTime() = 0;
		virtual void ResetChec() = 0;
		virtual void ArrangeStartTime() = 0;
		virtual void ArrangeFinishTime() = 0;
		
		void Draw( TDC& dc, TLSize& lTheSize, TLRect* lTheRect = NULL, unsigned long TheMinTime = 0 );
		void Move( TLPoint& lThePoint, unsigned long TheMinTime = 0 );
		bool FindForPoint( TLPoint& lThePoint, unsigned long TheMinTime = 0 );
		bool FindForDelete( TLPoint& lPointDown, TLPoint& lPointUp, unsigned long TheMinTime = 0 );
		TLRect GetRect( unsigned long TheMinTime = 0 );
		TLPoint GetStartPoint( unsigned long TheMinTime = 0 );
		TLPoint GetFinishPoint( unsigned long TheMinTime = 0 );
	};

typedef TISListImp<TEvent> TSEvents;
typedef TISListIteratorImp<TEvent> TSEventsIterator;
typedef TIListImp<TEvent> TEvents;
typedef TIListIteratorImp<TEvent> TEventsIterator;

int Code( TLPoint& lThePoint, TLRect& lTheRect );
bool Clip( TLRect& plTheRect, TLPoint& lThePointStart, TLPoint& lThePointFinish );
long StartMonth( long TheTime );
long NextMonth( long TheTime );
long StartYear( long TheTime );
long NextYear( long TheTime );
void DrawCalendar( TDC& dc, TLSize& lTheSize, TLRect* lTheRect = NULL, unsigned long TheMinTime = 0 );
void DrawNet( TDC& dc, TLSize& lTheSize, TLRect* lTheRect = NULL, unsigned long TheMinTime = 0 );
#endif // __BAS_OBJ_H

