#pragma once

#include <QMainWindow>
#include <QTextEdit>
#include <QString>
#include <QDialog>
#include <QLineEdit>

#include "spell_checker.h"
#include "spell_checker_highlighter.h"

class main_window : public QMainWindow {
private:
    QTextEdit *editor;
    QString current_file;

    spell_checker checker;
    spell_checker_highlighter *highlighter;

    void open_file();

    void save_file();

    void save_file_as();

    void update_title();

    void show_find_dialog();

    void find_next();

    void replace_text();

    void replace_all();

    QDialog *find_dialog;
    QLineEdit *find_input;
    QLineEdit *replace_input;

public:
    explicit main_window(QWidget *parent = nullptr);
};
