#include "services.h"
#include "../ThemeManager.h"

using namespace theming;

ThemeManager* services::themeManager;

void services::initialize(void)
{
	themeManager = new ThemeManager();
}

void services::dispose(void)
{
	delete themeManager;
}