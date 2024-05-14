#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
 #include <cstdlib>
#include <bits/stdc++.h>

using namespace std;

int main(){

    //string path = "/home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/CAST_basic/benchmarks/regression/" ;
    string path = "/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/benchmarks/mit_cep_gscl_synthesised/" ;
    string path2 = "/home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/reports/" ;

    //string file_name[] = {"s400","s420","s444","s510"};
    //string module_name[] = {"s400","s420","s444","s510"}; //this is also the file name
    //string suffix = ".v";

    string file_name[] = {"sha256","rsa","md5","iir","idft","gps","fir","dft","des3","aes_192"};
    string module_name[] = {"sha256","rsa","md5","iir","idft","gps","fir","dft","des3","aes_192"}; //this is also the file name
    float area_list[] = {27664.29623,1107934.218,16492.1405,15298.24171,643520.5779,468264.7178,9885.804709,645805.1303,8906.84454,459993.3054} ;
    float power_list[] = {2.8274,61.3684,0.6343,1.3618,32.4074,19.0851,1.0018,32.6183,3.2417,18.9379} ;
    float timing_list[] = {4.23,6.94,8.35,4.35,3.44,2.50,3.76,3.44,2.31,1.31} ;
    //float timing_list[] = {0.18,0.15,0.12,0.11,0.1,0.23,0.11,0.1,0.31,0.18} ;
    // float timing_list[] = {-2.32,-5.00,-6.44,-2.43,-1.50,-0.56,-1.83,-1.50,-0.37,0.63} ;
    string suffix = ".v";

    int puf[] = {128,256,512,1024,2048,3072,4096,5120,6144,7168,8192};
    int choice = 0 ;
    cout << "generate puf regression -> enter 1\n generate area power and timing report -> enter 2\n generate compiled report for the same -> enter 3\n full run -> enter 4\n";
    cin >> choice ;
    cout << "entered choice is: " << choice << endl ;
    //string system_command = "";

    int full_run = 0;

    if(choice == 4){
        full_run = 1;
        choice = 0;
    }

    do{
        if(full_run){
            choice++;
        }
        
        if(choice == 4){
            break;
        }

        cout << "Regression Full Run: Stage:" << choice << endl;

        if(choice == 1){
            cout << "starting the regression suite.\n";
            for(int i =0;i < sizeof(module_name)/sizeof(module_name[0]);i++){
                
                    
                    ofstream myfile;
                    myfile.open ("config.txt");

                    myfile << "set_hdl " << path << file_name[i] << suffix << endl ;
                    myfile << "set_top " << file_name[i] << endl ;
                    myfile << "set_clk clk" << endl ;
                    myfile << "set_rst rst" << endl;
                    myfile << "set_library /home/UFAD/a.chatterjee/Desktop/Lab_work/astra_gitlab/astra/libs/freepdk/gscl45nm.lib" << endl ;
                    myfile << "set_tool_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db" << endl ;
                    myfile << "set_dont_touch vivado" << endl ;
                    myfile << "set_insertion_type slack_aware constrained" << endl ;
                    myfile << "set_timing_file " << path << file_name[i] << "_dump.txt" << endl;
                    myfile << "set_regression";
            for(int ite=0;ite<sizeof(puf)/sizeof(puf[0]);ite++){
                myfile << " " << puf[ite];
            }

            myfile << endl;
            
                    myfile << "start" << endl;

                    myfile.close();

                    cout << "./astra -source config.txt" << endl;
                    system("./astra -source config.txt");
                
            
            }
        }else if(choice == 2){

            ofstream myfile;
            system("source /apps/settings");
            string clk_name;
            myfile.open ("regression.tcl");

            myfile << "set_app_var target_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db\n";
            myfile << "set_app_var link_library /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/mit_cep/gscl45nm.db\n";
            
            for(int i =0; i < sizeof(module_name)/sizeof(module_name[0]);i++){
                for(int j=0; j < sizeof(puf)/sizeof(puf[0]);j++){
                    string file_path = path2 + module_name[i] + "/";

                    //read_file -format verilog {/ecel/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/CAST_basic/reports/des3/des3_puf_137.v}
                            
                    myfile << "read_file -format verilog {" << file_path << module_name[i] << "_puf_" << puf[j] << ".v" << "}\n" ;

                    clk_name = module_name[i] == "gps" ? "sys_clk_50" : "clk";

                    myfile << "create_clock -name " << clk_name << " -period 7 {" << clk_name << "}" << endl;
                    myfile << "set_input_delay 0.28 -clock " << clk_name << " [all_inputs]" << endl;
                    myfile << "set_output_delay 0.28 -clock " << clk_name << " [all_outputs]" << endl;


                    myfile << "report_area > " << file_path << module_name[i] << "_" << puf[j] << "_area.rpt\n";
                    myfile << "report_power > "   << file_path <<  module_name[i] << "_" << puf[j] << "_power.rpt\n";
                    myfile << "report_timing > "   << file_path <<  module_name[i] << "_" << puf[j] << "_timing.rpt\n";
                    myfile << "write -f verilog -o "   << file_path <<  module_name[i] << "_" << puf[j] << ".v\n";

                    myfile << "remove_design -designs\n";

                }
            }

            myfile << "exit";
            myfile.close();

            system("dc_shell -f regression.tcl");

        }else if(choice == 3){
            
            fstream myfile;
            ofstream outFile;
            string word, t, q, filename;

            outFile.open("../extracted_data.txt");

            for(int i =0; i < sizeof(module_name)/sizeof(module_name[0]);i++){

                    outFile.width(15);outFile << left << module_name[i];
                    outFile.width(25);outFile << left << area_list[i];
                    outFile.width(25);outFile << left << power_list[i];
                    outFile.width(30);outFile << left << timing_list[i];
                    outFile<<endl;

                for(int j=0; j < sizeof(puf)/sizeof(puf[0]);j++){
                    char puf_number[100];
                    sprintf(puf_number,"%d",puf[j]);
                    string area = path2 + module_name[i] + "/" + module_name[i] + "_" + puf_number + "_area.rpt";
                    string power = path2 + module_name[i] + "/" + module_name[i] + "_" + puf_number + "_power.rpt";
                    string timing = path2 + module_name[i] + "/" + module_name[i]  + "_" + puf_number + "_timing.rpt";

                    

                    vector<string> area_v,power_v,timing_v ;
                    
                    myfile.open(area.c_str());
                    if(myfile.is_open()){
                        cout << "Could open file: ";
                    }else{
                        continue;
                    }
                

                    while (myfile >> word){

                    area_v.push_back(word);
            
                    //    if(word == "Total"){
                    //        myfile >> word;
                    //        if(word == "cell"){
                    //            myfile >> word;
                    //            if(word == "area:"){
                    //                myfile >> word;
                    //                //cout << "File: " << module_name[i] << "_puf_" << puf_number << "_area.rpt" << endl;
                    //                //cout << "Total Cell Area: " << word << endl;
                    //                outFile << word << " ";
                    //            }
                    //        }
                    //    }
            
                    }

                    myfile.close();

                    myfile.open(power.c_str());
                
                    vector<string> wordList;
                    int number = 0 ;

                    while (myfile >> word){
                        
                        power_v.push_back(word);
                        //    wordList.push_back(word);
                        //     number++;
            
                    }

                    //cout<< module_name[i] << "_" << puf_number << "_power.rpt" << endl ;
                // cout << "power: " << wordList[number-3] << " " << wordList[number-2] << endl;

                    //    outFile << wordList[number-3] << " " << wordList[number-2] << "   " ;


                    myfile.close();

                    myfile.open(timing.c_str());

                    while (myfile >> word){
                        timing_v.push_back(word);
                        // if(word == "data"){
                        //     myfile >> word;
                        //     if(word == "arrival"){
                        //        myfile >> word;
                        //         if(word == "time"){
                        //             myfile >> word;
                        //             //cout << "File: " << module_name[i] << "_puf_" << puf_number << "_timing.rpt" << endl;
                        //             //cout << "data arrival time: " << word << endl;
                        //             outFile <<  word << endl;
                        //         }
                        //     }
                        // }
            
                    }

                    //cout<< module_name[i] << "_" << puf_number << "_power.rpt" << endl ;
                // cout << "power: " << wordList[number-3] << " " << wordList[number-2] << endl;


                    myfile.close();

                    cout << j+1 << ") Design Name:" << module_name[i] << "_" << puf_number << ": area:" << area_v.back()<< ": power:" << power_v.back()<< ": timing:" << timing_v.back() <<  endl ;

                    string temp_string;
                    float area_record,power_record,timing_record;
                    if(area_v.back() == "1" && power_v.back() == "1" && timing_v.back() == "1"){
                        temp_string =  module_name[i] + "_" + puf_number + ": ";
                        outFile.width(15) ; outFile << left << temp_string;
                        //area
                        int iterator = 0 ; 
                        while(1){
                            if(iterator > area_v.size() - 1){
                                break;
                            }else if(area_v[iterator] == "Total" && area_v[iterator + 1] == "cell" && area_v[iterator + 2] == "area:"){
                                outFile.width(15); outFile << left << area_v[iterator+3] ;
                                area_record = stof(area_v[iterator+3]) ;
                                iterator++ ;
                            }else{
                                iterator++ ;
                            }
                            
                        }
                        
                        //area percentage

                        stringstream stream;
                        stream << fixed << setprecision(2) << (area_record - area_list[i])/area_list[i] * 100 ;
                        temp_string = stream.str();
                        outFile.width(10);outFile << temp_string ;

                        //power

                        iterator = 0 ; 
                        temp_string = power_v[power_v.size()-3] + " " + power_v[power_v.size()-2] ;
                        power_record = stof(power_v[power_v.size()-3]) ;
                        outFile.width(15); outFile << left  << temp_string;

                        //power percentage
                        stringstream stream1;
                        stream1 << fixed << setprecision(2) << (power_record - power_list[i])/power_list[i] * 100 ;
                        temp_string = stream1.str();
                        outFile.width(10);outFile << temp_string ;

                        //timing
                        iterator = 0 ; 
                        int flag = 1 ;
                        string slackValue = "";
                        while(1){
                            if(iterator > timing_v.size() - 1){
                                break;
                            }else if(flag == 1 && timing_v[iterator] == "data" && timing_v[iterator + 1] == "arrival" && timing_v[iterator + 2] == "time"){
                                temp_string =  timing_v[iterator+3];
                                timing_record = stof(timing_v[iterator+3]) ;
                                outFile.width(10); outFile << left << temp_string ;
                                //outFile << endl ;
                                iterator++ ;
                                flag = 0 ;
                            }else if(!flag){
                                if(timing_v[iterator] == "slack")
                                   slackValue = timing_v[iterator+1];
                                
                                iterator++ ;

                            }else{
                                iterator++ ;
                            }
                            
                        }

                        //timing percentage
                        stringstream stream2;
                        stream2 << fixed << setprecision(2) << (timing_record - timing_list[i])/timing_list[i] * 100 ;
                        temp_string = stream2.str();
                        outFile.width(10);outFile << temp_string ;outFile << " " << slackValue;
                        outFile << endl ;

                    }

                }
                
                outFile << endl ;
                cout <<  endl ;
            }

            outFile.close();

        }


    }while(full_run);




    return 0;
}


//./slot /home/UFAD/a.chatterjee/Desktop/Lab_work/ASTRA/CAST_basic/benchmarks/mit_cep_gscl_synthesised/sha256_gscl45nm.v 6 ../libs/freepdk/gscl45nm.lib sha256 seq 0.5 -noscan clock reset -1 0 0 1 1 137 0
