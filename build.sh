# quick and dirty build script for OSX
g++ -g remote_extract.cpp -lmordor -lssl -lz -lboost_regex-mt -lcrypto -Wl,-framework -Wl,CoreServices -lboost_system-mt -Wl,-framework -Wl,SystemConfiguration -Wl,-framework -Wl,Security -lboost_thread-mt -o remote_extract
