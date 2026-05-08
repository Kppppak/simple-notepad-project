#include "main_window.h"

main_window::main_window()
{
    setWindowTitle("Notepad");
    resize(800, 600);

    editor = new QTextEdit(this);

    setCentralWidget(editor);
}