# 🛠️ Before Uploading 'servo_control.py' to Raspberry Pi 
Make sure to follow these steps before running the code to ensure proper functionality of the  servo control system:

## ✅ Pre-requisites

1. **Install the `pigpio` library**  
   This code uses the `pigpio` library for precise servo motor control. Ensure it's installed on your Raspberry Pi.

     $ sudo apt update
     $ sudo apt install pigpio python3-pigpio

2.Start the pigpiod daemon
  After installing the library, you must start the pigpio daemon:

    $ sudo pigpiod

  ⚠️ Make sure to run this command every time before executing the script, or add it to your .bashrc for automatic startup.
