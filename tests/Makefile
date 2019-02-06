CLEANUP = rm -f
MKDIR = mkdir -p
TARGET_EXTENSION = out

.PHONY: clean test

PATH_UNITY = Unity/src/
PATH_SRC = ../src/
PATH_TEST = ./
PATH_BLD = build/
PATH_DEP = build/depends/
PATH_OBJ = build/objs/
PATH_RES = build/results/

BUILD_PATHS = $(PATH_BLD) $(PATH_DEP) $(PATH_OBJ) $(PATH_RES)

SRC_TEST = $(wildcard $(PATH_TEST)*.c)

COMPILE = gcc -c
LINK = gcc
DEPEND = gcc -MM -MG -MF
CFLAGS = -I. -I$(PATH_UNITY) -I$(PATH_SRC) -DTEST
CLIBS = -lpthread -lrt

RESULTS = $(patsubst $(PATH_TEST)Test%.c,$(PATH_RES)Test%.txt,$(SRC_TEST) )

PASSED = `grep -s PASS $(PATH_RES)*.txt`
FAIL = `grep -s FAIL $(PATH_RES)*.txt`
IGNORE = `grep -s IGNORE $(PATH_RES)*.txt`

test: $(BUILD_PATHS) $(RESULTS)
	@echo "-----------------------\nIGNORES:\n-----------------------"
	@echo "$(IGNORE)"
	@echo "-----------------------\nFAILURES:\n-----------------------"
	@echo "$(FAIL)"
	@echo "-----------------------\nPASSED:\n-----------------------"
	@echo "$(PASSED)"
	@echo "\nDONE"

$(PATH_RES)%.txt: $(PATH_BLD)%.$(TARGET_EXTENSION)
	-./$< > $@ 2>&1
	@echo ' '

$(PATH_BLD)Test%.$(TARGET_EXTENSION): $(PATH_OBJ)Test%.o $(PATH_OBJ)%.o \
									  $(PATH_UNITY)unity.o #$(PATH_DEP)Test%.d
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Linker'
	$(LINK) -o $@ $^ $(CLIBS)
	@echo 'Finished building target: $@'
	@echo ' '

$(PATH_OBJ)%.o:: $(PATH_TEST)%.c
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Compiler'
	$(COMPILE) $(CFLAGS) $< -o $@
	@echo 'Finished building target: $@'
	@echo ' '

$(PATH_OBJ)%.o:: $(PATH_SRC)%.c
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Compiler'
	$(COMPILE) $(CFLAGS) $< -o $@
	@echo 'Finished building target: $@'
	@echo ' '

$(PATH_OBJ)%.o:: $(PATH_UNITY)%.c $(PATH_UNITY)%.h
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Compiler'
	$(COMPILE) $(CFLAGS) $< -o $@
	@echo 'Finished building target: $@'
	@echo ' '

$(PATH_DEP)%.d:: $(PATH_TEST)%.c
	$(DEPEND) $@ $<

$(BUILD_PATHS):
	$(MKDIR) $(PATH_BLD)
	$(MKDIR) $(PATH_DEP)
	$(MKDIR) $(PATH_OBJ)
	$(MKDIR) $(PATH_RES)

clean:
	$(CLEANUP) $(PATH_OBJ)*.o
	$(CLEANUP) $(PATH_DEP)*.d
	$(CLEANUP) $(PATH_BLD)*.$(TARGET_EXTENSION)
	$(CLEANUP) $(PATH_RES)*.txt

.PRECIOUS: $(PATH_BLD)Test%.$(TARGET_EXTENSION)
.PRECIOUS: $(PATH_DEP)%.d
.PRECIOUS: $(PATH_OBJ)%.o
.PRECIOUS: $(PATH_RES)%.txt
