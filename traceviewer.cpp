#include "traceviewer.h"

extern "C" {
#define this self
#include <mkd64/common.h>
#include <mkd64/imodule.h>
#include <mkd64/track.h>
#undef this
}

typedef struct
{
    IModule mod;
    TraceViewer *viewer;
} TraceViewAdapter;

// dynamic module interface

static void
_delete(IModule *mod)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    delete a->viewer;
    free(a);
}

static void
initImage(IModule *mod, Image *img)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    a->viewer->initImage(img);
}

static void
statusChanged(IModule *mod, const BlockPosition *pos)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    a->viewer->statusChanged(pos);
}

extern "C" {
#include <stdlib.h>
#include <unistd.h>

// static module interface

MKD64_MODULE("qttrace")

SOEXPORT IModule *
instance()
{
    TraceViewAdapter *a = (TraceViewAdapter *)malloc(sizeof(TraceViewAdapter));
    a->viewer = new TraceViewer();
    a->mod.id = &id;
    a->mod.free = &_delete;
    a->mod.initImage = &initImage;
    a->mod.statusChanged = &statusChanged;

    return (IModule *)a;
}
}

static int fakeArgc = 1;
static char *fakeArgv[] = { "qttrace", NULL };

TraceViewer::TraceViewer() : app(fakeArgc, fakeArgv), display(NULL)
{
}

void TraceViewer::initImage(Image *img)
{
    this->img = img;

    int i = 0;
    Track *t;
    while ((t = image_track(img, ++i)))
    {
        display.addTrack(t);
    }
    display.show();
    app.processEvents();
}

void TraceViewer::statusChanged(const BlockPosition *pos)
{
    BlockStatus s = block_status(image_block(img, pos));

    if (s & BS_ALLOCATED) display.setBlock(pos->track, pos->sector, 2);
    else if (s & BS_RESERVED) display.setBlock(pos->track, pos->sector, 1);
    else display.setBlock(pos->track, pos->sector, 0);

    app.processEvents();
    usleep(200000);
}
