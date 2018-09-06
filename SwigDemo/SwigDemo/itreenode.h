#ifndef INTERFACE_TREENODE_H_
#define INTERFACE_TREENODE_H_
namespace mzh
{
namespace cpptools
{
class ITreeNode
{
public:
    virtual ~ITreeNode()
    {
    };

    
    virtual int printParams(int source, int destination)=0;
#ifdef SWIG
    %include <typemaps.i>
    %apply double* INPUT{ double *source, double *destination }
#endif // SWIG
    virtual void ptrParams(double *source, double *destination) = 0;
};
}
}
#endif