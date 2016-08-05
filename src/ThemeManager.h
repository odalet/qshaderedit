#pragma once

#include <QtCore/QObject>

#include "theme.h"

using namespace std;

namespace theming
{
	enum ThemeKey
	{
		Default = 0, // this is Light
		Light = 0,
		Dark = 1
	};

	class ThemeManager : public QObject
	{
		Q_OBJECT

	public:
		ThemeManager();
		~ThemeManager();

		void switchTo(ThemeKey themeKey);

		inline const map<ThemeKey, Theme&>& themes() const { return availableThemes; }
		inline const Theme& current() const { return *currentTheme; }
		inline const ThemeKey& currentKey() const { return currentThemeKey; }

	signals:
		void changed();

	private:

		map<ThemeKey, Theme&> availableThemes;

		ThemeKey currentThemeKey;
		Theme* currentTheme;
		Theme* lightTheme;
		Theme* darkTheme;

		void initializeLightTheme();
		void initializeDarkTheme();

		void setCurrentTheme(ThemeKey key, Theme* theme);
	};
}