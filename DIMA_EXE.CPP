				 /*  Win_Exe FoxPro App  */


#include "sp_lMDIc.h"

void spu_oleMDIClient::CmDima()

 {

WORD wReturn;
char szMsg[80];

wReturn = WinExec("mainw4", SW_SHOW);

if (wReturn < 32) {
//	 sprintf(szMsg, "WinExec failed; error code = %d", wReturn);
//	 MessageBox(HWindow, szMsg, "Error", MB_ICONSTOP);
}
else {
//sprintf(szMsg, "WinExec returned %d", wReturn);
//	 MessageBox(HWindow, szMsg, "", MB_OK);
}
}






