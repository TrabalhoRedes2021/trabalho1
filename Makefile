CC = gcc
LDFLAGS = -lrt 
BLDDIR = .
INCDIR = $(BLDDIR)/inc
SRCDIR = $(BLDDIR)/src
OBJDIR = $(BLDDIR)/obj
CFLAGS = -c -Wall -I$(INCDIR)
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
EXE = bin/main

all: clean $(EXE) 
    
$(EXE): $(OBJ) 
	$(CC)  $(OBJDIR)/*.o $(LDFLAGS) -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(OBJDIR)/*.o $(EXE)

run:
	chmod +x $(EXE)
	$(EXE) $(IP_SERVER) $(PORT_SERVER) $(PORT_SERVER2) $(IP_CLIENT) $(PORT_CLIENT)
