#ifndef METODOS_H
#define METODOS_H

#include<QList>
#include <QString>

struct dadosProcesso{
    int pid;
    QString user;
    QString status;
    int cpu;
    float pcpu;
    float pmem;
    QString command;
};

int suspender(int pid_vitima);
int continuar(int pid_vitima);
int matar(int pid_vitima);
int numCPUs();
int trocarCPU(int pid_vitima, int core);
QList<dadosProcesso> listarProcessos();

#endif // METODOS_H
