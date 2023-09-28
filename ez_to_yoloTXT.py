def save_yolo_format_to_file(line, image_width, image_height):
    parts = line.split(', ')
    image_path = parts[0]
    
    boxes = []
    for box_str in parts[2:]:
        box_parts = box_str.split('-')
        x0, y0, x1, y1 = map(float, box_parts[:4])
        score = float(box_parts[4])
        if score <= 0.5:
            continue
        class_id = int(box_parts[5])
        distance = box_parts[6] if len(box_parts) > 6 else "0.00"
        
        box_width = x1 - x0
        box_height = y1 - y0
        center_x = x0 + box_width / 2
        center_y = y0 + box_height / 2
        
        yolo_x = center_x / image_width * 2
        yolo_y = center_y / image_height * 2
        yolo_width = box_width / image_width *2
        yolo_height = box_height / image_height * 2
        
        boxes.append(f"{class_id} {yolo_x:.6f} {yolo_y:.6f} {yolo_width:.6f} {yolo_height:.6f}")
    
    file_name = image_path.split('/')[-1].split('.')[0] + '.txt'
    file_name = "data/label/" + file_name
    output_data = '\n'.join(boxes)
    
    with open(file_name, "w") as f:
        f.write(output_data)

# Example usage
#line = "/mnt/usb/coco84//230807_7035824_0000_01.jpg, 3, 512-170-569-189-0.85-1-0.00, 295-168-417-195-0.70-0-0.00, 436-174-454-185-0.52-1-0.00"
input_file_path = "robvac.txt"
image_width = 1280
image_height = 720
#save_yolo_format_to_file(line, image_width, image_height)

with open(input_file_path, "r") as input_file:
    lines = input_file.readlines()
    for line in lines:
        save_yolo_format_to_file(line, image_width, image_height)