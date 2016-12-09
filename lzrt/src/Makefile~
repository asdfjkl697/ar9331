RM = rm
#CC = linux-gcc
OBJS = client.o hardware.o obsolete.o packet.o rs232.o taskmanager.o tcpserver.o udpbroadcast.o main.o
bin = lzrt
CFLAGS += -I.
LIBS += -levent
#LIBS += -lusb-1.0
LIBS += -lpthread
LIBS += -lcrypt

#LIBDIR = -L/lib/
all : $(bin)
$(bin) : $(OBJS)
	$(CC) $(CFLAGS) -Wall  $(OBJS) $(LIBDIR) -o $(bin) $(LIBS)
clean :
	$(RM) -f $(OBJS)
	$(RM) -f $(bin)
