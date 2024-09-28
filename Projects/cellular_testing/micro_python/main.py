import urequests
import ujson
from machine import UART
import utime


"""
This Micropython code is intended to be flashed onto the cellular module.

It reads an incoming CAN message from the cellular module's UART, packages it
into a JSON, and makes an HTTP POST request to a URL of your choice.
"""

server_url = "http://httpbin.org/post"  # Full URL for testing POST request
read_delay = 500  # Time delay between consecutive UART reads (in ms)


# Random CAN message generation for testing purposes
def generate_can_message():
    can_msg = dict()
    can_msg["timestamp"] = 100
    can_msg["id"] = 123
    can_msg["data"] = 223
    can_msg["len"] = 334
    return can_msg


while True:
    can_msg = generate_can_message()  # Create a random CAN message
    can_msg_json = ujson.dumps(can_msg)

    try:
        # Make the POST request with error handling
        response = urequests.post(server_url, json=can_msg_json)
        print("CAN message JSON:", can_msg_json)
        print("Server Response:", response.text)
    except Exception as e:
        print("Error sending POST request:", e)

    utime.sleep_ms(10000)
