#include <owl\owlpch.h>
#include <owl\scroller.h>
#include <owl\opensave.h>                                          
#include <owl\edit.h>                                          
#include <dir.h>

#include "bas_obj.h"
#include "cal_wind.h"
#include "mainw4.rh"

STR_SET_CALEDAR TCALWindow::StrSetCalendar = { { 2, 1, 0, 0L }, { 2, 1, 0, 255L }, 8388608L };

TCALWindow::TCALWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit )
	: TBASWindow( parent, TheStrPackEdit )
	{
	Scroller = new TScroller( this, 10, 10, 3200, 3200 );
	Scroller->AutoOrg = FALSE;
	Scroller->AutoMode = FALSE;
	}

void TCALWindow::Paint( TDC& dc, bool, TRect& iTheRect )
	{
	TLRect lRect;
	lRect = (TIRect)iTheRect;
	TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
	lRect += lSize;
	lRect.left *= TEvent::StrSetScale.HScaleGNTGST;
	lRect.right *= TEvent::StrSetScale.HScaleGNTGST;

	DrawCalendar( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );
	}

