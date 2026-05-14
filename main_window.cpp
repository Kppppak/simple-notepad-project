#include "main_window.h"
#include "text_transform.h"
#include "spell_checker.h"
#include "spell_checker_highlighter.h"

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

main_window::main_window(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Notepad");
    resize(800, 600);

    editor = new QTextEdit(this);
    setCentralWidget(editor);
    editor->setContextMenuPolicy(
        Qt::CustomContextMenu
    );

    connect(
        editor,
        &QTextEdit::customContextMenuRequested,
        this,
        [this](const QPoint &pos) {
            QTextCursor cursor =
                    editor->cursorForPosition(
                        pos
                    );

            cursor.select(
                QTextCursor::WordUnderCursor
            );

            QString word =
                    cursor.selectedText();

            QMenu *menu =
                    editor->createStandardContextMenu();

            if (
                !checker.is_correct(
                    word
                )
            ) {
                auto list =
                        checker.suggestions(
                            word
                        );

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
                        [cursor,suggestion]() mutable {
                            cursor.insertText(
                                suggestion
                            );
                        }
                    );
                }
            }

            menu->exec(
                editor->mapToGlobal(
                    pos
                )
            );

            delete menu;
        }
    );

    checker.load_words(
        "data/words.txt"
    );

    highlighter =
            new spell_checker_highlighter(
                editor->document(),
                checker
            );

    QMenu *file_menu =
            menuBar()->addMenu("File");
    QMenu *transform_menu =
            menuBar()->addMenu("Transform");
    QMenu *edit_menu =
            menuBar()->addMenu("Edit");
    QMenu *view_menu =
            menuBar()->addMenu("View");

    QAction *zoom_in_action =
            view_menu->addAction(
                "Zoom In"
            );

    QAction *zoom_out_action =
            view_menu->addAction(
                "Zoom Out"
            );

    zoom_in_action->setShortcut(
        QKeySequence("Ctrl++")
    );

    zoom_out_action->setShortcut(
        QKeySequence("Ctrl+-")
    );
    QMenu *tools_menu =

            menuBar()->addMenu("Tools");

    QAction *check_spelling_action =

            tools_menu->addAction(

                "Check Spelling..."

            );

    QToolBar *toolbar =
            addToolBar("Format");
    QAction *bold_action =
            toolbar->addAction("Bold");

    QAction *italic_action =
            toolbar->addAction("Italic");

    QAction *underline_action =
            toolbar->addAction("Underline");
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
    QAction *open_action =
            file_menu->addAction("Open");

    QAction *save_action =
            file_menu->addAction("Save");

    QAction *save_as_action =
            file_menu->addAction("Save As");

    open_action->setShortcut(QKeySequence::Open);

    save_action->setShortcut(QKeySequence::Save);

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

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::critical(
                    this,
                    "Error",
                    "Cannot open file"
                );

                return;
            }

            QTextStream in(&file);

            editor->setText(in.readAll());

            file.close();
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

            if (!file.open(QIODevice::WriteOnly)) {
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

            if (!file.open(QIODevice::WriteOnly)) {
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
}
