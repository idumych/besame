# Notes: the original BeSame 1.0

Findings from the original binary (`apps/besame/BeSame` in
`besame_1.0-3_gcc2.hpkg`, x86 gcc2, not stripped), decompiled with Ghidra.
The decompiled code is not in this repository.

## Structure

| Class | Job | Port |
|---|---|---|
| `SameApp` | `application/x-vnd.SameGame`; finds its folder, loads scores, opens the window | `App.cpp` |
| `SameWindow` | `BRect(100,100,579,456)`, "Same Game", not resizable; menu "Project": New Game (0x1005), About (0x1006), Quit (0x1004); swaps one view under the 20 px menu bar | `MainWindow.cpp` |
| `TitelView` | logo at (10,0); two point rows at y 82 and 300 coloured from a 512-entry table (0x21,0,0→255→0), offset +5 per draw; text alternates every 100 pulses | `TitleView.cpp` |
| `PlayView` | 15 x 10 cells of `{int color (-1 empty), int frame, bool dirty}`, 32 px; sprite source `(frame*32, color*32)` | `PlayView.cpp`, `core/Board.cpp` |
| `GameOverView` | Gameover.bmp at (10,100) for 25 pulses, then MSG 0xa02a (name) or 0xa029 (title) | `GameOverView.cpp` |
| `EnterNameView` | be_bold_font 40: "You have a High-score." at (15,100), "Please enter your name:" at (10,140), name centred at y 185; max 19 bytes | `EnterNameView.cpp` |
| `Hiscore` | 5 x `{char name[28]; int32 score}`, default "Naish"/100 | `core/HighScores.cpp` |

## Rules

- Hovering selects the connected group (`FindAllSelected`, recursive);
  a lone ball is deselected. Selected balls advance one frame per pulse,
  modulo 7, while the pointer is inside 0 ≤ x < 479, 0 ≤ y < 320.
- A click removes the selection if it has 2+ balls: +100 per ball. Then
  `ShuffleMatrix` drops balls down and moves non-empty columns left, the
  selection is cleared and redone at the click point, and `CheckIfGameOver`
  (no same-coloured orthogonal neighbours) posts 41000 with "Score".
- Every 5th pulse while the window is active, score -= 10 if score > 9.
- A score qualifies when it's ≥ any entry; it's inserted above the first
  entry it equals or beats.
- Colours: text (0x55,0x99,0xcc) on low (0x22,0x22,0xaa); title fonts
  be_bold_font 20; play view score strip white from y 320, "Score: %d" in
  black at (10,330).
- Strings: About alert "BeSame V1.0\nBy Michael Wulff Nielsen
  (Naish@get2net.dk).\nwww.wulffnet.dk/Beos\n", button "Ok".

## Bugs found

- `SetPulseRate(50)`: 50 µs. On Haiku the title flips pages every 0.1–0.2 s
  and the score drains in a blink (measured in a Renku x86_gcc2h build00068
  VM). BeOS delivered pulses in 0.1 s steps; the port uses 100 ms.
- `rand()` never seeded: the same board every game.
- MouseDown below the board computes row 10 and reads the next column.
- Backspace with an empty name writes `name[-1]`; any byte (arrows, Tab) is
  accepted as a character.
- `Hiscore.dat` is read from and written to the app's own folder.
- Missing BMPs: `exit(-1)`.

## Measured in the reference VM

The port's field (480 x 337 under the menu bar) was compared with the
original's at the same origin: logo template error 0, identical text rows on
the credits and high-score pages, identical ball rows on the field.
