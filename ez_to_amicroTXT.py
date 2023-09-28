def convert_to_custom_format(line, class_names):
    parts = line.split(', ')
    image_path = parts[0]
    boxes = parts[2:]
    
    custom_lines = []
    for box_str in boxes:
        box_parts = box_str.split('-')
        x0, y0, x1, y1 = map(float, box_parts[:4])
        score = float(box_parts[4])
        if score < 0.70:
            continue
        class_id = int(box_parts[5])
        distance = box_parts[6] if len(box_parts) > 6 else "0.00"
        class_name = class_names[class_id]
        x1 = x1 * 2
        y1 = y1 * 2
        x0 = x0 * 2
        y0 = y0 * 2 
        box_width = (x1 - x0)
        box_height = (y1 - y0)
        
        custom_lines.append(f"image: {image_path} cls {class_id} cls_name {class_name} prob: {score:.4f} bbox: {int(x0)} {int(y0)} {int(box_width)} {int(box_height)}")
    
    return custom_lines

# Read data from the input txt file
input_file_path = "robvac.txt"  # Replace with your input txt file path
class_names = ["background", "shoes", "trashcan", "carpet", "clothes", "feces", "stains", "fanbase","background","background","background","background"]  # Modify this based on your class names

output_lines = []

with open(input_file_path, "r") as input_file:
    lines = input_file.readlines()
    for line in lines:
        custom_lines = convert_to_custom_format(line.strip(), class_names)
        output_lines.extend(custom_lines)

output_file_path = "ABC-recoginze-070.txt"  # Replace with the desired output file path

with open(output_file_path, "w") as output_file:
    for line in output_lines:
        output_file.write(line + "\n")