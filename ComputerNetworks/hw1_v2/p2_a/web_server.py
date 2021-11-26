# -*- coding: utf-8 -*-
'''
ICN HW1(2)
usage:
    cd /Users/yangqingwen/Desktop/110-1課程/電網導/hw1_src/p2_a
    run the server (run the python program with cmd: python3 web_server.py)
    open your browser and go to link http://127.0.0.1:8888/index.html
    click the link in request of another page "helloword.html"
(server times out when the timer of 30 mins is up)
(http://127.0.0.1:8888 returns 404 Not Found because chrome browser requests /favicon when pressing multiple enters)
(Don't run this with jupyter notebook open; they share the same default localhost and port)
Notes: send() -> client, print() on server terminal
'''
from socket import *
import time
import sys # In order to terminate the program
import time

ROOT = "./" # MAKE SURE TO RUN UNDER THE RIGHT DIR
TIME = 1800
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
HOST, PORT = "127.0.0.1", 8888
serverSocket.bind((HOST, PORT))
# hostname = gethostname()
# print("Host IP:", gethostbyname(hostname))  # [Errno 8] nodename nor servname provided, or not known
start = time.time()
serverSocket.settimeout(TIME)
while True:
    #Establish the connection
    print(f'* Listening on {HOST}:{PORT}...')
    serverSocket.listen(10)
    print('* Ready to serve...')
    try:
        conn, address = serverSocket.accept()
        print("* Client address:", address)
    except timeout: # socket.timeout
        print("* Notification: Server Timeout")
        break
    try:
        # Receive http request from the client
        message = conn.recv(1024).decode()
        msg_sp = message.split(' ')
        print("* Request msg:", msg_sp)
        filename = 'index.html'
        if msg_sp[0] == 'GET':
            filename = msg_sp[1].strip()
            if filename == '/' or '':
                filename = 'index.html' # default
        # print(msg_sp)
        print("* Request Filename:", filename)
        with open(ROOT+filename) as f:# Read data from the file that the client requested
            outputdata = f.readlines()
        headerline = "HTTP / 1.0 200 OK\r\n"
        content_type = 'Content-Type: text/html\r\n\r\n'
        finalmsg = headerline+content_type
        conn.sendall(finalmsg.encode())
        for i in range(len(outputdata)):
            print(outputdata[i])
            conn.send(outputdata[i].encode())
    except IOError:
        headerline = "HTTP / 1.0 404 Not Found\
        \r\n\r\n<body><center><b>404 Not Found</b></center><body>"
        conn.send(headerline.encode())
        print(f"* Warning: {filename} not found")
        print(headerline)
        # Send response message for file not found
    print("------------------------------------")
    # conn.close()
serverSocket.close()
sys.exit() #Terminate the program after sending the corresponding data
