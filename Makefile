ROOTDIR = mips64-elf
GCCN64PREFIX = $(ROOTDIR)/bin/mips64-elf-
CHKSUM64PATH = $(ROOTDIR)/bin/chksum64
MKDFSPATH = $(ROOTDIR)/bin/mkdfs
HEADERPATH = $(ROOTDIR)/lib
N64TOOL = $(ROOTDIR)/bin/n64tool
HEADERNAME = header
LINK_FLAGS = -L$(ROOTDIR)/lib -L$(ROOTDIR)/mips64-elf/lib -lmikmod -ldragon -lm -lc -ldragonsys -Tn64ld.x
PROG_NAME = $(APP_NAME)
# insert '-ILibName' for every additional source folder!
CFLAGS = -std=gnu99 -march=vr4300 -mtune=vr4300 -O2 -Wall -I$(ROOTDIR)/include -Ilib -Iponglib
ASFLAGS = -mtune=vr4300 -march=vr4300
CC = $(GCCN64PREFIX)gcc
AS = $(GCCN64PREFIX)as
LD = $(GCCN64PREFIX)ld
OBJCOPY = $(GCCN64PREFIX)objcopy

$(info $$ROOTDIR is [${ROOTDIR}])
$(info $$GCCN64PREFIX is [${GCCN64PREFIX}])
$(info $$CHKSUM64PATH is [${CHKSUM64PATH}])
$(info $$MKDFSPATH is [${MKDFSPATH}])
$(info $$HEADERPATH is [${HEADERPATH}])
$(info $$N64TOOL is [${N64TOOL}])
$(info $$CC is [${CC}])
$(info $$AS is [${AS}])
$(info $$LD is [${LD}])
$(info $$OBJCOPY is [${OBJCOPY}])

$(PROG_NAME).v64: $(PROG_NAME).elf spritemap.dfs
	$(OBJCOPY) $(PROG_NAME).elf $(PROG_NAME).bin -O binary
	rm -f $(PROG_NAME).v64
	$(N64TOOL) -b -l 2M -t "application" -h $(HEADERPATH)/$(HEADERNAME) -o $(PROG_NAME).v64 $(PROG_NAME).bin -s 1M spritemap.dfs
	$(CHKSUM64PATH) $(PROG_NAME).v64

# after first line: all .h and .o
# after second: all .o
# remember to break lines with ' \'!
$(PROG_NAME).elf : $(PROG_NAME).o lib/grafix.o lib/grafix.h lib/sound.h lib/sound.o lib/game.h lib/game.o lib/effects.o lib/effects.h  lib/sound.h  lib/tools.o lib/tools.h
	$(LD) -o $(PROG_NAME).elf $(PROG_NAME).o lib/effects.o lib/sound.o lib/grafix.o lib/game.o lib/tools.o \
$(LINK_FLAGS)
copy: $(PROG_NAME).v64
	cp $(PROG_NAME).v64 ~/public_html/
 
spritemap.dfs:
	$(MKDFSPATH) spritemap.dfs ./filesystem/

all: $(PROG_NAME).v64

clean:
	rm -f *.v64 *.elf *.o *.bin *.dfs


