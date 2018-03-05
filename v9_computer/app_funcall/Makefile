GCC		= gcc
GXX		= g++
PROG		= funcall
PROG_C		= funcall.c
PROG_S		= funcall.s
TOOL_DIR	= ../tools
#CFLAGS_32 	= -O3 -m32
CFLAGS_32 	= -g -O0 -m32
LIBS		= -lm
#CFLAGS 	= -O3
CFLAGS 	= -g -O0
XCC		= $(TOOL_DIR)/xc
SIM		= $(TOOL_DIR)/xem
DIS		= $(TOOL_DIR)/dis
CHGHEADER	= $(TOOL_DIR)/changeheader

all: $(XCC) $(SIM) $(DIS) $(CHGHEADER) $(PROG)
$(XCC): $(TOOL_DIR)/c.c
	$(GCC) $(CFLAGS_32) -o $(XCC) $(TOOL_DIR)/c.c $(LIBS)
$(SIM): $(TOOL_DIR)/em.c
	$(GCC) $(CFLAGS_32) -o $(SIM) $(TOOL_DIR)/em.c $(LIBS)
$(DIS): $(TOOL_DIR)/c.c $(TOOL_DIR)/em.c $(TOOL_DIR)/dis.c
	$(GCC) $(CFLAGS) -o $(DIS) $(DIS).c $(LIBS)
$(CHGHEADER): $(TOOL_DIR)/changeheader.c
	$(GXX) $(CFLAGS) -o $(CHGHEADER) $(CHGHEADER).c
$(PROG): $(PROG_C)
	$(XCC) -o $(PROG) $(PROG_C) 
	echo "CC USR APP $< Done"
	$(DIS) $(PROG) -o $(PROG_S) -c
	echo "DISAMBLE USR APP $< Done"
	$(CHGHEADER) $(PROG)
	echo "CHGHEADER USR APP $< Done"
clean :
	rm -f $(PROG) $(PROG).s $(XCC) $(SIM) $(DIS) $(CHGHEADER)
	echo "Clean Done"
