import pygame
import time
import serial

# Initialize Pygame and the joystick
pygame.init()
pygame.joystick.init()

# Ensure that the joystick is connected
if pygame.joystick.get_count() == 0:
    print("No joystick connected")
    exit()

# Get the first joystick
joystick = pygame.joystick.Joystick(0)
joystick.init()

# Serial port setup (update the port to your system's port)
arduino = serial.Serial(port='/dev/cu.usbserial-1130', baudrate=9600, timeout=.1)  # This is your Arduino port

# Initial toggle state
toggle_state = -1

# Time of the last button press
last_toggle_time = 0

# Debounce time in seconds
debounce_time = 1  # 1 second

# Function to map axis values (-1.0 to 1.0) to 0-255
def map_axis_value(value):
    return int((value + 1) * 127.5)

# Loop to read joystick input and send data to Arduino
try:
    while True:
        pygame.event.pump()  # Process event queue

        # Read Axis 4 (Right Stick X) and Axis 5 (Right Stick Y)
        axis_4_value = joystick.get_axis(4)
        axis_5_value = joystick.get_axis(5)

        # Map the values to 0-255
        mapped_axis_4 = map_axis_value(axis_4_value)
        mapped_axis_5 = map_axis_value(axis_5_value)

        # Read Button 0 (typically the "A" button)
        button_0 = joystick.get_button(0)

        # Get the current time
        current_time = time.time()

        # Toggle the state if Button 0 is pressed and debounce time has passed
        if button_0 and (current_time - last_toggle_time) > debounce_time:
            toggle_state = 1 if toggle_state == -1 else -1
            last_toggle_time = current_time

        # Prepare data to send to Arduino
        data_to_send = f"{mapped_axis_4},{mapped_axis_5},{toggle_state}\n"
        
        # Send data to Arduino
        arduino.write(data_to_send.encode())

        # Print the data being sent
        print(f"Sent to Arduino: Axis 4: {mapped_axis_4}, Axis 5: {mapped_axis_5}, Toggle State: {toggle_state}")

        # Small delay to prevent overwhelming the terminal output
        pygame.time.wait(100)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    pygame.quit()
    arduino.close()
