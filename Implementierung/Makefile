#Resource: https://www.gnu.org/software/make/manual/make.html
#Idea: Inspired by CMake-Build System

.PHONY: all clean perf
all = tonwertkorrektur

# Compiler 
CC = gcc 

# Flags
WFLAGS = -Wall -Wextra -Wpedantic 
CFLAGS = -std=c17 -msse4.2 
LIBS = -lm
    # For make perf - Used to check the Performance
	ifeq ($(filter perf, $(MAKECMDGOALS)), perf)
	CFLAGS += -O3
	else
	CFLAGS += -O0
	endif

# Build Directory 
OUTDIR = ./build
OBJDIR = ./build/objects
RESDIR = ./resources/result

# Header and Source files
SUBDIR = src src/core_logic src/io_utilities src/tests
SOURCES = $(wildcard *.c $(foreach path, $(SUBDIR), $(path)/*.c))
HEADERS = $(wildcard *.h $(foreach path, $(SUBDIR), $(path)/*.h))

# Objects and include/header directories
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:c=out))
INCDIRS = -I./ $(addprefix -I, $(SUBDIR))

# Compile source Files 
$(OBJDIR)/%.out: %.c $(HEADERS)
	mkdir -p -v $(@D)
	$(CC) -o $@ $(WFLAGS) $(CFLAGS) -c $< $(INCDIRS) 

# Entry point	
tonwertkorrektur perf: $(OBJECTS)
	mkdir -p -v $(RESDIR) 
	chmod 000 ./resources/testing/top_secret.ppm
	cd $(OUTDIR) 
	$(CC) -o $(OUTDIR)/$(all).out $(WFLAGS) $(CFLAGS) $(OBJECTS) $(LIBS)

# Cleanup 
clean: 	
	rm -rf $(OUTDIR) $(RESDIR) *.data *.old *.dSYM
	find . -type f -name "*.pgm" -delete
	chmod 700 ./resources/testing/top_secret.ppm


