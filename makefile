
all : oss user_proc
oss : oss.o
	gcc -pthread -g -o oss oss.o
oss.o : oss.c
	gcc -c -g oss.c
user_proc : user_proc.o
	gcc -pthread -g -o user_proc user_proc.o
user_proc.o : user_proc.c
	gcc -c -g user_proc.c
clean :
	rm oss.o user_proc.o msglog.out oss user_proc
