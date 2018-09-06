#include "treenode.h"
;
namespace mzh
{
namespace cpptools
{
int SGTreeNode::printParams(int  source, int  destination)
{
    printf("the number in cpp is from %d  to %d\n", source, destination);
    return 0;
}

void SGTreeNode::ptrParams(double *source, double *destination)
{
    printf("ptr test from %lf to %lf", *source, *destination);
}

}
}

