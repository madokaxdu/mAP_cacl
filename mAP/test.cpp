#include "data_read.cpp"
#include "compute.cpp"

using namespace std;

/*
*       readtext 是读取pirror bbox的函数，这个函数有两种形态：
*       1、默认形态为读取亿智与AMICRO输出，仅需修改filename1。
*       2、涉及到与AI100的比较时，用AI100_readtext(recoginfo_array,filename1,type)函数，此函数type有0\1两种
*           2-0、AI100 type为0
*           2-1、与AI100比较的type为1
*       
*       getXmlsInFolder是读取xml形式的gt bbox函数。同样是两种形态
*       1、默认为getXmlsInFolder读取全部GT，仅需修改folderPath。
*       2、涉及到AI100时，用AI100_AI100_getXmlsInFolder。
*/

int main() {
    vector<vector<BoundingBox>> recoginfo_array(10);
    string filename1 = "C:\\Users\\18792\\Desktop\\data\\five_class_test_data_revise\\recoginfo.txt";
    int piror_count = readtext(recoginfo_array,filename1);


    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    string folderPath = "C:\\Users\\18792\\Desktop\\data\\five_class_test_data_revise\\labels";
    vector<vector<XMLData>> xml_data(10);
    //int bbox_count = getFilesInFolder(folderPath,xml_data,1);
    int bbox_count = getTXTsInFolder(folderPath,xml_data);
    int gt_count = 0;
    for(int i = 0;i < 10;i++){
        cout << "第 " << clsss[i] << " 类的有:" << xml_data[i].size() << endl;
        gt_count += xml_data[i].size(); 
    }
    cout << "共有" << gt_count << "个GT" <<endl;
    // int a1 = xml_data[0].size();
    // int a2 = xml_data[1].size();
    // int a3 = xml_data[2].size();
    // int a4 = xml_data[3].size();
    // int a5 = xml_data[4].size();
    // int a6 = xml_data[5].size();
    // int a7 = xml_data[9].size();
    cout << "ground-truth数量为：" << bbox_count << endl;
    
    // for(int j = 0;j < 11;j++){
    //     cout << clsss[j] << "类的有："  << endl;
    //     if(!xml_data[j].size()){
    //         cout << "此类无" << endl;
    //         continue;
    //     }
    // for(int i = 0;i < xml_data[j].size();i++){
    // cout << "name = " << xml_data[j][i].filename << "\t";
    // cout << "cls = "  << xml_data[j][i].cls      << "\t";
    // cout << "xmin = " << xml_data[j][i].xmin     << "\t";
    // cout << "xmax = " << xml_data[j][i].xmax     << "\t";
    // cout << "ymin = " <<xml_data[j][i].ymin      << "\t";
    // cout << "ymax = " <<xml_data[j][i].ymax      << "\t";
    // cout << endl;
    // }
    // }
    
    float sumAP = 0.0;
    float weight_mAP = 0.0;
    int count = 0;
    vector<float> mAP_arr(10);
    for(int i = 0;i < 10;i++){
        //if(i == 0 || i == 6) continue;
        cout << i << "：" << clsss[i] << "类为：" << endl; 
        if(recoginfo_array[i].size() == 0){ cout << "未检测到数据mAP = 0" << endl;  continue;}
        vector<vector<float>> AP = compute_AP(recoginfo_array[i],xml_data[i]);
        if(AP.empty()){
            cout << "无数据。" << endl;
            continue;
        }
        cout << "AP数组为：" <<  endl;
        //  for(int i = 0;i <10;i++){
        //      cout <<AP[i][0] << "~~~~" << AP[i][1] << endl;
        //  }
        sort(AP.begin(),AP.end(),compareByRecall);
        //removeDuplicates(AP);
        //  cout << "处理后AP数组为：" <<  endl;
        for(int i = 0;i < AP.size();i++){
            cout <<left << setw(10)<<AP[i][0] << "\t" << AP[i][1] << endl;
        
        }
            mAP_arr[i] = calculateAPRC(AP);
            cout << "第 " << clsss[i] << " 类的AP为:" << mAP_arr[i] << endl;
            sumAP += mAP_arr[i];
            weight_mAP += mAP_arr[i] * xml_data[i].size() / bbox_count;
            count++;
            cout << "**********************************************************" << endl;
    }

    cout << "************************************************************"<< endl;
    for(int i = 0;i < 10;i++){
        cout << "第 " << clsss[i] << " 类的AP为:" << mAP_arr[i] << endl;
    }
    cout << "mAP为：" << sumAP / count << endl;

    
    return 0;
}

