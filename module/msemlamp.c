

#include "msem.h"

void msemlamp_init(msem_t * sem,uint8_t origval)
{
    sem->se_val = origval;
}
void msemlamp_post(msemlamp_t *sem)
{
    sem->se_val = 1;
}
uint8_t msemlamp_get(msemlamp_t *sem)
{
    if(sem->se_val){
        sem->se_val = 0;
        return 1;
    }

    return 0;
}

