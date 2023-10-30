DIR = ~/Projects/red-wave-renderer-v2/Source

OBJ_S =	TaskGraphAspectTest.o \
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
		$(DIR)/Runtime/Core/TaskGraph/GraphTaskThreadPool.o \
		$(DIR)/Runtime/Core/TaskGraph/GraphTaskThread/GraphTaskThreadBase.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphAspect/TaskGraphAspectBase.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphAspect/TaskGraphAspectManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphAspect/TaskGraphAspect.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphDaemon/TaskGraphDaemonBase.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphDaemon/TaskGraphDaemon.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphDaemon/TaskGraphDaemonManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphAdapter/TaskGraphFence/TaskGraphFence.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphAdapter/TaskGraphFunction/TaskGraphFunction.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphElement.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphElementManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphElementRepository.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphDynamicEngine/TaskGraphDynamicEngine.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphStaticEngine/TaskGraphStaticEngine.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphEngine/TaskGraphEngine.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphGroup/TaskGraphCluster/TaskGraphCluster.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphGroup/TaskGraphGroup.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphElement/TaskGraphNode/TaskGraphNode.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphEvent/TaskGraphEvent.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphEvent/TaskGraphEventManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphParam/TaskGraphParam.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphParam/TaskGraphParamManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphPipeline/TaskGraphPerf/TaskGraphPerf.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphPipeline/TaskGraphPipeline.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphPipeline/TaskGraphPipelineFactory.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphPipeline/TaskGraphPipelineManager.o \
		$(DIR)/Runtime/Core/TaskGraph/TaskGraphPipeline/TaskGraphSchedule/TaskGraphSchedule.o \
	
main: $(OBJ_S)
	g++ $(OBJ_S) -o main.out

$(OBJ_S):
	g++ -std=c++20 -Wall -c $(patsubst %.o, %.cpp, $@) -o $@

.PHONY : clean
clean:
	rm *.out $(OBJ_S)
