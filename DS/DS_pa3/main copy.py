import argparse
"""
ds pa3 final 
2020/5/9 
b06102020 
test case 3/5, score: 80 
5/24 修正條件缺失;修正錯誤變數名
"""
class BSTree_Node():
    def __init__(self, key):
        self.value = key
        self.left_child = None
        self.right_child = None
    def __repr__(self):
        return str(self.value)

class BSTree():
    def __init__(self):
        self.root = None

    def insert(self, key):
        if not self.root:
            self.root = BSTree_Node(key)
        else:
            curr = self.root 
            while curr:
                if curr.value > key:
                    if curr.left_child: 
                        curr = curr.left_child
                    else:
                        curr.left_child = BSTree_Node(key)
                        break
                elif  curr.value < key:
                    if curr.right_child:
                        curr = curr.right_child
                    else: 
                        curr.right_child = BSTree_Node(key) 
                        break
                else:
                    return "Node already exists"

    def search(self, key):
        # okay
        # return the node to be deleted and its parent
        if self.root:
            curr = self.root
            prev = None
            while curr.value != key:
                if key < curr.value and curr.left_child:
                    prev = curr
                    curr = curr.left_child
                elif  key > curr.value and curr.right_child:
                    prev = curr
                    curr = curr.right_child
                else:
                    return None
            return prev, curr 
            # del_paren, del_key = self.search(key) 
    def min_node(self):
        if self.root:
            curr = self.root
            prev = None
            while curr.left_child:
                prev = curr
                curr = curr.left_child
        return prev, curr # return min_node 與其parent node 
    
    
    def delete(self, key):
        if self.root: 
            if not self.search(key):
                return None
            else:
                del_paren, del_node = self.search(key)
                # print(del_paren, del_node)
                # delete對象為root
                if self.root == del_node:
                    if self.root.right_child and self.root.left_child:
                        rr = self.root.right_child
                        RR = BSTree()
                        RR.insert(rr.value)
                        RR.root.left_child = rr.left_child
                        RR.root.right_child = rr.right_child
                        Min = RR.min_node()[1]
                        self.delete(Min.value)
                        self.root.value = Min.value   
                                 
                    # root只有一邊的樹
                    elif self.root.right_child:
                        self.root = self.root.right_child
                    elif self.root.left_child:
                        self.root = self.root.left_child
                    # root沒有subtrees
                    else:
                        self.root = None
                # delete對象並非root
                else:
                    # case 1 delete對象為leaf node
                    if not del_node.left_child and not del_node.right_child:
                        if del_node.value > del_paren.value:
                            del_paren.right_child = None      
                        else:
                            del_paren.left_child = None
                        """
                        with open ("output_", "w") as f:
                            print("after deleting", del_node.value)
                            self.inorder(f)
                            print("---")
                        """
                    # case 2-1 one subtree: Promote the whole subtree 
                    elif del_node.left_child and not del_node.right_child:
                        if del_node.value > del_paren.value:
                            del_paren.right_child = del_node.left_child
                        else:
                            del_paren.left_child = del_node.left_child
                    # case 2-2 one subtree: Promote the whole subtree 
                    elif del_node.right_child and not del_node.left_child:
                        if del_node.value > del_paren.value:
                            del_paren.right_child = del_node.right_child
                        else:
                            del_paren.left_child = del_node.right_child
                    # case 3
                    else: # both subtrees
                        dr = del_node.right_child
                        DR = BSTree()
                        DR.insert(dr.value)
                        DR.root.left_child = dr.left_child 
                        DR.root.right_child = dr.right_child 
                        Min = DR.min_node()[1]
                        self.delete(Min.value)
                        del_node.value = Min.value
        
             
    def inorder(self, output):
        # okay
        if self.root:
            if self.root.left_child:
                lc = self.root.left_child
                l = BSTree()
                l.insert(lc.value)
                l.root.left_child = lc.left_child
                l.root.right_child = lc.right_child
                l.inorder(output)
            # print(str(self.root.value)+" ")
            output.write(str(self.root.value)+" ")
            if self.root.right_child:
                rc = self.root.right_child
                r = BSTree()
                r.insert(rc.value)
                r.root.left_child = rc.left_child
                r.root.right_child = rc.right_child
                r.inorder(output)
        

    def preorder(self, output):
        # okay
        if self.root:
            output.write(str(self.root.value)+" ")
            if self.root.left_child:
                lc = self.root.left_child
                l = BSTree()
                l.insert(lc.value)
                l.root.left_child = lc.left_child 
                l.root.right_child = lc.right_child 
                l.preorder(output)
            if self.root.right_child:
                rc = self.root.right_child
                r = BSTree()
                r.insert(rc.value)
                r.root.left_child = rc.left_child 
                r.root.right_child = rc.right_child 
                r.preorder(output)

def main(input_path, output_path):
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # DO NOT MODIFY CODES HERE
    # It's important and repeat three times
    bstree = BSTree()
    output = open(output_path, 'w')
    with open(input_path) as f:
        for line in f.readlines():
            line = line.strip()
            if line.lower() == 'inorder':
                bstree.inorder(output)
                output.write("\n")
            elif line.lower() == 'preorder':
                bstree.preorder(output)
                output.write("\n")
            else:
                operation = line.split(' ')[0]
                key = int(line.split(' ')[1])
                if operation.lower() == 'insert':
                    bstree.insert(key)
                elif operation.lower() == 'delete':
                    bstree.delete(key)
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

