#include "funcoes.h"
#include <signal.h> // definição dos sinais de interrupções
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // system()
#include <sched.h>
#include <QProcess>
#include <QDebug>
#include <QString>

int suspender(int pid_vitima)
{
    return kill(pid_vitima, SIGSTOP);
}

int continuar(int pid_vitima)
{
    return kill(pid_vitima, SIGCONT);
}

int matar(int pid_vitima)
{
    return kill(pid_vitima, SIGKILL);
}

int prioridade(int pid_vitima)
{

}

int numCPUs()
{
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int trocarCPU(int pid_vitima, int core)
{
    int CPUs;
    CPUs = numCPUs();

    if(core>=0 && core<CPUs)
    {
        cpu_set_t my_set;
        CPU_ZERO(&my_set);
        CPU_SET(core, &my_set);
        return (int)sched_setaffinity(pid_vitima, sizeof(cpu_set_t), &my_set);
    }

    return 1;
}

QList<dadosProcesso> listarProcessos()
{
    QList<dadosProcesso> listaProcessos;
    QProcess processo;

    QString command = "ps --no-header -e -o pid,uname,stat,psr,pcpu,pmem,cmd";

    processo.start(command);
    processo.waitForFinished(-1);

    QString output = processo.readAllStandardOutput();
    QStringList dados = output.split("\n");
    foreach (QString it, dados)
    {
        QString dado_aux = it.simplified();
        QStringList dado = dado_aux.split(" ");

        //verifica se dado possui 7 elementos
        if(dado.size() == 7)
        {
            dadosProcesso aux;

            aux.pid =     dado[0].toInt();
            aux.user =    dado[1];
            aux.status =  dado[2];
            aux.cpu =     dado[3].toInt();
            aux.pcpu =    dado[4].toFloat();
            aux.pmem =    dado[5].toFloat();
            aux.command = dado[6];

            listaProcessos.append(aux);
        }
    }
    return listaProcessos;
}
