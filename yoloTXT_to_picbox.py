import cv2
import os
import glob

class_name = []

#yolo格式的box信息文件夹
box_folder = "D:\\My programs\\mAP\\mAP_cacl\\data\\GT"
#是否是GT（预测框比GT多一个得分）
is_GT = True
#图片尺寸
img_w = 1280
img_h = 720
# 读取class类别信息
class_path = "D:\\My programs\\mAP\\mAP_cacl\\data\\classes.txt"
#图片的读取路径与保存路径
read_folder_path = "D:\\My programs\\mAP\\images"
send_folder_path = "D:\\My programs\\mAP\\images_out"

#读取yolo格式box信息
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
                if not is_GT:
                    pro = values[-1]
                    values.pop()
                    data.append((label, values,pro))
                else:
                    data.append((label, values))
    return file_name,data

# yolo格式的x_center,y_center,w,h翻译为xywh
def yolo_to_xywh(bbox):
    x = (bbox[0] - bbox[2]/2) * img_w
    y = (bbox[1] - bbox[3]/2) * img_h
    w = bbox[2] * img_w
    h = bbox[3] * img_h
    return int(x),int(y),int(w),int(h)

# 读取class信息函数
def read_classes(file_path):
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()  # 去除首尾空白字符
            if line:
                class_name.append(line)

read_classes(class_path)
box_files = glob.glob(os.path.join(box_folder, '*.txt'))
count = 0
for box_file in box_files:
    file_name,data = read_yolo_data(box_file)
    pic_name = file_name[:-3] + "jpg"
    #print(data)
    is_draw = False
    for bbox_data in data:
        cls_ = bbox_data[0]
        bbox = bbox_data[1]
        
        #如果保存过这张图，则继续绘制框。如果没保存过，绘制新的图。
        img_path = send_folder_path + '\\' + pic_name
        if not os.path.exists(img_path):
            img_path = read_folder_path + '\\' + pic_name
        
        x,y,w,h = yolo_to_xywh(bbox)

        img = cv2.imread(img_path)
        color = (0, 255, 0)  # 绿色边界框
        if is_GT:
            color = (255,0,0)
        thickness = 2
        cv2.rectangle(img, (x, y), (x + w, y + h), color, thickness)

        #在边界框上添加标签信息q
        label = ''
        if not is_GT:
            prob = bbox_data[2]
            label = f'{class_name[int(cls_)]}: {prob:.4f}'
        else:
            label = f'{class_name[int(cls_)]}'
        font = cv2.FONT_HERSHEY_SIMPLEX
        font_scale = 0.5
        text_size = cv2.getTextSize(label, font, font_scale, thickness)[0]
        text_x = x
        text_y = y - 5 if y - 5 > 0 else y + h + 20
        cv2.putText(img, label, (text_x, text_y), font, font_scale, color, thickness)

        # 保存绘制边界框后的图片
        output_path = send_folder_path + "\\" + pic_name  # 构造输出路径
        cv2.imwrite(output_path, img)
    count += 1 
    precent = '{:.1f}'.format(100*count/len(box_files))
    print("[",precent,"%]",pic_name,' done!')
