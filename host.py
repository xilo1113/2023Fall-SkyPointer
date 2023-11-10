import socket
import json
import numpy as np
HOST = "192.168.249.116" # IP address
PORT = 6543 # Port to listen on (use ports > 1023)


class STMSocket:
    """
    Define socket used to connect to STM
    """
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self, host, port):
        self.socket.bind((host, port))
        self.socket.listen()
        conn, addr = self.socket.accept()
        return (conn, addr)
    
    def send(self, altitude, azimuth):
        raise NotImplementedError
# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
#     s.bind((HOST, PORT))
#     s.listen()
#     print("Starting server at: ", (HOST, PORT))
#     conn, addr = s.accept()
#     with conn:
#         print("Connected at", addr)
#         while True:
#             data = conn.recv(1024).decode('utf-8')
#             print("Received from socket server:", data)
#             if (data.count('{') != 1):
#                 # Incomplete data are received.
#                 choose = 0
#                 buffer_data = data.split('}')
#                 while buffer_data[choose][0] != '{':
#                     choose += 1
#                 data = buffer_data[choose] + '}'
#             obj = json.loads(data)
#             t = obj['s']