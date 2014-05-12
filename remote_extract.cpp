#include "mordor/predef.h"
#include <iostream>
#include <string>
#include "mordor/config.h"
#include "mordor/iomanager.h"
#include "mordor/streams/http.h"
#include "mordor/streams/transfer.h"
#include "mordor/streams/singleplex.h"
#include "mordor/streams/std.h"
#include "mordor/zip.h"

using namespace Mordor;

int main(int argc, char **argv)
{
    Config::loadFromEnvironment();
    StdoutStream stdoutStream;
    IOManager ioManager;

    if (argc != 3) {
        std::cerr << "given a url to a zipfile and the name of a file within,\n";
        std::cerr << "extract the file to stdout without downloading the entire zip.\n";
        std::cerr << "usage: " << argv[0] << " <url> <filename>" << std::endl;
        return 1;
    }
    std::string url = argv[1], filename = argv[2];

    try {
        HTTP::RequestBrokerOptions options;
        options.ioManager = &ioManager;
        HTTP::RequestBroker::ptr requestBroker = HTTP::createRequestBroker(options).first;
        boost::shared_ptr<HTTPStream> stream(new HTTPStream(url, requestBroker));

        stream->adviseRead(4096);
        stream->start();

        boost::shared_ptr<Stream> ss(new SingleplexStream(stream, SingleplexStream::READ, false));
        Zip zipfile(ss, Zip::READ);
        const ZipEntries &entries = zipfile.getAllEntries();
        ZipEntries::const_iterator ze = entries.find(filename);
        if (ze == entries.end()) {
            std::cerr << "file '" + filename + "' not found in zipfile";
            return 2;
        }

        transferStream(ze->second.stream(), stdoutStream);
    } catch (...) {
        std::cerr << boost::current_exception_diagnostic_information() << std::endl;
        return 2;
    }
    return 0;
}
