CFLAGS=-Wall -ggdb
LDFLAGS=-L./cjson -lcjson -pthread

OBJDIR=./obj
LIB_OBJECT=libgateway.a

#CC:=/mydev/rsdk-1.5.8-4281-EB-2.6.30-0.9.30.3-m32ub-120907/bin/mips-linux-gcc 
CC:=gcc
#CXX:=/mydev/rsdk-1.5.8-4281-EB-2.6.30-0.9.30.3-m32ub-120907/bin/mips-linux-gcc 
CXX:=gcc
#AR:=/mydev/rsdk-1.5.8-4281-EB-2.6.30-0.9.30.3-m32ub-120907/bin/mips-linux-ar
AR:=ar
.PHONY: all clean

all : $(LIB_OBJECT)
	
$(LIB_OBJECT) :	${OBJDIR}/HSZCommon.o ${OBJDIR}/HSZInterface.o ${OBJDIR}/HSZMessage.o ${OBJDIR}/HSZDeviceData.o   ${OBJDIR}/HSZBindDevice.o ${OBJDIR}/HSZHandleZclData.o ${OBJDIR}/ha_client.o ${OBJDIR}/ringbuffer.o ${OBJDIR}/HSZSmartLockMessage.o ${OBJDIR}/HSZSelfProtocolLock.o ${OBJDIR}/HSZSelfProtocolSL500Lock.o
	$(AR) crs $@ $^	
    	
${OBJDIR}/HSZCommon.o : HSZCommon.c
	${CXX} -I./cjson   -c $^ -o $@ ${CFLAGS} -lm -lpthread 
	
${OBJDIR}/HSZInterface.o : HSZInterface.c
	${CXX} -I./cjson   -c $^ -o $@ ${CFLAGS}
	
${OBJDIR}/HSZMessage.o : HSZMessage.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}
	
${OBJDIR}/HSZDeviceData.o : HSZDeviceData.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}	
	
${OBJDIR}/HSZBindDevice.o :HSZBindDevice.c
	${CXX} -I./cjson   -c $^ -o $@ ${CFLAGS}
	
${OBJDIR}/HSZHandleZclData.o :HSZHandleZclData.c
	${CXX} -I./cjson   -c $^ -o $@ ${CFLAGS}
	
${OBJDIR}/ha_client.o : ha_client.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}	

${OBJDIR}/ringbuffer.o : ringbuffer.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}	
	
${OBJDIR}/HSZSmartLockMessage.o : HSZSmartLockMessage.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}	
	
${OBJDIR}/HSZSelfProtocolLock.o : HSZSelfProtocolLock.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}

${OBJDIR}/HSZSelfProtocolSL500Lock.o : HSZSelfProtocolSL500Lock.c
	${CXX} -I./cjson -c $^ -o $@ ${CFLAGS}		
	
clean : 
	-rm -f ${OBJDIR}/*.o GatewayHost libgateway.a
