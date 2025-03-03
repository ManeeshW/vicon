import time
import fdcl_vicon_module  # This should match the name of your compiled Python module
import numpy as np

def main():
    # Create an instance of the VICON class
    vicon_tracker = fdcl_vicon_module.vicon()

    # Open a connection to the VICON system
    vicon_tracker.open("Quad@192.168.10.1")  # You can pass a specific object string if needed, e.g., "racing@192.168.10.1"

    try:
        print("Starting VICON tracking... Press Ctrl+C to stop.")
        
        while True:
            # Perform the main loop to receive data
            x_v, R_vm = vicon_tracker.loop()  # Fetch the position and rotation data

            # Arrange the rotation matrix and position vector in a 4x4 homogeneous transformation matrix
            H_vm = np.eye(4)  # Initialize 4x4 identity matrix
            H_vm[:3, :3] = np.array(R_vm)  # Top-left 3x3 block for rotation matrix
            H_vm[:3, 3] = np.array(x_v)  # Top-right 3x1 block for position vector
            
            # Print the results
            print(f"x_v -> : {x_v}")  # Position vector
            print(f"R_vm -> :\n{R_vm}")  # Rotation matrix
            print(f"Homogeneous matrix -> :\n{H_vm}")  # 4x4 Homogeneous transformation matrix
            
            time.sleep(0.01)  # Adjust as necessary

    except KeyboardInterrupt:
        print("Stopping VICON tracking...")

    finally:
        # Ensure the connection is closed on exit
        vicon_tracker.close()

if __name__ == "__main__":
    main()