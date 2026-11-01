/*
 * Copyright 2026 the BeSame contributors
 * SPDX-License-Identifier: MIT
 *
 * Host tests for the portable core: make test
 */
#include "Board.h"
#include "HighScores.h"

#include <cstdio>
#include <string>

using besame::Board;
using besame::HighScores;

static int sFailures = 0;

#define CHECK(condition) \
	do { \
		if (!(condition)) { \
			std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
			sFailures++; \
		} \
	} while (false)


// Builds a board from rows of text, top row first: '.' empty, '0'-'2' colours.
static void
Load(Board& board, const char* const rows[Board::kRows])
{
	for (int r = 0; r < Board::kRows; r++) {
		for (int c = 0; c < Board::kColumns; c++) {
			char ch = rows[r][c];
			board.SetColor(c, r, ch == '.' ? Board::kEmpty : ch - '0');
		}
	}
}


static std::string
Row(const Board& board, int row)
{
	std::string text;
	for (int c = 0; c < Board::kColumns; c++) {
		int color = board.Color(c, row);
		text += color == Board::kEmpty ? '.' : char('0' + color);
	}
	return text;
}


static void
TestDeal()
{
	Board a, b, c;
	a.Deal(42);
	b.Deal(42);
	c.Deal(43);
	bool same = true, differs = false;
	for (int col = 0; col < Board::kColumns; col++) {
		for (int r = 0; r < Board::kRows; r++) {
			CHECK(a.Color(col, r) >= 0 && a.Color(col, r) < Board::kColors);
			same = same && a.Color(col, r) == b.Color(col, r);
			differs = differs || a.Color(col, r) != c.Color(col, r);
		}
	}
	CHECK(same);
	CHECK(differs);
	CHECK(a.BallsLeft() == Board::kColumns * Board::kRows);
	CHECK(a.Score() == 0);
}


static void
TestSelection()
{
	Board board;
	const char* rows[] = {
		"...............",
		"...............",
		"...............",
		"...............",
		"...............",
		"...............",
		"...............",
		"1..............",
		"00.............",
		"012............",
	};
	Load(board, rows);
	CHECK(board.SelectAt(0, 9) == 3);	// the 0s, joined at the edges
	CHECK(board.IsSelected(1, 8));
	CHECK(!board.IsSelected(0, 7));
	CHECK(board.SelectAt(2, 9) == 0);	// a lone ball isn't a group
	CHECK(board.SelectionCount() == 0);
	CHECK(board.SelectAt(5, 5) == 0);	// empty cell
	CHECK(board.SelectAt(-1, 3) == 0);	// outside (the original read past the field)
	CHECK(board.SelectAt(0, Board::kRows) == 0);
	CHECK(board.RemoveSelection() == 0);
}


static void
TestGravityAndColumns()
{
	Board board;
	const char* rows[] = {
		"...............",
		"...............",
		"...............",
		"...............",
		"...............",
		"...............",
		"2..............",
		"1..............",
		"002............",
		"0012...........",
	};
	Load(board, rows);
	CHECK(board.SelectAt(0, 9) == 4);
	CHECK(board.RemoveSelection() == 4);
	CHECK(board.Score() == 400);
	// column 0 falls: 2 over 1; column 1 is now empty and closes up
	CHECK(Row(board, 7) == "...............");
	CHECK(Row(board, 8) == "22.............");
	CHECK(Row(board, 9) == "112............");
	CHECK(!board.IsGameOver());
	CHECK(board.SelectAt(0, 8) == 2);
	board.RemoveSelection();
	CHECK(Row(board, 9) == "112............");
	CHECK(board.SelectAt(0, 9) == 2);
	board.RemoveSelection();
	CHECK(Row(board, 9) == "2..............");
	CHECK(board.IsGameOver());
	CHECK(board.BallsLeft() == 1);
	CHECK(board.Score() == 800);
}


static void
TestSpinAndTick()
{
	Board board;
	const char* rows[] = {
		"...............", "...............", "...............", "...............",
		"...............", "...............", "...............", "...............",
		"...............", "00.............",
	};
	Load(board, rows);
	board.SelectAt(0, 9);
	for (int i = 0; i < Board::kSpinFrames + 2; i++)
		board.Spin();
	CHECK(board.Frame(0, 9) == 2);
	CHECK(board.Frame(5, 5) == 0);

	board.SetScore(25);
	for (int i = 0; i < 4; i++)
		board.Tick();
	CHECK(board.Score() == 25);
	board.Tick();
	CHECK(board.Score() == 15);
	for (int i = 0; i < 10; i++)
		board.Tick();
	CHECK(board.Score() == 5);	// stops below 10
}


static void
TestHighScores()
{
	HighScores table;
	CHECK(table.Name(0) == "Naish" && table.Score(4) == 100);
	CHECK(!table.Qualifies(99));
	CHECK(table.Qualifies(100));
	CHECK(table.Insert("first", 500) == 0);
	CHECK(table.Insert("second", 300) == 1);
	CHECK(table.Insert("tie", 300) == 1);	// above the equal score
	CHECK(table.Name(2) == "second");
	CHECK(table.Name(4) == "Naish");
	CHECK(table.Insert("low", 50) == -1);
	CHECK(table.Insert("a very long name indeed", 120) == 3);
	CHECK(table.Name(3) == "a very long name in");

	// 18 ASCII bytes + a two-byte character: the character doesn't fit
	CHECK(besame::TruncateUTF8("abcdefghijklmnopqr\xc3\xa9", 19)
		== "abcdefghijklmnopqr");
	std::string name = "Ren\xc3\xa9";
	besame::RemoveLastUTF8(name);
	CHECK(name == "Ren");
	std::string empty;
	besame::RemoveLastUTF8(empty);	// the original wrote before the buffer here
	CHECK(empty.empty());
}


int
main()
{
	TestDeal();
	TestSelection();
	TestGravityAndColumns();
	TestSpinAndTick();
	TestHighScores();
	if (sFailures == 0)
		std::printf("core tests: all passed\n");
	return sFailures == 0 ? 0 : 1;
}
