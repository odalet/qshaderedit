/*
    QShaderEdit - Simple multiplatform shader editor
    Copyright (C) 2007 Ignacio Castaño <castano@gmail.com>
    Copyright (C) 2007 Lars Uebernickel <larsuebernickel@gmx.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "qshaderedit.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
//#include <QtWidgets/QPlastiqueStyle>

#if defined(Q_WS_X11)
#include <X11/Xlib.h>
#endif


int main(int argc, char **argv)
{
#if defined(Q_WS_X11)
    // This causes lockups in some X servers.
    //XInitThreads();
#endif

    QApplication app(argc, argv);

////#if defined(Q_WS_WIN) || defined(WIN32)
////    //app.setStyle(new QPlastiqueStyle());
////    app.setStyle("fusion");
////#endif

	////app.setStyle(QStyleFactory::create("fusion"));

	////QPalette palette;
	////palette.setColor(QPalette::Window, QColor(53, 53, 53));
	////palette.setColor(QPalette::WindowText, Qt::white);
	////palette.setColor(QPalette::Base, QColor(15, 15, 15));
	////palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	////palette.setColor(QPalette::ToolTipBase, Qt::white);
	////palette.setColor(QPalette::ToolTipText, Qt::white);
	////palette.setColor(QPalette::Text, Qt::white);
	////palette.setColor(QPalette::Button, QColor(53, 53, 53));
	////palette.setColor(QPalette::ButtonText, Qt::white);
	////palette.setColor(QPalette::BrightText, Qt::red);

	////palette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
	////palette.setColor(QPalette::HighlightedText, Qt::black);

	////palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
	////palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);

	////qApp->setPalette(palette);

	// From https://contingencycoder.wordpress.com/2013/08/15/dark-fusion-theme-for-qt-5/
	qApp->setStyle(QStyleFactory::create("Fusion"));

	//QPalette original = qApp->palette();	

	QPalette darkPalette;
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

	qApp->setPalette(darkPalette);
	qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

	//auto w = original.color(QPalette::WindowText);
	//auto w2 = darkPalette.color(QPalette::WindowText);

    QString filename;
    if (qApp->arguments().size() > 1) {
        filename = qApp->arguments().at(1);
    }

    QShaderEdit * shaderEdit = new QShaderEdit(filename);

    return app.exec();
}
