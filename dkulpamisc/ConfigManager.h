#ifndef MISC_CONFIGMANAGER_H
#define MISC_CONFIGMANAGER_H


#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <rapidxml.hpp>

using namespace rapidxml;
using namespace std;


class ConfigManager {
public:
    explicit ConfigManager(string name);

    void Open();

    void SetValue(string key, string value);
    void SetValue(string key, int value);
    void SetValue(string key, float value);

    void GoUp(string to);
    void GoDown();
    void ResetPrefix();

    string GetString(string key, string def="");
    int GetInt(string key, int def=0);
    float GetFloat(string key, float def=0);
    double GetDouble(string key, double def=0);

    void Remove(string key);

    void Close();

    string ToString();
private:
    string removeWhitespaces(string line);
    char *getFileContext(char *path);

    string name;
    bool isOpen;

    pthread_mutex_t rwMutex;

    string keyPrefix;
    vector<string> keys;
    vector<string> values;
};


#endif //MISC_CONFIGMANAGER_H
