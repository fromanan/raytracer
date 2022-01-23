
// Raytracer.h : main header file for the Raytracer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRaytracerApp:
// See Raytracer.cpp for the implementation of this class
//

class CRaytracerApp : public CWinApp
{
public:
	CRaytracerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRaytracerApp theApp;
