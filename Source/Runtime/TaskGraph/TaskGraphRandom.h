#ifndef TASK_GRAPH_RANDOM_H
#define TASK_GRAPH_RANDOM_H
#include "TaskGraphDefine.h"
#include "../Core/Misc/MacroTools.h"
#include <random>
namespace sablin{

template<typename T = float,
        int32_t SEED = kRandomSeed, 
        typename RandomEngine = std::mt19937>
class TaskGraphRandom{
public:
    static std::string GenerateSession(const std::string& key = STRING_UNKNOWN, size_t size = 3){
        std::string session;
        std::vector<float>* vector_float = Generate(size, 100000, 999999);
        for(size_t i = 0;i != size; ++i){
            session += std::to_string(int32_t(vector_float->at(i)));
            session += '-';
        }
        delete vector_float;
        session += key;
        return session;
    }

    static std::vector<T>* Generate(size_t dim, const T& min_value, const T& max_value){
        std::random_device rd;
        RandomEngine engine(SEED == kRandomSeed? rd() : SEED);
        std::uniform_real_distribution<T> urd(min_value, max_value);

        std::vector<T>* result = new std::vector<T>();
        result->reserve(dim);
        for(size_t i = 0;i != dim; ++i)
            result->emplace_back(urd(engine));
        return result;
    }

};

}
#endif
