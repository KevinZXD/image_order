############################################################# 
# Makefile for shared library.
# 编译动态链接库
#############################################################
#set your own environment option
CC = g++
CC_FLAG = -I /usr/local/sina_mobile/nginx/luajit/include/luajit-2.1
#make target lib and relevant obj 
PRG = image_order.so
OBJ = image_order.o

#all target
all:$(PRG)

$(PRG):$(OBJ)
	$(CC) image_order.cpp  -fPIC  -shared -o $@  $(CC_FLAG)

clean:
	rm -f *.so *.o