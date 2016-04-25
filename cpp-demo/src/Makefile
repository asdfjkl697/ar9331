RM = rm
#CC = linux-gcc
OBJS = main.o
bin = cpp-demo
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
