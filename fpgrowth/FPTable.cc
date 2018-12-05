#include "FPTable.h"
#include <string>
#include <memory>



FPTable::FPTable() {}
FPTable::~FPTable() {
    for( auto row=m_table.begin(); row!=m_table.end(); ++row ) {
        TNodeList &nodeList = (row->second).m_nodeList;
        for ( auto it=nodeList.begin(); it!=nodeList.end(); ++it ) {
            delete *it;
            *it = nullptr;
        }
    }
}

void FPTable::insertItem(const std::string &name, const size_t &cnt) {
    if ( m_table.find(name)==m_table.end() ) 
        m_table[name] = TTableRow(cnt);
    else
        m_table[name].m_cnt = cnt;

    auto it_end = m_rowOrder.end();
    auto it = m_rowOrder.begin();
    for ( ; it!=it_end; ++it ) {
        if ( cnt > m_table[*it].m_cnt ) {
            m_rowOrder.insert(it, name);
            break;
        }
    }
    if ( it == it_end ) {
        m_rowOrder.push_back(name);
    }
}

FPNode* FPTable::createNode(const std::string &name, const size_t &cnt) {
    FPNode* ptr_node = new FPNode(name, cnt);
    m_table[name].m_nodeList.push_back(ptr_node);
    return ptr_node;
}

