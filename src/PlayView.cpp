/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * The playing field, after the original's PlayView: 32 px balls from a
 * sprite sheet (a row per colour, a column per frame), a white score strip
 * below. The group under the pointer spins; a click removes it.
 */
#include "PlayView.h"

#include <Bitmap.h>
#include <Catalog.h>
#include <Window.h>

#include <cstdio>

#include "Constants.h"
#include "Settings.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "PlayView"

using besame::Board;

static const float kBallSize = 32;
static const float kScoreTop = Board::kRows * kBallSize;	// 320
static const BPoint kScorePosition(10, 330);


PlayView::PlayView(BRect frame, uint32 seed)
	:
	BView(frame, "play", B_FOLLOW_NONE, B_WILL_DRAW | B_PULSE_NEEDED),
	fBalls(LoadArt("balls")),
	fPointerInside(false),
	fHoverColumn(-1),
	fHoverRow(-1)
{
	SetViewColor(B_TRANSPARENT_COLOR);
	fBoard.Deal(seed);
}


PlayView::~PlayView()
{
	delete fBalls;
}


void
PlayView::Draw(BRect updateRect)
{
	SetHighColor(kWhite);
	SetLowColor(kWhite);
	FillRect(BRect(0, kScoreTop, Bounds().right, Bounds().bottom));
	SetHighColor(kBlack);
	char score[64];
	snprintf(score, sizeof(score), B_TRANSLATE("Score: %d"), fBoard.Score());
	DrawString(score, kScorePosition);

	SetLowColor(kBlack);
	for (int column = 0; column < Board::kColumns; column++) {
		for (int row = 0; row < Board::kRows; row++) {
			BRect frame = _CellFrame(column, row);
			if (!frame.Intersects(updateRect))
				continue;
			int color = fBoard.Color(column, row);
			if (color == Board::kEmpty || fBalls == NULL) {
				FillRect(frame);
				continue;
			}
			BRect source(0, 0, kBallSize - 1, kBallSize - 1);
			source.OffsetTo(fBoard.Frame(column, row) * kBallSize, color * kBallSize);
			DrawBitmapAsync(fBalls, source, frame);
		}
	}
	Sync();
}


void
PlayView::MouseDown(BPoint where)
{
	int column, row;
	if (!_CellAt(where, column, row) || fBoard.SelectionCount() < 2)
		return;

	fBoard.RemoveSelection();
	fBoard.SelectAt(column, row);
	fHoverColumn = column;
	fHoverRow = row;
	Invalidate();

	if (fBoard.IsGameOver()) {
		BMessage message(kMsgGameOver);
		message.AddInt32("score", fBoard.Score());
		Window()->PostMessage(&message);
	}
}


// The original polled the pointer in Pulse(); a new cell selects its group,
// and the balls stop spinning while the pointer is off the field.
void
PlayView::MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage)
{
	int column = -1, row = -1;
	fPointerInside = transit != B_EXITED_VIEW && transit != B_OUTSIDE_VIEW
		&& _CellAt(where, column, row);
	if (!fPointerInside || (column == fHoverColumn && row == fHoverRow))
		return;

	fHoverColumn = column;
	fHoverRow = row;
	fBoard.SelectAt(column, row);
	Invalidate();
}


void
PlayView::Pulse()
{
	// The clock only runs while the window is active
	if (!Window()->IsActive())
		return;

	fBoard.Tick();
	if (fPointerInside)
		fBoard.Spin();
	Invalidate();
}


bool
PlayView::_CellAt(BPoint where, int& column, int& row) const
{
	if (where.x < 0 || where.y < 0)
		return false;
	column = static_cast<int>(where.x / kBallSize);
	row = static_cast<int>(where.y / kBallSize);
	return Board::IsInside(column, row);
}


BRect
PlayView::_CellFrame(int column, int row) const
{
	return BRect(column * kBallSize, row * kBallSize,
		(column + 1) * kBallSize - 1, (row + 1) * kBallSize - 1);
}
