#include <QCoreApplication>
#include <QDebug>
#include "attributeparser.h"

// Static members for singleton AttributeParser
ap::AttributeParser* ap::AttributeParser::m_singletonPtr = nullptr;
std::mutex ap::AttributeParser::m_mutex;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ap::AttributeParser* ap_ptr = ap::AttributeParser::getInstance();
    try {
        ap_ptr->GetParams();
    } catch(const std::length_error& le_err) {
        qWarning() << le_err.what();
        qWarning() << "Aborting!";

        return -1;
    }

    ap_ptr->ParseHRML();
    //ap_ptr->GetInfo();
    ap_ptr->ParseQueries();

    return 0;
}
