/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#include "Settings.h"

#include <Application.h>
#include <Bitmap.h>
#include <Directory.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>
#include <Path.h>
#include <Resources.h>
#include <Screen.h>

#include <cstdio>
#include <cstring>

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


// The pictures are raw B_RGBA32 pixels (tools/png2raw.py) rather than PNGs:
// not every Haiku has a PNG translator (the arm64 nightlies have none).
BBitmap*
LoadArt(const char* name)
{
	size_t size = 0;
	const uint8* data = NULL;
	BResources* resources = BApplication::AppResources();
	if (resources != NULL)
		data = (const uint8*)resources->LoadResource('BSRW', name, &size);

	uint32 width = 0, height = 0;
	if (data != NULL && size >= 12 && memcmp(data, "BSRW", 4) == 0) {
		width = data[4] | data[5] << 8 | data[6] << 16 | (uint32)data[7] << 24;
		height = data[8] | data[9] << 8 | data[10] << 16 | (uint32)data[11] << 24;
	}
	if (width == 0 || height == 0 || width > 4096 || height > 4096
		|| size < 12 + (size_t)width * height * 4) {
		fprintf(stderr, "BeSame: can't load the picture \"%s\"\n", name);
		return NULL;
	}

	BBitmap* bitmap = new BBitmap(BRect(0, 0, width - 1, height - 1), B_RGBA32);
	if (bitmap->InitCheck() != B_OK) {
		delete bitmap;
		return NULL;
	}
	uint8* bits = (uint8*)bitmap->Bits();
	for (uint32 y = 0; y < height; y++)
		memcpy(bits + y * bitmap->BytesPerRow(), data + 12 + y * width * 4, width * 4);
	return bitmap;
}
