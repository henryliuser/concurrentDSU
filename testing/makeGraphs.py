import sys
from random import randint

def makeGraph(V, E):
    seen = set()
    while len(seen) < E:
        u = randint(0, V-1)
        v = randint(0, V-1)
        if u == v: continue
        seen.add( tuple(sorted((u,v))) )
    return (V, E, seen)

def toString(edges):
    ptup = lambda x : f"{x[0]} {x[1]}"
    return "\n".join( [*map(ptup, edges)] )

for i in range(10):
    j = pow(i+6, 3)
    a, b = 100*j+10, 400*j+20
    m = makeGraph(a, b)
    q = makeGraph(a, b//2)
    base = "/Users/henryliu/Desktop/school/CMU/Fall21/15418/assignments/cmu15418f21"
    with open(base+f"/a5/{i}.in", 'w') as f:
        f.write(f"{m[0]}\n{m[1]}\n{q[1]}\n")
        f.write(toString(m[2]) + '\n\n')
        f.write(toString(q[2]) + '\n')
