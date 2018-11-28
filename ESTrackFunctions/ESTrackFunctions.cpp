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

void CESTrackFunctions::CleanFlightPlan()
{
	auto flightplan{ FlightPlanSelectASEL() };
	if (!flightplan.IsValid()) 
		return;
	auto route{ FlightPlanFunctions.StripRouteString(flightplan.GetFlightPlanData().GetRoute(), flightplan) };
	flightplan.GetFlightPlanData().SetRoute(route.c_str());
	flightplan.GetFlightPlanData().AmendFlightPlan();
}

void CESTrackFunctions::DisplayMessage(const std::string message, const std::string handler)
{
	DisplayUserMessage(MY_PLUGIN_NAME, handler.c_str(), message.c_str(), true, true, true, false, false);
}

void CESTrackFunctions::LoadSettings()
{
	auto ad{ GetDataFromSettings(SETTING_AUTODROP) };
	if (ad != nullptr)
		autoDrop = (strcmp(ad, "1") ? 1 : 0);
}

bool CESTrackFunctions::OnCompileCommand(const char* sCommandLine)
{
	if (!strcmp(sCommandLine, ".fpt"))
	{
		CleanFlightPlan();
		return true;
	}
	if (!strcmp(sCommandLine, ".autodrop"))
	{
		autoDrop = (autoDrop == 1 ? 0 : 1);
		SaveDataToSettings(SETTING_AUTODROP, "Drop Track for landing AC", (autoDrop == 1 ? "1" : "0"));
		if (autoDrop == 1)
			DisplayMessage("AutoDrop ON", "Info");
		else
			DisplayMessage("AutoDrop OFF", "Info");
		return true;
	}
	return false;
}

void CESTrackFunctions::OnFunctionCall(int FunctionId, const char * sItemString, POINT Pt, RECT Area)
{
	if (FunctionId == ITEM_FUNCTION_CLEAN_FP)
		CleanFlightPlan();
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