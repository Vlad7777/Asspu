#include <owl\owlpch.h>
#include <owl\scroller.h>

#include "cal_wind.h"

TBASWindow::TBASWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit )
	{
	Init( parent, 0, 0 );
	StrPackEdit	= TheStrPackEdit;

	EV_RELAYOUT = ::RegisterWindowMessage( "EV_RELAYOUT" );
	EV_DIRTY = ::RegisterWindowMessage( "EV_DIRTY" );
	EV_GETDRAWNET = ::RegisterWindowMessage( "EV_GETDRAWNET" );
	EV_GETINDEXRESOURCE = ::RegisterWindowMessage( "EV_GETINDEXRESOURCE" );

	EV_INVALIDATEMDL = ::RegisterWindowMessage( "EV_INVALIDATEMDL" );
	EV_INVALIDATEGNT = ::RegisterWindowMessage( "EV_INVALIDATEGNT" );
	EV_INVALIDATEGST = ::RegisterWindowMessage( "EV_INVALIDATEGST" );
	EV_SCROLLMDL = ::RegisterWindowMessage( "EV_SCROLLMDL" );
	EV_SCROLLGNT = ::RegisterWindowMessage( "EV_SCROLLGNT" );
	EV_SCROLLGST = ::RegisterWindowMessage( "EV_SCROLLGST" );

	EV_NEWELEMENT = ::RegisterWindowMessage( "EV_NEWELEMENT" );
	EV_DELETEELEMENT = ::RegisterWindowMessage( "EV_DELETEELEMENT" );
	EV_ENTRANCEINWORK = ::RegisterWindowMessage( "EV_ENTRANCEINWORK" );
	EV_ENTRANCEINPACK = ::RegisterWindowMessage( "EV_ENTRANCEINPACK" );
	EV_MOVEINPACK = ::RegisterWindowMessage( "EV_MOVEINPACK" );
	}

void TBASWindow::ScrollTo( long TheX, long TheY )
	{ Scroller->ScrollTo( TheX, TheY ); }



