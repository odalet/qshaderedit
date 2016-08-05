#pragma once

#include "../ThemeManager.h"

using namespace theming;

namespace services
{
	extern ThemeManager* themeManager;

	void initialize(void);
	void dispose(void);
}