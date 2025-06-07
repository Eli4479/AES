#include <bits/stdc++.h>
#include <iostream>

using namespace std;
class AES
{
  string Input;
  string CurrentOutput;
  int RoundUsed = 10;
  string Method;
  vector<uint8_t> InputBytes;
  vector<vector<vector<uint8_t>>> State;
  vector<vector<vector<uint8_t>>> Keys = vector<vector<vector<uint8_t>>>(11, vector<vector<uint8_t>>(4, vector<uint8_t>(4)));
vector<uint8_t> Sbox;
vector<vector<uint8_t>> Constant={
  {0x02, 0x03, 0x01, 0x01},
  {0x01, 0x02, 0x03, 0x01},
  {0x01, 0x01, 0x02, 0x03},
  {0x03, 0x01, 0x01, 0x02}
};
int Padding=0;
public:
  AES(string input,int padding, string method)
  {
    Input = input;
    Padding = padding;
    Method = method;
  }
  void calculate()
  {
    if (Method == "encryption")
    {
      cout << "Encrypting for " << RoundUsed << " rounds." << endl;
      Chunking();
      for(int CurrentChunk=0;CurrentChunk<State.size();CurrentChunk++)
        {
          Xor_matrix(0, CurrentChunk);
        }
      for(int Round=1;Round<RoundUsed;Round++)
      {
        Substitute();
        ShiftRows();
        MixColumns();
        for(int CurrentChunk=0;CurrentChunk<State.size();CurrentChunk++)
        {
          Xor_matrix(Round, CurrentChunk);
        }
        cout << "Round " << Round + 1 << " completed." << endl;
      }
      Substitute();
      ShiftRows();
      for(int CurrentChunk=0;CurrentChunk<State.size();CurrentChunk++)
        {
          Xor_matrix(RoundUsed, CurrentChunk);
        }
    }
    else if (Method == "decryption")
    {
      cout << "Decrypting for " << RoundUsed << " rounds." << endl;
    }
    else
    {
      cout << "Invalid method specified. Use 'encryption' or 'decryption'." << endl;
      return;
    }
  }
  void Chunking()
  {
    int Length = Input.length();
    vector<vector<uint8_t>> Grid = vector<vector<uint8_t>>(4, vector<uint8_t>(4));
    for (int i = 0; i < Length; i += 16)
    {
      string Chunk = Input.substr(i, 16);
      vector<uint8_t> Bytes(Chunk.begin(), Chunk.end());
      for (int j = 0; j < 4; j++)
      {
        for (int i = 0; i < 4; i++)
        {
          Grid[i][j] = Bytes[i + j * 4];
        }
      }
      State.push_back(Grid);
    }
  }
  void Xor_matrix(int Round,int CurrentChunk)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        State[CurrentChunk][i][j] ^= Keys[Round][i][j];
      }
    }
  }
  void Substitute()
  {
    for(int CurrentChunk = 0; CurrentChunk < State.size(); CurrentChunk++)
    {
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          State[CurrentChunk][i][j] = Sbox[State[CurrentChunk][i][j]];
        }
      }
    }
  }
  void ShiftRows()
  {
    for(int CurrentChunk = 0; CurrentChunk < State.size(); CurrentChunk++)
    {
      vector<vector<uint8_t>> NewState(4, vector<uint8_t>(4));
      for (int row= 0; row < 4; row++)
      {
        for (int col = 0; col < 4; col++)
        {
          NewState[row][col] = State[CurrentChunk][row][(col + row) % 4];
        }
      }
      State[CurrentChunk] = NewState;
    }
  }
  void MixColumns()
  {
    for(int CurrentChunk = 0; CurrentChunk < State.size(); CurrentChunk++)
    {
      State[CurrentChunk] = MultiplyMatrix(State[CurrentChunk]);
    }
  }
  vector<vector<uint8_t>>MultiplyMatrix(vector<vector<uint8_t>> &matrix)
  {
    vector<vector<uint8_t>> result(4, vector<uint8_t>(4));
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        result[i][j] = 0;
        for(int k=0;k<4;k++)
      {
        result[i][j]^=GF8Multiply(Constant[i][k],matrix[k][j]);
      }
    }
  }
    return result;
  }
  uint8_t GF8Multiply(uint8_t a, uint8_t b) {
  uint8_t result = 0;
  while (b) {
    if (b & 1)
      result ^= a;
    bool highBitSet = a & 0x80;
    a <<= 1;
    if (highBitSet)
      a ^= 0x1B;
    b >>= 1;
  }
  return result;
}
string GetOutput()
{
  CurrentOutput = "";
  for (const auto &chunk : State)
  {
    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 4; j++)
      {
        CurrentOutput += chunk[i][j];
      }
    }
  }
  if (Padding > 0)
  {
    CurrentOutput = CurrentOutput.substr(0, CurrentOutput.length() - Padding);
  }
  return CurrentOutput;
  }
};
pair<string,int> GetPaddedInput(string Input)
{
  int Length = Input.length();
  if (Length % 16 == 0)
    return {Input, 0};
  int Padding = 16 - (Length % 16);
  for (int i = 0; i < Padding; i++)
    Input += (char)Padding;
  return {Input, Padding};
}
int main()
{
  string Input;
  int Key;
  string Method;
  cout << "Enter Input: ";
  cin >> Input;
  cout << "Enter Method you want (encryption/decryption): ";
  cin >> Method;
  pair<string, int> PaddedInput = GetPaddedInput(Input);
  AES aes(PaddedInput.first, PaddedInput.second, Method);
  aes.calculate();
  cout << "Processing completed." << endl;
  cout << "Output: " << endl;
  cout << aes.GetOutput() << endl;
  return 0;
}
