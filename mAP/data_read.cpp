#include "eval.h"

using namespace std;

string clsss[10] ={"data_wire", "shoes", "trashcan", "carpet", "soft_object","feces", "scale", "stains","fanbase","paper"};
//string EZ_clsss[7] = {"data_wire", "shoes", "trashcan", "carpet", "clothes","feces","paper"};


XMLData parseXMLFile(const std::string& filePath) {
    XMLData xmlData;
    std::wifstream stream(filePath.c_str()); 
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(stream);
    if(!result){
        cout << "加载XML：fail" << endl;
        return xmlData;
    }
    //cout << "加载XML：success" << endl;
    pugi::xml_node root = doc.child("annotation");

    pugi::xml_node filenameNode = root.child("filename");
    xmlData.filename = filenameNode.text().as_string();

    pugi::xml_node nameNode = root.child("object").child("name");
    string name = nameNode.text().as_string();
    for(int i = 0;i < 10;i++) if(name.compare(clsss[i]) == 0) xmlData.cls = i;
    pugi::xml_node bndboxNode = root.child("object").child("bndbox");
    xmlData.xmin = bndboxNode.child("xmin").text().as_int();
    xmlData.xmax = bndboxNode.child("xmax").text().as_int();
    xmlData.ymin = bndboxNode.child("ymin").text().as_int();
    xmlData.ymax = bndboxNode.child("ymax").text().as_int();
    
    return xmlData;
}





int readtext(vector<std::vector<BoundingBox>>& recoginfo_array,string &filename){
    int count = 0;
    std::ifstream file(filename);
    //std::cout << "1" << std::endl;
    if (!file) {
        std::cout << "Error opening recoginfo.txt file." << std::endl;
        exit(1);
    }
    std::cout << "打开文件成功" << std::endl;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        if(token == "")
            break;
        BoundingBox bbox;
        // for(int i = 0;i < tokens.size();i++)
        //     cout << tokens[i] << "\t";
        // cout << endl;
        //std::cout << token << std::endl;

        bbox.filename_v = tokens[1];
        bbox.cls_v = std::stoi(tokens[3]);
        bbox.x_min_v = std::stoi(tokens[9]);
        bbox.y_min_v = std::stoi(tokens[10]);
        int w_v = std::stoi(tokens[11]);
        //cout << tokens[12] << endl;
        int h_v = std::stoi(tokens[12]);
        bbox.x_max_v = bbox.x_min_v + w_v;
        bbox.y_max_v = bbox.y_min_v + h_v;
        bbox.prob_v = std::stof(tokens[7]);
        switch (bbox.cls_v)
        {
        case 0:recoginfo_array[0].push_back(bbox);break;
        case 1:recoginfo_array[1].push_back(bbox);break;
        case 2:recoginfo_array[2].push_back(bbox);break;
        case 3:recoginfo_array[4].push_back(bbox);break;
        case 4:recoginfo_array[8].push_back(bbox);break;
        case 5:recoginfo_array[8].push_back(bbox);break;
        case 6:recoginfo_array[9].push_back(bbox);break;
        }
        // switch (bbox.cls_v)
        // {
        // case 0:recoginfo_array[0].push_back(bbox);break;
        // case 1:recoginfo_array[1].push_back(bbox);break;
        // case 2:recoginfo_array[2].push_back(bbox);break;
        // case 3:recoginfo_array[3].push_back(bbox);break;
        // case 4:recoginfo_array[4].push_back(bbox);break;
        // case 5:recoginfo_array[5].push_back(bbox);break;
        // case 6:recoginfo_array[6].push_back(bbox);break;
        // case 7:recoginfo_array[7].push_back(bbox);break;
        // case 8:recoginfo_array[8].push_back(bbox);break;
        // case 9:recoginfo_array[9].push_back(bbox);break;
        // }
        // count++;

    }
    // for(int i = 0;i < recoginfo_array_tmp.size();i++){
    //     cout << "name = " << recoginfo_array_tmp[i].filename_v << endl;
    //     cout << "xmin = " << recoginfo_array_tmp[i].x_min_v    << endl;
    //     cout << "xmax = " << recoginfo_array_tmp[i].x_max_v    << endl;
    //     cout << "ymin = " << recoginfo_array_tmp[i].y_min_v    << endl;
    //     cout << "ymax = " << recoginfo_array_tmp[i].y_max_v    << endl;
    // }
    // // // Convert the temporary vector to an array
    cout << "预测数据读取成功！" << endl;
    return count;
    // for(int i = 0;i < recoginfo_array.size();i++){
    //     cout << "name = " << recoginfo_array[i].filename_v << endl;
    //     cout << "xmin = " << recoginfo_array[i].x_min_v    << endl;
    //     cout << "xmax = " << recoginfo_array[i].x_max_v    << endl;
    //     cout << "ymin = " << recoginfo_array[i].y_min_v    << endl;
    //     cout << "ymax = " << recoginfo_array[i].y_max_v    << endl;
    // }
}



int getXmlsInFolder(const std::string& folderPath,vector<vector<XMLData>>& files) {
    std::string searchPath = folderPath + "\\*.*" + "xml";
    WIN32_FIND_DATAA fileData;
    HANDLE hFind;
    int count = 0;
    hFind = FindFirstFileA(searchPath.c_str(), &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                
                string filename = fileData.cFileName;
                filename = "\\" + filename;
                filename = folderPath + filename;

                std::wifstream stream(filename.c_str()); 
                pugi::xml_document doc;
                pugi::xml_parse_result result = doc.load(stream);
                if(!result){
                    cout << "加载XML：fail" << endl;
                    return 0;
                }

                pugi::xml_node annotationNode = doc.child("annotation");
                for (pugi::xml_node objectNode : annotationNode.children("object")) {
                    XMLData xmlData;

                    xmlData.filename = annotationNode.child_value("filename");
                    string name = objectNode.child_value("name");
                    
                    for(int i = 0;i < 10;i++) if(name.compare(clsss[i]) == 0) xmlData.cls = i;


                    xmlData.ymin = std::stoi(objectNode.child("bndbox").child_value("ymin"));
                    xmlData.xmin = std::stoi(objectNode.child("bndbox").child_value("xmin"));
                    xmlData.xmax = std::stoi(objectNode.child("bndbox").child_value("xmax"));
                    xmlData.ymax = std::stoi(objectNode.child("bndbox").child_value("ymax"));
                    
                    
                    switch (xmlData.cls)
                    {
                    case 0:files[0].push_back(xmlData);break;
                    case 1:files[1].push_back(xmlData);break;
                    case 2:files[2].push_back(xmlData);break;
                    case 3:files[3].push_back(xmlData);break;
                    case 4:files[4].push_back(xmlData);break;
                    case 5:files[5].push_back(xmlData);break;
                    case 6:files[6].push_back(xmlData);break;
                    case 7:files[7].push_back(xmlData);break;
                    case 8:files[8].push_back(xmlData);break;
                    case 9:files[9].push_back(xmlData);break;
                    }
                    count++;
                }
                // cout << filename<<endl;
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    cout << "xml读取完成！" << endl;
    return count;
}

int getTXTsInFolder(const std::string& folderPath,vector<vector<XMLData>>& recoginfo_array) {
    std::string searchPath = folderPath + "\\*.*" + "txt";
    WIN32_FIND_DATAA fileData;
    HANDLE hFind;
    int count = 0;
    hFind = FindFirstFileA(searchPath.c_str(), &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                
                
                string filename = fileData.cFileName;
                string filename1 = filename;
                filename = "\\" + filename;
                filename = folderPath + filename;

    std::ifstream file(filename);

    std::string line;
    vector<BoundingBox> oneimage;
    bool staattr = 0;
    string image_name;
    //std::cout << "1" << std::endl;
    if (!file) {
        std::cout << "Error opening recoginfo.txt file." << std::endl;
        exit(1);
    }


    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        BoundingBox bbox;
        while (iss >> token) {
            tokens.push_back(token);
        }
        if(token == "")
            break;
        int cls;
    double x_center, y_center, width, height;
    
    cls = stoi(tokens[0]);
    x_center = stof(tokens[1]);
    y_center = stof(tokens[2]);
    width= stof(tokens[3]);
    height= stof(tokens[4]);


    int image_width = 1280;
    int image_height = 720;

    int x_min = static_cast<int>((x_center - width / 2) * image_width);
    int y_min = static_cast<int>((y_center - height / 2) * image_height);
    int x_max = static_cast<int>((x_center + width / 2) * image_width);
    int y_max = static_cast<int>((y_center + height / 2) * image_height);

    XMLData xml_data;
    xml_data.filename = filename1;
    xml_data.cls = cls;
    xml_data.xmin = x_min;
    xml_data.xmax = x_max;
    xml_data.ymin = y_min;
    xml_data.ymax = y_max;
        switch (xml_data.cls) {
    case 0:
        recoginfo_array[0].push_back(xml_data);
        break;
    case 1:
        recoginfo_array[1].push_back(xml_data);
        break;
    case 2:
        recoginfo_array[2].push_back(xml_data);
        break;
    case 3:
        recoginfo_array[4].push_back(xml_data);
        break;
    case 4:
        recoginfo_array[8].push_back(xml_data);
        break;
    case 5:
        recoginfo_array[8].push_back(xml_data);
        break;
    case 6:
        recoginfo_array[9].push_back(xml_data);
        break;
}

    }
            

            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    cout << "xml读取完成！" << endl;
    return count;
}


int AI100_getXmlsInFolder(const std::string& folderPath,vector<vector<XMLData>>& files) {
    std::string searchPath = folderPath + "\\*.*" + "xml";
    WIN32_FIND_DATAA fileData;
    HANDLE hFind;
    int count = 0;
    hFind = FindFirstFileA(searchPath.c_str(), &fileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                
                string filename = fileData.cFileName;
                filename = "\\" + filename;
                filename = folderPath + filename;

                std::wifstream stream(filename.c_str()); 
                pugi::xml_document doc;
                pugi::xml_parse_result result = doc.load(stream);
                if(!result){
                    cout << "加载XML：fail" << endl;
                    return 0;
                }
                XMLData xmlData;
                
                xmlData.ymin = 640;
                xmlData.cls = 10;
                pugi::xml_node annotationNode = doc.child("annotation");
                for (pugi::xml_node objectNode : annotationNode.children("object")) {
                    

                    xmlData.filename = annotationNode.child_value("filename");
                    string name = objectNode.child_value("name");
                    int cls;
                    for(int i = 0;i < 10;i++) if(name.compare(clsss[i]) == 0) cls = i;
                    if(cls > 4 && cls < 8) continue;

                    int ymin = std::stoi(objectNode.child("bndbox").child_value("ymin"));
                    
                    if(ymin > xmlData.ymin) continue; 

                    xmlData.ymin = std::stoi(objectNode.child("bndbox").child_value("ymin"));
                    xmlData.xmin = std::stoi(objectNode.child("bndbox").child_value("xmin"));
                    xmlData.xmax = std::stoi(objectNode.child("bndbox").child_value("xmax"));
                    xmlData.ymax = std::stoi(objectNode.child("bndbox").child_value("ymax"));
                    xmlData.cls = cls;
                    
                }
                if((xmlData.cls > 4 && xmlData.cls < 8 ) || xmlData.cls == 10) continue;
                switch (xmlData.cls)
                {
                case 0:files[0].push_back(xmlData);break;
                case 1:files[1].push_back(xmlData);break;
                case 2:files[2].push_back(xmlData);break;
                case 3:files[3].push_back(xmlData);break;
                case 4:files[4].push_back(xmlData);break;
                case 8:files[8].push_back(xmlData);break;
                case 9:files[9].push_back(xmlData);break;
                }
                count++;


                // cout << filename<<endl;
            }
        } while (FindNextFileA(hFind, &fileData) != 0);
        FindClose(hFind);
    }
    cout << "xml读取完成！" << endl;
    return count;
}

int AI100_readtext(vector<std::vector<BoundingBox>>& recoginfo_array,string &filename,int type){
    int count = 0;
    std::ifstream file(filename);
    //std::cout << "1" << std::endl;
    if (!file) {
        std::cout << "Error opening recoginfo.txt file." << std::endl;
        exit(1);
    }
    std::cout << "打开文件成功" << std::endl;
    std::string line;
    vector<BoundingBox> oneimage;
    bool staattr = 0;
    string image_name;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        BoundingBox bbox;
        while (iss >> token) {
            tokens.push_back(token);
        }
        if(token == "")
            break;

        if(type == 0){
            bbox.filename_v = tokens[0];
            bbox.x_min_v = stoi(tokens[1].substr(2));
            bbox.y_min_v = stoi(tokens[2].substr(2));
            int w_v = stoi(tokens[3].substr(2));
            int h_v = stoi(tokens[4].substr(2));
            bbox.x_max_v = bbox.x_min_v + w_v;
            bbox.y_max_v = bbox.y_min_v + h_v;
            bbox.cls_v = stoi(tokens[5].substr(2));
            bbox.prob_v = 0.001 * stoi(tokens[7].substr(6));
            if(bbox.x_min_v == 0 && bbox.y_min_v == 0 && bbox.x_max_v == 0 && bbox.y_max_v == 0) continue;
            switch (bbox.cls_v)
            {
            case 0:recoginfo_array[0].push_back(bbox);break;
            case 1:recoginfo_array[1].push_back(bbox);break;
            case 2:recoginfo_array[2].push_back(bbox);break;
            case 3:recoginfo_array[3].push_back(bbox);break;
            case 4:recoginfo_array[4].push_back(bbox);break;
            case 5:recoginfo_array[8].push_back(bbox);break;
            case 6:recoginfo_array[9].push_back(bbox);break;
            }
        count++;
        }
        if(type == 1){
            if(staattr == 0){
            image_name = tokens[1];
            staattr = 1;
            }
        bbox.filename_v = tokens[1];
        bbox.cls_v = std::stoi(tokens[3]);
        bbox.x_min_v = std::stoi(tokens[9]);
        bbox.y_min_v = std::stoi(tokens[10]);
        int w_v = std::stoi(tokens[11]);
        int h_v = std::stoi(tokens[12]);
        bbox.x_max_v = bbox.x_min_v + w_v;
        bbox.y_max_v = bbox.y_min_v + h_v;
        bbox.prob_v = std::stof(tokens[7]);
        if(bbox.cls_v > 4 && bbox.cls_v < 8) continue;

        if(bbox.filename_v.compare(image_name)){
            BoundingBox ABC = one_img(oneimage);
            switch (ABC.cls_v)
{
case 0:recoginfo_array[0].push_back(ABC);break;
case 1:recoginfo_array[1].push_back(ABC);break;
case 2:recoginfo_array[2].push_back(ABC);break;
case 3:recoginfo_array[3].push_back(ABC);break;
case 4:recoginfo_array[4].push_back(ABC);break;
case 5:recoginfo_array[5].push_back(ABC);break;
case 6:recoginfo_array[6].push_back(ABC);break;
case 7:recoginfo_array[7].push_back(ABC);break;
case 8:recoginfo_array[8].push_back(ABC);break;
case 9:recoginfo_array[9].push_back(ABC);break;
}
            count++;
            image_name = bbox.filename_v;
            oneimage.clear();
        }
        oneimage.push_back(bbox);
    }
    }
    // for(int i = 0;i < recoginfo_array_tmp.size();i++){
    //     cout << "name = " << recoginfo_array_tmp[i].filename_v << endl;
    //     cout << "xmin = " << recoginfo_array_tmp[i].x_min_v    << endl;
    //     cout << "xmax = " << recoginfo_array_tmp[i].x_max_v    << endl;
    //     cout << "ymin = " << recoginfo_array_tmp[i].y_min_v    << endl;
    //     cout << "ymax = " << recoginfo_array_tmp[i].y_max_v    << endl;
    // }
    // // // Convert the temporary vector to an array
    cout << "预测数据读取成功！" << endl;
    return count;
    // for(int i = 0;i < recoginfo_array.size();i++){
    //     cout << "name = " << recoginfo_array[i].filename_v << endl;
    //     cout << "xmin = " << recoginfo_array[i].x_min_v    << endl;
    //     cout << "xmax = " << recoginfo_array[i].x_max_v    << endl;
    //     cout << "ymin = " << recoginfo_array[i].y_min_v    << endl;
    //     cout << "ymax = " << recoginfo_array[i].y_max_v    << endl;
    // }
}

BoundingBox one_img(vector<BoundingBox>& a){
    int num = a.size();
    BoundingBox bbox_min;
    bbox_min.y_min_v = 640;
    for(int i = 0;i < num;i++){
        if(a[i].y_min_v < bbox_min.y_min_v)
            bbox_min = a[i];
    }
    return bbox_min;

}