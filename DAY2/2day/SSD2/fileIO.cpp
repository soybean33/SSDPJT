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

vector<int> readNandFile(); //nand����(���̳ʸ�����)�� �о ���ͷ� ��ȯ
void writeNandFile(const vector<int>& contents); //nand���Ͽ� ����
void writeToResult(const string& value); //result���Ͽ� ����� ����

void writeLBA(int lba, string& value); //����
void readLBA(int lba); //�б�
void eraseLBA(int lba, int size); //�����


// write��ɾ�
void writeLBA(int lba, string& value) {
    auto contents = readNandFile();
    contents[lba] = stoi(value.substr(2), nullptr, 16);
    writeNandFile(contents);
}

// ���̳ʸ� ���Ͽ��� ��� ������ �о ���ͷ� ��ȯ
vector<int> readNandFile() {
    vector<int> datas(SIZE, DEFAULT_DATA);
    ifstream file(NAND, ios::binary);

    if (file.is_open()) {
        for (int i = 0; i < SIZE; ++i) {
            file.read(reinterpret_cast<char*>(&datas[i]), sizeof(datas[i]));
            if (file.fail()) {
                break; // ���� ���� �����ϰų� �б� ������ �߻��ߴٸ� �ߴ�
            }
        }
        file.close();
    }
    return datas;
}

// nand.txt ���Ͽ� ���̳ʸ� ���·� ������ ����
void writeNandFile(const vector<int>& contents) {
    ofstream file(NAND, ios::binary);

    if (file.is_open()) {
        for (const auto& data : contents) {
            file.write(reinterpret_cast<const char*>(&data), sizeof(data));
        }
        file.close();
    }
}

// read��ɾ�
void readLBA(int lba) {
    auto contents = readNandFile();
    // int ���� 16���� ���ڿ��� ��ȯ
    stringstream ss;
    ss << "0x" << setfill('0') << setw(8) << hex << contents[lba];
    writeToResult(ss.str());
}

// read��ɾ�
void readLBA(int lba, string data) {
    writeToResult(data);
}

// result.txt ���Ͽ� ������ ����
void writeToResult(const string& value) {
    ofstream file(RESULT);

    if (file.is_open()) {
        file << value << endl;
        file.close();
    }
}

//erase��ɾ�
void eraseLBA(int lba, int size) {
    auto contents = readNandFile();

    for (int i = lba; (i < lba + size) && (i < SIZE); ++i) {
        contents[i] = DEFAULT_DATA; // ���� ���� �����͸� DEFAULT_DATA�� ����
    }
    writeNandFile(contents);
}
