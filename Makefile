CC = g++
DEBUG = -g -std=c++17
CFLAGS = -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)
SDIR = ./src
ODIR = ./obj
SRCS = $(wildcard $(SDIR)/*.cpp)
OBJS = $(patsubst $(SDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))
INCLUDE = -IC:\LIBRARYES -I./include
LIBS = -LC:\LIBRARYES\GLFW -lglfw3


LINK_TARGET = First_Window.exe

# all : $(LINK_TARGET)

$(LINK_TARGET) : $(OBJS)	
	$(CC) $(LFLAGS) $(LIBS) -o $@ $^
    
$(ODIR)/%.o : $(SDIR)/%.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -MMD -c -o $@ $<

-include $(OBJS:.o=.d)