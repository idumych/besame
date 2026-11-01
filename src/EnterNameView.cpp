/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * Asks for a name in big letters (the original's EnterNameView): type it,
 * Backspace to correct, Return to put it in the table.
 */
#include "EnterNameView.h"

#include <Catalog.h>
#include <Window.h>

#include "Constants.h"
#include "HighScores.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "EnterNameView"

static const float kTextSize = 40;
static const float kCenter = 240;


EnterNameView::EnterNameView(BRect frame, int32 score)
	:
	BView(frame, "enter name", B_FOLLOW_NONE, B_WILL_DRAW | B_NAVIGABLE),
	fScore(score)
{
	SetViewColor(kBlack);
}


void
EnterNameView::AttachedToWindow()
{
	BView::AttachedToWindow();
	BFont font(be_bold_font);
	font.SetSize(kTextSize);
	SetFont(&font);
	MakeFocus(true);
}


void
EnterNameView::Draw(BRect updateRect)
{
	SetHighColor(kTitleText);
	SetLowColor(kBlack);
	_DrawFitted(B_TRANSLATE("You have a high score."), 15, 100);
	_DrawFitted(B_TRANSLATE("Please enter your name:"), 10, 140);
	DrawString(fName.c_str(),
		BPoint(kCenter - StringWidth(fName.c_str()) / 2, 185));
}


void
EnterNameView::KeyDown(const char* bytes, int32 numBytes)
{
	switch (bytes[0]) {
		case B_ENTER:
		{
			BMessage message(kMsgNameEntered);
			message.AddString("name", fName.c_str());
			message.AddInt32("score", fScore);
			Window()->PostMessage(&message);
			return;
		}

		case B_BACKSPACE:
			besame::RemoveLastUTF8(fName);
			Invalidate();
			return;

		default:
			// printable characters only; the original took any byte
			if (static_cast<unsigned char>(bytes[0]) < B_SPACE
				|| bytes[0] == B_DELETE
				|| fName.size() + numBytes > besame::HighScores::kMaxNameBytes) {
				BView::KeyDown(bytes, numBytes);
				return;
			}
			fName.append(bytes, numBytes);
			Invalidate();
			return;
	}
}


void
EnterNameView::MouseDown(BPoint where)
{
	MakeFocus(true);
}


// The original's strings are wider than the window in some fonts:
// shrink a line until it fits.
void
EnterNameView::_DrawFitted(const char* text, float x, float y)
{
	BFont font;
	GetFont(&font);
	float maxWidth = Bounds().Width() - 2 * x;
	while (font.Size() > 12 && font.StringWidth(text) > maxWidth)
		font.SetSize(font.Size() - 1);
	SetFont(&font, B_FONT_SIZE);
	DrawString(text, BPoint(x, y));
	font.SetSize(kTextSize);
	SetFont(&font, B_FONT_SIZE);
}
