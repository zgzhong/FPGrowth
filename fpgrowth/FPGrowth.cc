#include "FPGrowth.h"
#include <vector>
#include <string>


void FPGrowth::addRecord(const std::vector<std::string> &record){
    auto record_it = record.cbegin();
    FPNode &curr_node = m_fpRoot;
    while ( record_it != record.cend() ) {
        FPNode *entry = curr_node.findChild(*record_it);
        if ( nullptr == entry ) {
            entry = m_fpTable.createNode(*record_it);
            curr_node.m_listDescendant.push_back(entry);
        }
        ++entry->m_nodeCnt;
        curr_node = *entry;
        ++record_it;
    }
}



void FPGrowth::buildFPTable() {


}




void FPGrowth::buildFPTree() {


}
