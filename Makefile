COMPILER = gcc main.c bikes.c htmlTable.c
OUTPUT_FILES = bikeSharingMON bikeSharingNYC

FLAGS = -pedantic -std=c99 -Wall -fsanitize=address
DEBUG = -g

all: clean mon nyc

debug: COMPILER += $(DEBUG)
debug: all

mon: $(OBJECT_FILES)
	$(COMPILER) -DMON -o bikeSharingMON $(FLAGS)

nyc: $(OBJECT_FILES)
	$(COMPILER) -DNYC -o bikeSharingNYC  $(FLAGS)

clean:
	rm -f $(OUTPUT_FILES) $(OBJECT_FILES)
	@echo "Makefile: Object files and output files deleted."
