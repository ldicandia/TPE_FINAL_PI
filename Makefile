COMPILER = gcc main.c bikes.c htmlTable.c
OUTPUT_FILES = bikeSharingMON bikeSharingNYC
OBJECT_FILES = main.o bikes.o htmlTable.o

FLAGS = -pedantic -std=c99 -Wall -fsanitize=address
DEBUG = -g

all: clean mon nyc

debug: COMPILER += $(DEBUG)
debug: all

mon: $(OBJECT_FILES)
	$(COMPILER) -DMON -o bikeSharingMON $(FLAGS)

nyc: $(OBJECT_FILES)
	$(COMPILER) -DNYC -o bikeSharingNYC  $(FLAGS)

main.o: main.c
    $(COMPILER) $(FLAGS) -c -o main.o main.c

bikes.o: bikes.c
    $(COMPILER) $(FLAGS) -c -o bikes.o bikes.c

htmlTable.o: htmlTable.c
    $(COMPILER) $(FLAGS) -c -o htmlTable.o htmlTable.c

clean:
	rm -f $(OUTPUT_FILES) $(OBJECT_FILES)
	@echo "Makefile: Object files and output files deleted."
