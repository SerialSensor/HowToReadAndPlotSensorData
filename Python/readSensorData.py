import socket
import struct
import errno

# General config
UDP_IP = "192.168.0.0"  # Change to your ip
UDP_PORT = 8888
MESSAGE_LENGTH = 13  # one sensor data frame has 13 bytes

# Prepare UDP
print("This PC's IP: ", UDP_IP)
print("Listening on Port: ", UDP_PORT)
sock = socket.socket(socket.AF_INET,
                     socket.SOCK_DGRAM)
sock.setblocking(False)
sock.bind((UDP_IP, UDP_PORT))

# Read data forever
while True:
    # Get latest data
    keepReceiving = True
    newestData = None
    while keepReceiving:
        try:
            data, fromAddr = sock.recvfrom(MESSAGE_LENGTH)
            if data:
                newestData = data
        except socket.error as why:
            if why.args[0] == errno.EWOULDBLOCK:
                keepReceiving = False
            else:
                raise why
    if newestData is not None:
        print("received data: ",
              "%1.6f" % struct.unpack_from('<f', newestData, 1),
              "%1.6f" % struct.unpack_from('<f', newestData, 5),
              "%1.6f" % struct.unpack_from('<f', newestData, 9))
