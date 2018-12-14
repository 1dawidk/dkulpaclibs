#include <iostream>
#include "ConfigManager.h"


ConfigManager::ConfigManager(string name) {
    this->name= name;
    isOpen= false;
    rwMutex= PTHREAD_MUTEX_INITIALIZER;
}

void ConfigManager::Open() {
    /*char rpath[256];
    char *fContext= nullptr;
    realpath(("/etc/mort/" + name + ".cfg").c_str(), rpath);
    xml_document <>doc;

    try{
        fContext=  getFileContext(rpath);
        doc.parse<0>(fContext);
    } catch (const parse_error &e){
        delete[] fContext;
        return;
    } catch (const exception& e){
        delete[] fContext;
        return;
    }

    delete[] fContext;*/

    pthread_mutex_lock(&rwMutex);
    if(!isOpen) {
        char abs_path[128];
        realpath(("/etc/mort/" + name + ".cfg").c_str(), abs_path);
        ifstream f(abs_path);
        string line;
        size_t eq_pos;
        string prefix = "";

        while (getline(f, line)) {
            //Check if not empty line
            if (!line.empty()) {
                //Remove whitespaces at the beginning
                line = removeWhitespaces(line);
                //Check if not comment line
                if (line[0] != '#') {
                    //Check if prefix change lina
                    if (line[0] == '[') {
                        //Find closing char
                        unsigned long prefEnd = line.find(']');
                        if (prefEnd != string::npos) {
                            //Check if add prefix or new prefix
                            if (line[1] == '~')
                                prefix += line.substr(2, prefEnd - 2);
                            else if(line[1]=='.' && line[2]=='.'){
                                unsigned long lastS=prefix.rfind('/',prefix.length()-2);

                                if(lastS!=string::npos){
                                    prefix=prefix.substr(0, lastS+1);
                                } else {
                                    prefix="/";
                                }
                            } else
                                prefix = line.substr(1, prefEnd - 1);

                            //Check if prefix ends with '/'
                            if (prefix[prefix.length() - 1] != '/')
                                prefix += "/";
                        }
                    } else {
                        //Find value beginning
                        eq_pos = line.find('=');
                        //If found
                        if (eq_pos != string::npos) {
                            //Add value key
                            keys.push_back(prefix + line.substr(0, eq_pos));
                            //Remove whitespaces at the beginning and add value (as string)
                            values.push_back(removeWhitespaces(line.substr(eq_pos + 1)));
                        }
                    }
                }
            }
        }

        //Set keyPrefix as absolute
        keyPrefix = "/";

        f.close();
        isOpen= true;
    }
    pthread_mutex_unlock(&rwMutex);
}

char *ConfigManager::getFileContext(char *path) {
    char* fileContext= nullptr;
    ifstream f(path);

    if(f){
        f.seekg(0, f.end);
        long size= f.tellg();
        f.seekg(0);

        fileContext= new char[size+10];
        f.read(fileContext, size);
    }


    return fileContext;
}

string ConfigManager::removeWhitespaces(string line) {
    while(!line.empty() && (line[0]==' ' || line[0]=='\t')){
        line=line.substr(1);
    }

    return line;
}



void ConfigManager::Close() {
    pthread_mutex_lock(&rwMutex);
    // ######### NEW ########################
    if(isOpen){
        char abs_path[128];
        realpath(("/etc/mort/"+name+".cfg").c_str(),abs_path);
        fstream f(abs_path, ios_base::in | ios_base::out);
    
    }

    // ######### OLD ########################
    /*if(isOpen) {
        char abs_path[128];
        realpath(("/etc/mort/tmp_" + name + "_cfg").c_str(), abs_path);
        fstream f(abs_path, ios_base::in | ios_base::out);

        while (!keys.empty()) {
            f << keys.back() << "=" << values.back() << endl;
            keys.pop_back();
            values.pop_back();
        }

        system(("mv /etc/mort/tmp_" + name + "_cfg /etc/mort/" + name + ".cfg 2>/dev/null").c_str());
    }*/
    pthread_mutex_unlock(&rwMutex);
}

void ConfigManager::SetValue(string key, string value) {
    pthread_mutex_lock(&rwMutex);

    int cnt=0;

    while(cnt<keys.size() && (keyPrefix+key)!=keys[cnt])
        cnt++;

    if(cnt>=keys.size()){
        keys.push_back(key);
        values.push_back(value);
    } else {
        values[cnt] = value;
    }

    pthread_mutex_unlock(&rwMutex);
}

void ConfigManager::SetValue(string key, int value) {
    SetValue(std::move(key), to_string(value));
}

void ConfigManager::SetValue(string key, float value) {
    SetValue(std::move(key), to_string(value));
}

string ConfigManager::GetString(string key, string def) {
    pthread_mutex_lock(&rwMutex);

    int cnt=0;

    while(cnt<keys.size() && ((keyPrefix+key)!=keys[cnt]))
        cnt++;

    pthread_mutex_unlock(&rwMutex);

    if(cnt==keys.size())
        return def;
    else
        return values[cnt];
}

int ConfigManager::GetInt(string key, int def) {
    string val= GetString(std::move(key));

    if(val.empty())
        return def;
    else
        return stoi(val);
}

float ConfigManager::GetFloat(string key, float def) {
    string val= GetString(std::move(key));

    if(val.empty())
        return def;
    else
        return stof(val);
}

double ConfigManager::GetDouble(string key, double def) {
    string val= GetString(std::move(key));

    if(val.empty())
        return def;
    else
        return stod(val);
}

void ConfigManager::Remove(string key) {
    pthread_mutex_lock(&rwMutex);

    int cnt=0;

    while(cnt<keys.size() && ((keyPrefix+key)!=keys[cnt]))
        cnt++;

    if(cnt!=keys.size()) {
        values.erase(values.begin()+cnt);
        keys.erase(keys.begin()+cnt);
    }

    pthread_mutex_unlock(&rwMutex);
}

void ConfigManager::GoUp(string to) {
    pthread_mutex_lock(&rwMutex);
    keyPrefix+= to+"/";
    pthread_mutex_unlock(&rwMutex);
}

void ConfigManager::GoDown() {
    pthread_mutex_lock(&rwMutex);
    unsigned long lastS=keyPrefix.rfind('/',keyPrefix.length()-2);

    if(lastS!=string::npos){
        keyPrefix=keyPrefix.substr(0, lastS+1);
    }
    pthread_mutex_unlock(&rwMutex);
}

void ConfigManager::ResetPrefix() {
    pthread_mutex_lock(&rwMutex);
    keyPrefix="/";
    pthread_mutex_unlock(&rwMutex);
}

string ConfigManager::ToString() {
    string r=name+"\n";

    for(int i=0;i<keys.size();i++){
        r+= keys[i]+": "+values[i]+"\n";
    }
}
