#include "bas_wind.h"

struct STR_SET_CALEDAR
	{
	LOGPEN LogPenMain;
	LOGPEN LogPenCrit;
	COLORREF ColorRefFont;
	LOGFONT LogFont;
	};

class TCALWindow : public TBASWindow
	{
	public:
		static STR_SET_CALEDAR StrSetCalendar;

	public:
		TCALWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit );

	protected:
		// Message response functions
		void Paint( TDC& dc, bool, TRect& );
	};


