
#include <iostream>
#include <vector>
#include <random>
using namespace std;

// Function to encode the input data using Hamming encoding
vector<int> hammingEncode(const vector<int>& data) {
    int m = 0;
    while ((1 << m) < data.size() + m + 1) {
        m++;
    }

    vector<int> encoded(data.size() + m);
    int j = 0;
    for (int i = 0; i < encoded.size(); i++) {
        if (((i + 1) & i) == 0) {
            continue;
        }
        encoded[i] = data[j++];
    }

    for (int i = 0; i < m; i++) {
        int parity = 0;
        int bitmask = 1 << i;
        for (int j = 0; j < encoded.size(); j++) {
            if (((j + 1) & bitmask) != 0) {
                parity ^= encoded[j];
            }
        }
        encoded[bitmask - 1] = parity;
    }

    return encoded;
}

// Function to decode the encoded data and check for errors
vector<int> hammingDecode(const vector<int>& encoded) {
    int m = 0;
    while ((1 << m) < encoded.size()) {
        m++;
    }

    vector<int> decoded(encoded.size() - m);
    vector<int> errorBits(m, 0);
    for (int i = 0; i < m; i++) {
        int parity = 0;
        int bitmask = 1 << i;
        for (int j = 0; j < encoded.size(); j++) {
            if (((j + 1) & bitmask) != 0) {
                parity ^= encoded[j];
            }
        }
        errorBits[i] = parity;
    }

    int dataIndex = 0;
    for (int i = 0; i < encoded.size(); i++) {
        if (!((i + 1) & i)) {
            continue;
        }
        decoded[dataIndex++] = encoded[i];
    }

    return decoded;
}

void errSimulation(vector<int>& encoded)
{
    random_device rd;
    mt19937 gen(rd());

    // Define a distribution range for the random numbers
    uniform_int_distribution<int> dis(1, 100);

    // Generate and print a random number
    int randomNumber = dis(gen);

    int errChance = 10; // Err chance to appear

    for (int i = 0; i < encoded.size(); i++)
    {
        if ((randomNumber) <= errChance)
        {
            encoded[i] = (encoded[i] == 0) ? 1 : 0;  // Bit value change
            cout << "Error generated on position " << i + 1 << endl;
            cout << "Msg with error: ";
            for (int j = 0; j < encoded.size(); j++)
            {
                cout << encoded[j] << " ";
            }
            cout << endl;
            break;
        }
        randomNumber = dis(gen);
    }
}

// Function to check for errors in the encoded data
bool hammingCheck(vector<int>& encoded) {
    int m = 0;
    while ((1 << m) < encoded.size()) {
        m++;
    }

    vector<int> errorBits(m, 0);
    for (int i = 0; i < m; i++) {
        int parity = 0;
        int bitmask = 1 << i;
        for (int j = 0; j < encoded.size(); j++) {
            if (((j + 1) & bitmask) != 0) {
                parity ^= encoded[j];
            }
        }
        errorBits[i] = parity;
    }

    bool errorDetected = false;
    int errorPosition = 0;
    for (int i = 0; i < errorBits.size(); i++) {
        if (errorBits[i] != 0) {
            errorDetected = true;
            errorPosition += (1 << i);
        }
    }

    if (errorDetected) {
        cout << "Error detected at position: " << errorPosition << endl;
        encoded[errorPosition-1] = (encoded[errorPosition-1] == 1) ? 0 : 1;
        cout << "Correcting an error: ";
        for (int j = 0; j < encoded.size(); j++)
        {
            cout << encoded[j] << " ";
        }
        cout << endl;

        return false;
    }
    else {
        cout << "No errors detected." << endl;
        return true;
    }
}

int main() {
    vector<int> data = { 0, 1, 1, 0 };

    vector<int> encoded = hammingEncode(data);

    cout << "Encoded data: ";
    for (int bit : encoded) {
        cout << bit << " ";
    }
    cout << endl;

    errSimulation(encoded);

    bool errorDetected = hammingCheck(encoded);

    vector<int> decoded = hammingDecode(encoded);

    cout << "Decoded data: ";
    for (int bit : decoded) {
        cout << bit << " ";
    }
    cout << endl;

    return 0;
}