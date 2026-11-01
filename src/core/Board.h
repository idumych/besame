/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * The BeSame playing field: 15 columns of 10 balls in 3 colours.
 * Portable: no Haiku headers, so it is tested on the host (make test).
 * Follows the original's PlayView (BeSame 1.0, Michael Wulff Nielsen).
 */
#ifndef BESAME_BOARD_H
#define BESAME_BOARD_H

#include <cstdint>

namespace besame {

class Board {
public:
	static const int kColumns = 15;
	static const int kRows = 10;		// row 0 is the top
	static const int kColors = 3;
	static const int kSpinFrames = 7;	// the sprite sheet has 8; the original used 7
	static const int kEmpty = -1;
	static const int kPointsPerBall = 100;

						Board();

	// Fills the field with random balls; seed 0 picks one from the clock.
	void				Deal(uint32_t seed = 0);
	// Sets one cell, for tests.
	void				SetColor(int column, int row, int color);

	int					Color(int column, int row) const;
	int					Frame(int column, int row) const;
	bool				IsSelected(int column, int row) const;
	int					SelectionCount() const { return fSelectionCount; }
	int					Score() const { return fScore; }
	void				SetScore(int score) { fScore = score; }

	// Selects the group of same-coloured balls connected to the cell;
	// a lone ball isn't a group, so the selection stays empty.
	// Returns the size of the selection.
	int					SelectAt(int column, int row);
	void				ClearSelection();

	// Removes the selected group (2 balls or more), scores it, lets the
	// balls above fall and closes empty columns to the left.
	// Returns the number of balls removed.
	int					RemoveSelection();

	// Turns every selected ball one frame further.
	void				Spin();

	// The original took 10 points off every fifth pulse while the score
	// was 10 or more.
	void				Tick();

	// No two neighbouring balls of the same colour are left.
	bool				IsGameOver() const;
	int					BallsLeft() const;

	static bool			IsInside(int column, int row)
							{ return column >= 0 && column < kColumns
								&& row >= 0 && row < kRows; }

private:
	struct Cell {
		int8_t			color;
		int8_t			frame;
		bool			selected;
	};

	void				_Compact();

	Cell				fCells[kColumns][kRows];
	int					fSelectionCount;
	int					fScore;
	int					fTicks;
};

} // namespace besame

#endif // BESAME_BOARD_H
