
#ifndef IVESC_SEND_RECIEVE_H
#define IVESC_SEND_RECIEVE_H

class IVesc_send_recieve
{
  public:
    IVesc_send_recieve()
    {
    }

    virtual void send(unsigned char *data, unsigned int len) = 0;
    virtual void recieve(unsigned char *data, unsigned int len) = 0;
};

#endif
