#include <time.h>      // My !!!
#include <owl\owlpch.h>
#include <owl\scroller.h>
#include <typeinfo.h>      // New


#include "bas_obj.h"  // My !!!!
#include "pack.h"
#include "gnt_wind.h"
#include "cal_wind.h"
#include "mainw4.rh"

DEFINE_RESPONSE_TABLE1( TGNTWindow, TWindow )
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDBLCLK,
	EV_WM_MOUSEMOVE,
	EV_WM_HSCROLL,
END_RESPONSE_TABLE;

TGNTWindow::TGNTWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit )
	: TBASWindow( parent, TheStrPackEdit )
	{
	EventTmp = NULL;
	lPointTmp = NULL;
	bLButtonDown = FALSE;
	bRButtonDown = FALSE;
	Attr.Style |= WS_VSCROLL | WS_HSCROLL;
	Scroller = new TScroller( this, 10, 10, 3200, 3200 );
	Scroller->AutoOrg = FALSE;
	}

void TGNTWindow::Paint( TDC& dc, bool, TRect& iTheRect )
	{
	TLRect lRect;
	lRect = (TIRect)iTheRect;
	TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
	lRect += lSize;
	lRect.left *= TEvent::StrSetScale.HScaleGNTGST;
	lRect.right *= TEvent::StrSetScale.HScaleGNTGST;
	if ( (bool)Parent->SendMessage( EV_GETDRAWNET )	)
	DrawNet( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

	lRect.left -= ( lRect.left != 0L ) ? 1L : 0L;
	lRect.right += 1L;
	lRect.top *= TEvent::StrSetScale.VScaleGNT;
	lRect.bottom *= TEvent::StrSetScale.VScaleGNT;
	lRect.top -= Max( TPack::StrSetEvent.VConstSizeBottomGNT, TWork::StrSetEvent.VConstSizeBottomGNT ) + 1;
	lRect.bottom += Max( TPack::StrSetEvent.VConstSizeTopGNT, TWork::StrSetEvent.VConstSizeTopGNT ) + 1;

	TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
	while ( ConnectsIterator )
		ConnectsIterator++->Draw( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		DPacksIterator++->Draw( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->Draw( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );
	}

void TGNTWindow::EvLButtonDown( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		bLButtonDown = TRUE;
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint.x *= TEvent::StrSetScale.HScaleGNTGST;
		lPoint.y *= TEvent::StrSetScale.VScaleGNT;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) && work->GetPercent() == 0 )
				StrPackEdit->WorkStart = work;
			}
		if ( StrPackEdit->WorkStart )
			{
			TWork* work = new TWork();
			*work = *StrPackEdit->WorkStart;
			work->SetParent( StrPackEdit->PackView );
			EventTmp = work;
//			}       was here !!!  move  to   bottom  !!!
//////////////////////////////////////////  My  ////////////////////////
	 //		TWork* work = new TWork();
	 //		*work = *StrPackEdit->WorkStart;
	 //	   work = EventTmp ;
			TClientDC  dc(*this);  //  Myy !!!!!!!!!!!
			LOGPEN LogPen = work->GetLogPen();
			dc.SelectObject( TPen( &LogPen ) );
			dc.SetTextColor( TColor(work->GetColorRefFont() ) );
			LOGFONT LogFont = work->GetLogFont();
			LogFont.lfHeight = (short int)LogFont.lfHeight / TEvent::StrSetScale.ScaleMDL;
			dc.SelectObject( TFont( &LogFont ) );

			int HSizeMDL = work->GetHConstSizeMDL() / TEvent::StrSetScale.ScaleMDL;
			int VSizeTopMDL = work->GetVConstSizeTopMDL() / TEvent::StrSetScale.ScaleMDL;
			int VSizeBottomMDL = work->GetVConstSizeBottomMDL() / TEvent::StrSetScale.ScaleMDL;

			TIPoint iPoint;
			iPoint = lPoint / TEvent::StrSetScale.HScaleGNTGST; // My !!! - lTheSize;


			dc.Rectangle( iPoint.x - HSizeMDL, iPoint.y,
							  iPoint.x + HSizeMDL, iPoint.y + VSizeBottomMDL );
 //			dc.MoveTo( iPoint.x - HSizeMDL + 1, iPoint.y );
 //			dc.LineTo( iPoint.x + HSizeMDL - 1, iPoint.y );

	/////////////////////////////// mY //////////////////////////
			dc.FillRect ( iPoint.x - HSizeMDL + 1, iPoint.y + 1,
							  iPoint.x + HSizeMDL - 1, iPoint.y + VSizeBottomMDL - 1,
							  TBrush(TColor::LtGray) );

			dc.SetBkColor(TColor::LtGray); ///  RESTORE !!!!
				// !!! RESTORE  TColor !!???
	////////////////////////////////////////////////////////////

			dc.MoveTo( iPoint.x, iPoint.y + 1 );
			dc.LineTo( iPoint.x, iPoint.y + VSizeBottomMDL - 1 );


			TISize iSize( 0 , VSizeTopMDL /  3  );           // New
			TIRect iRect( iPoint.x - HSizeMDL, iPoint.y - VSizeTopMDL,
							 iPoint.x + HSizeMDL - 1, iPoint.y - VSizeTopMDL + iSize.cy - 1 );
			char string[255];

			iRect += iSize;

			iSize.cy = VSizeBottomMDL / 2 ;
			iRect.Set( iPoint.x - HSizeMDL, iPoint.y,
						  iPoint.x - 1 , iPoint.y + iSize.cy - 1 );

			struct tm *tblock;
			unsigned long MyTime = work->GetStartTime();
			tblock = localtime( &(long)MyTime);
			strftime( string, 25, "%H:%M:%S", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			iRect += iSize;
			strftime( string, 25, "%d/%m/%y", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			iRect.Set( iPoint.x, iPoint.y,
						  iPoint.x + HSizeMDL - 1, iPoint.y + iSize.cy - 1 );
			MyTime =  work->GetFinishTime();
			tblock = localtime( &(long)MyTime );
			strftime( string, 25, "%H:%M:%S", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			iRect += iSize;
			strftime( string, 25, "%d/%m/%y", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			dc.RestorePen();
			dc.RestoreFont();
			dc.RestoreBrush();
      } // NEW !!!  from  work !!!!

////////////////////////////////////////////////////////////////////////
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
				Parent->SendMessage( EV_SCROLLGST, (WPARAM)Scroller->XPos );
			}
		ReleaseCapture();	
		}
	}

void TGNTWindow::EvLButtonUp( UINT, TPoint& )
	{
	if ( bLButtonDown )
		{
		if ( EventTmp )
			{
			if ( StrPackEdit->WorkStart )
				{
				StrPackEdit->PackView->MoveAtTail( StrPackEdit->WorkStart );
				TLPoint lPoint( 0L, 0L );
				if ( ((TWork*)EventTmp)->GetFinishTime() > StrPackEdit->WorkStart->GetFinishTime() )
					lPoint.x = ((TWork*)EventTmp)->GetFinishTime();
				else
					lPoint.x = ((TWork*)EventTmp)->GetStartTime();
				lPoint.x -= StrPackEdit->PackView->GetStartTime();
				StrPackEdit->WorkStart->GetParent()->DetachBitTimes( StrPackEdit->WorkStart );
				StrPackEdit->WorkStart->Move( lPoint, StrPackEdit->PackView->GetStartTime() );
				StrPackEdit->WorkStart->GetParent()->AddBitTimes( StrPackEdit->WorkStart );
				StrPackEdit->WorkStart = NULL;
			 //delete EventTmp;
            delete (dynamic_cast<TWork *>(EventTmp));  // new delete !!!!!
				EventTmp = NULL;
				StrPackEdit->PackView->CorrectTime();
				Parent->PostMessage( EV_INVALIDATEMDL );
				Parent->PostMessage( EV_INVALIDATEGST );
				}
			Invalidate();
			}
		bLButtonDown = FALSE;
		}
	}

void TGNTWindow::EvLButtonDblClk( UINT, TPoint& )
	{}

void TGNTWindow::EvRButtonDown( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		bRButtonDown = TRUE;
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint.x *= TEvent::StrSetScale.HScaleGNTGST;
		lPoint.y *= TEvent::StrSetScale.VScaleGNT;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) )
				StrPackEdit->WorkStart = work;
			}
		if ( !StrPackEdit->WorkStart )
			{
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				TPack* pack = DPacksIterator++;
				if ( pack->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) )
					StrPackEdit->PackStart = pack;
				}
			if ( !StrPackEdit->PackStart )	
				lPointTmp = new TLPoint( lPoint );
			}

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
				Parent->SendMessage( EV_SCROLLGST, (WPARAM)Scroller->XPos );
			}
		ReleaseCapture();	
		}
	}

void TGNTWindow::EvRButtonUp( UINT, TPoint& iThePoint )
	{
	if ( bRButtonDown )
		{
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint.x *= TEvent::StrSetScale.HScaleGNTGST;
		lPoint.y *= TEvent::StrSetScale.VScaleGNT;
		
		if ( StrPackEdit->WorkStart || StrPackEdit->PackStart )
			{
			TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;
				if ( work->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) && StrPackEdit->WorkStart != work )
					{
					StrPackEdit->WorkFinish = work;
					break;
					}
				}
			if ( !StrPackEdit->WorkFinish )
				{
				TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
				while ( DPacksIterator )
					{
					TPack* pack = DPacksIterator++;
					if ( pack->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) && StrPackEdit->PackStart != pack )
						{
						StrPackEdit->PackFinish = pack;
						break;
						}
					}
				}
			if ( ( StrPackEdit->PackStart || StrPackEdit->WorkStart ) && ( StrPackEdit->PackFinish || StrPackEdit->WorkFinish ) )
				Parent->PostMessage( EV_NEWELEMENT );
			else	
				{
				StrPackEdit->PackStart = NULL;
				StrPackEdit->WorkStart = NULL; 
				}
			}
		if ( lPointTmp )
			{
			TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;
				if ( work->FindForDelete( *lPointTmp, lPoint, StrPackEdit->PackView->GetStartTime() ) )
					StrPackEdit->WorkStart = work;
				}
			if ( StrPackEdit->WorkStart )
				Parent->PostMessage( EV_DELETEELEMENT );
			else
				{
				TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
				while ( DPacksIterator )
					{
					TPack* pack = DPacksIterator++;
					if ( pack->FindForDelete( *lPointTmp, lPoint, StrPackEdit->PackView->GetStartTime() ) )
						StrPackEdit->PackStart = pack;
					}
				if ( StrPackEdit->PackStart )
					Parent->PostMessage( EV_DELETEELEMENT );
				else
					{
					TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
					while ( ConnectsIterator )
						{
						TConnect* connect = ConnectsIterator++;
						if ( connect->FindForDelete( *lPointTmp, lPoint, StrPackEdit->PackView->GetStartTime() ) )
							 StrPackEdit->Connect = connect;
						}
					if ( StrPackEdit->Connect )
						Parent->PostMessage( EV_DELETEELEMENT );
					}
				}
			delete lPointTmp;
			lPointTmp = NULL;
			}
		bRButtonDown = FALSE;
		}
	}
	
void TGNTWindow::EvRButtonDblClk( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint.x *= TEvent::StrSetScale.HScaleGNTGST;
		lPoint.y *= TEvent::StrSetScale.VScaleGNT;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) )
				{
				StrPackEdit->WorkStart = work;
				Parent->PostMessage( EV_ENTRANCEINWORK );
				return;
				}
			}
		TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
		while ( DPacksIterator )
			{
			TPack* pack = DPacksIterator++;
			if ( pack->FindForPoint( lPoint, StrPackEdit->PackView->GetStartTime() ) )
				{
				StrPackEdit->PackStart = pack;
				Parent->PostMessage( EV_ENTRANCEINPACK );
				return;
				}
			}
		Parent->PostMessage( EV_RELAYOUT, (WPARAM)HWindow );
		}
	}

void TGNTWindow::EvMouseMove( UINT, TPoint& iThePoint )
	{
	if ( bLButtonDown && EventTmp && iThePoint.x >= 0 )
		{
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );

		TLRect lRect;                                
		lRect = EventTmp->GetRect( StrPackEdit->PackView->GetStartTime() );
		lRect.left /= TEvent::StrSetScale.HScaleGNTGST;
		lRect.right /= TEvent::StrSetScale.HScaleGNTGST;
		lRect.top /= TEvent::StrSetScale.VScaleGNT;
		lRect.bottom /= TEvent::StrSetScale.VScaleGNT;
		lRect -= lSize;
		TIRect iRect;
		iRect = lRect;
		InvalidateRect( iRect );

		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		lPoint += lSize;
		lPoint.x *= TEvent::StrSetScale.HScaleGNTGST;
		lPoint.y *= TEvent::StrSetScale.VScaleGNT;
		EventTmp->Move( lPoint, StrPackEdit->PackView->GetStartTime() );

		lRect = GetClientRect();
		lRect += lSize;
		lRect.left *= TEvent::StrSetScale.HScaleGNTGST;
		lRect.right *= TEvent::StrSetScale.HScaleGNTGST;
		lRect.top *= TEvent::StrSetScale.VScaleGNT;
		lRect.bottom *= TEvent::StrSetScale.VScaleGNT;
		TClientDC dc( *this );
		EventTmp->Draw( dc, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

		lRect = EventTmp->GetRect( StrPackEdit->PackView->GetStartTime() );
		lRect.left /= TEvent::StrSetScale.HScaleGNTGST;
		lRect.right /= TEvent::StrSetScale.HScaleGNTGST;
		lRect.top /= TEvent::StrSetScale.VScaleGNT;
		lRect.bottom /= TEvent::StrSetScale.VScaleGNT;
		lRect -= lSize;
		iRect = lRect;
		ValidateRect( iRect );
		}
	}

void TGNTWindow::EvHScroll( UINT scrollCode, UINT thumbPos, HWND hWndCtl )
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
		Parent->SendMessage( EV_SCROLLGST, (WPARAM)Scroller->XPos );
	}

