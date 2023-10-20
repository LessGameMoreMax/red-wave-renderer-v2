OBJ_S =	ThreadPoolTest.o \
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
		../../HAL/RStatus.o \
		../../HAL/RunnableThread.o \
		../../Linux/LinuxPlatformThread.o \
		../../Linux/LinuxPlatformProcess.o \
		../../Windows/WindowsPlatformThread.o \
		../../Windows/WindowsPlatformProcess.o \
		../GraphTaskThreadPool.o \
		../GraphTaskThread/GraphTaskThreadBase.o \

CPP_S =	ThreadPoolTest.cpp \
		../../HAL/MallocAnsi.cpp \
		../../HAL/MallocBase.cpp \
		../../HAL/MallocTC.cpp \
		../../HAL/MemoryBase.cpp \
		../../HAL/MemoryManager.cpp \
		../../GenericPlatform/GenericPlatformMemory.cpp \
		../../Linux/LinuxPlatformMemory.cpp \
		../../Windows/WindowsPlatformMemory.cpp \
		../../HAL/TCSizeMap.cpp \
		../../HAL/TCSizeTools.cpp \
		../../HAL/TCGlobals.cpp \
		../../HAL/TCThreadCache.cpp \
		../../HAL/TCCentralCache.cpp \
		../../HAL/TCSpan.cpp \
		../../HAL/TCSpanList.cpp \
		../../HAL/TCPageCache.cpp \
		../../HAL/TCPageMap.cpp \
		../../HAL/TCSystemMalloc.cpp \
		../../HAL/RStatus.cpp \
		../../HAL/RunnableThread.cpp \
		../../Linux/LinuxPlatformThread.cpp \
		../../Linux/LinuxPlatformProcess.cpp \
		../../Windows/WindowsPlatformThread.cpp \
		../../Windows/WindowsPlatformProcess.cpp \
		../GraphTaskThreadPool.cpp \
		../GraphTaskThread/GraphTaskThreadBase.cpp \

main: ${OBJ_S} $(HEADER_S)
	g++ -std=c++20 -Wall -o main.out $(OBJ_S)

$(OBJ_S): $(HEADER_S)
	g++ -std=c++20 -c $(CPP_S)

.PHONY : clean
clean:
	rm main.out $(OBJ_S)

.PHONY : debug
debug: ${OBJ_S} $(HEADER_S)
	g++ -std=c++20 -Wall -g -o debug.out $(OBJ_S)

$(OBJ_S): $(HEADER_S)
	g++ -std=c++20 -g -c $(CPP_S)
