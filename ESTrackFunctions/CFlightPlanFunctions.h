#pragma once
#include <Windows.h>
#include <regex>
#include <string>
#include "EuroScopePlugIn.h"
class CFlightPlanFunctions
{
public:
	CFlightPlanFunctions();
	virtual ~CFlightPlanFunctions();

	std::string StripRouteString(std::string route, EuroScopePlugIn::CFlightPlan & flightplan);

private:
	void StripWhitespace(std::string & str);
	void StripInvalid(std::string & str, EuroScopePlugIn::CFlightPlan & flightplan);
};

