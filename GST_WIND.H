#include "bas_wind.h"

class TGSTWindow : public TBASWindow
	{
	protected:
		bool bLButtonDown;
		bool bRButtonDown;

	public:
		TGSTWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit );

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

	DECLARE_RESPONSE_TABLE( TGSTWindow );
	};


