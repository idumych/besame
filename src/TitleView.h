/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef TITLE_VIEW_H
#define TITLE_VIEW_H

#include <View.h>

class BBitmap;

namespace besame {
class HighScores;
}

class TitleView : public BView {
public:
							TitleView(BRect frame,
								const besame::HighScores& highScores);
	virtual					~TitleView();

	virtual void			Draw(BRect updateRect);
	virtual void			Pulse();

private:
	void					_DrawCentered(const char* text, float y);

	const besame::HighScores& fHighScores;
	BBitmap*				fLogo;
	rgb_color				fGradient[512];
	int32					fGradientOffset;
	int32					fPulses;
	bool					fShowHighScores;
};

#endif // TITLE_VIEW_H
