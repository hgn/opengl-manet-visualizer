NS3_HOME=../../ns-3-dev
NS3_BUILD=${NS3_HOME}/build/debug

INC_DIR=${NS3_BUILD}/
LIB_DIR=${NS3_BUILD}/
LIBS=-lns3
CFLAGS=-Wall -W 


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${NS3_BUILD}

TARGET      = wifi-ap
TARGET_SRC  = $(addsuffix .cc,$(TARGET))

$(TARGET): $(TARGET_SRC)
	$(CC) $(CFLAGS) -o $@ $+ -I${INC_DIR} -L${LIB_DIR} ${LIBS}

run: $(TARGET)
	./$(TARGET)


clean:
	-rm -rf $(TARGET) *~ core
