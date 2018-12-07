#ifndef FPTABLE_H_20181205
#define FPTABLE_H_20181205

#include <list>
#include <map>
#include <string>
#include "FPNode.h"

class FPGrowth;


// 频繁表项, 负责记录1频繁模式
// 以及树节点的创建和内存回收
class FPTable {
    public:
        friend class FPGrowth;
         
        FPTable();
        ~FPTable();
        void insertItem(const std::string&, const size_t&);
        FPNode* createNode(const std::string&, const size_t &cnt=0 );
         
        using TNodeList = std::list<FPNode*>;
        class TTableRow {
            public:
                TTableRow(const size_t &cnt=0): m_cnt(cnt) {}
            public:
                TNodeList   m_nodeList;
                size_t      m_cnt; 
        };
        #ifdef DEBUG
        void print(){
            for ( auto it=m_rowOrder.begin(); it!=m_rowOrder.end(); ++it) {
                std::cerr << *it << " "<< m_table[*it].m_cnt << std::endl;
            }
        }
        #endif

    private:
        std::map<std::string, TTableRow> m_table;
        std::list<std::string>           m_rowOrder;
};

#endif
