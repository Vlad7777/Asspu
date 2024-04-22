#include <owl\owlpch.h>
#include <owl\scroller.h>
#include <owl\inputdia.h>
#include <typeinfo.h>  // New header  !!!

#include "pack.h"
#include "mdl_wind.h"
#include "mainw4.rh"

DEFINE_RESPONSE_TABLE1( TMDLWindow, TWindow )
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONUP,
	EV_WM_LBUTTONDBLCLK,
	EV_WM_RBUTTONDOWN,
	EV_WM_RBUTTONUP,
	EV_WM_RBUTTONDBLCLK,
	EV_WM_MOUSEMOVE,
	//EV_WM_KEYDOWN,  // NEW  SUPPORT  KEYBOARD  from TFrameWin...!!!
END_RESPONSE_TABLE;

TMDLWindow::TMDLWindow( TWindow* parent, STR_PACK_EDIT* TheStrPackEdit )
	: TBASWindow( parent, TheStrPackEdit )
	{
	EventTmp = NULL;
	lPointTmp = NULL;
	bLButtonDown = FALSE;
	bRButtonDown = FALSE;
	Attr.Style |= WS_VSCROLL | WS_HSCROLL;
	Scroller = new TScroller( this, 10, 10, 3200, 3200 );
  // Scroller->TrackMode=FALSE;  // Whithout  push !!!!!
	Scroller->AutoOrg = FALSE;
	}

void TMDLWindow::Paint( TDC& dc, bool, TRect& iTheRect )
	{
	TLRect lRect;
	lRect = (TIRect)iTheRect;
	TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
	lRect += lSize;
	lRect *= TEvent::StrSetScale.ScaleMDL;
	lRect.left -=  Max( TPack::StrSetEvent.HConstSizeMDL, TWork::StrSetEvent.HConstSizeMDL );
	lRect.right += Max( TPack::StrSetEvent.HConstSizeMDL, TWork::StrSetEvent.HConstSizeMDL );
	lRect.top -= Max( TPack::StrSetEvent.VConstSizeBottomMDL, TWork::StrSetEvent.VConstSizeBottomMDL ) + 1;
	lRect.bottom += Max( TPack::StrSetEvent.VConstSizeTopMDL, TWork::StrSetEvent.VConstSizeTopMDL ) + 1;


	TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
	while ( ConnectsIterator )
		ConnectsIterator++->Draw( dc, lSize, &lRect );

	TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
	while ( DPacksIterator )
		DPacksIterator++->Draw( dc, lSize, &lRect );

	TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
	while ( DWorksIterator )
		DWorksIterator++->Draw( dc, lSize, &lRect );
	}


//////////////////////////////////////////////////////////
		//  New ->  work  with  TFrameWin...!!!!
/*
//void TMDLWindow::EvKeyDown( UINT key, UINT , UINT )
/*
	{
	int xLine = Scroller->XLine;
	int yLine = Scroller->YLine;
	int xPage = Scroller->YPage;
	switch  (key) {
	  case  VK_UP:
		 Scroller->ScrollBy(0, -yLine );
		 break;
	  case  VK_DOWN:
		 Scroller->ScrollBy(0, yLine );
		 break;
		 ...........
		 ...........
	  default: DefaultProcessing();

	  }
	}
	*/
/////////////////////////////////////////////////////////
void TMDLWindow::EvLButtonDown( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		bLButtonDown = TRUE;
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint ) )
				StrPackEdit->WorkStart = work;
          //  break;    NO !!!!???
			}
		if ( StrPackEdit->WorkStart )
			{
			TWork* work = new TWork();
			*work = *StrPackEdit->WorkStart;
			EventTmp = work;
			}
		else
			{
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				TPack* pack = DPacksIterator++;
				if ( pack->FindForPoint( lPoint ) )
					StrPackEdit->PackStart = pack;
				}
			if ( StrPackEdit->PackStart )
				{
				TPack* pack = new TPack();
				*pack = *StrPackEdit->PackStart;
				EventTmp = pack;
				}
			}

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
			Scroller->AutoScroll();
			}
		ReleaseCapture();
		}
	}

void TMDLWindow::EvLButtonUp( UINT, TPoint& iThePoint )
	{
	if ( bLButtonDown )
		{
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;
		if ( EventTmp )
			{
	  //		delete EventTmp;   ??????????????????????????????????//
	  //		EventTmp = NULL;   ??????????????????????????????????
			if ( StrPackEdit->WorkStart )
				{
            //delete ((TWork*)(EventTmp));  // new delete !!!!!
            delete (dynamic_cast<TWork *>(EventTmp));  // new delete !!!!!
            EventTmp = NULL;  // new !!!

				StrPackEdit->PackView->MoveAtTail( StrPackEdit->WorkStart );
				StrPackEdit->WorkStart->Move( lPoint );
				}
			if ( StrPackEdit->PackStart )
				{
            delete (dynamic_cast<TPack *>(EventTmp));  // new delete !!!!!
            EventTmp = NULL;  // new !!!
				StrPackEdit->PackView->MoveAtTail( StrPackEdit->PackStart );
				StrPackEdit->PackStart->Move( lPoint );
				}
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				TPack* pack = DPacksIterator++;
				if ( pack->FindForPoint( lPoint ) && StrPackEdit->PackStart != pack )
					{
					StrPackEdit->PackFinish = pack;
					Parent->PostMessage( EV_MOVEINPACK );
					}
				}
			if ( !StrPackEdit->PackFinish )
				{
				StrPackEdit->WorkStart = NULL;
				StrPackEdit->PackStart = NULL;
				Invalidate();
				Parent->PostMessage( EV_INVALIDATEGNT );
				}
			}
		else
			Parent->PostMessage( EV_NEWELEMENT, (WPARAM)lPoint.x, (LPARAM)lPoint.y );
		bLButtonDown = FALSE;
		}
	}

void TMDLWindow::EvLButtonDblClk( UINT, TPoint& iThePoint )
	{
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;
			//  Dialog  here  by number ore name !!!
		char FindName [255];
		int ptr;
		strset(FindName , '\0') ;
		if ((TInputDialog(this, "Name of Work",
								"Input a Search Name:",
								FindName,
								sizeof(FindName))).Execute() == IDOK)
								{

		TLPoint lPointFind;
		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;

	ptr = strncmp(FindName, work->GetName(), strlen(FindName));
		if (ptr == 0)

				  {
					lPointFind = work->GetPoint();
					//lPointFind -= lSize; // ???????????????
					lPointFind /= TEvent::StrSetScale.ScaleMDL;
					lPointFind.x -= TWork::StrSetEvent.HConstSizeMDL / TEvent::StrSetScale.ScaleMDL;
					lPointFind.y -= TWork::StrSetEvent.VConstSizeTopMDL / TEvent::StrSetScale.ScaleMDL + 1;
					//Scroller->ScrollTo(0,0); // Try in begin ???!!!
					Scroller->ScrollBy((lPointFind.x - lPoint.x) / Scroller->XUnit, (lPointFind.y - lPoint.y) / Scroller->YUnit); // Error !!!
					Invalidate(); //???
					// delete lPoints ???? !!!!!!!
					break;  //  ore  next Work ???

					}
				 }
      if (ptr != 0)
		MessageBox("The Work didn't find !", "Searching of Work", MB_OK) == IDNO;
			 }
		 }

void TMDLWindow::EvRButtonDown( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		bRButtonDown = TRUE;
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint ) )
				StrPackEdit->WorkStart = work;
			}
		if ( !StrPackEdit->WorkStart )
			{
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				TPack* pack = DPacksIterator++;
				if ( pack->FindForPoint( lPoint ) )
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
			Scroller->AutoScroll();
			}
		ReleaseCapture();	
		}
	}

void TMDLWindow::EvRButtonUp( UINT, TPoint& iThePoint )
	{
	if ( bRButtonDown )
		{
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;
		
		if ( StrPackEdit->WorkStart || StrPackEdit->PackStart )
			{
			TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;
				if ( work->FindForPoint( lPoint ) && StrPackEdit->WorkStart != work )
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
					if ( pack->FindForPoint( lPoint ) && StrPackEdit->PackStart != pack )
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
				if ( work->FindForDelete( *lPointTmp, lPoint ) )
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
					if ( pack->FindForDelete( *lPointTmp, lPoint ) )
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
						if ( connect->FindForDelete( *lPointTmp, lPoint ) )
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

void TMDLWindow::EvRButtonDblClk( UINT, TPoint& iThePoint )
	{
	if ( !bLButtonDown && !bRButtonDown )
		{
		Parent->PostMessage( EV_DIRTY );
		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;

		TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
		while ( DWorksIterator )
			{
			TWork* work = DWorksIterator++;
			if ( work->FindForPoint( lPoint ) )
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
			if ( pack->FindForPoint( lPoint ) )
				{
				StrPackEdit->PackStart = pack;
				Parent->PostMessage( EV_ENTRANCEINPACK );
				return;
				}
			}
		Parent->PostMessage( EV_RELAYOUT, (WPARAM)HWindow );
		}
	}

void TMDLWindow::EvMouseMove( UINT, TPoint& iThePoint )
	{
	if ( bLButtonDown && EventTmp && iThePoint.x >= 0 && iThePoint.y >= 0 )
		{
		TLSize lSize( Scroller->XPos * Scroller->XUnit, Scroller->YPos * Scroller->YUnit );

		TLRect lRect;
		lRect = EventTmp->GetRect();
		lRect /= TEvent::StrSetScale.ScaleMDL;
		lRect -= lSize;
		TIRect iRect;
		iRect = lRect;
		InvalidateRect( iRect );

		TLPoint lPoint;
		lPoint = (TIPoint)iThePoint;
		lPoint += lSize;
		lPoint *= TEvent::StrSetScale.ScaleMDL;
		EventTmp->Move( lPoint );
		
		TClientDC dc( *this );
		EventTmp->Draw( dc, lSize );

		lRect = EventTmp->GetRect();
		lRect /= TEvent::StrSetScale.ScaleMDL;
		lRect -= lSize;
		iRect = lRect;
		ValidateRect( iRect );
		}
	}

