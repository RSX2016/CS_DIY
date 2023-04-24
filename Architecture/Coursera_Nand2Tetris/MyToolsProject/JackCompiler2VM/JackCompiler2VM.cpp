// JackCompiler2VM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include "Token.h"
#include "JackTokenizer.h"
#include "TokenXMLWriter.h"
#include "CompilationEngine.h"

void GetAllFiles(std::string path, std::vector<std::string>& files);
void flexmain();


int main(int argc, char* argv[])
{

    //std::cout << "call flexmain() A" << std::endl;
    //flexmain();
    //std::cout << "call flexmain() B" << std::endl;

    //return 0;

    std::string dir = "D:\\MyWorkspace\\CS_DIY_GIT\\CS_DIY\\Architecture\\Coursera_Nand2Tetris\\MyHomework\\MyProject11\\ComplexArrays\\";
    std::vector<std::string> file_arr;
    file_arr.push_back("Main.jack");
    //file_arr.push_back("SquareGame.jack");
    //file_arr.push_back("Square.jack");
    if (argc > 1) {
        std::string arg_filenameOrPath = argv[1];
        file_arr.clear();

        if (arg_filenameOrPath.find(".jack") != -1) {
            std::cout << "input is a file" << std::endl;
            int pos = arg_filenameOrPath.find_last_of("\\");
            file_arr.push_back(arg_filenameOrPath.substr(pos + 1, arg_filenameOrPath.length()));
            dir = arg_filenameOrPath.substr(0, pos + 1);
        }
        else {
            std::cout << "input is a dir" << std::endl;
            dir = arg_filenameOrPath;
            std::vector<std::string> allFiles;
            GetAllFiles(arg_filenameOrPath, allFiles);
            for (int i = 0; i < allFiles.size(); ++i) {
                if (allFiles[i].find(".jack") != -1) {
                    file_arr.push_back(allFiles[i]);
                }
            }

            std::cout << "file_arr.size() == " << file_arr.size() << std::endl;
        }
    }


    for (int i = 0; i < file_arr.size(); ++i) {
        std::cout << "dir = " << dir << std::endl;
        std::cout << "file = " << file_arr[i] << std::endl;

        JackTokenizer jackTolenize(dir + file_arr[i]);
        //TokenXMLWriter tokenXmlWriter;
        //tokenXmlWriter.start(dir + file_arr[0] + "_out.xml", jackTolenize);

        CompliationEngine compliationEngine;
        compliationEngine.start(dir + file_arr[i].substr(0, file_arr[i].length() - 5), jackTolenize);
    }


}

// 可在这个函数中再加一个format格式参数，push到vector前判断下文件名后缀，仅保留指定格式
void GetAllFiles(std::string path, std::vector<std::string>& files) {
    // 用来存储文件信息的结构体，在头文件 <io.h>
    struct _finddata_t fileinfo;  // _finddata_t 这是一个struct类，c++中可以不要前面的struct的

    intptr_t hFile = 0;

    std::string p;  // 不在这p(path)初始化
    // 第一次查找
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
        do {
            // 如果找到的是文件夹
            if ((fileinfo.attrib & _A_SUBDIR)) {
                // 不想进入文件夹，就在这里continue
                if (std::strcmp(fileinfo.name, ".") != 0 && std::strcmp(fileinfo.name, "..") != 0) {
                    // 进入查找
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                    GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                }
            }
            // 如果找到的不是文件夹
            else {
                // 保存的是文件名
                files.push_back(p.assign(fileinfo.name));
                // 也可以是保存绝对路径
                // files.push_back(p.assign(path).append("\\").append(fileinfo.name));  
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        // 结束查找
        _findclose(hFile);
    }

}


