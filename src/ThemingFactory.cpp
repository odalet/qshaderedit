#include "ThemingFactory.h"

#include <QtWidgets/QApplication>

ThemingFactory::ThemingFactory() 
{
	originalPalette = qApp->palette();
	originalStyleSheet = qApp->styleSheet();

	// From https://contingencycoder.wordpress.com/2013/08/15/dark-fusion-theme-for-qt-5/
	darkPalette = QPalette(originalPalette);

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

	darkStyleSheet = "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }";
}

ThemingFactory::~ThemingFactory() { }

void ThemingFactory::initialize()
{
	qApp->setPalette(darkPalette);
	qApp->setStyleSheet(darkStyleSheet);
}

void ThemingFactory::restore()
{
	qApp->setPalette(originalPalette);
	qApp->setStyleSheet(originalStyleSheet);
}
