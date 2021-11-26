class node:
    # this class infor testing
    # Do Not Modify
    def __init__(self, value):
        self.value = value
        # 指到自己
        self.right = self
        self.left = self
    def __repr__(self):
        return 'Node{}'.format(self.value).strip()

# Time complexity has to be O(1). 
# You shouldn't use len() method.

# bottom 在右側，左側推入
class stack:
    def __init__(self):
        self.num_element = 0
        self.root = node(None)
    
    def pop(self):
        if self.num_element == 0:
            raise ValueError('Cannot execute pop() on an empty stack')
        else:
            self.root.left = self.root.left.left
            self.root.left.right = self.root
            self.num_element -= 1
            """
            self.root.right = self.root.right.right # root的右手指向他右邊那顆node的右手的指向
            self.root.right.left = self.root
            """
            # todo:
            # Connect the second last element >> root
            # Connect root >> the second last element 
    def push(self, n):
        temp = node(n)
        self.root.left.right = temp
        temp.left = self.root.left
        self.root.left = temp
        temp.right = self.root
        self.num_element += 1
        """
        self.root.right.left = temp
        temp.right = self.root.right
        self.root.right = temp
        temp.left = self.root
        """
        
        # Connect the last element >> inserted node
        # Connect the inserted node >> root

    def __repr__(self):
        # 直接輸入對象(a)本身等同於在螢幕上打印出repr(a)
        # 使用print(a)時等於打印出str(a)
        ret = ''
        node = self.root.right
        while node != self.root:
            ret  = ret + '>>' + str(node)
            node = node.right
        return ret
# Queue是正的！
class queue:
    def __init__(self):
        self.num_element = 0
        self.root = node(None)

    def pop(self):
        if self.num_element == 0:
            raise ValueError('Can not execute pop() on an empty queue')
        else:
            self.num_element -= 1
            self.root.right = self.root.right.right
            self.root.right.left = self.root
            # todo:
            # Connect the second element >> root
            # Connect root >> the second element 
    def push(self, n):
        self.num_element += 1
        temp = node(n)
        self.root.left.right = temp
        temp.left = self.root.left
        self.root.left = temp
        temp.right = self.root

    def __repr__(self):
        ret = ''
        node = self.root.right
        while node != self.root:
            ret  = ret + '>>' + str(node)
            node = node.right
        return ret


