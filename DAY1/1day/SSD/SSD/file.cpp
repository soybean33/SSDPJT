//#include <fstream>
//#include <iomanip> 
//#include <string>
//#include <vector>
//#include <sstream>
//#include <iostream>
//
//using namespace std;
//
//const string NAND = "nand.txt";
//const string RESULT = "result.txt";
//const int SIZE = 100;
//const string DEFAULT_DATA = "00000000";
//
//// nand.txt ���Ͽ��� ��� ������ �о ���ͷ� ��ȯ
//vector<uint32_t> readNandFile() {
//    vector<uint32_t> datas(SIZE,0);
//    ifstream file(NAND, ios::binary);
//
//    if (file.is_open()) {
//        uint32_t data;
//        cout << "������ �Է� ����" << endl;
//        int index = 0;
//        while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
//            datas[index++] = data;
//            cout << data << endl;
//        }
//        cout << "������ �Է� ��" << endl;
//        file.close();
//    }
//    return datas;
//}
//
//// nand.txt ���Ͽ� write
//void writeNandFile(const vector<uint32_t>& contents) {
//    ofstream file(NAND, ios::binary);
//
//    if (file.is_open()) {
//        for (const auto& data : contents) {
//            file.write(reinterpret_cast<const char*>(&data), sizeof(data));
//        }
//        file.close();
//    }
//}
//
//// write��ɾ�: LBA ��ġ�� 16���� ���ڿ��� ǥ���� ���� ����
//void writeLBA(int lba, const string& hexValue) {
//    uint32_t value = stoul(hexValue.substr(2), nullptr, 16); // 16���� ���ڿ��� uint32_t�� ��ȯ
//
//    cout << lba << " " << hexValue << " " << value << endl;
//
//    auto contents = readNandFile();
//    contents[lba] = value;
//    writeNandFile(contents);
//}
//
//// result.txt ���Ͽ� ������ ����
//void writeToResult(const string& value) {
//    cout << value;
//    ofstream file(RESULT);
//
//    if (file.is_open()) {
//        file << value << endl;
//        file.close();
//    }
//}
//
//string convertToHex(uint32_t value) {
//    stringstream ss;
//
//    // 16���� ���·� ��ȯ�Ͽ� ss�� ����
//    ss << hex << showbase << value;
//
//    return ss.str();
//}
//
//// read��ɾ�: LBA ��ġ�� ���� �о� result.txt�� ����
//void readLBA(int lba) {
//    auto contents = readNandFile();
//    cout << contents[lba] << endl;
//    writeToResult(convertToHex(contents[lba]));
//}