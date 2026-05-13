#include "main_window.h"
#include "text_transform.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

main_window::main_window()
{
    setWindowTitle("Notepad");
    resize(800, 600);

    editor = new QTextEdit(this);
    setCentralWidget(editor);

    QMenu *file_menu =
        menuBar()->addMenu("File");
    QMenu *transform_menu =
        menuBar()->addMenu("Transform");
    QMenu *edit_menu =
    menuBar()->addMenu("Edit");

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


    connect(
        open_action,
        &QAction::triggered,
        this,
        [this]()
        {
            QString file_name =
                QFileDialog::getOpenFileName(
                    this,
                    "Open File"
                );

            if (file_name.isEmpty())
            {
                return;
            }

            QFile file(file_name);

            if (!file.open(QIODevice::ReadOnly))
            {
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
        save_action,
        &QAction::triggered,
        this,
        [this]()
        {
            QString file_name =
                QFileDialog::getSaveFileName(
                    this,
                    "Save File"
                );

            if (file_name.isEmpty())
            {
                return;
            }

            QFile file(file_name);

            if (!file.open(QIODevice::WriteOnly))
            {
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
        [this]()
        {
            QString file_name =
                QFileDialog::getSaveFileName(
                    this,
                    "Save As"
                );

            if (file_name.isEmpty())
            {
                return;
            }

            QFile file(file_name);

            if (!file.open(QIODevice::WriteOnly))
            {
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
        [this]()
        {
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
        [this]()
        {
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
        [this]()
        {
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
}