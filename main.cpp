#include <iostream>
#include "libavformat/avio.h"

int main() {
    const char* url = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4";

    AVIODirContext *ctx = NULL;
    if (avio_open_dir(&ctx, url, NULL) < 0) {
        fprintf(stderr, "Cannot open directory.\n");
        abort();
    }
    return 0;
}
