
#include "Driver.h"
#include <bitset>
#include <algorithm>
#include <sstream>


void Stringsplit(string str, const char split,vector<string>& res)
{
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		res.push_back(token);
	}
}

void read_data(string filename, std::vector<vector<int>> &vec_list){
    ifstream ifs;
    ifs.open(filename, ios::in);

    if (!ifs.is_open())
    {
        cout << "read fail." << endl;
        return ;
    }


    string line;
    
    while(getline(ifs, line)){
        vector<string> strList;
        Stringsplit(line, ' ', strList); 
        vector<int> intList;
        for (auto s :strList){
            intList.push_back(stoi(s));
        }
        vec_list.push_back(intList);
        // ==output test==
        // for (auto s : intList){
        //     cout << s << " ";
        // }
        // cout << endl;
        // ===============
    }

    

}

void Generate_Inst(string output_dir, int PE_num, string name, int mission_type, int row_size, std::vector<vector<int>> lines_position_all, std::vector<vector<int>> lines_num_all){

    int visual_block_shape = 1;
    int pos_block_num = 2;
    if(mission_type==VINS_MONO){
        visual_block_shape = 1;
        pos_block_num = 2;
    }else if(mission_type==VINS_fusion){
        visual_block_shape = 1;
        pos_block_num = 2;
    }else if(mission_type==SFM_2){
        visual_block_shape = 3;
        pos_block_num = 1;
    }else if(mission_type==SFM_3){
        visual_block_shape = 3;
        pos_block_num = 2;
    }
    std::vector<int> test_num_list = lines_num_all[0];
    vector<int>::iterator vis_iter;
    vis_iter=find(test_num_list.begin(),test_num_list.end(),visual_block_shape);
    int vis_index = std::distance(test_num_list.begin(),vis_iter);

    std::vector<int> pos_index;
    for (int i=0; i<test_num_list.size(); i++){
        if(i != vis_index){
            pos_index.push_back(i);
            cout << "indexp: "<< i<<endl;
        }
    }

    cout << "index: "<< vis_index<<endl;


    

    ofstream I_file(output_dir+std::string("Inst_")+name+std::string(".txt"), ios::binary);

    cout <<"open fstream"<< row_size<<endl;
    
    vector<string> code_list;
    vector<int> code_pos;
    string uni_code;
    vector<int> pos_list;
    vector<int> position_col(pos_block_num);
    vector<int> pre_position_col(pos_block_num);
    int ETF_block_num = 0;
    int vision_col = 0;
    int pre_vision_col = 0;
    int row_num= 1;
    int L_pi = 0;
    int E_pi = 0;
    int G_pi = 0;
    int count = 0;
    
    for(int row_index=0; row_index < row_size; ++row_index){
        std::vector<int> lines_position,lines_num;
        if(row_index != row_size - 1){
            lines_position = lines_position_all[row_index];
            lines_num = lines_num_all[row_index];
        }
        else{
            lines_position = lines_position_all[0];
            lines_num = lines_num_all[0];
        }
        
        cout << "data_now: "<< lines_num[0]<< lines_num[1]<<lines_position[0]<<lines_position[1]<<endl;

        if(1){
            count += 1;
            cout << "count: "<< count <<"=====writing Instruction=====" << endl;
            pre_vision_col = vision_col;
            vision_col = lines_position[vis_index];
            
            for(int i=0; i<pos_block_num; i++){
                pre_position_col[i] = position_col[i];
                position_col[i] = lines_position[pos_index[i]];
            }
            
            // pre_position_col[0] = position_col[0];
            // pre_position_col[1] = position_col[1];
            // position_col[0] = lines_position[0];
            // position_col[1] = lines_position[1];

            if(count!=1){
                for(int i=0;i<2;i++){
                    if(i<pos_block_num){
                        cout << "pos_now: " << pre_position_col[i] << endl;
                        vector<int>::iterator it,it2;
                        it=find(pos_list.begin(),pos_list.end(),pre_position_col[i]);
                        if(it==pos_list.end()){
                            cout<<"no matched position"<<endl;
                            for(int j=0;j<code_list.size();j++){
                                code_list[j]+='0'; 
                            }
                        }
                        else{
                            it2=find(code_pos.begin(),code_pos.end(),pre_position_col[i]);
                            if(it2==code_pos.end()){
                                cout<<"no excited position, create_new"<<endl;
                                int index = std::distance(pos_list.begin(),it);
                                string new_code(uni_code);
                                new_code.replace(index,1,"1");
                                new_code += '1';
                                cout<<"new_code: "<< new_code <<endl;
                                for(int j=0;j<code_list.size();j++){
                                    code_list[j]+='0'; 
                                }
                                code_list.push_back(new_code);
                                code_pos.push_back(pre_position_col[i]);
                                
                            }
                            else{
                                cout<<"find excited position, modified code"<<endl;
                                int index = std::distance(code_pos.begin(),it2);
                                for(int j=0;j<code_list.size();j++){
                                    if(j==index){code_list[j] += '1';}
                                    else{code_list[j]+='0';}   
                                }
                                cout <<"index: "<< index <<endl;
                                // for(int j=0;j<code_list.size();j++){
                                //     string code_now = code_list[j];
                                //     cout<< code_now << endl;
                                // }
                            }
                            ETF_block_num -= 1;
                        }
                        pos_list.push_back(pre_position_col[i]);
                        ETF_block_num += 1;
                    }
                    else{
                        for(int j=0;j<code_list.size();j++){
                            code_list[j]+='0'; 
                        }
                    }
                    uni_code += "0";
                }
            }

            cout <<"vision_col: "<< pre_vision_col <<" "<< vision_col << endl;

            if(count != 1){
                if((vision_col == pre_vision_col)){
                    row_num += 1;
                }else{
                    // int lines = row_num / 2;
                    int lines = row_num;
                    row_num = 1;
                    string len = std::bitset<13>(lines).to_string();
                    string L_pi_s = std::bitset<4>(L_pi).to_string();
                    string Load_Instruction = "0001"+L_pi_s+"000000001"+len+"11";
                    I_file << Load_Instruction << endl;
                    cout << "Load_Instruction: " << Load_Instruction <<endl;
                    //
                    for(int j=0; j<code_list.size(); j++){
                        string last = j==(code_list.size()-1) ? "1" : "0";
                        string code = code_list[j];
                        string ADD_Instruction;
                        if(code.size()<=18){
                            ADD_Instruction = string("0010")+"000000001"+last+code;
                            for(int k=0;k<(18-code.size());k++){ADD_Instruction+='0';}
                            I_file << ADD_Instruction << endl;
                            cout << "ADD_Instruction: " << ADD_Instruction <<endl;
                        }
                        else{
                            while(1){
                                if(code.size()<=18){
                                    ADD_Instruction = string("0010")+"000000001"+last+code;
                                    for(int k=0;k<(18-code.size());k++){ADD_Instruction+='0';}
                                    I_file << ADD_Instruction << endl;
                                    cout << "ADD_Instruction: " << ADD_Instruction <<endl;
                                    break;
                                }else{
                                    string code_used = code.substr(0,18);
                                    ADD_Instruction = string("0010")+"000000001"+"0"+code_used;
                                    code=code.substr(18,code.size());
                                    I_file << ADD_Instruction << endl;
                                    cout << "ADD_Instruction: " << ADD_Instruction <<endl;
                                }
                            }
                        }
                    }
                    
                    //
                    string ETF_num = std::bitset<24>(ETF_block_num).to_string();
                    string E_pi_s = std::bitset<4>(E_pi).to_string();
                    string multi_Instruction = "0011" + E_pi_s  + ETF_num;
                    I_file << multi_Instruction << endl;
                    cout << "multi_Instruction: " << multi_Instruction <<endl;
                    string G_pi_s = std::bitset<4>(G_pi).to_string();
                    string GTG_Instruction = "0100" + G_pi_s  + ETF_num;
                    I_file << GTG_Instruction << endl;
                    cout << "GTG_Instruction: " << GTG_Instruction <<endl;
                    //
                    code_list.clear();
                    code_pos.clear();
                    pos_list.clear();
                    ETF_block_num = 0;
                    uni_code.clear();
                    L_pi =  (L_pi+1)%PE_num;
                    E_pi =  (E_pi+1)%PE_num;
                    G_pi =  (G_pi+1)%PE_num;
                }
            } 
            // I_file << endl;
        }
    }

}


int main(int argc, char *argv[]){

    int Mission_type = VINS_MONO;
    string filename1 = "test_pos.txt";
    string filename2 = "test_num.txt";
    string filename_out = "0";
    string output_dir = "../data/";
    int PE_num = 6;

    cout<<argc<<endl;
    if(argc>1){
        string param = std::string(argv[1]);
        if(argc>4){
            filename1 = std::string(argv[2]);
            filename2 = std::string(argv[3]);
            filename_out = std::string(argv[4]);
        }
        if(param=="VINS_MONO" || param=="1"){
            Mission_type = VINS_MONO;
        }else if(param=="VINS_fusion" || param=="2"){
            Mission_type = VINS_fusion;
        }else if(param=="SFM_2" || param=="3"){
            Mission_type = SFM_2;
        }else if(param=="SFM_3" || param=="4"){
            Mission_type = SFM_3;
        }
        if(argc>5){
            PE_num = stoi(argv[5]);
        }
    }
    cout<<"Mission: "<<Mission_type<<endl;


    std::vector<vector<int>> lines_position,lines_num;

    read_data(filename1,lines_position);
    read_data(filename2,lines_num);

    cout<<"num: "<<lines_num.size()<<endl;

    Generate_Inst(output_dir, PE_num, filename_out, Mission_type, lines_num.size(), lines_position, lines_num);

    for (auto s : lines_position){
        for (auto a : s){
            cout << a << " ";
        }
        
    }
    cout << endl;

    return 0;
}