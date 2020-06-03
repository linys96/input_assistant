INC_DIR:= ../include
SRC_DIR:= ./
SRCS:=$(wildcard *.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -ljsoncpp

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I, $(INC_DIR)) $(LIBS) 

EXE:=./client.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
