# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Executables
OSS_EXEC = oss
USER_PROC_EXEC = user_proc

# Default target
all: $(OSS_EXEC) $(USER_PROC_EXEC)

# Build oss executable
$(OSS_EXEC): oss.o
	$(CC) $(CFLAGS) $^ -o $@

# Build user_proc executable
$(USER_PROC_EXEC): user_proc.o
	$(CC) $(CFLAGS) $^ -o $@

# Compile oss.c
oss.o: oss.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile user_proc.c
user_proc.o: user_proc.c header.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f oss.o user_proc.o $(OSS_EXEC) $(USER_PROC_EXEC)
