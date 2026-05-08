#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTextEdit>

class main_window : public QMainWindow {
public:
    main_window();

private:
    QTextEdit* editor { nullptr };
};

#endif