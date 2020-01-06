DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard ${DIR_SRC}/*.cpp)
TAR = $(patsubst %.cpp, %.o, $(notdir ${SRC}))
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

COMPILE = g++ -c -I $(DIR_INC) -I $(DIR_SRC)
MAKEEXE = g++ -g
EXE = main

ALL: START ${TAR}
	$(MAKEEXE) ${OBJ} -o $(EXE) 
	@echo "=============编译结束============="

START:
	@echo "=============编译开始============="

#注意%是在vpath路径下搜索，当然%.o是在变量中搜索
%.o: $(DIR_SRC)/%.cpp	
	$(COMPILE) $< -o $(DIR_OBJ)/$@

ECHO:
	@echo "SRC: "$(SRC)
	@echo "TAR: "$(TAR)
	@echo "OBJ: "$(OBJ)
	
.PHONY:clean
clean:
	rm -rf $(DIR_OBJ)/*.o $(EXE)
