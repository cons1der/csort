import random

def generate_random_numbers(filename, num_numbers):
    with open(filename, 'w') as file:
        for _ in range(num_numbers):
            number = random.randint(0, 1000000)  # 生成0到1000000之间的随机数
            file.write(f"{number}\n")

if __name__ == "__main__":
    output_filename = 'input.txt'  # 设置输出文件名
    number_of_randoms = 1000000    # 要生成的随机数数量
    generate_random_numbers(output_filename, number_of_randoms)