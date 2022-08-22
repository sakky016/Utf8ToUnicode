#include <iostream>
#include <string>
#include <iomanip>
#include <bitset>
#include <sstream>

using namespace std;

//------------------------------------------------------------------------------------------------------------
void updateUnicodeBinary(const char text, string & unicodeBinary, int offset)
{
    bitset<8> binary(text);
    string binaryStr = binary.to_string();
    unicodeBinary.append(binaryStr, offset, string::npos);
}

//------------------------------------------------------------------------------------------------------------
// Find out the Unicode for the given UTF8 character. A UTF8 character can be composed of multiple characters.
//
// Reference: https://www.fileformat.info/info/unicode/utf8.htm
//------------------------------------------------------------------------------------------------------------
unsigned long getUnicode(const string & text)
{
    unsigned long  unicode;
    string unicodeBinary;

    if (text.empty())
    {
        return unicode;
    }

    bitset<8> tmp(text[0]);
    int byte1 = tmp.to_ulong();
    if (byte1 >= 0 && byte1 <= 191)
    {
        // First and only byte of a sequence
        updateUnicodeBinary(text[0], unicodeBinary, 1);
    }
    else if (byte1 >= 192 && byte1 <= 223)
    {
        updateUnicodeBinary(text[0], unicodeBinary, 3);
        updateUnicodeBinary(text[1], unicodeBinary, 2);
    }
    else if (byte1 >= 224 && byte1 <= 239)
    {
        updateUnicodeBinary(text[0], unicodeBinary, 4);
        updateUnicodeBinary(text[1], unicodeBinary, 2);
        updateUnicodeBinary(text[2], unicodeBinary, 2);
    }
    else if (byte1 >= 240 && byte1 <= 247)
    {
        updateUnicodeBinary(text[0], unicodeBinary, 5);
        updateUnicodeBinary(text[1], unicodeBinary, 2);
        updateUnicodeBinary(text[2], unicodeBinary, 2);
        updateUnicodeBinary(text[3], unicodeBinary, 2);
    }

    return bitset<32>(unicodeBinary).to_ulong();
}


//------------------------------------------------------------------------------------------------------------
string getBinary(const string & text)
{
    string binary;
    for (const auto & ch : text)
    {
        bitset<8> x(ch);
        binary.append(x.to_string());
        binary.append(" ");
    }

    return binary;
}


//------------------------------------------------------------------------------------------------------------
string getHex(const string & text)
{
    stringstream sstream;
    for (const auto & ch : text)
    {
        unsigned short iCh = static_cast<unsigned short>(ch);
        sstream << hex << iCh << " ";
    }

    return sstream.str();
}


//------------------------------------------------------------------------------------------------------------
// M A I N
//------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage:\n";
        cout << argv[0] << " <input text>\n";
        return 0;
    }

    string text(argv[1]);
    cout << "Input         : " << text << endl;
    cout << "Length        : " << text.size() << endl;
    
    cout << setbase(16);
    cout << "Unicode       : U+" << getUnicode(text) << endl;

    // Hex
    cout << "UTF8          : " << getHex(text) << endl;

    // Binary
    cout << setbase(10);
    cout << "UTF-8 Binary  : " << getBinary(text) << endl;


    return 0;
}
