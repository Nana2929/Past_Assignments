import argparse

class Heap_Node():
    def __init__(self, key):
        self.value = key

    def __repr__(self):
        return 'Heap_Node({})'.format(str(self.value))

    def __gt__(self, other):
        if self.value > other.value:
            return True
        else:
            return False

    def __lt__(self, other):
        if self.value > other.value:
            return False
        else:
            return True

class MinHeap:
    def __init__(self):
        self.heapList = [0]
        self.currentSize = 0

    def __repr__(self):
        return str(self.heapList)

    def insert(self, node):
        Inserted = node
        # insert a node instead of a key
        self.heapList.append(Inserted)
        # O(1) access to last node no.
        self.currentSize += 1
        i = self.currentSize
        curr = Inserted
        while curr:
            parent = self.heapList[i//2]
            if parent != 0: # heapList[0] is an int 
                if curr.__lt__(parent):
                    # print(i,"curr",curr.value, "pare", parent.value)
                    temp = curr.value
                    curr.value = parent.value
                    parent.value = temp
                else:
                    break
            curr = parent
            i = i//2

    def delMin(self):
        lastNode = self.heapList[self.currentSize]
        temp = self.heapList[1].value
        del_node = Heap_Node(temp)
        self.heapList[1].value = lastNode.value
        # default: pop the last element
        self.heapList.pop()
        # Percolate Down
        self.currentSize -= 1
        i = 1
        # print(self.heapList)
        if self.heapList != [0]:
            curr= self.heapList[1] # start from root 
            while curr:
                # two children 
                if 2*i+1 <= self.currentSize:
                    left  = self.heapList[2*i]
                    right = self.heapList[2*i+1]
                    if curr.__gt__(left) or curr.__gt__(right):
                        if curr.__gt__(left) and not curr.__gt__(right):
                            temp = curr.value
                            curr.value = left.value
                            left.value = temp
                            curr = left
                            i = 2*i
                        elif not curr.__gt__(left) and curr.__gt__(right):
                            temp = curr.value
                            curr.value = right.value
                            right.value = temp
                            curr = right
                            i = 2*i+1
                        elif  curr.__gt__(left) and curr.__gt__(right):
                            if right.__lt__(left):
                                temp = curr.value
                                curr.value = right.value
                                right.value = temp
                                curr = right
                                i = 2*i+1
                            elif left.__lt__(right):
                                temp = curr.value
                                curr.value = left.value
                                left.value = temp
                                curr = left
                                i = 2*i
                    else:
                        break
                # one child: only one node can have one child in a heap (a heap is almost fully bloom)
                elif 2*i == self.currentSize:
                    left = self.heapList[2*i]
                    if curr.__gt__(left):
                        temp = curr.value
                        curr.value = left.value
                        left.value = temp
                        curr = left
                        i = 2*i
                    else:
                        break
                # no child 
                else:
                    break
        # print(self.currentSize, del_node)
        return del_node

def main(input_path, output_path):
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # It's important and repeat three times
    minheap = MinHeap()
    output = open(output_path, 'w')
    with open(input_path) as f:
        for line in f.readlines():
            line = line.strip()
            if line.lower() == 'print':
                print(minheap, file=output)
            else:
                operation = line.split(' ')[0]
                if operation.lower() == 'insert':
                    key = int(line.split(' ')[1])
                    minheap.insert(Heap_Node(key))
                elif operation.lower() == 'delmin':
                    print(minheap.delMin(), file=output)
                else:
                    raise NotImplementedError
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

    main(args.input, args.output)


