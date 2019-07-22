#include "PostPairs.h"
#include "debug/Log.h"

const int PostPairs::type_mismatch=-2;
const int PostPairs::wrong_key=-1;
const int PostPairs::key_found= 0;


PostPairs::PostPairs(const string content) {
    this->content= content;
}


// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18
// D a w i d = t o   j a     i     t  y  l  e  &
//
// val_pos = 6
// end_pos = 18
// n = end_pos - val_pos = 12


int PostPairs::getString(const string key, string &val) {
    size_t key_pos= content.find(key);

    if(key_pos!=string::npos){
        size_t val_pos= content.find_first_of('=', key_pos)+1;
        size_t end_pos= content.find_first_of('&', val_pos);

        val= content.substr(val_pos, end_pos-val_pos);
    } else {
        return PostPairs::wrong_key;
    }

    return PostPairs::key_found;
}

int PostPairs::getInt(const string key, int &val) {
    string tmpVal;

    if(getString(key, tmpVal)==PostPairs::key_found){
        try {
            val = stoi(tmpVal);
        } catch (std::invalid_argument &a){
            return PostPairs::type_mismatch;
        }
    }

    return PostPairs::key_found;
}

int PostPairs::getDouble(const string key, double &val) {
    string tmpVal;

    if(getString(key, tmpVal)==PostPairs::key_found){
        try {
            val = stod(tmpVal);
        } catch (std::invalid_argument &a){
            return PostPairs::type_mismatch;
        }
    }

    return PostPairs::key_found;
}

int PostPairs::getFloat(const string key, float &val) {
    string tmpVal;

    if(getString(key, tmpVal)==PostPairs::key_found){
        try {
            val = stof(tmpVal);
        } catch (std::invalid_argument &a){
            return PostPairs::type_mismatch;
        }
    }

    return PostPairs::key_found;
}

int PostPairs::getChar(const string key, char &val) {
    string tmpVal;

    if(getString(key, tmpVal)==PostPairs::key_found){
        val= tmpVal.c_str()[0];
    }

    return PostPairs::key_found;
}

int PostPairs::getBool(const string key, bool &val) {
    string tmpVal;

    if(getString(key, tmpVal)==PostPairs::key_found){
        if(tmpVal=="true")
            val= true;
        else if(tmpVal=="false")
            val= false;
        else
            return PostPairs::type_mismatch;
    }

    return PostPairs::key_found;
}
