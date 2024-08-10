from socket import *

tcp_socket = socket(AF_INET, SOCK_STREAM)
# 服务端的IP地址和使用的接口
serve_ip = "172.20.10.2"
serve_port = 8000
tcp_socket.connect((serve_ip, serve_port))

while True:
    data = tcp_socket.recv(1)
    print(f"Received: {data.decode()}")

tcp_socket.close()