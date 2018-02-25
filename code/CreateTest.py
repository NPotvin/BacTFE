import random
import sys

def createTest(fileName, length):
    file = open(fileName, 'w')
    random.seed()
    for i in range(length):
        file.write("{}\n".format(random.randint(0,131071)))
    file.close()

if __name__ == '__main__':
    file = "array.txt"
    length = 2**20
    if len(sys.argv)>1:
        file, length = sys.argv[1], int(sys.argv[2])
    createTest(file, length)