PROJECT_NAME=projeto-aed

CC=gcc
FLAGS=-c -W -Wall -ansi -pedantic -Wextra -std=c99 -O3

# .c files
C_SOURCE=$(wildcard ./*.c)
# .h files
H_SOURCE=$(wildcard ./*.h)
# .o files
OBJ=$(subst .c,.o,$(subst source,objects,$(C_SOURCE)))

RM = rm -rf


#
# Compilation and linking
#
all: objFolder $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $(FLAGS) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '

./objects/%.o: ./source/%.c ./source/%.h
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '

./objects/main.o: ./source/main.c $(H_SOURCE)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(FLAGS) -o $@
	@ echo ' '

objFolder:
	@ mkdir -p objects

clean:
	@ $(RM) ./objects/*.o $(PROJECT_NAME) *~
	@ rmdir objects

.PHONY: all clean
