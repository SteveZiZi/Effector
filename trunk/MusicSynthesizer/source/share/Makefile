#define executable file name
EXECUTABLE := libshare.a

#define c++ compiler,such as ppc_8xx-g++,g++
CC=arm-uclibc-gcc
CORE_INCLUDE := -I../../../platform/foundation -I../../../platform/foundation/confuse -I../../../platform/mprocess -I../../../platform/basebusiness 
HEAD_INCLUDE := $(CORE_INCLUDE) -I. -I.. -I../mpcfg 
CFLAGS := -Wall -Werror -O2 -D ARCH_ARM -D ARCH_ARM7 $(HEAD_INCLUDE)
CPPFLAGS += -MMD
RM-F := rm -f

#define ar
AR=ar
ARFLAGS := -rcs

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

$(EXECUTABLE) : $(ALLOBJS)
	$(AR) $(ARFLAGS) $(EXECUTABLE) $(ALLOBJS)
	cp -f $(EXECUTABLE) ../main/
	
