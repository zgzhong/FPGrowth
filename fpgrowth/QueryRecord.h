#ifndef FPGROW_QueryRecord_H_20181205
#define FPGROW_QueryRecord_H_20181205

#include <string>
#include <list>


class AbstractQueryRecord {
    public:
        AbstractQueryRecord();
        virtual ~AbstractQueryRecord() = 0;
        virtual void loadDataSource() = 0;
        
        class iterator {
            public:

        
        };
};



#endif