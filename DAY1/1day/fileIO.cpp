#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

const string NAND = "nand.txt";
const string RESULT = "result.txt";
const int SIZE = 100;
const int DEFAULT_DATA = 0x00000000;

// 바이너리 파일에서 모든 내용을 읽어서 벡터로 반환
vector<int> readNandFile() {
    vector<int> datas(SIZE, DEFAULT_DATA);
    ifstream file(NAND, ios::binary);

    if (file.is_open()) {
        for (int i = 0; i < SIZE; ++i) {
            file.read(reinterpret_cast<char*>(&datas[i]), sizeof(datas[i]));
            if (file.fail()) {
                break; // 파일 끝에 도달하거나 읽기 오류가 발생했다면 중단
            }
        }
        file.close();
    }
    return datas;
}

// nand.txt 파일에 바이너리 형태로 데이터 쓰기
void writeNandFile(const vector<int>& contents) {
    ofstream file(NAND, ios::binary);

    if (file.is_open()) {
        for (const auto& data : contents) {
            file.write(reinterpret_cast<const char*>(&data), sizeof(data));
        }
        file.close();
    }
}

// write명령어
void writeLBA(int lba, string& value) {
    auto contents = readNandFile();
    contents[lba] = stoi(value.substr(2), nullptr, 16);
    writeNandFile(contents);
}

// result.txt 파일에 데이터 쓰기
void writeToResult(const string& value) {
    ofstream file(RESULT);

    if (file.is_open()) {
        file << value << endl;
        file.close();
    }
}

// read명령어
void readLBA(int lba) {
    auto contents = readNandFile();
    // int 값을 16진수 문자열로 변환
    stringstream ss;
    ss << "0x" << setfill('0') << setw(8) << hex << contents[lba];
    writeToResult(ss.str());
}
