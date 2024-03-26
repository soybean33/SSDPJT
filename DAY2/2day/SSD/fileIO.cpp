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

    // �̱��� �ν��Ͻ�
    static FileManager* instance;

    // Private ������
    FileManager() {}

    // ���ͷ� ��ȯ�� nand���� �б�
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

    // nand���Ͽ� ����
    void writeNandFile(const vector<int>& contents) {
        ofstream file(NAND, ios::binary);

        if (file.is_open()) {
            for (const auto& data : contents) {
                file.write(reinterpret_cast<const char*>(&data), sizeof(data));
            }
            file.close();
        }
    }

    // ������� result���Ͽ� ����
    void writeToResult(const string& value) {
        ofstream file(RESULT);

        if (file.is_open()) {
            file << value << endl;
            file.close();
        }
    }

public:
    // �̱��� �ν��Ͻ��� �����ϱ� ���� �޼ҵ�
    static FileManager* getInstance();
    // write��ɾ�
    void writeLBA(int lba, string& value) {
        auto contents = readNandFile();
        contents[lba] = stoi(value.substr(2), nullptr, 16);
        writeNandFile(contents);
    }

    // read��ɾ�
    void readLBA(int lba) {
        auto contents = readNandFile();
        stringstream ss;
        ss << "0x" << setfill('0') << setw(8) << hex << contents[lba];
        writeToResult(ss.str());
    }

    // read��ɾ�
    void readLBA(int lba, string data) {
        // int ���� 16���� ���ڿ��� ��ȯ
        stringstream ss;
        ss << "0x" << setfill('0') << setw(8) << hex << data;
        writeToResult(ss.str());
    }

    // erase��ɾ�
    void eraseLBA(int lba, int size) {
        auto contents = readNandFile();

        for (int i = lba; (i < lba + size) && (i < SIZE); ++i) {
            contents[i] = DEFAULT_DATA;
        }
        writeNandFile(contents);
    }
};
FileManager* FileManager::instance = nullptr; // ���� ��� ���� �ʱ�ȭ

FileManager* FileManager::getInstance() {
    if (instance == nullptr) {
        instance = new FileManager();
    }
    return instance;
}