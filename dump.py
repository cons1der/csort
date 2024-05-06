import ctypes 
import sys 

libsort = ctypes.CDLL('./libsort.dylib')

libsort.sort_from_file.argtypes = [ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]

def sort_file(input, output, nthreads):
    libsort.sort_from_file(input.encode('utf-8'), output.encode('utf-8'), nthreads)


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print('invalid args')
        sys.exit(1)

    input = sys.argv[1]
    output = sys.argv[2]
    nums = int(sys.argv[3])

    sort_file(input, output, nums)
