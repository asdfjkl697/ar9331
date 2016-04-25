RM = rm
#CC = linux-gcc
OBJS = client.o hardware.o packet.o rs232.o taskmanager.o tcpserver.o obsolete.o udpserver.o radiopacket.o netpacket.o radionetwork.o radiosynchandler.o main.o
bin = gsbox
CFLAGS += -I.
LIBS += -levent
#LIBS += -lusb-1.0
LIBS += -lpthread
LIBS += -lcrypt
LIBS += -lxml2
LIBS += -lstdc++

#LIBDIR = -L/lib/
all : $(bin)
$(bin) : $(OBJS)
	$(CXX) $(CFLAGS) -Wall  $(OBJS) $(LIBDIR) -o $(bin) $(LIBS)
clean :
	$(RM) -f $(OBJS)
	$(RM) -f $(bin)
