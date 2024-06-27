from PIL import Image
import numpy as np
import pyperclip

# Load the image
image_path = 'images/n03028079_church.JPEG'

def img_to_cpp_array(image_path):
    image = Image.open(image_path)

    # Resize the image
    image = image.resize((224, 224))

    # Convert image to numpy array and normalize it to [-1, 1]
    image_array = np.asarray(image)
    print(np.shape(image_array))
    # image_array = image_array / 127.5 - 1

    # Optionally, convert to C++ array (as a string format if needed)
    cpp_array_str = ', '.join(map(str, image_array.flatten()))
    cpp_array_format = f"{{{cpp_array_str}}};"  
    array_len = len(cpp_array_format)

    if input(f"Are you sure you want to copy a {array_len} character string into your clip board? \n Y/n: ").lower() == "y":
        pyperclip.copy(cpp_array_format)
        print("Copied to Clipboard")
    else:
        print("Aborted")

if __name__ == "__main__":
    img_to_cpp_array(image_path)