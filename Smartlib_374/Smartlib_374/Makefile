CFLAGS=-Wall -ggdb
LDFLAGS=-lpthread -L./lib -L./lib/cjson -lgateway -lcjson -lm

OBJDIR=./obj

#CC:=/mydev/rsdk-1.5.8-4281-EB-2.6.30-0.9.30.3-m32ub-120907/bin/mips-linux-gcc 
CC:=gcc
#CXX:=/mydev/rsdk-1.5.8-4281-EB-2.6.30-0.9.30.3-m32ub-120907/bin/mips-linux-gcc 
CXX:=gcc
.PHONY: all clean

all : GatewayHost

GatewayHost : ${OBJDIR}/HSZSDKTest.o 
	${CC} $^ -o $@ ${LDFLAGS} -I./lib -I./lib/cjson
	
${OBJDIR}/HSZSDKTest.o : HSZSDKTest.c
	${CC} -I./lib -c $^ -o $@ ${LDFLAGS}
	
clean : 
	-rm -f ${OBJDIR}/*.o GatewayHost
