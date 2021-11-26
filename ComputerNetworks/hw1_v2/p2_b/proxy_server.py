# -*- coding: utf-8 -*-
'''
ICN hw1(3)
proxy server of hw1(2)
(to prevent direct access to our REAL dynamic address)
Run proxy_server.py along with web_server.py in p2_a
Open PROXY_HOST:PORT/{filename} (eg. http://127.0.0.1:7776/helloworld.html)
Note that  http://127.0.0.1:{PORT} may return 404 Not Found or illegal request
since the chrome browser automatically send nonsense requests
'''
from socket import *
import sys
import select
import time
if len(sys.argv) <= 1:
    print('Usage : "python ProxyServer.py server_ip"\n\
    [server_ip : the IP Address Of Proxy Server],\n\
    eg.python3 proxy_server.py 127.0.0.1 7777')
    sys.exit(2)
TIME = 1800
PROXY_HOST, PORT = sys.argv[1], int(sys.argv[2]) # set it to 7776
print('Host, Port:', PROXY_HOST,',',PORT)
# tcpSerSock: proxy server
tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
tcpSerSock.bind((PROXY_HOST, PORT))
tcpSerSock.listen(10)
tcpSerSock.settimeout(TIME)
while True:
    # Strat receiving data from the client
    print('* Ready to serve...')
    try:
        tcpCliSock, addr = tcpSerSock.accept()
        print('* Received a connection from:', addr)
        # Receive request from the client
        message = tcpCliSock.recv(1024).decode()
        print("* Message from client:", message)
    except timeout:
        print("* Notification: Server Timeout")
        break
    # Extract the filename from the given message

    try:
        filename = message.split()[1].partition("/")[2]
        if filename.strip() == '':
            filename = 'index.html'
    except:
        filename = 'index.html'
    print("* Request filename:", filename)
    fileExist = "false"
    filetouse = "/" + filename
    # Case 1: File in the cache
    try:
        with open (filetouse[1:], "r") as f:
            outputdata = f.read()
        # ProxyServer finds a cache hit and generates a response message
        # Send the file data to the client
        # TODO start.
        fileExist = "true"
        tcpCliSock.send(b"HTTP/1.1 200 OK \r\n") # Header for status
        tcpCliSock.send(b"Content Type:text/html\r\n\r\n")
        tcpCliSock.send(outputdata.encode())
        print("File read from cache.")
        # TODO end.
    # Case1: Error handling for file not found in cache
    except IOError:
        if fileExist == "false":
            # connection socket used for proxy server
            c = socket(AF_INET, SOCK_STREAM)
            hostn = filename.replace("www.", "", 1)
            try:
                WEB_HOST, WEB_PORT = "127.0.0.1", 8888
                c.connect((WEB_HOST, WEB_PORT))

                print("Proxy successfully connected.")
                # Create a temporary request file on this socket and ask webserver port for the file requested by the client
                print("* Request file to use:", filetouse)
                c.setblocking(0)
                fileobj = c.makefile('rw', None, encoding = 'utf-8')
                fileobj.write("GET "+ filetouse + " HTTP/1.0\n\n")
                fileobj.flush()
                req = fileobj.readline()
                print("* Request:", req)

                # STUCK HERE
                c.send(req.encode())
                # Read the response into buffer
                buffer = []
                while select.select([c], [], [], 0.5)[0]:
                    time.sleep(0.1)
                    ws_response = c.recv(8192).decode()
                    ws_response = ws_response.split('\n')
                    for i in range(len(ws_response)):
                        buffer.append(ws_response[i]) # skip the header status
                for t in buffer:
                    if "DOCTYPE" in t:
                        break
                    buffer = buffer[1:]

                print(buffer)
                # Also send the response in the buffer to client socket and the corresponding file in the cache
                if len(buffer) > 0:
                    fileExist = "true"
                    tcpCliSock.send(b"HTTP/1.1 200 OK \r\n") # Header for status
                    tcpCliSock.send(b"Content Type:text/html\r\n\r\n")
                    tcpCliSock.sendall('\n'.join(buffer).encode())
                    # Create a new file in the cache for the requested file.
                    with open(filename, 'w') as cachef:
                        cachef.write('\n'.join(buffer))
                else:
                    tcpCliSock.send(b"HTTP/1.1 404 Not Found\
                    \r\n\r\n<body><center><b>404 Not Found</b></center><body>")
                    print(f'{filename} not found')
            except: # file not found, ...
               print("Illegal request")
            c.close()
    # Close the client sockets
    tcpCliSock.close()
# Close the server socket
# TODO start.
tcpSerSock.close()
sys.exit()
# TODO end.
