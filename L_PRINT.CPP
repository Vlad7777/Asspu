#include <owl\owlpch.h>
#include <owl\printer.h>
#include <owl\checkbox.h>
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "layout.h"
#include "mainw4.h"
#include "mainw4.rh"
#include "cal_wind.h"

//
// Abort procedure used during printing
//
bool FAR PASCAL _export TPrinterAbortProc(HDC, short)
	{
	TApplication* app = GetApplicationObject();
	app->PumpWaitingMessages();
	return !TPrinter::GetUserAbort(); // UserAbort will have been set in the AbortDialog
	}

struct STR_PRINT_DIALOG
	{
	bool MDLWindow;
	bool GNTWindow;
	bool GSTWindow;
	};

class TPrintWindowDialog : public TDialog
	{
	public:
		TPrintWindowDialog( TWindow* parent, int resId, STR_PRINT_DIALOG& TheStrPrintDialog );
	};

TPrintWindowDialog::TPrintWindowDialog( TWindow* parent, int resId, STR_PRINT_DIALOG& TheStrPrintDialog )
	: TDialog( parent, resId )
	{
	new TCheckBox( this, ID_MDLCHECK, 0 );
	new TCheckBox( this, ID_GNTCHECK, 0 );
	new TCheckBox( this, ID_GSTCHECK, 0 );

	SetTransferBuffer( &TheStrPrintDialog );
	}

bool TMyLayout::Print()
	{
	STR_PRINT_DIALOG StrPrintDialog;
	StrPrintDialog.MDLWindow = TRUE;
	StrPrintDialog.GNTWindow = TRUE;
	StrPrintDialog.GSTWindow = TRUE;

	if ( TMyApp::Printer && TPrintWindowDialog( this, IDD_PRINT_WINDOW, StrPrintDialog ).Execute() == IDOK )
		{
		TPrintDC* PrnDC;
		PrnDC = new TPrintDC( TMyApp::Printer->GetSetup().GetDriverName(),
									 TMyApp::Printer->GetSetup().GetDeviceName(),
									 TMyApp::Printer->GetSetup().GetOutputName(),
									 TMyApp::Printer->GetSetup().GetDevMode() );
		TWindow* AbortWin = new TPrinterAbortDlg( this,
																IDD_ABORTDIALOG,
																FileName,
																TMyApp::Printer->GetSetup().GetDeviceName(),
																TMyApp::Printer->GetSetup().GetOutputName() );
		AbortWin->Create();

		ABORTPROC abortProcInst = (ABORTPROC)MakeProcInstance( (FARPROC)TPrinterAbortProc, *GetModule() );
		PrnDC->SetAbortProc( abortProcInst );
		PrnDC->StartDoc( FileName, 0 );

		TISize iPrintPage( PrnDC->GetDeviceCaps( HORZRES ), PrnDC->GetDeviceCaps( VERTRES ) );
		TLSize lPrintSheet( 0L, 0L );
		TLRect lRect;
		TLRect lRectTmp;
		int   HeightFont = 50 ;   // New

		if ( StrPrintDialog.MDLWindow )
			{
			TLPoint lPoint( 0L, 0L );
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				lPoint = DPacksIterator++->GetPoint();
				if ( lPrintSheet.cx < lPoint.x )
					lPrintSheet.cx = lPoint.x;
				if ( lPrintSheet.cy < lPoint.y )
					lPrintSheet.cy = lPoint.y;
				}
			TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				lPoint = DWorksIterator++->GetPoint();
				if ( lPrintSheet.cx < lPoint.x )
					lPrintSheet.cx = lPoint.x;
				if ( lPrintSheet.cy < lPoint.y )
					lPrintSheet.cy = lPoint.y;
				}
			lPrintSheet.cx += Max( TPack::StrSetEvent.HConstSizeMDL, TWork::StrSetEvent.HConstSizeMDL );
			lPrintSheet.cy += Max( TPack::StrSetEvent.VConstSizeBottomMDL, TWork::StrSetEvent.VConstSizeBottomMDL );
			lPrintSheet.cx /= TEvent::StrSetScale.ScaleMDL;
			lPrintSheet.cy /= TEvent::StrSetScale.ScaleMDL;
			int QuantityPageX = (int)lPrintSheet.cx / iPrintPage.cx + 1;
			int QuantityPageY = (int)lPrintSheet.cy / iPrintPage.cy + 1;


			TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
			for ( int CurentPageX = 0;/* PrnDC->QueryAbort() &&*/ CurentPageX < QuantityPageX; CurentPageX++ )
				for ( int CurentPageY = 0;/* PrnDC->QueryAbort() && */CurentPageY < QuantityPageY; CurentPageY++ )
					{
					PrnDC->StartPage();

					lRect.Set( 0L, 0L, (long)iPrintPage.cx, (long)iPrintPage.cy );
					TLSize lSize( (long)CurentPageX * iPrintPage.cx, (long)CurentPageY * iPrintPage.cy );
					lRect += lSize;
					lRect *= TEvent::StrSetScale.ScaleMDL;
					lRect.left -=  Max( TPack::StrSetEvent.HConstSizeMDL, TWork::StrSetEvent.HConstSizeMDL );
					lRect.right += Max( TPack::StrSetEvent.HConstSizeMDL, TWork::StrSetEvent.HConstSizeMDL );
					lRect.top -= Max( TPack::StrSetEvent.VConstSizeBottomMDL, TWork::StrSetEvent.VConstSizeBottomMDL ) + 1;
					lRect.bottom += Max( TPack::StrSetEvent.VConstSizeTopMDL, TWork::StrSetEvent.VConstSizeTopMDL ) + 1;

///////////////////////////////////////////////////////////////////////////////////////
					if ( CurentPageY == 0 )
						{

//						char  S[80];
//						strcpy(S," Мой экземпляр ");
//						PrnDC->TextOut(50,0, S, strlen (S));
/////////////////////////////////////////////////////////////
						lRect.top = 0;
						lRect.bottom = (long)( ( iPrintPage.cy - HeightFont - 5 ) /*  * TEvent::StrSetScale.VScaleGNT  */ );
						TIPoint Point( 0, - HeightFont - 5 );
						PrnDC->OffsetWindowOrg( Point );
						}
					else
						{
		  //				lSize.cy = (long)CurentPageY * iPrintPage.cy - 40;
		  //				lRect.top = lRect.bottom;
		  //				lRect.bottom += iPrintPage.cy * TEvent::StrSetScale.VScaleGNT;
						}
////////////////////////////////////////////////////////////////////////////////////////
					ConnectsIterator.Restart();
					while ( ConnectsIterator )
						ConnectsIterator++->Draw( *PrnDC, lSize, &lRect );
					DPacksIterator.Restart();
					while ( DPacksIterator )
						DPacksIterator++->Draw( *PrnDC, lSize, &lRect );
					DWorksIterator.Restart();
					while ( DWorksIterator )
						DWorksIterator++->Draw( *PrnDC, lSize, &lRect );

					PrnDC->SelectObject( TPen( TColor::Black, 1 ) );
					if ( CurentPageX == 0 )
						{
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( 0, iPrintPage.cy - 1 );
						}
					if ( CurentPageY == 0 )
						{
			  //		TIPoint Point( 0, + HeightFont + 5 );  //  New
			  //		PrnDC->OffsetWindowOrg( Point );       //  New
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, 0 );
						}
					if ( CurentPageX == QuantityPageX - 1 )
						{
						PrnDC->MoveTo( iPrintPage.cx - 1, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageY == QuantityPageY - 1 )
						{
						PrnDC->MoveTo( 0, iPrintPage.cy - 1 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageX == QuantityPageX - 1 && CurentPageY == QuantityPageY - 1 )
						;
					PrnDC->RestorePen();

					PrnDC->EndPage();
					}
			}
		if ( StrPrintDialog.GNTWindow )
			{
			TDPacksIterator DPacksIterator( *StrPackEdit->PackView->GetPacks() );
			while ( DPacksIterator )
				{
				TPack* pack = DPacksIterator++;
				if ( lPrintSheet.cx < pack->GetFinishTime() )
					lPrintSheet.cx = pack->GetFinishTime();
				if ( lPrintSheet.cy < pack->GetPoint().y )
					lPrintSheet.cy = pack->GetPoint().y;
				}
			TDWorksIterator DWorksIterator( *StrPackEdit->PackView->GetWorks() );
			while ( DWorksIterator )
				{
				TWork* work = DWorksIterator++;
				if ( lPrintSheet.cx < work->GetFinishTime() )
					lPrintSheet.cx = work->GetFinishTime();
				if ( lPrintSheet.cy < work->GetPoint().y )
					lPrintSheet.cy = work->GetPoint().y;
				}

			lPrintSheet.cx -= StrPackEdit->PackView->GetStartTime();
			lPrintSheet.cy += 40 + Max( TPack::StrSetEvent.VConstSizeBottomMDL, TWork::StrSetEvent.VConstSizeBottomMDL );
			lPrintSheet.cx /= TEvent::StrSetScale.HScaleGNTGST;
			lPrintSheet.cy /= TEvent::StrSetScale.VScaleGNT;

			int QuantityPageX = (int)lPrintSheet.cx / iPrintPage.cx + 1;
			int QuantityPageY = (int)lPrintSheet.cy / iPrintPage.cy + 1;

			TConnectsIterator ConnectsIterator( *StrPackEdit->PackView->GetConnects() );
			for ( int CurentPageX = 0; /*PrnDC->QueryAbort() &&*/ CurentPageX < QuantityPageX; CurentPageX++ )
				for ( int CurentPageY = 0; /*PrnDC->QueryAbort() && */CurentPageY < QuantityPageY; CurentPageY++ )
					{
					PrnDC->StartPage();

					lRect.left = (long)( CurentPageX * iPrintPage.cx * TEvent::StrSetScale.HScaleGNTGST );
					lRect.right = (long)( ( CurentPageX + 1 ) * iPrintPage.cx * TEvent::StrSetScale.HScaleGNTGST );
					TLSize lSize( (long)CurentPageX * iPrintPage.cx, 0 );
					if ( CurentPageY == 0 )
						{
						DrawCalendar( *PrnDC, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );
						lRect.top = 0;
						lRect.bottom = (long)( ( iPrintPage.cy - 40 ) * TEvent::StrSetScale.VScaleGNT );
						TIPoint Point( 0, -40 );
						PrnDC->OffsetWindowOrg( Point );
						}
					else
						{
						lSize.cy = (long)CurentPageY * iPrintPage.cy - 40;
						lRect.top = lRect.bottom;
						lRect.bottom += iPrintPage.cy * TEvent::StrSetScale.VScaleGNT;
						}

					if ( DrawNetButton )
						DrawNet( *PrnDC, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

					lRectTmp = lRect;
					lRectTmp.left -= ( lRect.left != 0L ) ? 1L : 0L;
					lRectTmp.right += 1L;
					lRectTmp.top -= Max( TPack::StrSetEvent.VConstSizeBottomGNT, TWork::StrSetEvent.VConstSizeBottomGNT ) + 1;
					lRectTmp.bottom += Max( TPack::StrSetEvent.VConstSizeTopGNT, TWork::StrSetEvent.VConstSizeTopGNT ) + 1;

					ConnectsIterator.Restart();
					while ( ConnectsIterator )
						ConnectsIterator++->Draw( *PrnDC, lSize, &lRectTmp, StrPackEdit->PackView->GetStartTime() );
					DPacksIterator.Restart();
					while ( DPacksIterator )
						DPacksIterator++->Draw( *PrnDC, lSize, &lRectTmp, StrPackEdit->PackView->GetStartTime() );
					DWorksIterator.Restart();
					while ( DWorksIterator )
						DWorksIterator++->Draw( *PrnDC, lSize, &lRectTmp, StrPackEdit->PackView->GetStartTime() );

					PrnDC->SelectObject( TPen( TColor::Black, 1 ) );
					if ( CurentPageY == 0 )
						{
						TIPoint Point( 0, +40 );
						PrnDC->OffsetWindowOrg( Point );
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, 0 );
						}
					if ( CurentPageX == 0 )
						{
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( 0, iPrintPage.cy - 1 );
						}
					if ( CurentPageY == QuantityPageY - 1 )
						{
						PrnDC->MoveTo( 0, iPrintPage.cy - 1 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageX == QuantityPageX - 1 )
						{
						PrnDC->MoveTo( iPrintPage.cx - 1, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageX == QuantityPageX - 1 && CurentPageY == QuantityPageY - 1 )
						;
					PrnDC->RestorePen();

					PrnDC->EndPage();
					}
			}
		if ( StrPrintDialog.GSTWindow )
			{
			TBitTime* BitTime;
			TSBitTimesIterator BitTimesIterator( *StrPackEdit->PackView->GetBitTimes( IndexResource ) );
			while ( BitTimesIterator )
				{
				BitTime = BitTimesIterator++;
				if ( BitTime->GetTime() != 4294967295L && lPrintSheet.cx < BitTime->GetTime() )
					lPrintSheet.cx = BitTime->GetTime();
				if ( lPrintSheet.cy < BitTime->GetResource() )
					lPrintSheet.cy = BitTime->GetResource();
				}
			lPrintSheet.cx -= StrPackEdit->PackView->GetStartTime();
			lPrintSheet.cx /= TEvent::StrSetScale.HScaleGNTGST;
			lPrintSheet.cy += 40;
			lPrintSheet.cy /= TEvent::StrSetScale.VScaleGST;

			int QuantityPageX = (int)lPrintSheet.cx / iPrintPage.cx + 1;
			int QuantityPageY = (int)lPrintSheet.cy / iPrintPage.cy + 1;
			for ( int CurentPageX = 0;/* PrnDC->QueryAbort() && */CurentPageX < QuantityPageX; CurentPageX++ )
				for ( int CurentPageY = 0; /*PrnDC->QueryAbort() && */CurentPageY < QuantityPageY; CurentPageY++ )
					{
					PrnDC->StartPage();

					lRect.left = (long)( CurentPageX * iPrintPage.cx * TEvent::StrSetScale.HScaleGNTGST );
					lRect.right = (long)( ( CurentPageX + 1 ) * iPrintPage.cx * TEvent::StrSetScale.HScaleGNTGST );
					TLSize lSize( (long)CurentPageX * iPrintPage.cx, 0 );
					if ( CurentPageY == 0 )
						{
						DrawCalendar( *PrnDC, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );
						lRect.top = 0;
						lRect.bottom = (long)( ( iPrintPage.cy - 40 ) * TEvent::StrSetScale.VScaleGNT );
						TIPoint Point( 0, -40 );
						PrnDC->OffsetWindowOrg( Point );
						}
					else
						{
						lSize = (long)CurentPageY * iPrintPage.cy - 40;
						lRect.top = lRect.bottom;
						lRect.bottom += iPrintPage.cy * TEvent::StrSetScale.VScaleGST;
						}

					if ( DrawNetButton )
						DrawNet( *PrnDC, lSize, &lRect, StrPackEdit->PackView->GetStartTime() );

					lRectTmp = lRect;
					lRect.left += StrPackEdit->PackView->GetStartTime();
					lRect.right += StrPackEdit->PackView->GetStartTime();
					lRectTmp.left -= ( lRect.left != 0L ) ? 1L : 0L;
					lRectTmp.right += 1L;

					unsigned long OldResource = 0;
					unsigned long OldTime = 0;
					unsigned long NewTime = 0;
					TIRect Rect;

					PrnDC->SelectObject( TPen( &TEvent::StrSetScale.LogPenResource ) );
					BitTimesIterator.Restart();
					while ( BitTimesIterator )
						{
						BitTime = BitTimesIterator++;
						NewTime = BitTime->GetTime();
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
							PrnDC->Rectangle( Rect );
							}
						if ( NewTime > lRect.left && NewTime > lRect.right )
							{
							Rect.Set( (int)max( (long)( ( OldTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
										 (int)( OldResource / TEvent::StrSetScale.VScaleGST - lSize.cy ),
										 (int)max( (long)( ( NewTime - StrPackEdit->PackView->GetStartTime() ) / TEvent::StrSetScale.HScaleGNTGST ) - lSize.cx, 0L ),
										 (int)( 0 - lSize.cy ) );
							PrnDC->Rectangle( Rect );
							break;
							}
						OldResource = BitTime->GetResource();
						OldTime = NewTime;
						}
					PrnDC->RestorePen();

					PrnDC->SelectObject( TPen( TColor::Black, 1 ) );
					if ( CurentPageY == 0 )
						{
						TIPoint Point( 0, +40 );
						PrnDC->OffsetWindowOrg( Point );
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, 0 );
						}
					if ( CurentPageX == 0 )
						{
						PrnDC->MoveTo( 0, 0 );
						PrnDC->LineTo( 0, iPrintPage.cy - 1 );
						}
					if ( CurentPageY == QuantityPageY - 1 )
						{
						PrnDC->MoveTo( 0, iPrintPage.cy - 1 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageX == QuantityPageX - 1 )
						{
						PrnDC->MoveTo( iPrintPage.cx - 1, 0 );
						PrnDC->LineTo( iPrintPage.cx - 1, iPrintPage.cy - 1 );
						}
					if ( CurentPageX == QuantityPageX - 1 && CurentPageY == QuantityPageY - 1 )
						;
					PrnDC->RestorePen();

					PrnDC->EndPage();
					}
			}

		if ( TPrinter::GetUserAbort() )
			{
			PrnDC->AbortDoc();
			TPrinter::SetUserAbort( FALSE );
			}
		else
			PrnDC->EndDoc();

		AbortWin->Destroy();
		delete AbortWin;
		delete PrnDC;
		}
	else
		return FALSE;
	return TRUE;
	}

