GCC		= gcc
GXX		= g++
PROG		= ucore_enable_usrapp
USRAPP		= usrapp_exit
TOOL_DIR	= ../tools
CFLAGS_32 	= -O3 -m32
LIBS		= -lm
CFLAGS 	= -O3
XCC		= $(TOOL_DIR)/xc
SIM		= $(TOOL_DIR)/xem
DIS		= $(TOOL_DIR)/dis
CHGHEADER	= $(TOOL_DIR)/changeheader

all: $(XCC) $(SIM) $(DIS) $(CHGHEADER) $(PROG) $(USRAPP)
$(XCC): $(TOOL_DIR)/c.c
	@$(GCC) $(CFLAGS_32) -o $(XCC) $(TOOL_DIR)/c.c $(LIBS)
$(SIM): $(TOOL_DIR)/em.c
	@$(GCC) $(CFLAGS_32) -o $(SIM) $(TOOL_DIR)/em.c $(LIBS)
$(DIS): $(TOOL_DIR)/c.c $(TOOL_DIR)/em.c $(TOOL_DIR)/dis.c
	@$(GCC) $(CFLAGS) -o $(DIS) $(DIS).c $(LIBS)
$(CHGHEADER): $(TOOL_DIR)/changeheader.c
	@$(GXX) $(CFLAGS) -o $(CHGHEADER) $(CHGHEADER).c
$(PROG): $(PROG).c
	@$(XCC) -o $@ $<
	@$(DIS) $(PROG) -o $(PROG).s -c
	@echo "CC OS $< Done"
$(USRAPP): $(USRAPP).c
	@$(XCC) -o $@ $<
	@$(DIS) $(USRAPP) -o $(USRAPP).s -c
	@$(CHGHEADER) $(USRAPP)
	@echo "CC USR APP $< Done"
clean :
	@rm -f $(PROG) $(PROG).s $(USRAPP) $(USRAPP).s $(XCC) $(SIM) $(DIS) $(CHGHEADER)
	@echo "Clean Done"
run: all
	@$(SIM) -f $(USRAPP) $(PROG)
	@echo "Simulation Done"


