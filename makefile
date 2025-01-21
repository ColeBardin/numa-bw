CC=cc
CFLAGS += --std=c99
CFLAGS += -pthread
CFLAGS += -lnuma

SRC=src
OBJ=obj
BIN=bin
EXE=numabw

${EXE}: ${SRC}/main.c 
	[ -d $(BIN) ] || mkdir -p $(BIN)
	${CC} ${CFLAGS} -o ${BIN}/${EXE} ${SRC}/main.c 

clean:
	rm -f ${BIN}/${EXE}
	rm -r -f bin
	rm -r -f obj
