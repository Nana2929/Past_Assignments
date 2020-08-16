import argparse
import time

# 參考：https://www.youtube.com/watch?v=l3hda49XcDE

def pattern_match(text, pattern):
    """
    RegEx Matching
    Algorithm Design:
    m,n = len(text), len(pattern)
    1. Create a m * n array to store boolean values.
    2. Set Array[0][0] as True.("" = "")
    3. Determine the value of the first row, note that a*, a*b*, a*b*c*... can equal to "". 
    4. for i* for j nested-loop: 
        (1)If t[i-1]== p[j-1] or p[j-1] == ".", Array[i][j] = Array[i-1][j-1]
        (2)If p[j-1] == "*",考慮三種情況。
    5. return Array[m][n]. 
    """
    if pattern == ".*":
        return True
    Array = [[False]* (len(pattern)+1) for _ in range(len(text)+1)]
    Array[0][0] = True
    for j in range(1, len(pattern)+1):
        if pattern[j-1] == "*" :
            Array[0][j] = Array[0][j-2]
    for i in range(1, len(text)+1):
        for j in range(1, len(pattern)+1):
            # index-1
            if text[i-1] == pattern[j-1] or pattern[j-1] == ".": 
                Array[i][j]= Array[i-1][j-1] 
            elif pattern[j-1] == "*": 
                Array[i][j] = Array[i][j-2] or (Array[i-1][j] and (pattern[j-2] == text[i-1] or pattern[j-2] =='.'))
                    
    return Array[-1][-1] 
te, pat = "aaa", "ab*a*c*a"
print(pattern_match(te, pat))
 

def main(input_path, output_path):
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # It's important and repeat three times
    output = open(output_path, 'w')
    with open(input_path) as f:
        for line in f.readlines():
            line = line.strip().split()
            text, pattern = line[0], line[1]
            if pattern_match(text, pattern):
                print('1', file=output)
            else:
                print('0', file=output)
    output.close()

if __name__ == '__main__':
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # It's important and repeat three times
    parser = argparse.ArgumentParser()
    parser.add_argument('--input', default='./input')
    parser.add_argument('--output', default='./output')
    args = parser.parse_args()

    ts = time.time()
    main(args.input, args.output)
    te = time.time()
    print('Run Time: {:.5f}s'.format(te-ts))

