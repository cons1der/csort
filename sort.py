import ctypes
import os

libsort = ctypes.CDLL('./libsort.dylib')
libsort.sort_from_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]

def sort_file():
    input_filename = 'input.txt'
    output_filename = 'output.txt'
    num_threads = 4

    # 检查输入文件是否存在
    if not os.path.exists(input_filename):
        print(f"Error: Input file '{input_filename}' not found.")
        return

    # 调用C库函数
    libsort.sort_from_file(input_filename.encode('utf-8'), output_filename.encode('utf-8'), num_threads)
    print("Sorting completed successfully.")

if __name__ == "__main__":
    sort_file()
