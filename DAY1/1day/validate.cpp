#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

/*
string -> int ��ȯ
*/
int convertString2Int(const string& lbaAddress) {
    int lbaValue;
    try {
        lbaValue = std::stoi(lbaAddress);
    }
    //���ڰ� �ƴ� ���
    catch (invalid_argument const& e) {
        return -1;
    }
    //int���� �ƴ� ���
    catch (out_of_range const& e) {
        return -1;
    }
    return lbaValue;
}

/*
0�̻� 99���� üũ
*/
bool checkNumberRange(const int& lbaAddress) {
    if (lbaAddress >= 0 && lbaAddress <= 99) {
        return true;
    }
    return false;
}

bool isValidHex(const string& data) {
    // �Է� ���ڿ��� ���̰� 10�� �ƴϸ� ��ȿ���� ����
    if (data.length() != 10)
        return false;

    // ù ��° ���ڰ� 0�̰� �� ��° ���ڰ� x���� Ȯ��
    if (data[0] != '0' || tolower(data[1]) != 'x')
        return false;

    // ������ ���ڵ��� ��ȿ�� 16���� �������� Ȯ��
    for (size_t i = 2; i < data.length(); i++) {
        if (!isxdigit(data[i]))
            return false;
    }

    return true;
}

/*
// 16���� ���ڿ��� uint32_t�� ��ȯ�ϴ� �Լ�
uint32_t hexStringToUint32(const string& hexStr) {
    uint32_t result;
    stringstream ss;

    // ���ڿ����� "0x" ���λ� ����
    string valueWithoutPrefix = hexStr.substr(2);

    // 16���� ���ڿ��� uint32_t ������ ��ȯ
    ss << hex << valueWithoutPrefix;
    ss >> result;

    return result;
}
*/