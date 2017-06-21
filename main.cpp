#include <QApplication> 
#include <QThread> 
#include "bot.h" 
#include "controller.h" 
 
int main(int argc, char *argv[]) 
{ 
    QApplication a(argc, argv); 

    Controller controller(1000, 1000, 8, 150);
    controller.setBot(new Bot(&argc, argv)); 
    controller.run(); 

    return a.exec(); 
} 
