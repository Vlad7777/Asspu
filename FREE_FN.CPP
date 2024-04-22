#include <time.h>
#include <owl\owlpch.h>
#include <owl\scroller.h>
#include <owl\opensave.h>                                          
#include <owl\edit.h>                                          
#include <dir.h>

#include "bas_obj.h"
#include "cal_wind.h"

int Code( TLPoint& lThePoint, TLRect& lTheRect )
	{
	return ( ( lThePoint.x < lTheRect.left ) << 3 ) |
			 ( ( lThePoint.x > lTheRect.right ) << 2 ) |
			 ( ( lThePoint.y < lTheRect.top ) << 1 ) |
			 ( ( lThePoint.y > lTheRect.bottom ) );
	}

bool Clip( TLRect& lTheRect, TLPoint& lThePointStart, TLPoint& lThePointFinish )
	{
	int CodStart = Code( lThePointStart, lTheRect );
	int CodFinish = Code( lThePointFinish, lTheRect );
	while( CodStart | CodFinish )
		{
		if ( CodStart & CodFinish )
			return FALSE;
		long lDelx = lThePointFinish.x - lThePointStart.x;
		long lDely = lThePointFinish.y - lThePointStart.y;
		if ( CodStart )
			{
			if ( lThePointStart.x < lTheRect.left )
				{
				lThePointStart.y += (long)( (float)lDely * ( lTheRect.left - lThePointStart.x ) / lDelx );
				lThePointStart.x = lTheRect.left;
				}
			else
				if ( lThePointStart.x > lTheRect.right )
					{
					lThePointStart.y += (long)( (float)lDely * ( lTheRect.right - lThePointStart.x ) / lDelx );
					lThePointStart.x = lTheRect.right;
					}
				else
					if ( lThePointStart.y < lTheRect.top )
						{
						lThePointStart.x += (long)( (float)lDelx * ( lTheRect.top - lThePointStart.y ) / lDely );
						lThePointStart.y = lTheRect.top;
						}
					else
						if ( lThePointStart.y > lTheRect.bottom )
							{
							lThePointStart.x += (long)( (float)lDelx * ( lTheRect.bottom - lThePointStart.y ) / lDely );
							lThePointStart.y = lTheRect.bottom;
							}
			CodStart = Code( lThePointStart, lTheRect );
			}
		else
			{
			if ( lThePointFinish.x < lTheRect.left )
				{
				lThePointFinish.y += (long)( (float)lDely * ( lTheRect.left - lThePointFinish.x ) / lDelx );
				lThePointFinish.x = lTheRect.left;
				}
			else
				if ( lThePointFinish.x > lTheRect.right )
					{
					lThePointFinish.y += (long)( (float)lDely * ( lTheRect.right - lThePointFinish.x ) / lDelx );
					lThePointFinish.x = lTheRect.right;
					}
				else
					if ( lThePointFinish.y < lTheRect.top )
						{
						lThePointFinish.x += (long)( (float)lDelx * ( lTheRect.top - lThePointFinish.y ) / lDely );
						lThePointFinish.y = lTheRect.top;
						}
					else
						if ( lThePointFinish.y > lTheRect.bottom )
							{
							lThePointFinish.x += (long)( (float)lDelx * ( lTheRect.bottom - lThePointFinish.y ) / lDely );
							lThePointFinish.y = lTheRect.bottom;
							}
			CodFinish = Code( lThePointFinish, lTheRect );
			}
		}
	return TRUE;
	}

long StartMonth( long TheTime )
	{
	long TimeTmp = TheTime - TheTime % TEvent::StrSetScale.Day;
	return TimeTmp - ( localtime( &(long)TimeTmp )->tm_mday - 1 ) * TEvent::StrSetScale.Day;
	}

long NextMonth( long TheTime )
	{
	long TimeTmp = TheTime;
	int Month = localtime( &(long)TimeTmp )->tm_mon;
	TimeTmp += 27 * TEvent::StrSetScale.Day;
	if ( Month == localtime( &(long)TimeTmp )->tm_mon )
		TimeTmp += 27 * TEvent::StrSetScale.Day;
	return StartMonth( TimeTmp );
	}

long StartYear( long TheTime )
	{
	long TimeTmp = TheTime - TheTime % TEvent::StrSetScale.Day;
	return TimeTmp - localtime( &(long)TimeTmp )->tm_yday * TEvent::StrSetScale.Day;
	}

long NextYear( long TheTime )
	{
	long TimeTmp = TheTime;
	int Year = localtime( &(long)TimeTmp )->tm_year;
	TimeTmp += 360 * TEvent::StrSetScale.Day;
	if ( Year == localtime( &(long)TimeTmp )->tm_year )
		TimeTmp += 360 * TEvent::StrSetScale.Day;
	return StartYear( TimeTmp );
	}

void DrawCalendar( TDC& dc, TLSize& lTheSize, TLRect* lTheRect, unsigned long TheMinTime )
	{
	dc.SelectObject( TPen( &TCALWindow::StrSetCalendar.LogPenMain ) );
   dc.SelectObject( TPen( TColor::Black, 1 ));
	dc.SetTextColor( TColor( TCALWindow::StrSetCalendar.ColorRefFont ) );
	dc.SelectObject( TFont( &TCALWindow::StrSetCalendar.LogFont ) );

	long StepTime;
	long TimeTmp;	
	long TimeOldTmp;
	char string[255];
	struct tm *tblock;
	long StartTimeTmp = lTheRect->left + TheMinTime;
	long FinishTimeTmp = lTheRect->right + TheMinTime;
	TIRect iRect( 0, 0, 0, 19 );

	if ( TEvent::StrSetScale.ScaleMinute )
		{
		StepTime = TEvent::StrSetScale.Day / 24L;
		TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
		TimeOldTmp = TimeTmp - StepTime;
		iRect.Set( 0, 0, 0, 19 );
		iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
		while ( TimeTmp < FinishTimeTmp + StepTime )
			{
			iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
			dc.MoveTo( iRect.right + 1, iRect.top );
			dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
			tblock = localtime( &(long)TimeOldTmp );
			strftime( string, 25, "%Hh %d %B %Y", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			TimeOldTmp = TimeTmp;
			TimeTmp += StepTime;
			iRect.left = iRect.right + 1;
			}
		
		StepTime = TEvent::StrSetScale.Day / 1440UL;
		TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
		TimeOldTmp = TimeTmp - StepTime;
		iRect.Set( 0, 20, 0, 39 );     //   was   (  0, 20, 0, 39)  !!!!!
		iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
		while ( TimeTmp < FinishTimeTmp + StepTime )
			{
			iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
			dc.MoveTo( iRect.right + 1, iRect.top );
			dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
			tblock = localtime( &(long)TimeOldTmp );
			strftime( string, 25, "%M", tblock );
			dc.ExtTextOut( iRect.left + 2,
								iRect.top + 1,
								ETO_CLIPPED,
								&iRect,
								string,
								lstrlen( string ) );
			TimeOldTmp = TimeTmp;
			TimeTmp += StepTime;
			iRect.left = iRect.right + 1;
			}
		}
	else
		if ( TEvent::StrSetScale.ScaleHour )
			{
			StepTime = TEvent::StrSetScale.Day;
			TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
			TimeOldTmp = TimeTmp - StepTime;
			iRect.Set( 0, 0, 0, 19 );
			iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
			while ( TimeTmp < FinishTimeTmp + StepTime )
				{
				iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
				dc.MoveTo( iRect.right + 1, iRect.top );
				dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
				tblock = localtime( &(long)TimeOldTmp );
				strftime( string, 25, "%d %B %Y", tblock );
				dc.ExtTextOut( iRect.left + 2,
									iRect.top + 1,
									ETO_CLIPPED,
									&iRect,
									string,
									lstrlen( string ) );
				TimeOldTmp = TimeTmp;
				TimeTmp += StepTime;
				iRect.left = iRect.right + 1;
				}

			StepTime = TEvent::StrSetScale.Day / 24L;
			TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
			TimeOldTmp = TimeTmp - StepTime;
			iRect.Set( 0, 20, 0, 39 );  //
			iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
			while ( TimeTmp < FinishTimeTmp + StepTime )
				{
				iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
				dc.MoveTo( iRect.right + 1, iRect.top );
				dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
				tblock = localtime( &(long)TimeOldTmp );
				strftime( string, 25, "%H", tblock );
				dc.ExtTextOut( iRect.left + 2,
									iRect.top + 1,
									ETO_CLIPPED,
									&iRect,
									string,
									lstrlen( string ) );
				TimeOldTmp = TimeTmp;
				TimeTmp += StepTime;
				iRect.left = iRect.right + 1;
				}
			}
		else
			if ( TEvent::StrSetScale.ScaleDay )
				{
				StepTime = TEvent::StrSetScale.Day * 32;
				TimeTmp = NextMonth( StartTimeTmp );
				TimeOldTmp = StartMonth( StartTimeTmp );
				iRect.Set( 0, 0, 0, 19 );
				iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
				while ( TimeTmp < FinishTimeTmp + StepTime  )
					{
					iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
					dc.MoveTo( iRect.right + 1, iRect.top );
					dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
					tblock = localtime( &(long)TimeOldTmp );
					strftime( string, 25, "%B %Y", tblock );
					dc.ExtTextOut( iRect.left + 2,
										iRect.top + 1,
										ETO_CLIPPED,
										&iRect,
										string,
										lstrlen( string ) );
					TimeOldTmp = TimeTmp;
					TimeTmp = NextMonth( TimeTmp );
					iRect.left = iRect.right + 1;
					}

				StepTime = TEvent::StrSetScale.Day;
				TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
				TimeOldTmp = TimeTmp - StepTime;
				iRect.Set( 0, 20, 0, 39 );	    // was  	iRect.Set( 0, 20, 0, 39 );
				iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
				while ( TimeTmp < FinishTimeTmp + StepTime )
					{
					iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
					dc.MoveTo( iRect.right + 1, iRect.top );
					dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
					tblock = localtime( &(long)TimeOldTmp );
					/////////////////////////////// mY //////////////////////////
					if ( tblock->tm_wday == 0 || tblock->tm_wday == 6 )
					{
					dc.FillRect( iRect.left + 1, iRect.top,
					             iRect.right + 1, iRect.bottom,TBrush(TColor::LtRed));
					dc.SetBkColor(RGB(255, 0, 0));
					}
					// !!! RESTORE  TColor !!???
					////////////////////////////////////////////////////////////
					strftime( string, 25, "%d", tblock );
					dc.ExtTextOut( iRect.left + 2,
										iRect.top + 1,
										ETO_CLIPPED,
										&iRect,
										string,
										lstrlen( string ) );
					//////////////////////My///////////////////////////////////
					dc.SetBkColor(RGB(255, 255, 255));
					///////////////////////////////////////////////////////////
					TimeOldTmp = TimeTmp;
					TimeTmp += StepTime;
					iRect.left = iRect.right + 1;
					}
				}
			else
				if ( TEvent::StrSetScale.ScaleMonth )
					{
					StepTime = TEvent::StrSetScale.Day * 367;
					TimeTmp = NextYear( StartTimeTmp );
					TimeOldTmp = StartYear( StartTimeTmp );
					iRect.Set( 0, 0, 0, 19 );
					iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
					while ( TimeTmp < FinishTimeTmp + StepTime  )
						{
						iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
						dc.MoveTo( iRect.right + 1, iRect.top );
						dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
						tblock = localtime( &(long)TimeOldTmp );
						strftime( string, 25, "%Y", tblock );
						dc.ExtTextOut( iRect.left + 2,
											iRect.top + 1,
											ETO_CLIPPED,
											&iRect,
											string,
											lstrlen( string ) );
						TimeOldTmp = TimeTmp;
						TimeTmp = NextYear( TimeTmp );
						iRect.left = iRect.right + 1;
						}

					StepTime = TEvent::StrSetScale.Day * 32;
					TimeTmp = NextMonth( StartTimeTmp );
					TimeOldTmp = StartMonth( StartTimeTmp );
					iRect.Set( 0, 20, 0, 39 );
					iRect.left = (int)( ( TimeOldTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
					while ( TimeTmp < FinishTimeTmp + StepTime  )
						{
						iRect.right = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ) - 1;
						dc.MoveTo( iRect.right + 1, iRect.top );
						dc.LineTo( iRect.right + 1, iRect.bottom + 1 );
						tblock = localtime( &(long)TimeOldTmp );
						strftime( string, 25, "%B", tblock );
						dc.ExtTextOut( iRect.left + 2,
											iRect.top + 1,
											ETO_CLIPPED,
											&iRect,
											string,
											lstrlen( string ) );
						TimeOldTmp = TimeTmp;
						TimeTmp = NextMonth( TimeTmp );
						iRect.left = iRect.right + 1;
						}
					}
				else
					if ( TEvent::StrSetScale.ScaleYear )
						{
						}

	dc.MoveTo( (int)( lTheRect->left / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 20 );
	dc.LineTo( (int)( lTheRect->right / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 20 );
	dc.MoveTo( (int)( lTheRect->left / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 39 );
	dc.LineTo( (int)( lTheRect->right / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 39 );

//	dc.MoveTo( (int)( lTheRect->left / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 59 );  // New
//	dc.LineTo( (int)( lTheRect->right / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx ), 59 ); //  New
	dc.RestorePen();
	dc.RestoreFont();
	}


void DrawNet( TDC& dc, TLSize& lTheSize, TLRect* lTheRect, unsigned long TheMinTime )
	{
	dc.SelectObject( TPen( &TCALWindow::StrSetCalendar.LogPenMain ) );

	long StepTime;
	long TimeTmp;
	int CurentX;
	long StartTimeTmp = lTheRect->left + TheMinTime;
	long FinishTimeTmp = lTheRect->right + TheMinTime;
	TRect iRect( 0, 0, 0, 19 );

	if ( TEvent::StrSetScale.ScaleMinute )
		{
		StepTime = TEvent::StrSetScale.Day / 1440UL ;
		TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
		while ( TimeTmp < FinishTimeTmp )
			{
			CurentX = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
			dc.MoveTo( CurentX, (int)( lTheRect->top - lTheSize.cy ) );
			dc.LineTo( CurentX, (int)( lTheRect->bottom - lTheSize.cy ) );
			TimeTmp += StepTime;
			}
		}
	else
		if ( TEvent::StrSetScale.ScaleHour )
			{
			StepTime = TEvent::StrSetScale.Day / 24L;
			TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
			while ( TimeTmp < FinishTimeTmp )
				{
				CurentX = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
				dc.MoveTo( CurentX, (int)( lTheRect->top - lTheSize.cy ) );
				dc.LineTo( CurentX, (int)( lTheRect->bottom - lTheSize.cy ) );
				TimeTmp += StepTime;
				}
			}	
		else
			if ( TEvent::StrSetScale.ScaleDay )
				{
				StepTime = TEvent::StrSetScale.Day;
				TimeTmp = StartTimeTmp - ( StartTimeTmp % StepTime );
				while ( TimeTmp < FinishTimeTmp )
					{
					CurentX = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
					dc.MoveTo( CurentX, (int)( lTheRect->top - lTheSize.cy ) );
					dc.LineTo( CurentX, (int)( lTheRect->bottom - lTheSize.cy ) );
					TimeTmp += StepTime;
					}
				}
			else
				if ( TEvent::StrSetScale.ScaleMonth )
					{
					StepTime = TEvent::StrSetScale.Day;
					TimeTmp = NextMonth( StartTimeTmp );
					while ( TimeTmp < FinishTimeTmp )
						{
						CurentX = (int)( ( TimeTmp - (long)TheMinTime ) / TEvent::StrSetScale.HScaleGNTGST - lTheSize.cx );
						dc.MoveTo( CurentX, (int)( lTheRect->top - lTheSize.cy ) );
						dc.LineTo( CurentX, (int)( lTheRect->bottom - lTheSize.cy ) );
						TimeTmp = NextMonth( TimeTmp );
						}
					}
				else
					if ( TEvent::StrSetScale.ScaleYear )
						{
						}		
	} 

void TSBitTimes::Sum( TSBitTimes* TheBitTimes )
	{
	unsigned long OldResourceTmp = 0L;	
	unsigned long TimeTmp;	
	TBitTime* BitTimeTmp;	
	unsigned long OldResourceTmpD = 0;
	unsigned long OldTimeTmpD = 0;
	unsigned long NewTimeTmp = 0;
	TBitTime* BitTimeTmpD;
	TDBitTimes* DBitTimesTmp = new TDBitTimes();

	TSBitTimesIterator BitTimesIterator( *this );
	while ( BitTimesIterator )
		DBitTimesTmp->AddAtTail( BitTimesIterator++ );

	TSBitTimesIterator TheBitTimesIterator( *TheBitTimes );
	while ( TheBitTimesIterator )
		{            
		BitTimeTmp = TheBitTimesIterator++;
		TimeTmp = BitTimeTmp->GetTime();
		while ( !DBitTimesTmp->IsEmpty() )
			{
			BitTimeTmpD = DBitTimesTmp->PeekHead();
			NewTimeTmp = BitTimeTmpD->GetTime();
			if ( TimeTmp > NewTimeTmp )
				{
				OldResourceTmpD = BitTimeTmpD->GetResource();
				OldTimeTmpD = BitTimeTmpD->GetTime();
				BitTimeTmpD->AddResource( OldResourceTmp );
				DBitTimesTmp->DetachAtHead();
				}
			else
				if ( TimeTmp > OldTimeTmpD && TimeTmp < NewTimeTmp )
					{
					BitTimeTmpD = new TBitTime( BitTimeTmp );
					BitTimeTmpD->AddResource( OldResourceTmpD );
					this->Add( BitTimeTmpD );
					break;
					}
				else
					if ( TimeTmp == NewTimeTmp )
						{
						OldResourceTmpD = BitTimeTmpD->GetResource();
						OldTimeTmpD = BitTimeTmpD->GetTime();
						BitTimeTmpD->AddResource( BitTimeTmp );
						DBitTimesTmp->DetachAtHead();	
						break;
						}
			}
		if ( DBitTimesTmp->IsEmpty() && TimeTmp != NewTimeTmp )
			this->Add( new TBitTime( BitTimeTmp ) );
		OldResourceTmp = BitTimeTmp->GetResource();	
		}
	DBitTimesTmp->Flush();	
	delete DBitTimesTmp;
	}

void TSBitTimes::Sub( TSBitTimes* TheBitTimes )
	{
	unsigned long OldResourceTmp = 0L;	
	unsigned long TimeTmp;	
	TBitTime* BitTimeTmp;	
	unsigned long NewTimeTmp;    
	TBitTime* BitTimeTmpD;
	TDBitTimes* DBitTimesTmp = new TDBitTimes();

	TSBitTimesIterator BitTimesIterator( *this );
	while ( BitTimesIterator )
		DBitTimesTmp->AddAtTail( BitTimesIterator++ );

	TSBitTimesIterator TheBitTimesIterator( *TheBitTimes );
	while ( TheBitTimesIterator )
		{
		BitTimeTmp = TheBitTimesIterator++;
		TimeTmp = BitTimeTmp->GetTime();
		while ( !DBitTimesTmp->IsEmpty() )
			{
			BitTimeTmpD = DBitTimesTmp->PeekHead();
			NewTimeTmp = BitTimeTmpD->GetTime();
			if ( TimeTmp > NewTimeTmp )
				{
				BitTimeTmpD->DelResource( OldResourceTmp );
				DBitTimesTmp->DetachAtHead();
				}
			else
				if ( TimeTmp == NewTimeTmp )
					{
					DBitTimesTmp->DetachAtHead();
					BitTimeTmpD->DelResource( BitTimeTmp );
					if ( BitTimeTmpD->GetQuantity() == 0L )
						this->Detach( BitTimeTmpD, TRUE );
					break;
					}	
			}
		OldResourceTmp = BitTimeTmp->GetResource();
		}
	DBitTimesTmp->Flush();	
	delete DBitTimesTmp;
	}


