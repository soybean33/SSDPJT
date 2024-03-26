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

vector<int> readNandFile(); //nand파일(바이너리파일)을 읽어서 벡터로 변환
void writeNandFile(const vector<int>& contents); //nand파일에 쓰기
void writeToResult(const string& value); //result파일에 결과값 쓰기

void writeLBA(int lba, string& value); //쓰기
void readLBA(int lba); //읽기
void eraseLBA(int lba, int size); //지우기


// write명령어
void writeLBA(int lba, string& value) {
    auto contents = readNandFile();
    contents[lba] = stoi(value.substr(2), nullptr, 16);
    writeNandFile(contents);
}

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

// read명령어
void readLBA(int lba) {
    auto contents = readNandFile();
    // int 값을 16진수 문자열로 변환
    stringstream ss;
    ss << "0x" << setfill('0') << setw(8) << hex << contents[lba];
    writeToResult(ss.str());
}

// read명령어
void readLBA(int lba, string data) {
    writeToResult(data);
}

// result.txt 파일에 데이터 쓰기
void writeToResult(const string& value) {
    ofstream file(RESULT);

    if (file.is_open()) {
        file << value << endl;
        file.close();
    }
}

//erase명령어
void eraseLBA(int lba, int size) {
    auto contents = readNandFile();

    for (int i = lba; (i < lba + size) && (i < SIZE); ++i) {
        contents[i] = DEFAULT_DATA; // 범위 내의 데이터를 DEFAULT_DATA로 설정
    }
    writeNandFile(contents);
}
