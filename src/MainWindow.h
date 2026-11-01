/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <Window.h>

#include "HighScores.h"

class MainWindow : public BWindow {
public:
							MainWindow();

	virtual void			MessageReceived(BMessage* message);
	virtual bool			QuitRequested();

private:
	void					_Show(BView* view);
	BRect					_FieldFrame() const;

	BView*					fField;
	BView*					fCurrent;
	besame::HighScores		fHighScores;
};

#endif // MAIN_WINDOW_H
