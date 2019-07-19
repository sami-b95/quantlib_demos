CPPFLAGS=--std=c++11
CPP=g++
LD=g++
LDFLAGS=-lQuantLib -lboost_program_options
OBJS=asian.o
BIN=asian

ALL: $(BIN)

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(BIN)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(BIN) $(OBJS)