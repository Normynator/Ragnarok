import socket
import threading

class ThreadedServer(object):
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))

    def listen(self):
        self.sock.listen(5)
        #while True:
        print("Waiting for incoming connection ...")
        self.client, self.address = self.sock.accept()
        #self.client.settimeout(60)
        threading.Thread(target = self.listenToClient,args =
                         (self.client,self.address)).start()


    def listenToClient(self, client, address):
        size = 1024
        while True:
            try:
                data = client.recv(size)
                if data:
                    # Set the response to echo back the recieved data
                    response = data
                    #client.send(response)
                    #callback("test")
                    #print(response)
                    callback(response)
                else:
                    raise error('Client disconnected')
            except:
                client.close()
                self.listen()
                return False

    def sendToClient(self, msg):
        try:
            self.client.send(str.encode(msg + '\0'))
        except:
            return

def callback(client_msg):
    #client_msg = "<client>: "+ client_msg
    print("<client>: ", client_msg)


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
    while True:
        msg = input("")
        server.sendToClient(msg)

