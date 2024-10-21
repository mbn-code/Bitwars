# @brief: This script converts binary files to C header files.
# This is useful for embedding binary files in the compiled binary.
# The generated header files can be included in the source code to access the binary data.
# This works well for small files, but not recommended for large files.

import os

def convert_to_header(file_path, output_dir):
    file_name = os.path.basename(file_path)
    variable_name = file_name.replace('.', '_').replace('-', '_')
    output_file = os.path.join(output_dir, f"{variable_name}.h")

    with open(file_path, 'rb') as f:
        data = f.read()

    """
    @brief: Write the binary data to a C header file.

    The header file contains the following:
    - A comment with the file name
    - An array of unsigned char with the binary data
    - The length of the binary data
    """
    with open(output_file, 'w') as f:
        f.write(f"// Auto-generated header file for {file_name}\n")
        f.write(f"const unsigned char {variable_name}[] = {{\n")
        for i, byte in enumerate(data):
            if i % 12 == 0:
                f.write("\n    ")
            f.write(f"0x{byte:02x}, ")
        f.write("\n};\n")
        f.write(f"const unsigned int {variable_name}_len = {len(data)};\n")

if __name__ == "__main__":
    assets = [
        "../Images/Base_texture_BR.png",
        "../Images/Base2_texture_BR.png",
        "../Images/background.png",
        "../Sound/Default_Sound_Track.wav"
    ]
    output_dir = "src/generated"
    os.makedirs(output_dir, exist_ok=True)
    for asset in assets:
        convert_to_header(asset, output_dir)
