CC = gcc
LD = ld

INCLUDEDIR = /usr/src/linux-2.4/include
CFLAGS = -D__KERNEL__ -DMODULE -O -Wall -I$(INCLUDEDIR) -I/usr/src/linux-2.4 -I. -DCONFIG_HOTPLUG -DPCI_BUS -I./include

ifeq ($(DEBUG),1)
CFLAGS += -DDK_DEBUG
endif

ifeq ($(UART),1)
CFLAGS += -DDK_UART
endif

LDFLAGS	= -r

SRCS	 = main.c dk_pci_bus.c client.c dk_func.c dk_isr.c dk_event.c
OBJS    :=  $(patsubst %.c,%.o,$(SRCS))

BIN = dkkernel.o

all : $(BIN)

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJS) $(BIN)

.phony:
	clean all

