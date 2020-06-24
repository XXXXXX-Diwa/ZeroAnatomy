#include "ZeroAnatomy.h"
#include "HeaderSeriesOut.h"


int main(int argc,char* const argv[]){
    ios::sync_with_stdio(false);
    unique_ptr<File>readFile;
    if(argc==1){
        File::AuthorInfo();
    }else{
        readFile=make_unique<File>(argv[1]);
    }
    
    readFile.reset();
    return 0;
}