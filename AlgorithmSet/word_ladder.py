#word_ladder
#Given two words (beginWord and endWord), and a dictionary's word list, find the length of shortest transformation sequence from beginWord to endWord, such that:

#1. Only one letter can be changed at a time.
#2. Each transformed word must exist in the word list. Note that beginWord is not a transformed word.

#create a vocab dictionary with letter pos as key and uniq letters as values

from collections import deque

#create nodes by changing one letter of the beginWord with each letter in the vocab set at designated positions

def begin_search(start_word,end_word,word_list): #bfs
  vocab = {}
  
  #create a vocab dicionary
  for l in range(len(word_list[0])):
    dum = set()
    for word in word_list:
      dum.add(word[l])
    vocab[l]= dum
    
  word_list = set(word_list)
  print(start_word)
  discovered = {}
  searched = set()
  mainQ = deque()             #queue for current node
  discovered[start_word] = 1  # level of curr_word
  mainQ.append(start_word)
  searched.add(start_word)
  #print(searched)
  
  while len(mainQ)!=0:
    curr_word = mainQ.popleft()
    #begin search for next word/node: Edited into generators based on example by *some_member
    def create_node():  #create word/node
      for pos,set_val in vocab.items():
        for val in set_val:
          temp = curr_word[:pos]+val+curr_word[pos+1:]
          if temp in word_list and temp not in searched: 
            yield temp
    for nodeWord in create_node():
      print("-->"+str(nodeWord))

      discovered[nodeWord] = discovered[curr_word]+1 #add word and its depth
      searched.add(nodeWord)
      if nodeWord == end_word:  # check if key node found
        return discovered[end_word]
      else:
        mainQ.append(nodeWord)    #add word to queue        
  return 0