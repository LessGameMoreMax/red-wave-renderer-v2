#ifndef TASK_GRAPH_MAX_PARA_OPTIMIZER_H
#define TASK_GRAPH_MAX_PARA_OPTIMIZER_H
#include "TaskGraphOptimizerBase.h"
namespace sablin{
    
class TaskGraphMaxParaOptimizer: public TaskGraphOptimizerBase{
    friend class TaskGraphElementManager;
protected:
    bool Match(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements) override{
        return !std::any_of(elements.begin(), elements.end(), [](TaskGraphElement* ptr){
            return ptr->IsGroup();
        });
    }

    size_t GetMaxParaSize(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements){
        const size_t size = elements.size();
        std::vector<std::vector<TaskGraphElement*>> paths;
        CollectPaths(elements, paths);

        std::vector<std::vector<int32_t>> ref_graph(size, std::vector<int32_t>(size, 1));
        BuildReverseGraph(elements, paths, ref_graph);
        return CalcMaxCliqueSize(ref_graph);
    }

    void CollectPaths(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements,
            std::vector<std::vector<TaskGraphElement*>>& paths){
        for(auto& element: elements){
            std::vector<TaskGraphElement*> cur_path;
            if(element->dependence_.empty()){
                Collect(element, cur_path, paths);
            }
        }
    }

    void Collect(TaskGraphElement* element,
            std::vector<TaskGraphElement*>& cur_path,
            std::vector<std::vector<TaskGraphElement*>>& paths){
        cur_path.emplace_back(element);
        if(element->run_before_.empty()){
            paths.emplace_back(cur_path);
        }else{
            for(auto& cur: element->run_before_){
                Collect(cur, cur_path, paths);
            }
        }
        cur_path.pop_back();
    }

    static void BuildReverseGraph(const std::set<TaskGraphElement*, TaskGraphElementSorter>& elements,
            const std::vector<std::vector<TaskGraphElement*>>& paths,
            std::vector<std::vector<int32_t>>& graph){
        for(auto& path: paths){
            for(int32_t i =0;i < path.size() - 1; ++i){
                int32_t height = std::distance(elements.begin(), elements.find(path[i]));
                for(int32_t j = i + 1; j < path.size(); ++j){
                    int32_t column = std::distance(elements.begin(), elements.find(path[j]));
                    graph[height][column] = 0;
                    graph[column][height] = 0;
                }
            }
        }
    }

    static size_t CalcMaxCliqueSize(const std::vector<std::vector<int32_t>>& graph){
        size_t element_size = graph.size();
        size_t max_clique_size = 0;
        std::vector<size_t> cur_clique;
        std::function<void(size_t, size_t)> backtrack = [&](size_t start, size_t depth){
            for(size_t i = start; i < element_size; ++i){
                if(depth + element_size - i <= max_clique_size) return;
                if(std::all_of(cur_clique.begin(), cur_clique.end(), [&](const int32_t j){
                    return graph[j][i] == 1;
                })){
                    cur_clique.push_back(i);
                    backtrack(i+1, depth+1);
                    cur_clique.pop_back();
                }
            }
            max_clique_size = Max(cur_clique.size(), max_clique_size);
        };
        backtrack(0, 0);
        return max_clique_size;
    }

};
}
#endif
