/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 */
#include "Board.h"

#include <chrono>
#include <random>
#include <utility>
#include <vector>

namespace besame {

Board::Board()
	:
	fSelectionCount(0),
	fScore(0),
	fTicks(0)
{
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++)
			fCells[c][r] = Cell{ kEmpty, 0, false };
	}
}


// PlayView::PlayView. The original called rand() without seeding it, so
// every game started with the same field; we seed.
void
Board::Deal(uint32_t seed)
{
	if (seed == 0) {
		seed = static_cast<uint32_t>(std::chrono::steady_clock::now()
			.time_since_epoch().count());
	}
	std::mt19937 random(seed);
	std::uniform_int_distribution<int> color(0, kColors - 1);
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++)
			fCells[c][r] = Cell{ static_cast<int8_t>(color(random)), 0, false };
	}
	fSelectionCount = 0;
	fScore = 0;
	fTicks = 0;
}


void
Board::SetColor(int column, int row, int color)
{
	if (IsInside(column, row))
		fCells[column][row].color = static_cast<int8_t>(color);
}


int
Board::Color(int column, int row) const
{
	return IsInside(column, row) ? fCells[column][row].color : kEmpty;
}


int
Board::Frame(int column, int row) const
{
	return IsInside(column, row) ? fCells[column][row].frame : 0;
}


bool
Board::IsSelected(int column, int row) const
{
	return IsInside(column, row) && fCells[column][row].selected;
}


void
Board::ClearSelection()
{
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++)
			fCells[c][r].selected = false;
	}
	fSelectionCount = 0;
}


// PlayView::FindAllSelected, with a work list instead of recursion.
int
Board::SelectAt(int column, int row)
{
	ClearSelection();
	int color = Color(column, row);
	if (color == kEmpty)
		return 0;

	std::vector<std::pair<int, int> > work;
	work.push_back(std::make_pair(column, row));
	fCells[column][row].selected = true;
	int count = 0;
	while (!work.empty()) {
		std::pair<int, int> cell = work.back();
		work.pop_back();
		count++;
		static const int kSteps[4][2] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
		for (int i = 0; i < 4; i++) {
			int c = cell.first + kSteps[i][0];
			int r = cell.second + kSteps[i][1];
			if (Color(c, r) == color && !fCells[c][r].selected) {
				fCells[c][r].selected = true;
				work.push_back(std::make_pair(c, r));
			}
		}
	}

	if (count < 2) {
		ClearSelection();
		return 0;
	}
	fSelectionCount = count;
	return count;
}


// PlayView::MouseDown: 100 points for every ball, whatever the group's size.
int
Board::RemoveSelection()
{
	if (fSelectionCount < 2)
		return 0;

	int removed = 0;
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++) {
			if (fCells[c][r].selected) {
				fCells[c][r] = Cell{ kEmpty, 0, false };
				removed++;
			}
		}
	}
	fScore += removed * kPointsPerBall;
	fSelectionCount = 0;
	_Compact();
	return removed;
}


void
Board::Spin()
{
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++) {
			if (fCells[c][r].selected)
				fCells[c][r].frame = (fCells[c][r].frame + 1) % kSpinFrames;
		}
	}
}


// PlayView::Pulse
void
Board::Tick()
{
	if (++fTicks == 5) {
		fTicks = 0;
		if (fScore > 9)
			fScore -= 10;
	}
}


// PlayView::CheckIfGameOver
bool
Board::IsGameOver() const
{
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++) {
			int color = fCells[c][r].color;
			if (color == kEmpty)
				continue;
			if (Color(c + 1, r) == color || Color(c, r + 1) == color)
				return false;
		}
	}
	return true;
}


int
Board::BallsLeft() const
{
	int count = 0;
	for (int c = 0; c < kColumns; c++) {
		for (int r = 0; r < kRows; r++) {
			if (fCells[c][r].color != kEmpty)
				count++;
		}
	}
	return count;
}


// PlayView::ShuffleMatrix: balls fall to the bottom of their column, keeping
// their order and frame; then the remaining columns close up to the left.
void
Board::_Compact()
{
	for (int c = 0; c < kColumns; c++) {
		int to = kRows - 1;
		for (int r = kRows - 1; r >= 0; r--) {
			if (fCells[c][r].color == kEmpty)
				continue;
			if (r != to) {
				fCells[c][to] = fCells[c][r];
				fCells[c][r] = Cell{ kEmpty, 0, false };
			}
			to--;
		}
	}

	int to = 0;
	for (int c = 0; c < kColumns; c++) {
		if (fCells[c][kRows - 1].color == kEmpty)
			continue;
		if (c != to) {
			for (int r = 0; r < kRows; r++) {
				fCells[to][r] = fCells[c][r];
				fCells[c][r] = Cell{ kEmpty, 0, false };
			}
		}
		to++;
	}
}

} // namespace besame
