GCC		= gcc
PROG		= os_user_task_switch
TOOL_DIR	= ../tools
CFLAGS_32 	= -O3 -m32
LIBS		= -lm
CFLAGS 	= -O3
XCC		= $(TOOL_DIR)/xc
SIM		= $(TOOL_DIR)/xem
DIS		= $(TOOL_DIR)/dis

all: $(XCC) $(SIM) $(DIS) $(PROG)
$(XCC): $(TOOL_DIR)/c.c
	@$(GCC) $(CFLAGS_32) -o $(XCC) $(TOOL_DIR)/c.c $(LIBS)
$(SIM): $(TOOL_DIR)/em.c
	@$(GCC) $(CFLAGS_32) -o $(SIM) $(TOOL_DIR)/em.c $(LIBS)
$(DIS): $(TOOL_DIR)/c.c $(TOOL_DIR)/em.c $(TOOL_DIR)/dis.c
	@$(GCC) $(CFLAGS) -o $(DIS) $(DIS).c $(LIBS)
$(PROG): $(PROG).c
	@$(XCC) -o $@ $<
	@$(DIS) $(PROG) -o $(PROG).s -c
	@echo "CC $< Done"
clean :
	@rm -f $(PROG) $(PROG).s $(XCC) $(SIM) $(DIS)
	@echo "Clean Done"
run: all
	@$(SIM) $(PROG)
	@echo "Simulation Done"


