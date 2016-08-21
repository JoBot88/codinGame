# *-coding:Utf-8 -*

class edge:

    def __init__(self, sVertex, aVertex, value):
       
        if(value>=0):
            self.value = value
            self.sVertex = sVertex
            self.aVertex = aVertex
        else:
            print("Les valuations des arrêtes doivent être positives")

    def __str__(self):
        return str(self.sVertex)+str(self.aVertex)+" : "+str(self.value)

class Graphe:

    def __init__(self, vertices, edges):
       
        self.vertices = vertices
        self.edges = edges
        self.cc = range(len(self.vertices))

    def __str__(self):
        chaine = ''
        for edge in self.edges:
            chaine = chaine +  str(edge)+"\n"
        return chaine

        
    def initialisation(self):
        
        self.sortEdges()
        return Graphe(self.vertices, [])


    def union(self, edge, Gr):

        if(self.cc[edge.sVertex]!=self.cc[edge.aVertex]):
            for i in self.cc:
                if(self.cc[i] == self.cc[edge.sVertex]):
                    self.cc[i] = self.cc[edge.aVertex]
            Gr.edges.append(edge)

    def sortEdges(self):
        self.edges.sort(key=lambda x: x.value)


AB = edge(0, 1, 3)
BC = edge(1, 2,  5)
CD = edge(2, 3, 1)
DE = edge(3, 4, 2)
AC = edge(0, 2, 7)
AD = edge(0, 3, 1)
BD = edge(1, 3, 3)
BE = edge(1, 4, 9)

edges = [AB, BC, CD, DE, AC, AD, BD, BE] 

G = Graphe([0, 1, 2, 3, 4], edges)  

Gr = G.initialisation()

for edge in G.edges:
    G.union(edge, Gr)
    if(len(Gr.edges) == len(G.vertices)-1):
        break
print(Gr)
                
