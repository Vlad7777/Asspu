//#include "pch.h"
//#pragma hdrstop
#define NO_WIN32_LEAN_AND_MEAN //????
#define _OWLVCLPCH    // ##########?????
#define STRICT
#define _USE_OWL_CM_EXIT  // ######################333 ????
#include <owl/pch.h> // ##########?????
#include <condefs.h>

USEUNIT("mainw4.cpp");
USEUNIT("work.cpp");
USEUNIT("pack.cpp");
USEUNIT("new_work.cpp");
USEUNIT("new_pack.cpp");
USEUNIT("mdl_wind.cpp");
USEUNIT("main_set.cpp");
USEUNIT("main_sav.cpp");
USEUNIT("layout.cpp");
USEUNIT("l_relay.cpp");
USEUNIT("l_print.cpp");
USEUNIT("l_math2.cpp");
USEUNIT("l_math1.cpp");
USEUNIT("l_ce.cpp");
USEUNIT("gst_wind.cpp");
USEUNIT("gnt_wind.cpp");
USEUNIT("free_fn.cpp");
USEUNIT("connect.cpp");
USEUNIT("cal_wind.cpp");
USEUNIT("bas_wind.cpp");
USEUNIT("bas_obj.cpp");
USERC("mainw4.rc");
USEDEF("asspu.def");
USEUNIT("l_dde.cpp");
USEUNIT("l_ev.cpp");
USEFORM("WorkDialogUnit.cpp", WorkDialogForm);
USEFORM("SPUDMUnit.cpp", SPUDM); /* TDataModule: DesignClass */
USEFORM("DBTESTUnit.cpp", DBTESTForm);
//---------------------------------------------------------------------------
#define WinMain
