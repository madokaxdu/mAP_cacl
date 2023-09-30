#include "eval.h"
using namespace std;





float calculate_iou(const BoundingBox& box1, const XMLData& box2) {
    // 计算交集的坐标
    float x_min_inter = std::max(box1.x_min_v, box2.xmin);
    float y_min_inter = std::max(box1.y_min_v, box2.ymin);
    float x_max_inter = std::min(box1.x_max_v, box2.xmax);
    float y_max_inter = std::min(box1.y_max_v, box2.ymax);

    // 计算交集的宽度和高度
    float width_inter = std::max(0.0f, x_max_inter - x_min_inter);
    float height_inter = std::max(0.0f, y_max_inter - y_min_inter);

    // 计算交集的面积
    float area_inter = width_inter * height_inter;

    // 计算两个边界框的面积
    float area_box1 = (box1.x_max_v - box1.x_min_v) * (box1.y_max_v - box1.y_min_v);
    float area_box2 = (box2.xmax - box2.xmin) * (box2.ymax - box2.ymin);

    // 计算并集的面积
    float area_union = area_box1 + area_box2 - area_inter;

    // 计算IoU值
    float iou = (area_inter > 0.0f && area_union > 0.0f) ? area_inter / area_union : 0.0f;

    return iou;
}

vector<int> clss_match(const vector<BoundingBox>& b,const vector<XMLData>& x){
    vector<int> matches;
    matches.resize(b.size(),0);
    for(int cls_i = 0;cls_i < x.size();cls_i++){
        vector<vector<int>> picture_(2);                                                       //picture_同一张图片bbox的下标，[0]是预测[1]是标注
        int num = picture_match(b,x,picture_,picture_id(x[cls_i].filename));
        // cout << "同为一张图片的有：" << endl; 
        // cout << "预测数据：" << endl;
        // for(int j = 0;j < picture_[0].size();j++){
        //     cout << b[picture_[0][j]].filename_v << endl;
        // }
        // cout << "标注数据：" << endl;
        // for(int j = 0;j < picture_[1].size();j++){
        //     cout << x[picture_[1][j]].filename << endl;
        // }



        extern const float iou_threshold;
        const int num_b = picture_[0].size();
        const int num_x = picture_[1].size();

        vector<vector<float>> ious(num_x, std::vector<float>(num_b, 0.0));
        for (int i = 0; i < num_x; ++i) {
            for (int j = 0; j < num_b; ++j) {
            // 计算IoU值
                float iou = calculate_iou(b[picture_[0][j]],x[picture_[1][i]]);
                if(iou >= iou_threshold)
                    ious[i][j] = iou;
            }
        }

        
        for (int i = 0; i < num_x; ++i) {
        // 找到与当前ground truth box具有最大IoU的prior box
            int max_iou_index = max_element(ious[i].begin(), ious[i].end()) - ious[i].begin();
        //如果有可匹配的 piror box 则匹配
            if(ious[i][max_iou_index] >= iou_threshold)
                matches[picture_[0][max_iou_index]] = picture_[1][i] + 1;  // 匹配结果中加1，用于区分背景类（0）
            else{
                continue;
            }
        // 将该prior box与其他ground truth box的IoU置为0，避免重复匹配
            for (int j = 0; j < num_x; ++j) {
                ious[j][max_iou_index] = 0.0;
            }
        }

        // 根据第二个原则进行匹配
        // for (int j = 0; j < num_b; ++j) {
        //     // 如果该prior box已经与某个ground truth匹配，则跳过
        //     if (matches[picture_[0][j]] != 0) {
        //         continue;
        //     }

        //     // 找到当前prior box未匹配的情况下，与其具有最大IoU的ground truth box
        //     float max_iou = 0.0;
        //     int max_iou_index = -1;

        //     for (int i = 0; i < num_x; ++i) {
        //         if (ious[i][j] > max_iou) {
        //             max_iou = ious[i][j];
        //             max_iou_index = i;
        //         }
        //     }

        //     // 如果IoU大于阈值，则将该prior box与该ground truth box进行匹配
        //     if (max_iou > iou_threshold) {
        //         matches[picture_[0][j]] = picture_[1][max_iou_index] + 1;  // 匹配结果中加1，用于区分背景类（0）

        //         // 将该ground truth box与其他prior boxes的IoU置为0，避免重复匹配
        //         for (int k = 0; k < num_b; ++k) {
        //             ious[max_iou_index][k] = 0.0;
        //         }
        //     }
        // }
        //  cout << "匹配结果为：" << endl;
        // for(int i = 0;i< b.size();i++){
        //     cout << matches[i] << "\t";
        // }
        // cout << endl;
        // for (int j = 0; j < num_b; ++j) {
        //     if(matches[picture_[0][j]])
        //         std::cout << "Prior Box " << b[picture_[0][j]].filename_v << " matches Ground Truth " << matches[picture_[0][j]] - 1 << std::endl;
        // }

        cls_i = cls_i + num - 1;
    }
    return matches;
}

vector<vector<float>> compute_AP(const vector<BoundingBox>& b,const vector<XMLData>& x){
    if(x.empty())   return {};
    vector<int> matchs =  clss_match(b,x);
    const int num_ob = x.size();
    const int num_pr = b.size();

    vector<piror> ap_(num_pr);
    for(int i = 0;i < num_pr;i++){
        
        ap_[i].confidence = b[i].prob_v;
        ap_[i].TF = matchs[i];
        //cout << ap_[i].confidence << " -------- " << ap_[i].TF << endl;  
    }
    // cout << "AP数组为：" <<  endl;
    // for(int i = 0;i < num_pr;i++){
    //     cout <<ap_[i].confidence << "~~~~" << ap_[i].TF << endl;
    // }
    sort(ap_.begin(),ap_.end(),compareByAge);
    // cout << "AP数组为：" <<  endl;
    // for(int i = 0;i < num_pr;i++){
    //     cout <<ap_[i].confidence << "~~~~" << ap_[i].TF << endl;
    // }
    
    
    vector<vector<float>> _AP(100,vector<float> (2,0.0));
    for(int i = 0;i < 100;i++){
        int TP = 0;
        int FP = 0;
        int FN = 0;
        float cfdc = 1.0 - i * 0.01;
        int j = 0;
        for(;ap_[j].confidence > cfdc && j < num_pr;j++){
            if(ap_[j].TF)
                TP++;
        }
        
        if(j == 0) continue;
        FP = j - TP;
        FN =  num_ob - TP;
        
        
        _AP[i][1] = 1.0 * TP / (TP + FP);  //准确率
        _AP[i][0] = 1.0 * TP / (TP + FN);   //召回率
        cout << left <<"confidence=" <<setw(8) << cfdc<< "count=" <<setw(5)<< j<<"TP=" << setw(5) << TP << "FP=" << setw(5) << FP<< "FN = " << setw(5) << FN  <<"recall=" << setw(10) <<_AP[i][0] << "\tPrecision=" << _AP[i][1] << endl;  
    }
    
    return _AP;
}

int picture_match(const vector<BoundingBox>& b,const vector<XMLData>& x,vector<vector<int>>& picture_,const string& pictureID){
    int num = 0;
    for(int i = 0;i < b.size();i++){
    //cout << pictureID << "-------" << picture_id(b[i].filename_v) << endl;
        if(!pictureID.compare(picture_id(b[i].filename_v)))
            picture_[0].push_back(i);
    }
    for(int i = 0;i < x.size();i++)
        if(!pictureID.compare(picture_id(x[i].filename))){
            picture_[1].push_back(i);
            num++;
        }
    return num;
}

void removeDuplicates(vector<vector<float>>& data) {
    vector<vector<float>> newData;
    float maxPrecision = 0.0;
    float newrecall = data[0][0];
    for (int i = 0;i < data.size();i++) {
        
        while((newrecall - data[i][0]) <0.1 && (newrecall - data[i][0]) > -0.1){
            if(data[i][1] > maxPrecision)
                maxPrecision = data[i][1];
            i++;
            if(i == data.size()) break;
        }
        newData.push_back({newrecall,maxPrecision});
        if(i == data.size()) break;
        newrecall = data[i][0];
    }

    data = newData;
}

float calculateAPRC(vector<vector<float>>& data) {
    float area = data[0][0] * data[0][1];
    int n = data.size();
    
    // 计算曲线下的面积
    for (int i = 1; i < n; i++) {
        float delta_recall = data[i][0] - data[i - 1][0];
        area += data[i][1] * delta_recall;
    }
    return area;
}

vector<vector<float>> AI100_compute_AP(const vector<BoundingBox>& b,const vector<XMLData>& x){
    if(x.empty())   return {};
    vector<int> matchs =  clss_match(b,x);
    const int num_ob = x.size();
    const int num_pr = b.size();

    vector<piror> ap_(num_pr);
    for(int i = 0;i < num_pr;i++){
        
        ap_[i].confidence = b[i].prob_v;
        ap_[i].TF = matchs[i];
        //cout << ap_[i].confidence << " -------- " << ap_[i].TF << endl;  
    }
    // cout << "AP数组为：" <<  endl;
    // for(int i = 0;i < num_pr;i++){
    //     cout <<ap_[i].confidence << "~~~~" << ap_[i].TF << endl;
    // }
    sort(ap_.begin(),ap_.end(),compareByAge);
    // cout << "AP数组为：" <<  endl;
    // for(int i = 0;i < num_pr;i++){
    //     cout <<ap_[i].confidence << "~~~~" << ap_[i].TF << endl;
    // }
    
    
    vector<vector<float>> _AP(10,vector<float> (2,0.0));
    
    for(int i = 0;i < 7;i++){
        int TP = 0;
        int FP = 0;
        int FN = 0;
        float cfdc = 0.95 - i * 0.05;
        int j = 0;
        for(;ap_[j].confidence > cfdc && j < num_pr;j++){
            if(ap_[j].TF)
                TP++;
        }
        
        if(j == 0) continue;
        FP = j - TP;
        FN =  num_pr - TP;
        
        
        _AP[i][1] = 1.0 * TP / (TP + FP);  //准确率
        _AP[i][0] = 1.0 * TP / (TP + FN);   //召回率
        cout << left <<"confidence=" <<setw(8) << cfdc<< "count=" <<setw(5)<< j<<"TP=" << setw(5) << TP << "FP=" << setw(5) << FP<< "FN = " << setw(5) << FN  <<"recall=" << setw(10) <<_AP[i][0] << "\tPrecision=" << _AP[i][1] << endl;  
    }
    
    return _AP;
}