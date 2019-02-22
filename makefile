APP=mid_report.app
ELF=mid_report.elf
cross_tool=
cc = $(cross_tool)gcc
ld = $(cross_tool)ld
ldflags= 

dir_root=$(shell pwd)
sub_dir =$(shell ls -l | grep ^d |awk '{ if( $$9 != "doc" && $$9 != "include" )  print $$9}' )
inc_dir =$(dir_root)/include
#依赖的子目录目标集合
sub_depend:= $(foreach N,$(sub_dir),$(N)/$(N))

INCLUDE=-I $(inc_dir)

cflags=	$(INCLUDE) 
		

export cc ld  cflags ldflags link_gcc_lib
$(APP):$(sub_depend)
	echo "$(sub_depend)"
	$(cc)  $(sub_depend) $(ldflags)  -o $(APP)  -lpthread


$(sub_depend):
	$(foreach N, $(sub_dir),make -C $(N);)
	
clean:
	rm -f  $(app) $(ELF)
	$(foreach N, $(sub_dir),make clean -C $(N);)
