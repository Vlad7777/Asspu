//----------------------------------------------------------------------------
// ObjectWindows
// Copyright (c) 1996, 1998 by Borland International, All Rights Reserved
//
// ADOPT.CPP
//
// Implements the function for adopting the VCL's hidden main window
// as a child of an OWL window.
//----------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "adopt.h"

//
// Associate VCL form with an OWL parent.
//
void AdoptVCLAppWindow(HWND newParent)
{
  SetParent(Application->Handle, newParent);
  return;
}

  
