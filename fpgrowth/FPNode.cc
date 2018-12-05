#include "FPNode.h"


FPNode* FPNode::findChild(const std::string &key) {
    FPNode* found = nullptr;
    for ( auto it=m_listDescendant.cbegin(); it!=m_listDescendant.cend(); ++it ){
        if ( key == (*it)->m_nodeName ) {
            found = *it;
            break;
        }
    }
    return found;
}
