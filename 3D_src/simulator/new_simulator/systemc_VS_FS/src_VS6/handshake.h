#ifndef HANDSHAKE_H
#define HANDSHAKE_H

class Req : public std::string
{
public:
    bool Type;
    unsigned int Addr;
    unsigned int *DataDest;
    unsigned int Burst;
};

#endif //HANDSHAKE_H

/*#ifndef XYZ_H
#define XYZ_H

namespace xyz123{
bool read = 0;
bool write = 1;
bool ERR = 0;
bool OK = 1;
};//end  namespace

#endif
*/
