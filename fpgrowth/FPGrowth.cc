#include "FPGrowth.h"
#include <set>
#include <string>


bool FPGrowth::addRecord(const std::set<std::string> &record){
    if ( isTableEmpty() )
        return false;

    FPNode &curr_node = m_fpRoot;

    auto it = m_fpTable.m_rowOrder.cbegin();
    auto itend = m_fpTable.m_rowOrder.cend();
    for ( ;it!=itend; ++it ) {
        if ( record.find(*it)==record.end() ) 
            continue;
        FPNode *entry = curr_node.findChild(*it);
        if ( nullptr == entry ) {
            entry = m_fpTable.createNode(*it);
            curr_node.m_listDescendant.push_back(entry);
        }
        ++entry->m_nodeCnt;
        curr_node = *entry;
    }
    return true;
}

void FPGrowth::buildFPTable(const std::map<std::string, size_t> &table){ 
    for (auto it=table.begin(); it!=table.end(); ++it) {
        if( it->second >= m_minSupportCnt )
            m_fpTable.insertItem(it->first, it->second);
    }
#ifdef DEBUG
    m_fpTable.print();
#endif
}

std::vector<FPGrowth::freq_patten> FPGrowth::findAllFreqPattern() {
    auto it = m_fpTable.m_table.crbegin();
    auto it_end = m_fpTable.m_table.crend();

    std::vector<FPGrowth::freq_patten> outcome;
    for ( ; it != it_end; ++it ) {
        FreqPattenBasis basis(it->first, this);
        for ( FPGrowth::freq_patten& x: basis.getAllFreqPatten() )
            outcome.push_back(x);
    }
    return outcome;
}

