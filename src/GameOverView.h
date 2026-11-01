/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef GAME_OVER_VIEW_H
#define GAME_OVER_VIEW_H

#include <View.h>

class BBitmap;

namespace besame {
class HighScores;
}

class GameOverView : public BView {
public:
							GameOverView(BRect frame, int32 score,
								const besame::HighScores& highScores);
	virtual					~GameOverView();

	virtual void			Draw(BRect updateRect);
	virtual void			Pulse();

private:
	const besame::HighScores& fHighScores;
	BBitmap*				fBanner;
	int32					fScore;
	int32					fPulses;
	bool					fDone;
};

#endif // GAME_OVER_VIEW_H
