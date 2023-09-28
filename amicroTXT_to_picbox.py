import cv2

class_name_am = ["data_wire","shoes","trashcan","clothes","scale","fanbase","paper"]
class_name_ez = ["data_wire","shoes","trashcan","carpet","duster","feces","scales","stauns","fanbase","paper"]

# 读取txt文件
read_folder_path = "C:\\Users\\18792\\Desktop\\data\\GT8-10\\testData0810\\divided_to_combinedImages\\"
send_folder_path = "C:\\Users\\18792\\Desktop\data\\GT8-10_Prediction_box\\ez_box\\"
img_name = ''
with open('8-10recoginfo\\EZ-recoginfo_0.5.txt', 'r') as f:
    lines = f.readlines()

# 循环处理每一行数据
for line in lines:
    parts = line.strip().split()
    
    # 解析数据
    image_path = parts[1]
    cls_name = parts[3]
    prob = float(parts[7])
    bbox = list(map(int, parts[9:]))
    img_path = ''
    # 加载图片
    image_path = image_path[26::]
    if image_path == img_name:
        img_path =send_folder_path + image_path 
    else:
        img_path = read_folder_path + image_path
        img_name = image_path
    print(img_path,"画框：")
    img = cv2.imread(img_path)
    
    # 提取边界框坐标
    x, y, w, h = bbox
    
    # 绘制边界框
    color = (0, 255, 0)  # 绿色边界框
    thickness = 2
    cv2.rectangle(img, (x, y), (x + w, y + h), color, thickness)
    
    # 在边界框上添加标签信息
    label = f'{class_name_ez[int(cls_name)]}: {prob:.4f}'
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.5
    text_size = cv2.getTextSize(label, font, font_scale, thickness)[0]
    text_x = x
    text_y = y - 5 if y - 5 > 0 else y + h + 20
    cv2.putText(img, label, (text_x, text_y), font, font_scale, color, thickness)

    # 保存绘制边界框后的图片
    output_path = send_folder_path + image_path  # 构造输出路径
    print(output_path)
    cv2.imwrite(output_path, img)

    print(f'Processed: {image_path}')

print('Done!')
