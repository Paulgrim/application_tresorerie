#include "interface.h"

int main(int argc, char* argv[]){
    try{
        QApplication app(argc, argv);
        Widget_global f;
        f.show();
        return app.exec();
    }
    catch(TresorerieException e){cout<<e.getInfo();};
   return 0;
}

