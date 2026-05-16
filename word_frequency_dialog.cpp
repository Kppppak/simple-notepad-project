#include "word_frequency_dialog.h"
#include "ui_word_frequency_dialog.h"

#include <QTableWidgetItem>
#include <QHeaderView>

word_frequency_dialog::word_frequency_dialog(
    const std::map<std::string, int>& freq,
    QWidget *parent
)
    : QDialog(parent),
      ui(new Ui::word_frequency_dialog)
{
    ui->setupUi(this);

    ui->frequency_table->setRowCount(
        static_cast<int>(freq.size())
    );

    int row = 0;

    for (const auto& [word, count] : freq)
    {
        ui->frequency_table->setItem(
            row,
            0,
            new QTableWidgetItem(
                QString::fromStdString(word)
            )
        );

        ui->frequency_table->setItem(
            row,
            1,
            new QTableWidgetItem(
                QString::number(count)
            )
        );

        ++row;
    }

    ui->frequency_table
        ->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch
        );
}

word_frequency_dialog::~word_frequency_dialog()
{
    delete ui;
}