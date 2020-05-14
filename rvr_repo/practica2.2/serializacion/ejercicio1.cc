#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <fstream>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
      int32_t total = 80 * sizeof(char) + 2 *sizeof(int16_t);

      alloc_data(total);

      char* tmp = _data;

      memcpy(tmp, name, 80);
      tmp += 80 * sizeof(char);

      memcpy(tmp, &x, sizeof(int16_t));
      tmp += sizeof(int16_t);

      memcpy(tmp, &y, sizeof(int16_t));
      //tmp += sizeof(int16_t);
    }

    int from_bin(char * data)
    {
        char * tmp = data;

        memcpy(name, tmp, 80);
        tmp += 80* sizeof(char);

        memcpy(&x, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);

        memcpy(&y, tmp, sizeof(int16_t));
        tmp += sizeof(int16_t);
        return 0;
    }


public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
    Jugador one_r("", 0, 0);
    Jugador one_w("Player_ONE", 123, 987);

    one_w.to_bin();
    std::ofstream file;
    file.open(one_w.name);
    file.write(one_w.data(), one_w.size());
    file.close();

    std::ifstream file2;
    file2.open(one_w.name);
    char data [one_w.size()];
    file2.read(data, one_w.size());
    one_r.from_bin(data);
    printf("%s %d %d", one_r.name, one_r.x, one_r.y);
    //int fd = open(one_w.name, O_CREAT);
    //write(fd, one_w.data(), one_w.size());
    return 0;
    //Serializar y escribir one_w en un fichero
    //Leer el fichero en un buffer y "deserializar" en one_r
    //Mostrar el contenido one_r
}
