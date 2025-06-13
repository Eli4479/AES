#include <bits/stdc++.h>
using namespace std;

class Padding
{
public:
    void addPadding(std::vector<uint8_t> &input)
    {
        uint8_t pad_len = 16 - (input.size() % 16);
        if (pad_len == 0)
            pad_len = 16;
        input.insert(input.end(), pad_len, pad_len);
    }

    bool removePadding(std::vector<uint8_t> &input)
    {
        if (input.empty())
            return false;

        uint8_t pad_len = input.back();
        if (pad_len == 0 || pad_len > 16 || pad_len > input.size())
            return false;

        for (size_t i = input.size() - pad_len; i < input.size(); ++i)
        {
            if (input[i] != pad_len)
                return false;
        }

        input.resize(input.size() - pad_len);
        return true;
    }
};

class ChunkKey
{
public:
    vector<vector<vector<uint8_t>>> Keys;
    vector<uint8_t> Sbox = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
        0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
        0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
        0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
        0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
        0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
        0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
        0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
        0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
        0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
        0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
        0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
        0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
        0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
        0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
        0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
        0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
    ChunkKey(int roundUsed, vector<uint8_t> &userKey)
    {
        Keys = vector<vector<vector<uint8_t>>>(roundUsed + 1, vector<vector<uint8_t>>(4, vector<uint8_t>(4)));
        vector<uint8_t> KeyBytes(userKey.begin(), userKey.end());
        // Fill round 0 key
        for (int i = 0; i < 16; i++)
        {
            Keys[0][i % 4][i / 4] = KeyBytes[i];
        }
        vector<uint8_t> Rcon = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
        for (int round = 1; round <= roundUsed; round++)
        {
            // Get last column of previous key
            vector<uint8_t> temp(4);
            for (int i = 0; i < 4; i++)
            {
                temp[i] = Keys[round - 1][i][3];
            }

            // Rotate word
            uint8_t first = temp[0];
            for (int i = 0; i < 3; i++)
            {
                temp[i] = temp[i + 1];
            }
            temp[3] = first;

            // Apply S-box
            for (int i = 0; i < 4; i++)
            {
                temp[i] = Sbox[temp[i]];
            }

            // First column with Rcon
            Keys[round][0][0] = Keys[round - 1][0][0] ^ temp[0] ^ Rcon[round];
            for (int i = 1; i < 4; i++)
            {
                Keys[round][i][0] = Keys[round - 1][i][0] ^ temp[i];
            }

            // Remaining columns
            for (int j = 1; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    Keys[round][i][j] = Keys[round - 1][i][j] ^ Keys[round][i][j - 1];
                }
            }
        }
    }
};
class ChunkInput
{
public:
    vector<vector<vector<uint8_t>>> State;
    ChunkInput(vector<uint8_t> &input)
    {
        int Length = input.size();
        vector<vector<uint8_t>> Grid(4, vector<uint8_t>(4));
        for (int i = 0; i < Length; i += 16)
        {
            vector<uint8_t> Bytes = vector<uint8_t>(input.begin() + i, input.begin() + min(i + 16, Length));
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    Grid[k][j] = Bytes[k + j * 4];
                }
            }
            State.push_back(Grid);
        }
    }
};
class XorHandle
{
public:
    static void XorMatrix(vector<vector<vector<uint8_t>>> &State, vector<vector<vector<uint8_t>>> &Keys, int CurrentRound)
    {
        for (int i = 0; i < State.size(); i++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    State[i][j][k] ^= Keys[CurrentRound][j][k];
                }
            }
        }
    }
};
class Substitute
{
    vector<uint8_t> Sbox = {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
        0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
        0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
        0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
        0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
        0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
        0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
        0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
        0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
        0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
        0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
        0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
        0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
        0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
        0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
        0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
        0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
    vector<uint8_t> SboxInverse = {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38,
        0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87,
        0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d,
        0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2,
        0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16,
        0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda,
        0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a,
        0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02,
        0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea,
        0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85,
        0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89,
        0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20,
        0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31,
        0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d,
        0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0,
        0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26,
        0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

public:
    void sub(vector<vector<vector<uint8_t>>> &State, const bool encryption = true)
    {
        for (int CurrentChunk = 0; CurrentChunk < State.size(); CurrentChunk++)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    int number = State[CurrentChunk][i][j];
                    if (encryption)
                    {
                        State[CurrentChunk][i][j] = Sbox[number];
                    }
                    else
                    {
                        State[CurrentChunk][i][j] = SboxInverse[number];
                    }
                }
            }
        }
    }
};
class ShiftRows
{
public:
    static void shift(vector<vector<vector<uint8_t>>> &State, const bool encryption = true)
    {
        for (int CurrentChunk = 0; CurrentChunk < State.size(); CurrentChunk++)
        {
            vector<vector<uint8_t>> NewState(4, vector<uint8_t>(4));
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 4; col++)
                {
                    if (encryption)
                        NewState[row][col] = State[CurrentChunk][row][(col + row) % 4];
                    else
                        NewState[row][col] = State[CurrentChunk][row][(col - row + 4) % 4];
                }
            }
            State[CurrentChunk] = NewState;
        }
    }
};
class MixColumns
{
public:
    static const vector<vector<uint8_t>> Constant;
    static const vector<vector<uint8_t>> ConstantInverse;
    static vector<vector<uint8_t>> MultiplyMatrix(const vector<vector<uint8_t>> &matrix, const bool encryption = true)
    {
        vector<vector<uint8_t>> result(4, vector<uint8_t>(4));
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    if (encryption)
                        result[i][j] ^= GF8Multiply(Constant[i][k], matrix[k][j]);
                    else
                        result[i][j] ^= GF8Multiply(ConstantInverse[i][k], matrix[k][j]); // Inverse for decryption
                }
            }
        }
        return result;
    }
    static uint8_t GF8Multiply(uint8_t a, uint8_t b)
    {
        uint8_t result = 0;
        while (b)
        {
            if (b & 1)
                result ^= a;
            bool highBitSet = a & 0x80;
            a <<= 1;
            if (highBitSet)
                a ^= 0x1B; // AES irreducible polynomial
            b >>= 1;
        }
        return result;
    }
};
const vector<vector<uint8_t>> MixColumns::Constant = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};
const vector<vector<uint8_t>> MixColumns::ConstantInverse = {
    {0x0E, 0x0B, 0x0D, 0x09},
    {0x09, 0x0E, 0x0B, 0x0D},
    {0x0D, 0x09, 0x0E, 0x0B},
    {0x0B, 0x0D, 0x09, 0x0E}};
class ChunkOutput
{
public:
    vector<uint8_t> getOutput(const vector<vector<vector<uint8_t>>> &State, const bool encryption = true)
    {
        vector<uint8_t> output;
        for (const auto &chunk : State)
        {
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    output.push_back(chunk[j][i]);
                }
            }
        }
        return output;
    }
};
class AES
{
    vector<uint8_t> Input;
    vector<uint8_t> UserKey;
    Padding InputPadding;
    Padding KeyPadding;
    int RoundUsed = 10;
    ChunkKey *Keybox;
    ChunkInput *Inputbox;
    XorHandle XorHandler;
    Substitute Substitution;
    ShiftRows RowShifter;
    MixColumns MixColumnHandler;
    ChunkOutput Outputbox;

public:
    AES(string input, string userKey)
    {
        Input = vector<uint8_t>(input.begin(), input.end());
        UserKey = vector<uint8_t>(userKey.begin(), userKey.end());
        InputPadding.addPadding(Input);
        KeyPadding.addPadding(UserKey);
        Keybox = new ChunkKey(RoundUsed, UserKey);
        Inputbox = new ChunkInput(Input);
        Substitution = Substitute();
        RowShifter = ShiftRows();
        MixColumnHandler = MixColumns();
        XorHandler = XorHandle();
    }
    void calculate(const bool encryption = true)
    {
        if (encryption)
        {
            XorHandler.XorMatrix(Inputbox->State, Keybox->Keys, 0);
            for (int CurrentRound = 1; CurrentRound <= RoundUsed; CurrentRound++)
            {
                Substitution.sub(Inputbox->State, encryption);
                RowShifter.shift(Inputbox->State, encryption);
                if (CurrentRound < RoundUsed)
                {
                    for (int i = 0; i < Inputbox->State.size(); i++)
                    {
                        Inputbox->State[i] = MixColumnHandler.MultiplyMatrix(Inputbox->State[i], encryption);
                    }
                }
                    XorHandler.XorMatrix(Inputbox->State, Keybox->Keys, CurrentRound);
            }
        }
    }
    vector<uint8_t> output(const bool encryption = true)
    {
        vector<uint8_t> Output = Outputbox.getOutput(Inputbox->State, encryption);
            return Output;
    }
    ~AES()
    {
        delete Keybox;
        delete Inputbox;
    }
};

class DecryptAES
{
    vector<uint8_t> Input;
    vector<uint8_t> UserKey;
    Padding KeyPadding;
    int RoundUsed = 10;
    ChunkKey *Keybox;
    ChunkInput *Inputbox;
    XorHandle XorHandler;
    ChunkOutput Outputbox;
    Substitute Substitution;
    ShiftRows RowShifter;
    MixColumns MixColumnHandler;

public:
    DecryptAES(string input, string userKey)
    {
        for (size_t i = 0; i < input.length(); i += 2)
        {
            string byteString = input.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
            Input.push_back(byte);
        }

        UserKey = vector<uint8_t>(userKey.begin(), userKey.end());
        KeyPadding.addPadding(UserKey);
        Keybox = new ChunkKey(RoundUsed, UserKey);
        Inputbox = new ChunkInput(Input);
        Substitution = Substitute();
        RowShifter = ShiftRows();
        MixColumnHandler = MixColumns();
        XorHandler = XorHandle();
    }
    void calculate()
    {
        for (int CurrentRound = RoundUsed;  CurrentRound>= 1; CurrentRound--)
        {
            XorHandler.XorMatrix(Inputbox->State, Keybox->Keys, CurrentRound);
            if (CurrentRound < RoundUsed)
            {
                for (int i = 0; i < Inputbox->State.size(); i++)
                {
                    Inputbox->State[i] = MixColumnHandler.MultiplyMatrix(Inputbox->State[i], false);
                }
            }
            RowShifter.shift(Inputbox->State, false);
            Substitution.sub(Inputbox->State, false);
        }
        XorHandler.XorMatrix(Inputbox->State, Keybox->Keys, 0);
    }
    vector<uint8_t> getoutput()
    {
        vector<uint8_t> Output = Outputbox.getOutput(Inputbox->State);

        if (!KeyPadding.removePadding(Output))
        {
            cerr << "Error: Invalid padding detected." << endl;
            return {};
        }
        return Output;
    }
    ~DecryptAES()
    {
        delete Keybox;
        delete Inputbox;
    }
};
int main(int argc, char *argv[])
{
     if (argc != 4) {
        cerr << "Usage: <program> <encrypt|decrypt> <input> <key>" << endl;
        return 1;
    }
    string choice = argv[1];
    string input = argv[2];
    string userKey = argv[3];

    if (choice == "encrypt")
    {
        AES aes(input, userKey);
        aes.calculate(true);
        vector<uint8_t> encryptedOutput = aes.output(true);
        for (uint8_t byte : encryptedOutput)
        {
            cout << hex << setw(2) << setfill('0') << (int)byte;
        }
        cout << endl;
    }
    else
    {
        DecryptAES decryptAES(input, userKey);
        decryptAES.calculate();
        vector<uint8_t> decryptedOutput = decryptAES.getoutput();
        for(uint8_t byte : decryptedOutput)
        {
            cout << (char)byte;
        }
        cout << endl;
    }
    return 0;
}
