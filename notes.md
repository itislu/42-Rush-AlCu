# Algorithm

|
|||||||||||||


Last one picks loses
A || H

A |||||| A
|||



A ||| A


H- | H
H+ |||||||| A
A+ ||| A


Force other to take last one:
	n % 4 == 0
	Number of matches mod 4 equals 1 is BAD to start

Force player to let us take last one:
	n % 4 == 1
	Number of matches mod 4 equals 0 is BAD to start


n % 4 == 1
We lose:
1
5
9

We won:
2
3
4
6
7
8


# Modules

Input:
	Args:
		If 0, read from stdin
		If 1, read from given filename
		Else, print usage

	File
	Stdin
	Double '\n'
	EOF always means just quit

	Validation:
		Each row must be pure number between 1 and 10.000 (tbd)
	
	Read into linked list, then convert to array

Pre-game loop:
	Calculate who should start each row

Game loop:
		Calc best move (only takes current row into account)
		If bad game state, take 1

		Prompt user:
			Validate isnumber
			Validate amount
			Otherwise keep prompting
			EOF is stop game

	Print board

	If no matches and no rows left, print winner and end



## Structures

Row:
	cur_amount
	pref_finisher

Board:
	rows - array of rows
	cur_row

enum Player
	AI
	PLAYER


## Output

Print board in-place

||||||||
||||
||||||||||||||||||||||||
