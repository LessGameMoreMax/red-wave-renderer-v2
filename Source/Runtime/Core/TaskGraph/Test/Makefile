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
		../../TaskGraph/GraphTaskThreadPool.o \
		../../TaskGraph/GraphTaskThread/GraphTaskThreadBase.o
	
main: $(OBJ_S)
	g++ $(OBJ_S) -o main.out

$(OBJ_S):
	g++ -std=c++20 -Wall -c $(patsubst %.o, %.cpp, $@) -o $@

.PHONY : clean
clean:
	rm *.out $(OBJ_S)
