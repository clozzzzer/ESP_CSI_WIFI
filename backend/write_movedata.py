import csv
import random
import time
import os

csv_file_path = 'movedata.csv'

# 模拟生成数据的函数
def generate_random_data():
    return random.randint(0, 1)


# 写入数据到 CSV 文件
def write_to_csv():
    try:
        while True:
            with open(csv_file_path, mode='a', newline='') as file:
                writer = csv.writer(file)
            
                # 生成新的数据
                data = [generate_random_data()]
                # 写入文件
                writer.writerow(data)
                os.fsync(file.fileno())  # 强制刷新缓存，确保数据立即写入文件
                print(f'Wrote data: {data}')
                time.sleep(1)  # 每秒写入一次
    except KeyboardInterrupt:
        print("Data writing stopped by user.")
    except Exception as e:
        print(f'An error occurred: {e}')

if __name__ == '__main__':
    write_to_csv()

