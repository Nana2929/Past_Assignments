# -*- coding: utf-8 -*-
'''
ICN HW1(1)
usage:
    open two terminals
    run socket_server.py and socket_client.py respectively
    input format: 1 + 5 (space separate)
    exception: y/n not typed correctly: default is y
    exception: wrong input format leads to socket closure
'''
import socket
import ast
import LA # self lib
# UDP: socket.socket(socket.AF_INET, socket.SOCK_DGRAM) datagram
def main():
    mysocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # AF_INET 對外 AF:UNIX 本機內部
    # TCP; data stream
    mysocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    HOST, PORT = "127.0.0.1", 8888 # localhost
    mysocket.bind((HOST, PORT)) # sudo lsof -i:8888
    while(True):
        # Listen for any request
        # TODO start
        mysocket.listen(10) # value 10 specifies the max number of sockets accepted by the server
        # TODO end
        print("The Grading server for HW1 is running..")
        while(True):
            # Accept a new request and admit the connection
            # TODO start
            client, address = mysocket.accept()
            # when you accept, you need to change all socket names to client_socket
            # TODO end
            print(str(address)+" connected")
            # b-prefix: bytes object
            try:
                while (True):
                        client.send(b"(S) Welcome to the calculator server.\n    Supported operations: (Real) +, -, *, /, %, matmul, dot, norm, determinant.\n\
    Input your problem ?\n")
                        data = client.recv(1024).decode() # the data transmitted is byte object!
                        datas = data.strip().split(' ')
                        print("Server received: ", datas)
                        isSCALAR = False
                        if len(datas) == 3:
                            try:
                                isSCALAR = True
                                [a, op, b] = datas
                                a = float(a)
                                b = float(b)
                                ans = 0
                                if op == '+':
                                    ans = a+b
                                elif op == '-':
                                    ans = a-b
                                elif op == '*':
                                    ans = a*b
                                elif op == '/':
                                    ans = a/b
                                elif op == '%':
                                    ans = a%b
                                # inner product
                                # [vec] . [vec]
                            except:
                                if op == '.':
                                    a = ast.literal_eval(datas[0])
                                    b = ast.literal_eval(datas[2])
                                    ans = LA.dot(a, b)
                                elif op == '*':
                                    isSCALAR = False
                                    A = ast.literal_eval(datas[0])
                                    B = ast.literal_eval(datas[2])
                                    ans = LA.matmul(A,B)
                        elif len(datas) == 2:
                            isSCALAR = True
                            # norm [vector]
                            op = datas[0]
                            if  op == 'norm':
                                vec = datas[1]
                                a = ast.literal_eval(vec)
                                ans = LA.norm(a)
                            # det [[matrix]]
                            elif op == 'det':
                                A = ast.literal_eval(datas[1]) # a list of list matrix
                                ans = LA.det(A)
                        # OUTPUT
                        if isSCALAR:
                            if int(ans) == ans:
                                outmsg = str("(S) The answer is %d.\n\
    Do you have any more questions (y/n)?\n"%(ans)).encode()
                            else: outmsg = str("(S)The answer is %.4f.\n\
    Do you have any more questions (y/n)?\n"%(ans)).encode()
                        else:
                            ans = LA.stringmat(ans) # matrix to string
                            outmsg = ("(S) The answer is\n%s.\nDo you have any questions (y/n)?\n"%(ans)).encode()
                        client.send(outmsg)
                        reply = client.recv(1024).decode() # bytes
                        if reply.lower() in ["no", "n"]:
                            client.send(b"(S) The client socket is closing...\
    \nThanks for utilizing our service.")
                            break
            except:
                client.send(b"(S) Exception: failed to execute (check extra whitespaces, matrix sizes, etc.).\n\
    Supported operations: (Real) +, -, *, /, %, matmul, dot, norm, determinant\n\
    The client socket is closing...")
                # print("Exception: failed to execute.") # ~
            client.close()

if __name__ == '__main__':
    main()