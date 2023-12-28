import socket
import sys
import time
import trans
HOST = "192.168.191.116"
PORT = 8872
t = 0
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server:
    print("Socket created")
    server.bind((HOST, PORT))
    print("Server started")
    server.listen()
    print("Waiting for connection...")
    conn, addr = server.accept()
    with conn:
        print("Connected by", addr)
        while True:
            starname = input("Please input the name of the star: ")
            altitude, azimuth = trans.get_ninjas_star(starname)
            serverMessage = str(altitude) + " " + str(azimuth)
            conn.sendall(serverMessage.encode())
print("Server closed")

