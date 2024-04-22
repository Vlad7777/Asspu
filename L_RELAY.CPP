#include <owl\owlpch.h>
#include <ddeml.h>

#include "layout.h"
#include "mainw4.rh"

void TMyLayout::ReLayout()
	{
   int i ;///////////////////////////////////////////////new!!!
	TLayoutMetrics lm;
	lm.X.Units = lm.Y.Units = lm.Width.Units = lm.Height.Units = lmPixels;
	if ( AllWindow[0].MaxScreen || AllWindow[1].MaxScreen || AllWindow[2].MaxScreen )
		{
		for ( /*int */ i = 0; i < 3; i++ )
			if ( AllWindow[i].BASScreen )
				{
				RemoveChildLayoutMetrics( *AllWindow[i].BASWindow );
				if ( AllWindow[i].MaxScreen )
					{
					if ( i == 1 || i == 2 )
						{
						RemoveChildLayoutMetrics( *Button );
						lm.X.Set( lmLeft, lmLeftOf, lmParent, lmRight, 15 );
						lm.Y.SameAs( lmParent, lmTop );
						lm.Height.Absolute( 40 );
						lm.Width.SameAs( lmParent, lmRight );
						SetChildLayoutMetrics( *Button, lm );

						RemoveChildLayoutMetrics( *AllWindow[3].BASWindow );
						lm.X.SameAs( lmParent, lmLeft );
						lm.Y.SameAs( lmParent, lmTop );
						lm.Height.Absolute( 40 );
						lm.Width.LeftOf( Button );
						SetChildLayoutMetrics( *AllWindow[3].BASWindow, lm );

						lm.Y.Below( AllWindow[3].BASWindow );
						}
					else
						{
						if ( AllWindow[3].BASScreen )
							{
							RemoveChildLayoutMetrics( *AllWindow[3].BASWindow );
							AllWindow[3].BASWindow->Show( SW_HIDE );
							RemoveChildLayoutMetrics( *Button );
							Button->Show( SW_HIDE );
							}
						lm.Y.SameAs( lmParent, lmTop );
						}
					lm.X.SameAs( lmParent, lmLeft );
					lm.Height.SameAs( lmParent, lmBottom );
					lm.Width.SameAs( lmParent, lmRight );
					SetChildLayoutMetrics( *AllWindow[i].BASWindow, lm );
					}
				else
					AllWindow[i].BASWindow->Show( SW_HIDE );
				}
		}
	else
		{
   

		TBASWindow* BASWindow = NULL;
		int QuantityWindow = 0;
		int CurrentWindow = 0;
		bool Calendar = FALSE;
		for (/* int */ i = 0; i < 4; i++ )  //
      	{
			if ( i < 3 && AllWindow[i].BASScreen )
				QuantityWindow++;
			RemoveChildLayoutMetrics( *AllWindow[i].BASWindow );
			AllWindow[i].BASWindow->Show( SW_HIDE );
			}
		RemoveChildLayoutMetrics( *Button );
		Button->Show( SW_HIDE );

		for ( i = 0; i < 3; i++ )  //  without int   my !!
			if ( AllWindow[i].BASScreen )
				{
				AllWindow[i].BASWindow->Show( SW_SHOW );
				CurrentWindow++;
				if ( !Calendar && ( i == 1 || i == 2 ) )
					{
					Calendar = TRUE;

					Button->Show( SW_SHOW );
					RemoveChildLayoutMetrics( *Button );
					lm.X.Set( lmLeft, lmLeftOf, lmParent, lmRight, 15 );
					if ( BASWindow )
						lm.Y.Below( BASWindow );
					else
						lm.Y.SameAs( lmParent, lmTop );
					lm.Height.Absolute( 40 );
					lm.Width.SameAs( lmParent, lmRight );
					SetChildLayoutMetrics( *Button, lm );

					AllWindow[3].BASWindow->Show( SW_SHOW );
					RemoveChildLayoutMetrics( *AllWindow[3].BASWindow );
					lm.X.SameAs( lmParent, lmLeft );
					if ( BASWindow )
						lm.Y.Below( BASWindow );
					else
						lm.Y.SameAs( lmParent, lmTop );
					lm.Height.Absolute( 40 );
					lm.Width.LeftOf( Button );
					SetChildLayoutMetrics( *AllWindow[3].BASWindow, lm );

					BASWindow = AllWindow[3].BASWindow;
					}
				if ( BASWindow )
					lm.Y.Below( BASWindow );
				else
					lm.Y.SameAs( lmParent, lmTop );
				lm.Width.SameAs( lmParent, lmRight );
				if ( CurrentWindow != QuantityWindow )
					lm.Height.PercentOf( lmParent, lmBottom, CurrentWindow * (int)( 100 / QuantityWindow ) );
				else
					lm.Height.PercentOf( lmParent, lmBottom, 100 );
				lm.X.SameAs( lmParent, lmLeft );
				if ( BASWindow )
					lm.Y.Below( BASWindow );
				else
					lm.Y.SameAs( lmParent, lmTop );
				SetChildLayoutMetrics( *AllWindow[i].BASWindow, lm );
				BASWindow = AllWindow[i].BASWindow;
				}
		}
	Layout();
	}

void TMyLayout::CmActivateMDL()
	{
	if ( AllWindow[0].BASScreen )
		AllWindow[0].BASScreen = FALSE;
	else
		AllWindow[0].BASScreen = TRUE;
	ReLayout();
	}

void TMyLayout::CmActivateGNT()
	{
	if ( AllWindow[1].BASScreen )
		AllWindow[1].BASScreen = FALSE;
	else
		AllWindow[1].BASScreen = TRUE;
	ReLayout();
	}

void TMyLayout::CmActivateGST()
	{
	if ( AllWindow[2].BASScreen )
		{
		AllWindow[2].BASScreen = FALSE;
		PackMain->ResetBitTimes();
		}
	else
		{
		AllWindow[2].BASScreen = TRUE;
		PackMain->BuildBitTimes();
		}
	ReLayout();
	}


