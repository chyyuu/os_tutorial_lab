GCC		= gcc
GXX		= g++
PROG		= ucore_enable_filesystem
USRAPPDIR	= ./usrapps
TOOL_DIR	= ../tools
CFLAGS_32 	= -O3 -m32
LIBS		= -lm
CFLAGS 	= -O3
XCC		= $(TOOL_DIR)/xc
SIM		= $(TOOL_DIR)/xem
DIS		= $(TOOL_DIR)/dis
CHGHEADER	= $(TOOL_DIR)/changeheader
MKFS		= $(TOOL_DIR)/mkfs
DISK		= user.img
USRAPPS	= $(USRAPPDIR)/sh \
                $(USRAPPDIR)/badarg \
                $(USRAPPDIR)/divzero \
                $(USRAPPDIR)/exit \
                $(USRAPPDIR)/faultread \
                $(USRAPPDIR)/faultreadkernel \
                $(USRAPPDIR)/forktree \
                $(USRAPPDIR)/hello \
                $(USRAPPDIR)/matrix \
                $(USRAPPDIR)/pgdir \
                $(USRAPPDIR)/priority \
                $(USRAPPDIR)/spin \
                $(USRAPPDIR)/waitkill \
                $(USRAPPDIR)/yield

all: $(XCC) $(SIM) $(DIS) $(CHGHEADER) $(MKFS) $(PROG) $(DISK)
$(DISK): $(USRAPPS)
	@$(MKFS) user.img ./usrapps
$(XCC): $(TOOL_DIR)/c.c
	@$(GCC) $(CFLAGS_32) -o $(XCC) $(TOOL_DIR)/c.c $(LIBS)
$(SIM): $(TOOL_DIR)/em.c
	@$(GCC) $(CFLAGS_32) -o $(SIM) $(TOOL_DIR)/em.c $(LIBS)
$(DIS): $(TOOL_DIR)/c.c $(TOOL_DIR)/em.c $(TOOL_DIR)/dis.c
	@$(GCC) $(CFLAGS) -o $(DIS) $(DIS).c $(LIBS)
$(CHGHEADER): $(TOOL_DIR)/changeheader.c
	@$(GXX) $(CFLAGS) -o $(CHGHEADER) $(CHGHEADER).c
$(MKFS): $(TOOL_DIR)/$(MKFS).c
	@$(GCC) $(CFLAGS_32) -o $(MKFS) $(MKFS).c
$(PROG): $(PROG).c
	@$(XCC) -o $@ $<
	@$(DIS) $(PROG) -o $(PROG).s -c
	@echo "CC OS $< Done"
$(USRAPPDIR)/sh: $(USRAPPDIR)/sh.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/badarg: $(USRAPPDIR)/badarg.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/divzero: $(USRAPPDIR)/divzero.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/exit: $(USRAPPDIR)/exit.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/faultread: $(USRAPPDIR)/faultread.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/faultreadkernel:$(USRAPPDIR)/faultreadkernel.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/forktree: $(USRAPPDIR)/forktree.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/hello: $(USRAPPDIR)/hello.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/matrix: $(USRAPPDIR)/matrix.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/pgdir: $(USRAPPDIR)/pgdir.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/priority: $(USRAPPDIR)/priority.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/spin: $(USRAPPDIR)/spin.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/waitkill: $(USRAPPDIR)/waitkill.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
$(USRAPPDIR)/yield: $(USRAPPDIR)/yield.c
	@$(XCC) -o $@ $<
	@$(CHGHEADER) $@
	@echo "CC USR APP $< Done"
clean :
	@rm -f $(PROG) $(PROG).s $(USRAPPS) $(DISK) $(XCC) $(SIM) $(DIS) $(CHGHEADER) $(MKFS)
	@echo "Clean Done"
run: all
	@$(SIM) -f $(DISK) $(PROG)
	@echo "Simulation Done"


