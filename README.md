# Companions-of-Floris
This program generates viable groups of specific sizes according to http://floris.wikia.com/wiki/Companions_of_Floris

Our son wanted to play this game and asked if I could help find the largest stable group of companions. While this is pretty straightforward task to look at n!m combinations of the 22 companions, finding a programmatic way to actually generate all those possibilities wasn't obvious, until I found this nice code by Matthew Belmont

 Coded by Matthew Belmonte <mkb4@Cornell.edu>, 23 March 1996.  This
 implementation Copyright (c) 1996 by Matthew Belmonte.  Permission for use and
 distribution is hereby granted, subject to the restrictions that this
 copyright notice and reference list be included in its entirety, and that any
 and all changes made to the program be clearly noted in the program text.
 
 Reference:
 
 Phillip J Chase, `Algorithm 382: Combinations of M out of N Objects [G6]',
 Communications of the Association for Computing Machinery 13:6:368 (1970).
 
 The program generates all possible variations of 22!n groups and tests them for viability according to the rules of likes and dislikes. The forums had indicated 3 groups of 13 but this program found all 5 stable groups of 13 companions. There are no stable groups larger than 13.
 
