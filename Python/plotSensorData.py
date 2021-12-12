import socket
import struct
from collections import deque
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import errno

# General config
UDP_IP = "192.168.0.220"  # Change to your ip
UDP_PORT = 8888
MESSAGE_LENGTH = 13  # one sensor data frame has 13 bytes
PLOT_UPDATE_IN_MS = 10  # Update rate of animation
DATA_STORAGE_IN_MS = 10000  # Last x ms which are shown in the plot
DATA_OFFSETS = [1, 5, 9]  # sensor data offsets inside the received frame

# Prepare UDP
print("This PC's IP: ", UDP_IP)
print("Listening on Port: ", UDP_PORT)
sock = socket.socket(socket.AF_INET,
                     socket.SOCK_DGRAM)
sock.setblocking(False)
sock.bind((UDP_IP, UDP_PORT))

# Prepare Plots
fig, ax = plt.subplots()


def onClick(event):
    global pause
    pause ^= True
    if pause:
        print("Animation paused")


fig.canvas.mpl_connect('button_press_event', onClick)
print("Click inside the plot to pause animation")
ax.ticklabel_format(useOffset=False)
ax.set_ylabel('Sensor Data')
ax.set_xlabel('Time in ms')
maxDataLength = DATA_STORAGE_IN_MS // (PLOT_UPDATE_IN_MS)
plotData = []
plotData.append(deque([(0, 0)], maxlen=maxDataLength))
plotData.append(deque([(0, 0)], maxlen=maxDataLength))
plotData.append(deque([(0, 0)], maxlen=maxDataLength))
lines = []
lines.append(plt.plot(*zip(*plotData[0]), c='black')[0])
lines.append(plt.plot(*zip(*plotData[1]), c='red')[0])
lines.append(plt.plot(*zip(*plotData[2]), c='blue')[0])

# Global start time
startTime = None
pause = False


def getCurrentTimeInMS():
    return (time.time_ns() + 500000) // 1000000


def update(frame):
    global startTime
    newestData = None
    keepReceiving = True
    # Get latest data
    while keepReceiving and not pause:
        try:
            data, fromAddr = sock.recvfrom(MESSAGE_LENGTH)
            if data:
                newestData = data
                if startTime is None:
                    startTime = getCurrentTimeInMS()
        except socket.error as why:
            if why.args[0] == errno.EWOULDBLOCK:
                keepReceiving = False
            else:
                raise why
    if newestData is not None:
        deltaTime = getCurrentTimeInMS() - startTime
        for currentPlotData, currentLine, offset in zip(plotData, lines, DATA_OFFSETS):
            value, = struct.unpack_from('<f', newestData, offset)
            currentPlotData.append((deltaTime, value))
            currentLine.set_data(*zip(*currentPlotData))
        ax.relim()
        ax.autoscale_view(True, True, True)

    return lines


# Enable blit if you want to speed up things, but you need to take care of the axis limits then
ani = animation.FuncAnimation(
    fig, update, interval=PLOT_UPDATE_IN_MS, blit=False)
plt.show()
