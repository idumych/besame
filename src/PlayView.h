/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef PLAY_VIEW_H
#define PLAY_VIEW_H

#include <View.h>

#include "Board.h"

class BBitmap;

class PlayView : public BView {
public:
							PlayView(BRect frame, uint32 seed = 0);
	virtual					~PlayView();

	virtual void			Draw(BRect updateRect);
	virtual void			MouseDown(BPoint where);
	virtual void			MouseMoved(BPoint where, uint32 transit,
								const BMessage* dragMessage);
	virtual void			Pulse();

	const besame::Board&	Board() const { return fBoard; }

private:
	bool					_CellAt(BPoint where, int& column, int& row) const;
	BRect					_CellFrame(int column, int row) const;

	besame::Board			fBoard;
	BBitmap*				fBalls;
	bool					fPointerInside;
	int						fHoverColumn;
	int						fHoverRow;
};

#endif // PLAY_VIEW_H
