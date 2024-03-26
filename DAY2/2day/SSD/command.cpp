//#include <iostream>
//#include <string>
//#include <cstdlib>
//#include <stdexcept>
//#include <vector>
//
//using namespace std;
//
////validate
//int convertstring2int(const string& lbaaddress); //string -> int ��ȯ
//bool checknumberrange(const int& lbaaddress); //lba �ּ� ���� 0�̻� 99����
//bool checksizerange(const int& size); //erase ���� �� �ִ� ���� 1�̻� 10����
//bool isvalidhex(const string& data); //16���� �´��� üũ
//
////fileio
//void writelba(int lba, string& value); //����
//void readlba(int lba); //�б�
//void eraselba(int lba, int size); //�����
//
//
//int main(int argc, char* argv[]) {
//	string command = argv[1]; //��ɾ�
//	//write
//	if (command == "w") {
//		int lbaaddress = convertstring2int(argv[2]); //lba ���� �ּ�
//		string data = argv[3];
//
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba �ּ� �߸��� �Է�";
//			return 0;
//		}
//		if (!isvalidhex(data)) {
//			cout << "error:: ���� ������ �߸��� �Է�";
//			return 0;
//		}
//		writelba(lbaaddress, data);
//	}
//	//read
//	else if (command == "r") {
//		int lbaaddress = convertstring2int(argv[2]); //lba ���� �ּ�
//
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba �ּ� �߸��� �Է�";
//			return 0;
//		}
//		readlba(lbaaddress);
//		
//	}
//	//erase
//	else if (command == "e") {
//		int lbaaddress = convertstring2int(argv[2]); //lba ���� �ּ�
//		int size = convertstring2int(argv[3]);//������ ���� ũ��
//		//validate
//		if (lbaaddress == -1 || !checknumberrange(lbaaddress)) {
//			cout << "error:: lba �ּ� �߸��� �Է�";
//			return 0;
//		}
//		if (!checksizerange(size)) {
//			cout << "error:: �߸��� ������ �Է�";
//			return 0;
//		}
//
//		eraselba(lbaaddress, size);
//	}
//	//�߸��� ��ɾ�
//	else {
//		cout << "error:: command �߸��� �Է�";
//		return 0;
//	}
//}