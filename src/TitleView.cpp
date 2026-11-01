/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * The title screen, after the original's TitelView: the logo, two lines
 * with a blue gradient running along them, and between them the credits or
 * the high scores, changing every 100 pulses.
 */
#include "TitleView.h"

#include <Bitmap.h>
#include <Catalog.h>

#include <cstdio>

#include "Constants.h"
#include "HighScores.h"
#include "Settings.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "TitleView"

static const BPoint kLogoPosition(10, 0);
static const float kLineTop = 82;
static const float kLineBottom = 300;
static const int32 kPulsesPerPage = 100;
static const float kTextSize = 20;
static const float kCenter = 240;


TitleView::TitleView(BRect frame, const besame::HighScores& highScores)
	:
	BView(frame, "title", B_FOLLOW_NONE, B_WILL_DRAW | B_PULSE_NEEDED),
	fHighScores(highScores),
	fLogo(LoadArt("logo")),
	fGradientOffset(0),
	fPulses(0),
	fShowHighScores(false)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	// up from black-ish violet to blue, and back down
	for (int32 i = 0; i < 256; i++) {
		fGradient[i] = make_color(0x21, 0, i);
		fGradient[511 - i] = make_color(0x21, 0, i);
	}
}


TitleView::~TitleView()
{
	delete fLogo;
}


void
TitleView::Draw(BRect updateRect)
{
	BRect bounds = Bounds();
	SetHighColor(kBlack);
	FillRect(bounds);
	if (fLogo != NULL)
		DrawBitmap(fLogo, kLogoPosition);

	// The original drew a point per column: a line at the top and the
	// bottom of the text, with the colours moving along it
	BeginLineArray(2 * (bounds.IntegerWidth() + 1));
	for (int32 x = 0; x <= bounds.IntegerWidth(); x++) {
		rgb_color color = fGradient[(fGradientOffset + x) % 512];
		AddLine(BPoint(x, kLineTop), BPoint(x, kLineTop), color);
		AddLine(BPoint(x, kLineBottom), BPoint(x, kLineBottom), color);
	}
	EndLineArray();

	BFont font(be_bold_font);
	font.SetSize(kTextSize);
	SetFont(&font);
	SetHighColor(kTitleText);
	SetLowColor(kTitleTextLow);

	if (!fShowHighScores) {
		_DrawCentered(B_TRANSLATE("Programmed by:"), 120);
		_DrawCentered("Michael Wulff Nielsen", 140);
		_DrawCentered(B_TRANSLATE("Graphics by:"), 170);
		_DrawCentered("Michael Wulff Nielsen", 190);
		_DrawCentered("Jeppe Isaaksen", 210);
		_DrawCentered(B_TRANSLATE("Betatesting by:"), 240);
		_DrawCentered("Matthias Schonder", 260);
		return;
	}

	_DrawCentered(B_TRANSLATE("High scores:"), 120);
	for (int i = 0; i < besame::HighScores::kCount; i++) {
		float y = 165 + 20 * i;
		DrawString(fHighScores.Name(i).c_str(), BPoint(20, y));
		char score[32];
		snprintf(score, sizeof(score), "%d", fHighScores.Score(i));
		DrawString(score, BPoint(460 - StringWidth(score), y));
	}
}


void
TitleView::Pulse()
{
	fGradientOffset = (fGradientOffset + 5) % 512;
	if (++fPulses == kPulsesPerPage) {
		fPulses = 0;
		fShowHighScores = !fShowHighScores;
	}
	Invalidate();
}


void
TitleView::_DrawCentered(const char* text, float y)
{
	DrawString(text, BPoint(kCenter - StringWidth(text) / 2, y));
}
