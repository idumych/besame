/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#include "Settings.h"

#include <Bitmap.h>
#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Path.h>
#include <Screen.h>
#include <TranslationUtils.h>

#include <cstdio>

#include "Constants.h"

namespace {

const char* kDirectory = "BeSame";
const char* kHighScoreFile = "Hiscore.dat";
const char* kSettingsFile = "Settings";
// Where Michel Clasquin-Johnson's package of the original keeps its scores
const char* kOldHighScoreDirectory = "besame";


status_t
SettingsPath(BPath& path, const char* directory, const char* leaf, bool create)
{
	status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path, create);
	if (status == B_OK)
		status = path.Append(directory);
	if (status == B_OK && create)
		status = create_directory(path.Path(), 0755);
	if (status == B_OK)
		status = path.Append(leaf);
	return status;
}


status_t
LoadMessage(const char* directory, const char* leaf, BMessage& message)
{
	BPath path;
	status_t status = SettingsPath(path, directory, leaf, false);
	if (status != B_OK)
		return status;
	BFile file(path.Path(), B_READ_ONLY);
	status = file.InitCheck();
	if (status != B_OK)
		return status;
	return message.Unflatten(&file);
}


status_t
SaveMessage(const char* leaf, const BMessage& message)
{
	BPath path;
	status_t status = SettingsPath(path, kDirectory, leaf, true);
	if (status != B_OK)
		return status;
	BFile file(path.Path(), B_WRITE_ONLY | B_CREATE_FILE | B_ERASE_FILE);
	status = file.InitCheck();
	if (status != B_OK)
		return status;
	return message.Flatten(&file);
}

} // namespace


// LoadHighscore() in the original. Scores from the old package are picked
// up the first time.
void
Settings::LoadHighScores(besame::HighScores& table)
{
	table.Reset();
	BMessage message;
	if (LoadMessage(kDirectory, kHighScoreFile, message) != B_OK
		&& LoadMessage(kOldHighScoreDirectory, kHighScoreFile, message) != B_OK)
		return;

	for (int i = 0; i < besame::HighScores::kCount; i++) {
		char nameField[16];
		char scoreField[16];
		snprintf(nameField, sizeof(nameField), "Name%d", i);
		snprintf(scoreField, sizeof(scoreField), "Score%d", i);
		const char* name;
		int32 score;
		if (message.FindString(nameField, &name) == B_OK
			&& message.FindInt32(scoreField, &score) == B_OK)
			table.Set(i, name, score);
	}
}


status_t
Settings::SaveHighScores(const besame::HighScores& table)
{
	BMessage message;
	for (int i = 0; i < besame::HighScores::kCount; i++) {
		char nameField[16];
		char scoreField[16];
		snprintf(nameField, sizeof(nameField), "Name%d", i);
		snprintf(scoreField, sizeof(scoreField), "Score%d", i);
		message.AddString(nameField, table.Name(i).c_str());
		message.AddInt32(scoreField, table.Score(i));
	}
	return SaveMessage(kHighScoreFile, message);
}


bool
Settings::LoadWindowPosition(BPoint& position)
{
	BMessage message;
	if (LoadMessage(kDirectory, kSettingsFile, message) != B_OK
		|| message.FindPoint("window position", &position) != B_OK)
		return false;

	// keep the window's tab on screen
	BRect screen = BScreen().Frame();
	return screen.InsetByCopy(20, 20).Contains(position + BPoint(20, 0));
}


void
Settings::SaveWindowPosition(BPoint position)
{
	BMessage message;
	message.AddPoint("window position", position);
	SaveMessage(kSettingsFile, message);
}


BBitmap*
LoadArt(const char* name)
{
	BBitmap* bitmap = BTranslationUtils::GetBitmap('PNG ', name);
	if (bitmap == NULL)
		fprintf(stderr, "BeSame: can't load the picture \"%s\"\n", name);
	return bitmap;
}
