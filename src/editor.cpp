/*
	QShaderEdit - Simple multiplatform shader editor
	Copyright (C) 2007 Ignacio Casta�o <castano@gmail.com>
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

#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "highlighter.h"
#include "effect.h"

#include <QtCore/QDebug>
#include <QtWidgets/QTabBar>
#include <QtWidgets/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtGui/QTextLayout>
#include <QtGui/QTextBlock>
//#include <QtGui/QTextDocument>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>

#define DARK_THEME

SourceEdit::SourceEdit(QWidget * parent) : QTextEdit(parent), m_line(0), m_lineRect(lineRect())
{
	connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(cursorChanged()));
}

void SourceEdit::keyPressEvent(QKeyEvent * event)
{
	QTextEdit::keyPressEvent(event);
	if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
	{
		QTextCursor cursor = this->textCursor();
		QTextBlock prevBlock = cursor.block().previous();
		if (prevBlock.isValid()) {
			QString text = prevBlock.text();
			QString indentStr("");
			for (int n = 0; text[n].isSpace(); n++)
				indentStr += text[n];

			// increase indent
			text = text.trimmed();
			QRegExp rx("^(if|else|while|for).*");
			if (text.endsWith('{') || rx.exactMatch(text))
				indentStr += '\t';
			cursor.insertText(indentStr);
		}
	}
	else if (event->key() == Qt::Key_BraceRight)
	{
		// decrease indent
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::Left);
		if (cursor.block().text().endsWith("\t}"))
			cursor.deletePreviousChar();
	}
	else if (event->key() == Qt::Key_BraceLeft)
	{
		// decrease indent
		QTextCursor cursor = this->textCursor();
		cursor.movePosition(QTextCursor::Left);
		if (cursor.block().text().endsWith("\t{"))
			cursor.deletePreviousChar();
	}
}

void SourceEdit::paintEvent(QPaintEvent * event)
{
	QPainter p(viewport());
	QRect rect = lineRect().intersected(event->region().boundingRect());
#ifdef DARK_THEME
	p.fillRect(rect, QBrush(QColor(7, 7, 7)));
#else
	p.fillRect(rect, QBrush(QColor(248, 248, 248)));
#endif
	p.end();

	QTextEdit::paintEvent(event);
}

QRect SourceEdit::lineRect()
{
	QRect rect = cursorRect();
	rect.setLeft(0);
	rect.setWidth(viewport()->width());
	return rect;
}

void SourceEdit::cursorChanged()
{
	if (m_line != textCursor().blockNumber())
	{
		viewport()->update();

		// @@ Optimize updated.
		//viewport()->update(m_lineRect);

		m_lineRect = lineRect();
		m_line = textCursor().blockNumber();

		//viewport()->update(m_lineRect);
	}
}


Editor::Editor(QWidget * parent) : QTabWidget(parent)
{
#ifdef Q_OS_MAC
	tabBar()->setDocumentMode(true);
#endif
	tabBar()->setShape(QTabBar::RoundedNorth);
	tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
	tabBar()->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred, QSizePolicy::TabWidget));

	//tabBar()->setDocumentMode(true);
	//tabBar()->setShape(QTabBar::RoundedNorth);
	//tabBar()->setMovable(true);
	tabBar()->hide();

#if 0
	/*this->setStyleSheet(
		  "QTabBar::tab { background: gray; color: white; padding: 10px; } "
		  "QTabBar::tab:selected { background: lightgray; } "
		  "QTabWidget::pane { border: 0; } ");
		  //"QWidget { background: lightgray; } ");
	*/
	this->setStyleSheet(
		"QTabWidget::pane { /* The tab widget frame */"
		"border-top: 2px solid #C2C7CB;"
		"}"
		"QTabWidget::tab-bar {"
		"left: 5px; /* move to the right by 5px */"
		"}"
		"/* Style the tab using the tab sub-control. Note that it reads QTabBar _not_ QTabWidget */"
		"QTabBar::tab {"
		"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 0.4 #DDDDDD, stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);"
		"border: 2px solid #C4C4C3;"
		"border-bottom-color: #C2C7CB; /* same as the pane color */"
		"border-top-left-radius: 4px;"
		"border-top-right-radius: 4px;"
		"min-width: 8ex;"
		"padding: 2px;"
		"}"
		"QTabBar::tab:selected, QTabBar::tab:hover {"
		"background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);"
		"}"
		"QTabBar::tab:selected {"
		"border-color: #9B9B9B;"
		"border-bottom-color: #C2C7CB; /* same as pane color */"
		"}"
		"QTabBar::tab:!selected {"
		"margin-top: 2px; /* make non-selected tabs look smaller */"
		"}"
	);
#endif

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));

#if defined(Q_OS_DARWIN)
	m_font.setFamily("Monaco");
	m_font.setPointSize(11);
#elif defined(Q_OS_WIN32)
	m_font.setFamily("Courier");
	m_font.setPointSize(8);
#else
	m_font.setFamily("Monospace");
	m_font.setPointSize(8);
#endif
	m_font.setFixedPitch(true);
	//  m_font.setStyleHint(QFont::Courier, QFont::PreferQuality);
}

QTextEdit * Editor::currentTextEdit() const
{
	return static_cast<QTextEdit *>(currentWidget());
}

int Editor::line() const
{
	return currentTextEdit()->textCursor().blockNumber() + 1;
}

int Editor::column() const
{
	return currentTextEdit()->textCursor().columnNumber() + 1;
}

bool Editor::isModified() const
{
	bool modified = false;

	const int num = this->count();
	for (int i = 0; i < num; i++)
	{
		QTextEdit * editor = qobject_cast<QTextEdit *>(this->widget(i));

		if (editor != NULL && editor->document()->isModified()) {
			modified = true;
		}
	}

	return modified;
}

void Editor::setModified(bool b)
{
	const int num = this->count();
	for (int i = 0; i < num; i++)
	{
		QTextEdit * editor = qobject_cast<QTextEdit *>(this->widget(i));

		if (editor != NULL) {
			editor->document()->setModified(b);
		}
	}
}

// slots.
void Editor::undo()
{
	currentTextEdit()->document()->undo();
}
void Editor::redo()
{
	currentTextEdit()->document()->redo();
}
void Editor::cut()
{
	currentTextEdit()->cut();
}
void Editor::copy()
{
	currentTextEdit()->copy();
}
void Editor::paste()
{
	currentTextEdit()->paste();
}

void Editor::gotoLine(int tab, int line, int column)
{
	setCurrentIndex(tab);
	QTextCursor cursor(currentTextEdit()->textCursor());
	cursor.movePosition(QTextCursor::Start);
	cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);
	if (column >= 1)
		cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, column - 1);
	currentTextEdit()->setTextCursor(cursor);
	currentWidget()->setFocus();
}

void Editor::nextTab()
{
	setCurrentIndex(currentIndex() + 1);
}

void Editor::previousTab()
{
	setCurrentIndex(currentIndex() - 1);
}

void Editor::findDialog()
{
	FindDialog dialog(this);

	// Get current selection and set default value.
	QTextCursor cursor = currentTextEdit()->textCursor();
	QString selection = cursor.selectedText();
	if (selection != "") {
		dialog.setSearchText(selection);
	}

	int result = dialog.exec();
	if (result == QDialog::Accepted) {

		lastSearch = dialog.searchText();

		lastSearchOptions = 0;
		if (dialog.caseSensitive()) lastSearchOptions |= QTextDocument::FindCaseSensitively;
		if (dialog.wholeWord()) lastSearchOptions |= QTextDocument::FindWholeWords;
		if (dialog.direction() == FindDialog::Direction_Backward) lastSearchOptions |= QTextDocument::FindBackward;

		if (!find(lastSearch, lastSearchOptions)) {
			QMessageBox::information(this, tr("Find"), tr("String '%0' not found").arg(lastSearch));
		}
	}
}

void Editor::gotoDialog()
{
	GotoDialog dialog(this);

	// Get current line and set default value.
	dialog.setLine(line());

	// Set valid range.
	int lineCount = currentTextEdit()->document()->blockCount();
	dialog.setRange(1, lineCount);

	int result = dialog.exec();
	if (result == QDialog::Accepted) {
		gotoLine(currentIndex(), dialog.line());
	}
}

void Editor::findNext()
{
	if (lastSearch == "") {
		findDialog();
	}
	else {
		if (!find(lastSearch, lastSearchOptions)) {
			QMessageBox::information(this, tr("Find Next"), tr("String '%0' not found").arg(lastSearch));
		}
	}
}

void Editor::findPrevious()
{
	if (lastSearch == "") {
		findDialog();
	}
	else {
		if (!find(lastSearch, lastSearchOptions ^ QTextDocument::FindBackward)) {
			QMessageBox::information(this, tr("Find Previous"), tr("String '%0' not found").arg(lastSearch));
		}
	}
}

bool Editor::find(const QString & text, QTextDocument::FindFlags flags/*=0*/)
{
	QTextCursor cursor = currentTextEdit()->textCursor();

	if (flags & QTextDocument::FindBackward)
	{
		// Move cursor to the beginning of selection.
		cursor.setPosition(cursor.selectionStart());
		currentTextEdit()->setTextCursor(cursor);
	}
	else
	{
		// Move cursor to the end of selection.
		cursor.setPosition(cursor.selectionEnd());
		currentTextEdit()->setTextCursor(cursor);
	}

	return currentTextEdit()->find(text, flags);
}

void Editor::onCurrentChanged(int idx)
{
	if (idx < 0) return;

	emit cursorPositionChanged();

	QTextDocument * document = currentTextEdit()->document();
	QTextCursor cursor = currentTextEdit()->textCursor();

	// update cursor position.
	emit cursorPositionChanged();

	// update copy/undo/redo available.
	emit copyAvailable(cursor.position() != cursor.anchor());
	emit undoAvailable(document->isUndoAvailable());
	emit redoAvailable(document->isRedoAvailable());
	emit pasteAvailable(true);

	connect(currentTextEdit(), SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
	connect(currentTextEdit(), SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
	connect(currentTextEdit(), SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
}

void Editor::onCopyAvailable(bool available)
{
	emit copyAvailable(available);
}
void Editor::onUndoAvailable(bool available)
{
	emit undoAvailable(available);
}
void Editor::onRedoAvailable(bool available)
{
	emit redoAvailable(available);
}

void Editor::setEffect(Effect * effect)
{
	if (effect == NULL)
	{
		// Remove all tabs.
		while (this->count() > 0)
		{
			this->removeTab(0);
		}
	}
	else
	{
		int inputNum = effect->getInputNum();
		for (int i = 0; i < inputNum; i++)
		{
			this->addEditor(effect->getInputName(i), effect, i);
		}
	}
}

QTextEdit * Editor::addEditor(const QString & name, const Effect * effect, int i)
{
	SourceEdit * textEdit = new SourceEdit(this);
	this->addTab(textEdit, name);
	textEdit->setFont(m_font);
	textEdit->setLineWrapMode(QTextEdit::NoWrap);
	textEdit->setTabStopWidth(28);
	textEdit->setAcceptRichText(false);

	QTextDocument * textDocument = textEdit->document();

	Highlighter * hl = new Highlighter(textDocument);
	hl->setRules(effect->factory()->highlightingRules());
	hl->setMultiLineCommentStart(effect->factory()->multiLineCommentStart());
	hl->setMultiLineCommentEnd(effect->factory()->multiLineCommentEnd());

	textEdit->setPlainText(effect->getInput(i));
	textDocument->setModified(false);

	connect(textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
	connect(textEdit, SIGNAL(cursorPositionChanged()), this, SIGNAL(cursorPositionChanged()));
	connect(textDocument, SIGNAL(modificationChanged(bool)), this, SIGNAL(modifiedChanged(bool)));

	if (count() == 1) {
		emit onCurrentChanged(currentIndex());
	}

	return textEdit;
}

void Editor::tabInserted(int index)
{
	QTabWidget::tabInserted(index);

	if (count() == 2) {
		tabBar()->show();
	}
}

void Editor::tabRemoved(int index)
{
	QTabWidget::tabRemoved(index);

	if (count() == 1) {
		tabBar()->hide();
	}
}

