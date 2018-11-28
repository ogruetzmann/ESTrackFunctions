#include "ESTrackFunctions.h"

CESTrackFunctions * pMyPlugIn{ nullptr };

void __declspec (dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn ** pPlugInInstance)
{
	*pPlugInInstance = pMyPlugIn = new CESTrackFunctions;
}

void __declspec (dllexport) EuroScopePlugInExit(void)
{
	delete pMyPlugIn;
}
