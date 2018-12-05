#ifndef FPNode_H_20181204
#define FPNode_H_20181204

#include <string>
#include <list>
#include <memory>

class FPGrowth;

class FPNode {
    public:
        friend class FPGrowth;
        FPNode(): FPNode("", 0) {}
        FPNode(const std::string &name, const size_t& cnt): m_nodeName(name), m_nodeCnt(cnt), m_ptrParent(nullptr) {}

        FPNode* findChild(const std::string &key);
        bool    isRoot() { return nullptr==m_ptrParent; }
        bool    isLeaf() { return 0==m_listDescendant.size(); }
    
    protected:
        std::string         m_nodeName;
        size_t              m_nodeCnt;
        FPNode*             m_ptrParent;
        std::list<FPNode*>  m_listDescendant;
};



#endif
