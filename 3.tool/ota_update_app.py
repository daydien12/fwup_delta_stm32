
import time
import os
import struct
import serial
import time

sizedata = 1024
arduino = serial.Serial(port='COM7', baudrate=9600, timeout=.8)
def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)
    data = arduino.readline()
    return data

def send_ota_start():
    sof         = 0xAA
    packet_type = 0x00
    data_len    = struct.pack('H',1)
    cmd         = 0x00
    crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 10
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0] = sof
    sampleBuffer[1] = packet_type
    sampleBuffer[2] = data_len[0]
    sampleBuffer[3] = data_len[1]
    sampleBuffer[4] = cmd
    sampleBuffer[5] = crc[0]
    sampleBuffer[6] = crc[1]
    sampleBuffer[7] = crc[2]
    sampleBuffer[8] = crc[3]
    sampleBuffer[9] = eof
    arduino.write(sampleBuffer)

def send_ota_end():
    sof         = 0xAA
    packet_type = 0x00
    data_len    = struct.pack('H',1)
    cmd         = 0x01
    crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 10
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0] = sof
    sampleBuffer[1] = packet_type
    sampleBuffer[2] = data_len[0]
    sampleBuffer[3] = data_len[1]
    sampleBuffer[4] = cmd
    sampleBuffer[5] = crc[0]
    sampleBuffer[6] = crc[1]
    sampleBuffer[7] = crc[2]
    sampleBuffer[8] = crc[3]
    sampleBuffer[9] = eof
    arduino.write(sampleBuffer)

def send_ota_header(dataLength):
    sof         = 0xAA
    packet_type = 0x02
    data_len    = struct.pack('H',4)
    sidedata    = struct.pack('I',dataLength)
    crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 13
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0]  = sof
    sampleBuffer[1]  = packet_type
    sampleBuffer[2]  = data_len[0]
    sampleBuffer[3]  = data_len[1]
    sampleBuffer[4]  = sidedata[0]
    sampleBuffer[5]  = sidedata[1]
    sampleBuffer[6]  = sidedata[2]
    sampleBuffer[7]  = sidedata[3]
    sampleBuffer[8]  = crc[0]
    sampleBuffer[9]  = crc[1]
    sampleBuffer[10] = crc[2]
    sampleBuffer[11] = crc[3]
    sampleBuffer[12] = eof
    arduino.write(sampleBuffer)

def send_ota_data(dataLength):
    length = len(dataLength)
    index = 0
    
    sof         = 0xAA
    packet_type = 0x01
    data_len    = struct.pack('H',length)
    crc         = struct.pack('I',0x12345678)
    eof         = 0xBB

    bufferLength = 9+length
    sampleBuffer = bytearray(bufferLength)
    sampleBuffer[0]  = sof
    sampleBuffer[1]  = packet_type
    sampleBuffer[2]  = data_len[0]
    sampleBuffer[3]  = data_len[1]
    index = index + 4
    for i in range(length):
        sampleBuffer[index] = dataLength[i]
        index = index + 1
    sampleBuffer[index]   = crc[0]
    sampleBuffer[index+1] = crc[1]
    sampleBuffer[index+2] = crc[2]
    sampleBuffer[index+3] = crc[3]
    sampleBuffer[index+4] = eof
    arduino.write(sampleBuffer)


def calculate_fram_data():
    # get the size of file
    #path = r"C:/Users/nhanma/Desktop/20.bootloder/2.Apptest_stm32/MDK/Objects/gpio_blynk.bin"
    path = r"C:/Users/nhanma/Desktop/blinky-k64f-old.bin"
    size = os.path.getsize(path) 
    print('Size of file is', size, 'bytes')
    file = open(path, "rb")
    number = list(file.read(size))
    bytes_of_values = bytes(number)
    file.close()
    chia = size/sizedata
    index = 0
    print("-------Start-------")  
    send_ota_start()
    time.sleep(2)
    print("-------Header-------")  
    send_ota_header(size)
    time.sleep(2)
    if (int(chia)*sizedata) == size:
        bufferLength = sizedata
        sampleBuffer = bytearray(bufferLength)
        for i in range(int(chia)):
            print("-------LOADA: ", i,  "-------")  
            for j in range(sizedata):
                    sampleBuffer[j] = number[index]
                    index = index + 1
            send_ota_data(sampleBuffer)
            time.sleep(0.1)
    else:
        offset = 0
        size_array = 0
        for i in range(int(chia)+1):
            print("-------LOAD: ", i,  "-------")
            size_array = sizedata
            if((offset+sizedata) > size):
                size_array = size - offset
            sampleBuffer = bytearray(size_array)
            for j in range(size_array):
                sampleBuffer[j] = number[index]
                index = index + 1
                offset = offset + 1;
            send_ota_data(sampleBuffer)
            time.sleep(0.1)
    time.sleep(1)
    send_ota_end()
    print("-------Done-------")                  
calculate_fram_data()
