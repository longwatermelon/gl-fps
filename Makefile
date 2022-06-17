CXX=g++
CXXFLAGS=-std=c++17 -ggdb -Wall -Werror
INC=-Ideps/include
LIBS=-Ldeps/lib -lglfw -lm -lglad -lstb_image -lassimp

SRC=$(wildcard src/*.cpp)
OBJS=$(addprefix obj/, $(SRC:.cpp=.o))

all:
	mkdir -p obj/src
	$(MAKE) target

target: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $^ $(LIBS)

obj/src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@ $(LIBS)

clean:
	-rm -rf obj/ a.out

