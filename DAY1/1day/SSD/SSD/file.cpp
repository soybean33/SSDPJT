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
//// nand.txt 파일에서 모든 내용을 읽어서 벡터로 반환
//vector<uint32_t> readNandFile() {
//    vector<uint32_t> datas(SIZE,0);
//    ifstream file(NAND, ios::binary);
//
//    if (file.is_open()) {
//        uint32_t data;
//        cout << "데이터 입력 시작" << endl;
//        int index = 0;
//        while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
//            datas[index++] = data;
//            cout << data << endl;
//        }
//        cout << "데이터 입력 끝" << endl;
//        file.close();
//    }
//    return datas;
//}
//
//// nand.txt 파일에 write
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
//// write명령어: LBA 위치에 16진수 문자열로 표현된 값을 저장
//void writeLBA(int lba, const string& hexValue) {
//    uint32_t value = stoul(hexValue.substr(2), nullptr, 16); // 16진수 문자열을 uint32_t로 변환
//
//    cout << lba << " " << hexValue << " " << value << endl;
//
//    auto contents = readNandFile();
//    contents[lba] = value;
//    writeNandFile(contents);
//}
//
//// result.txt 파일에 데이터 쓰기
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
//    // 16진수 형태로 변환하여 ss에 저장
//    ss << hex << showbase << value;
//
//    return ss.str();
//}
//
//// read명령어: LBA 위치의 값을 읽어 result.txt에 쓰기
//void readLBA(int lba) {
//    auto contents = readNandFile();
//    cout << contents[lba] << endl;
//    writeToResult(convertToHex(contents[lba]));
//}