from collections import deque
#graph represented by G.dictionary = {V,E} whre V = set of Vectors , E = edge info
class GraphNode: #Graph repres in dictionary. directed/undirected : G(V,E) -> G(dict.key, dict.key.value). weighted graphs have dict for value with vertex and weight information.
  def __init__(self,key,parent,depth=0):
    self.key = key
    self.parent = parent
    self.depth = depth

def breath_first_search(inpGraph,source): #inputGraph,source. creates bfs tree, discovers all vertices (only once) at distance k from source/root vertex before k+1 vertices. base of prims,dijkstra's algo.
  if not bool(inpGraph):  #input graph(dict) is empty
    return 0
  else:
    mainQ = deque() #queue for discovered nodes
    if(source!= None):
      root = GraphNode(source,None,0) #starting node of the graph
      mainQ.append(source)               #add source to root node value
      #discovered = {}
      discovered = {root.key:root}  #can carry any values to display later
      #print(len(mainQ))
      while len(mainQ)!=0:         #queue shd have min one node
        #print(mainQ.popleft().key)
        curr_val = mainQ.popleft()
        #discovered.append(root)
        if curr_val in inpGraph: #check 
          print("node: "+str(curr_val))
          for adj in inpGraph[curr_val]: # each node connected to the curr_node
            if adj not in discovered and adj not in mainQ:
              print("->"+str(adj))
              node = GraphNode(adj,discovered[curr_val],discovered[curr_val].depth+1)   #create node,assign its parent,depth
              discovered[adj] = node
              mainQ.append(adj)
        else:
          print("source does not exist")
      for k,v in discovered.items():
        print(str(k)+":"+str(v.depth))

print("Graph")
#inp = {1:[2,5],2:[1,5,3,4],3:[2,4],4:[2,5,3],5:[4,1,2]}
inp = {'r':['s','v'],'s':['r','w'],'v':'r','w':['s','t','x'],'t':['x','w','u'],'x':['t','w','u','y'],'u':['t','x','y'],'y':['x','u']}
print(inp)
print("breath_first_search")
breath_first_search(inp,'x')