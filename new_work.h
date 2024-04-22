#if !defined(__NEW_WORK_H)
#define __NEW_WORK_H

struct STR_WORK_DIALOG
	{
	char Name[ 255 ];
	char IndexBrigade[ 11 ];
	char IndexPack[ 11 ];
	char IndexShift[ 11 ];
	char Percent[ 11 ];
	char Duration_hh[ 11 ];
	char Duration_mm[ 11 ];
	char Duration_ss[ 11 ];
	char Volume[ 11 ];
	char ResourceMin[ 11 ];
	char ResourceOpt[ 11 ];
	char ResourceMax[ 11 ];
	char Resource[ 11 ];
	char ShiftStart_hh[ 11 ];
	char ShiftStart_mm[ 11 ];
	char ShiftStart_ss[ 11 ];
	char ShiftFinish_hh[ 11 ];
	char ShiftFinish_mm[ 11 ];
	char ShiftFinish_ss[ 11 ];
	char Start_DD[ 11 ];
	char Start_MM[ 11 ];
	char Start_YY[ 11 ];
	char Start_hh[ 11 ];
	char Start_mm[ 11 ];
	char Start_ss[ 11 ];
	char Finish_DD[ 11 ];
	char Finish_MM[ 11 ];
	char Finish_YY[ 11 ];
	char Finish_hh[ 11 ];
	char Finish_mm[ 11 ];
	char Finish_ss[ 11 ];
	};

class TWorkDialog : public TDialog
	{
	private:
		TWork* Work;
		TMyResource* Resource;
		TSResources* Resources;
		STR_WORK_DIALOG StrWorkDialog;
		TEdit* edit1;
		TEdit* edit2;
		TEdit* edit3;
		TEdit* edit4;
		TEdit* edit5;
		TEdit* edit6;
		TEdit* edit7;
		TEdit* edit8;
		TEdit* edit9;
		TEdit* edit10;
		TEdit* edit11;
		TEdit* edit12;

		TStatic* static1;
		TStatic* static2;
		TStatic* static3;
		TStatic* static4;
		TStatic* static5;
		TStatic* static6;
		TStatic* static7;
		TStatic* static8;
		TStatic* static9;
		TStatic* static10;
		TStatic* static11;
		TStatic* static12;
		TStatic* static13;
		TStatic* static14;
		TStatic* static15;
		TStatic* static16;
		TStatic* static17;
		TStatic* static18;
		TStatic* static19;
		TListBox* ListBoxPack;
		TListBox* ListBoxBrigade;
		TListBox* ListBoxResourceAll;
		TListBox* ListBoxResourceWork;
		TCheckBox* CheckBox;
	public:
		TWorkDialog( TWindow* parent, int resId, TWork* TheWork );
		void SetupWindow();
		void IdAddResource();
		void IdRemoveResource();
		void ListBoxResourceWorkChange();
		void CloseWindow( int ret );

	DECLARE_RESPONSE_TABLE( TWorkDialog );
	};

#endif //__NEW_WORK_H

