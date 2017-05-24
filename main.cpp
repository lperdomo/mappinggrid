#include <QApplication> 
#include <QThread> 
#include "bot.h" 
#include "controller.h" 
 
int main(int argc, char *argv[]) 
{ 
    QApplication a(argc, argv); 
 
    Controller controller(500, 500, 5, 200);
    controller.setBot(new Bot(&argc, argv)); 
    controller.run(); 
 
    return a.exec(); 
} 
