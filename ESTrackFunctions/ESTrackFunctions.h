#pragma once
#include <Windows.h>
#include <EuroScopePlugIn.h>
#include <string>

#define MY_PLUGIN_NAME      "ESTrackFunctions"
#define MY_PLUGIN_VERSION   "1.0"
#define MY_PLUGIN_DEVELOPER "Oliver Grützmann"
#define MY_PLUGIN_COPYRIGHT "Free to be distributed as source code"

class CESTrackFunctions :
	public EuroScopePlugIn::CPlugIn
{
public:
	bool auto_accept { true };

	CESTrackFunctions();
	virtual ~CESTrackFunctions();
	bool OnCompileCommand(const char* sCommandLine);
	void OnTimer(int Counter);

private:
	bool CheckDrop(EuroScopePlugIn::CFlightPlan& flightplan);
	bool CheckHandover(EuroScopePlugIn::CFlightPlan& flightplan);
};

