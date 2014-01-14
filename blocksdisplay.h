#ifndef BLOCKSDISPLAY_H
#define BLOCKSDISPLAY_H

#include <QMainWindow>
#include <QPixmap>

extern "C" {
#define this self
#include <mkd64/track.h>
#undef this
}

namespace Ui {
class BlocksDisplay;
}

class BlocksDisplay : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit BlocksDisplay(QWidget *parent = 0);
    ~BlocksDisplay();
    void addTrack(Track *t);
    void setBlock(int track, int sector, int status);
    void setStatusMsg(const QString &text);
    
private:
    int nextTrack;
    Ui::BlocksDisplay *ui;
    QPixmap *statusIcons;
};

#endif // BLOCKSDISPLAY_H
