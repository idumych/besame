/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * The five best scores. Portable; HighScoreFile does the reading and writing.
 */
#ifndef BESAME_HIGH_SCORES_H
#define BESAME_HIGH_SCORES_H

#include <string>

namespace besame {

class HighScores {
public:
	static const int kCount = 5;
	static const int kMaxNameBytes = 19;	// the original's limit

						HighScores();

	// The original's table: "Naish", 100 points, five times.
	void				Reset();

	const std::string&	Name(int index) const { return fNames[index]; }
	int					Score(int index) const { return fScores[index]; }
	void				Set(int index, const std::string& name, int score);

	// GameOverView::Pulse: a score equal to an entry also gets in.
	bool				Qualifies(int score) const;

	// SameWindow::MessageReceived (MSG_ENTERNAME): goes in above the first
	// entry it equals or beats. Returns the position, or -1.
	int					Insert(const std::string& name, int score);

private:
	std::string			fNames[kCount];
	int					fScores[kCount];
};

// Cuts a name to at most maxBytes bytes without splitting a UTF-8 character.
std::string TruncateUTF8(const std::string& text, size_t maxBytes);

// Removes the last UTF-8 character.
void RemoveLastUTF8(std::string& text);

} // namespace besame

#endif // BESAME_HIGH_SCORES_H
