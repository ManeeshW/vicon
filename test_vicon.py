import time
import vicon_tracker  # This matches the compiled Python module
import numpy as np
import os

def load_config(config_file="config.cfg"):
    """Read the Vicon object name from config.cfg."""
    object_name = "OriginsX@192.168.10.1"  # Default fallback
    try:
        with open(config_file, 'r') as file:
            for line in file:
                line = line.strip()
                if line.startswith("object:"):
                    object_name = line.split("object:")[1].strip().strip('"')
                    break
        print(f"Loaded Vicon object: {object_name}")
    except FileNotFoundError:
        print(f"Config file {config_file} not found, using default object: {object_name}")
    except Exception as e:
        print(f"Error reading config file: {e}, using default object: {object_name}")
    return object_name

def main():
    # Load the Vicon object name from config.cfg
    config_file = os.path.join(os.path.dirname(__file__), "config.cfg")
    object_name = load_config(config_file)

    # Create an instance of the VICON class
    vicon = vicon_tracker.vicon()

    # Open a connection to the VICON system
    vicon.open(object_name)

    try:
        print("Starting VICON tracking... Press Ctrl+C to stop.")
        
        while True:
            # Perform the main loop to receive data
            x_v, R_vm = vicon.loop()  # Fetch the position and rotation data

            # Arrange the rotation matrix and position vector in a 4x4 homogeneous transformation matrix
            H_vm = np.eye(4)  # Initialize 4x4 identity matrix
            H_vm[:3, :3] = np.array(R_vm)  # Top-left 3x3 block for rotation matrix
            H_vm[:3, 3] = np.array(x_v)  # Top-right 3x1 block for position vector
            
            # Print the results
            print(f"x_v -> : {x_v}")  # Position vector
            print(f"R_vm -> :\n{R_vm}")  # Rotation matrix
            
            time.sleep(0.01)  # Adjust as necessary

    except KeyboardInterrupt:
        print("Stopping VICON tracking...")

    finally:
        # Ensure the connection is closed on exit
        vicon.close()

if __name__ == "__main__":
    main()