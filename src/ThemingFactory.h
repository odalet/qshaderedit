#pragma once

#include <QtGui/QPalette>

class ThemingFactory
{
public:
	ThemingFactory();
	~ThemingFactory();

	void initialize();
	void restore();

private:
	QPalette originalPalette;
	QString originalStyleSheet;

	QPalette darkPalette;
	QString darkStyleSheet;
};

