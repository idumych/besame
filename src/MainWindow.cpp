/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * The original's SameWindow: a menu bar over one of four views (title,
 * playing field, game over, name entry), swapped as the game goes on.
 */
#include "MainWindow.h"

#include <Application.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>

#include "Constants.h"
#include "EnterNameView.h"
#include "GameOverView.h"
#include "PlayView.h"
#include "Settings.h"
#include "TitleView.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "MainWindow"


MainWindow::MainWindow()
	:
	BWindow(BRect(0, 0, kFieldWidth - 1, kFieldHeight - 1), "BeSame",
		B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE
			| B_AUTO_UPDATE_SIZE_LIMITS | B_QUIT_ON_WINDOW_CLOSE),
	fCurrent(NULL)
{
	Settings::LoadHighScores(fHighScores);

	BMenuBar* menuBar = new BMenuBar("menu bar");
	BMenu* game = new BMenu(B_TRANSLATE("Game"));
	game->AddItem(new BMenuItem(B_TRANSLATE("New game"),
		new BMessage(kMsgNewGame), 'N'));
	game->AddSeparatorItem();
	BMenuItem* about = new BMenuItem(B_TRANSLATE("About BeSame" B_UTF8_ELLIPSIS),
		new BMessage(B_ABOUT_REQUESTED));
	about->SetTarget(be_app);
	game->AddItem(about);
	game->AddSeparatorItem();
	game->AddItem(new BMenuItem(B_TRANSLATE("Quit"),
		new BMessage(B_QUIT_REQUESTED), 'Q'));
	menuBar->AddItem(game);

	fField = new BView("field", 0);
	fField->SetViewColor(kBlack);
	fField->SetExplicitSize(BSize(kFieldWidth - 1, kFieldHeight - 1));

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.Add(menuBar)
		.Add(fField);

	SetPulseRate(kPulseRate);
	_Show(new TitleView(_FieldFrame(), fHighScores));

	ResizeToPreferred();
	BPoint position;
	if (Settings::LoadWindowPosition(position))
		MoveTo(position);
	else
		CenterOnScreen();
}


void
MainWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kMsgNewGame:
			_Show(new PlayView(_FieldFrame()));
			break;

		case kMsgGameOver:
			_Show(new GameOverView(_FieldFrame(),
				message->GetInt32("score", 0), fHighScores));
			break;

		case kMsgShowTitle:
			_Show(new TitleView(_FieldFrame(), fHighScores));
			break;

		case kMsgEnterName:
			_Show(new EnterNameView(_FieldFrame(),
				message->GetInt32("score", 0)));
			break;

		case kMsgNameEntered:
			fHighScores.Insert(message->GetString("name", ""),
				message->GetInt32("score", 0));
			Settings::SaveHighScores(fHighScores);
			_Show(new TitleView(_FieldFrame(), fHighScores));
			break;

		default:
			BWindow::MessageReceived(message);
			break;
	}
}


bool
MainWindow::QuitRequested()
{
	Settings::SaveWindowPosition(Frame().LeftTop());
	return true;
}


void
MainWindow::_Show(BView* view)
{
	if (fCurrent != NULL) {
		fCurrent->RemoveSelf();
		delete fCurrent;
	}
	fCurrent = view;
	fField->AddChild(view);
}


BRect
MainWindow::_FieldFrame() const
{
	return BRect(0, 0, kFieldWidth - 1, kFieldHeight - 1);
}
