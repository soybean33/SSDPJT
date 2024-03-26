//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <stdexcept>
//#include <vector>
//
//using namespace std;
//
////validate
//int convertstring2int(const string& lbaaddress); //string -> int 변환
//bool checknumberrange(const int& lbaaddress); //lba 주소 범위 0이상 99이하
//bool checksizerange(const int& size); //erase 지울 수 있는 범위 1이상 10이하
//bool isvalidhex(const string& data); //16진법 맞는지 체크
//
////fileio
//void writelba(int lba, string& value); //쓰기
//void readlba(int lba); //읽기
//void eraselba(int lba, int size); //지우기
//
//
//int main(int argc, char* argv[]) {
//	string command = argv[1]; //명령어
//	//write
//	if (command == "w") {
//		int lbaaddress = convertstring2int(argv[2]); //lba 영역 주소
//		string data = argv[3];
//
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba 주소 잘못된 입력";
//			return 0;
//		}
//		if (!isvalidhex(data)) {
//			cout << "error:: 저장 데이터 잘못된 입력";
//			return 0;
//		}
//		writelba(lbaaddress, data);
//	}
//	//read
//	else if (command == "r") {
//		int lbaaddress = convertstring2int(argv[2]); //lba 영역 주소
//
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba 주소 잘못된 입력";
//			return 0;
//		}
//		readlba(lbaaddress);
//		
//	}
//	//erase
//	else if (command == "e") {
//		int lbaaddress = convertstring2int(argv[2]); //lba 영역 주소
//		int size = convertstring2int(argv[3]);//삭제할 영역 크기
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba 주소 잘못된 입력";
//			return 0;
//		}
//		if (!checksizerange(size)) {
//			cout << "error:: 잘못된 사이즈 입력";
//			return 0;
//		}
//
//		eraselba(lbaaddress, size);
//	}
//	//잘못된 명령어
//	else {
//		cout << "error:: command 잘못된 입력";
//		return 0;
//	}
//}