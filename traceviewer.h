#ifndef TRACEVIEWER_H
#define TRACEVIEWER_H

extern "C"
{
#define this self
#include <mkd64/image.h>
#include <mkd64/block.h>
#include <mkd64/diskfile.h>
#undef this
}

#include "blocksdisplay.h"

#include <QApplication>

class TraceViewer
{
private:
    QApplication app;
    BlocksDisplay display;
    int speed;
    Image *img;

public:
    TraceViewer();
    void initImage(Image *img);
    int globalOption(char opt, const char *arg);
    int fileOption(DiskFile *file, char opt, const char *arg);
    void statusChanged(const BlockPosition *pos);
    void imageComplete();
};

#endif // TRACEVIEWER_H
