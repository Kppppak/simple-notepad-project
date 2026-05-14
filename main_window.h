#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QString>

class main_window : public QMainWindow
{
private:
    QTextEdit* editor;
    QString current_file;

    void open_file();
    void save_file();
    void save_file_as();
    void update_title();

public:
    main_window();
};