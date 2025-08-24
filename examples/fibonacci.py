def fibonacci(n):
   if n <= 1:
      return n
   return fibonacci(n - 1) + fibonacci(n - 2)

def fibonacci_iterative(n):
   if n <= 1:
      return n
   
   prev1 = 1
   prev2 = 0
   curr = 0

   for i in range(n, 1, -1):
      curr = prev1 + prev2
      prev2 = prev1
      prev1 = curr
   return curr

def func(n):
   return n

for i in range(0, 2 ** 20):
   func(i)

fibonacci(20)
# fibonacci_iterative(20)
