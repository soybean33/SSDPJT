#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class FileManager {
private:
    const string NAND = "nand.txt";
    const string RESULT = "result.txt";
    const int SIZE = 100;
    const int DEFAULT_DATA = 0x00000000;

    // 싱글톤 인스턴스
    static FileManager* instance;

    // Private 생성자
    FileManager() {}

    // 벡터로 변환된 nand파일 읽기
    vector<int> readNandFile() {
        vector<int> datas(SIZE, DEFAULT_DATA);
        ifstream file(NAND, ios::binary);

        if (file.is_open()) {
            for (int i = 0; i < SIZE; ++i) {
                file.read(reinterpret_cast<char*>(&datas[i]), sizeof(datas[i]));
                if (file.fail()) {
                    break;
                }
            }
            file.close();
        }
        return datas;
    }

    // nand파일에 쓰기
    void writeNandFile(const vector<int>& contents) {
        ofstream file(NAND, ios::binary);

        if (file.is_open()) {
            for (const auto& data : contents) {
                file.write(reinterpret_cast<const char*>(&data), sizeof(data));
            }
            file.close();
        }
    }

    // 결과값을 result파일에 쓰기
    void writeToResult(const string& value) {
        ofstream file(RESULT);

        if (file.is_open()) {
            file << value << endl;
            file.close();
        }
    }

public:
    // 싱글톤 인스턴스에 접근하기 위한 메소드
    static FileManager* getInstance();
    // write명령어
    void writeLBA(int lba, string& value) {
        auto contents = readNandFile();
        contents[lba] = stoi(value.substr(2), nullptr, 16);
        writeNandFile(contents);
    }

    // read명령어
    void readLBA(int lba) {
        auto contents = readNandFile();
        stringstream ss;
        ss << "0x" << setfill('0') << setw(8) << hex << contents[lba];
        writeToResult(ss.str());
    }

    // read명령어
    void readLBA(int lba, string data) {
        // int 값을 16진수 문자열로 변환
        stringstream ss;
        ss << "0x" << setfill('0') << setw(8) << hex << data;
        writeToResult(ss.str());
    }

    // erase명령어
    void eraseLBA(int lba, int size) {
        auto contents = readNandFile();

        for (int i = lba; (i < lba + size) && (i < SIZE); ++i) {
            contents[i] = DEFAULT_DATA;
        }
        writeNandFile(contents);
    }
};
FileManager* FileManager::instance = nullptr; // 정적 멤버 변수 초기화

FileManager* FileManager::getInstance() {
    if (instance == nullptr) {
        instance = new FileManager();
    }
    return instance;
}