//#include <fstream> 
//#include <iomanip> 
//#include <string> 
//#include <vector>
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
//vector<string> readNandFile() { 
//	vector<string> datas(SIZE, DEFAULT_DATA);
//	ifstream file(NAND);
//
//	string line;
//	int index = 0;
//	if (file.is_open()) {
//		while (getline(file, line) && index < 100) {
//			if (line == "") {
//				index++;
//				continue;
//			}
//			datas[index++] = line;
//		}
//		file.close();
//	}
//	return datas;
//}
//
//// nand.txt ���Ͽ� write 
//void writeNandFile(const vector<string>& contents) {
//	ofstream file(NAND);
//
//	if (file.is_open()) {
//		for (const auto& line : contents) {
//			file << line << "\n";
//		}
//		file.close();
//	}
//}
//
////write��ɾ� 
//void writeLBA (int lba, const std::string& value) { 
//	auto contents = readNandFile(); 
//	contents[lba] = value.substr(2); 
//	writeNandFile(contents); 
//}
//
//// result.txt ���Ͽ� ������ ���� 
//void writeToResult(const string& value) {
//	ofstream file(RESULT);
//
//	if (file.is_open()) {
//		file << value << endl;
//		file.close();
//	}
//}
//
////read��ɾ� 
//void readLBA(int lba) { 
//	auto contents = readNandFile(); 
//	writeToResult("0x" + contents[lba]); 
//}