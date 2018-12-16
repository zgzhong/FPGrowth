#ifndef FPGROWTH_H_20181205
#define FPGROWTH_H_20181205

#include "global.h"

#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

NAMESPACE_ALPHALAB_BEGIN

class FPTreeNode;
class FPTable;
class FPGrowth;
class FPBasis;


class FPTreeNode {
public:
    typedef FPTreeNode* TChildPtr;
    typedef FPTreeNode* TParentPtr;

    FPTreeNode();
    FPTreeNode(const std::string &, const size_t &cnt=0);
    TChildPtr           findChild(const std::string &) const;
    TParentPtr          getParent() const { return m_ptrParent; }
    void                addChild(TChildPtr child) { m_seqNodeChildren.push_back(child); }
    void                setParent(TParentPtr ptr) { m_ptrParent = ptr; }
    bool                isLeaf() const { return m_seqNodeChildren.empty(); }
    void                increase() { ++m_nNodeCnt; }
    const std::string&  getName() const { return m_strNodeName; }
    const size_t&       getSize() const { return m_nNodeCnt; }

protected:
    size_t                  m_nNodeCnt;
    std::string             m_strNodeName;
    TParentPtr              m_ptrParent;
    std::vector<TChildPtr>  m_seqNodeChildren;
};


class FPTable {
public:
    class TableRow {
        public:
            TableRow(const std::string &name, const size_t &cnt=0):
                m_strRowName(name), m_nCnt(cnt) {}
        public:
            std::string                m_strRowName;
            std::vector<FPTreeNode*>   m_seqNodes;
            size_t                     m_nCnt;
    };

    typedef std::vector<FPTreeNode*> TNodeList;
    typedef std::vector<TableRow> TTable;

    ~FPTable();
    //! Create FPTreeNode and insert to table row
    FPTreeNode* createNode(const std::string &);
    //! Build table from one-freq item map. Table format: { freq_cnt: row_name }
    static FPTable* createTable(const std::multimap<size_t, std::string> &);
    bool buildTable_helper(const std::multimap<size_t, std::string> &);

    const TTable& getTable() const { return m_seqTable; }

protected:
    FPTable() {}
    TTable          m_seqTable;
};



class FPGrowth {
    public:
        friend class FPBasis;
        FPGrowth(size_t totalSize, double support, double confidence);
        ~FPGrowth() { delete m_ptrFPTable; delete m_ptrFPTreeRoot; }
        void buildFPTable(const std::map<std::string, size_t> &);
        bool addRecord(const std::set<std::string> &);

        typedef std::set<std::string> TFreqPattern;
        typedef struct { std::string name; size_t cnt; } TPatternItem;
        std::vector<TFreqPattern> findAllFreqPattern(bool reverse_short=false );

    protected:
        void eliminateShortPattern(std::vector<TFreqPattern>&);

        size_t      m_totalSize;
        size_t      m_supportSize;
        double      m_confidence;

        FPTable     *m_ptrFPTable;
        FPTreeNode  *m_ptrFPTreeRoot;
};


class FPBasis {
    public:
        typedef FPGrowth::TFreqPattern TFreqPattern;
        typedef FPGrowth::TPatternItem TPatternItem;
        struct BasisNode {
            std::string name;
            size_t cnt;
            std::vector<BasisNode*> sub;
        };
        
        FPBasis(FPGrowth *instance, 
                const std::string &name, 
                const std::map<std::string , size_t>&,
                const std::vector<std::vector<TPatternItem> >& );
        ~FPBasis() { destroy(_root);}
        std::vector<TFreqPattern> getAllFreqPattern();

    private:
        void destroy(BasisNode *root);
        void findPatternHelper(BasisNode *, TFreqPattern &, std::vector<TFreqPattern> &);
        FPGrowth    *_ptr;
        std::string m_prefixName;
        BasisNode   *_root;
};


NAMESPACE_ALPHALAB_END


#endif
