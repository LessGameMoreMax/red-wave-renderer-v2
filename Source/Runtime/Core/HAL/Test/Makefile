OBJ_S =	TCFreeList.o \
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

main : ${OBJ_S}
	g++ -Wall -o main.out $(OBJ_S)
	
$(OBJ_S) : $(HEADER_S)

.PHONY : clean
clean:
	rm main.out $(OBJ_S)
