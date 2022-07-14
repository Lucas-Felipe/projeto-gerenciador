#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTimer>
#include <vector>
#include "funcoes.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    QTimer *tempoTabela;
    QTimer *tempoGrafico;
    QList<dadosProcesso> lista;

    void atualizarTabela();
    void atualizarGrafico();
    void limparTabela();
    void keyPressEvent(QKeyEvent *event);

private slots:

    void on_tableWidget_cellClicked(int row, int);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_6_clicked();
    void on_lineEdit_3_textChanged(const QString);

    void tempoAtualizarTabela();
    void tempoAtualizarGrafico();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
