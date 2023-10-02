OBJ_S =	TCBenchMarkTest.o \
		../MallocAnsi.o \
		../MallocBase.o \
		../MallocTC.o \
		../MemoryBase.o \
		../MemoryManager.o \
		../../GenericPlatform/GenericPlatformMemory.o \
		../../Linux/LinuxPlatformMemory.o \
		../TCSizeMap.o \
		../TCSizeTools.o \
		../TCGlobals.o \
		../TCThreadCache.o \
		../TCCentralCache.o \
		../TCSpan.o \
		../TCSpanList.o \
		../TCPageCache.o \
		../TCPageMap.o \
		../TCSystemMalloc.o \

CPP_S =	TCBenchMarkTest.cpp \
		../MallocAnsi.cpp \
		../MallocBase.cpp \
		../MallocTC.cpp \
		../MemoryBase.cpp \
		../MemoryManager.cpp \
		../../GenericPlatform/GenericPlatformMemory.cpp \
		../../Linux/LinuxPlatformMemory.cpp \
		../TCSizeMap.cpp \
		../TCSizeTools.cpp \
		../TCGlobals.cpp \
		../TCThreadCache.cpp \
		../TCCentralCache.cpp \
		../TCSpan.cpp \
		../TCSpanList.cpp \
		../TCPageCache.cpp \
		../TCPageMap.cpp \
		../TCSystemMalloc.cpp \

DEBUG_S = TCBenchMarkTest.o \
		MallocAnsi.o \
		MallocBase.o \
		MallocTC.o \
		MemoryBase.o \
		MemoryManager.o \
		GenericPlatformMemory.o \
		LinuxPlatformMemory.o \
		TCSizeMap.o \
		TCSizeTools.o \
		TCGlobals.o \
		TCThreadCache.o \
		TCCentralCache.o \
		TCSpan.o \
		TCSpanList.o \
		TCPageCache.o \
		TCPageMap.o \
		TCSystemMalloc.o \

main: ${OBJ_S} $(HEADER_S)
	g++ -Wall -o main.out $(OBJ_S)

# main: ${DEBUG_S} $(HEADER_S)
# 	g++ -Wall -g -o TCBenchMarkTest.out $(DEBUG_S)
#
# $(OBJ_S): $(HEADER_S)
# 	g++ -g -c $(CPP_S)

.PHONY : clean
clean:
	rm main.out $(OBJ_S)
