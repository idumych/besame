/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#include "HighScores.h"

namespace besame {

HighScores::HighScores()
{
	Reset();
}


// Hiscore::Hiscore
void
HighScores::Reset()
{
	for (int i = 0; i < kCount; i++) {
		fNames[i] = "Naish";
		fScores[i] = 100;
	}
}


void
HighScores::Set(int index, const std::string& name, int score)
{
	if (index < 0 || index >= kCount)
		return;
	fNames[index] = TruncateUTF8(name, kMaxNameBytes);
	fScores[index] = score;
}


bool
HighScores::Qualifies(int score) const
{
	for (int i = 0; i < kCount; i++) {
		if (fScores[i] <= score)
			return true;
	}
	return false;
}


int
HighScores::Insert(const std::string& name, int score)
{
	int at = 0;
	while (at < kCount && score < fScores[at])
		at++;
	if (at == kCount)
		return -1;

	for (int i = kCount - 1; i > at; i--) {
		fNames[i] = fNames[i - 1];
		fScores[i] = fScores[i - 1];
	}
	fNames[at] = TruncateUTF8(name, kMaxNameBytes);
	fScores[at] = score;
	return at;
}


std::string
TruncateUTF8(const std::string& text, size_t maxBytes)
{
	if (text.size() <= maxBytes)
		return text;
	size_t end = maxBytes;
	// back up over continuation bytes (10xxxxxx) to a character start
	while (end > 0 && (static_cast<unsigned char>(text[end]) & 0xc0) == 0x80)
		end--;
	return text.substr(0, end);
}


void
RemoveLastUTF8(std::string& text)
{
	if (text.empty())
		return;
	size_t end = text.size() - 1;
	while (end > 0 && (static_cast<unsigned char>(text[end]) & 0xc0) == 0x80)
		end--;
	text.erase(end);
}

} // namespace besame
