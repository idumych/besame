/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * Files in ~/config/settings/BeSame: "Hiscore.dat" (the original's format,
 * a flattened BMessage with Name0..4 and Score0..4) and "Settings" (the
 * window position).
 */
#ifndef SETTINGS_H
#define SETTINGS_H

#include <Point.h>

#include "HighScores.h"

class BBitmap;

namespace Settings {

void LoadHighScores(besame::HighScores& table);
status_t SaveHighScores(const besame::HighScores& table);

// false when there's no saved position, or it's off screen
bool LoadWindowPosition(BPoint& position);
void SaveWindowPosition(BPoint position);

}

// One of the original's pictures, from the app's resources ('BSRW' by name).
BBitmap* LoadArt(const char* name);

#endif // SETTINGS_H
