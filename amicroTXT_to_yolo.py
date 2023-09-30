import os

prediction_folder = "data\\prediction"
recoginfo_path = 'data\\recoginfo.txt'
img_w = 1280
img_h = 720


if not os.path.exists(prediction_folder):
    os.makedirs(prediction_folder)

with open(recoginfo_path, 'r') as f:
    lines = f.readlines()

# 循环处理每一行数据
for line in lines:
    parts = line.strip().split()
    
    # 解析数据
    image_path = parts[1]
    cls = int(parts[3])
    prob = float(parts[7])
    bbox = list(map(int, parts[9:]))

    output_file_path = prediction_folder + "\\" + image_path[:-3] + "txt"
    x_center = (bbox[0] + (bbox[2] / 2)) / img_w
    y_center = (bbox[1] + (bbox[3] / 2)) / img_h
    w = bbox[2] / img_w
    h = bbox[3] / img_h

    # 格式化输出字符串
    output_string = "{} {:.6f} {:.6f} {:.6f} {:.6f} {:.6f}".format(cls,x_center,y_center,w,h,prob)
    # 如果不存在文件则创建，存在则续写
    if not os.path.isfile(output_file_path):
        with open(output_file_path, 'w') as output_file:
            output_file.write(output_string + "\n")
    else:
        with open(output_file_path, "a") as output_file:
            output_file.write(output_string + "\n")


