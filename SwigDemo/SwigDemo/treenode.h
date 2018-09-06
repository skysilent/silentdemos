#ifndef TREENODE_H_
#define TREENODE_H_
#include <iostream>
#include "itreenode.h"
namespace mzh
{
namespace cpptools
{
class SGTreeNode :public ITreeNode
{
private:
public:
    virtual int  printParams(int source, int destination);
    virtual void ptrParams(double *source, double *destination);
};
}
}
#endif
