#ifndef PCH_H_
#define PCH_H_

// NO_WIN32_LEAN_AND_MEAN is needed so <vcl.h> will include
// all of <windows.h> as OWL expects.
#define NO_WIN32_LEAN_AND_MEAN

// <owl/window.rh> forces us to choose whether we want the value of
// CM_EXIT from VCL (<controls.hpp>, to be precise), or the value
// that it would normally define.  in this case we chose the VCL
// value for no discernable reason.  see <owl/window.rh> for more
// detail.
#define _USE_VCL_CM_EXIT

// STRICT needs to be defined if <windows.h> is included before OWL,
// for example, when <vcl.h> is included first.
#define STRICT

// BI_NAMESPACE needs to be defined because OWL is now in a
// namespace.  this is a bug:  BI_NAMESPACE should be defined by
// default!
#define BI_NAMESPACE

// request the normal set of owl headers to be included in the .csm
#define _OWLPCH

// include the world
#include <vcl.h>
#include <owl/pch.h>

#endif
