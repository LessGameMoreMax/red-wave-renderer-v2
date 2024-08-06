# 指定main函数文件名
CPP_S = Memory/TCBenchMarkWindowsTest.cpp

# 指定执行文件名
Out = main.out

# 增加编译模块
Modules = Runtime/Core \
		  Runtime/WorkQueue \

# 增加头文件搜索路径
IncludeDir = $(SourceDir)/Runtime/ \

# 编译选项
CFLAGS = -std=c++20 \
		 -Wall \
		 $(IncludePath) \

# 依赖生成选项
DFLAGS = -MM \
		 $(IncludePath) \





CppSuffix = .cpp
ObjSuffix = .o
HeaderSuffix = .h
DependSuffix = .d

DEBUG = debug
CLEAN = clean

ifeq ($(DEBUG), $(MAKECMDGOALS))
CFLAGS += -g
endif

INCLUDE = iquote

TestDir = $(shell pwd)
RootDir = $(TestDir)/..
SourceDir = $(RootDir)/Source
BuildDir = Build

ifneq ($(wildcard $(BuildDir)), $(BuildDir))
$(shell mkdir $(BuildDir))
endif

define to_obj
$(BuildDir)/$(basename $(notdir $(1)))$(ObjSuffix)
endef

define to_cpp
$(findstring %(basename $(notdir $(1))$(CppSuffix)), $(CPP_S))
endef

IncludePath = $(foreach Temp, $(IncludeDir), -$(INCLUDE)$(Temp))

Rwildcard = $(foreach DorF, $(wildcard $(1)*), $(call Rwildcard,$(DorF)/,$(2))$(filter $(2), $(DorF)))
CPP_S += $(foreach Module, $(Modules), $(call Rwildcard, $(SourceDir)/$(Module), %$(CppSuffix)))
OBJ_S = $(foreach Cpp, $(CPP_S), $(call to_obj, $(Cpp)))
CPP_DIR = $(sort $(foreach Cpp, $(CPP_S), $(dir $(Cpp))))
DEP_S = $(OBJ_S:$(ObjSuffix)=$(DependSuffix))

vpath %$(CppSuffix) $(CPP_DIR)
vpath %$(HeaderSuffix) $(CPP_DIR)

all: $(OBJ_S)
	g++ $(OBJ_S) -o $(Out)

$(BuildDir)/%$(ObjSuffix): %$(CppSuffix)
	g++ $(CFLAGS) -c $< -o $@

$(BuildDir)/%$(DependSuffix): %$(CppSuffix)
	@g++ $(DFLAGS) $< > $@ && sed -i "1s#^#$(BuildDir)/#" $@

ifneq ($(CLEAN), $(MAKECMDGOALS))
-include $(DEP_S)
endif

.PHONY : $(CLEAN) $(DEBUG)
clean:
	-rm -rf $(Out) $(BuildDir)

$(DEBUG): all
