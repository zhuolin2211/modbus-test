#GCC := gcc
ifeq ($(HOST),x86)
GCC := gcc
else
GCC := $(CC)
endif



LD := ld
OBJCOPY := objcopy
OBJDUMP := objdump
LIBS := -lpthread -luci
LIBSPATH := -L/usr/local/lib/
TARGET ?= modbus-test

# 头文件路径
INCDIRS :=	. \
			libmodbus \
			




# 源文件路径
SRCDIRS :=	. \
			libmodbus \
			
# 每个头文件路径加 -I
INCLUDE	:= $(patsubst %, -I %, $(INCDIRS))

# 获取所有源文件
SFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# 获取所有无路径的源文件
SFILENDIR := $(notdir $(SFILES))
CFILENDIR := $(notdir $(CFILES))

# 将源文件转换成.o文件
SOBJS := $(patsubst %, objects/%, $(SFILENDIR:.S=.o))
COBJS := $(patsubst %, objects/%, $(CFILENDIR:.c=.o))
OBJS := $(SOBJS) $(COBJS)

# 自动寻找文件依赖
VPATH := $(SRCDIRS)

.PHONY: clean
.PHONY: print

$(TARGET) : $(OBJS)
	$(GCC) $^  $(LIBSPATH) $(LIBS) -o $@ -g -O1


$(SOBJS) : objects/%.o : %.S
	$(GCC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $< -O1 -g

$(COBJS) : objects/%.o : %.c
	$(GCC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $< -O1 -g

clean:
	rm -rf $(TARGET)  $(OBJS)

print:
	@echo INCLUDE = $(INCLUDE)
	@echo SFILES = $(SFILES)
	@echo CFILES = $(CFILES)
	@echo SFILENDIR = $(SFILENDIR)
	@echo CFILENDIR = $(CFILENDIR)
	@echo OBJS = $(OBJS)
