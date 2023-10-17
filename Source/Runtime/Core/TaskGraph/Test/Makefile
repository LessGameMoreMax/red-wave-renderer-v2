OBJ_S =	LinuxThreadSuspendTest.o \
		../../HAL/MallocAnsi.o \
		../../HAL/MallocBase.o \
		../../HAL/MallocTC.o \
		../../HAL/MemoryBase.o \
		../../HAL/MemoryManager.o \
		../../GenericPlatform/GenericPlatformMemory.o \
		../../Linux/LinuxPlatformMemory.o \
		../../Windows/WindowsPlatformMemory.o \
		../../HAL/TCSizeMap.o \
		../../HAL/TCSizeTools.o \
		../../HAL/TCGlobals.o \
		../../HAL/TCThreadCache.o \
		../../HAL/TCCentralCache.o \
		../../HAL/TCSpan.o \
		../../HAL/TCSpanList.o \
		../../HAL/TCPageCache.o \
		../../HAL/TCPageMap.o \
		../../HAL/TCSystemMalloc.o \
		../RStatus.o \
		../RunnableThread.o \
		../../Linux/LinuxPlatformThread.o \
		../../Linux/LinuxPlatformProcess.o \
		../../Windows/WindowsPlatformThread.o \
		../../Windows/WindowsPlatformProcess.o \

main: ${OBJ_S} $(HEADER_S)
	g++ -Wall -o main.out $(OBJ_S)

.PHONY : clean
clean:
	rm main.out $(OBJ_S)
