#include "FPGrowth.h"
#include <set>
#include <string>
#include <algorithm>


NAMESPACE_ALPHALAB_BEGIN

/* ******************** *
 * FPTreeNode implement *
 * ******************** */
FPTreeNode::FPTreeNode(): FPTreeNode("", 0) {}
FPTreeNode::FPTreeNode(const std::string &name, const size_t &cnt):m_nNodeCnt(cnt), m_strNodeName(name), m_ptrParent(nullptr) {}

FPTreeNode::TChildPtr
FPTreeNode::findChild(const std::string &key) const {
    for (TChildPtr ptr: m_seqNodeChildren) {
        if ( key == ptr->m_strNodeName ) {
            return ptr;
        }
    }
    return nullptr;
}


/* ***************** *
 * FPTable implement *
 * ***************** */
FPTable::~FPTable() {
    for (TableRow &row: m_seqTable) {
        for (FPTreeNode* ptr: row.m_seqNodes) {
            delete ptr;
        }
    }
}

FPTable*
FPTable::createTable(const std::multimap<size_t, std::string> &items) {
    FPTable* table = new FPTable();
    table->buildTable_helper(items);
    return table;
}

bool
FPTable::buildTable_helper(const std::multimap<size_t, std::string> &items) {
    if ( !m_seqTable.empty() )
        return false;
    
    for ( auto it=items.rbegin(); it!=items.rend(); ++it ) {
        m_seqTable.emplace_back(it->second, it->first);
    }
    return true;
}

FPTreeNode*
FPTable::createNode(const std::string &name) {
    FPTreeNode *ptrNode = new FPTreeNode(name);
    for ( auto it=m_seqTable.begin(); it!=m_seqTable.end(); ++it ) {
        if ( it->m_strRowName == name ) {
            it->m_seqNodes.push_back(ptrNode);
            return ptrNode;
        }
    }
    delete ptrNode;
    return nullptr;
}


/* ***************** *
 * FPGrowth implement *
 * ***************** */
FPGrowth::FPGrowth(size_t totalSize, double support, double confidence):
    m_totalSize(totalSize), m_supportSize(totalSize * support), m_confidence(confidence),
    m_ptrFPTable(nullptr), m_ptrFPTreeRoot(new FPTreeNode)
{}

void
FPGrowth::buildFPTable(const std::map<std::string, size_t> &items) {
    std::multimap<size_t, std::string> tableMap;
    for ( auto it=items.begin(); it!=items.end(); ++it ) {
        if ( it->second > m_supportSize )
            tableMap.insert(std::make_pair(it->second, it->first));
    }
    m_ptrFPTable = FPTable::createTable(tableMap);
}

bool
FPGrowth::addRecord(const std::set<std::string> &record) {
    if ( nullptr==m_ptrFPTable )
        return false;
    
    FPTreeNode *currNode = m_ptrFPTreeRoot;
    auto it = m_ptrFPTable->getTable().begin();
    auto itend = m_ptrFPTable->getTable().end();
    for ( ; it!=itend; ++it ) {
        if ( record.find(it->m_strRowName) == record.end() )
            continue;
        FPTreeNode *entry = currNode->findChild(it->m_strRowName);
        if ( nullptr == entry ){
            entry = m_ptrFPTable->createNode(it->m_strRowName);
            currNode->addChild(entry);
        }
        entry->increase();
        entry->setParent(currNode);
        currNode = entry;
    }
    return true;
}

std::vector<FPGrowth::TFreqPattern>
FPGrowth::findAllFreqPattern(bool reverse_short) {
    const auto &table = m_ptrFPTable->getTable();
    auto item = table.rbegin();
    
    std::vector<TFreqPattern> outcome;

    for ( ; item!=table.rend(); ++item ) { // 逆序迭代项头表
        const std::vector<FPTreeNode*> &nodes = item->m_seqNodes;
        std::map<std::string, size_t> subPatternMap;
        std::vector<std::vector<TPatternItem> > subPatternVec;

        // 自底向上扫描
        for ( auto nodeIt = nodes.cbegin(); nodeIt != nodes.cend(); ++nodeIt ) {
            FPTreeNode *curr_node = *nodeIt;
            std::vector<TPatternItem> subPattern;
            while ( nullptr != curr_node->getParent() ) {
                subPatternMap[curr_node->getName()] += curr_node->getSize();
                subPattern.push_back(TPatternItem{curr_node->getName(), curr_node->getSize()});
                curr_node = curr_node->getParent();
            }
            subPatternVec.push_back(subPattern);
        }
        // 构建条件模式基
        FPBasis basis(this, item->m_strRowName, subPatternMap, subPatternVec);
        for ( auto &pattern: basis.getAllFreqPattern() ) {
            outcome.push_back(pattern);
        }
    }

    if ( !reverse_short ) // 只保留
        eliminateShortPattern(outcome);
    return outcome;
}

void
FPGrowth::eliminateShortPattern(std::vector<FPGrowth::TFreqPattern> &patterns) {
    std::sort(patterns.begin(), patterns.end(), 
        [](const FPGrowth::TFreqPattern&a, const FPGrowth::TFreqPattern&b){ return a.size() > b.size(); }
    );
    std::vector<FPGrowth::TFreqPattern> temp;
    for ( auto r_it=patterns.rbegin(); r_it!=patterns.rend(); ++r_it) {
        auto ahead_it = r_it+1;
        bool isKeep = true;
        for ( ; ahead_it!=patterns.rend(); ++ahead_it ) {
            if (std::includes(ahead_it->begin(), ahead_it->end(), r_it->begin(), r_it->end())){
                isKeep = false;
                break;
            }
        }
        if (isKeep)
            temp.push_back(*r_it);
    }
    patterns.swap(temp);
}


FPBasis::FPBasis(
    FPGrowth *instance, 
    const std::string &name, 
    const std::map<std::string , size_t> &projectMap,
    const std::vector<std::vector<TPatternItem> > &projectPattern)
        : _ptr(instance), m_prefixName(name), _root(nullptr)
{
    _root = new BasisNode;
    for ( const std::vector<TPatternItem> &pattern: projectPattern ){
        BasisNode *curr_node = _root;
        for ( auto it=pattern.rbegin(); it!=pattern.rend(); ++it ){
            if ( projectMap.at(it->name) < _ptr->m_supportSize) {
                continue;
            }
            bool exist = false;
            for ( auto subNode : curr_node->sub ) {
                if ( subNode->name == it->name ) {
                    subNode->cnt += it->cnt;
                    curr_node = subNode;
                    exist = true;
                    break;
                }
            }
            if (!exist) {
                BasisNode *newNode = new BasisNode();
                newNode->name = it->name;
                newNode->cnt = it->cnt;
                curr_node->sub.push_back(newNode);
                curr_node = newNode;
            }
        }
    }
}

void
FPBasis::destroy (FPBasis::BasisNode *root) {
    for( auto ptr: root->sub){ destroy(ptr); }
    delete root;
    root = nullptr;
}

std::vector<FPGrowth::TFreqPattern>
FPBasis::getAllFreqPattern() {
    std::vector<FPGrowth::TFreqPattern> patterns;
    FPGrowth::TFreqPattern temp;
    findPatternHelper(_root, temp, patterns);
    return patterns;
}

void
FPBasis::findPatternHelper(FPBasis::BasisNode *root, TFreqPattern& path, std::vector<TFreqPattern> &total) {
    if (root->name.size())
        path.insert(root->name);

    
    if ( root->sub.empty() ){
        total.push_back(path);
    } else {
        for ( auto ptr: root->sub) {
            findPatternHelper(ptr, path, total);
        }
    }
    if (root->name.size())
        path.erase(root->name);
}

NAMESPACE_ALPHALAB_END
