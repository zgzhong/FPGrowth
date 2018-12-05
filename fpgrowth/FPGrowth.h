#ifndef FPGROWTH_H_20181205
#define FPGROWTH_H_20181205

#include "FPNode.h"
#include "FPTable.h"

#include <vector>


class FPGrowth {
    public:
        FPGrowth(size_t totalCnt): m_nTotalCnt(totalCnt) {}
        void buildFPTable();
        void buildFPTree();
     
    protected:
        void addRecord(const std::vector<std::string>&);
     
    protected:
        size_t    m_nTotalCnt;
        
        FPNode    m_fpRoot;
        FPTable   m_fpTable;
};

#endif
