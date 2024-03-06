#pragma once
#include <iostream>
#include <vector>
#include <string>

char out_of_bounds = '\0'; //Memory location for references to '\0' (like with CharacterAt). Technically not const, but in practice it should always return '\0'.

class String
{
public: //External Constructors
    String() //No input.
    {
        SetVStr("");
    }
    String(const char* _str) //Input is a cstr.
    {
        SetVStr(_str);
    }
    String(std::string _str) //Input is an std::string.
    {
        SetVStr(_str.c_str());
    }
    String(std::vector<char> _str) //Input is a char vector.
    {
        vstr = _str;
    }
    String(int _num) //Input is an int.
    {
        SetVStr(std::to_string(_num).c_str()); //Int to std::string to cstr to char vector.
    }
    String(String&& _other) noexcept //Move constructor. No exceptions because of the warning in Visual Studio.
    {
        vstr = _other.vstr;
        delete& _other; //Executes destructor ~String().
        _other = nullptr;
    }
    String(const String& _other) //Copy constructor.
    {
        vstr = _other.vstr;
    }
    ~String() //Destructor.
    {
        vstr.clear(); //Remove all elements.
        vstr.shrink_to_fit(); //Free memory.
    }
public: //External Functions
    const size_t Length() const //Returns an integer representing the count of characters up to the null termination character.
    {
        return vstr.size();
    }
    char& CharacterAt(size_t _index) //Returns a char representing the character at the location.
    {
        out_of_bounds = '\0'; //Reset out of bounds char so it always returns '\0'.
        if (_index < 0 || _index >= Length()) { return out_of_bounds; } //If index is less than 0 or greater than length, return '\0'.
        return vstr[_index]; //Return the character at in the char vector.
    }
    const char& CharacterAt(size_t _index) const //Const overload.
    {
        out_of_bounds = '\0'; //Reset out of bounds char so it always returns '\0'.
        if (_index < 0 || _index >= Length()) { return out_of_bounds; } //If index is less than 0 or greater than length, return '\0'.
        return vstr[_index]; //Return the character at in the char vector.
    }
    bool EqualTo(const String& _other) const //Returns true if _other contains the same characters.
    {
        return (strcmp(CStr(), _other.CStr()) == 0); //If the strcmp of both string's char arrays is 0 (and therefore the char arrays have the same contents).
    }

    String& Append(const String& _str) //Adds _str to the end of the string.
    {
        size_t i = 0; //Iterator
        vstr.pop_back(); //Remove null terminator.
        while (_str.CharacterAt(i) != '\0') //Add appended string character by character until null terminator.
        {
            vstr.push_back(_str.vstr[i]); //Add character i from _str.vstr to vstr.
            i++;
        }
        vstr.push_back('\0'); //Add null terminator.
        return *this; //Return this string.
    }
    String& Insert(const String& _str, int _offset = 0) //Inserts _str at _offset in the string.
    {
        int i = _str.Length();
        while (i >= 0) //In reverse order, add each character of _str.vstr to the beggining of the vstr.
        {
            if (_str.CharacterAt(i) != '\0') //If not a null character.
            {
                vstr.insert(vstr.begin() + _offset, _str.vstr[i]); //Add character i from _str.vstr to the start of vstr.
            }
            i--;
        }
        return *this; //Return this string.
    }
    String& Prepend(const String& _str) //Adds _str to the beginning of the string.
    {
        return Insert(_str, 0); //Insert at the beggining.
    }

    const char* CStr() const //Return the const char * that is useable with std::cout. eg: std::cout << str.cstr() << std::endl;
    {
        return vstr.data(); //Returns vstr as an array.
    }

    String& ToLower() //Convert all characters to lowercase.
    {
        const signed char offset = 'a' - 'A'; //Offset from capital to lowercase.
        size_t i = 0;

        while (CharacterAt(i) != '\0') //Loops until the null character.
        {
            if (vstr[i] >= 'A' && vstr[i] <= 'Z') //If capital letter.
            {
                vstr[i] += offset; //Convert capital to lowercase.
            }
            i++;
        }
        return *this; //Return this string.
    }
    String& ToUpper() //Convert all characters to uppercase.
    {
        const signed char offset = 'A' - 'a'; //Offset from lowercase to capital.
        size_t i = 0;

        while (CharacterAt(i) != '\0') //Loops until the null character.
        {
            if (vstr[i] >= 'a' && vstr[i] <= 'z') //If lowercase letter.
            {
                vstr[i] += offset; //Convert lowercase to capital.
            }
            i++;
        }
        return *this; //Return this string.
    }

    int Find(const String& _str) //Find from the entire string.
    {
        return Find(0, _str); //Effectively the same as starting at zero.
    }
    int Find(size_t _startIndex, const String& _str) //Returns the location of _str. Beginning the search from _startIndex.
    {
        size_t i = _startIndex;
        while (vstr[i] != 0) //Loops until the null character.
        {
            //std::cout << "i = " << i << "\n"; //Debug - displays i.
            if (RecursiveFind(i, 0, _str)) //If the substring was found starting at i.
            {
                return (int)i; //Return the index of the substring.
            }
            i++;
        }
        return -1; //If not found, return -1.
    }
    String& SingleReplace(const String& _find, const String& _replace) //Replaces only one instance of the substring. Better for performance if you already know there's only one.
    {
        int find_start = Find(_find); //Replaceable substring.
        if (find_start != -1) //If the replaceable substring exists.
        {
            int find_end = find_start + _find.Length() - 1; //Position of replaceable substring plus its length (excluding the null terminator).
            vstr.erase(vstr.begin() + find_start, vstr.begin() + find_end); //Removes the replaceable substring.
            Insert(_replace, find_start); //Inserts the replacing substring where the replaceable substring was.
        }
        return (*this);
    }
    String& Replace(const String& _find, const String& _replace) //Replaces all occurrences of _find with _replace.
    {
        while (Find(_find) != -1) //While an instance of the replaceable substring still exists.
        {
            SingleReplace(_find, _replace); //Replace one instance.
        }
        return (*this);
    }

    static String ReadFromConsole() //Wait for input in the console window and store the result
    {
        std::string newstr; //New std:string.
        std::getline(std::cin, newstr); //Get std:string's contents from console input, including spaces. Uses #include <string>.
        return String(newstr.c_str()); //Get the std:string's cstr and make a string out of it.
    }
    const void WriteToConsole() const //Write the string to the console window.
    {
        std::cout << CStr(); //Writes the char array to the console.
    }

public: //External Operators
    bool operator==(const String& _other) const //Returns true if lhs == rhs.
    {
        return EqualTo(_other); //Equal.
    }
    bool operator!=(const String& _other) const //Returns true if lhs != rhs.
    {
        return !(EqualTo(_other)); //Not equal.
    }
    String& operator=(const String& _str) //Replaces the characters in lhs with the characters in rhs.
    {
        vstr = _str.vstr; //Set to new string.
        return *this;
    }
    char& operator[](size_t _index) //Returns the character located at position n.
    {
        return CharacterAt(_index); //Returns non-const ver.
    }
    const char& operator[](size_t _index) const //Const overload.
    {
        return CharacterAt(_index); //Returns const ver.
    }
    bool operator<(const String& _str) //Returns true if this string comes before _str in the alphabet.
    {
        return (strcmp(CStr(), _str.CStr()) < 0); //strcmp is alphabetically before.
    }
    bool operator>(const String& _str) //Returns true if this string comes after _str in the alphabet.
    {
        return (strcmp(CStr(), _str.CStr()) > 0); //strcmp is alphabetically after.
    }
    bool operator<=(const String& _str) //Returns true if this string comes before or equal to _str in the alphabet.
    {
        return !((*this) > _str); //strcmp is not alphabetically after.
    }
    bool operator>=(const String & _str) //Returns true if this string comes before or equal to _str in the alphabet.
    {
        return !((*this) < _str); //strcmp is not alphabetically before.
    }
    String operator+(const String& _str) const //Return a new string that combines the lhs and rhs.
    {
        String* s = new String(*this); //Copy this string.
        s->Append(_str); //Append the second string to the copy.
        return *s; //Return the copy.
    }
    void operator+=(const String& _str) //Modifies lhs, appending rhs to lhs operands
    {
        Append(_str); //Literally just append the second string.
    }
private: //Internal Variables
    std::vector<char> vstr;
    void SetVStr(const char* arr) //Converts char array to char vector.
    {
        vstr.clear(); //Reset
        int i = 0;
        while (arr[i] != '\0') //For the entire string.
        {
            vstr.push_back(arr[i]); //Add element to vector.
            i++;
        }
        vstr.push_back('\0'); //Add null terminator.
    }
    bool RecursiveFind(size_t _mainIndex, size_t _subIndex, const String& _str) //At every character, it loops through subsequent characters to see if there is a matching string.
    {
        //std::cout << _mainIndex << ": Is " << CharacterAt(_mainIndex) << " equal to " << _str.CharacterAt(_subIndex) << "?\n"; //Debug - displays what the recursive function is searching for.
        if (CharacterAt(_mainIndex) == _str.CharacterAt(_subIndex)) //If the character at _mainIndex for the main string and _subIndex for the potential substring are the same.
        {
            //std::cout << "Yes\n"; //Debug - displays if the right character was found.
            if (_subIndex >= _str.Length() - 2) { return true; } //If t the end of the potential substring, return true.
            else if (_mainIndex >= Length() - 1) { return false; } //If at the end of the main string and not the potential substring, return false.
            else { return RecursiveFind(_mainIndex + 1, _subIndex + 1, _str); } //Otherwise, continue recursion and iterate both variables.
        }
        return false;
    }
};