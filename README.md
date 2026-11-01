# BeSame

A ball-removing puzzle for Haiku and Renku OS: a native port of **BeSame 1.0**
for BeOS (1999) by Michael Wulff Nielsen, with its original graphics.

Point at a ball and its group of same-coloured neighbours starts spinning.
Click to remove the group (two balls or more). The balls above fall down and
empty columns close up to the left. Every ball is worth 100 points, but the
clock takes 10 points off every half second while the window is active, so
be quick. The game ends when no two neighbouring balls share a colour; the
five best scores are kept.

## Why a port

The only Haiku package of BeSame (Michel Clasquin-Johnson's
`besame-1.0-4`) holds the original gcc2 binary, which runs only on 32-bit
gcc2-hybrid systems. Run there today it's unplayable: it asks for a pulse
every 50 microseconds, which BeOS rounded to its 0.1 s steps but Haiku
honours, so the clock drains the score faster than you can click.

There is no source code for BeSame, so this is a rewrite in modern C++
against the Haiku API, following the original's behaviour as recovered from
its binary, and checked against it side by side in a virtual machine. The
title screen, playing field and high-score page match the original's layout
pixel for pixel.

## What's different from the original

- **Speed:** the game pulses every 0.1 s, as it did on BeOS.
- **New board every game:** the original never seeded its random numbers, so
  every game started with the same board.
- **Menus:** a "Game" menu with New game (Alt+N), About BeSame… and Quit
  (Alt+Q), in Haiku's style, instead of "Project" with New Game, About and
  Quit without shortcuts. The window is titled "BeSame" instead of "Same
  Game".
- **High scores** live in `~/config/settings/BeSame/Hiscore.dat`, in the
  original's format, instead of next to the program (read-only on Haiku).
  Scores from the old package (`~/config/settings/besame/`) are picked up on
  the first start. They are saved as soon as a name is entered.
- **Window position:** centred on the first start, then remembered.
- **Name entry:** "Please enter your name:" ran off the window in today's
  fonts; it's shrunk to fit. Only printable characters are taken (UTF-8
  too), and Backspace on an empty name no longer writes outside the name.
- **Clicks below the board** (on the score) no longer read past the field.
- **Missing pictures** no longer make the game exit.

## Building

On Haiku or Renku:

    make            # ./BeSame
    make test       # tests of the game logic, on any system with g++
    make install    # into ~/config/non-packaged/apps

Linux builds go through [Hokku](https://github.com/idumych/hokku) (Cosmoe)
and are made by CI as an AppImage.

## Credits

- BeSame 1.0: programming by Michael Wulff Nielsen, graphics by Michael
  Wulff Nielsen and Jeppe Isaaksen, beta testing by Matthias Schonder.
- Icon and the Haiku package of the original: Michel Clasquin-Johnson.

## Licence

The code is MIT. The pictures in `art/` are the original freeware game's;
see [LICENSE](LICENSE).
