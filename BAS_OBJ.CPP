#include <time.h>
#include <owl\owlpch.h>
#include <owl\scroller.h>
#include <owl\opensave.h>                                          
#include <owl\edit.h>                                          
#include <dir.h>

#include "bas_obj.h"


STR_SET_SCALE TEvent::StrSetScale = { 0, 0, 1, 0, 0, 1, 1, 1, 3600, 86400UL, 50, 3600UL, 0UL, { 0, 1, 0, 0L }, 12632256L };

void TEvent::GetEntrancesConnect( TConnects* Check )
	{
	TConnectsIterator EntrancesIterator( *Entrances );
	while ( EntrancesIterator )
		Check->Add( EntrancesIterator++ );
	}

void TEvent::GetExitsConnect( TConnects* Check )
	{
	TConnectsIterator ExitsIterator( *Exits );
	while ( ExitsIterator )
		Check->Add( ExitsIterator++ );
	}
	
void TEvent::Draw( TDC& dc, TLSize& lTheSize, TLRect* lTheRect, unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		{
		if ( lTheRect == NULL || lTheRect->Contains( lPoint ) )
			{
			LOGPEN LogPen = GetLogPen();
			dc.SelectObject( TPen( &LogPen ) );
			dc.SetTextColor( TColor( GetColorRefFont() ) );
			LOGFONT LogFont = GetLogFont();
			LogFont.lfHeight = (short int)LogFont.lfHeight / StrSetScale.ScaleMDL;
			dc.SelectObject( TFont( &LogFont ) );

			int HSizeMDL = GetHConstSizeMDL() / StrSetScale.ScaleMDL;
			int VSizeTopMDL = GetVConstSizeTopMDL() / StrSetScale.ScaleMDL;
			int VSizeBottomMDL = GetVConstSizeBottomMDL() / StrSetScale.ScaleMDL;

			TIPoint iPoint;
			iPoint = lPoint / StrSetScale.ScaleMDL - lTheSize;
			dc.Rectangle( iPoint.x - HSizeMDL, iPoint.y - VSizeTopMDL,
							  iPoint.x + HSizeMDL, iPoint.y + VSizeBottomMDL );
			dc.MoveTo( iPoint.x - HSizeMDL + 1, iPoint.y );
			dc.LineTo( iPoint.x + HSizeMDL - 1, iPoint.y );
			dc.MoveTo( iPoint.x, iPoint.y + 1 );
			dc.LineTo( iPoint.x, iPoint.y + VSizeBottomMDL - 1 );

	  //	TISize iSize( 0 , VSizeTopMDL / ( /*StrSetEvent.QuantityStr +*/ 2 ) );
			TISize iSize( 0 , VSizeTopMDL /  3  );           // New
			TIRect iRect( iPoint.x - HSizeMDL, iPoint.y - VSizeTopMDL,
							 iPoint.x + HSizeMDL - 1, iPoint.y - VSizeTopMDL + iSize.cy - 1 );
			char string[255];
			ultoa( Number, string, 10 );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			iRect += iSize;
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								Name,
								lstrlen( Name ) );
			iRect += iSize;
/////////////////////////////// New  ////////////////////////////////////
			char string1 [30];
			char string2 [30];

			struct tm *tblock;
			tblock = localtime( &(long)ShiftStart );
			strftime( string1 , 25, "%H:%M:%S", tblock );
	  /*		dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
		*/
			strcpy(string , string1);  // New
			strcat(string , "-");      // New
					  //  coordinate  !!!!
//			iRect.Set( iPoint.x, iPoint.y   ,
//						  iPoint.x + HSizeMDL - 1, iPoint.y + iSize.cy - 1 );
			tblock = localtime( &(long)ShiftFinish );
			strftime( string2 , 25, "%H:%M:%S", tblock );
	  /*		dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
		*/
			strcat(string , string2);  // New
			strcat(string , "/");      // New
  /*****************************************************/
		char string3 [10];
		unsigned long ResourceTmp = 0;
		unsigned long ResourceSum = 0;
		TBitTime* BitTimeTmp;
		TSBitTimes* BitTimesTmp = new TSBitTimes();
		GetBitTimes( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
		TSBitTimesIterator BitTimesIterator( *BitTimesTmp );
		while ( BitTimesIterator )
			{
			BitTimeTmp = BitTimesIterator++;
			ResourceTmp = BitTimeTmp->GetResource();
			if (ResourceTmp!= 0)
			ResourceSum  =  ResourceTmp;
			}
      BitTimesTmp->Flush(true);
		delete BitTimesTmp;
		ultoa(ResourceSum , string3 ,10 ); //
		strcat(string , string3);  // New


  /*****************************************************/
		dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );

			iRect += iSize;

///////////////////////////////////////////////////////////////////////////

			iSize.cy = VSizeBottomMDL / 2 ;
			iRect.Set( iPoint.x - HSizeMDL, iPoint.y,
						  iPoint.x - 1 , iPoint.y + iSize.cy - 1 );
	 //		struct tm *tblock;
			tblock = localtime( &(long)StartTime );
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
			tblock = localtime( &(long)FinishTime );
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
			}
		return;
		}
	if ( ( ( lTheRect->left  <= StartTime - TheMinTime &&
				lTheRect->right >= StartTime - TheMinTime ) ||
			 ( lTheRect->left  <= FinishTime - TheMinTime &&
				lTheRect->right >= FinishTime - TheMinTime ) ||
			 ( lTheRect->left  > StartTime - TheMinTime &&
				lTheRect->right < FinishTime - TheMinTime ) ) &&
		  lTheRect->bottom  > lPoint.y - GetVConstSizeTopGNT() &&
		  lTheRect->top     < lPoint.y + GetVConstSizeBottomGNT() )
		{
		LOGPEN LogPen = GetLogPen();
		dc.SelectObject( TPen( &LogPen ) );
		dc.SetTextColor( TColor( GetColorRefFont() ) );
		LOGFONT LogFont = GetLogFont();
		LogFont.lfHeight = (short int)LogFont.lfHeight / StrSetScale.VScaleGNT;
		dc.SelectObject( TFont( &LogFont ) );

		int VSizeTopGNT = GetVConstSizeTopGNT() / StrSetScale.VScaleGNT;
		int VSizeBottomGNT = GetVConstSizeBottomGNT() / StrSetScale.VScaleGNT;

		TIPoint iPointStart;
		TIPoint iPointFinish;
		unsigned long StartTimeTmp = StartTime;
		unsigned long FinishTimeTmp = FinishTime;
		if ( StartTime - TheMinTime >= (unsigned long)lTheRect->left )
			iPointStart.x = (int)( ( StartTime - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx );
		else
			{
			iPointStart.x = (int)( lTheRect->left / StrSetScale.HScaleGNTGST - lTheSize.cx ) - 2;
			StartTimeTmp = (unsigned long)( ( iPointStart.x + lTheSize.cx ) * StrSetScale.HScaleGNTGST ) + TheMinTime;
			}
		if ( FinishTime - TheMinTime <= (unsigned long)lTheRect->right )
			iPointFinish.x = (int)( ( FinishTime - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx );
		else
			{
			iPointFinish.x = (int)( lTheRect->right / StrSetScale.HScaleGNTGST - lTheSize.cx ) + 2;
			FinishTimeTmp = (unsigned long)( ( iPointFinish.x + lTheSize.cx ) * StrSetScale.HScaleGNTGST ) + TheMinTime;
			}
		iPointStart.y = iPointFinish.y = (int)( lPoint.y / StrSetScale.VScaleGNT - lTheSize.cy );

		TIRect iRect( iPointStart.x, iPointStart.y - VSizeTopGNT,
						 iPointFinish.x, iPointFinish.y + VSizeBottomGNT );
		dc.Rectangle( iRect );
		dc.MoveTo( iPointStart );
		dc.LineTo( iPointFinish );

		if ( StartTimeTmp < GetCarentTime() )
			{
			if ( FinishTimeTmp > GetCarentTime() )
				iRect.Set( iPointStart.x,
							  iPointStart.y,
							  (int)( ( GetCarentTime() - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx ),
							  iPointFinish.y + VSizeBottomGNT );
			else
				iRect.Set( iPointStart.x,
							  iPointStart.y,
							  iPointFinish.x,
							  iPointFinish.y + VSizeBottomGNT );
			dc.SelectObject( TBrush( TColor( GetColorRefPercent() ) ) );
			dc.Rectangle( iRect );
			dc.RestoreBrush();
			}


		if ( ( StartTime - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx < 32767L )
			{
			iRect.Set( (int)( ( StartTime - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx ),
							iPointStart.y - VSizeTopGNT,
							iPointFinish.x - 1,
							iPointFinish.y - VSizeTopGNT / 2 - 1 );
			char string[280];
			ultoa( Number, string, 10 );
			strcat( string, " " );
			strcat( string, Name );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			}

		dc.SelectObject( TBrush( TColor( GetColorRefResource() ) ) );
		unsigned long OldTimeTmp = 0;
		unsigned long OldResourceTmp = 0;
		unsigned long NewTimeTmp;
		iRect.bottom = iPointStart.y;
		TBitTime* BitTimeTmp;
		TSBitTimes* BitTimesTmp = new TSBitTimes();
		GetBitTimes( BitTimesTmp, TEvent::StrSetScale.QuantityResource );
		TSBitTimesIterator BitTimesIterator( *BitTimesTmp );
		while ( BitTimesIterator )
			{
			BitTimeTmp = BitTimesIterator++;
			NewTimeTmp = BitTimeTmp->GetTime();
			if ( OldTimeTmp < FinishTimeTmp && NewTimeTmp > StartTimeTmp && OldResourceTmp != 0 )
				{
				iRect.left = iPointStart.x;
				iRect.right = iPointFinish.x;
				iRect.top = iPointStart.y - OldResourceTmp / TEvent::StrSetScale.VScaleGST - 1;
				if ( iRect.bottom - iRect.top > VSizeTopGNT / 2 )
					iRect.top = iRect.bottom - VSizeTopGNT / 2;
				if ( OldTimeTmp > StartTimeTmp )
					iRect.left = (int)( ( OldTimeTmp - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx );
				if ( NewTimeTmp < FinishTimeTmp )
					iRect.right = (int)( ( NewTimeTmp - TheMinTime ) / StrSetScale.HScaleGNTGST - lTheSize.cx );
				dc.Rectangle( iRect );
				}
			OldTimeTmp = NewTimeTmp;
			OldResourceTmp = BitTimeTmp->GetResource();
			}
      BitTimesTmp->Flush(true);
		delete BitTimesTmp;

		dc.RestoreBrush();
		dc.RestorePen();
		dc.RestoreFont();
		}
	}
	
void TEvent::Move( TLPoint& lThePoint, unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		{
		lPoint = lThePoint;
		return;
		}

	unsigned long StartTimeTmp;
	unsigned long FinishTimeTmp;	
	unsigned long TimeTmp;
	StartTimeTmp = FinishTimeTmp = lThePoint.x + TheMinTime;
	if ( StartTimeTmp < StartTime )
		{
		TConnectsIterator EntrancesIterator( *Entrances );
		while ( EntrancesIterator )
			{
			TimeTmp = ((TEvent*)EntrancesIterator++->GetWorkStart())->GetFinishTime();
			if ( StartTimeTmp < TimeTmp )
				StartTimeTmp = TimeTmp;
			}	
		StartTime = StartTimeTmp;
		ArrangeStartTime();
		}	
	if ( FinishTimeTmp > FinishTime )
		{
		TConnectsIterator ExitsIterator( *Exits );
		while ( ExitsIterator )
			{
			TimeTmp = ((TEvent*)ExitsIterator++->GetWorkFinish())->GetStartTime();
			if ( FinishTimeTmp > TimeTmp )
				FinishTimeTmp = TimeTmp;
			}	
		FinishTime = FinishTimeTmp;		
		ArrangeFinishTime();
		}
	}

bool TEvent::FindForPoint( TLPoint& lThePoint, unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		return lThePoint.x >= lPoint.x - GetHConstSizeMDL() && 
				 lThePoint.x <  lPoint.x + GetHConstSizeMDL() &&
				 lThePoint.y >= lPoint.y - GetVConstSizeTopMDL() && 
				 lThePoint.y <  lPoint.y + GetVConstSizeBottomMDL();
	return lThePoint.x >= StartTime - TheMinTime &&
			 lThePoint.x <  FinishTime - TheMinTime &&
			 lThePoint.y >= lPoint.y - GetVConstSizeTopGNT() && 
			 lThePoint.y <  lPoint.y + GetVConstSizeBottomGNT();
	}

bool TEvent::FindForDelete( TLPoint& lPointDown, TLPoint& lPointUp, unsigned long TheMinTime )
	{
	TLPoint lPointStart; 
	TLPoint lPointFinish;
	lPointStart = lPointDown;
	lPointFinish = lPointUp;
	return Clip( GetRect( TheMinTime ), lPointStart, lPointFinish );				  
	}

TLRect TEvent::GetRect( unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		return TLRect( lPoint.x - GetHConstSizeMDL(), lPoint.y - GetVConstSizeTopMDL(),
							lPoint.x + GetHConstSizeMDL(), lPoint.y + GetVConstSizeBottomMDL() );
	return TLRect( StartTime - TheMinTime, lPoint.y - GetVConstSizeTopGNT(),
						FinishTime - TheMinTime, lPoint.y + GetVConstSizeBottomGNT() );
	}

TLPoint TEvent::GetStartPoint( unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		return TLPoint( lPoint.x + GetHConstSizeMDL(), lPoint.y );
	return TLPoint( FinishTime - TheMinTime, lPoint.y );
	}

TLPoint TEvent::GetFinishPoint( unsigned long TheMinTime )
	{
	if ( TheMinTime == 0 )
		return TLPoint( lPoint.x - GetHConstSizeMDL(), lPoint.y );
	return TLPoint( StartTime - TheMinTime, lPoint.y );
	}



