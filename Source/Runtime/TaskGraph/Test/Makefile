DIR = ~/Projects/red-wave-renderer-v2/Source

OBJ_S =	TaskGraphDaemonTest.o \
		$(DIR)/Runtime/Core/HAL/MallocAnsi.o \
		$(DIR)/Runtime/Core/HAL/MallocBase.o \
		$(DIR)/Runtime/Core/HAL/MallocTC.o \
		$(DIR)/Runtime/Core/HAL/MemoryBase.o \
		$(DIR)/Runtime/Core/HAL/MemoryManager.o \
		$(DIR)/Runtime/Core/HAL/TCSizeMap.o \
		$(DIR)/Runtime/Core/HAL/TCSizeTools.o \
		$(DIR)/Runtime/Core/HAL/TCGlobals.o \
		$(DIR)/Runtime/Core/HAL/TCThreadCache.o \
		$(DIR)/Runtime/Core/HAL/TCCentralCache.o \
		$(DIR)/Runtime/Core/HAL/TCSpan.o \
		$(DIR)/Runtime/Core/HAL/TCSpanList.o \
		$(DIR)/Runtime/Core/HAL/TCPageCache.o \
		$(DIR)/Runtime/Core/HAL/TCPageMap.o \
		$(DIR)/Runtime/Core/HAL/TCSystemMalloc.o \
		$(DIR)/Runtime/Core/HAL/RStatus.o \
		$(DIR)/Runtime/Core/HAL/RunnableThread.o \
		$(DIR)/Runtime/Core/HAL/ThisThread.o \
		$(DIR)/Runtime/Core/GenericPlatform/GenericPlatformMemory.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformMemory.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformThread.o \
		$(DIR)/Runtime/Core/Windows/WindowsPlatformProcess.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformMemory.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformThread.o \
		$(DIR)/Runtime/Core/Linux/LinuxPlatformProcess.o \
		$(DIR)/Runtime/TaskGraph/GraphTaskThreadPool.o \
		$(DIR)/Runtime/TaskGraph/GraphTaskThread/GraphTaskThreadBase.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphAspect/TaskGraphAspectBase.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphAspect/TaskGraphAspectManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphAspect/TaskGraphAspect.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphDaemon/TaskGraphDaemonBase.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphDaemon/TaskGraphDaemon.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphDaemon/TaskGraphDaemonManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphAdapter/TaskGraphFence/TaskGraphFence.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphAdapter/TaskGraphFunction/TaskGraphFunction.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphElement.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphElementManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphElementRepository.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphDynamicEngine/TaskGraphDynamicEngine.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphStaticEngine/TaskGraphStaticEngine.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphEngine.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphGroup/TaskGraphGroup.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphElement/TaskGraphNode/TaskGraphNode.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphEvent/TaskGraphEvent.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphEvent/TaskGraphEventManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphParam/TaskGraphParam.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphParam/TaskGraphParamManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphPipeline/TaskGraphPerf/TaskGraphPerf.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphPipeline/TaskGraphPipeline.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphPipeline/TaskGraphPipelineFactory.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphPipeline/TaskGraphPipelineManager.o \
		$(DIR)/Runtime/TaskGraph/TaskGraphPipeline/TaskGraphSchedule/TaskGraphSchedule.o \
	
main: $(OBJ_S)
	g++ $(OBJ_S) -o main.out

$(OBJ_S):
	g++ -std=c++20 -Wall -c $(patsubst %.o, %.cpp, $@) -o $@

.PHONY : clean
clean:
	rm *.out $(OBJ_S)
