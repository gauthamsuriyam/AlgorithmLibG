#maximum subarray problem
#max crossing subarray
def max_crossing_subarray(ipArray,low,mid,high):
  left_max_sum = None
  max_sum = 0
  for i in range(mid,low-1,-1):
    max_sum = max_sum + ipArray[i]
    if max_sum > left_max_sum:
      left_max_sum = max_sum
      left_max_low = i
    
  right_max_sum = None
  max_sum = 0
  for i in range(mid+1,high+1):
    max_sum = max_sum + ipArray[i]
    if max_sum > right_max_sum:
      right_max_sum = max_sum
      right_min_high = i

  return left_max_sum+right_max_sum,left_max_low,right_min_high
  
    
#max subarray
def max_subarray(ipArray, low, high):
	if low == high:
		return ipArray[low],low,high
	else:
		#print(ipArray)
		mid = (low + high) // 2
		print("mid pt: " + str(mid))

		left_max, left_max_low, left_min_high = max_subarray(ipArray, low, mid)

		right_max, right_max_low, right_min_high = max_subarray(ipArray, mid + 1, high)

		#max crossing subarray
		crossing_max, crossing_max_low, crossing_min_high = max_crossing_subarray(ipArray,low,mid,high)

		if left_max >= right_max and left_max >= crossing_max:return left_max, left_max_low, left_min_high
		elif right_max >= crossing_max:return right_max, right_max_low, right_min_high
		else:return crossing_max,crossing_max_low, crossing_min_high
  

A = [-2, -3, 4, -1, -2, 1, 5, -3]
maxVal, maxlow, minhigh = max_subarray(A, 0, len(A)-1)
print("maxVal: " + str(maxVal) + " low: " + str(maxlow) + " high: " +str(minhigh))