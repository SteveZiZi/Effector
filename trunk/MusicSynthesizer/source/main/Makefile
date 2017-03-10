#define executable file name
EXECUTABLE := dyjc
EXE_OUTDIR := ../../mnt/dyjc/program/
CORE_SO := fnd mproc baseb 
APP_AR := dbcj httx lcd ppp mpcfg share jlcy fhkz

#define shared libs used by the lib,separated by space 
LIBS := stdc++ m $(CORE_SO) $(APP_AR)
LIBPATH := .

#define c++ compiler,such as ppc_8xx-g++,g++
CC=arm-uclibc-gcc
CORE_INCLUDE := -I../../../platform/foundation -I../../../platform/foundation/confuse -I../../../platform/mprocess -I../../../platform/basebusiness -I../../../platform/nwbusiness -I../lcd -I../lcd/sgui -I../httx/rule -I../dsp
HEAD_INCLUDE := $(CORE_INCLUDE) -I. -I.. -I../mpcfg -I../share -I../dbcj  -I../httx/rule -I../dsp -I../httx -I../ppp -I../lcd -I../lcd/sgui -I../jlcy -I../fhkz 
CFLAGS := -Wall -Werror -fPIC -O2 -D ARCH_ARM -D ARCH_ARM7 $(HEAD_INCLUDE)
CPPFLAGS += -MMD
RM-F := rm -f

#define model rules for c and cpp files
%.o:%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
%.o:%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

SOURCE_DIRS :=
SOURCE := $(wildcard *.c) $(wildcard *.cpp) 
SOURCE += $(foreach DIR,$(SOURCE_DIRS),$(wildcard $(DIR)/*.c $(DIR)/*.cpp))

ALLOBJS := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
EXISTDEPS := $(wildcard *.d)
EXISTDEPS += $(foreach DIR,$(SOURCE_DIRS),$(wildcard $(DIR)/*.d))

LIBS_AR := $(addprefix lib,$(addsuffix .a, $(APP_AR)))
LIBS_SO := $(addprefix ../../../platform/lib,$(addsuffix .so, $(CORE_SO)))

.PHONY : everything objs clean veryclean rebuild debug
everything : $(EXECUTABLE)
objs : $(ALLOBJS)
clean :
	$(RM-F) $(ALLOBJS)
	$(RM-F) $(EXISTDEPS)
veryclean : clean
	$(RM-F) $(EXECUTABLE)
rebuild : veryclean everything
debug :
	@echo EXISTDEPS=$(EXISTDEPS)
	@echo ALLOBJS=$(ALLOBJS)

include $(EXISTDEPS)

$(EXECUTABLE) : $(ALLOBJS) $(LIBS_AR)
	cp -f $(LIBS_SO) ./
	$(CC) -L$(LIBPATH)/ -s -o $(EXECUTABLE) $(ALLOBJS) $(addprefix -l,$(LIBS))
	cp -f $(LIBS_SO) $(EXE_OUTDIR)
	cp -f $(EXECUTABLE) $(EXE_OUTDIR)
