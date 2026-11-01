/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef ENTER_NAME_VIEW_H
#define ENTER_NAME_VIEW_H

#include <View.h>

#include <string>

class EnterNameView : public BView {
public:
							EnterNameView(BRect frame, int32 score);

	virtual void			AttachedToWindow();
	virtual void			Draw(BRect updateRect);
	virtual void			KeyDown(const char* bytes, int32 numBytes);
	virtual void			MouseDown(BPoint where);

	const std::string&		Name() const { return fName; }

private:
	void					_DrawFitted(const char* text, float x, float y);

	std::string				fName;
	int32					fScore;
};

#endif // ENTER_NAME_VIEW_H
