OBJ_S =	TCBenchMarkTest.o \
		../MallocAnsi.o \
		../MallocBase.o \
		../MallocTC.o \
		../MemoryBase.o \
		../MemoryManager.o \
		../../GenericPlatform/GenericPlatformMemory.o \
		../../Linux/LinuxPlatformMemory.o \
		../../Windows/WindowsPlatformMemory.o \
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

main: ${OBJ_S} $(HEADER_S)
	g++ -Wall -o main.out $(OBJ_S)

.PHONY : clean
clean:
	rm main.out $(OBJ_S)
