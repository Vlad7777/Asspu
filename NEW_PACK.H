#if !defined(__NEW_PACK_H)
#define __NEW_PACK_H

struct STR_PACK_DIALOG
	{
	char Name[ 255 ];
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
	
class TPackDialog : public TDialog 
	{
	private:
		TPack* Pack;
		STR_PACK_DIALOG StrPackDialog;
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
		TEdit* edit13;
	public:
		TPackDialog( TWindow* parent, int resId, TPack* ThePack );
		void SetupWindow();
		void CloseWindow( int ret );
	};

#endif //__NEW_PACK_H
