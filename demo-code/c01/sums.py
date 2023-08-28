print("sum1:")
sum1 = 0
for i in range(1, 1000000001):
    sum1 += 1/i
    if i % 100000000 ==0:
        print(i, sum1)

print("sum2:")
sum2 = 0
for i in range(1, 1000000001):
    sum2 += 1/i/i
    if i % 100000000 ==0:
        print(i, sum2)

