#include <QMainWindow>
#include <QTextEdit>

class main_window : public QMainWindow
{
private:
    QTextEdit *editor;

public:
    main_window();
};