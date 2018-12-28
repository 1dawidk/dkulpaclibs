#include "JSON.h"

const int JSON::type_mismatch=-2;
const int JSON::wrong_key=-1;
const int JSON::key_found= 0;


JSON::JSON(const string content) {
    this->content= content;
}

int JSON::getString(const string key, string &val) {
    size_t key_pos= content.find_first_of(key);

    if(key_pos!=string::npos){
        size_t colon_pos= content.find_first_of(':', key_pos);
        
    } else {
        return JSON::wrong_key;
    }

    return JSON::key_found;
}

int JSON::getInt(const string key, int &val) {
    string tmpVal;

    if(getString(key, tmpVal)==JSON::key_found){
        try {
            val = stoi(tmpVal);
        } catch (std::invalid_argument &a){
            return JSON::type_mismatch;
        }
    }

    return JSON::key_found;
}

int JSON::getDouble(const string key, double &val) {
    string tmpVal;

    if(getString(key, tmpVal)==JSON::key_found){
        try {
            val = stod(tmpVal);
        } catch (std::invalid_argument &a){
            return JSON::type_mismatch;
        }
    }

    return JSON::key_found;
}

int JSON::getFloat(const string key, float &val) {
    string tmpVal;

    if(getString(key, tmpVal)==JSON::key_found){
        try {
            val = stof(tmpVal);
        } catch (std::invalid_argument &a){
            return JSON::type_mismatch;
        }
    }

    return JSON::key_found;
}

int JSON::getChar(const string key, char &val) {
    string tmpVal;

    if(getString(key, tmpVal)==JSON::key_found){
        val= tmpVal.c_str()[0];
    }

    return JSON::key_found;
}

int JSON::getBool(const string key, bool &val) {
    string tmpVal;

    if(getString(key, tmpVal)==JSON::key_found){
        if(tmpVal=="true")
            val= true;
        else if(tmpVal=="false")
            val= false;
        else
            return JSON::type_mismatch;
    }

    return JSON::key_found;
}
