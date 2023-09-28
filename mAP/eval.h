#ifndef GDDI_EVAL_H
#define GDDI_EVAL_H
extern const float iou_threshold = 0.5;

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include "pugixml\src\pugixml.cpp"

struct BoundingBox {
    std::string filename_v;
    int cls_v;
    int x_min_v;
    int y_min_v;
    int x_max_v;
    int y_max_v;
    float prob_v;
};
struct XMLData {
    std::string filename;
    int cls;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};
struct piror{
    float confidence;
    bool TF;
};
// 从名字中裁剪出ID
std::string picture_id(const std::string&);
std::string picture_id(const std::string& name){
    int len = name.size();
    int i;
    int count = 0;
    for(i = len - 5;i >0;i--){
        if(name[i] != '_' || (int)name[i] > 57 || (int)name[i] < 48)
            break;
            count++;
    }

    return name.substr(i+1,count);
}
//给sort的比较函数，用于排序置信度
bool compareByAge(const piror& p1, const piror& p2);
bool compareByAge(const piror& p1, const piror& p2) {
    return p1.confidence > p2.confidence;
}
//比较函数，用于排序召回率
bool compareByRecall(const std::vector<float>& pr1, const std::vector<float>& pr2);
bool compareByRecall(const std::vector<float>& pr1, const std::vector<float>& pr2) {
    return pr1[0] < pr2[0];
}
// readtexr 读出recoginfo.txt中的bbox信息存到数组中
int readtext(std::vector<std::vector<BoundingBox>>&,std::string&);

//解析单个XML文件
XMLData parseXMLFile(const std::string&);

//遍历windows中xml的文件并储存在数组中，如果换到linux需要改
int getXmlsInFolder(const std::string& ,std::vector<std::vector<XMLData>>&);

//遍历yolo格式TXT
int getTXTsInFolder(const std::string& ,std::vector<std::vector<XMLData>>&);


//计算iou (boundingbox,xmldata)
float calculate_iou(const BoundingBox&, const XMLData&);

//匹配groundtruth 与oiror 返回结果为匹配结果
std::vector<int> clss_match(const std::vector<BoundingBox>& b,const std::vector<XMLData>& x);

//将两组数据中相同图片ID的匹配到一起
int picture_match(const std::vector<BoundingBox>& b,const std::vector<XMLData>& x,std::vector<std::vector<int>>& picture_,const std::string& pictureID);

//计算精确率-召回率数组
std::vector<std::vector<float>> compute_AP(const std::vector<BoundingBox>& b,const std::vector<XMLData>& x);

//整理AP数组使相同召回率只保留最大准确率
void removeDuplicates(std::vector<std::vector<float>>& data);

//根据AP数组计算AP的值
float calculateAPRC(std::vector<std::vector<float>>& data);

//计算AI100的AP
std::vector<std::vector<float>> AI100_compute_AP(const std::vector<BoundingBox>& b,const std::vector<XMLData>& x);

//计算并处理AI100的gt
int AI100_getXmlsInFolder(const std::string& folderPath,std::vector<std::vector<XMLData>>&);

int AI100_readtext(std::vector<std::vector<BoundingBox>>& recoginfo_array,std::string&,int);

//所有预测框里取最小的
BoundingBox one_img(std::vector<BoundingBox>& a);












// class GddiEval {
// public:
//     GddiEval(const std::string& recoginfo_path, bool draw_box = false);
//     void main();
//     std::vector<std::vector<float>> get_xywhc_with_imgnames(const std::vector<std::string>& img_names);
//     void generate_ground_truth_txt(const std::string& folder);
//     void generate_predict_res_txt();
//     std::vector<std::vector<std::string>> parse_xml(const std::string& file);

// private:
//     std::string recoginfo_path;
//     bool draw_box;
//     std::vector<std::string> class_list;
//     std::vector<BoundingBox> recoginfo_array;
// };

#endif  // GDDI_EVAL_H
