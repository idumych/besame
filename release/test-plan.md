# Test plan: BeSame

BeSame is **self-contained**: a game with built-in pictures and no files to
open. The checks drive the UI and read the result from the screen. The
command key is Alt on Haiku and Ctrl on Linux.

## Fixtures

None.

## Fresh state

Delete the settings folder before the first launch:

- Haiku: `rm -rf ~/config/settings/BeSame`
- Linux: `xtest.sh` starts with an empty HOME; nothing to delete.

## Feature checks

### title

Steps: launch BeSame on the fresh state. The title screen changes between
the credits and the high scores every 10 seconds: take screenshots once a
second until one shows the credits, and save its `ocr-find.sh --text` output
as `$E/{p}-title.txt`.

PASS when: `{p}-title.txt` has "Programmed by" (EVIDENCE_1) and the
screenshot shows the BeSame logo above it.

### play

Steps: choose New game from the Game menu (command key + N). Read the board
from a screenshot and find two balls of the same colour side by side (the
field is 15 x 10 balls of 32 px, from the window's content area just under
the menu bar). Move the pointer onto one of them, wait half a second, and
click. Take a screenshot at once and save its `ocr-find.sh --text` output as
`$E/{p}-screen.txt`.

PASS when: the two balls are gone in the screenshot (the balls above have
fallen into their place) and `{p}-screen.txt` shows "Score:" followed by a
number above zero (EVIDENCE_2). The score drops by 10 every half second
while the window is active, so read it right after the click: two balls are
worth 200.
