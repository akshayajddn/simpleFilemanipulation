Problem:- Given a large file, reverse the file word-wise.
input:-
this is line one
this is line two
this is line three
this is line four

Output:-
four line is this
three line is this
two line is this
one line is this

Solution:- explained in main's description

Deliverables:-
Mandatory:-
1. revFileWordWise.c :- file which contains the main logic.
2. input.txt:- input file
3. output.txt:- output.txt(formed after running the code).
4. 0.txt, 1.txt, 2.txt:- scratch files created by the binary.

Optional:-
1. fileops_ineff.c:- file contains an alternative logic where
   we read the file backwards. This way we dont need to reverse
   2 times, but it involves heavy usage of fseek, which makes it
   highly inefficient.

How to run:-
1. Download the repo.
2. Delete 0.txt,1.txt, 2.txt and output.txt
3. Only input.txt should be remaining.

4. gcc -Wall revFileWordWise.c -o revFileWordWise
5. ./revFileWordWise
6. check output.txt
