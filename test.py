import numpy as np
m = 10
n = 20
array_zeros = np.zeros((m, n))

a = 0


def abc(a):
    a += 2
    return a


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

# 示例数据

if __name__ == '__main__':
    a =  abc(a)
    print(a)
    a =  abc(a)
    print(a)
    a =  abc(a)
    print(a)
    data1 = [0, 0, 4, 4]
    data2 = [0,0 , 2, 2]

    iou = calculate_iou(data1, data2)
    print("IOU:", iou)
    IOU_array = np.full((3, 2), 0, dtype=float)
    print(IOU_array)