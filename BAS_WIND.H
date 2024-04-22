#ifndef __BAS_WIND_H
#define __BAS_WIND_H

#include "pack.h"

class TBASWindow : public TWindow
	{
	protected:
		STR_PACK_EDIT* StrPackEdit;

		int EV_RELAYOUT;
		int EV_DIRTY;
		int EV_GETDRAWNET;
		int EV_GETINDEXRESOURCE;

		int EV_INVALIDATEMDL;
		int EV_INVALIDATEGNT;
		int EV_INVALIDATEGST;
		int EV_SCROLLMDL;
		int EV_SCROLLGNT;
		int EV_SCROLLGST;

		int EV_NEWELEMENT;
		int EV_DELETEELEMENT;
		int EV_ENTRANCEINWORK;
		int EV_ENTRANCEINPACK;
		int EV_MOVEINPACK;

	public:
		TBASWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit );
		void ScrollTo( long TheX, long TheY );
	};

#endif //__BAS_WIND_H
