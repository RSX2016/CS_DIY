// VM2Hack.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <vector>
#include <io.h>
#include "Command.h"
#include "CodeWriter.h"
#include "Parser.h"

void GetAllFiles(std::string path, std::vector<std::string>& files);

int main(int argc, char *argv[])
{
    std::cout << "Hello World!\n";
    
    std::string dir = "D:\\MyWorkspace\\CS_DIY\\CS_DIY\\Architecture\\Coursera_Nand2Tetris\\MyHomework\\MyProject07\\StackArithmetic\\SimpleAdd\\";
    std::vector<std::string> file_arr;
    file_arr.push_back("SimpleAdd.vm");

    if (argc > 1) {
        std::string arg_filenameOrPath = argv[1];
        file_arr.clear();

        if (arg_filenameOrPath.find(".vm") != -1) {
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
                if (allFiles[i].find(".vm") != -1) {
                    file_arr.push_back(allFiles[i]);
                }
            }
        }
    }




    std::cout << "dir = " << dir << std::endl;
    std::cout << "file = " << file_arr[0] << std::endl;



    CodeWriter codeWriter(dir + "out.asm");

    for (int i = 0; i < file_arr.size(); ++i) {
        int pos = file_arr[i].find(".");
        codeWriter.setFileName(file_arr[i].substr(0, pos));

        Parser parser(dir + file_arr[i]);
        Command cmd;
        
        while (parser.hasMoreCommands()) {
            if (parser.advance(cmd)) {
                codeWriter.cmdWrite(cmd);
            }
        }
    }


    //for (int i = 2; i < argc; ++i) {
    //    std::string cmd_arg = argv[i];
    //    codeWriter.testWrite(cmd_arg + "\n");
    //    std::cout << cmd_arg << std::endl;
    //}
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





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
