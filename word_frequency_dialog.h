#pragma once

#include <QDialog>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui {
    class word_frequency_dialog;
}
QT_END_NAMESPACE

class word_frequency_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit word_frequency_dialog(
        const std::map<std::string, int>& freq,
        QWidget *parent = nullptr
    );

    ~word_frequency_dialog();

private:
    Ui::word_frequency_dialog *ui;
};