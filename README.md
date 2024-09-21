![Untitled](https://github.com/user-attachments/assets/bc6fd4f4-439b-48fe-831a-7214a588c2f9)

-----------------------------------------------------------------------
CHIP-8 interpreter/emulator
------------------------------------------------------------------------

Difficult, but nothing compared to writing a filesystem in pintOS. 

-------------------------------------------------------------------------
Files
-------------------------------------------------------------------------

chip.c: implementation of chip8 instructions

mysdl.c: implementation of various functions used for rendering graphics using the SDL library

main.c: main function. self explanatory 


You need to manually link the SDL2 library to compile this. 


--------------------------------------------------------------------------
SOURCES:
-------------------------------------------------------------------------

https://austinhenley.com/blog/challengingprojects.html <- Inspiration. Challenge accepted.

https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ <- A lot of help figuring out a lot of the bits and pieces of this project because there are a lot. A decent starter.

https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set <- Extremely useful for understanding the intuition behind every instruction. 

ROM files taken from https://github.com/kripod/chip8-roms/tree/master/games for testing purposes. 

