TARGET:=main
OBJECT:=main.o threadpool.o task.o sysutils.o

CXX:=g++
CXXFLAGS:=-Wall -fsanitize=address -g -std=c++11
LIBS:=-lpthread

all:$(TARGET)
$(TARGET):$(OBJECT)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)
%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
.PHONEY:all

clean:
	rm *.o $(TARGET)
.PHONEY:clean