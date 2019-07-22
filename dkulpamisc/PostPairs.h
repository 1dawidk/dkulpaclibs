#ifndef DKULPAMISC_POSTPAIRS_H
#define DKULPAMISC_POSTPAIRS_H

#include <string>
#include <stdexcept>

using namespace std;

class PostPairs {
public:
    explicit PostPairs(string content);

    int getInt(string key, int &val);
    int getDouble(string key, double &val);
    int getFloat(string key, float &val);
    int getChar(string key, char &val);
    int getString(string key, string &val);
    int getBool(string key, bool &val);

    const static int wrong_key;
    const static int key_found;
    const static int type_mismatch;
private:
    string content;


};


#endif //DKULPAMISC_JSON_H
