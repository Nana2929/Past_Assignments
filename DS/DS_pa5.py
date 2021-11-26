# -*- coding: UTF-8 -*-
import argparse
# 6/25 hw5 revision: add one line: break
class Node():
    def __init__(self, key):
        self.value = key
        self.next = None
class SLList():
    def __init__(self):
        self.head = None
    def insert(self, key):
        if not self or not self.head:
            # not self.head
            # 微尷尬的寫法，但要是曾經建立過linked list但是裡面的元素被刪光會留下空頭，
            # 所以重建head
            self.head = Node(key)
        else:
            insert_node = Node(key)
            insert_node.next = self.head
            self.head = insert_node
    """
    skip
    """
    def insert_back(self, key):
        if not self:
            self.head = Node(key)
        else:
            current = self.head
            prev = None
            while current:
                prev = current
                current = current.next
            prev.next = Node(key)

    def search(self, key):
        if not self or not self.head:
            return None
        else:
            current = self.head
            prev = None
            while current.value != key:
                prev = current
                current = current.next
                if not current: # if current == None
                    return None
            return True

    def delete(self, key):
        # print(self)
        if (not self) or (not self.head):
            return "Error"
        else:
            current = self.head
            prev = None
            while current.value != key:
                prev = current
                current = current.next
                if not current: # if current == None
                    return "Error"
            # current.value == key
            if prev != None:
                prev.next = current.next
            elif current == self.head and current.next:
                self.head = current.next
            else: #current == self.head and not current.next
                self.head = None
    """
    skip
    """
    def __repr__(self):
        ret = ''
        node = self.head
        while node != None:
            ret  = ret + ' ' + str(node.value)
            node = node.next
        return ret

class hash_table():
    """
    A hash table with 1001 slots.
    Hash function: key mod 1001.
    """
    def __init__(self):
        # implement a hash table with 1001 slots
        self.table = ["Null"]*1001

    def convert(self, key):
        """
        Test cases: 27-carry string.
        Convert test cases into 10-base integers.
        """
        listkey= list(key)
        ckey = 0
        count = 0
        for i in range(len(listkey), 0, -1):
            ckey += (27**count*(ord(listkey[i-1].lower())-96))
            count += 1
        return ckey

    def insert(self, key):
        convert_key = self.convert(key)
        # hash_key 是放置slot的編號
        hash_key = convert_key%1001
        if self.table[hash_key] == "Null":
            # 要放key原本input的格式
            x = SLList()
            x.insert(key)
            self.table[hash_key] = x
        else:
            # SLList的insert()會檢查自己的頭是不是 == None
            # 如果是一個屍體SSList，頭會重建
            self.table[hash_key].insert(key)
    def look(self, k):
        """
        k in {0, 1000}
        """
        if not (0 <= k <= 1000):
            return "List index out of range"
        if self.table[k] != "Null" and self.table[k].head != None:
            ret = ''
            node = self.table[k].head
            ret += self.table[k].head.value
            node =  self.table[k].head.next
            while node != None:
                ret  = ret + ' ' + str(node.value)
                node = node.next
            return ret
        else:
            return "Null"
    def search(self, key):
        convert_key = self.convert(key)
        hash_key = convert_key%1001
        if self.table[hash_key] == "Null":
            return "No"
        else:
            if self.table[hash_key].search(key):
                return "Yes"
            else:
                return "No"
    def delete(self, key):
        convert_key = self.convert(key)
        hash_key = convert_key%1001
        if self.table[hash_key] != "Null":
            Del = self.table[hash_key].delete(key)
            if Del == "Error":
                return "Error"
        else:
            return "Error"

def main(input_path, output_path):
    table = hash_table()
    output = open(output_path, 'w')
    with open(input_path) as f:
        for line in f.readlines():
            line = line.strip()
            if line.lower() != "end":
                op = line.split(" ")[0]
                key = line.split(" ")[1]
                if op.lower() == "insert":
                    table.insert(key)
                elif op.lower() == "look":
                    output.write(table.look(int(key)))
                    output.write("\n")
                elif op.lower() == "search":
                    output.write(table.search(key))
                    output.write("\n")
                elif op.lower() == "delete":
                    if table.delete(key) == "Error":
                        output.write("Error")
                        output.write("\n")
            else:
                output.close()
                # ver1. 忘記寫break...
                break

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
