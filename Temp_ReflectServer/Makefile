DIR_INC = ./include
DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

#检查文件夹是否存在
$(shell if [ ! -d $(DIR_OBJ) ]; then mkdir $(DIR_OBJ); fi;)
$(shell if [ ! -d $(DIR_BIN) ]; then mkdir $(DIR_BIN); fi;)

SRC = $(wildcard ${DIR_SRC}/*.cpp)
TAR = $(patsubst %.cpp, %.o, $(notdir ${SRC}))
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir ${SRC}))

COMPILE = g++ -c -I $(DIR_INC) -I $(DIR_SRC)
MAKEEXE = g++ -g

#可执行文件路径
SERVER_NAME = $(DIR_BIN)/server
CLIENT_NAME = $(DIR_BIN)/client

ALL: START ${TAR} SERVER CLIENT 
	@echo "=============编译结束============="

START:
	@echo "=============编译开始============="

SERVER:
	$(MAKEEXE) $(filter-out ${DIR_OBJ}/client.o,$(OBJ)) -o $(SERVER_NAME)

CLIENT:
	$(MAKEEXE) $(filter-out ${DIR_OBJ}/server.o,$(OBJ)) -o $(CLIENT_NAME)

#注意%是在vpath路径下搜索，当然%.o是在变量中搜索
%.o: $(DIR_SRC)/%.cpp	
	$(COMPILE) $< -o $(DIR_OBJ)/$@

ECHO:
	@echo "SRC: "$(SRC)
	@echo "TAR: "$(TAR)
	@echo "OBJ: "$(OBJ)
	
.PHONY:clean
clean:
	rm -rf $(DIR_OBJ) $(DIR_BIN)
