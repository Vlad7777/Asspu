#ifndef __MAINW4_H
#define __MAINW4_H

//#include "spu_lapp.rh"     // New
#include "vp.hh"     // New

class TMyApp : public OWL::TApplication  // ###################BCB3
//class TMyApp : public TApplication
	{
	private:
	 bool            HelpState;                          // Has the help engine been used.
	 bool            ContextHelp;                        // SHIFT-F1 state (context sensitive HELP).
	 HCURSOR         HelpCursor;                         // Context sensitive help cursor.

	protected:
		char FileNameSet[ MAXPATH ];
	public:
	    static OWL::TPrinter* Printer;   // #####################BCB3
//        static TPrinter* Printer;
		static TMDIClient* Client;
	public:
	//	TMyApp() : OWL::TApplication() {} //####################
		TMyApp();
		~TMyApp();

	//{{spu_oleAppVIRTUAL_BEGIN}}
	public:
		virtual bool CanClose ();
		virtual bool ProcessAppMsg (MSG& msg);
	//{{spu_oleAppVIRTUAL_END}}


	 // Override methods of TApplication
	protected:
		void InitMainWindow();
      void InitApplication();
		void CmFileNew();
		void CmFileOpen();
		void CmPrinterSetup();
		void CmAbout();
		void CmSetWork();     // CM_SETWORK
		void CmSetPack();     // CM_SETPACK
		void CmSetConnect();  // CM_SETCONNECT
		void CmSetCalendar(); // CM_SETCALENDAR
		void CmSetBuild();    // CM_SETBUILD
		void CmSetScale();    // CM_SETSCALE
		void CmWriteSet() { WriteSet(); }  // CM_WRITESET
		bool ReadSet();
		bool WriteSet();
		void RedrawAll();

	//{{spu_oleAppRSP_TBL_BEGIN}}
	protected:
		void CmHelpAbout ();
		void CmHelpContents ();
		void CmHelpUsing ();
	//{{spu_oleAppRSP_TBL_END}}

	DECLARE_RESPONSE_TABLE( TMyApp );
	};

#endif // __MAINW4_H
