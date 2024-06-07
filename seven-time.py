import serial
import time
from datetime import datetime
import argparse
import logging

from typing import Optional

# Set up command line arguments
parser = argparse.ArgumentParser(description='Send current time to a seven-segment display via serial port.')
parser.add_argument('--port', '-p', type=str, default='/dev/ttyAMA0', help='Serial port to use (default: /dev/ttyAMA0)')
parser.add_argument('--baud_rate', '-b', type=int, default=9600, help='Baud rate for serial communication (default: 9600)')
parser.add_argument('--log_level', type=str, default='INFO', help='Log level (default: INFO)')
args = parser.parse_args()

# Set up logging
logging.basicConfig(level=args.log_level)
logger = logging.getLogger("clock")

# Configure the serial connection
ser = serial.Serial(args.port, args.baud_rate)

logger.info(f"Connected to serial port: {ser.name}")

def send_time(last_sent_time: Optional[str] = None):
    # Get current time
    now = datetime.now()
    time_str = now.strftime("%H%M%S")  # Format time as a four-digit string

    # Check if the time has changed since last sent
    if last_sent_time is None or time_str != last_sent_time:
        # Send the time starting with '$D', the time string, and a newline
        command = f'$D{time_str}'
        logger.debug(f"Sending: {command}")

        # Send the command to the serial port with a newline
        ser.write((command + '\n').encode())

        return time_str

    return last_sent_time

try:
    last_sent_time = None
    while True:
        last_sent_time = send_time(last_sent_time)
        # 1 second poll
        time.sleep(1)
except KeyboardInterrupt:
    print("Stopped by user.")
except Exception as e:
    print(f"Error: {e}")
finally:
    ser.close()  # Close the serial connection to clean up

