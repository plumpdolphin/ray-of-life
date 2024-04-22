#!/usr/bin/python

import sys

if __name__ == '__main__':

    # Pull paths from arguments
    path_icon   = sys.argv[1]
    path_header = sys.argv[2]
    path_raylib = sys.argv[3]

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