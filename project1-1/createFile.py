limit = 9000

with open('numbers.txt', 'w') as file:
    for i in range(1, limit + 1):
        file.write(f"{i}\n")