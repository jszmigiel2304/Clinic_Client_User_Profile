#include "c_moduleapplication.h"

c_moduleApplication::c_moduleApplication(int &argc, char **argv) : QApplication(argc, argv)
{

}

const QByteArray &c_moduleApplication::getProcessIdentifier() const
{
    return processIdentifier;
}

void c_moduleApplication::setProcessIdentifier(const QByteArray &newProcessIdentifier)
{
    processIdentifier = newProcessIdentifier;
}

qint32 c_moduleApplication::getThreadIdentifier() const
{
    return threadIdentifier;
}

void c_moduleApplication::setThreadIdentifier(qint32 newThreadIdentifier)
{
    threadIdentifier = newThreadIdentifier;
}
