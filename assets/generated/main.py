import os

def convert_to_header(file_path, output_dir):
    file_name = os.path.basename(file_path)
    variable_name = file_name.replace('.', '_').replace('-', '_')
    output_file = os.path.join(output_dir, f"{variable_name}.h")

    with open(file_path, 'rb') as f:
        data = f.read()

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
