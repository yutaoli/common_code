BINARY = $(patsubst %.cpp,%,$(wildcard *.cpp)) $(patsubst %.c, %, $(wildcard *.c))

INC+=-I./
LIB+=-lpthread

GCC=gcc
CXX=g++

all:$(BINARY)

RED = \\e[1m\\e[31m
RESET = \\e[m
GREEN = \\e[1m\\e[32m

clean:
	@rm -rf ${BINARY}

%:%.c
	@echo -e "Make $(GREEN)$@$(RESET) begin......\c"
	$(GCC) $(INC) $< -o $@ $(CFLAGS) -g -Xlinker "-(" ${LIB} -Xlinker "-)" 
	@echo -e $(RED)"ok."$(RESET)

%:%.cpp
	@echo -e "Make $(GREEN)$@$(RESET) begin......\c"
	$(CXX) $(INC) $< -o $@ $(CFLAGS) -g -Xlinker "-(" ${LIB} -Xlinker "-)" 
	@echo -e $(RED)"ok."$(RESET)
