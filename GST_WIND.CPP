#include <owl\owlpch.h>
#include <owl\scroller.h>

#include "mainw4.rh"
#include "pack.h"
#include "gst_wind.h"
#include "cal_wind.h"
#include "time.h"

DEFINE_RESPONSE_TABLE1( TGSTWindow, TWindow )
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDBLCLK,
	EV_WM_MOUSEMOVE,
	EV_WM_HSCROLL,
END_RESPONSE_TABLE;

TGSTWindow::TGSTWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit )
	: TBASWindow( parent, TheStrPackEdit )
	{
	bLButtonDown = FALSE;
	bRButtonDown = FALSE;
	Attr.Style |= WS_VSCROLL | WS_HSCROLL;
	Scroller = new TScroller( this, 10, 10, 3200, 3200 );
	Scroller->AutoOrg = FALSE;
	}

void TGSTWindow::Paint( TDC& dc, bool, TRect& iTheRect )
	{
	TLRect lRect;
	lRect = (TIRect)iTheRect;
	TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
	lRect += lSize;

	lRect.left *= TEvent::StrSetScale.HScaleGNTGST;
	lRect.right *= TEvent::StrSetScale.HScaleGNTGST;
	if ( (bool)Parent->SendMessage( EV_GETDRAWNET )	)
		DrawNet( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

	lRect.left += StrPackEdit->PackView->GetStartTime();
	lRect.right += StrPackEdit->PackView->GetStartTime();
	lRect.top *= TEvent::StrSetScale.VScaleGST;
	lRect.bottom *= TEvent::StrSetScale.VScaleGST;

	  ////////////////////////////////////////////My //////////////////
		TIRect iRect(0, 100 ,20 ,120);
		char string[255];
		unsigned long NewTimeMy = 0;
		struct tm *tblock;
		unsigned long MaxResource = 0;
		int  i = 0;
		int Lastmday = 0; // ???
	  ///////////////////////////////////////////////////////////


	unsigned long OldResource = 0;
	unsigned long OldTime = 0;
	unsigned long NewTime = 0;
	TRect Rect;
	TBitTime* BitTime;
	dc.SelectObject( TPen( &TEvent::StrSetScale.LogPenResource ) );
	dc.SelectObject( TBrush( TColor( TEvent::StrSetScale.ColorRefResource ) ) );

	TSBitTimesIterator BitTimesIterator( *StrPackEdit->PackView->GetBitTimes( (int)Parent->SendMessage( EV_GETINDEXRESOURCE ) ) );
	while ( BitTimesIterator )
		{
		BitTime = BitTimesIterator++;
		NewTime = BitTime->GetTime();
		//////////////////////////////////////////////////////////////////
		//tblock = localtime( &(long)NewTime );
		/////////////////////////////////////////////////////////////////
		if ( NewTime > lRect.left && NewTime < lRect.right )
			{
			if ( OldTime < lRect.left )
				 Rect.Set( (int)max( (long)( ( OldTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
							  (int)( OldResource / TEvent::StrSetScale.VScaleGST - lSize.cy ),
							  (int)max( (long)( ( NewTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
							  (int)( 0 - lSize.cy ) );
			else
				Rect.Set( (int)max( (long)( ( OldTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
							 (int)( OldResource / TEvent::StrSetScale.VScaleGST - lSize.cy ),
							 (int)max( (long)( ( NewTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
							 (int)( 0 - lSize.cy ) );
			dc.Rectangle( Rect );
			}
		if ( NewTime > lRect.left && NewTime > lRect.right )
			{
			Rect.Set( (int)max( (long)( ( OldTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
						 (int)( OldResource / TEvent::StrSetScale.VScaleGST - lSize.cy ),
						 (int)max( (long)( ( NewTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
						 (int)( 0 - lSize.cy ) );
			dc.Rectangle( Rect );
			break;
			}
		OldResource = BitTime->GetResource();
		////////////////////////////////////////////////////////////////

		if  (OldResource > MaxResource)
			 {
			 MaxResource = OldResource;
			 iRect = Rect;
			 }
	  ////////////////////////////////////////////My //////////////////
	  //		TIRect iRect(0, 100 ,20 ,120);
	  //		char string[255];
	  ///////////////////////////////////////////////////////////
		/*	tblock = localtime( &(long)NewTime );
			if(i != 0 && tblock->tm_mday != Lastmday)
			  {
			//  then   Print
			ultoa( MaxResource, string, 10 );
			dc.ExtTextOut( iRect.left + 3,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );

		iRect.left  = iRect.right + 2;
		iRect.right = iRect.right+ 20 + 2;
		MaxResource = 0;
		}
		else
		{
		if ((Lastmday == tblock -> tm_mday) && (OldResource > MaxResource))
		MaxResource = OldResource;
			ultoa( MaxResource, string, 10 );
			dc.ExtTextOut( iRect.left + 3,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );

		iRect.left  = iRect.right + 2;
		iRect.right = iRect.right+ 20 + 2;
		}
		Lastmday = tblock -> tm_mday;
		i++;
		 */
		//////////////////////////////////////////////////////////////
		OldTime = NewTime;
		}
	////////////////////////////  My  ///////////////////////////////
		if ( MaxResource != 0 )
			{
			iRect.Set (iRect.left, iRect.top + 20, iRect.left + 20, iRect.top + 40);
			ultoa( MaxResource, string, 10 );
			dc.ExtTextOut( iRect.left,
								iRect.top,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
         }
	////////////////////////////////////////////////////////////////
	dc.RestoreBrush();
	dc.RestorePen();
	}


void TGSTWindow::EvLButtonDown( UINT, TPoint& )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		bLButtonDown = TRUE;
		MSG  loopMsg;
		loopMsg.message = 0;
		SetCapture();
		while ( loopMsg.message != WM_LBUTTONUP && ( loopMsg.message != WM_MOUSEMOVE || ( loopMsg.wParam & MK_LBUTTON ) ) ) 
			{
			if ( ::PeekMessage( &loopMsg, 0, 0, 0, PM_REMOVE ) )
				{
				::TranslateMessage( &loopMsg );
				::DispatchMessage( &loopMsg );
				}
			long HScroll = Scroller->XPos * Scroller->XUnit;			
			Scroller->AutoScroll();
			if ( HScroll != Scroller->XPos * Scroller->XUnit )
				Parent->SendMessage( EV_SCROLLGNT, (WPARAM)Scroller->XPos );
			}
		ReleaseCapture();	
		}
	}

void TGSTWindow::EvLButtonUp( UINT, TPoint& )
	{
	if ( bLButtonDown )
		bLButtonDown = FALSE;
	}

void TGSTWindow::EvLButtonDblClk( UINT, TPoint& )
	{}

void TGSTWindow::EvRButtonDown( UINT, TPoint& )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		bRButtonDown = TRUE;
		MSG  loopMsg;
		loopMsg.message = 0;
		SetCapture();
		while ( loopMsg.message != WM_RBUTTONUP && ( loopMsg.message != WM_MOUSEMOVE || ( loopMsg.wParam & MK_RBUTTON ) ) ) 
			{
			if ( ::PeekMessage( &loopMsg, 0, 0, 0, PM_REMOVE ) )
				{
				::TranslateMessage( &loopMsg );
				::DispatchMessage( &loopMsg );
				}
			long HScroll = Scroller->XPos * Scroller->XUnit;			
			Scroller->AutoScroll();
			if ( HScroll != Scroller->XPos * Scroller->XUnit )
				Parent->SendMessage( EV_SCROLLGNT, (WPARAM)Scroller->XPos );
			}
		ReleaseCapture();	
		}
	}

void TGSTWindow::EvRButtonUp( UINT, TPoint& )
	{
	if ( bRButtonDown )
		bRButtonDown = FALSE;
	}

void TGSTWindow::EvRButtonDblClk( UINT, TPoint& )
	{
	Parent->PostMessage( EV_RELAYOUT, (WPARAM)HWindow );
	}

void TGSTWindow::EvMouseMove( UINT, TPoint& )
	{
	;
	}
	
void TGSTWindow::EvHScroll( UINT scrollCode, UINT thumbPos, HWND hWndCtl )
	{
	long HScroll = Scroller->XPos * Scroller->XUnit;
	if ( !( GetWindowLong( GWL_STYLE ) & WS_HSCROLL ) )
		DispatchScroll( scrollCode, thumbPos, hWndCtl );  // from scrollbar control
	else
		if ( Scroller )
			Scroller->HScroll( scrollCode, thumbPos );
		else
			DefaultProcessing();
	if ( HScroll != Scroller->XPos * Scroller->XUnit )
		Parent->SendMessage( EV_SCROLLGNT, (WPARAM)Scroller->XPos );
	}


