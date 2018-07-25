#fibonacci
from collections import deque
Q = deque()
def fibo(val):
  if val==0:
    return 0
  elif val ==1:
    return 1
  else:
    if not bool(Q):
      re1 = fibo(val - 2) 
      re2 = fibo(val-1)
    else:
      re1 = Q.popleft()
      if bool(Q):
        re2 = Q.popleft()
      else:
        re2 = 0
    Q.clear()
    Q.append(re2)
    re = re1+re2
    Q.append(re)
    return re