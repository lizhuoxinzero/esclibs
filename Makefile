

CC=gcc -g

OUTPUT=test_exe

#小工具代码
LITTLE_TOOL:=/home/pi/work/c/littletool

#包含头文件目录
INCLUDE+=-I$(LITTLE_TOOL) #

#链接库
#LIBS+=-levent #
#LIBS+=-lm #
LIBS+=-lcrypto #

#目标文件
OBJS+=testmain.o #
OBJS+=listfile.o #
OBJS+=buffer.o #
OBJS+=listqueue.o #
OBJS+=sha1.o #

all: $(OBJS)
	$(CC) $(LIBS) $(INCLUDE) $^ -o $(OUTPUT)
	rm -rf *.o


listfile.o: listfile.c
	$(CC)  $(INCLUDE) $^ -c
testmain.o: testmain.c
	$(CC)  $(INCLUDE) $^ -c
testmain.o: testmain.c
	$(CC)  $(INCLUDE) $^ -c
buffer.o: container/buffer.c
	$(CC)  $(INCLUDE) $^ -c
listqueue.o: container/queue/listqueue.c
	$(CC)  $(INCLUDE) $^ -c
sha1.o: hash/sha1/sha1.c
	$(CC)  $(INCLUDE) $^ -c

clean:
	rm -rf $(OUTPUT)
	rm -rf *.o
