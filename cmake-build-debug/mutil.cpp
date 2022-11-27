//
// Created by 12466 on 2022/11/27.
//
#include <thread>
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char **argv){

    if (argc < 2)
        return 1;

//    int thread_num = std::stoi(argv[1]);
    int thread_num = 3;

    std::vector<std::thread> threads;

    for (int i = 0; i < thread_num; ++i) {
        threads.emplace_back([](){
            system("mem.exe");
        });
    }

    for (auto &t : threads)
        t.join();

}