#pragma once
#include <Windows.h>
#include <EuroScopePlugIn.h>
#include <string>
#include <regex>
#include "CFlightPlanFunctions.h"

constexpr auto MY_PLUGIN_VERSION{ "1.1" };
constexpr auto MY_PLUGIN_NAME{ "ESTrackFunctions" };
constexpr auto MY_PLUGIN_DEVELOPER{ "Oliver Grützmann" };
constexpr auto MY_PLUGIN_COPYRIGHT{ "Free to be distributed as source code" };

constexpr auto DROP_DISTANCE{ 10 };
constexpr auto DROP_LEVEL{ 2500 };

constexpr auto ITEM_FUNCTION_CLEAN_FP{ 1 };
constexpr auto SETTING_AUTODROP{ "AutoDrop" };

class CESTrackFunctions :
	public EuroScopePlugIn::CPlugIn
{
public:
	CESTrackFunctions();
	virtual ~CESTrackFunctions();

private:
	int dropLevel{ DROP_LEVEL };
	int dropDistance{ DROP_DISTANCE };
	int autoDrop{ 1 };
	CFlightPlanFunctions FlightPlanFunctions;

	bool CheckDrop(const EuroScopePlugIn::CFlightPlan& flightplan) const;
	void CleanFlightPlan();
	void DisplayMessage(const std::string message, const std::string handler = "Debug");
	void LoadSettings();
	bool OnCompileCommand(const char* sCommandLine);
	void OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area);
	void OnTimer(int Counter);
};

