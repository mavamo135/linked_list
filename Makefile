CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION = out

.PHONY: clean test project

PATH_SRC = src/
PATH_BLD = build/
PATH_DEP = build/depends/
PATH_OBJ = build/objs/

BUILD_PATHS = $(PATH_BLD) $(PATH_DEP) $(PATH_OBJ)

SRC= $(wildcard $(PATH_SRC)*c)
OBJ= $(patsubst $(PATH_SRC)%.c,$(PATH_OBJ)%.o,$(SRC))

COMPILE = gcc -c
LINK = gcc
DEPEND = gcc -MM -MG -MF
CFLAGS = -I. -I$(PATH_SRC) -ansi -Wall -std=c99 -O0 -ggdb
CLIBS = -lpthread

PROJECT = $(PATH_BLD)project.$(TARGET_EXTENSION)

all: project

project: $(BUILD_PATHS) $(PROJECT)

$(PROJECT): $(OBJ) #$(PATH_DEP)%.d
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Linker'
	$(LINK) -o $@ $^ $(CLIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(PATH_OBJ)%.o: $(PATH_SRC)%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Compiler'
	$(COMPILE) $(CFLAGS) -MMD -MP -MF \
		$(patsubst $(PATH_OBJ)%.o,$(PATH_DEP)%.d,$(@)) -MT $@ -o $@ $<
	@echo 'Finished building: $<'
	@echo ' '

$(BUILD_PATHS):
	$(MKDIR) $(PATH_BLD)
	$(MKDIR) $(PATH_DEP)
	$(MKDIR) $(PATH_OBJ)

clean:
	$(CLEANUP) $(PATH_OBJ)*.o
	$(CLEANUP) $(PATH_DEP)*.d
	$(CLEANUP) $(PATH_BLD)*.$(TARGET_EXTENSION)

run: project
	./build/project.out

.PRECIOUS: $(PATH_DEP)%.d
.PRECIOUS: $(PATH_OBJ)%.o
