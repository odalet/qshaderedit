#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QWidget>
#include <map>
#include "ThemeManager.h"

using namespace theming;

static QString defaultStyle = "Fusion";

ThemeManager::ThemeManager()
{
	qApp->setStyle(QStyleFactory::create(defaultStyle));

	initializeLightTheme();
	initializeDarkTheme();

	availableThemes.insert(pair<ThemeKey, Theme&>(Light, *lightTheme));
	availableThemes.insert(pair<ThemeKey, Theme&>(Dark, *darkTheme));
}

ThemeManager::~ThemeManager()
{
	if (darkTheme) delete darkTheme;
	if (lightTheme) delete lightTheme;
}

void ThemeManager::initializeLightTheme()
{
	lightTheme = new Theme();
	lightTheme->name = "Light";
	lightTheme->palette = QPalette(qApp->palette());
	lightTheme->styleSheet = qApp->styleSheet();
}

void ThemeManager::initializeDarkTheme()
{
	darkTheme = new Theme();
	darkTheme->name = "Dark";

	auto& darkPalette = darkTheme->palette;

	// From https://contingencycoder.wordpress.com/2013/08/15/dark-fusion-theme-for-qt-5/
	//darkPalette = QPalette(originalPalette);

	darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::WindowText, Qt::white);
	darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
	darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
	darkPalette.setColor(QPalette::ToolTipText, Qt::white);
	darkPalette.setColor(QPalette::Text, Qt::white);
	darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
	darkPalette.setColor(QPalette::ButtonText, Qt::white);
	darkPalette.setColor(QPalette::BrightText, Qt::red);
	darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

	darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	darkPalette.setColor(QPalette::HighlightedText, Qt::black);

	darkTheme->styleSheet = ".QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }";
}

void ThemeManager::switchTo(ThemeKey themeKey)
{
	auto& found = availableThemes.at(themeKey);
	setCurrentTheme(themeKey, &found);
	qApp->setPalette(currentTheme->palette);

	////switch (themeKey)
	////{
	////case Light: setCurrentTheme(Light, lightTheme); break;
	////case Dark: setCurrentTheme(Dark, darkTheme); break;
	////}

	////// Stylesheets and palettes do not play nice together!
	////// See http://stackoverflow.com/questions/27803626/qt-stylesheet-system-is-invalidating-qpalette-how-to-make-them-work-together
	////// It seems that stylesheets are better supported than palettes. But for now, we stick to palettes...

	//////qApp->setStyleSheet(currentTheme->styleSheet);
	////qApp->setPalette(currentTheme->palette);
}

void ThemeManager::setCurrentTheme(ThemeKey key, Theme* theme)
{
	currentThemeKey = key;
	currentTheme = theme;
	emit changed();
}
