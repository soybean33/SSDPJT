#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>

using namespace std;

class Validator {
private:
    // Private ������
    Validator() {}

    // ������ ���� �����ڿ� �Ҵ� �����ڸ� ���� ���縦 ����
    Validator(const Validator&) = delete;
    Validator& operator=(const Validator&) = delete;

public:
    // Ŭ������ ������ �ν��Ͻ��� �����ϴ� �Լ�
    static Validator& getInstance() {
        static Validator instance;
        return instance;
    }

    // 0�̻� 99���� üũ
    bool checkNumberRange(const int& lbaAddress) {
        return lbaAddress >= 0 && lbaAddress <= 99;
    }

    // 1�̻� 10���� üũ
    bool checkSizeRange(const int& size) {
        return size >= 1 && size <= 10;
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
};
