#!/usr/bin/python

import argparse

if __name__ == '__main__':

    # Parse arguments
    parser = argparse.ArgumentParser(description="Generates a C header file with icon file embedded.")
    
    parser.add_argument('-i', '--icon', type=str, help='Icon path', required=True)
    parser.add_argument('-o', '--output', type=str, help='Output header path', required=True)
    parser.add_argument('-r', '--raylib', type=str, help='Raylib header path', required=True)

    args = parser.parse_args()

    # Pull paths from arguments
    path_icon   = args.icon
    path_header = args.output
    path_raylib = args.raylib

    # Get extension
    ext = path_icon.split('.')[-1]

    # Format image to encoded byte string
    with open(path_icon, 'rb') as f:
        byte_str = ''
        byte_len = 0
        for byte in f.read():
            byte_str += f'\\x{byte:02X}'
            byte_len += 1

    # Write header file
    with open(path_header, 'w') as f:
        f.write(f'// NOTE: File generated from "{path_icon}"\n')
        f.write(f'#include "{path_raylib}"\n')
        f.write(f'void SetEmbeddedWindowIcon(){{Image i=LoadImageFromMemory(".{ext}",(unsigned char*)"{byte_str}",{byte_len});SetWindowIcon(i);UnloadImage(i);}}')