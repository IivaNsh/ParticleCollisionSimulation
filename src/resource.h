#include <iostream>
#include <fstream>
#include <string>




class Resource{
public:

    Resource(std::string& name){
        std::ifstream file;
        file.open(name);
    }
    


    ~Resource(){ }

private:
    std::ifstream file;
    
};