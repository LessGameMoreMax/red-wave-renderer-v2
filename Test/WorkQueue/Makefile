DIR = ~/Projects/red-wave-renderer-v2/Source

OBJ_S =	WorkQueueTests.o \
		$(DIR)/Runtime/Core/HAL/MallocAnsi.o \
		$(DIR)/Runtime/Core/HAL/MallocBase.o \
		$(DIR)/Runtime/Core/HAL/MallocTC.o \
		$(DIR)/Runtime/Core/HAL/MemoryBase.o \
		$(DIR)/Runtime/Core/HAL/MemoryManager.o \
		$(DIR)/Runtime/Core/HAL/RStatus.o \
		$(DIR)/Runtime/Core/HAL/RunnableThread.o \
		$(DIR)/Runtime/Core/HAL/TCCentralCache.o \
		$(DIR)/Runtime/Core/HAL/TCGlobals.o \
		$(DIR)/Runtime/Core/HAL/TCPageCache.o \
		$(DIR)/Runtime/Core/HAL/TCPageMap.o \
		$(DIR)/Runtime/Core/HAL/TCSizeMap.o \
		$(DIR)/Runtime/Core/HAL/TCSizeTools.o \
		$(DIR)/Runtime/Core/HAL/TCSpan.o \
		$(DIR)/Runtime/Core/HAL/TCSpanList.o \
		$(DIR)/Runtime/Core/HAL/TCSystemMalloc.o \
		$(DIR)/Runtime/Core/HAL/TCThreadCache.o \
		$(DIR)/Runtime/Core/HAL/ThisThread.o \
		$(DIR)/Runtime/Core/GenericPlatform/GenericPlatformMemory.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformMemory.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformThread.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformProcess.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformMemory.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformThread.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformProcess.o \
		$(DIR)/Runtime/WorkQueue/WorkQueue.o \
		$(DIR)/Runtime/WorkQueue/WorkThreadPool.o \
	
main: $(OBJ_S)
	g++ $(OBJ_S) -o main.out

$(OBJ_S):
	g++ -std=c++20 -Wall -c $(patsubst %.o, %.cpp, $@) -o $@

.PHONY : clean
clean:
	rm *.out $(OBJ_S)
