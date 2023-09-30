import os
import glob


GT_folder_path = "data/GT_test"
prediction_folder_path = "data/prediction_test"

# 文件路径转为文件名函数

# 定义一个函数来读取文件并存储数据与文件名

def read_yolo_data(file_path):
    data = []
    file_name = os.path.basename(file_path)  # 获取文件名
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()  # 去除首尾空白字符
            if line:
                parts = line.split()  # 按空格分割每行数据
                label = int(parts[0])
                values = [float(x) for x in parts[1:]]
                data.append((label, values))
    
    return data





if __name__ == '__main__':
    GT_files = glob.glob(os.path.join(GT_folder_path, '*.txt'))
    Prediction_files = glob.glob(os.path.join(prediction_folder_path, '*.txt'))
    #print(GT_files)
    

    matches = []
    GT_unmatched = []
    Prediction_unmatched = []

    for i, GT_file_path in enumerate(GT_files):
        GT_file_name = os.path.basename(GT_file_path)
        match_name = prediction_folder_path + "\\" + GT_file_name
        if match_name in Prediction_files:
            # 如果项在第二个列表中，则将其添加到匹配项列表中
            matches.append((GT_file_name, i, Prediction_files.index(match_name)))
        else:
            # 否则，将其添加到未匹配项列表中
            GT_unmatched.append((GT_file_name, i))

# 遍历第二个列表，找出在第一个列表中但未在第二个列表中的项
    for i, Prediction_file_path in enumerate(Prediction_files):
        Prediction_file_name = os.path.basename(Prediction_file_path)
        match_name = GT_folder_path + "\\" + Prediction_file_name
        if match_name not in GT_files:
            Prediction_unmatched.append((Prediction_file_name, i))

    print("匹配成功的文件数量为: ")
    print(len(matches))
    print("GT多余的数量为: ")
    print(len(GT_unmatched))
    print("Prediction多余数量的为: ")
    print(len(Prediction_unmatched))


    for match_file, i, j in matches:
        GT_data_path = GT_folder_path + "/" + match_file
        prediction_data_path = prediction_folder_path + "/" + match_file
        GT_data = read_yolo_data(GT_data_path)
        print("gt:")
        print(GT_data)
        prediction_data = read_yolo_data(prediction_data_path)
        print("prediction_data:")
        print(prediction_data)


    # for i in Prediction_unmatched:
    #     print(i)


    # i = 0
    # for txt_file in GT_txt_files:
    #     file_name, file_data = read_and_store_data(txt_file)
    #     # 打印文件名
    #     print("文件名:", file_name)

    #     # 打印数据
    #     print("数据:")
    #     for label, values in file_data:
    #         print(f"标签: {label}, 数据: {values}")
    #     i += 1
    # print(i)
        