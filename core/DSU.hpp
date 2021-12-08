#ifndef DSU_HPP
#define DSU_HPP

class DSU
{
public:
    virtual bool unite(int u, int v) = 0;
    virtual bool sameSet(int u, int v) = 0;
    virtual int countSets() = 0;

    int size()
    {
        return N;
    }
    
protected:
    int N;

};

#endif
