import argparse
import time
# import sys
# sys.path.append("/Users/yangqingwen/Desktop/python_training/Data_Structure/ds_pa1/struct.py")
from struct import node, stack, queue

# import pandas as pd

def main(struct, input_file, output_file):
    if struct == 'queue':
        test = queue()
    else:
        test = stack()
    input = open(input_file)
    lines = input.readlines()
    output = open(output_file, 'w')
    # TODO: you should parse the input commands here
    for line in lines:
        if "POP" in line:
            test.pop()
        else:
            value = line.rstrip("\n")[5:]
            test.push(value)
        output.write(test.__repr__())
        output.write("\n")
    input.close()
    output.close()

    
if __name__ == '__main__':
    # Do Not Change the code here
    parser = argparse.ArgumentParser()
    parser.add_argument('--structure', choices=['queue', 'stack'], default='stack')
    # parser.add_argument('--input', default = "./MEOW.txt")
    parser.add_argument('--input', default='./input.txt')
    parser.add_argument('--output', default='./output.txt')
    args = parser.parse_args()
    ts = time.time()
    main(args.structure, args.input, args.output)
    te = time.time()
    print('Run Time: {:.5f}s'.format(te-ts))
