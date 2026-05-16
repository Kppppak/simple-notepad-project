#include "main_window.h"
#include "text_transform.h"
#include "spell_checker.h"
#include "spell_checker_highlighter.h"
#include "word_frequency_dialog.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QToolBar>
#include <QTextCharFormat>
#include <QFont>
#include <QMenu>
#include <QTextCursor>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDialog>
#include <QStatusBar>
#include <QFontDialog>

#include <map>
#include <sstream>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Notepad");

    resize(800, 600);

    editor = new QTextEdit(this);

    setCentralWidget(editor);

    statusBar()->showMessage(
    "Line: 1   Column: 1"
);

    editor->setContextMenuPolicy(
        Qt::CustomContextMenu
    );

    checker.load_words(
        "data/words.txt"
    );

    highlighter =
            new spell_checker_highlighter(
                editor->document(),
                checker
            );

    connect(
        editor,
        &QTextEdit::customContextMenuRequested,
        this,
        [this](const QPoint &pos) {
            QTextCursor cursor =
                    editor->cursorForPosition(pos);

            cursor.select(
                QTextCursor::WordUnderCursor
            );

            QString word =
                    cursor.selectedText();

            QMenu *menu =
                    editor->createStandardContextMenu();

            if (
                !checker.is_correct(word)
            ) {
                auto list =
                        checker.suggestions(word);

                menu->addSeparator();

                for (
                    const auto &suggestion
                    : list
                ) {
                    QAction *action =
                            menu->addAction(
                                suggestion
                            );

                    connect(
                        action,
                        &QAction::triggered,
                        this,
                        [cursor, suggestion]() mutable {
                            cursor.insertText(
                                suggestion
                            );
                        }
                    );
                }
            }

            menu->exec(
                editor->mapToGlobal(pos)
            );

            delete menu;
        }
    );

    QMenu *file_menu =
            menuBar()->addMenu("File");

    QMenu *edit_menu =
            menuBar()->addMenu("Edit");

    QMenu *transform_menu =
            menuBar()->addMenu("Transform");

    QMenu *view_menu =
            menuBar()->addMenu("View");

    QMenu *format_menu =
    menuBar()->addMenu("Format");

    QMenu *tools_menu =
            menuBar()->addMenu("Tools");

    QMenu *search_menu =
            menuBar()->addMenu("Search");

    QAction *open_action =
            file_menu->addAction("Open");

    QAction *save_action =
            file_menu->addAction("Save");

    QAction *save_as_action =
            file_menu->addAction("Save As");

    QAction *undo_action =
            edit_menu->addAction("Undo");

    QAction *redo_action =
            edit_menu->addAction("Redo");

    edit_menu->addSeparator();

    QAction *cut_action =
            edit_menu->addAction("Cut");

    QAction *copy_action =
            edit_menu->addAction("Copy");

    QAction *paste_action =
            edit_menu->addAction("Paste");

    edit_menu->addSeparator();

    QAction *select_all_action =
            edit_menu->addAction("Select All");

    QAction *upper_action =
            transform_menu->addAction(
                "UPPERCASE"
            );

    QAction *lower_action =
            transform_menu->addAction(
                "lowercase"
            );

    QAction *capitalize_action =
            transform_menu->addAction(
                "Capitalize"
            );

    QAction *zoom_in_action =
            view_menu->addAction(
                "Zoom In"
            );

    QAction *zoom_out_action =
            view_menu->addAction(
                "Zoom Out"
            );
    QAction *font_action =
    format_menu->addAction(
        "Font..."
    );

    QAction *check_spelling_action =
            tools_menu->addAction(
                "Check Spelling..."
            );

    QAction *find_action =
            search_menu->addAction(
                "Find"
            );

    QToolBar *toolbar =
            addToolBar("Format");

    QAction *bold_action =
            toolbar->addAction("Bold");

    QAction *italic_action =
            toolbar->addAction("Italic");

    QAction *underline_action =
            toolbar->addAction("Underline");

    QAction *word_frequency_action =
    tools_menu->addAction(
        "Word Frequency"
    );

    open_action->setShortcut(
        QKeySequence::Open
    );

    save_action->setShortcut(
        QKeySequence::Save
    );

    save_as_action->setShortcut(
        QKeySequence::SaveAs
    );

    undo_action->setShortcut(
        QKeySequence::Undo
    );

    redo_action->setShortcut(
        QKeySequence::Redo
    );

    cut_action->setShortcut(
        QKeySequence::Cut
    );

    copy_action->setShortcut(
        QKeySequence::Copy
    );

    paste_action->setShortcut(
        QKeySequence::Paste
    );

    select_all_action->setShortcut(
        QKeySequence::SelectAll
    );

    find_action->setShortcut(
        QKeySequence::Find
    );

    zoom_in_action->setShortcut(
        QKeySequence("Ctrl++")
    );

    zoom_out_action->setShortcut(
        QKeySequence("Ctrl+-")
    );

    bold_action->setCheckable(true);

    italic_action->setCheckable(true);

    underline_action->setCheckable(true);

    bold_action->setShortcut(
        QKeySequence("Ctrl+B")
    );

    italic_action->setShortcut(
        QKeySequence("Ctrl+I")
    );

    underline_action->setShortcut(
        QKeySequence("Ctrl+U")
    );

    connect(
        open_action,
        &QAction::triggered,
        this,
        [this]() {
            QString file_name =
                    QFileDialog::getOpenFileName(
                        this,
                        "Open File"
                    );

            if (file_name.isEmpty()) {
                return;
            }

            QFile file(file_name);

            if (
                !file.open(
                    QIODevice::ReadOnly
                )
            ) {
                QMessageBox::critical(
                    this,
                    "Error",
                    "Cannot open file"
                );

                return;
            }

            QTextStream in(&file);

            editor->setText(
                in.readAll()
            );

            file.close();
        }
    );

    connect(
        save_action,
        &QAction::triggered,
        this,
        [this]() {
            QString file_name =
                    QFileDialog::getSaveFileName(
                        this,
                        "Save File"
                    );

            if (file_name.isEmpty()) {
                return;
            }

            QFile file(file_name);

            if (
                !file.open(
                    QIODevice::WriteOnly
                )
            ) {
                QMessageBox::critical(
                    this,
                    "Error",
                    "Cannot save file"
                );

                return;
            }

            QTextStream out(&file);

            out << editor->toPlainText();

            file.close();
        }
    );

    connect(
        save_as_action,
        &QAction::triggered,
        this,
        [this]() {
            QString file_name =
                    QFileDialog::getSaveFileName(
                        this,
                        "Save As"
                    );

            if (file_name.isEmpty()) {
                return;
            }

            QFile file(file_name);

            if (
                !file.open(
                    QIODevice::WriteOnly
                )
            ) {
                QMessageBox::critical(
                    this,
                    "Error",
                    "Cannot save file"
                );

                return;
            }

            QTextStream out(&file);

            out << editor->toPlainText();

            file.close();
        }
    );

    connect(
        upper_action,
        &QAction::triggered,
        this,
        [this]() {
            editor->setText(
                text_transform::to_upper(
                    editor->toPlainText()
                )
            );
        }
    );

    connect(
        lower_action,
        &QAction::triggered,
        this,
        [this]() {
            editor->setText(
                text_transform::to_lower(
                    editor->toPlainText()
                )
            );
        }
    );

    connect(
        capitalize_action,
        &QAction::triggered,
        this,
        [this]() {
            editor->setText(
                text_transform::capitalize(
                    editor->toPlainText()
                )
            );
        }
    );

    connect(
        undo_action,
        &QAction::triggered,
        editor,
        &QTextEdit::undo
    );

    connect(
        redo_action,
        &QAction::triggered,
        editor,
        &QTextEdit::redo
    );

    connect(
        cut_action,
        &QAction::triggered,
        editor,
        &QTextEdit::cut
    );

    connect(
        copy_action,
        &QAction::triggered,
        editor,
        &QTextEdit::copy
    );

    connect(
        paste_action,
        &QAction::triggered,
        editor,
        &QTextEdit::paste
    );

    connect(
        select_all_action,
        &QAction::triggered,
        editor,
        &QTextEdit::selectAll
    );

    connect(
        zoom_in_action,
        &QAction::triggered,
        this,
        [this]() {
            editor->zoomIn(1);
        }
    );

    connect(
        zoom_out_action,
        &QAction::triggered,
        this,
        [this]() {
            editor->zoomOut(1);
        }
    );
    connect(
    editor,
    &QTextEdit::cursorPositionChanged,
    this,
    [this]()
    {
        update_status();
    }
);
    connect(
        check_spelling_action,
        &QAction::triggered,
        this,
        [this]() {
            highlighter->rehighlight();
        }
    );

    connect(
        bold_action,
        &QAction::triggered,
        this,
        [this](bool checked) {
            QTextCharFormat format;

            format.setFontWeight(
                checked
                    ? QFont::Bold
                    : QFont::Normal
            );

            editor->mergeCurrentCharFormat(
                format
            );
        }
    );

    connect(
        italic_action,
        &QAction::triggered,
        this,
        [this](bool checked) {
            QTextCharFormat format;

            format.setFontItalic(
                checked
            );

            editor->mergeCurrentCharFormat(
                format
            );
        }
    );

    connect(
        underline_action,
        &QAction::triggered,
        this,
        [this](bool checked) {
            QTextCharFormat format;

            format.setFontUnderline(
                checked
            );

            editor->mergeCurrentCharFormat(
                format
            );
        }
    );

    connect(
        find_action,
        &QAction::triggered,
        this,
        [this]() {
            show_find_dialog();
        }
    );
    connect(
    word_frequency_action,
    &QAction::triggered,
    this,
    [this]()
    {
        show_word_frequency();
    }
);

    connect(
        font_action,
        &QAction::triggered,
        this,
        [this](bool)
        {
            bool ok = false;

            QFont font =
                QFontDialog::getFont(
                    &ok,
                    editor->font(),
                    this
                );

            if (ok)
            {
                editor->setFont(font);
            }
        }
    );
}

void main_window::show_find_dialog() {
    find_dialog =
            new QDialog(this);

    find_dialog->setWindowTitle(
        "Find"
    );

    auto *layout =
            new QVBoxLayout(
                find_dialog
            );

    find_input =
            new QLineEdit(
                find_dialog
            );

    replace_input =
            new QLineEdit(
                find_dialog
            );

    auto *find_button =
            new QPushButton(
                "Find Next",
                find_dialog
            );

    auto *replace_button =
            new QPushButton(
                "Replace",
                find_dialog
            );

    auto *replace_all_button =
            new QPushButton(
                "Replace All",
                find_dialog
            );

    layout->addWidget(
        find_input
    );

    layout->addWidget(
        replace_input
    );

    layout->addWidget(
        find_button
    );

    layout->addWidget(
        replace_button
    );

    layout->addWidget(
        replace_all_button
    );

    connect(
        find_button,
        &QPushButton::clicked,
        this,
        [this]() {
            find_next();
        }
    );

    connect(
        replace_button,
        &QPushButton::clicked,
        this,
        [this]() {
            replace_text();
        }
    );

    connect(
        replace_all_button,
        &QPushButton::clicked,
        this,
        [this]() {
            replace_all();
        }
    );

    find_dialog->show();
}

void main_window::find_next() {
    QString text =
            find_input->text();

    if (text.isEmpty()) {
        return;
    }

    bool found =
            editor->find(text);

    if (!found) {
        QTextCursor cursor =
                editor->textCursor();

        cursor.movePosition(
            QTextCursor::Start
        );

        editor->setTextCursor(
            cursor
        );

        editor->find(text);
    }
}

void main_window::replace_text() {
    QTextCursor cursor =
            editor->textCursor();

    if (cursor.hasSelection()) {
        cursor.insertText(
            replace_input->text()
        );
    }

    find_next();
}

void main_window::replace_all() {
    QString find_text =
            find_input->text();

    QString replace_text_value =
            replace_input->text();

    if (find_text.isEmpty()) {
        return;
    }

    QTextCursor cursor =
            editor->textCursor();

    cursor.beginEditBlock();

    editor->moveCursor(
        QTextCursor::Start
    );

    while (
        editor->find(find_text)
    ) {
        QTextCursor current =
                editor->textCursor();

        current.insertText(
            replace_text_value
        );
    }

    cursor.endEditBlock();
}
void main_window::show_word_frequency()
{
    QString text =
        editor->toPlainText().toLower();

    std::stringstream stream(
        text.toStdString()
    );

    std::map<std::string, int> freq;

    std::string word;

    while (stream >> word)
    {
        ++freq[word];
    }

    auto* dialog =
        new word_frequency_dialog(
            freq,
            this
        );

    dialog->exec();
}

void main_window::update_status()
{
    QTextCursor cursor =
        editor->textCursor();

    int line =
        cursor.blockNumber() + 1;

    int column =
        cursor.columnNumber() + 1;

    statusBar()->showMessage(
        QString(
            "Line: %1   Column: %2"
        )
        .arg(line)
        .arg(column)
    );
}