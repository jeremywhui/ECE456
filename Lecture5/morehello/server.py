# echo-server.py

import socket

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 9000 # Port to listen on (non-privileged ports are > 1023)
HELLO = "Hello from Python server"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    while True:
        conn, addr = s.accept()
        with conn:
            #print(f"Connected by {addr}")
            data = conn.recv(1024)
            if not data:
                break
            print(data.decode())
            conn.sendall(HELLO.encode())
