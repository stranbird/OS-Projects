osh: main.o do_external.o do_cd.o do_ls.o do_pwd.o
	cc -o osh main.o do_external.o do_cd.o do_ls.o do_pwd.o

main.o: main.c includes/glo.h
	cc -c main.c

do_external.o: do_external.c
	cc -c do_external.c

do_cd.o: builtins/do_cd.c
	cc -c builtins/do_cd.c

do_ls.o: builtins/do_ls.c
	cc -c builtins/do_ls.c

do_pwd.o: builtins/do_pwd.c
	cc -c builtins/do_pwd.c