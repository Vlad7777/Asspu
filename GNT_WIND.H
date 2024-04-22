#include "bas_wind.h"

class TGNTWindow : public TBASWindow
	{
	protected:
		bool bLButtonDown;
		bool bRButtonDown;
		TEvent* EventTmp;
		TLPoint* lPointTmp;

	public:
		TGNTWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit );

	protected:
		// Message response functions
		void Paint( TDC& dc, bool, TRect& );
		void EvLButtonDown( UINT, TPoint& );
		void EvLButtonUp( UINT, TPoint& );
		void EvLButtonDblClk( UINT, TPoint& );
		void EvRButtonDown( UINT, TPoint& );
		void EvRButtonUp( UINT, TPoint& );
		void EvRButtonDblClk( UINT, TPoint& );
		void EvMouseMove( UINT, TPoint& );
		void EvHScroll( UINT, UINT, HWND );

	DECLARE_RESPONSE_TABLE(TGNTWindow);
	};


