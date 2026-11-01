/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <GraphicsDefs.h>
#include <Point.h>
#include <SupportDefs.h>

#define APP_SIGNATURE "application/x-vnd.idumych-BeSame"

// The original's window: BRect(100, 100, 579, 456), a 20 px menu bar and
// four 480 x 337 views shown one at a time.
static const float kFieldWidth = 480;
static const float kFieldHeight = 337;

// The original asked for a pulse every 50 microseconds. BeOS delivered
// pulses in steps of 0.1 s; Haiku honours the rate, which makes the original
// unplayable there (the clock eats the score in a blink). 0.1 s it is.
static const bigtime_t kPulseRate = 100000;

enum {
	kMsgNewGame			= 'NewG',	// menu: start a game
	kMsgGameOver		= 'GOvr',	// PlayView: no more groups ("score")
	kMsgShowTitle		= 'Titl',	// GameOverView: back to the title
	kMsgEnterName		= 'Name',	// GameOverView: a high score ("score")
	kMsgNameEntered		= 'NmOK'	// EnterNameView: "name", "score"
};

// The original's colours
static const rgb_color kBlack = { 0, 0, 0, 255 };
static const rgb_color kWhite = { 255, 255, 255, 255 };
static const rgb_color kTitleText = { 0x55, 0x99, 0xcc, 255 };
static const rgb_color kTitleTextLow = { 0x22, 0x22, 0xaa, 255 };

#endif // CONSTANTS_H
