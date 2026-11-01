/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * "Game over" for 25 pulses, then the name entry for a high score or the
 * title screen (the original's GameOverView).
 */
#include "GameOverView.h"

#include <Bitmap.h>
#include <Window.h>

#include "Constants.h"
#include "HighScores.h"
#include "Settings.h"

static const BPoint kBannerPosition(10, 100);
static const int32 kPulsesShown = 25;


GameOverView::GameOverView(BRect frame, int32 score,
	const besame::HighScores& highScores)
	:
	BView(frame, "game over", B_FOLLOW_NONE, B_WILL_DRAW | B_PULSE_NEEDED),
	fHighScores(highScores),
	fBanner(LoadArt("gameover")),
	fScore(score),
	fPulses(0),
	fDone(false)
{
	SetViewColor(kBlack);
}


GameOverView::~GameOverView()
{
	delete fBanner;
}


void
GameOverView::Draw(BRect updateRect)
{
	if (fBanner != NULL)
		DrawBitmap(fBanner, kBannerPosition);
}


void
GameOverView::Pulse()
{
	if (fDone || ++fPulses < kPulsesShown)
		return;

	fDone = true;
	if (fHighScores.Qualifies(fScore)) {
		BMessage message(kMsgEnterName);
		message.AddInt32("score", fScore);
		Window()->PostMessage(&message);
	} else
		Window()->PostMessage(kMsgShowTitle);
}
