#if !defined(__PACK_H)
#define __PACK_H

#include "work.h"

struct STR_PACK_EDIT
	{
	TPack* PackView;
	TPack* PackStart;
	TPack* PackFinish;
	TWork* WorkStart;
	TWork* WorkFinish;
	TConnect* Connect;
	};

class TPack : public TEvent
	{
	protected:
		unsigned long MaxNumber;
		TIDoubleListImp<TPack>* DPacks;
		TDWorks* DWorks;
		TConnects* Connects;
		TSBitTimes** BitTimes;
		TIDoubleListImp<TPack>* ShadePacks;
		TDCutOffTimes* CutOffTimes;	
	public:
		static STR_SET_EVENT StrSetEvent;
	public:
		TPack();
		~TPack();

		unsigned long GetFreeNumber()
			{ return MaxNumber + 1; }
		void SetMaxNumber( unsigned long TheMaxNumber )
			{ MaxNumber = TheMaxNumber; }
		unsigned long GetQuantityEvent();
		TWork* SelectWork();
		TWork* GetWork( unsigned long TheNumber );

		TDWorks* GetWorks()
			{ return DWorks; }
		TIDoubleListImp<TPack>* GetPacks()
			{ return DPacks; }
		TConnects* GetConnects()
			{ return Connects; }
		TSBitTimes* GetBitTimes( int TheIndexResource )
			{ return ( BitTimes ) ? *( BitTimes + TheIndexResource ) : NULL; }
		TIDoubleListImp<TPack>* GetShade()
			{ return ShadePacks; }
		TDCutOffTimes* GetCutOffTimes()
			{ return ( !ParentPack ) ? CutOffTimes : ParentPack->GetCutOffTimes(); }
			
		void Add( TWork* TheWork, bool Move = FALSE );
		void Detach( TWork* TheWork, bool Del = FALSE );
		void MoveAtTail( TWork* TheWork );
		void Add( TPack* ThePack, bool Move = FALSE );
		void Detach( TPack* ThePack, bool Del = FALSE );
		void MoveAtTail( TPack* ThePack );
		void Add( TConnect* TheConnect );
		void Detach( TConnect* TheConnect, bool Del = FALSE );
		void AddBitTimes( TWork* TheWork );
		void DetachBitTimes( TWork* TheWork );
		void AddBitTimes( TPack* ThePack );
		void DetachBitTimes( TPack* ThePack );
		void SortConnects( TConnects* TheConnects );
		void DeleteConnects( TConnects* TheConnects );

		void UnPack( TIDoubleListImp<TPack>* MainDPacks = NULL, TDWorks* MainDWorks = NULL, TConnects* MainConnects = NULL, TIDoubleListImp<TPack>* MainShadePacks = NULL );
		void OutShade();
		void FlushShade();
		void CorrectTime();
		void BuildBitTimes();
		void ResetBitTimes();
		void ResetCutOffTimes();

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
			{ return StrSetEvent.LogPenMain; }
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
		void ResetChec();

		virtual void ArrangeStartTime();
		virtual void ArrangeFinishTime();

		TPack&  operator = ( TPack& other );

		void write( opstream& os, unsigned long* TheIndex = NULL );
		void* read( ipstream& is, TPack* ThePack = NULL, unsigned long* TheIndex = NULL, TEvent** ArrayEvents = NULL );
	};

typedef TIDoubleListImp<TPack> TDPacks;
typedef TIDoubleListIteratorImp<TPack> TDPacksIterator;

#endif // __PACK_H
