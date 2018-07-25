from collections import deque
#graph represented by G.dictionary = {V,E} whre V = set of Vectors , E = edge info
class GraphNode: #Graph repres in dictionary. directed/undirected : G(V,E) -> G(dict.key, dict.key.value). weighted graphs have dict for value with vertex and weight information.
  def __init__(self,key,parent,depth=0,startTime=0):
    self.key = key
    self.parent = parent
    self.depth = depth                      #number of edges
    self.startTime = startTime                      #start time stamp

  endTime = 0

def print_path(graphDict,source,target):
  if target.key == source.key:
    return target.key
  elif target.parent == None:
    return None
  else:
    print(str(print_path(graphDict,source,target.parent))+"->")
    print(target.key)

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
          #print("node: "+str(curr_val))
          for adj in inpGraph[curr_val]: # each node connected to the curr_node
            if adj not in discovered and adj not in mainQ:
              #print("->"+str(adj))
              node = GraphNode(adj,discovered[curr_val],discovered[curr_val].depth+1)   #create node,assign its parent,depth
              discovered[adj] = node
              mainQ.append(adj)
        else:
          print("source does not exist")
      #for k,v in discovered.items():
        #print(str(k)+":"+str(v.depth))
      return discovered

def depth_first_search(inpGraph):       
  def dfs_visit(vtx,time,visited):
    for j in inpGraph[vtx.key]:             #get adjacent nodes
      if j not in visited:
        time+=1
        visited.add(j)
        curr_node = GraphNode(j,vtx,startTime = time)
        time = dfs_visit(curr_node,time,visited) + 1
        #time+=1
        curr_node.endTime = time
        dfs_tree[j] = curr_node
    return time

  time = 0              #time stamp for node
  dfs_tree = {}
  visited = set()       #visited nodes

  for i in inpGraph:
    if i not in visited: 
      visited.add(i)
      time += 1
      print(time)
      node = GraphNode(i,None,startTime = time)
      time = dfs_visit(node,time,visited)+1
      #time +=1
      node.endTime = time
      dfs_tree[i] = node

  for t in dfs_tree:
    if dfs_tree[t].parent != None:
      print "key: "+str(dfs_tree[t].key)+" | parent: "+str(dfs_tree[t].parent.key)+" | startTime: "+str(dfs_tree[t].startTime)+" | "+" endTime: "+str(dfs_tree[t].endTime)
    else:
      print "key: "+str(dfs_tree[t].key)+" | parent: "+str(dfs_tree[t].parent)+" | startTime: "+str(dfs_tree[t].startTime)+" | "+" endTime: "+str(dfs_tree[t].endTime)