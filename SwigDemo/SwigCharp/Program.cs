using System;

namespace SwigCharp
{
    class Program
    {
        static void Main(string[] args)
        {
            ITreeNode treeNode = new SGTreeNode();
           int answer=  treeNode.printParams(24, 10);
            Console.WriteLine(answer);
            double cppA = 64;
            double cppB = 74;
            treeNode.ptrParams( cppA,  cppB);
            Console.ReadKey();
        }
    }
}
