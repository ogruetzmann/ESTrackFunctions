#include "CFlightPlanFunctions.h"



CFlightPlanFunctions::CFlightPlanFunctions()
{
}


CFlightPlanFunctions::~CFlightPlanFunctions()
{
}

std::string CFlightPlanFunctions::StripRouteString(std::string route, EuroScopePlugIn::CFlightPlan & flightplan)
{
	StripInvalid(route, flightplan);
	StripWhitespace(route);
	return route;
}

void CFlightPlanFunctions::StripWhitespace(std::string & str)
{
	str = std::regex_replace(str, std::regex{ "^\\s+|(\\s)\\s+" }, "$1");
}

void CFlightPlanFunctions::StripInvalid(std::string & str, EuroScopePlugIn::CFlightPlan & flightplan)
{
	std::string departure{ flightplan.GetFlightPlanData().GetOrigin() };
	std::string destination{ flightplan.GetFlightPlanData().GetDestination() };
	std::regex rDeparture{ "(?:^|\\s)" + departure + "\\s"};
	std::regex rDestination{ "\\s" + destination + "(?:\\s|$)" };
	std::regex rSpeedLevel{ "(?:^|\\s)[NKM]\\d{3,4}[FAM]\\d{3,4}" };
	std::regex rSidStar{ "(?:^|\\s)SID | STAR(?:$|\\s)" };
	std::regex rSid{ "([A-Z]{4})(\\d[A-Z])\\s+(\\1[A-Z])" };
	std::regex rStar{ "(([A-Z]{4})[A-Z])\\s+\\2(\\d[A-Z])" };
	str = std::regex_replace(str, rSidStar, " ");
	str = std::regex_replace(str, rDeparture, " ");
	str = std::regex_replace(str, rDestination, " ");
	str = std::regex_replace(str, rSidStar, " ");
	str = std::regex_replace(str, rSpeedLevel, " ");
	str = std::regex_replace(str, rSid, "$3$2 $3");
	str = std::regex_replace(str, rStar, "$1 $1$3");
}
