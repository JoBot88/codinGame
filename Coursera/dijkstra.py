# *-coding:Utf-8 -*

class vertex:

    def __init__(self, name, value):
        self.name = name
        self.buddies = dict()
        self.value = value

    def __str__(self):
        return str(self.name)

class Graph:

    def __init__(self, name, vertices):
        
        self.vertices = vertices
        self.name = name

def Dijkstra(G, source):

    sPaths = [(float('inf'), vertex) for vertex in G.vertices] 
   
    sPaths[source.value] = (0, source)

    pivot = source
   
    papas = [None]*len(G.vertices)

    done = []

   
    while len(done) != (len(G.vertices)-1):
       
        done.append(pivot)
                
        for buddy in pivot.buddies.keys():
            if(sPaths[buddy.value][0] > sPaths[pivot.value][0] + pivot.buddies[buddy]):
                sPaths[buddy.value] = (sPaths[pivot.value][0] + pivot.buddies[buddy], sPaths[buddy.value][1])
                papas[buddy.value] = pivot

                
       
        for sommet in set([t[1] for t in sPaths])-set(done):
            if sPaths[sommet.value][0] == min(set([t[0] for t in sPaths])-set(sPaths[i.value][0] for i in done)):        
                pivot = sommet
              

    for path in sPaths:
        print("\ndistance de "+str(source)+" à "+ str(path[1])+" = "+str(path[0])+"\n")

A = vertex("A", 0)
B = vertex("B", 1)
C = vertex("C", 2)
D = vertex("D", 3)
E = vertex("E", 4)
S = vertex("S", 5)

E.buddies = {A:3, B:1}
A.buddies = {E:3, B:1, C:3}
B.buddies = {E:1, A:1, C:3, D:5}
C.buddies = {A:3, B:3, D:1, S:3}
D.buddies = {B:5, C:1, S:1}


G = Graph("G", [A, B, C, D, E, S])

Dijkstra(G,E)
