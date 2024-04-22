#include <math.h>

#include "pack.h"

STR_SET_CONNECT TConnect::StrSetConnect = { 10, 0.14, { 0, 1, 0, 0L }, { 0, 1, 0, 255L } };

TConnect::TConnect( TWork* TheWorkStart, TWork* TheWorkFinish, TPack* ThePackStart, TPack* ThePackFinish )
	{
	PackStart = NULL;
	PackFinish = NULL;
	SetStart( TheWorkStart );
	SetFinish( TheWorkFinish );
	SetStart( ThePackStart );
	SetFinish( ThePackFinish );
	}

TConnect::TConnect()
	{
	PackStart = NULL;
	PackFinish = NULL;
	}

TConnect::~TConnect()
	{
	ResetPack();
	if ( WorkStart )
		WorkStart->DetachExit( this );
	if ( WorkFinish )
		WorkFinish->DetachEntrance( this );
	}

void TConnect::ResetPack()
	{
	if ( PackStart )
		{
		PackStart->DetachExit( this );
		PackStart = NULL;
		}
	if ( PackFinish )
		{
		PackFinish->DetachEntrance( this );
		PackFinish = NULL;
		}
	}

void TConnect::SetStart( TPack* ThePack )
	{
	PackStart = ThePack;
	if ( PackStart  )
		PackStart->AddExit( this );
	}
	
void TConnect::SetFinish( TPack* ThePack )
	{
	PackFinish = ThePack;
	if ( PackFinish )
		PackFinish->AddEntrance( this );
	}

void TConnect::SetStart( TWork* TheWork )
	{
	WorkStart = TheWork;
	WorkStart->AddExit( this );
	}

void TConnect::SetFinish( TWork* TheWork )
	{
	WorkFinish = TheWork;
	WorkFinish->AddEntrance( this );
	}

void TConnect::DrawArrow( TDC& dc, TIPoint& iThePointStart, TIPoint& iThePointFinish )
	{
	TIPoint iPointStart;
	int SizeArrow = StrSetConnect.ConstSizeArrow / TEvent::StrSetScale.ScaleMDL;
	long lDelx = iThePointFinish.x - iThePointStart.x;
	long lDely = iThePointFinish.y - iThePointStart.y;
	double result = ( lDely != 0 ) ? atan( (double)lDelx / lDely ) : M_PI_2;
	if ( lDelx > 0 )
		if ( lDely >= 0 )
			{
			iPointStart.x = (int)( iThePointFinish.x - SizeArrow * sin( result - StrSetConnect.ConstAngjeArrow ) );
			iPointStart.y = (int)( iThePointFinish.y - SizeArrow * cos( result - StrSetConnect.ConstAngjeArrow ) );
			}
		else
			{
			iPointStart.x = (int)( iThePointFinish.x - SizeArrow * sin( -( result - StrSetConnect.ConstAngjeArrow ) ) );
			iPointStart.y = (int)( iThePointFinish.y + SizeArrow * cos( -( result - StrSetConnect.ConstAngjeArrow ) ) );
			}
	else
		if ( lDely > 0 )
			{
			iPointStart.x = (int)( iThePointFinish.x + SizeArrow * sin( -( result-StrSetConnect.ConstAngjeArrow ) ) );
			iPointStart.y = (int)( iThePointFinish.y - SizeArrow * cos( -( result-StrSetConnect.ConstAngjeArrow ) ) );
			}
		else
			{
			iPointStart.x = (int)( iThePointFinish.x + SizeArrow * sin( result - StrSetConnect.ConstAngjeArrow ) );
			iPointStart.y = (int)( iThePointFinish.y + SizeArrow * cos( result - StrSetConnect.ConstAngjeArrow ) );
			}
	dc.MoveTo( iPointStart );
	dc.LineTo( iThePointFinish );
	if ( lDelx > 0 )
		if ( lDely >= 0 )
			{
			iPointStart.x = (int)( iThePointFinish.x - SizeArrow * sin( result + StrSetConnect.ConstAngjeArrow ) );
			iPointStart.y = (int)( iThePointFinish.y - SizeArrow * cos( result + StrSetConnect.ConstAngjeArrow ) );
			}
		else
			{
			iPointStart.x = (int)( iThePointFinish.x - SizeArrow * sin( -( result + StrSetConnect.ConstAngjeArrow ) ) );
			iPointStart.y = (int)( iThePointFinish.y + SizeArrow * cos( -( result + StrSetConnect.ConstAngjeArrow ) ) );
			}
	else
		if ( lDely > 0 )
			{
			iPointStart.x = (int)( iThePointFinish.x + SizeArrow * sin( -( result + StrSetConnect.ConstAngjeArrow ) ) );
			iPointStart.y = (int)( iThePointFinish.y - SizeArrow * cos( -( result + StrSetConnect.ConstAngjeArrow ) ) );
			}
		else
			{
			iPointStart.x = (int)( iThePointFinish.x + SizeArrow * sin( result + StrSetConnect.ConstAngjeArrow ) );
			iPointStart.y = (int)( iThePointFinish.y + SizeArrow * cos( result + StrSetConnect.ConstAngjeArrow ) );
			}
	dc.MoveTo( iPointStart );
	dc.LineTo( iThePointFinish );
	}

void TConnect::Draw( TDC& dc, TLSize& lTheSize, TLRect* lTheRect, unsigned long TheMinTime )
	{
	TLPoint lPointStart;
	TLPoint lPointFinish;
	TLPoint lPointFinishD;
	lPointStart = GetEventStart()->GetStartPoint( TheMinTime );
	lPointFinish = lPointFinishD = GetEventFinish()->GetFinishPoint( TheMinTime );
	if ( Clip( *lTheRect, lPointStart, lPointFinish ) )
		{
		if ( GetWorkStart()->GetCritical() == CRITICAL_MAIN && GetWorkFinish()->GetCritical() == CRITICAL_MAIN )
			dc.SelectObject( TPen( &StrSetConnect.LogPenCrit ) );
		else
			dc.SelectObject( TPen( &StrSetConnect.LogPenMain ) );

		TIPoint iPointStart;
		TIPoint iPointFinish;
		if ( TheMinTime == 0 )
			{
			iPointStart = lPointStart / TEvent::StrSetScale.ScaleMDL - lTheSize;
			iPointFinish = lPointFinish / TEvent::StrSetScale.ScaleMDL - lTheSize;
			if ( lPointFinish == lPointFinishD )
				DrawArrow( dc, iPointStart, iPointFinish );
			}
		else
			{
			iPointStart.x = (int)( (long)( lPointStart.x / TEvent::StrSetScale.HScaleGNTGST ) - lTheSize.cx );
			iPointStart.y = (int)( (long)( lPointStart.y / TEvent::StrSetScale.VScaleGNT ) - lTheSize.cy );
			iPointFinish.x = (int)( (long)( lPointFinish.x / TEvent::StrSetScale.HScaleGNTGST ) - lTheSize.cx );
			iPointFinish.y = (int)( (long)( lPointFinish.y / TEvent::StrSetScale.VScaleGNT ) - lTheSize.cy );
			}
		dc.MoveTo( iPointStart );
		dc.LineTo( iPointFinish );
		dc.RestorePen();
		}
	}

bool TConnect::FindForDelete( TLPoint& lPointDown, TLPoint& lPointUp, unsigned long TheMinTime )
	{
	TLPoint lPointStart = GetEventStart()->GetStartPoint( TheMinTime );
	TLPoint lPointFinish = GetEventFinish()->GetFinishPoint( TheMinTime );
	if ( ( lPointDown.x != lPointUp.x || lPointDown.y != lPointUp.y ) && ( lPointStart.x != lPointFinish.x || lPointStart.y != lPointFinish.y ) )
		{
		TLPoint lPointReturn;
		float A1 = lPointDown.y - lPointUp.y;
		float B1 = lPointUp.x - lPointDown.x;
		float C1 = lPointDown.y * ( lPointDown.x - lPointUp.x ) + lPointDown.x * ( lPointUp.y - lPointDown.y );
		float A2 = lPointStart.y - lPointFinish.y;
		float B2 = lPointFinish.x - lPointStart.x;
		float C2 = lPointStart.y * ( lPointStart.x - lPointFinish.x ) + lPointStart.x * ( lPointFinish.y - lPointStart.y );

		if ( ( A2 != 0 && B2 != 0 && A1 / A2 != B1 / B2 ) ||
			  ( A1 != 0 && B1 != 0 && A2 / A1 != B2 / B1 ) ||
			  ( A2 == 0 && B1 == 0 ) || ( A1 == 0 && B2 == 0 ) )
			{
			lPointReturn.x = (long)(( C1 * B2 - C2 * B1 ) / ( B1 * A2 - B2 * A1 ));
			lPointReturn.y = (long)(( C2 * A1 - C1 * A2 ) / ( B1 * A2 - B2 * A1 ));
			return( ( ( ( lPointReturn.x >= lPointDown.x && lPointReturn.x <= lPointUp.x ) ||
							( lPointReturn.x <= lPointDown.x && lPointReturn.x >= lPointUp.x ) ) &&
						 ( ( lPointReturn.x >= lPointStart.x && lPointReturn.x <= lPointFinish.x ) ||
							( lPointReturn.x <= lPointStart.x && lPointReturn.x >= lPointFinish.x ) ) ) &&
					  ( ( ( lPointReturn.y >= lPointDown.y && lPointReturn.y <= lPointUp.y ) ||
							( lPointReturn.y <= lPointDown.y && lPointReturn.y >= lPointUp.y ) ) &&
						 ( ( lPointReturn.y >= lPointStart.y && lPointReturn.y <= lPointFinish.y ) ||
							( lPointReturn.y <= lPointStart.y && lPointReturn.y >= lPointFinish.y ) ) ) );
			}
		}	
	return FALSE;
	}

void TConnect::write( opstream& os ) const
	{
	if ( PackStart != NULL )
		os << PackStart->GetTmp() << WorkStart->GetTmp();
	else
		os << 0L << WorkStart->GetTmp();
	if ( PackFinish != NULL )
		os << PackFinish->GetTmp() << WorkFinish->GetTmp();
	else
		os << 0L << WorkFinish->GetTmp();
	}

void* TConnect::read( ipstream& is, TEvent** TheArrayEvents )
	{
	unsigned long Tmp;
	is  >> Tmp;
	if ( Tmp == 0L )
		SetStart( (TPack*)NULL );
	else	
		SetStart( (TPack*)*( TheArrayEvents + Tmp - 1L ) );	
	is  >> Tmp;
	SetStart( (TWork*)*( TheArrayEvents + Tmp - 1L ) );
	is  >> Tmp;
	if ( Tmp == 0L )
		SetFinish( (TPack*)NULL );
	else	
		SetFinish( (TPack*)*( TheArrayEvents + Tmp - 1L ) );
	is  >> Tmp;
	SetFinish( (TWork*)*( TheArrayEvents + Tmp - 1L ) );
	return this;
	}


