# -*- coding: utf-8 -*-
import socket

HOST, PORT ="127.0.0.1", 8888
# Test on TA's Server: HOST, PORT ="140.112.42.108", 7777
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while True:
    server_msg = s.recv(1024).decode()
    print(server_msg)
    # add:
    if "failed" in server_msg:
        break
    string = input('(C)Input:')
    s.send(string.encode())
    if string.lower() in ["no", "n"]:
        server_msg = s.recv(1024).decode()
        print(server_msg)
        break  # test correct
