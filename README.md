This is a simple enough implementation of a brainf**k interpreter written
in ANSI C. I made this purely to see how brainf**k works and to have an
interpreter to do it with. In my experience it is good enough to tinker
around with brainf**k.

For more info about brainf**k go [here](http://en.wikipedia.org/wiki/Brainfuck).

Compile the project with:
`make`

It can be used to interpret bf code and to generate an ANSI C implementation
of the given bf code.

Interpret bf code(also works without -i):
`brainfuck -i file_with_bfcode.bf`

Clean up bf code(without interpreting it):
`brainfuck -n -w file_with_bfcode.bf`

"Compile" bf code:
`brainfuck -n -c file_with_bfcode.bf > c_code.c && gcc -o program_name c_code.c`

The examples are from Wikipedia and the game of life example is from [here](http://www.df.lth.se/~lft/brainfuck/).
I personally think the code formatting for the game of life example is
awesome! If you are reading this Linus Åkesson, well done, sir!

If you need to contact me for any further info or to send me a tip on how to
better this project or any other project of mine, drop me a line or more at
*mladjo2505@gmail.com*

So to paraphrase the great Donald Ervin Knuth:
Beware of bugs in the above code; I have only tried it, not proved it correct. :)

Happy hacking and the best of luck to you all.
