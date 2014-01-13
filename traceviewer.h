#ifndef TRACEVIEWER_H
#define TRACEVIEWER_H

extern "C"
{
#define this self
#include <mkd64/image.h>
#include <mkd64/block.h>
#undef this
}

#include "blocksdisplay.h"

#include <QApplication>

class TraceViewer
{
private:
    QApplication app;
    BlocksDisplay display;
    Image *img;

public:
    TraceViewer();
    void initImage(Image *img);
    void statusChanged(const BlockPosition *pos);
};

#endif // TRACEVIEWER_H
