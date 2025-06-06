import RPi.GPIO as GPIO
import socket
import csv
import time
import os
import math
import pigpio

global duty
duty = 0

phw = pigpio.pi()

addr_IP = "xx.xx.xx.xx"        // your IP address of RPi
addr_PORT = 5555  

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM) 
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1) 
sock.setsockopt(socket.SOL_SOCKET,socket.SO_BROADCAST,1)
sock.bind((addr_IP, addr_PORT))

phw.hardware_PWM(18, 800, 0)  #vertical = 18
phw.hardware_PWM(13, 800, 0)  #horizontal = 13


def setAngle(angle):
    return (angle+90)/18 + 2
    
def dist(a,b):
    return math.sqrt((a*a)+(b*b))
 
def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return math.degrees(radians)
 
def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

def get_acc(data):
	imu_op = data.split(',')    
	acc_x = float(imu_op[2])
	acc_y = float(imu_op[3])
	acc_z = float(imu_op[4])
	return [acc_x,acc_y,acc_z]

while True:
    data, adr = sock.recvfrom(1024)
    data =  data.decode()			
    acc_op = get_acc(data)
	
    y_theta = get_y_rotation(acc_op[0],acc_op[1],acc_op[2])
    x_theta = get_x_rotation(acc_op[0],acc_op[1],acc_op[2])
    
    duty_x = setAngle(x_theta) * 10000
    phw.hardware_PWM(13, 50, int(duty_x))  
    # time.sleep()
    duty_y= setAngle(-y_theta) * 10000
    phw.hardware_PWM(18, 50, int(duty_y))  

phw.stop()
