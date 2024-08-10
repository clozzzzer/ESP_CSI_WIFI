import csv
import os


csv_file_path = csv_file_path = 'movedata.csv'

def clear_csv_file():
    try:
        # 打开 CSV 文件并读取标题行
        with open(csv_file_path, mode='r', newline='') as infile:
            reader = csv.reader(infile)
            header = next(reader)  # 读取标题行
        
        # 重新写入标题行，清空其他数据
        with open(csv_file_path, mode='w', newline='') as outfile:
            writer = csv.writer(outfile)
            writer.writerow(header)  # 写入标题行

        print(f'{csv_file_path} has been cleared and only contains the header row.')
    
    except Exception as e:
        print(f'An error occurred: {e}')

if __name__ == '__main__':
    clear_csv_file()
