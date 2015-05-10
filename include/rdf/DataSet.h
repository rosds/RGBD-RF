#ifndef DataSet_HH
#define DataSet_HH

#include <vector>

namespace crf {

    template <class DataType, class LabelType> 
    class TrainExample 
    {
    public:
        TrainExample();
        ~TrainExample();
    
    private:
        vector<DataType> v;
        LabelType l;
    };

    class DataSet 
    {
    public:
        DataSet();
        ~DataSet();
    
    private:
        size_t size;
    };
}

#endif
