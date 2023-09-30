#include "data_read.cpp"
#include "compute.cpp"

using namespace std;

/*
*       readtext �Ƕ�ȡpirror bbox�ĺ��������������������̬��
*       1��Ĭ����̬Ϊ��ȡ������AMICRO����������޸�filename1��
*       2���漰����AI100�ıȽ�ʱ����AI100_readtext(recoginfo_array,filename1,type)�������˺���type��0\1����
*           2-0��AI100 typeΪ0
*           2-1����AI100�Ƚϵ�typeΪ1
*       
*       getXmlsInFolder�Ƕ�ȡxml��ʽ��gt bbox������ͬ����������̬
*       1��Ĭ��ΪgetXmlsInFolder��ȡȫ��GT�������޸�folderPath��
*       2���漰��AI100ʱ����AI100_AI100_getXmlsInFolder��
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
        cout << "�� " << clsss[i] << " �����:" << xml_data[i].size() << endl;
        gt_count += xml_data[i].size(); 
    }
    cout << "����" << gt_count << "��GT" <<endl;
    // int a1 = xml_data[0].size();
    // int a2 = xml_data[1].size();
    // int a3 = xml_data[2].size();
    // int a4 = xml_data[3].size();
    // int a5 = xml_data[4].size();
    // int a6 = xml_data[5].size();
    // int a7 = xml_data[9].size();
    cout << "ground-truth����Ϊ��" << bbox_count << endl;
    
    // for(int j = 0;j < 11;j++){
    //     cout << clsss[j] << "����У�"  << endl;
    //     if(!xml_data[j].size()){
    //         cout << "������" << endl;
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
        cout << i << "��" << clsss[i] << "��Ϊ��" << endl; 
        if(recoginfo_array[i].size() == 0){ cout << "δ��⵽����mAP = 0" << endl;  continue;}
        vector<vector<float>> AP = compute_AP(recoginfo_array[i],xml_data[i]);
        if(AP.empty()){
            cout << "�����ݡ�" << endl;
            continue;
        }
        cout << "AP����Ϊ��" <<  endl;
        //  for(int i = 0;i <10;i++){
        //      cout <<AP[i][0] << "~~~~" << AP[i][1] << endl;
        //  }
        sort(AP.begin(),AP.end(),compareByRecall);
        //removeDuplicates(AP);
        //  cout << "�����AP����Ϊ��" <<  endl;
        for(int i = 0;i < AP.size();i++){
            cout <<left << setw(10)<<AP[i][0] << "\t" << AP[i][1] << endl;
        
        }
            mAP_arr[i] = calculateAPRC(AP);
            cout << "�� " << clsss[i] << " ���APΪ:" << mAP_arr[i] << endl;
            sumAP += mAP_arr[i];
            weight_mAP += mAP_arr[i] * xml_data[i].size() / bbox_count;
            count++;
            cout << "**********************************************************" << endl;
    }

    cout << "************************************************************"<< endl;
    for(int i = 0;i < 10;i++){
        cout << "�� " << clsss[i] << " ���APΪ:" << mAP_arr[i] << endl;
    }
    cout << "mAPΪ��" << sumAP / count << endl;

    
    return 0;
}

