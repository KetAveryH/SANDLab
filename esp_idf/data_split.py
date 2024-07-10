from PIL import Image
import numpy as np
import pyperclip

# Load the image
image_path = '/home/sand-lab-2024/Pictures/Screenshots/please_work_chainsaw.jpeg'

def img_to_cpp_array(image_path):
    # Load the image from the specified path
    image = Image.open(image_path).convert('RGB')  # Convert to RGB to remove alpha channel

    # Resize the image to 224x224 pixels
    image = image.resize((224, 224))

    # Convert the image to a numpy array
    image_array = np.asarray(image)
    print("Image array shape:", np.shape(image_array))  # Should print (224, 224, 3)
    
    # Flatten the image array (from 3D to 1D)
    flattened_array = image_array.flatten()
    
    # Convert the flattened array to a comma-separated string
    cpp_array_str = ', '.join(map(str, flattened_array))
    
    # Get the number of integers in the flattened array
    num_integers = len(flattened_array)
    print(f"Number of integers in the flattened array: {num_integers}")  # Should print 150528
    
    # Check the total size of the integer array in bytes
    total_size_bytes = flattened_array.nbytes
    print(f"Total size of the integer array: {total_size_bytes / 1024:.2f} KB")
    
    # Copy to clipboard
    array_len = len(cpp_array_str)
    if input(f"Are you sure you want to copy a {array_len} character string into your clipboard? \n Y/n: ").lower() == "y":
        pyperclip.copy(cpp_array_str)
        print("Copied to Clipboard")
    else:
        print("Aborted")

if __name__ == "__main__":
    img_to_cpp_array(image_path)
