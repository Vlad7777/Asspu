#include <owl\owlpch.h>
#include <owl\printer.h>
#include <owl\choosefo.h>
#include <owl\chooseco.h>
#include <owl\control.h>
#include <owl\scrollba.h>
#include <ddeml.h>
#include <stdlib.h>
#include <dir.h>

#include "mainw4.h"
#include "mainw4.rh"
#include "pack.h"
#include "cal_wind.h"

bool TMyApp::WriteSet()
	{
	ofpstream os( FileNameSet );
	if (!os)
		{
        /////////////  BCP !!!!!!!!!!!!!!!!!!!!!!!!!!
		TResString msgTmp1( *this, IDS_UNABLEWRITE );
		TResString msgTmp2( *this, IDS_ERROR );
		char* msg = new char[ MAXPATH + msgTmp1.length() ];
		wsprintf( msg, msgTmp1.c_str(), FileNameSet );
		::MessageBox( 0, msg, msgTmp2.c_str(), MB_OK | MB_ICONQUESTION );
		delete msg;
		return FALSE;
		}

	os << TEvent::StrSetScale.ScaleYear;
	os << TEvent::StrSetScale.ScaleMonth;
	os << TEvent::StrSetScale.ScaleDay;
	os << TEvent::StrSetScale.ScaleHour;
	os << TEvent::StrSetScale.ScaleMinute;
	os << TEvent::StrSetScale.ScaleMDL;
	os << TEvent::StrSetScale.VScaleGNT;
	os << TEvent::StrSetScale.VScaleGST;
	os << TEvent::StrSetScale.HScaleGNTGST;
	os << TEvent::StrSetScale.QuantityResource;
	os << TEvent::StrSetScale.StepTime;
	os << TEvent::StrSetScale.CriticalTime;
	os << TEvent::StrSetScale.LogPenResource.lopnStyle; 
	os << TEvent::StrSetScale.LogPenResource.lopnWidth.x;
	os << TEvent::StrSetScale.LogPenResource.lopnWidth.y;
	os << TEvent::StrSetScale.LogPenResource.lopnColor;
	os << TEvent::StrSetScale.ColorRefResource;

	os << TWork::StrSetEvent.HConstSizeMDL;
	os << TWork::StrSetEvent.VConstSizeTopMDL;
	os << TWork::StrSetEvent.VConstSizeBottomMDL;
	os << TWork::StrSetEvent.VConstSizeTopGNT;
	os << TWork::StrSetEvent.VConstSizeBottomGNT;	
	os << TWork::StrSetEvent.LogPenMain.lopnStyle; 
	os << TWork::StrSetEvent.LogPenMain.lopnWidth.x;
	os << TWork::StrSetEvent.LogPenMain.lopnWidth.y;
	os << TWork::StrSetEvent.LogPenMain.lopnColor;
	os << TWork::StrSetEvent.LogPenCrit.lopnStyle; 
	os << TWork::StrSetEvent.LogPenCrit.lopnWidth.x;
	os << TWork::StrSetEvent.LogPenCrit.lopnWidth.y;
	os << TWork::StrSetEvent.LogPenCrit.lopnColor;
	os << TWork::StrSetEvent.ColorRefResource;
	os << TWork::StrSetEvent.ColorRefPercent;
	os << TWork::StrSetEvent.ColorRefFont;
	os << TWork::StrSetEvent.LogFont.lfHeight;
	os << TWork::StrSetEvent.LogFont.lfWidth;
	os << TWork::StrSetEvent.LogFont.lfEscapement;
	os << TWork::StrSetEvent.LogFont.lfOrientation;
	os << TWork::StrSetEvent.LogFont.lfWeight;
	os << TWork::StrSetEvent.LogFont.lfItalic;
	os << TWork::StrSetEvent.LogFont.lfUnderline;
	os << TWork::StrSetEvent.LogFont.lfStrikeOut;
	os << TWork::StrSetEvent.LogFont.lfCharSet;
	os << TWork::StrSetEvent.LogFont.lfOutPrecision;
	os << TWork::StrSetEvent.LogFont.lfClipPrecision;

	os << TWork::StrSetEvent.LogFont.lfQuality;
	os << TWork::StrSetEvent.LogFont.lfPitchAndFamily;
	os.fwriteString( TWork::StrSetEvent.LogFont.lfFaceName );
  
	os.fwriteString( TWork::StrSetField.FieldName );
	os.fwriteString( TWork::StrSetField.FieldNumber );
	os.fwriteString( TWork::StrSetField.FieldStartTime );
	os.fwriteString( TWork::StrSetField.FieldFinishTime );
	os.fwriteString( TWork::StrSetField.FieldDuration );
	os.fwriteString( TWork::StrSetField.FieldPercent );
	os.fwriteString( TWork::StrSetField.FieldShiftLimStart );
	os.fwriteString( TWork::StrSetField.FieldShiftLimFinish );		
	os.fwriteString( TWork::StrSetField.FieldShiftOptStart );
	os.fwriteString( TWork::StrSetField.FieldShiftOptFinish );		
	os.fwriteString( TWork::StrSetField.FieldShiftStart );
	os.fwriteString( TWork::StrSetField.FieldShiftFinish );		
	os.fwriteString( TWork::StrSetField.FieldCritical );		
	os.fwriteString( TWork::StrSetField.FieldStartKnot );
	os.fwriteString( TWork::StrSetField.FieldFinishKnot );
	os.fwriteString( TWork::StrSetField.FieldNamePack );
	os.fwriteString( TWork::StrSetField.FieldNumberPack );		
	os.fwriteString( TWork::StrSetField.FieldNumberParentPack );
	os.fwriteString( TWork::StrSetField.FieldIndexResource );
	os.fwriteString( TWork::StrSetField.FieldResource );	
	os.fwriteString( TWork::StrSetField.FieldResourceMin );	
	os.fwriteString( TWork::StrSetField.FieldResourceOpt );	
	os.fwriteString( TWork::StrSetField.FieldResourceMax );	
	os.fwriteString( TWork::StrSetField.FieldVolume );			

	os << TPack::StrSetEvent.HConstSizeMDL;
	os << TPack::StrSetEvent.VConstSizeTopMDL;
	os << TPack::StrSetEvent.VConstSizeBottomMDL;
	os << TPack::StrSetEvent.VConstSizeTopGNT;
	os << TPack::StrSetEvent.VConstSizeBottomGNT;
	os << TPack::StrSetEvent.LogPenMain.lopnStyle;
	os << TPack::StrSetEvent.LogPenMain.lopnWidth.x;
	os << TPack::StrSetEvent.LogPenMain.lopnWidth.y;
	os << TPack::StrSetEvent.LogPenMain.lopnColor;
	os << TPack::StrSetEvent.LogPenCrit.lopnStyle; 
	os << TPack::StrSetEvent.LogPenCrit.lopnWidth.x;
	os << TPack::StrSetEvent.LogPenCrit.lopnWidth.y;
	os << TPack::StrSetEvent.LogPenCrit.lopnColor;
	os << TPack::StrSetEvent.ColorRefResource;
	os << TPack::StrSetEvent.ColorRefPercent;
	os << TPack::StrSetEvent.ColorRefFont;
	os << TPack::StrSetEvent.LogFont.lfHeight;
	os << TPack::StrSetEvent.LogFont.lfWidth;
	os << TPack::StrSetEvent.LogFont.lfEscapement;
	os << TPack::StrSetEvent.LogFont.lfOrientation;
	os << TPack::StrSetEvent.LogFont.lfWeight;
	os << TPack::StrSetEvent.LogFont.lfItalic;
	os << TPack::StrSetEvent.LogFont.lfUnderline;
	os << TPack::StrSetEvent.LogFont.lfStrikeOut;
	os << TPack::StrSetEvent.LogFont.lfCharSet;
	os << TPack::StrSetEvent.LogFont.lfOutPrecision;
	os << TPack::StrSetEvent.LogFont.lfClipPrecision;
	os << TPack::StrSetEvent.LogFont.lfQuality;
	os << TPack::StrSetEvent.LogFont.lfPitchAndFamily;
	os.fwriteString( TPack::StrSetEvent.LogFont.lfFaceName );

	os << TConnect::StrSetConnect.ConstSizeArrow;
	os << TConnect::StrSetConnect.ConstAngjeArrow;
	os << TConnect::StrSetConnect.LogPenMain.lopnStyle;
	os << TConnect::StrSetConnect.LogPenMain.lopnWidth.x;
	os << TConnect::StrSetConnect.LogPenMain.lopnWidth.y;
	os << TConnect::StrSetConnect.LogPenMain.lopnColor;
	os << TConnect::StrSetConnect.LogPenCrit.lopnStyle; 
	os << TConnect::StrSetConnect.LogPenCrit.lopnWidth.x;
	os << TConnect::StrSetConnect.LogPenCrit.lopnWidth.y;
	os << TConnect::StrSetConnect.LogPenCrit.lopnColor;

	os << TCALWindow::StrSetCalendar.LogPenMain.lopnStyle; 
	os << TCALWindow::StrSetCalendar.LogPenMain.lopnWidth.x;
	os << TCALWindow::StrSetCalendar.LogPenMain.lopnWidth.y;
	os << TCALWindow::StrSetCalendar.LogPenMain.lopnColor;
	os << TCALWindow::StrSetCalendar.LogPenCrit.lopnStyle; 
	os << TCALWindow::StrSetCalendar.LogPenCrit.lopnWidth.x;
	os << TCALWindow::StrSetCalendar.LogPenCrit.lopnWidth.y;
	os << TCALWindow::StrSetCalendar.LogPenCrit.lopnColor;
	os << TCALWindow::StrSetCalendar.ColorRefFont;
	os << TCALWindow::StrSetCalendar.LogFont.lfHeight;
	os << TCALWindow::StrSetCalendar.LogFont.lfWidth;
	os << TCALWindow::StrSetCalendar.LogFont.lfEscapement;
	os << TCALWindow::StrSetCalendar.LogFont.lfOrientation;
	os << TCALWindow::StrSetCalendar.LogFont.lfWeight;
	os << TCALWindow::StrSetCalendar.LogFont.lfItalic;
	os << TCALWindow::StrSetCalendar.LogFont.lfUnderline;
	os << TCALWindow::StrSetCalendar.LogFont.lfStrikeOut;
	os << TCALWindow::StrSetCalendar.LogFont.lfCharSet;
	os << TCALWindow::StrSetCalendar.LogFont.lfOutPrecision;
	os << TCALWindow::StrSetCalendar.LogFont.lfClipPrecision;
	os << TCALWindow::StrSetCalendar.LogFont.lfQuality;
	os << TCALWindow::StrSetCalendar.LogFont.lfPitchAndFamily;
	os.fwriteString( TCALWindow::StrSetCalendar.LogFont.lfFaceName );

	return TRUE;
	}

bool TMyApp::ReadSet()
	{
	getcwd( FileNameSet, MAXPATH );
	strcat( FileNameSet, "\\config.cfg" );
	ifpstream is( FileNameSet );
	if ( !is )
		{
        //////////////////// BCP !!!!!!!!!!!!!!!!!!!!!!!!!
		TResString msgTmp1( *this, IDS_UNABLEREAD );
		TResString msgTmp2( *this, IDS_ERROR );
		char* msg = new char[ MAXPATH + msgTmp1.length() ];
		wsprintf( msg, msgTmp1.c_str(), FileNameSet );
		::MessageBox( 0, msg, msgTmp2.c_str(), MB_OK | MB_ICONQUESTION );
		delete msg;

		LOGFONT LogFont;
		HGDIOBJ Font = GetStockObject( SYSTEM_FIXED_FONT );
		GetObject( Font, sizeof(LOGFONT), &LogFont );

		TWork::StrSetEvent.LogFont = LogFont;
		TPack::StrSetEvent.LogFont = LogFont;
		TCALWindow::StrSetCalendar.LogFont = LogFont;

		return FALSE;
		}
	char* String;
	
	is >> TEvent::StrSetScale.ScaleYear;
	is >> TEvent::StrSetScale.ScaleMonth;
	is >> TEvent::StrSetScale.ScaleDay;
	is >> TEvent::StrSetScale.ScaleHour;
	is >> TEvent::StrSetScale.ScaleMinute;
	is >> TEvent::StrSetScale.ScaleMDL;
	is >> TEvent::StrSetScale.VScaleGNT;
	is >> TEvent::StrSetScale.VScaleGST;
	is >> TEvent::StrSetScale.HScaleGNTGST;
	is >> TEvent::StrSetScale.QuantityResource;
	is >> TEvent::StrSetScale.StepTime;
	is >> TEvent::StrSetScale.CriticalTime;
	is >> TEvent::StrSetScale.LogPenResource.lopnStyle; 
	is >> TEvent::StrSetScale.LogPenResource.lopnWidth.x;
	is >> TEvent::StrSetScale.LogPenResource.lopnWidth.y;
	is >> TEvent::StrSetScale.LogPenResource.lopnColor;
	is >> TEvent::StrSetScale.ColorRefResource;

	is >> TWork::StrSetEvent.HConstSizeMDL;
	is >> TWork::StrSetEvent.VConstSizeTopMDL;
	is >> TWork::StrSetEvent.VConstSizeBottomMDL;
	is >> TWork::StrSetEvent.VConstSizeTopGNT;
	is >> TWork::StrSetEvent.VConstSizeBottomGNT;
	is >> TWork::StrSetEvent.LogPenMain.lopnStyle; 
	is >> TWork::StrSetEvent.LogPenMain.lopnWidth.x;
	is >> TWork::StrSetEvent.LogPenMain.lopnWidth.y;
	is >> TWork::StrSetEvent.LogPenMain.lopnColor;
	is >> TWork::StrSetEvent.LogPenCrit.lopnStyle; 
	is >> TWork::StrSetEvent.LogPenCrit.lopnWidth.x;
	is >> TWork::StrSetEvent.LogPenCrit.lopnWidth.y;
	is >> TWork::StrSetEvent.LogPenCrit.lopnColor;
	is >> TWork::StrSetEvent.ColorRefResource;
	is >> TWork::StrSetEvent.ColorRefPercent;
	is >> TWork::StrSetEvent.ColorRefFont;
	is >> TWork::StrSetEvent.LogFont.lfHeight;
	is >> TWork::StrSetEvent.LogFont.lfWidth;
	is >> TWork::StrSetEvent.LogFont.lfEscapement;
	is >> TWork::StrSetEvent.LogFont.lfOrientation;
	is >> TWork::StrSetEvent.LogFont.lfWeight;
	is >> TWork::StrSetEvent.LogFont.lfItalic;
	is >> TWork::StrSetEvent.LogFont.lfUnderline;
	is >> TWork::StrSetEvent.LogFont.lfStrikeOut;
	is >> TWork::StrSetEvent.LogFont.lfCharSet;
	is >> TWork::StrSetEvent.LogFont.lfOutPrecision;
	is >> TWork::StrSetEvent.LogFont.lfClipPrecision;
	is >> TWork::StrSetEvent.LogFont.lfQuality;
	is >> TWork::StrSetEvent.LogFont.lfPitchAndFamily;
	String = is.freadString();
	strcpy( TWork::StrSetEvent.LogFont.lfFaceName, String );
 //	free( String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldName, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldNumber, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldStartTime, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldFinishTime, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldDuration, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldPercent, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftLimStart, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftLimFinish, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftOptStart, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftOptFinish, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftStart, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldShiftFinish, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldCritical, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldStartKnot, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldFinishKnot, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldNamePack, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldNumberPack, String );
   delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldNumberParentPack, String );
	delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldIndexResource, String );
	 delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldResourceMin, String );
	 delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldResourceOpt, String );
	 delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldResourceMax, String );
	 delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldResource, String );
	 delete[] String;
	String = is.freadString();
	strcpy( TWork::StrSetField.FieldVolume, String );
	 delete[] String;

	is >> TPack::StrSetEvent.HConstSizeMDL;
	is >> TPack::StrSetEvent.VConstSizeTopMDL;
	is >> TPack::StrSetEvent.VConstSizeBottomMDL;
	is >> TPack::StrSetEvent.VConstSizeTopGNT;
	is >> TPack::StrSetEvent.VConstSizeBottomGNT;

	is >> TPack::StrSetEvent.LogPenMain.lopnStyle;
	is >> TPack::StrSetEvent.LogPenMain.lopnWidth.x;
	is >> TPack::StrSetEvent.LogPenMain.lopnWidth.y;
	is >> TPack::StrSetEvent.LogPenMain.lopnColor;
	is >> TPack::StrSetEvent.LogPenCrit.lopnStyle;
	is >> TPack::StrSetEvent.LogPenCrit.lopnWidth.x;
	is >> TPack::StrSetEvent.LogPenCrit.lopnWidth.y;
	is >> TPack::StrSetEvent.LogPenCrit.lopnColor;
	is >> TPack::StrSetEvent.ColorRefResource;
	is >> TPack::StrSetEvent.ColorRefPercent;
	is >> TPack::StrSetEvent.ColorRefFont;
	is >> TPack::StrSetEvent.LogFont.lfHeight;
	is >> TPack::StrSetEvent.LogFont.lfWidth;
	is >> TPack::StrSetEvent.LogFont.lfEscapement;
	is >> TPack::StrSetEvent.LogFont.lfOrientation;
	is >> TPack::StrSetEvent.LogFont.lfWeight;
	is >> TPack::StrSetEvent.LogFont.lfItalic;
	is >> TPack::StrSetEvent.LogFont.lfUnderline;
	is >> TPack::StrSetEvent.LogFont.lfStrikeOut;
	is >> TPack::StrSetEvent.LogFont.lfCharSet;
	is >> TPack::StrSetEvent.LogFont.lfOutPrecision;
	is >> TPack::StrSetEvent.LogFont.lfClipPrecision;
	is >> TPack::StrSetEvent.LogFont.lfQuality;
	is >> TPack::StrSetEvent.LogFont.lfPitchAndFamily;
	String = is.freadString();
	strcpy( TPack::StrSetEvent.LogFont.lfFaceName, String );
	 delete[] String;


	is >> TConnect::StrSetConnect.ConstSizeArrow;
	is >> TConnect::StrSetConnect.ConstAngjeArrow;
	is >> TConnect::StrSetConnect.LogPenMain.lopnStyle;
	is >> TConnect::StrSetConnect.LogPenMain.lopnWidth.x;
	is >> TConnect::StrSetConnect.LogPenMain.lopnWidth.y;
	is >> TConnect::StrSetConnect.LogPenMain.lopnColor;
	is >> TConnect::StrSetConnect.LogPenCrit.lopnStyle;
	is >> TConnect::StrSetConnect.LogPenCrit.lopnWidth.x;
	is >> TConnect::StrSetConnect.LogPenCrit.lopnWidth.y;
	is >> TConnect::StrSetConnect.LogPenCrit.lopnColor;


	is >> TCALWindow::StrSetCalendar.LogPenMain.lopnStyle;
	is >> TCALWindow::StrSetCalendar.LogPenMain.lopnWidth.x;
	is >> TCALWindow::StrSetCalendar.LogPenMain.lopnWidth.y;
	is >> TCALWindow::StrSetCalendar.LogPenMain.lopnColor;
	is >> TCALWindow::StrSetCalendar.LogPenCrit.lopnStyle;
	is >> TCALWindow::StrSetCalendar.LogPenCrit.lopnWidth.x;
	is >> TCALWindow::StrSetCalendar.LogPenCrit.lopnWidth.y;
	is >> TCALWindow::StrSetCalendar.LogPenCrit.lopnColor;
	is >> TCALWindow::StrSetCalendar.ColorRefFont;
	is >> TCALWindow::StrSetCalendar.LogFont.lfHeight;
	is >> TCALWindow::StrSetCalendar.LogFont.lfWidth;
	is >> TCALWindow::StrSetCalendar.LogFont.lfEscapement;
	is >> TCALWindow::StrSetCalendar.LogFont.lfOrientation;
	is >> TCALWindow::StrSetCalendar.LogFont.lfWeight;
	is >> TCALWindow::StrSetCalendar.LogFont.lfItalic;
	is >> TCALWindow::StrSetCalendar.LogFont.lfUnderline;
	is >> TCALWindow::StrSetCalendar.LogFont.lfStrikeOut;
	is >> TCALWindow::StrSetCalendar.LogFont.lfCharSet;
	is >> TCALWindow::StrSetCalendar.LogFont.lfOutPrecision;
	is >> TCALWindow::StrSetCalendar.LogFont.lfClipPrecision;
	is >> TCALWindow::StrSetCalendar.LogFont.lfQuality;
	is >> TCALWindow::StrSetCalendar.LogFont.lfPitchAndFamily;
	String = is.freadString();
	strcpy( TCALWindow::StrSetCalendar.LogFont.lfFaceName, String );
	 delete[] String;

	return TRUE;
	}
