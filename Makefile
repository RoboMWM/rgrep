# Your program must compile with 'make'
# You must not change this file.

CC = gcc
FLAGS = -std=c99 -O0 -Werror -g -pedantic 

rgrep:
	$(CC) $(FLAGS) rgrep.c -o rgrep

clean:
	rm -rf rgrep *.dSYM

check: clean rgrep
	test "`echo "a\nb\nc" | ./rgrep 'a'`" = "a"
	test "`echo "a\n" | ./rgrep 'a'`" = "a"
	test "`echo "a" | ./rgrep '...'`" = ""
	test "`echo "abc" | ./rgrep '.b.'`" = "abc"
	test "`echo "h\naaaaah" | ./rgrep 'a+h'`" = "aaaaah"
	test "`echo "h\naaaaahhhhh" | ./rgrep 'aa+hh+'`" = "aaaaahhhhh"
	test "`echo "h\naaaaahhhhh\n" | ./rgrep 'aa+hh+'`" = "aaaaahhhhh"
	test "`echo "a" | ./rgrep 'a?a'`" = "a"
	test "`echo "woot\nwot\nwat\n" | ./rgrep 'wo?t'`" = "wot"
	test "`echo "CCCCCCC\nC++\nC+" | ./rgrep '.\+\+'`" = "C++"
	test "`echo "GG" | ./rgrep '.+'`" = "GG"
	test "`echo "woooooo_CSE31.jpg" | ./rgrep 'w.+_...31\.jpg'`" = "woooooo_CSE31.jpg"
	test "`echo "aab" | ./rgrep 'bb?'`" = "aab"
	test "`echo "aaab" | ./rgrep 'a+b'`" = "aaab"
	test "`echo "aaab" | ./rgrep 'a+ab'`" = "aaab"
	test "`echo "bah" | ./rgrep 'ba?ah'`" = "bah"
	test "`echo "baah" | ./rgrep 'ba?ah'`" = "baah"
	test "`echo "baah\nbah" | ./rgrep 'ba+ah'`" = "baah"
	test "`echo "baaaah" | ./rgrep 'ba+ah'`" = "baaaah"
	test "`echo "baah\nbaaaah" | ./rgrep 'ba+aah'`" = "baaaah"
	@echo "Passed sanity check."

