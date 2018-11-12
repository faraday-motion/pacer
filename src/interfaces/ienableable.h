#ifndef IENABLEABLE_H
#define IENABLEABLE_H

class IEnableable {
   public:
    virtual void setEnabled(bool enable) = 0;
    virtual bool enabled() = 0;
};

#endif
