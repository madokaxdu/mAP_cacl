import os
import glob
import numpy as np

IOU = 0.5
GT_folder_path = "data/GT_test"
prediction_folder_path = "data/prediction_test"
img_w = 1280
img_h = 720
class_path = "D:\\My programs\\mAP\\mAP_cacl\\data\\classes.txt"

# 类别信息
class_name = []
class_quantity = 0



# 定义一个函数来读取yolotxt格式文件并存储数据与文件名
def read_yolo_data(file_path,is_GT):
    data = []
    file_name = os.path.basename(file_path)  # 获取文件名
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()  # 去除首尾空白字符
            if line:
                parts = line.split()  # 按空格分割每行数据
                label = int(parts[0])
                values = [float(x) for x in parts[1:]]
                if not is_GT:
                    pro = values[-1]
                    values.pop()
                    data.append((label, values,pro))
                else:
                    data.append((label, values))
    return data


# yolo格式的x_center,y_center,w,h翻译为xywh
def yolo_to_xywh(bbox):
    x = (bbox[0] - bbox[2]/2) * img_w
    y = (bbox[1] - bbox[3]/2) * img_h
    w = bbox[2] * img_w
    h = bbox[3] * img_h
    return int(x),int(y),int(w),int(h)

# 读取class信息函数
def read_classes(file_path):
    count = 0
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()  # 去除首尾空白字符
            if line:
                class_name.append(line)
                count += 1
    return count

# 计算IOU的函数
def calculate_iou(data1, data2):
    x1, y1, w1, h1 = data1
    x2, y2, w2, h2 = data2

    # 计算交集的坐标和宽高
    x_intersection = max(x1, x2)
    y_intersection = max(y1, y2)
    w_intersection = max(0, min(x1 + w1, x2 + w2) - x_intersection)
    h_intersection = max(0, min(y1 + h1, y2 + h2) - y_intersection)

    # 计算交集面积和并集面积
    intersection_area = w_intersection * h_intersection
    union_area = (w1 * h1) + (w2 * h2) - intersection_area

    # 计算IOU
    iou = intersection_area / union_area

    return iou

def one_pic_match(GT_data,prediction_data):
    GT_len = len(GT_data)
    prediction_len = len(prediction_data)
    IOU_array = np.full((GT_len, prediction_len), 0, dtype=float)
    i = 0
    for GT_box in GT_data:
        j = 0
        for predition_box in prediction_data:
            if(GT_box[0] == predition_box[0]):
                IOU_array[i][j] =  calculate_iou(GT_box[1],predition_box[1])
            else:
                IOU_array[i][j] = -1.0
    

    

if __name__ == '__main__':
    class_quantity = read_classes(class_path)

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
        GT_data = read_yolo_data(GT_data_path,True)
        prediction_data = read_yolo_data(prediction_data_path,False)

        print(GT_data)
        print(prediction_data)

        IOU_array = np.full((len(prediction_data), len(GT_data)), 0, dtype=float)
        IOU_array[2][3] = 6



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
        