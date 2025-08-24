def factorial(n):
   if n <= 1:
      return n
   return n * factorial(n - 1)

def factorial_iterative(n):
   if n <= 1:
      return n
   
   for i in range(n - 1, 1, -1):
      n *= i
   return n

# factorial(15)
factorial_iterative(15)
