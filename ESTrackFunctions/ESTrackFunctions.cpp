#include "ESTrackFunctions.h"

CESTrackFunctions::CESTrackFunctions()
	:CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
			  MY_PLUGIN_NAME,
			  MY_PLUGIN_VERSION,
			  MY_PLUGIN_DEVELOPER,
			  MY_PLUGIN_COPYRIGHT)
{
}


CESTrackFunctions::~CESTrackFunctions()
{
}

bool CESTrackFunctions::OnCompileCommand(const char* sCommandLine)
{
	if (!strcmp(sCommandLine, ".est aa"))
	{
		auto_accept = !auto_accept;
		std::string message = "Auto Accept ";
		if (auto_accept)
			message.append("on");
		else
			message.append("off");
		DisplayUserMessage("Message", GetPlugInName(), message.c_str(), true, false, false, false, false);
		return true;
	}
	return false;
}

void CESTrackFunctions::OnTimer(int Counter)
{
	for (auto flightplan = FlightPlanSelectFirst();
		 flightplan.IsValid();
		 flightplan = FlightPlanSelectNext(flightplan))
	{
		if (flightplan.GetSimulated())
			continue;

		if (flightplan.GetTrackingControllerIsMe())
			if (CheckDrop(flightplan))
				flightplan.EndTracking();

		if (auto_accept && flightplan.GetState() == EuroScopePlugIn::FLIGHT_PLAN_STATE_TRANSFER_TO_ME_INITIATED)
			if (CheckHandover(flightplan))
				flightplan.AcceptHandoff();
	}
}

bool CESTrackFunctions::CheckDrop(EuroScopePlugIn::CFlightPlan& flightplan)
{
	if (flightplan.GetFPTrackPosition().GetPressureAltitude() > 2500)
		return false;
	flightplan.GetFPTrackPosition().IsFPTrackPosition();
	if (flightplan.GetDistanceToDestination() < 10)
		return true;
	return false;
}

bool CESTrackFunctions::CheckHandover(EuroScopePlugIn::CFlightPlan& flightplan)
{
	auto entry_minutes = flightplan.GetSectorEntryMinutes();
	if (entry_minutes != -1 && entry_minutes <= 20)
		return true;
	return false;
}
