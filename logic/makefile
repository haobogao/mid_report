#这是一个底层目录的makefile 

#目标
DEST_OBJ=logic


#针对C文件 对应的 .o
CUR_SRC_C = ${wildcard *.c}
CUR_OBJ_C = $(patsubst %.c,%.o,$(CUR_SRC_C))
#针对汇编文件 对应的.o
CUR_SRC_S = ${wildcard *.s}
CUR_OBJ_S = $(patsubst %.s,%.o,$(CUR_SRC_S))
#当前目录下的所有.o
CUROBJ := $(CUR_OBJ_S) $(CUR_OBJ_C)



$(DEST_OBJ):$(CUROBJ) 
	$(ld) $(ldflags) -r -o $(DEST_OBJ)  $(CUROBJ)

	
#依赖生成方式	
$(CUR_OBJ_C): %.o:%.c
	$(cc) -c $(cflags) $^ -o $@
#依赖生成方式
$(CUR_OBJ_S): %.o:%.s
	$(cc) -c $(cflags) $^ -o $@	

	
clean:
	rm -f $(CUROBJ) $(DEST_OBJ)