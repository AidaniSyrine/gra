.PHONY: all opt clean debug
all: tonwertkorrektur

# Compiler
CC = gcc
# List of soucrce files
SOURCE = $(wildcard *.c)

# Flags
WFLAGS = -Wall -Wextra -Wpedantic
# Compile with disabled optimisation
tonwertkorrektur: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS)

simd: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 -msse4.1 $(WFLAGS)
# Compile for efficency Testing
opt: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O3 $(WFLAGS)

# Compile with address sanitizer enabled
asan: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=address

# Compile with UB sanitizer enabled
ubsan: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=undefined

# Compile with leak sanitzer enabled
lsan: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fsanitize=leak

# Compile with GCC static Analysis enabled
staticana: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS) -Werror -fanalyzer

# Set the debugging flag
debug: $(SOURCE)
	$(CC) $^ -o $@.out -std=c11 -O0 $(WFLAGS) -g

clean:
	rm -f $(wildcard *.out) *.dSYM ./images/*.pgm