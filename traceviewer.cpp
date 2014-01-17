#include "traceviewer.h"

#include <QMessageBox>
#include <mkd64/common.h>
#include <mkd64/imodule.h>
#include <mkd64/track.h>
#include <mkd64/util.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

static int
globalOption(IModule *mod, char opt, const char *arg)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    return a->viewer->globalOption(opt, arg);
}

static int
fileOption(IModule *mod, DiskFile *file, char opt, const char *arg)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    return a->viewer->fileOption(file, opt, arg);
}

static void
statusChanged(IModule *mod, const BlockPosition *pos)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    a->viewer->statusChanged(pos);
}

static void
imageComplete(IModule *mod)
{
    TraceViewAdapter *a = (TraceViewAdapter *)mod;
    a->viewer->imageComplete();
}

// static module interface

MKD64_MODULE("qttrace")

SOEXPORT IModule *
instance()
{
    TraceViewAdapter *a = (TraceViewAdapter *)calloc(1, sizeof(TraceViewAdapter));
    a->viewer = new TraceViewer();
    a->mod.id = &id;
    a->mod.free = &_delete;
    a->mod.initImage = &initImage;
    a->mod.globalOption = &globalOption;
    a->mod.fileOption = &fileOption;
    a->mod.statusChanged = &statusChanged;
    a->mod.imageComplete = &imageComplete;

    return (IModule *)a;
}

static int fakeArgc = 1;
static char *fakeArgv[] = { "qttrace", NULL };

TraceViewer::TraceViewer() : app(fakeArgc, fakeArgv),
    display(NULL), speed(50)
{ }

void TraceViewer::initImage(Image *img)
{
    this->img = img;

    int i = 0;
    Track *t;
    while ((t = Image_track(img, ++i)))
    {
        display.addTrack(t);
    }
    display.show();
    app.processEvents();
}

int TraceViewer::globalOption(char opt, const char *arg)
{
    if (opt == 's')
    {
        if (checkArgAndWarn(opt, arg, 0, 1, id()))
        {
            int intarg;
            if (tryParseInt(arg, &intarg) && intarg > 0)
            {
                speed = intarg;
            }
            else
            {
                fprintf(stderr, "[qttrace] Warning: invalid speed vaue "
                        "`%s' ignored.", arg);
            }
        }
        return 1;
    }
    return 0;
}

int TraceViewer::fileOption(DiskFile *file, char opt, const char *arg)
{
    if (opt == 'f')
    {
        if (arg)
        {
            display.setStatusMsg(QString("Writing file: ").append(arg));
        }
        else
        {
            display.setStatusMsg("<No file>");
        }
    }
    return 0;
}

void TraceViewer::statusChanged(const BlockPosition *pos)
{
    BlockStatus s = Block_status(Image_block(img, pos));

    if (s & BS_ALLOCATED) display.setBlock(pos->track, pos->sector, 2);
    else if (s & BS_RESERVED) display.setBlock(pos->track, pos->sector, 1);
    else display.setBlock(pos->track, pos->sector, 0);

    app.processEvents();
    usleep(speed*1000);
}

void TraceViewer::imageComplete()
{
    QMessageBox done(QMessageBox::Information, "Image finished",
                     "Image creation finished.", QMessageBox::Ok, &display);
    done.exec();
}
