# Aurel Strigáč <xstrig00>

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -O2 -std=c++17
LDFLAGS = -lpcap -lncurses

SRCDIR = src
OBJDIR = obj
INCDIR = include
CXXFLAGS += -I$(INCDIR)

TARGET = isa-top
OBJECTS = $(OBJDIR)/isa-top.o $(OBJDIR)/utils.o $(OBJDIR)/flow.o $(OBJDIR)/capture.o $(OBJDIR)/display.o


all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking to create $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete!"

$(OBJDIR)/isa-top.o: $(SRCDIR)/isa-top.cpp $(INCDIR)/isa-top.h $(INCDIR)/utils.h $(INCDIR)/flow.h $(INCDIR)/capture.h $(INCDIR)/display.h
	@echo "Compiling $(SRCDIR)/isa-top.cpp..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/isa-top.cpp -o $(OBJDIR)/isa-top.o

$(OBJDIR)/utils.o: $(SRCDIR)/utils.cpp $(INCDIR)/utils.h
	@echo "Compiling $(SRCDIR)/utils.cpp..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/utils.cpp -o $(OBJDIR)/utils.o

$(OBJDIR)/flow.o: $(SRCDIR)/flow.cpp $(INCDIR)/flow.h
	@echo "Compiling $(SRCDIR)/flow.cpp..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/flow.cpp -o $(OBJDIR)/flow.o

$(OBJDIR)/capture.o: $(SRCDIR)/capture.cpp $(INCDIR)/capture.h
	@echo "Compiling $(SRCDIR)/capture.cpp..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/capture.cpp -o $(OBJDIR)/capture.o

$(OBJDIR)/display.o: $(SRCDIR)/display.cpp $(INCDIR)/display.h
	@echo "Compiling $(SRCDIR)/display.cpp..."
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/display.cpp -o $(OBJDIR)/display.o

clean:
	@echo "Cleaning up build files..."
	rm -f $(TARGET)
	rm -rf $(OBJDIR)

tar:
	tar -cf xstrig00.tar $(SRCDIR)/*.cpp $(INCDIR)/*.h Makefile manual.pdf isa-top.1

.PHONY: all clean tar
