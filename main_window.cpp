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

    // ================= LOWER =================

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
}