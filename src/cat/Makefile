CC = gcc
CFLAGS = -Wall -Werror -Wextra -c


s21_cat: cat.o cat_define.o
	$(CC) cat.o cat_define.o -o s21_cat

cat.o:
	$(CC) $(CFLAGS) cat.c

cat_define.o :
	$(CC) $(CFLAGS) cat_define.c

test: s21_cat 
	bash ../../materials/build/scripts/t_cat.sh

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c
	clang-format -i *.c
	clang-format -n *.c
	clang-format -n *.h
	clang-format -i *.h
	clang-format -n *.h
	rm .clang-format

valgrind: 
	valgrind --tool=memcheck --leak-check=yes ./s21_cat -e text1.txt



clean:
	rm -rf *o
	rm -rf s21_cat
	rm -rf cat.txt
	rm -rf log.txt