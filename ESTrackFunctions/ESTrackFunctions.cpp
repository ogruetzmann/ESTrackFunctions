#include "ESTrackFunctions.h"

CESTrackFunctions::CESTrackFunctions()
	:CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
			  MY_PLUGIN_NAME,
			  MY_PLUGIN_VERSION,
			  MY_PLUGIN_DEVELOPER,
			  MY_PLUGIN_COPYRIGHT)
{
	RegisterTagItemFunction("Clean FP", ITEM_FUNCTION_CLEAN_FP);
}


CESTrackFunctions::~CESTrackFunctions()
{
}

bool CESTrackFunctions::OnCompileCommand(const char* sCommandLine)
{
	if (!strcmp(sCommandLine, ".fpt"))
	{
		CleanFlightPlan(FlightPlanSelectASEL());
		return true;
	}
	if (!strcmp(sCommandLine, ".autodrop"))
	{
		autoDrop = (autoDrop == 1 ? 0 : 1);
		SaveDataToSettings("AutoDrop", "Drop Track for landing AC", (autoDrop == 1 ? "1" : "0"));
		return true;
	}
	return false;
}

void CESTrackFunctions::OnTimer(int Counter)
{
	if (!ControllerMyself().IsValid() || !ControllerMyself().IsController())
		return;
	if (GetConnectionType() != EuroScopePlugIn::CONNECTION_TYPE_DIRECT)
		return;
	if (autoDrop)
	{
		for (auto flightplan = FlightPlanSelectFirst();
			flightplan.IsValid();
			flightplan = FlightPlanSelectNext(flightplan))
		{
			if (flightplan.GetSimulated()) continue;
			if (CheckDrop(flightplan))
				flightplan.EndTracking();
		}
	}
}

void CESTrackFunctions::PostDebugMessage(std::string message)
{
	DisplayUserMessage("Debug", "Debug", message.c_str(), true, true, true, false, false);
}

bool CESTrackFunctions::CheckDrop(const EuroScopePlugIn::CFlightPlan & flightplan) const
{
	if (!flightplan.GetTrackingControllerIsMe())
		return false;
	auto radarTarget = flightplan.GetCorrelatedRadarTarget();
	if (!radarTarget.IsValid())
		return false;
	auto position = radarTarget.GetPosition();
	if (!position.IsValid())
		return false;
	if (flightplan.GetClearedAltitude() != 1 && flightplan.GetClearedAltitude() != 2)
		return false;
	if (position.GetPressureAltitude() < dropLevel && flightplan.GetDistanceToDestination() < dropDistance)
		return true;
	return false;
}

void CESTrackFunctions::CleanFlightPlan(EuroScopePlugIn::CFlightPlan & flightplan)
{
	if (!flightplan.IsValid()) return;
	std::string route{ FlightPlanFunctions.StripRouteString(flightplan.GetFlightPlanData().GetRoute(), flightplan) };
	flightplan.GetFlightPlanData().SetRoute(route.c_str());
	flightplan.GetFlightPlanData().AmendFlightPlan();
}

void CESTrackFunctions::LoadSettings()
{
	auto ad = GetDataFromSettings("AutoDrop");
	if (ad != NULL)
		autoDrop = (strcmp(ad, "1") ? 1 : 0);
}

void CESTrackFunctions::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	PostDebugMessage(sItemString);
	if (FunctionId == ITEM_FUNCTION_CLEAN_FP)
		CleanFlightPlan(FlightPlanSelectASEL());
}

