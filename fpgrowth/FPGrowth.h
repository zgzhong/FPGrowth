#ifndef FPGROWTH_H_20181205
#define FPGROWTH_H_20181205

#include "FPNode.h"
#include "FPTable.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>


class FPGrowth {
    public:
        FPGrowth(size_t totalCnt, double min_support, double min_confidence): 
            m_nTotalCnt(totalCnt), m_minSupport(min_support), m_minConfidence(min_confidence), m_minSupportCnt(size_t(m_nTotalCnt*m_minSupport)) {}
     
        void buildFPTable(const std::map<std::string, size_t> &table); 
        bool isTableEmpty() { return m_fpTable.m_table.empty(); } 
        bool addRecord(const std::set<std::string>&);
        
        struct FreqItem {
            std::string name;
            size_t cnt;
        };
        using freq_patten = std::vector<FreqItem>;
        std::vector<freq_patten> findAllFreqPattern();

        class FreqPattenBasis {
            public:
                FreqPattenBasis(const std::string &nodeName, FPGrowth* pf):m_nodeName(nodeName), _pf(pf) {}
                 
                std::vector<freq_patten> getAllFreqPatten() {
                    size_t num = 0;
                    std::vector<freq_patten> total;
                    getAllFreqPatten_helper(&_pf->m_fpRoot, num, total);
                    return total;
                }
             
            private:
                bool getAllFreqPatten_helper(FPNode *root, size_t &num, std::vector<freq_patten> &total) {
                    if ( nullptr == root)
                        return false;
                     
                    if ( root->m_nodeName == m_nodeName ) {
                        num += root->m_nodeCnt;
                        if ( num > _pf->m_minSupportCnt ) {
                            freq_patten path = { FreqItem{root->m_nodeName, root->m_nodeCnt} };
                            total.push_back(path);
                            return true;
                        }
                        return false;
                    }
                     
                    bool haveSubPath = false;
                    std::vector<freq_patten> temp_total_path;
                    auto it_desc = root->m_listDescendant.begin();
                    auto it_desc_end = root->m_listDescendant.end();
                    for ( ; it_desc != it_desc_end; ++it_desc ) {
                        size_t temp_num = 0;
                        haveSubPath |= getAllFreqPatten_helper( *it_desc, temp_num, temp_total_path);
                        num += temp_num;
                    }
                     
                    if ( !haveSubPath ) { // 没有子树
                        if ( num > _pf->m_minSupportCnt ) { // 但是满足最小支持度
                            freq_patten path = { FreqItem{root->m_nodeName, num} };
                            total.push_back(path);
                            return true;
                        }
                    } else { // 有子树
                        for ( auto it=temp_total_path.begin(); it!=temp_total_path.end(); ++it ) {
                            it->push_back(FreqItem{root->m_nodeName, num});
                            total.push_back(*it);
                        }
                        return true;
                    }
                    return false;
                }
                 
            private:
                std::string     m_nodeName;
                FPGrowth*       _pf;
        };
     
    protected:
        size_t    m_nTotalCnt;
        double    m_minSupport;
        double    m_minConfidence;
        size_t    m_minSupportCnt;
         
        FPNode    m_fpRoot;
        FPTable   m_fpTable;
};

#endif
