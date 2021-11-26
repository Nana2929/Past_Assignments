'''
Linear Algebra
'''
def printmat(M):
    # ROWLEN = len(M[0])
    if not isinstance(M, list):
        print("[ERROR] Matrix type is problematic(lists of list).")
        return
    for rowidx, row in enumerate(M):
        if rowidx == 0:
            print("[",end = '')
            print(*row , sep = ' , ')
        elif rowidx == len(M)-1:
            print(*row , sep = ' , ', end = '')
            print("]")
        else:
            print(" ", end = '')
            print(*row , sep = ' , ')

def stringmat(M):
    # ROWLEN = len(M[0])
    if not isinstance(M, list):
        print("[ERROR] Matrix type is problematic(lists of list).")
    string = ''
    for rowidx, row in enumerate(M):
        row = list(map(str, row))
        if rowidx == 0:
            string += "[ "
            string+= ' , '.join(row)+'\n'
        elif rowidx == len(M)-1:
            string+= "  "
            string+= ' , '.join(row)
            string += " ]"
        else:
            string += '  '
            string += ' , '.join(row)+'\n'
    return string


def dot(v1, v2, toprint = 0):
    if len(v1) != len(v2):
        return "[ERROR] Vector sizes mismatch."
    conj_v2 = [ele.conjugate() for ele in v2]
    elementwise_product = [xi*yi for xi,yi in zip(v1, conj_v2)]
    dot = sum(elementwise_product)
    if toprint:
        print("first vector:", v1)
        print("second vector:", conj_v2)
        print("elementwise product:", elementwise_product)
        print("dot product:", dot.real if dot.real == dot else dot)
    if dot.real == dot:
        return dot.real
    return dot

def norm(v):
    return dot(v, v)**0.5

def matmul(M1, M2):
    # matrix multiplication
    # m1, m2 = two vectors of vectors
    if len(M1[0]) != len(M2):
        print("[ERROR] Matrix sizes mismatch.")
    dim1 = len(M1)
    dim2 = len(M2[0])
    Z = [[0 for i in range(dim2)]for j in range(dim1)]
    # print(dim1, dim2)
    for i in range(len(M1)):
        for j in range(len(M2[0])):
            for k in range(len(M2)):
                Z[i][j] += M1[i][k] * M2[k][j]
    return Z

def det(A):
    '''
    calculate determinant for matrix A
    recursive
    '''
    import copy
    # check sqr matrix
    for i in range(len(A)):
        assert len(A) == len(A[i])
    # base case
    if len(A) == len(A[0])== 2:
        return A[0][0] * A[1][1] - A[0][1]*A[1][0]
    # recursive steps
    indices = [i for i in range(len(A))]
    detA = 0
    for idx in indices:
        submatrix = copy.deepcopy(A)
        submatrix = submatrix[1:] # expand on the first row (any row/column is ok)
        size = len(A)-1
        for j in range(size): # height
            # 抽換每個row裡column的內容
            submatrix[j] = submatrix[j][0:idx]+ submatrix[j][idx+1:]
        sign = (-1)**(idx+2)
        cofactor = sign * det(submatrix)
        detA += A[0][idx] * cofactor
        # print("submatrix: ", sign, A[0][idx])
        # printmat(submatrix)
    return detA
