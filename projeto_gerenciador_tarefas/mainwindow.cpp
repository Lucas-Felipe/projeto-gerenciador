#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "funcoes.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QKeyEvent>
#include <QList>
#include <QTimer>
#include <QDebug>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Gerenciador de tarefas");

    ui->lineEdit->setValidator(new QIntValidator(0, 999999, this));     //rece apenas inteiros nesse range.

    QStringList titulos;
    titulos << "PID" << "USER" << "STATUS" << "#CPU" << "%CPU"<<"%MEMORIA"<<"CMD";
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeaderLabels(titulos);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnWidth(6, 200);

    int numcpu = numCPUs();

    atualizarTabela();

    //Temporizador de atualização da tabela e gráfico

    tempoTabela = new QTimer(this);
    //tempoGrafico = new QTimer(this);
    tempoTabela->start(1000);
    //tempoGrafico->start(1000);

    connect(tempoTabela, SIGNAL(timeout()), this, SLOT(tempoAtualizarTabela()));
    //connect(tempoGrafico, SIGNAL(timeout()), this, SLOT(tempoAtualizarGrafico()));*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::atualizarTabela()
{
    limparTabela();
    lista.clear();

    //recebe o filtro
    QString filtro = ui->lineEdit_3->text();
    //Recebe a lista de processos
    lista = listarProcessos();
    int process_i = 0;

    foreach(dadosProcesso dado, lista)
    {
        if(dado.user.contains(filtro) || dado.command.contains(filtro))
        {
            // Adicionando uma nova linha
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            //Atribuicao de linha
            ui->tableWidget->setItem(process_i, 0, new QTableWidgetItem(QString::number(dado.pid)));     //PID
            ui->tableWidget->setItem(process_i, 1, new QTableWidgetItem(dado.user));                     //USER
            ui->tableWidget->setItem(process_i, 2, new QTableWidgetItem(dado.status));                   //STATUS
            ui->tableWidget->setItem(process_i, 3, new QTableWidgetItem(QString::number(dado.cpu)));     //#CPU
            ui->tableWidget->setItem(process_i, 4, new QTableWidgetItem(QString::number(dado.pcpu)));    //%CPU
            ui->tableWidget->setItem(process_i, 5, new QTableWidgetItem(QString::number(dado.pmem)));    //MEMORIA
            ui->tableWidget->setItem(process_i, 6, new QTableWidgetItem(dado.command));                  //COMMAND

            process_i++;
        }
    }

    ui->label->setText(QString::number(process_i) + "/" +QString::number(lista.count()) + " processos");
}

void MainWindow::on_tableWidget_cellClicked(int row, int)
{
    //seleciona linha
    ui->tableWidget->selectRow(row);
    //armazena PID da linha selecionada
    QTableWidgetItem* item_pid = ui->tableWidget->item(row,0);
    //limpa e insere o PID no lineEdit
    ui->lineEdit_4->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_4->insert(item_pid->data(Qt::DisplayRole).toString());
    ui->lineEdit->insert(item_pid->data(Qt::DisplayRole).toString());
    ui->lineEdit_2->insert(item_pid->data(Qt::DisplayRole).toString());

    QTableWidgetItem* item_cpu = ui->tableWidget->item(row,3);
    ui->label_3->setText(item_cpu->data(Qt::DisplayRole).toString());
}

void MainWindow::limparTabela()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void MainWindow::on_pushButton_2_clicked()
{
    suspender(ui->lineEdit->text().toInt());
    atualizarTabela();
}

void MainWindow::on_pushButton_3_clicked()
{
    continuar(ui->lineEdit->text().toInt());
    atualizarTabela();
}

void MainWindow::on_pushButton_clicked()
{
    matar(ui->lineEdit->text().toInt());
    atualizarTabela();
}

void MainWindow::on_pushButton_6_clicked()
{
    //trocarCPU(ui->lineEdit_pid->text().toInt(), ui->comboBox_cpu->currentIndex());
    atualizarTabela();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        qApp->quit();
}

void MainWindow::on_lineEdit_3_textChanged(const QString)
{
    atualizarTabela();
}

void MainWindow::tempoAtualizarTabela()
{
    atualizarTabela();
}

/*============================ GRÁFICO =========================*/

/*void MainWindow::tempoAtualizarGrafico()
{
    atualizarGrafico();
}

void MainWindow::atualizarGrafico()
{
    float sum_0 = 0.0;
    float sum_1 = 0.0;
    float sum_2 = 0.0;
    float sum_3 = 0.0;

    int cpu = numCPUs();

    switch (cpu) {
    case 1:
        //desabilita os outros cores
        ui->groupBox_1->setHidden(true);
        ui->groupBox_2->setHidden(true);
        ui->groupBox_3->setHidden(true);

        foreach (dadosProcesso dado, lista) {

            if(dado.cpu==0)
            {
                sum_0 += dado.pcpu;
            }
        }

        ui->progressBar_0->setValue(sum_0);

        break;
    case 2:

        ui->groupBox_2->setHidden(true);
        ui->groupBox_3->setHidden(true);

        foreach (dadosProcesso dado, lista) {

            if(dado.cpu==0)
            {
                sum_0 += dado.pcpu;
            }
            else if(dado.cpu==1){
                sum_1 += dado.pcpu;
            }
        }

        ui->progressBar_0->setValue(sum_0);
        ui->progressBar_1->setValue(sum_1);

        break;
    case 3:

        ui->groupBox_3->setHidden(true);

        foreach (dadosProcesso dado, lista) {

            if(dado.cpu==0)
            {
                sum_0 += dado.pcpu;
            }
            else if(dado.cpu==1){
                sum_1 += dado.pcpu;
            }
            else if(dado.cpu==2)
            {
                sum_2 += dado.pcpu;
            }
        }

        ui->progressBar_0->setValue(sum_0);
        ui->progressBar_1->setValue(sum_1);
        ui->progressBar_2->setValue(sum_2);

        break;
    case 4:
        foreach (dadosProcesso dado, lista) {

            if(dado.cpu==0)
            {
                sum_0 += dado.pcpu;
            }
            else if(dado.cpu==1){
                sum_1 += dado.pcpu;
            }
            else if(dado.cpu==2)
            {
                sum_2 += dado.pcpu;
            }
            else if(dado.cpu==3)
            {
                sum_3 += dado.pcpu;
            }
        }

        ui->progressBar_0->setValue(sum_0);
        ui->progressBar_1->setValue(sum_1);
        ui->progressBar_2->setValue(sum_2);
        ui->progressBar_3->setValue(sum_3);

        break;
    default:
        break;
    }
}
*/
