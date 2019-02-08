import socket
import threading

class ThreadedServer(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))
        self.already_displayed = 0

    def listen(self):
        self.sock.listen(5)
        print("Waiting for incoming connection ...")
        self.client, self.address = self.sock.accept()
        threading.Thread(target = self.listenToClient,args =
                         (self.client,self.address)).start()
        print("DLL connected to debug-server!")

    def listenToClient(self, client, address):
        size = 1
        while True:
            try:
                data = client.recv(size)
                payload_size = 0
                if data:
                    payload_size = int.from_bytes(data, byteorder="little")
                else:
                    raise Exception('client disconnected!')
                if payload_size:
                    data = client.recv(payload_size)
                    try:
                        print(data.decode('UTF-8'))
                    except:
                        if not self.already_displayed: 
                            print("Message can't be displayed in UTF-8. This warning is displayed once, now silent!")
                            self.already_displayed = 1
                else:
                    raise Exception('payload size failed!')
            except:
                client.close()
                self.listen()
                return False


if __name__ == "__main__":
    while True:
        port_num = input("Port? ")
        try:
            port_num = int(port_num)
            break
        except ValueError:
            pass

    server = ThreadedServer('127.0.0.1',port_num)
    server.listen()



