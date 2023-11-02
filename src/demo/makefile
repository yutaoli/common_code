include $(COMMON_CODE_DIR)/extern/makefile.include

BINARY = $(patsubst %.cpp,%,$(wildcard *.cpp))

INC+=
LIB+= -lrt -lpthread

CFLAGS+= -std=c++11

all:$(BINARY)

RED = \\e[1m\\e[31m
RESET = \\e[m
GREEN = \\e[1m\\e[32m

clean:
	@rm -rf ${BINARY}

%:%.cpp
	@echo -e "Make $(GREEN)$@$(RESET) begin......\c"
	$(CXX) $(INC) $< -o $@ $(CFLAGS) -g -Xlinker "-(" ${LIB} -Xlinker "-)" 
	@echo -e $(RED)"ok."$(RESET)
